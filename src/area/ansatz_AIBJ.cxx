#include <algorithm>
#include <chrono>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>

#include "Algorithms.hxx"
#include "LinearAlgebra.hxx"
#include "Tree.hxx"
#include "Tensor.hxx"
#include "Eta.hxx"
#include "Scalar.hxx"
#include "Builder.hxx"

#include "Indices.hxx"

void eval_fun (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) {
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};
  std::pair<char, char> metric_indices_1 {0, 0};
  std::pair<char, char> metric_indices_2 {0, 0};

  std::cout << "Evaluating indices." << std::endl;
  std::cout << "Number of different combinations : " << 21 * 21 * 10 * 10 << std::endl;
  std::cout << "progress : 0 %" << std::flush;

  int counter = 0;
  int progress = 0;

  do {
    do {
      do {
        do {
          std::map<char, char> eval_map {
            {'a', area_indices_1.first.first},
            {'b', area_indices_1.first.second},
            {'c', area_indices_1.second.first},
            {'d', area_indices_1.second.second},
            {'e', area_indices_2.first.first},
            {'f', area_indices_2.first.second},
            {'g', area_indices_2.second.first},
            {'h', area_indices_2.second.second},
            {'i', metric_indices_1.first},
            {'j', metric_indices_1.second},
            {'k', metric_indices_2.first},
            {'l', metric_indices_2.second}
          };
          auto eval_res_tmp = evaluateTree (t, eval_map);
          if (!eval_res_tmp.empty()) {
            eval_res_set.insert (std::move(eval_res_tmp));
          }
          ++counter;
          int _progress = (100 * counter) / (21 * 21 * 10 * 10);
          if (_progress > progress) {
            progress = _progress;
            std::cout << "\r";
            std::cout << "progress : " << progress << " %" << std::flush;
          }
        } while (nextAreaIndices(area_indices_2));
      } while (nextAreaIndices(area_indices_1));
    } while (nextIndexPairSymmetric (metric_indices_2));
  } while (nextIndexPairSymmetric (metric_indices_1));

  std::cout << std::endl;
  std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
}

int main () {
  std::string indices {"abcdefghijkl"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'g', 'h'}, {'h', 'g'}}, -1},
    {{{'i', 'j'}, {'j', 'i'}}, 1},
    {{{'k', 'l'}, {'l', 'k'}}, 1},
    {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1},
    {{{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'l', 'f'}}, 1},
    {{{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'i', 'k'}, {'j', 'l'},
      {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}, {'k', 'i'}, {'l', 'j'}}, 1}
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

  std::for_each(exchange_symmetries.cbegin(), exchange_symmetries.cend(),
    [&tree,n=0,&exchange_symmetries] (auto const & p) mutable {
      std::cout << "Applying exchange symmetry " << ++n << " out of " << exchange_symmetries.size() << " ..." << std::endl;
      exchangeSymmetrizeTree (tree, p.first, p.second);
      redefineScalarsSym (tree);
      std::cout << "... applied." << std::endl;
    });

  std::cout << std::endl;
  std::cout << "Sorting ansatz tree ..." << std::endl;
  sortTree (tree);
  redefineScalarsSym (tree);
  std::cout << "... sorted." << std::endl;
  std::cout << std::endl;

  auto variable_set = getVariableSet (tree);
  std::cout << "There are " << variable_set.size() << " variables." << std::endl;

  reduceNumerical (tree, eval_fun);

  saveTree (tree, "ansatz_AIBJ.prs");

  return 0;
}
