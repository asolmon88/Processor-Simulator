#ifndef SECTION_H_
#define SECTION_H_

#include <string>
#include <iostream>
using namespace std;

typedef struct Section {
  std::string name;
  // this is the index where the instructions from this section start
  int index;

  friend ostream& operator<<(ostream& output, Section& section) {
    output << "NAME: " << section.name << std::endl;
    output << "INDEX: " << section.index << std::endl;
    return output;
  }

  Section& operator=(Section& other) {
    this->name = other.name;
    this->index = other.index;
    return *this;
  }

} Section_t;

#endif
