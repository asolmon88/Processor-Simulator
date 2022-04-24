#include <iostream>
#include <vector>
#include <string>

#include "Auxiliar.hpp"
#include "Instruction.hpp"
#include "Section.hpp"
#include "Simulator.hpp"

int main() {

  Simulator simulator;

  Auxiliar::getInstructions(simulator.instructions, simulator.sections);

  simulator.simulate();

  std::cout << simulator.registers[2] << std::endl;
  std::cout << "_________________________________" << std::endl;
  std::cout << "STATISTICS" << std::endl;
  std::cout << "T.I.\tCycles\tI.P.C\tC.B." << std::endl;
  std::cout << simulator.TI << "\t" << simulator.cycles << "\t"
    << simulator.IPC << "\t" << simulator.correctPredictions << std::endl;
  std::cout << "_________________________________" << std::endl;


 /*  for (auto x : simulator.instructions) {
    cout << x << endl;
  } */
  return 0;
}