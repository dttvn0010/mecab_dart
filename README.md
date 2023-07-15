# mecab_dart

MeCab(Japanese Morphological Analyzer) wrapper for Flutter on iOS/Android.

## Usage

1. Add this plug_in `mecab_dart` as a [dependency in your pubspec.yaml file](https://flutter.io/platform-plugins/).
```yaml
dependencies:   
   mecab_dart: 0.1.5
```

2. Copy Mecab dictionary (ipadic) to your assets folder

3. **Windows only setup**
Create a `blobs` folder on the top level of your application and copy `libmecab.dll` from `example/blobs` there.
Lastly, open `windows/CMakeLists.txt` of your application and append at the end:

``` CMake
install(
  FILES ${PROJECT_BUILD_DIR}/../blobs/libmecab.dll 
  DESTINATION ${INSTALL_BUNDLE_DATA_DIR}/../blobs/
)
```

1. Try `example/lib/main.dart` or the following example.

### Example

Init the tagger:

```dart
var tagger = new Mecab();
await tagger.init("assets/ipadic", true);
```

Set the boolean option in `init` function to true if you want to get the tokens including features,
set it to false if you only want the token surfaces.

Use the tagger to parse text:

```dart
var tokens = tagger.parse('にわにわにわにわとりがいる。');
var text = '';

for(var token in tokens) {
  text += token.surface + "\t";
  for(var i = 0; i < token.features.length; i++) {
    text += token.features[i];
    if(i + 1 < token.features.length) {
       text += ",";
    }
  }
  text += "\n";
}
```

### Building on Windows

Because mecab uses nmake on windows to compile, the mecab DLL needs to be created separately.
For this open a [**Developer Command Prompt**](https://learn.microsoft.com/en-us/visualstudio/ide/reference/command-prompt-powershell?view=vs-2022) and change in the `windows/src` directory.
In this directory execute `nmake -f  Makefile.x64.msvc`.
After the build process finished, there should be a `libmecab.dll` in `windows/src`.
