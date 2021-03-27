#include <fontconfig/fontconfig.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "utils.h"

using namespace std;

string getFontFamily(FcPattern *font) {
  FcChar8 *family;
  FcPatternGetString(font, FC_FAMILY, 0, &family);
  return string((char *) family);
}

int getMonospaceTrait(FcPattern *font) {
  int spacing = 0;
  FcPatternGetInteger(font, FC_SPACING, 0, &spacing);
  return spacing;
}

vector<string> *getMonospaceFonts() {
  // Cache monospace fonts for fast use in future calls
  static vector<string> *fonts = NULL;
  if (fonts != NULL) {
    return fonts;
  }

  FcInit();

  // Get system font set
  FcPattern *pattern = FcPatternCreate();
  FcObjectSet *objectSet = FcObjectSetBuild(FC_FAMILY, FC_SPACING, NULL);
  FcFontSet *fontSet = FcFontList(NULL, pattern, objectSet);
  if (!fontSet) {
    return new vector<string>();
  }

  fonts = new vector<string>();

  for (int i = 0; i < fontSet->nfont; i++) {
    FcPattern *font = fontSet->fonts[i];

    string fontFamily = getFontFamily(font);
    bool hasMonospaceTrait = getMonospaceTrait(font) == FC_MONO;
    if (isExcludedFontFamily(fontFamily, hasMonospaceTrait)) {
      continue;
    }

    vector<string>::iterator iterator= find(
      fonts->begin(),
      fonts->end(),
      fontFamily
    );

    // Add font family to the results list if it doesn't already exist
    if (iterator == fonts->end()) {
      fonts->push_back(fontFamily);
    }
  }

  FcPatternDestroy(pattern);
  FcObjectSetDestroy(objectSet);
  FcFontSetDestroy(fontSet);

  return fonts;
}
