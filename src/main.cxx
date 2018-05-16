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

#include "Indices.hxx"

int main () {
  std::string indices {"abcdefghpq"};
//  std::string indices {"abcd"};
//  std::string indices {"abcdpq"};
//  std::string indices {"abcdef"};
//  std::string indices {"abcdefgh"};
//  std::string indices {"abcdefghijklpq"};
//  std::string indices {"abcdefghijkl"};

  std::vector<std::pair<std::map<char, char>, int>> symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
//    {{{'a', 'b'}, {'b', 'a'}}, 1},
//    {{{'c', 'd'}, {'d', 'c'}}, 1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'g', 'h'}, {'h', 'g'}}, -1},
//    {{{'i', 'j'}, {'j', 'i'}}, -1},
//    {{{'k', 'l'}, {'l', 'k'}}, -1},
    {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1},
    {{{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'h', 'f'}}, 1},
//    {{{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1},
    {{{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}}, 1},
    {{{'p', 'q'}, {'q', 'p'}}, 1}
  };

  std::cout << "################################" << std::endl;
  std::cout << "Spacetime ansatz generator v0.1\u03b1" << std::endl;
  std::cout << "################################" << std::endl;
  std::cout << std::endl;
  std::cout << "     indices : " << indices << std::endl;
  std::cout << "# of indices : " << indices.size() << std::endl;
  std::cout << std::endl;
  std::cout << "Constructing ansatz tree ..." << std::endl;

  auto tree = buildEpsilonEtaTree (indices);

  std::cout << "... ansatz tree constructed." << std::endl;
  std::cout << std::endl;

  std::for_each(symmetries.cbegin(), symmetries.cend(),
    [&tree,n=0,&symmetries] (auto const & p) mutable {
      std::cout << "Applying symmetry " << ++n << " out of " << symmetries.size() << " ..." << std::endl;
      symmetrizeTree (tree, p.first, p.second);
      redefineScalarsSym (tree);
      std::cout << "... applied." << std::endl;
    });

  std::cout << std::endl;
  std::cout << "Sorting ansatz tree ..." << std::endl;
  sortTree (tree);
  redefineScalarsSym (tree);
  std::cout << "... sorted." << std::endl;
  std::cout << std::endl;

  if (!tree->isValid() || !isTreeSorted (tree)) {
    throw 0;
  }

  std::cout << "print tree? [y/n] ";

  char c;
  std::cin >> c;

  if (c == 'y') {
    std::cout << std::endl;
    std::cout << printTree (tree);
  }

  auto res = evaluateTree(tree, {
    {'a', 1},
    {'b', 0},
    {'c', 0},
    {'d', 2},
    {'e', 1},
    {'f', 3},
    {'g', 2},
    {'h', 3},
    {'p', 2},
    {'q', 2}
  });

  std::pair<std::pair<std::pair<char, char>, std::pair<char, char>>,std::pair<std::pair<char, char>, std::pair<char, char>>> area_indices {{{0, 1}, {0, 1}}, {{0, 1}, {0, 1}}};
  std::pair<char, char> derivative_indices {0, 0};

  std::set<std::map<size_t, mpq_class>> eval_res_set;

  do {
    do {
      std::map<char, char> eval_map {
        {'a', area_indices.first.first.first},
        {'b', area_indices.first.first.second},
        {'c', area_indices.first.second.first},
        {'d', area_indices.first.second.second},
        {'e', area_indices.second.first.first},
        {'f', area_indices.second.first.second},
        {'g', area_indices.second.second.first},
        {'h', area_indices.second.second.second},
        {'p', derivative_indices.first},
        {'q', derivative_indices.second},
      };
      auto eval_res_tmp = evaluateTree (tree, eval_map);
      if (!eval_res_tmp.empty()) {
        eval_res_set.insert (std::move(eval_res_tmp));
      }
    } while (nextAreaIndicesPairSymmetric(area_indices));
  } while (nextIndexPairSymmetric (derivative_indices));

  std::for_each (eval_res_set.cbegin(), eval_res_set.cend(),
    [] (auto const & m) {
      std::for_each(m.cbegin(), m.cend(),
        [] (auto const & p) {
          std::cout << p.first << " -> " << p.second << "      ";
        });
      std::cout << std::endl;
    });

  std::cout << eval_res_set.size() << std::endl;

  return 0;
}
