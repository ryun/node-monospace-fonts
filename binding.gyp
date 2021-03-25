{
  'targets': [
    {
      'target_name': 'node-monospace-fonts-native',
      'sources': ['src/get_monospace_fonts.cc'],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': ['-fno-exceptions'],
      'cflags_cc!': ['-fno-exceptions'],
      'xcode_settings': {
        'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
        'CLANG_CXX_LIBRARY': 'libc++',
        'MACOSX_DEPLOYMENT_TARGET': '10.7'
      },
      'msvs_settings': {
        'VCCLCompilerTool': {
          'ExceptionHandling': 1
        },
      },
      "conditions": [
        ['OS=="mac"', {
          'cflags+': ['-fvisibility=hidden'],
          'xcode_settings': {
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
          },
          'sources': ['src/get_monospace_fonts_darwin.mm'],
          'link_settings': {
            'libraries': ['CoreText.framework', 'Foundation.framework']
          }
        }],
      ]
    }
  ]
}
