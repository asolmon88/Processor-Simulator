#include <fstream>
#include "Auxiliar.hpp"

void Auxiliar::readFile(const char* filename, std::vector<std::string>& instructions) {
  std::ifstream file;
  file.open("/home/ariel/Documents/CA/Processor-Simulator/src/test.txt");
  std::vector<std::string> instructions;// read instructions from the file

  char charInstruction[50];
  std::string strInstruction;

  while (file.getline(charInstruction, 50)) {
    strInstruction = charInstruction;
    instructions.push_back(strInstruction);
  }

  /*for (size_t i = 0; i < readInstructions.size(); ++i) {
    std::cout << readInstructions[i] << std::endl;
  }*/
  Instruction first(instructions[0]);
  
  file.close();
}

/**
 * @brief Method to know if a line read is an instruction or section
 * 
 * @param instruction 
 * @return int, -1 means not and instruction
 */

int Auxiliar::isInstruction(std::string instruction) {
  int pos = instruction.find(" ");
  return pos;
}

void saveInstructions(
  std::vector<Instruction>& instructions, std::vector<Section_t>& sections) {
  
}
