#include <iostream>

#include "Algorithms.hxx"
#include "Indices.hxx"

std::unique_ptr<Tree<Node>> firstOrderEquation () {
  auto tree_eq_abmn = std::make_unique<Tree<Node>>();

  auto tree_g_am       = loadTree ("ansatz_2.prs");
  auto tree_g_abmqns   = loadTree ("ansatz_6.prs");

  exchangeTensorIndices (tree_g_am,     {{'b', 'm'}});
  exchangeTensorIndices (tree_g_abmqns, {{'c', 'm'}, {'d', 'q'}, {'e', 'n'}, {'f', 's'}});

  multiplyTree (tree_g_abmqns,   4);

  contractTreeWithEta (tree_g_abmqns, 'q', 's');
  multiplyTreeWithEta (tree_g_am,     'b', 'n');

  exchangeSymmetrizeTree (tree_g_am, {{'a', 'b'}, {'b', 'a'}}, 1);

  std::swap (tree_eq_abmn, tree_g_abmqns);
  mergeTrees (tree_eq_abmn, tree_g_am);
  
  return std::move(tree_eq_abmn);
}

std::unique_ptr<Tree<Node>> secondOrderEquation () {
  auto tree_eq_abcdefmn = std::make_unique<Tree<Node>>();

  auto tree_g_amcdef     = loadTree ("ansatz_6.prs");
  auto tree_g_abmdef     = copyTree (tree_g_amcdef);
  auto tree_g_abmfcd     = copyTree (tree_g_amcdef);
  auto tree_g_abcdefmqns = loadTree ("ansatz_10.prs");

  exchangeTensorIndices (tree_g_amcdef,     {{'b', 'm'}});
  exchangeTensorIndices (tree_g_abmdef,     {{'c', 'm'}});
  exchangeTensorIndices (tree_g_abmfcd,     {{'c', 'm'}, {'d', 'f'}, {'e', 'c'}, {'f', 'd'}});
  exchangeTensorIndices (tree_g_abcdefmqns, {{'i', 'm'}, {'j', 'q'}, {'k', 'n'}, {'l', 's'}});

  multiplyTree (tree_g_abcdefmqns, 4);

  contractTreeWithEta (tree_g_abcdefmqns, 'q', 's');
  multiplyTreeWithEta (tree_g_abmdef,     'c', 'n');
  multiplyTreeWithEta (tree_g_abmfcd,     'e', 'n');
  multiplyTreeWithEta (tree_g_amcdef,     'b', 'n');

  exchangeSymmetrizeTree (tree_g_abmdef, {{'c', 'd'}, {'d', 'c'}}, -1);
  exchangeSymmetrizeTree (tree_g_abmfcd, {{'e', 'f'}, {'f', 'e'}}, -1);
  exchangeSymmetrizeTree (tree_g_amcdef, {{'a', 'b'}, {'b', 'a'}},  1);

  std::swap (tree_eq_abcdefmn, tree_g_abcdefmqns);
  mergeTrees (tree_eq_abcdefmn, tree_g_abmdef);
  mergeTrees (tree_eq_abcdefmn, tree_g_abmfcd);
  mergeTrees (tree_eq_abcdefmn, tree_g_amcdef);

  return std::move(tree_eq_abcdefmn);
}

