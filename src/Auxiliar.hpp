#ifndef AUXILIAR_H_
#define AUXILIAR_H_

#include <fstream>
#include <string>
#include <vector>

#include "Instruction.hpp"
#include "Section.hpp"

class Auxiliar {
  private:
   static void separate(std::string line, std::vector<std::string>& separated);
  public:
   static void readFile(const char* filename, std::vector<std::string>& instructions);
   static int isInstruction(std::string instruction);
   static void saveInstructions(
    std::vector<Instruction>& instructions, std::string instruction, int& moves);
   static int saveSection(std::vector<Section_t>& sections, std::string instruction,
    int index);
   static void getInstructions(std::vector<Instruction>& instructions,
    std::vector<Section_t>& sections, const char* filename = "sum");
};

#endif  // AUXILIAR_H_