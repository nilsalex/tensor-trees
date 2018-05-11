#include <algorithm>
#include <chrono>
#include <iostream>
#include <vector>

#include "Algorithms.hxx"
#include "LinearAlgebra.hxx"
#include "Tree.hxx"
#include "Tensor.hxx"
#include "Eta.hxx"
#include "Scalar.hxx"
#include "Builder.hxx"

int main () {
//  std::string indices {"abcdefghpq"};
//  std::string indices {"abcd"};
//  std::string indices {"abcdpq"};
//  std::string indices {"abcdef"};
//  std::string indices {"abcdefgh"};
  std::string indices {"abcdefghijklpq"};
//  std::string indices {"abcdefghijkl"};

  std::vector<std::pair<std::map<char, char>, int>> symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
//    {{{'a', 'b'}, {'b', 'a'}}, 1},
//    {{{'c', 'd'}, {'d', 'c'}}, 1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'g', 'h'}, {'h', 'g'}}, -1},
    {{{'i', 'j'}, {'j', 'i'}}, -1},
    {{{'k', 'l'}, {'l', 'k'}}, -1},
    {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1},
    {{{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'h', 'f'}}, 1},
    {{{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1},
//    {{{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}}, 1},
//    {{{'p', 'q'}, {'q', 'p'}}, 1}
  };

  auto tree = buildEpsilonEtaTree (indices);

  std::for_each(symmetries.cbegin(), symmetries.cend(),
    [&tree,n=0,&symmetries] (auto const & p) mutable {
      std::cout << "Applying symmetry " << ++n << " out of " << symmetries.size() << std::endl;
      symmetrizeTree (tree, p.first, p.second);
    });

  redefineScalarsSym (tree);

  if (!tree->isValid() || !isTreeSorted (tree)) {
    throw 0;
  }

  std::cout << printTree (tree);

  return 0;
}
