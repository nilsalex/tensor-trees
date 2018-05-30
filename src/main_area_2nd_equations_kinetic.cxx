#include <iostream>

#include "Algorithms.hxx"
#include "Indices.hxx"
  
std::unique_ptr<Tree<Node>> firstOrderEquation () {
  auto tree_M_abcdmjnkpq = loadTree ("area_1st_kinetic.prs");  // load ansatz tree for kinetic coefficients of first order
                                                               // indices are abcd efgh pq
  auto tree_M_abcdmjklpq = copyTree (tree_M_abcdmjnkpq);         // copy ansatz tree (first tree contracted with eta, second with epsilon)

  exchangeTensorIndices (tree_M_abcdmjnkpq, {{'e', 'm'}, {'f', 'j'}, {'g', 'n'}, {'h', 'k'}});  // set correct indices for first tree
  exchangeTensorIndices (tree_M_abcdmjklpq, {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});  // set correct indices for second tree

  multiplyTree (tree_M_abcdmjnkpq,  2);  // set prefactor of first term
  multiplyTree (tree_M_abcdmjklpq, -1);  // set prefactor of second term

  contractTreeWithEta      (tree_M_abcdmjnkpq, 'j', 'k');           // contraction of first term with eta_{jk}
  contractTreeWithEpsilon3 (tree_M_abcdmjklpq, 'n', 'j', 'k', 'l'); // contraction of second term with epsilon^{n}_{jkl}

  mergeTrees (tree_M_abcdmjnkpq, tree_M_abcdmjklpq);  // add both terms
  tree_M_abcdmjklpq.reset(); // clear unused second tree

  multiExchangeSymmetrizeTree (tree_M_abcdmjnkpq,   // apply symmetry (pqm)
        {{{{'p', 'q'}, {'q', 'p'}}, 1},
         {{{'m', 'p'}, {'p', 'm'}}, 1},
         {{{'m', 'p'}, {'p', 'q'}, {'q', 'm'}}, 1},
         {{{'m', 'q'}, {'p', 'm'}, {'q', 'p'}}, 1},
         {{{'m', 'q'}, {'q', 'm'}}, 1}} );

  return std::move (tree_M_abcdmjnkpq);
}

std::unique_ptr<Tree<Node>> secondOrderEquation_1 () {
  auto tree_M_abcdmjklpq = loadTree ("area_1st_kinetic.prs");  // load ansatz tree for kinetic coefficients of first order
                                                               // indices are abcd efgh pq
  size_t first_order_var_num = getVariableSet (tree_M_abcdmjklpq).size();
  exchangeTensorIndices (tree_M_abcdmjklpq, {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}}); // set correct indices for first tree

  multiplyTreeWithEta (tree_M_abcdmjklpq, 'i', 'n'); // multiply first term with eta^{i n}

  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'i', 'j'}, {'j', 'i'}}, -1);
  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'k', 'l'}, {'l', 'k'}}, -1);                        // apply area metric symmetries to ijkl
  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1);

  auto tree_Q_abcdmfnhijklpq = loadTree ("area_2nd_kinetic.prs");  // load ansatz tree for kinetic coefficients of second order
                                                                   // indices are abcd efgh ijkl pq
  shiftVariables (tree_Q_abcdmfnhijklpq, first_order_var_num);
  auto tree_Q_abcdmfghijklpq = copyTree (tree_Q_abcdmfnhijklpq);

  exchangeTensorIndices (tree_Q_abcdmfnhijklpq, {{'e', 'm'}, {'g', 'n'}});  // set correct indices for first tree
  exchangeTensorIndices (tree_Q_abcdmfghijklpq, {{'e', 'm'}});              // set correct indices for first tree

  multiplyTree (tree_Q_abcdmfnhijklpq,  2);  // set correct prefactors
  multiplyTree (tree_Q_abcdmfghijklpq, -1);

  contractTreeWithEta      (tree_Q_abcdmfnhijklpq, 'f', 'h');            // apply contractions
  contractTreeWithEpsilon3 (tree_Q_abcdmfghijklpq, 'n', 'f', 'g', 'h');

  mergeTrees (tree_M_abcdmjklpq, tree_Q_abcdmfnhijklpq);
  mergeTrees (tree_M_abcdmjklpq, tree_Q_abcdmfghijklpq);
  tree_Q_abcdmfnhijklpq.reset();
  tree_Q_abcdmfghijklpq.reset();
 
  multiExchangeSymmetrizeTree (tree_M_abcdmjklpq,   // apply symmetry (pqm)
        {{{{'p', 'q'}, {'q', 'p'}}, 1},
         {{{'m', 'p'}, {'p', 'm'}}, 1},
         {{{'m', 'p'}, {'p', 'q'}, {'q', 'm'}}, 1},
         {{{'m', 'q'}, {'p', 'm'}, {'q', 'p'}}, 1},
         {{{'m', 'q'}, {'q', 'm'}}, 1}} );

  return std::move (tree_M_abcdmjklpq);
}

