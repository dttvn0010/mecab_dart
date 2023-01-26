//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <mecab_dart/mecab_dart_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) mecab_dart_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "MecabDartPlugin");
  mecab_dart_plugin_register_with_registrar(mecab_dart_registrar);
}
