#ifndef REGISTER_
#define REGISTER_

typedef struct Register {
  int value;
  int startIndex;
  int endIndex;

  Register(int start, int end) {
    this->startIndex = start;
    this->endIndex = end;
  }

  void setValue(int value) {
    this->value = value;
  }

} Register_t; 

#endif  // Register