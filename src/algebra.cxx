#include <iostream>

#include "Algorithms.hxx"
#include "Builder.hxx"

int main () {
/*
  auto tree = loadTree ("area_1st_mass.prs");

  std::cout << printTree (tree) << std::endl;
*/

  auto tree = buildEpsilonEtaTree ("abcdef");

  std::cout << printTree (tree) << std::endl;

  contractTreeWithEta (tree, 'd', 'f');

  std::cout << printTree (tree) << std::endl;
}
