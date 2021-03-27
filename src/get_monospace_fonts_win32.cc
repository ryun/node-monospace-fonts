#define WINVER 0x0600
#include <dwrite.h>
#include <dwrite_1.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>
#include "utils.h"

using namespace std;

// Throws a JS error when there is some exception in DirectWrite
#define HR(hr) \
  if (FAILED(hr)) throw "Font loading error";

char *utf16ToUtf8(const WCHAR *input) {
  unsigned int len = WideCharToMultiByte(
    CP_UTF8,
    0,
    input,
    -1,
    NULL,
    0,
    NULL,
    NULL
  );

  char *output = new char[len];
  WideCharToMultiByte(CP_UTF8, 0, input, -1, output, len, NULL, NULL);
  return output;
}

// Returns the index of the user's locale in the set of localized strings
unsigned int getLocaleIndex(IDWriteLocalizedStrings *strings) {
  unsigned int index = 0;
  BOOL exists = false;
  wchar_t localeName[LOCALE_NAME_MAX_LENGTH];

  // Get the default locale for this user
  int success = GetUserDefaultLocaleName(localeName, LOCALE_NAME_MAX_LENGTH);

  // If the default locale is returned, find
  // that locale name, otherwise use "en-us"
  if (success) {
    HR(strings->FindLocaleName(localeName, &index, &exists));
  }

  // If the above find did not find a match, retry with US English
  if (!exists) {
    HR(strings->FindLocaleName(L"en-us", &index, &exists));
  }

  if (!exists) {
    index = 0;
  }

  return index;
}

string getFontFamily(IDWriteFont *font) {
  char *fontFamily = NULL;

  IDWriteLocalizedStrings *strings = NULL;
  BOOL exists = false;
  HR(font->GetInformationalStrings(
    DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES,
    &strings,
    &exists
  ));

  if (exists) {
    unsigned int index = getLocaleIndex(strings);
    unsigned int len = 0;
    WCHAR *str = NULL;

    HR(strings->GetStringLength(index, &len));
    str = new WCHAR[len + 1];

    HR(strings->GetString(index, str, len + 1));

    // Convert to utf-8
    fontFamily = utf16ToUtf8(str);
    delete str;

    strings->Release();
  }

  if (!fontFamily) {
    fontFamily = new char[1];
    fontFamily[0] = '\0';
  }

  return string(fontFamily);
}

pair<string, bool> getFontFamilyAndMonospaceTrait(IDWriteFont *font) {
  string fontFamily;
  bool hasMonospaceTrait = false;

  IDWriteFontFace *face = NULL;
  HR(font->CreateFontFace(&face));

  // Get the font files from this font face
  unsigned int numFiles = 0;
  IDWriteFontFile *files = NULL;
  HR(face->GetFiles(&numFiles, NULL));
  HR(face->GetFiles(&numFiles, &files));

  // return the first one
  if (numFiles > 0) {
    IDWriteFontFileLoader *loader = NULL;
    HR(files[0].GetLoader(&loader));

    // Check if this is a local font file
    IDWriteLocalFontFileLoader *fileLoader = NULL;
    HRESULT hr = loader->QueryInterface(
      __uuidof(IDWriteLocalFontFileLoader),
      (void **)&fileLoader
    );

    if (SUCCEEDED(hr)) {
      // Get the file path
      const void *referenceKey = NULL;
      unsigned int referenceKeySize = 0;
      unsigned int nameLength = 0;
      HR(files[0].GetReferenceKey(&referenceKey, &referenceKeySize));
      HR(fileLoader->GetFilePathLengthFromKey(
        referenceKey,
        referenceKeySize,
        &nameLength
      ));

      WCHAR *name = new WCHAR[nameLength + 1];
      HR(fileLoader->GetFilePathFromKey(
        referenceKey,
        referenceKeySize,
        name,
        nameLength + 1
      ));

      fontFamily = getFontFamily(font);

      // This method requires windows 7, so we
      // need to cast to an IDWriteFontFace1
      IDWriteFontFace1 *face1;
      HRESULT hr = face->QueryInterface(
        __uuidof(IDWriteFontFace1),
        (void **)&face1
      );

      if (SUCCEEDED(hr)) {
        hasMonospaceTrait = face1->IsMonospacedFont() == TRUE;
      }

      delete name;
      // delete fontFamily;
      fileLoader->Release();
    }

    loader->Release();
  }

  face->Release();
  files->Release();

  return make_pair(fontFamily, hasMonospaceTrait);
}

vector<string> *getMonospaceFonts() {
  // Cache monospace fonts for fast use in future calls
  static vector<string> *fonts = NULL;
  if (fonts != NULL) {
    return fonts;
  }

  IDWriteFactory *factory = NULL;
  HR(DWriteCreateFactory(
    DWRITE_FACTORY_TYPE_SHARED,
    __uuidof(IDWriteFactory),
    reinterpret_cast<IUnknown**>(&factory)
  ));

  fonts = fonts = new vector<string>();

  // Get system font collection
  IDWriteFontCollection *collection = NULL;
  HR(factory->GetSystemFontCollection(&collection));

  int familyCount = collection->GetFontFamilyCount();
  for (int i = 0; i < familyCount; i++) {
    IDWriteFontFamily *family = NULL;
    HR(collection->GetFontFamily(i, &family));

    int fontCount = family->GetFontCount();
    for (int j = 0; j < fontCount; j++) {
      IDWriteFont *font = NULL;
      HR(family->GetFont(j, &font));

      pair<string, bool> result = getFontFamilyAndMonospaceTrait(font);
      string fontFamily = result.first;
      bool hasMonospaceTrait = result.second;

      if (isExcludedFontFamily(fontFamily, hasMonospaceTrait)) {
        continue; // TODO: try break?
      }

      vector<string>::iterator iterator = find(
        fonts->begin(),
        fonts->end(),
        fontFamily
      );

      // Add font family to the results list if it doesn't already exist
      if (iterator == fonts->end()) {
        fonts->push_back(fontFamily);
      }
    }

    family->Release();
  }

  collection->Release();
  factory->Release();

  return fonts;
}
