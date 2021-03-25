#include <Foundation/Foundation.h>
#include <CoreText/CoreText.h>
#include <iostream>
#include <vector>
#include "constants.h"

std::vector<std::string> *getMonospaceFonts() {
  // Cache monospace fonts for fast use in future calls
  static std::vector<std::string> *fonts = NULL;
  if (fonts != NULL) {
    return fonts;
  }

  fonts = new std::vector<std::string>();
  std::vector<std::string>::iterator iterator;
  std::string *index;

  // Get system font collection & font descriptors
  CTFontCollectionRef collection = CTFontCollectionCreateFromAvailableFonts(NULL);
  NSArray *descriptors = (NSArray *) CTFontCollectionCreateMatchingFontDescriptors(collection);

  for (id d in descriptors) {
    CTFontDescriptorRef descriptor = (CTFontDescriptorRef) d;

    // Get font family from descriptor
    NSString *familyVal = (NSString *) CTFontDescriptorCopyAttribute(descriptor, kCTFontFamilyNameAttribute);
    std::string family = [familyVal UTF8String];

    // Get monospace trait from descriptor
    NSDictionary *traits = (NSDictionary *) CTFontDescriptorCopyAttribute(descriptor, kCTFontTraitsAttribute);
    NSNumber *symbolicTraitsVal = traits[(id)kCTFontSymbolicTrait];
    unsigned int symbolicTraits = [symbolicTraitsVal unsignedIntValue];
    bool hasMonospaceTrait = (symbolicTraits & kCTFontMonoSpaceTrait) != 0;

    // Skip excluded fonts
    index = std::find(std::begin(EXCLUDED_FONTS), std::end(EXCLUDED_FONTS), family);
    if (index != std::end(EXCLUDED_FONTS)) {
      continue;
    }

    // Skip fonts without a monospace trait that don't exist in the include fonts list
    index = std::find(std::begin(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT), std::end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT), family);
    if (!hasMonospaceTrait && index == std::end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT)) {
      continue;
    }

    // Add font family to the results list if it doesn't already exist
    iterator = std::find(fonts->begin(), fonts->end(), family);
    if (iterator == fonts->end()) {
      fonts->push_back(family);
    }
  }

  [descriptors release];
  return fonts;
}
