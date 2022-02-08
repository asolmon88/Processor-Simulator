#ifndef SECTION_H_
#define SECTION_H_

#include <string>

typedef struct Section {
  std::string name;
  // this is the index where the instructions from this section starts
  int index;
} Section_t;

#endif