std::unique_ptr<Tree<Node>> secondOrderEquation_2 () {
  auto tree_M_abcdijklpm = loadTree ("area_1st_kinetic.prs");  // load ansatz tree for kinetic coefficients of first order
                                                               // indices are abcd efgh pq
  auto tree_M_abcdmjklpq = copyTree (tree_M_abcdijklpm);
  size_t first_order_var_num = getVariableSet (tree_M_abcdijklpm).size();
  exchangeTensorIndices (tree_M_abcdijklpm, {{'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}, {'q', 'm'}}); // set correct indices for first tree
  exchangeTensorIndices (tree_M_abcdmjklpq, {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});

  multiplyTree (tree_M_abcdijklpm, -1);
  multiplyTree (tree_M_abcdmjklpq, -8);

  multiplyTreeWithEta (tree_M_abcdijklpm, 'q', 'n');
  multiplyTreeWithEta (tree_M_abcdmjklpq, 'i', 'n');

  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'i', 'j'}, {'j', 'i'}}, -1);
  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'k', 'l'}, {'l', 'k'}}, -1);                        // apply area metric symmetries to ijkl
  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1);

  mergeTrees (tree_M_abcdijklpm, tree_M_abcdmjklpq);
  tree_M_abcdmjklpq.reset();

  auto tree_sum = std::make_unique<Tree<Node>>();
  auto tree_Q_mfghijklabcdpq = loadTree ("area_2nd_kinetic.prs");  // load ansatz tree for kinetic coefficients of second order
                                                                   // indices are abcd efgh ijkl pq
  shiftVariables (tree_Q_mfghijklabcdpq, first_order_var_num);
  auto tree_Q_mfghabcdijklpq = copyTree (tree_Q_mfghijklabcdpq);
  auto tree_Q_abcdijklmfghpq = copyTree (tree_Q_mfghijklabcdpq);

  exchangeTensorIndices (tree_Q_mfghijklabcdpq, {{'a', 'm'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'},
                                                 {'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'},
                                                 {'i', 'a'}, {'j', 'b'}, {'k', 'c'}, {'l', 'd'}});
  exchangeTensorIndices (tree_Q_mfghabcdijklpq, {{'a', 'm'}, {'b', 'f'}, {'c', 'g'}, {'d', 'h'},
                                                 {'e', 'a'}, {'f', 'b'}, {'g', 'c'}, {'h', 'd'}});
  exchangeTensorIndices (tree_Q_abcdijklmfghpq, {{'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'},
                                                 {'i', 'm'}, {'j', 'f'}, {'k', 'g'}, {'l', 'h'}});
  multiplyTree (tree_Q_mfghabcdijklpq, -1);
  multiplyTree (tree_Q_abcdijklmfghpq, -1);
  mergeTrees (tree_sum, tree_Q_mfghijklabcdpq);
  mergeTrees (tree_sum, tree_Q_mfghabcdijklpq);
  mergeTrees (tree_sum, tree_Q_abcdijklmfghpq);
  tree_Q_mfghijklabcdpq.reset();
  tree_Q_mfghabcdijklpq.reset();
  tree_Q_abcdijklmfghpq.reset();

  auto tree_sum_2 = copyTree (tree_sum);
  exchangeTensorIndices (tree_sum, {{'g', 'n'}});

  multiplyTree (tree_sum, 8);
  multiplyTree (tree_sum_2, -4);

  contractTreeWithEta (tree_sum, 'f', 'h');
  contractTreeWithEpsilon3 (tree_sum_2, 'n', 'f', 'g', 'h');

  mergeTrees (tree_M_abcdijklpm, tree_sum);
  mergeTrees (tree_M_abcdijklpm, tree_sum_2);

  tree_sum.reset();
  tree_sum_2.reset();

  exchangeSymmetrizeTree (tree_M_abcdijklpm, {{'p', 'q'}, {'q', 'p'}}, 1);

  return std::move (tree_M_abcdijklpm);
}

