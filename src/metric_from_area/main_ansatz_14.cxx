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

int main () {
  std::string indices {"abcdefijklpqrs"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, 1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'i', 'j'}, {'j', 'i'}}, -1},
    {{{'k', 'l'}, {'l', 'k'}}, -1},
    {{{'p', 'q'}, {'q', 'p'}}, -1},
    {{{'r', 's'}, {'s', 'r'}}, -1},
    {{{'c', 'e'}, {'d', 'f'}, {'e', 'c'}, {'f', 'd'}}, 1},
    {{{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1},
    {{{'p', 'r'}, {'q', 's'}, {'r', 'p'}, {'s', 'q'}}, 1}
  };

  std::vector<std::pair<std::map<char, char>, int>> multi_exchange_symmetry = {
    {{{'i', 'p'}, {'j', 'q'}, {'k', 'r'}, {'l', 's'}, {'p', 'i'}, {'q', 'j'}, {'r', 'k'}, {'s', 'l'}}, 1},
    {{{'c', 'i'}, {'d', 'j'}, {'e', 'k'}, {'f', 'l'}, {'i', 'c'}, {'j', 'd'}, {'k', 'e'}, {'l', 'f'}}, 1},
    {{{'c', 'i'}, {'d', 'j'}, {'e', 'k'}, {'f', 'l'}, {'i', 'p'}, {'j', 'q'}, {'k', 'r'}, {'l', 's'}, {'p', 'c'}, {'q', 'd'}, {'r', 'e'}, {'s', 'f'}}, 1},
    {{{'c', 'p'}, {'d', 'q'}, {'e', 'r'}, {'f', 's'}, {'i', 'c'}, {'j', 'd'}, {'k', 'e'}, {'l', 'f'}, {'p', 'i'}, {'q', 'j'}, {'r', 'k'}, {'s', 'l'}}, 1},
    {{{'c', 'p'}, {'d', 'q'}, {'e', 'r'}, {'f', 's'}, {'p', 'c'}, {'q', 'd'}, {'r', 'e'}, {'s', 'f'}}, 1}
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

//  std::cout << "Applying multi exchange symmetry ..." << std::endl;
//  multiExchangeSymmetrizeTree (tree, multi_exchange_symmetry);
//  std::cout << "... applied." << std::endl;

  std::cout << std::endl;
  std::cout << "Sorting ansatz tree ..." << std::endl;
  sortTree (tree);
  redefineScalarsSym (tree);
  std::cout << "... sorted." << std::endl;
  std::cout << std::endl;

  auto variable_set = getVariableSet (tree);
  std::cout << "There are " << variable_set.size() << " variables." << std::endl;

  auto function = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_3 {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices {0, 0};

    std::cout << "Evaluating indices." << std::endl;
    std::cout << "Number of different combinations : " << 21 * 21 * 21 * 10 << std::endl;
    std::cout << "progress : 0 %" << std::flush;

    int counter = 0;
    int progress = 0;

    do {
      do {
        do {
          do {
            std::map<char, char> eval_map {
              {'c', area_indices_1.first.first},
              {'d', area_indices_1.first.second},
              {'e', area_indices_1.second.first},
              {'f', area_indices_1.second.second},
              {'i', area_indices_2.first.first},
              {'j', area_indices_2.first.second},
              {'k', area_indices_2.second.first},
              {'l', area_indices_2.second.second},
              {'p', area_indices_3.first.first},
              {'q', area_indices_3.first.second},
              {'r', area_indices_3.second.first},
              {'s', area_indices_3.second.second},
              {'a', metric_indices.first},
              {'b', metric_indices.second}
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
            ++counter;
            int _progress = (100 * counter) / (21 * 21 * 21 * 10);
            if (_progress > progress) {
              progress = _progress;
              std::cout << "\r";
              std::cout << "progress : " << progress << " %" << std::flush;
            }
          } while (nextAreaIndices(area_indices_3));
        } while (nextAreaIndices(area_indices_2));
      } while (nextAreaIndices(area_indices_1));
    } while (nextIndexPairSymmetric (metric_indices));

    std::cout << std::endl;
    std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
  };

  reduceNumerical (tree, function);

  auto tree_2 = loadTree ("ansatz_2.prs");
  auto tree_6 = loadTree ("ansatz_6.prs");
  auto tree_10 = loadTree ("ansatz_10.prs");
  size_t tree_2_var_num = getVariableSet (tree_2).size();
  size_t tree_6_var_num = getVariableSet (tree_6).size();
  size_t tree_10_var_num = getVariableSet (tree_10).size();
  shiftVariables (tree, tree_2_var_num + tree_6_var_num + tree_10_var_num);

  saveTree (tree, "ansatz_14.prs");

  if (checkSaveAndLoad(tree)) {
    std::cout << "Tree and saved tree coincide." << std::endl;
  } else {
    std::cout << "ERROR! TREE DOES NOT COINCIDE WITH SAVED TREE!" << std::endl;
  }

  std::cout << printTree (tree) << std::endl;

  return 0;
}