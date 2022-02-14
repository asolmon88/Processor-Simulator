#include "Simulator.hpp"

Simulator::Simulator() {
  std::vector<int> registers = std::vector<int>(32);
  std::vector<int> memory = std::vector<int>(1024);
  this->finished = 0;
  this->cycles = 0;
  this->numberInstructions = 0;
  this->PC = 0;
}

void Simulator::fetch() {

}
void Simulator::decode() {

}
void Simulator::execute() {

}
void Simulator::simulate() {

}
