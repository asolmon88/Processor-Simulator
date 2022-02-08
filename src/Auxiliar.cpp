#include <fstream>
#include <iostream>
#include "Auxiliar.hpp"

void Auxiliar::readFile(const char* filename, std::vector<std::string>& instructions) {
  std::ifstream file;
  file.open(filename);

  char charInstruction[50];
  std::string strInstruction;

  while (file.getline(charInstruction, 50)) {
    strInstruction = charInstruction;
    instructions.push_back(strInstruction);
  }

  /*for (size_t i = 0; i < instructions.size(); ++i) {
    std::cout << instructions[i] << std::endl;
  }*/
  file.close();
}

/**
 * @brief Method to know if a line read is an instruction or section
 * 
 * @param instruction 
 * @return int, -1 means not and instruction
 */

int Auxiliar::isInstruction(std::string instruction) {
  int pos = instruction.find(",");
  return pos;
}

void Auxiliar::saveInstructions(
  std::vector<Instruction>& instructions, std::string instruction) {
  std::string currentText;
  size_t value = 0;
  int initialPos = 0;
  int endPos = 0;
  std::vector<std::string> separated;
  Instruction newInstruction;

  separate(instruction, separated);

  currentText = separated[0];
  newInstruction.setOpcode(currentText);
  // add load to mov between registers
  if (currentText == "ld") {
    if (separated[1][0] == 'R') {
      newInstruction.setMemIndex(stoll(separated[1].substr(
        1,separated[1].size())));
      newInstruction.setReg1(separated[2]);
    } else {
      newInstruction.setMemIndex(stoll(separated[1]));
      newInstruction.setReg1(separated[2]);
    }
  } else if (currentText == "str") {
    if (separated[1][0] == 'R') {
      newInstruction.setReg1(separated[1]);
    } else {
      newInstruction.setValue(stoll(separated[1]));
    }
    if (separated[2][0] == 'R') {
      newInstruction.setReg2(separated[2]);
    } else {
      newInstruction.setMemIndex(stoll(separated[2]));
    }
  } else if (currentText == "mov") {
    if (separated[1][0] == 'R') {
      newInstruction.setReg1(separated[1]);
      newInstruction.setReg2(separated[2]);
    } else {
      newInstruction.setValue(stoll(separated[1]));
      newInstruction.setReg2(separated[2]);
    }
  } else if (currentText == "add" || currentText == "sub" 
    || currentText == "mult") {
    newInstruction.setReg1(separated[1]);
    newInstruction.setReg2(separated[2]);
    newInstruction.setReg3(separated[3]);
  } else if (currentText == "cmp") {
    newInstruction.setReg1(separated[1]);
    newInstruction.setReg2(separated[2]);
  } else if (currentText == "jmp" || currentText == "je" 
      || currentText == "ja" || currentText == "jb") {
    newInstruction.setSection(separated[1]);
  } else {
    std::cout << currentText << "is not a valid instruction" << std::endl;
  }
}

// Adapted from: 
// https://www.geeksforgeeks.org/split-a-sentence-into-words-in-cpp/
void Auxiliar::separate(std::string line, std::vector<std::string>& separated) {
  std::string word = "";
  for (auto x : line) {
    if ((x == ' ' || x == ',') && word != "") {
      separated.push_back(word);
      word = "";
    }
    else {
      if (x != ' ') {
        word = word + x;
      }
    }
  }
  separated.push_back(word);
}
