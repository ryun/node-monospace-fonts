#include <iostream>

using namespace std;

std::string EXCLUDED_FONTS[] = {
  "Apple Braille",
  "Apple Color Emoji",
  "GB18030 Bitmap"
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

bool isExcludedFontFamily(string fontFamily) {
  string *index = find(begin(EXCLUDED_FONTS), end(EXCLUDED_FONTS), fontFamily);
  return index != end(EXCLUDED_FONTS);
}


