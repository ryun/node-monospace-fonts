#include <iostream>

using namespace std;

std::string EXCLUDED_FONTS[] = {
  "Apple Braille",     // darwin
  "Apple Color Emoji", // darwin
  "DotumChe",          // win32
  "GB18030 Bitmap",    // linux
  "GulimChe",          // win32
  "MingLiU",           // win32
  "MS Gothic",         // win32
  "NSimSun",           // win32
  "SimSun-ExtB"        // win32
};

std::string INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT[] = {
  "AverageMono",
  "Fira Code iScript",
  "Generic Mono II",
  "Lekton",
  "M+ 2m",
  "Metrickal",
  "Roboto Mono",
  "SF Mono",
  "Sudo",
  "SV Basic Manual",
  "Whois",
  "Fixedsys Excelsior 3.01",
  "FreeMono",
  "Inconsolata LGC",
  "Linux Libertine Mono",
  "M+ 1m",
  "M+ 1mn",
  "monoOne",
  "Monospace",
  "MonospaceTypewriter",
  "MonoSpatial",
  "Nimbus Mono",
  "NotCourierSans",
  "Terminus"
};

bool isExcludedFontFamily(string fontFamily, bool hasMonospaceTrait) {
  string *index;

  index = find(begin(EXCLUDED_FONTS), end(EXCLUDED_FONTS), fontFamily);
  if (index != end(EXCLUDED_FONTS)) {
    return true;
  }

  index = find(
    begin(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT),
    end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT),
    fontFamily
  );

  return !hasMonospaceTrait &&
    index == end(INCLUDED_FONTS_MISSING_MONOSPACE_TRAIT);
}


