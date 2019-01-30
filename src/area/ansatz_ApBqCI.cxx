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
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_3 {{0, 1}, {0, 1}};
  std::pair<char, char> metric_indices {0, 0};

  std::cout << "Evaluating indices." << std::endl;
  std::cout << "Number of different combinations : " << 210 * (42*85) << std::endl;
  std::cout << "progress : 0 %" << std::flush;

  int counter = 0;
  int progress = 0;

  do { // area_indices_1
    for (char p = 0; p < 4; ++p) {
      auto area_indices_2 = area_indices_1;
        do { // area_indices_2
          for (char q = 0; q < 4; ++q) {
            do { // area_indices_3
              do { // metric_indices

          if (area_indices_1 == area_indices_2 && q < p)
            continue;
          std::map<char, char> eval_map {
            {'a', area_indices_1.first.first},
            {'b', area_indices_1.first.second},
            {'c', area_indices_1.second.first},
            {'d', area_indices_1.second.second},
            {'e', area_indices_2.first.first},
            {'f', area_indices_2.first.second},
            {'g', area_indices_2.second.first},
            {'h', area_indices_2.second.second},
            {'i', area_indices_3.first.first},
            {'j', area_indices_3.first.second},
            {'k', area_indices_3.second.first},
            {'l', area_indices_3.second.second},
            {'p', p},
            {'q', q},
            {'r', metric_indices.first},
            {'s', metric_indices.second}
          };
          auto eval_res_tmp = evaluateTree (t, eval_map);
          if (!eval_res_tmp.empty()) {
            auto pivot = eval_res_tmp.begin()->second;
            if (pivot != 1) {
              std::for_each(eval_res_tmp.begin(), eval_res_tmp.end(),
                  [&pivot](auto & p) {
                    p.second = p.second / pivot;
                  });
            }
            eval_res_set.insert (std::move(eval_res_tmp));
          }
          ++counter;
          int _progress = (100 * counter) / (210 * (42*85));
          if (_progress > progress) {
            progress = _progress;
            std::cout << "\r";
            std::cout << "progress : " << progress << " %" << std::flush;
          }
        } while (nextIndexPairSymmetric (metric_indices));
      } while (nextAreaIndices(area_indices_3));
    } 
  } while (nextAreaIndices(area_indices_2));
  }
  } while (nextAreaIndices(area_indices_1));

  std::cout << std::endl;
  std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
}

int main () {
  /*
  std::string indices {"abcdefghijklpqrs"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'g', 'h'}, {'h', 'g'}}, -1},
    {{{'i', 'j'}, {'j', 'i'}}, -1},
    {{{'k', 'l'}, {'l', 'k'}}, -1},
    {{{'r', 's'}, {'s', 'r'}}, 1},
    {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1},
    {{{'e', 'g'}, {'f', 'h'}, {'g', 'e'}, {'h', 'f'}}, 1},
    {{{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1},
    {{{'a', 'e'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'}, {'p', 'q'},
      {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}, {'q', 'p'}}, 1}
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

  // save unreduced tree
  saveTree (tree, "ansatz_ApBqCI_not_reduced.prs");
  */
  auto tree = loadTree ("ansatz_ApBqCI.prs");

  /*
  std::set<size_t> to_remove = 
#include "vanish_2.hxx"
  ;

  setVariablesToZero (tree, to_remove);
  removeEmptyBranches (tree);

  std::map<size_t, size_t> var_map;
  auto var_set = getVariableSet (tree);

  std::transform (var_set.cbegin(), var_set.cend(), std::inserter (var_map, var_map.begin()),
    [counter=0] (auto const & v) mutable {
      return std::make_pair (v, ++counter);
    });

  substituteVariables (tree, var_map);


  saveTree (tree, "ansatz_ApBqCI.prs");
  */

  reduceNumerical (tree, eval_fun);

  return 0;
}
