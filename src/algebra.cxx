#include <iostream>

#include "Algorithms.hxx"

int main () {
  auto tree = loadTree ("area_1st_mass.prs");

  std::cout << printTree (tree) << std::endl;

  contractTreeWithEta (tree, 'a', 'b');

  removeEmptyBranches (tree);

  std::cout << printTree (tree) << std::endl;
}
