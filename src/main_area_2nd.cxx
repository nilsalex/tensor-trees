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
  std::string indices {"abcdefghijklpq"};

  std::vector<std::pair<std::map<char, char>, int>> symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
    {{{'e', 'f'}, {'f', 'e'}}, -1},
    {{{'g', 'h'}, {'h', 'g'}}, -1},
    {{{'i', 'j'}, {'j', 'i'}}, -1},
    {{{'k', 'l'}, {'l', 'k'}}, -1},
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

  auto variable_set = getVariableSet (tree);
  std::cout << "There are " << variable_set.size() << " variables." << std::endl;
/*
  std::cout << "print tree? [y/n] ";

  char c;
  std::cin >> c;

  if (c == 'y') {
    std::cout << std::endl;
    std::cout << printTree (tree);
  }
*/
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_3 {{0, 1}, {0, 1}};
  std::pair<char, char> derivative_indices {0, 0};

  std::set<std::map<size_t, mpq_class>> eval_res_set;

  std::cout << "Evaluating indices." << std::endl;
  std::cout << "Number of different combinations : " << 21 * 21 * 21 * 16 << std::endl;
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
            {'i', area_indices_3.first.first},
            {'j', area_indices_3.first.second},
            {'k', area_indices_3.second.first},
            {'l', area_indices_3.second.second},
            {'p', derivative_indices.first},
            {'q', derivative_indices.second},
          };
          auto eval_res_tmp = evaluateTree (tree, eval_map);
          if (!eval_res_tmp.empty()) {
            eval_res_set.insert (std::move(eval_res_tmp));
          }
          ++counter;
          int _progress = (100 * counter) / (21 * 21 * 21 * 16);
          if (_progress > progress) {
            progress = _progress;
            std::cout << "\r";
            std::cout << "progress : " << progress << " %" << std::flush;
          }
        } while (nextAreaIndices(area_indices_3));
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
  } while (nextIndexPair (derivative_indices));

  std::cout << std::endl;
  std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;

  std::map<size_t, size_t> var_map;
  std::set<size_t> erase_set = variable_set;
  std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> eval_res_mat;
  std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> eval_res_mat_mapped;

  std::for_each(eval_res_set.cbegin(), eval_res_set.cend(),
    [row_counter=0,&eval_res_mat] (auto const & m) mutable {
      std::for_each(m.cbegin(), m.cend(),
        [&row_counter,&eval_res_mat] (auto const & p) {
          eval_res_mat.insert(std::make_pair(std::make_pair(row_counter, p.first), p.second));
        }); 
      ++row_counter; 
    }); 

  eval_res_set.clear();

  std::for_each(eval_res_mat.cbegin(), eval_res_mat.cend(),
    [&eval_res_mat_mapped,&var_map,&erase_set,var=0] (auto const & v) mutable {
      erase_set.erase(v.first.second);
      auto it = var_map.find(v.first.second);
      if (it == var_map.end()) {
        var_map.insert(std::make_pair(v.first.second, var));
        ++var;
      }
      eval_res_mat_mapped.insert(std::make_pair(std::make_pair(v.first.first, var_map.at(v.first.second)), v.second));
    });

  eval_res_mat.clear();

  std::cout << "These equations contain " << var_map.size() << " variables." << std::endl;
  std::cout << "That is, " << erase_set.size() << " variables do not contribute at all." << std::endl;
  std::cout << std::endl;
  std::cout << "Computing LU decomposition of the linear system using Eigen subroutines ..." << std::endl;

  auto erase_set_2_mapped = findDependentVariables (eval_res_mat_mapped, eval_res_mat_mapped.rbegin()->first.first, var_map.size());
  std::cout << "... done! " << erase_set_2_mapped.size() << " variables are dependent." << std::endl;

  eval_res_mat_mapped.clear();

  std::map<size_t, size_t> var_rmap;
  std::transform (var_map.cbegin(), var_map.cend(), std::inserter(var_rmap, var_rmap.begin()),
    [] (auto const & p) {
      return std::make_pair(p.second, p.first);
    });

  std::set<size_t> erase_set_2;
  std::transform(erase_set_2_mapped.cbegin(), erase_set_2_mapped.cend(), std::inserter(erase_set_2, erase_set_2.begin()),
    [&var_rmap] (auto const & v) {
      return var_rmap.at(v);
    });

  erase_set_2_mapped.clear();

  erase_set.merge(erase_set_2);
  erase_set_2.clear();

  std::cout << "Removing all " << erase_set.size() << " variables from the ansatz." << std::endl;

  setVariablesToZero (tree, erase_set);
  removeEmptyBranches (tree);

  return 0;
}
