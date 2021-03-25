#include <napi.h>
#include <iostream>
#include <locale>
#include <vector>

using namespace Napi;

// This function is implemented by each platform
std::vector<std::string> *getMonospaceFonts();

bool compareAlphabetically (std::string a, std::string b) {
  if (islower(a[0])) {
    a[0] = toupper(a[0]);
  }
  if (islower(b[0])) {
    b[0] = toupper(b[0]);
  }
  return a < b;
}

Napi::Array getMonospaceFontsFunc(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // Get monospace fonts from platform specific code
  std::vector<std::string> fonts = *getMonospaceFonts();
  std::sort(fonts.begin(), fonts.end(), compareAlphabetically);

  // Convert fonts to a Napi::Array of Napi::String for interop with JS
  Napi::Array results = Napi::Array::New(env);
  for (unsigned int i = 0; i < fonts.size(); i++) {
    results.Set(uint32_t(i), Napi::String::New(env, fonts.at(i)));
  }
  return results;
}

Napi::Object Init(Napi::Env env, Napi::Object exports) {
  exports.Set(Napi::String::New(env, "getMonospaceFonts"),
              Napi::Function::New(env, getMonospaceFontsFunc));
  return exports;
}

NODE_API_MODULE(addon, Init)
