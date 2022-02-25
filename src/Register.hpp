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

  int operator+(Register other) {
    return this->value + other.value;
  }
  int operator-(Register other) {
    return this->value - other.value;
  }
  int operator*(Register other) {
    return this->value * other.value;
  }

  friend ostream& operator<<(ostream& output, Register& reg) {
    output << "start: " << reg.startIndex << endl;
    output << "end: " << reg.endIndex << endl;
    output << "Value: " << reg.value << endl;
    return output;
  }

} Register_t; 

#endif  // Register