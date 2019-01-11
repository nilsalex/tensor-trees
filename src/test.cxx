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
  std::string indices {"abcdpqrstu"};

  std::vector<std::pair<std::map<char, char>, int>> exchange_symmetries = {
    {{{'a', 'b'}, {'b', 'a'}}, -1},
    {{{'c', 'd'}, {'d', 'c'}}, -1},
    {{{'p', 'q'}, {'q', 'p'}}, -1},
    {{{'r', 's'}, {'s', 'r'}}, -1},
    {{{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1},
    {{{'p', 'r'}, {'q', 's'}, {'r', 'p'}, {'s', 'q'}}, 1},
    {{{'t', 'u'}, {'u', 't'}}, 1}
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

  auto function = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices {0, 0};

    std::cout << "Evaluating indices." << std::endl;
    std::cout << "Number of different combinations : " << 21 * 21 * 10 << std::endl;
    std::cout << "progress : 0 %" << std::flush;

    int counter = 0;
    int progress = 0;

    do {
      do {
        do {
            std::map<char, char> eval_map {
              {'a', area_indices_1.first.first},
              {'b', area_indices_1.first.second},
              {'c', area_indices_1.second.first},
              {'d', area_indices_1.second.second},
              {'p', area_indices_2.first.first},
              {'q', area_indices_2.first.second},
              {'r', area_indices_2.second.first},
              {'s', area_indices_2.second.second},
              {'t', metric_indices.first},
              {'u', metric_indices.second}
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
  std::for_each (eval_res_tmp.cbegin(), eval_res_tmp.cend(),
      [] (auto const & p) {
        std::cout << p.second << " * e_" << p.first << " + ";
      });

  std::cout << std::endl;
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
            ++counter;
            int _progress = (100 * counter) / (21 * 21 * 10);
            if (_progress > progress) {
              progress = _progress;
              std::cout << "\r";
              std::cout << "progress : " << progress << " %" << std::flush;
            }
        } while (nextAreaIndices(area_indices_2));
      } while (nextAreaIndices(area_indices_1));
    } while (nextIndexPairSymmetric (metric_indices));

    std::cout << std::endl;
    std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
  };

  reduceNumerical (tree, function);
  reduceNumerical (tree, function);

  std::map<size_t, Scalar> subs_map = {
    {1, Scalar(std::map<size_t, mpq_class>( {{2, mpq_class(1,1)}} ))},
    {2, Scalar(std::map<size_t, mpq_class>( {{1, mpq_class(1,1)}} ))},
    {3, Scalar(std::map<size_t, mpq_class>( {{8, mpq_class(-1,1)}, {9, mpq_class(-6, 1)}, {10, mpq_class(-1, 2)}} ))},
    {4, Scalar(std::map<size_t, mpq_class>( {{10, mpq_class(1,1)}} ))},
    {5, Scalar(std::map<size_t, mpq_class>( {{10, mpq_class(1,1)}} ))},
    {6, Scalar(std::map<size_t, mpq_class>( {{9, mpq_class(1,1)}} ))},
    {7, Scalar(std::map<size_t, mpq_class>( {{8, mpq_class(1,1)}} ))},
    {8, Scalar(std::map<size_t, mpq_class>( {{3, mpq_class(-1,6)}, {4, mpq_class(1, 6)}, {6, mpq_class(1, 3)}, {7, mpq_class(-2, 3)}} ))},
    {9, Scalar(std::map<size_t, mpq_class>( {{3, mpq_class(1,6)}} ))},
    {10, Scalar(std::map<size_t, mpq_class>( {{3, mpq_class(1,2)}, {4, mpq_class(-1, 1)}, {6, mpq_class(-2, 1)}, {7, mpq_class(2, 1)}} ))},
    {11, Scalar(std::map<size_t, mpq_class>( {{3, mpq_class(-1,2)}, {5, mpq_class(-1, 2)}} ))},
    {12, Scalar(std::map<size_t, mpq_class>( {{7, mpq_class(1,1)}} ))},
    {13, Scalar(std::map<size_t, mpq_class>( {{6, mpq_class(1,1)}} ))},
    {14, Scalar(std::map<size_t, mpq_class>( {{5, mpq_class(1,1)}} ))},
    {15, Scalar(std::map<size_t, mpq_class>( {{4, mpq_class(1,1)}} ))},
    {16, Scalar(std::map<size_t, mpq_class>( {{3, mpq_class(1,1)}} ))}
  };

//  substituteWithScalars (tree, subs_map);

//  canonicalizeTree (tree);
/*
  contractTreeWithEta (tree, 'a', 'c');
  contractTreeWithEta (tree, 'b', 'd');

  canonicalizeTree (tree);

  auto function3 = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices {0, 0};

    std::cout << "Evaluating indices." << std::endl;
    std::cout << "Number of different combinations : " << 21 * 10 << std::endl;
    std::cout << "progress : 0 %" << std::flush;

    int counter = 0;
    int progress = 0;

    do {
      do {
            std::map<char, char> eval_map {
              {'p', area_indices.first.first},
              {'q', area_indices.first.second},
              {'r', area_indices.second.first},
              {'s', area_indices.second.second},
              {'t', metric_indices.first},
              {'u', metric_indices.second}
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
            ++counter;
            int _progress = (100 * counter) / (21 * 10);
            if (_progress > progress) {
              progress = _progress;
              std::cout << "\r";
              std::cout << "progress : " << progress << " %" << std::flush;
            }
      } while (nextAreaIndices(area_indices));
    } while (nextIndexPairSymmetric (metric_indices));

    std::cout << std::endl;
    std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
  };

  reduceNumerical (tree, function3);
  */

//  std::cout << printTree (tree) << std::endl;

  /*

  auto tree_L_abcdpqrstu = copyTree (tree);
  auto tree_L_abcdpqrstm = copyTree (tree);
  auto tree_L_abcdmqrstu = copyTree (tree);

  exchangeTensorIndices (tree_L_abcdpqrstm, {{'u', 'm'}});
  exchangeTensorIndices (tree_L_abcdmqrstu, {{'p', 'm'}});

  multiplyTree (tree_L_abcdpqrstm, -2);
  multiplyTree (tree_L_abcdmqrstu, -4);

  contractTreeWithEta (tree_L_abcdpqrstu, 'a', 'c');
  contractTreeWithEta (tree_L_abcdpqrstm, 'a', 'c');
  contractTreeWithEta (tree_L_abcdmqrstu, 'a', 'c');
 
  multiplyTreeWithEta (tree_L_abcdpqrstu, 'm', 'n');
  multiplyTreeWithEta (tree_L_abcdpqrstm, 'u', 'n');
  multiplyTreeWithEta (tree_L_abcdmqrstu, 'p', 'n');
 
  exchangeSymmetrizeTree (tree_L_abcdpqrstm, {{'m', 'n'}, {'n', 'm'}}, 1);
  exchangeSymmetrizeTree (tree_L_abcdpqrstm, {{'t', 'u'}, {'u', 't'}}, 1);
  exchangeSymmetrizeTree (tree_L_abcdmqrstu, {{'m', 'n'}, {'n', 'm'}}, 1);
  exchangeSymmetrizeTree (tree_L_abcdmqrstu, {{'p', 'q'}, {'q', 'p'}}, -1);
  exchangeSymmetrizeTree (tree_L_abcdmqrstu, {{'r', 's'}, {'s', 'r'}}, -1);
  exchangeSymmetrizeTree (tree_L_abcdmqrstu, {{'p', 'r'}, {'q', 's'}, {'r', 'p'}, {'s', 'q'}}, 1);
 
  auto tree_eq = copyTree (tree_L_abcdpqrstu);
  mergeTrees (tree_eq, tree_L_abcdpqrstm);
  mergeTrees (tree_eq, tree_L_abcdmqrstu);
 
  exchangeSymmetrizeTree (tree_eq, {{'b', 'm'}, {'d', 'n'}, {'m', 'b'}, {'n', 'd'}}, -1);

  auto function2 = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices_1 {0, 0};
    std::pair<char, char> metric_indices_2 {0, 0};
    std::pair<char, char> metric_indices_3 {0, 0};

    std::cout << "Evaluating indices." << std::endl;
    std::cout << "Number of different combinations : " << 21 * 1000 << std::endl;
    std::cout << "progress : 0 %" << std::flush;

    int counter = 0;
    int progress = 0;

    do {
      do {
        do {
          do {
            std::map<char, char> eval_map {
              {'p', area_indices.first.first},
              {'q', area_indices.first.second},
              {'r', area_indices.second.first},
              {'s', area_indices.second.second},
              {'t', metric_indices_1.first},
              {'u', metric_indices_1.second},
              {'b', metric_indices_2.first},
              {'d', metric_indices_2.second},
              {'m', metric_indices_3.first},
              {'n', metric_indices_3.second}
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
            ++counter;
            int _progress = (100 * counter) / (21 * 1000);
            if (_progress > progress) {
              progress = _progress;
              std::cout << "\r";
              std::cout << "progress : " << progress << " %" << std::flush;
            }
          } while (nextAreaIndices(area_indices));
        } while (nextIndexPairSymmetric (metric_indices_1));
      } while (nextIndexPairSymmetric (metric_indices_2));
    } while (nextIndexPairSymmetric (metric_indices_3));

    std::cout << std::endl;
    std::cout << "Completed! There are " << eval_res_set.size() << " equations." << std::endl;
  };

  std::cout << printTree (tree_eq) << std::endl;

  solveNumerical ({{tree_eq, function2}});
  */

  return 0;
}
