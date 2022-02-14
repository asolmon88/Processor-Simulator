#include <iostream>
#include <vector>
#include <string>

#include "Auxiliar.hpp"
#include "Instruction.hpp"
#include "Section.hpp"

int main() {
  std::vector<size_t> registers;
  std::vector<size_t> memory;
  std::vector<Instruction> instructions;
  std::vector<Section_t> sections;

  Auxiliar::getInstructions(instructions, sections);

  for (int i = 0; i < sections.size(); ++i) {
    cout << sections[i] << endl;
  }  

  return 0;
}