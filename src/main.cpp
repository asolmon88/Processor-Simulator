#include <iostream>
#include <vector>
#include <string>

#include "Auxiliar.hpp"
#include "Instruction.hpp"

std::vector<size_t> registers;
std::vector<size_t> memory;
std::vector<Instruction> instructions;

int main() {
  std::vector<std::string> readInstructions;
  Auxiliar::readFile("/home/ariel/Documents/CA/Processor-Simulator/src/test.txt",
    readInstructions);
  for (int i = 0; i < readInstructions.size(); ++i) {
    if (Auxiliar::isInstruction(readInstructions[i]) != -1) {
      Auxiliar::saveInstructions(instructions, readInstructions[i]);
    } else {
      std::cout << "no es" << std::endl;
    }
  }
  return 0;
}