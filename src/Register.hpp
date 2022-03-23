#ifndef REGISTER_
#define REGISTER_

#include <iostream>

typedef struct Register {
  int value;
  int startIndex;
  int endIndex;

  Register(int start, int end) {
    this->startIndex = start;
    this->endIndex = end;
    this->value = 0;
  }

  void setValue(int value) {
    this->value = value;
  }

  int operator+(Register& other) {
    return this->value + other.value;
  }
  int operator-(Register& other) {
    return this->value - other.value;
  }
  int operator*(Register& other) {
    return this->value * other.value;
  }

  void operator=(const Register& other) {
    this->value = other.value;
    this->startIndex = other.startIndex;
    this->endIndex = other.endIndex;
  }

  friend std::ostream& operator<<(std::ostream& output, Register& reg) {
    output << "start: " << reg.startIndex << std::endl;
    output << "end: " << reg.endIndex << std::endl;
    output << "Value: " << reg.value << std::endl;
    return output;
  }

} Register_t; 

#endif  // Register