#ifndef REGISTER_
#define REGISTER_

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

  friend ostream& operator<<(ostream& output, Register& reg) {
    output << "start: " << reg.startIndex << endl;
    output << "end: " << reg.endIndex << endl;
    output << "Value: " << reg.value << endl;
    return output;
  }

} Register_t; 

#endif  // Register