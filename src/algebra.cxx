#include <iostream>

#include "Algorithms.hxx"
#include "Builder.hxx"

int main () {
  auto tree = loadTree ("area_2nd_kinetic.prs");
//  auto tree = buildEpsilonEtaTree ("abcdef");

//  std::cout << printTree (tree) << std::endl;

  contractTreeWithEta (tree, 'c', 'd');
  canonicalizeTree (tree);

  std::cout << "contracted tree" << std::endl;

  std::cout << printTree (tree) << std::endl;
}
