#include <fontconfig/fontconfig.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "utils.h"

std::vector<std::string> *getMonospaceFonts() {
  // Cache monospace fonts for fast use in future calls
  static std::vector<std::string> *fonts = NULL;
  if (fonts != NULL) {
    return fonts;
  }

  FcInit();

  // Get system font set
  FcPattern *pattern = FcPatternCreate();
  FcObjectSet *objectSet = FcObjectSetBuild(FC_FILE, FC_POSTSCRIPT_NAME, FC_FAMILY, FC_STYLE, FC_SPACING, NULL);
  FcFontSet *fontSet = FcFontList(NULL, pattern, objectSet);

  if (!fontSet) {
    return new std::vector<std::string>();
  }

  fonts = new std::vector<std::string>();
  std::vector<std::string>::iterator iterator;
  std::string *index;


  for (int i = 0; i < fontSet->nfont; i++) {
    FcChar8 *familyVal;
    int spacing = 0;
    FcPattern *font = fontSet->fonts[i];

    // Get font family
    FcPatternGetString(font, FC_FAMILY, 0, &familyVal);
    std::string fontFamily = std::string((char *) familyVal);

    // Get monospace trait
    FcPatternGetInteger(font, FC_SPACING, 0, &spacing);
    bool hasMonospaceTrait = spacing == FC_MONO;

    if (isExcludedFontFamily(fontFamily)) {
      continue;
    }

    // Skip fonts without a monospace trait that don't exist in the include fonts list
    index = std::find(std::begin(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT), std::end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT), fontFamily);
    if (!hasMonospaceTrait && index == std::end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT)) {
      continue;
    }

    // Add font family to the results list if it doesn't already exist
    iterator = std::find(fonts->begin(), fonts->end(), fontFamily);
    if (iterator == fonts->end()) {
      fonts->push_back(fontFamily);
    }
  }

  // Cleanup
  FcPatternDestroy(pattern);
  FcObjectSetDestroy(objectSet);
  FcFontSetDestroy(fontSet);

  return fonts;
}
