#ifndef FLUTTER_PLUGIN_MECAB_DART_PLUGIN_H_
#define FLUTTER_PLUGIN_MECAB_DART_PLUGIN_H_

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>

#include <memory>

namespace mecab_dart {

class MecabDartPlugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  MecabDartPlugin();

  virtual ~MecabDartPlugin();

  // Disallow copy and assign.
  MecabDartPlugin(const MecabDartPlugin&) = delete;
  MecabDartPlugin& operator=(const MecabDartPlugin&) = delete;

 private:
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

}  // namespace mecab_dart

#endif  // FLUTTER_PLUGIN_MECAB_DART_PLUGIN_H_
