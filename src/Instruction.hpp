#ifndef INSTRUCTIONS_H_
#define INSTRUCTIONS_H_

#include <string>

class Instruction {

  private:
   std::string opcode;
   std::string section;
   size_t value;
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
   void setValue(size_t value);
   void setMemIndex(size_t index);
   void setSection(std::string section);
   int changeToMem();
};

#endif  // INSTRUCTIONS_H_