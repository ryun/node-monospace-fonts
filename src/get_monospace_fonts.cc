#include <napi.h>
#include <iostream>
#include <locale>
#include <vector>

using namespace Napi;
using namespace std;

// This function is implemented by each platform
vector<string> *getMonospaceFonts();

bool compareAlphabetically (string a, string b) {
  for (int i = 0; i < 3; i++) {
    a[i] = toupper(a[i]);
    b[i] = toupper(b[i]);
  }
  return a < b;
}

Napi::Array getMonospaceFontsFunc(const Napi::CallbackInfo& info) {
  Napi::Env env = info.Env();

  // Get monospace fonts from platform specific code
  vector<string> fonts = *getMonospaceFonts();
  sort(fonts.begin(), fonts.end(), compareAlphabetically);

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