std::unique_ptr<Tree<Node>> thirdOrderEquation () {
  auto tree_eq_abcdefijklmn = std::make_unique<Tree<Node>>();

  auto tree_g_amcdefijkl     = loadTree ("ansatz_10.prs");
  auto tree_g_abmdefijkl     = copyTree (tree_g_amcdefijkl);
  auto tree_g_abmfcdijkl     = copyTree (tree_g_amcdefijkl);
  auto tree_g_abcdefmjkl     = copyTree (tree_g_amcdefijkl);
  auto tree_g_abcdefmlij     = copyTree (tree_g_amcdefijkl);
  auto tree_g_abcdefijklmqns = loadTree ("ansatz_14.prs");

  exchangeTensorIndices (tree_g_amcdefijkl     , {{'b', 'm'}});
  exchangeTensorIndices (tree_g_abmdefijkl     , {{'c', 'm'}});
  exchangeTensorIndices (tree_g_abmfcdijkl     , {{'c', 'm'}, {'d', 'f'}, {'e', 'c'}, {'f', 'd'}});
  exchangeTensorIndices (tree_g_abcdefmjkl     , {{'i', 'm'}});
  exchangeTensorIndices (tree_g_abcdefmlij     , {{'i', 'm'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}});
  exchangeTensorIndices (tree_g_abcdefijklmqns , {{'p', 'm'}, {'r', 'n'}});

  multiplyTree (tree_g_abcdefijklmqns, 4);

  contractTreeWithEta (tree_g_abcdefijklmqns, 'q', 's');
  multiplyTreeWithEta (tree_g_abmdefijkl    , 'c', 'n');
  multiplyTreeWithEta (tree_g_abmfcdijkl    , 'e', 'n');
  multiplyTreeWithEta (tree_g_abcdefmjkl    , 'i', 'n');
  multiplyTreeWithEta (tree_g_abcdefmlij    , 'k', 'n');
  multiplyTreeWithEta (tree_g_amcdefijkl    , 'b', 'n');

  exchangeSymmetrizeTree (tree_g_abmdefijkl, {{'c', 'd'}, {'d', 'c'}}, -1);
  exchangeSymmetrizeTree (tree_g_abmfcdijkl, {{'e', 'f'}, {'f', 'e'}}, -1);
  exchangeSymmetrizeTree (tree_g_abcdefmjkl, {{'i', 'j'}, {'j', 'i'}}, -1);
  exchangeSymmetrizeTree (tree_g_abcdefmlij, {{'k', 'l'}, {'l', 'k'}}, -1);
  exchangeSymmetrizeTree (tree_g_amcdefijkl, {{'a', 'b'}, {'b', 'a'}},  1);

  std::swap (tree_eq_abcdefijklmn, tree_g_abcdefijklmqns);
  mergeTrees (tree_eq_abcdefijklmn, tree_g_abmdefijkl);
  mergeTrees (tree_eq_abcdefijklmn, tree_g_abmfcdijkl);
  mergeTrees (tree_eq_abcdefijklmn, tree_g_abcdefmjkl);
  mergeTrees (tree_eq_abcdefijklmn, tree_g_abcdefmlij);
  mergeTrees (tree_eq_abcdefijklmn, tree_g_amcdefijkl);

  return std::move(tree_eq_abcdefijklmn);
}

int main () {
  
  auto tree_eq_1st = firstOrderEquation();
  auto tree_eq_2nd = secondOrderEquation();
  auto tree_eq_3rd = thirdOrderEquation();

  auto eval_fun_1st = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {

    for (char i_a = 0; i_a < 4; ++i_a) {
      for (char i_b = 0; i_b < 4; ++i_b) {
        for (char i_m = 0; i_m < 4; ++i_m) {
          for (char i_n = 0; i_n < 4; ++i_n) {
            std::map<char, char> eval_map {
              {'a', i_a},
              {'b', i_b},
              {'m', i_m},
              {'n', i_n},
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
          }
        }
      }
    }

  };

  auto eval_fun_2nd = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};

    do {
    for (char i_a = 0; i_a < 4; ++i_a) {
      for (char i_b = 0; i_b < 4; ++i_b) {
        for (char i_m = 0; i_m < 4; ++i_m) {
          for (char i_n = 0; i_n < 4; ++i_n) {
            std::map<char, char> eval_map {
              {'a', i_a},
              {'b', i_b},
              {'m', i_m},
              {'n', i_n},
              {'c', area_indices.first.first},
              {'d', area_indices.first.second},
              {'e', area_indices.second.first},
              {'f', area_indices.second.second}
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
          }
        }
      }
    }
    } while (nextAreaIndices(area_indices));

  };

  auto eval_fun_3rd = [] (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) -> void {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};

    do {
    do {
    for (char i_a = 0; i_a < 4; ++i_a) {
      for (char i_b = 0; i_b < 4; ++i_b) {
        for (char i_m = 0; i_m < 4; ++i_m) {
          for (char i_n = 0; i_n < 4; ++i_n) {
            std::map<char, char> eval_map {
              {'a', i_a},
              {'b', i_b},
              {'m', i_m},
              {'n', i_n},
              {'c', area_indices_1.first.first},
              {'d', area_indices_1.first.second},
              {'e', area_indices_1.second.first},
              {'f', area_indices_1.second.second},
              {'i', area_indices_2.first.first},
              {'j', area_indices_2.first.second},
              {'k', area_indices_2.second.first},
              {'l', area_indices_2.second.second}
            };
            auto eval_res_tmp = evaluateTree (t, eval_map);
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move(eval_res_tmp));
            }
          }
        }
      }
    }
    } while (nextAreaIndices(area_indices_1));
    } while (nextAreaIndices(area_indices_2));

  };

  solveNumerical ({
      {tree_eq_1st, eval_fun_1st},
      {tree_eq_2nd, eval_fun_2nd},
//      {tree_eq_3rd, eval_fun_3rd}
      });

}
