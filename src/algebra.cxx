#include <iostream>

#include "Algorithms.hxx"
#include "Builder.hxx"
#include "Epsilon.hxx"

int main () {

  auto tree = loadTree ("area_2nd_kinetic.prs");
//  auto tree = buildEpsilonEtaTree ("abcdef");

//  std::cout << printTree (tree) << std::endl;

  contractTreeWithEpsilon3 (tree, 'm', 'j', 'k', 'l');
  if (!tree->isValid()) {
    throw 0;
  }
//  contractTreeWithEta (tree, 'j', 'l');
  canonicalizeTree (tree);
  if (!tree->isValid()) {
    throw 0;
  }

  std::cout << "contracted tree" << std::endl;

  std::cout << printTree (tree) << std::endl;

/*
  Epsilon epsilon ('a', 'b', 'c', 'd');
  auto ret = epsilon.multiplyWithOther3 ('b', 'c', 'd');
  std::cout << std::get<0>(ret) << std::endl;
  std::cout << std::get<1>(ret) << std::endl;
  auto & m = std::get<2>(ret);
  std::for_each (m.cbegin(), m.cend(),
      [] (auto const & p) {
        std::cout << p.first << "\t" << p.second << std::endl;
      });
*/
}
