#ifndef AUXILIAR_H_
#define AUXILIAR_H_

#include <fstream>
#include <string>
#include <vector>

#include "Instruction.hpp"
#include "Section.hpp"

class Auxiliar {
  public:
   void readFile(const char* filename, std::vector<std::string>& instructions);
   int isInstruction(std::string instruction);
   void saveInstructions(
    std::vector<Instruction>& instructions, std::vector<Section_t>& sections);
};

#endif  // AUXILIAR_H_