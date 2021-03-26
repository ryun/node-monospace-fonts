#define WINVER 0x0600
#include <dwrite.h>
#include <dwrite_1.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include <unordered_set>

// throws a JS error when there is some exception in DirectWrite
#define HR(hr) \
  if (FAILED(hr)) throw "Font loading error";

std::string *resultFromFont(IDWriteFont *font) {
  std::string *fontFamily = NULL;
  IDWriteFontFace *face = NULL;
  unsigned int numFiles = 0;

  HR(font->CreateFontFace(&face));

  // get the font files from this font face
  IDWriteFontFile *files = NULL;
  HR(face->GetFiles(&numFiles, NULL));
  HR(face->GetFiles(&numFiles, &files));

  // return the first one
  if (numFiles > 0) {
    IDWriteFontFileLoader *loader = NULL;
    IDWriteLocalFontFileLoader *fileLoader = NULL;
    unsigned int nameLength = 0;
    const void *referenceKey = NULL;
    unsigned int referenceKeySize = 0;
    WCHAR *name = NULL;

    HR(files[0].GetLoader(&loader));

    // check if this is a local font file
    HRESULT hr = loader->QueryInterface(__uuidof(IDWriteLocalFontFileLoader), (void **)&fileLoader);
    if (SUCCEEDED(hr)) {
      // get the file path
      HR(files[0].GetReferenceKey(&referenceKey, &referenceKeySize));
      HR(fileLoader->GetFilePathLengthFromKey(referenceKey, referenceKeySize, &nameLength));

      name = new WCHAR[nameLength + 1];
      HR(fileLoader->GetFilePathFromKey(referenceKey, referenceKeySize, name, nameLength + 1));

      char *psName = utf16ToUtf8(name);
      char *postscriptName = getString(font, DWRITE_INFORMATIONAL_STRING_POSTSCRIPT_NAME);
      char *family = getString(font, DWRITE_INFORMATIONAL_STRING_WIN32_FAMILY_NAMES);
      char *style = getString(font, DWRITE_INFORMATIONAL_STRING_WIN32_SUBFAMILY_NAMES);

      bool monospace = false;
      // this method requires windows 7, so we need to cast to an IDWriteFontFace1

      IDWriteFontFace1 *face1;
      HRESULT hr = face->QueryInterface(__uuidof(IDWriteFontFace1), (void **)&face1);
      if (SUCCEEDED(hr)) {
        monospace = face1->IsMonospacedFont() == TRUE;
      }

      // res = new FontDescriptor(
      //   psName,
      //   postscriptName,
      //   family,
      //   style,
      //   (FontWeight) font->GetWeight(),
      //   (FontWidth) font->GetStretch(),
      //   font->GetStyle() == DWRITE_FONT_STYLE_ITALIC,
      //   monospace
      // );

      fontFamily = std::string(family)

      delete psName;
      delete name;
      delete postscriptName;
      delete family;
      delete style;
      fileLoader->Release();
    }

    loader->Release();
  }

  face->Release();
  files->Release();

  return fontFamily;
}

std::vector<std::string> *getAvailableFonts() {
  std::vector<std::string> *fonts = new std::vector<std::string>();
  int count = 0;

  IDWriteFactory *factory = NULL;
  HR(DWriteCreateFactory(
    DWRITE_FACTORY_TYPE_SHARED,
    __uuidof(IDWriteFactory),
    reinterpret_cast<IUnknown**>(&factory)
  ));

  // Get the system font collection
  IDWriteFontCollection *collection = NULL;
  HR(factory->GetSystemFontCollection(&collection));

  // Get the number of font families in the collection
  int familyCount = collection->GetFontFamilyCount();

  // Track postscript names we've already added
  // using a set so we don't get any duplicates
  std::unordered_set<std::string> psNames;

  for (int i = 0; i < familyCount; i++) {
    IDWriteFontFamily *family = NULL;

    // Get the font family
    HR(collection->GetFontFamily(i, &family));
    int fontCount = family->GetFontCount();

    for (int j = 0; j < fontCount; j++) {
      IDWriteFont *font = NULL;
      HR(family->GetFont(j, &font));

      std::string fontFamily = resultFromFont(font);
      if (psNames.count(fontFamily) == 0) {
        fonts->push_back(fontFamily);
        psNames.insert(fontFamily);
      }
    }

    family->Release();
  }

  collection->Release();
  factory->Release();

  return fonts;
}
