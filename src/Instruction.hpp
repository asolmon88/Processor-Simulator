#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <iostream>
#include <string>
using namespace std;

class Instruction {

  friend ostream& operator<<(ostream& output, Instruction& instruction) {
    instruction.print(output);
    return output;
  }

  private:
   std::string opcode;
   std::string section;
   int value;
   int offset;
   int offsetReg;
   int hasOffsetInFront;
   size_t memIndex;
   size_t register1;
   size_t register2;
   size_t register3; // only used by arithmetic

  public:
   Instruction();
   void setOpcode(std::string opcode);
   void setReg1(std::string reg1);
   void setReg2(std::string reg2);
   void setReg3(std::string reg3);
   void setValue(int value);
   void setOffset(int value);
   void setOffsetReg(std::string reg);
   void setOffsetLocation(int value);
   void setMemIndex(size_t index);
   void setSection(std::string section);
   ostream& print(ostream&);

   std::string getOpcode();
   std::string getSection();
   int getValue();
   int getOffset();
   int getOffsetReg();
   int offsetInFront();
   size_t getMemIndex();
   size_t getR1();
   size_t getR2();
   size_t getR3();

   int operator!=(Instruction&);
};

#endif  // INSTRUCTIONS_H_
