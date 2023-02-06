#include "include/mecab_dart/mecab_dart_plugin_c_api.h"

#include <flutter/plugin_registrar_windows.h>

#include "mecab_dart_plugin.h"

void MecabDartPluginCApiRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  mecab_dart::MecabDartPlugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
