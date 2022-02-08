#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <string>

class Instruction {

  private:
   std::string opcode;
   int value;
   size_t register1;
   size_t register2;
   size_t register3; // only used by arithmetic

  public:
   Instruction(std::string& instruction);
   void setReg1(size_t reg1);
   void setReg2(size_t reg1);
   void setReg3(size_t reg1);
   int changeToMem();
};

#endif  // INSTRUCTIONS_H_