std::unique_ptr<Tree<Node>> secondOrderEquation_3 () {
  auto tree_eqn = std::make_unique<Tree<Node>> ();

  auto tree_M_abcdijklmq = loadTree ("area_1st_kinetic.prs");
  size_t first_order_var_num = getVariableSet(tree_M_abcdijklmq).size();

  auto tree_M_abcdmjklpq = copyTree (tree_M_abcdijklmq);
  auto tree_M_mbcdijklpq = copyTree (tree_M_abcdijklmq);
  auto tree_M_abcdijklpq = copyTree (tree_M_abcdijklmq);

  exchangeTensorIndices (tree_M_abcdijklmq, {{'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}, {'p', 'm'}});
  exchangeTensorIndices (tree_M_abcdmjklpq, {{'e', 'm'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});
  exchangeTensorIndices (tree_M_mbcdijklpq, {{'a', 'm'}, {'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});
  exchangeTensorIndices (tree_M_abcdijklpq, {{'e', 'i'}, {'f', 'j'}, {'g', 'k'}, {'h', 'l'}});

  multiplyTree (tree_M_abcdijklmq, 2);
  multiplyTree (tree_M_abcdmjklpq, 4);
  multiplyTree (tree_M_mbcdijklpq, 4);
  multiplyTree (tree_M_abcdijklpq, -1);

  multiplyTreeWithEta (tree_M_abcdijklmq, 'p', 'n');
  multiplyTreeWithEta (tree_M_abcdmjklpq, 'i', 'n');
  multiplyTreeWithEta (tree_M_mbcdijklpq, 'a', 'n');
  multiplyTreeWithEta (tree_M_abcdijklpq, 'm', 'n');

  exchangeSymmetrizeTree (tree_M_abcdijklmq, {{'p', 'q'}, {'q', 'p'}}, 1);

  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'i', 'j'}, {'j', 'i'}}, -1);
  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'k', 'l'}, {'l', 'k'}}, -1);
  exchangeSymmetrizeTree (tree_M_abcdmjklpq, {{'i', 'k'}, {'j', 'l'}, {'k', 'i'}, {'l', 'j'}}, 1);

  exchangeSymmetrizeTree (tree_M_mbcdijklpq, {{'a', 'b'}, {'b', 'a'}}, -1);
  exchangeSymmetrizeTree (tree_M_mbcdijklpq, {{'c', 'd'}, {'d', 'c'}}, -1);
  exchangeSymmetrizeTree (tree_M_mbcdijklpq, {{'a', 'c'}, {'b', 'd'}, {'c', 'a'}, {'d', 'b'}}, 1);

  mergeTrees (tree_eqn, tree_M_abcdijklmq);
  mergeTrees (tree_eqn, tree_M_abcdmjklpq);
  mergeTrees (tree_eqn, tree_M_mbcdijklpq);
  mergeTrees (tree_eqn, tree_M_abcdijklpq);

  tree_M_abcdijklmq.reset();
  tree_M_abcdmjklpq.reset();
  tree_M_mbcdijklpq.reset();
  tree_M_abcdijklpq.reset();

  auto tree_Q_abcdmfnhijklpq = loadTree ("area_2nd_kinetic.prs");
  shiftVariables (tree_Q_abcdmfnhijklpq, first_order_var_num);
  auto tree_Q_abcdmfghijklpq = copyTree (tree_Q_abcdmfnhijklpq);

  exchangeTensorIndices (tree_Q_abcdmfnhijklpq, {{'e', 'm'}, {'g', 'n'}});
  exchangeTensorIndices (tree_Q_abcdmfghijklpq, {{'e', 'm'}});

  multiplyTree (tree_Q_abcdmfnhijklpq, 8);
  multiplyTree (tree_Q_abcdmfghijklpq, -4);

  contractTreeWithEta (tree_Q_abcdmfnhijklpq, 'f', 'h');
  contractTreeWithEpsilon3 (tree_Q_abcdmfghijklpq, 'n', 'f', 'g', 'h');

  mergeTrees (tree_eqn, tree_Q_abcdmfnhijklpq);
  mergeTrees (tree_eqn, tree_Q_abcdmfghijklpq);

  return std::move (tree_eqn);
}

void eval_fun_1st (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) {
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};
  std::pair<char, char> indices_pq {0, 0};

  do {
    do {
      for (char i_m = 0; i_m < 4; ++i_m) {
        for (char i_n = 0; i_n < 4; ++i_n) {
          auto eval_res_tmp = evaluateTree (t, {
                {'a', area_indices.first.first},
                {'b', area_indices.first.second},
                {'c', area_indices.second.first},
                {'d', area_indices.second.second},
                {'p', indices_pq.first},
                {'q', indices_pq.second},
                {'m', i_m},
                {'n', i_n}
              });
          if (!eval_res_tmp.empty()) {
            eval_res_set.insert (std::move (eval_res_tmp));
          }
        }
      }
    } while (nextAreaIndices (area_indices));
  } while (nextIndexPairSymmetric (indices_pq));
}

