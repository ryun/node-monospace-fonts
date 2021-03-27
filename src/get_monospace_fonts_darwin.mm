#include <Foundation/Foundation.h>
#include <CoreText/CoreText.h>
#include <iostream>
#include <vector>
#include "utils.h"

using namespace std;

NSArray *getSystemFontDesriptors() {
  CTFontCollectionRef collection =
    CTFontCollectionCreateFromAvailableFonts(NULL);

  return (NSArray *) CTFontCollectionCreateMatchingFontDescriptors(collection);
}

string getFontFamily(CTFontDescriptorRef descriptor) {
  NSString *family = (NSString *) CTFontDescriptorCopyAttribute(
    descriptor,
    kCTFontFamilyNameAttribute
  );

  return [family UTF8String];
}

int getMonospaceTrait(CTFontDescriptorRef descriptor) {
  NSDictionary *traits = (NSDictionary *) CTFontDescriptorCopyAttribute(
    descriptor,
    kCTFontTraitsAttribute
  );

  NSNumber *symbolicTraitsValue = traits[(id)kCTFontSymbolicTrait];
  unsigned int symbolicTraits = [symbolicTraitsValue unsignedIntValue];
  return (symbolicTraits & kCTFontMonoSpaceTrait);
}


vector<string> *getMonospaceFonts() {
  // Cache monospace fonts for fast use in future calls
  static vector<string> *fonts = NULL;
  if (fonts != NULL) {
    return fonts;
  }

  fonts = new vector<string>();
  NSArray *descriptors = getSystemFontDesriptors();

  for (id d in descriptors) {
    CTFontDescriptorRef descriptor = (CTFontDescriptorRef) d;

    string fontFamily = getFontFamily(descriptor);
    bool hasMonospaceTrait = getMonospaceTrait(descriptor) != 0;
    if (isExcludedFontFamily(fontFamily, hasMonospaceTrait)) {
      continue;
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

  [descriptors release];
  return fonts;
}
