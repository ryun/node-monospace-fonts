#include <fontconfig/fontconfig.h>
#include <algorithm>
#include <iostream>
#include <vector>
#include "constants.h"

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
    FcChar8 *file, *psName, *style, *familyVal;
    int spacing;
    FcPattern *font = fontSet->fonts[i];

    // Get font family
    FcPatternGetString(font, FC_FAMILY, 0, &familyVal);
    std::string family = std::string((char *) familyVal);
    std::cout << family << std::endl;

    FcPatternGetString(font, FC_FILE, 0, &file);
    std::cout << "file: " << (char *) file << std::endl;

    FcPatternGetString(font, FC_POSTSCRIPT_NAME, 0, &psName);
    std::cout << "postscript name: " << (char *) psName << std::endl;

    FcPatternGetString(font, FC_STYLE, 0, &style);
    std::cout << "style: " << (char *) style << std::endl;

    // Get monospace trait
    FcPatternGetInteger(font, FC_SPACING, 0, &spacing);
    bool hasMonospaceTrait = spacing == FC_MONO;
    std::cout << "hasMonospaceTrait: " << hasMonospaceTrait << std::endl;

    // Skip excluded fonts
    index = std::find(std::begin(EXCLUDED_FONTS), std::end(EXCLUDED_FONTS), family);
    if (index != std::end(EXCLUDED_FONTS)) {
      std::cout << "-> excluded" << std::endl << std::endl;
      continue;
    }

    // Skip fonts without a monospace trait that don't exist in the include fonts list
    index = std::find(std::begin(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT), std::end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT), family);
    if (!hasMonospaceTrait && index == std::end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT)) {
      std::cout << "-> not mono & not included" << std::endl << std::endl;
      continue;
    }

    // Add font family to the results list if it doesn't already exist
    iterator = std::find(fonts->begin(), fonts->end(), family);
    if (iterator == fonts->end()) {
      std::cout << "-> pushing to fonts lists" << std::endl << std::endl;
      fonts->push_back(family);
    } else {
      std::cout << "-> already exists in font list" << std::endl << std::endl;
    }
  }

  // Cleanup
  FcPatternDestroy(pattern);
  FcObjectSetDestroy(objectSet);
  FcFontSetDestroy(fontSet);

  return fonts;
}