void eval_fun_2nd (std::unique_ptr<Tree<Node>> const & t, std::set<std::map<size_t, mpq_class>> & eval_res_set) {
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
  std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};
  std::pair<char, char> indices_pq {0, 0};

  do {
    do {
      do {
        for (char i_m = 0; i_m < 4; ++i_m) {
          for (char i_n = 0; i_n < 4; ++i_n) {
            auto eval_res_tmp = evaluateTree (t, {
                  {'a', area_indices_1.first.first},
                  {'b', area_indices_1.first.second},
                  {'c', area_indices_1.second.first},
                  {'d', area_indices_1.second.second},
                  {'i', area_indices_2.first.first},
                  {'j', area_indices_2.first.second},
                  {'k', area_indices_2.second.first},
                  {'l', area_indices_2.second.second},
                  {'p', indices_pq.first},
                  {'q', indices_pq.second},
                  {'m', i_m},
                  {'n', i_n}
                });
            if (!eval_res_tmp.empty()) {
              eval_res_set.insert (std::move (eval_res_tmp));
            }
          }
        }
      } while (nextAreaIndices (area_indices_1));
    } while (nextAreaIndices (area_indices_2));
   } while (nextIndexPairSymmetric (indices_pq));
}

int main () {

  auto tree_eq_1st = firstOrderEquation ();

  auto tree_eq_2nd_1 = secondOrderEquation_1 ();
  auto tree_eq_2nd_2 = secondOrderEquation_2 ();
  auto tree_eq_2nd_3 = secondOrderEquation_3 ();

  solveNumerical ({
      {tree_eq_1st,   eval_fun_1st},
      {tree_eq_2nd_1, eval_fun_2nd},
      {tree_eq_2nd_2, eval_fun_2nd},
      {tree_eq_2nd_3, eval_fun_2nd}
      });

  return 0;
}
