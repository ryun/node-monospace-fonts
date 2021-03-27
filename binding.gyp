{
  'targets': [
    {
      'target_name': 'node-monospace-fonts-native',
      'sources': ['src/get_monospace_fonts.cc'],
      'include_dirs': ["<!@(node -p \"require('node-addon-api').include\")"],
      'dependencies': ["<!(node -p \"require('node-addon-api').gyp\")"],
      'cflags!': ['-fno-exceptions'],
      'cflags_cc!': ['-fno-exceptions'],
      'msvs_settings': {
        'VCCLCompilerTool': {
          'ExceptionHandling': 1
        },
      },
      "conditions": [
        ['OS=="mac"', {
          'sources': ['src/get_monospace_fonts_darwin.mm'],
          'cflags+': ['-fvisibility=hidden'],
          'link_settings': {
            'libraries': ['CoreText.framework', 'Foundation.framework']
          },
          'xcode_settings': {
            'CLANG_CXX_LIBRARY': 'libc++',
            'GCC_ENABLE_CPP_EXCEPTIONS': 'YES',
            'GCC_SYMBOLS_PRIVATE_EXTERN': 'YES', # -fvisibility=hidden
            'MACOSX_DEPLOYMENT_TARGET': '10.7'
          }
        }],
        ['OS=="win"', {
          'sources': ['src/get_monospace_fonts_win32.cc'],
          'link_settings': {
            'libraries': ['Dwrite.lib']
          }
        }],
        ['OS=="linux"', {
          'sources': ['src/get_monospace_fonts_linux.cc'],
          'link_settings': {
            'libraries': ['-lfontconfig']
          }
        }]
      ]
    }
  ]
}
