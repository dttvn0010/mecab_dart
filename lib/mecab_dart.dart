import 'dart:core';
import 'package:flutter/services.dart';

import 'dart:ffi';
import 'dart:io';

import 'package:path/path.dart';
import 'package:ffi/ffi.dart';
import 'package:path_provider/path_provider.dart';

typedef initMecabFunc = Pointer<Void> Function(
    Pointer<Utf8> options, Pointer<Utf8> dicdir);
typedef parseFunc = Pointer<Utf8> Function(
    Pointer<Void> m, Pointer<Utf8> input);
typedef destroyMecabFunc = Void Function(Pointer<Void> mecab);
typedef destroyMecab_func = void Function(Pointer<Void> mecab);

final DynamicLibrary mecabDartLib = () {
  if(Platform.isAndroid)
    return DynamicLibrary.open("libmecab_dart.so");
  else if(Platform.isWindows)
    return DynamicLibrary.open(
      "${Directory(Platform.resolvedExecutable).parent.path}/blobs/libmecab.dll"
    );
  else
    return DynamicLibrary.process();
} ();

final initMecabPointer =
    mecabDartLib.lookup<NativeFunction<initMecabFunc>>('initMecab');
final initMecabFfi = initMecabPointer.asFunction<initMecabFunc>();

final parsePointer = mecabDartLib.lookup<NativeFunction<parseFunc>>('parse');
final parseFfi = parsePointer.asFunction<parseFunc>();

final destroyMecabPointer =
    mecabDartLib.lookup<NativeFunction<destroyMecabFunc>>('destroyMecab');
final destroyMecabFfi = destroyMecabPointer.asFunction<destroyMecab_func>();

final int Function(int x, int y) nativeAdd = mecabDartLib
    .lookup<NativeFunction<Int32 Function(Int32, Int32)>>("native_add")
    .asFunction();


/// Class that represent one token from mecab's output.
class TokenNode {
  /// The surface form of the token (how it appears in the text)
  String surface = "";
  /// A list of features of this token (varies depending on the dictionar you
  /// are using)
  List<String> features = [];

  TokenNode(String item) {
    var arr = item.split('\t');
    if (arr.length > 0) {
      surface = arr[0];
    }
    if (arr.length == 2) {
      features = arr[1].split(',');
    } else {
      features = [];
    }
  }
}

/// Class that represents a Mecab instance
class Mecab {
  /// Pointer to the Mecab instance on the C side
  Pointer<Void>? mecabPtr;

  /// Copies `assetDicDir/fileName` to `dicdir/fileName` if it does not already
  /// exist
  Future<void> copyFile(String dicdir, String assetDicDir, String fileName) async 
  {
    if (FileSystemEntity.typeSync('$dicdir/$fileName') ==
        FileSystemEntityType.notFound) {
      var data = (await rootBundle.load('$assetDicDir/$fileName'));
      var buffer = data.buffer;
      var bytes = buffer.asUint8List(data.offsetInBytes, data.lengthInBytes);
      new File('$dicdir/$fileName').writeAsBytesSync(bytes);
    }
  }

  /// Initializes this mecab instance, this method needs to be called before
  /// any other method.
  /// `assetDicDir` is the directory of the dictionary (ex. IpaDic) from where
  /// it should be loaded. If `includeFeatures` is set, the output of mecab
  /// includes the token-features. If `dicDir` is null the dictionary is copied
  /// to a folder called like the folder in the assets directory. This new 
  /// folder is located inside the platforms documents directory. Otherwise,
  /// it is copied to `dicDir`.
  Future<void> init(
    String assetDicDir, bool includeFeatures, {String? dicDir}) async
  {
    if(dicDir == null){
      var dir = (await getApplicationDocumentsDirectory()).path;
      var dictName = basename(assetDicDir);
      dicDir = "$dir/$dictName";
    }
    var mecabrc = '$dicDir/mecabrc';

    if (FileSystemEntity.typeSync(mecabrc) == FileSystemEntityType.notFound) {
      // Create new mecabrc file
      var mecabrcFile = await (new File(mecabrc).create(recursive: true));
      mecabrcFile.writeAsStringSync("");
    }

    // Copy dictionary from asset folder to App Document folder
    await copyFile(dicDir, assetDicDir, 'char.bin');
    await copyFile(dicDir, assetDicDir, 'dicrc');
    await copyFile(dicDir, assetDicDir, 'left-id.def');
    await copyFile(dicDir, assetDicDir, 'matrix.bin');
    await copyFile(dicDir, assetDicDir, 'pos-id.def');
    await copyFile(dicDir, assetDicDir, 'rewrite.def');
    await copyFile(dicDir, assetDicDir, 'right-id.def');
    await copyFile(dicDir, assetDicDir, 'sys.dic');
    await copyFile(dicDir, assetDicDir, 'unk.dic');

    initWithIpadicDir(dicDir, includeFeatures);
  }

  /// Init this instance with ipadic without copying it
  void initWithIpadicDir(String dicdir, bool includeFeatures) async {
    var mecabrc = '$dicdir/mecabrc';

    if (FileSystemEntity.typeSync(mecabrc) == FileSystemEntityType.notFound) {
      // Create new mecabrc file
      var mecabrcFile = await (new File(mecabrc).create(recursive: true));
      mecabrcFile.writeAsStringSync("");
    }

    var options = includeFeatures ? "" : "-Owakati";
    mecabPtr = initMecabFfi(options.toNativeUtf8(), dicdir.toNativeUtf8());
  }

  /// Parses the given text using mecab and returns mecab's output
  List<TokenNode> parse(String input) {
    if (mecabPtr != null) {
      var resultStr =
          (parseFfi(mecabPtr!, input.toNativeUtf8())).toDartString().trim();

      var items;
      if (resultStr.contains('\n')) {
        items = resultStr.split('\n');
      } else {
        items = resultStr.split(' ');
      }

      List<TokenNode> tokens = [];
      for (var item in items) {
        tokens.add(TokenNode(item));
      }
      return tokens;
    }
    return [];
  }

  /// Frees the memory used by mecab and 
  void destroy() {
    if (mecabPtr != null) {
      destroyMecabFfi(mecabPtr!);
    }
  }

  static const MethodChannel _channel = const MethodChannel('mecab_dart');

  static Future<String> get platformVersion async {
    final String version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
