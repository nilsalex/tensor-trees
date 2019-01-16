#include <iostream>

#include "Algorithms.hxx"
#include "Indices.hxx"

int main() {
  auto tree_A = loadTree ("ansatz_A.prs");
  auto tree_AI = loadTree ("ansatz_AI.prs");
  auto tree_AB = loadTree ("ansatz_AB.prs");
  auto tree_ABI = loadTree ("ansatz_ABI.prs");
  auto tree_ApBq = loadTree ("ansatz_ApBq.prs");
  auto tree_AIBJ = loadTree ("ansatz_AIBJ.prs");
  int dof_A = getVariableSet(tree_A).size();
  int dof_AI = getVariableSet(tree_AI).size();
  int dof_AB = getVariableSet(tree_AB).size();
  int dof_ABI = getVariableSet(tree_ABI).size();
  int dof_ApBq = getVariableSet(tree_ApBq).size();
//  int dof_AIBJ = getVariableSet(tree_AIBJ).size();

  shiftVariables(tree_A, 1);
  shiftVariables(tree_AI, 1 + dof_A);
  shiftVariables(tree_AB, 1 + dof_A + dof_AI);
  shiftVariables(tree_ABI, 1 + dof_A + dof_AI + dof_AB);
  shiftVariables(tree_ApBq, 1 + dof_A + dof_AI + dof_AB + dof_ABI);
  shiftVariables(tree_AIBJ, 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq);

//  int dof_total = 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ;

  /*
  std::cout << dof_A << std::endl;
  std::cout << dof_AI << std::endl;
  std::cout << dof_AB << std::endl;
  std::cout << dof_ABI << std::endl;
  std::cout << dof_ApBq << std::endl;
  std::cout << dof_AIBJ << std::endl;

  std::cout << dof_total << std::endl;
  */

  std::map<std::vector<std::vector<int>>, std::map<size_t, mpq_class>> dep_dof;

  dep_dof[{}] = std::map<size_t, mpq_class>({{1, mpq_class(1, 1)}});

  {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};

    do {
      int a = area_indices.first.first;
      int b = area_indices.first.second;
      int c = area_indices.second.first;
      int d = area_indices.second.second;

      std::map<char, char> eval_map {
        {'a', a},
        {'b', b},
        {'c', c},
        {'d', d}
      };
      auto eval_res_temp = evaluateTree (tree_A, eval_map);
      dep_dof[{{a, b, c, d}}] = std::move(eval_res_temp);
    } while (nextAreaIndices(area_indices));
  }

  {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices {0, 0};

    do {
      do {
         int a = area_indices.first.first;
         int b = area_indices.first.second;
         int c = area_indices.second.first;
         int d = area_indices.second.second;
         int i = metric_indices.first;
         int j = metric_indices.second;

        std::map<char, char> eval_map {
          {'a', a},
          {'b', b},
          {'c', c},
          {'d', d},
          {'i', i},
          {'j', j}
        };
        auto eval_res_temp = evaluateTree (tree_AI, eval_map);
        dep_dof[{{a, b, c, d}, {i, j}}] = std::move(eval_res_temp);
      } while (nextIndexPairSymmetric(metric_indices));
    } while (nextAreaIndices(area_indices));
  }

  {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};

    do {
      auto area_indices_2 = area_indices_1;
      do {
          int a = area_indices_1.first.first;
          int b = area_indices_1.first.second;
          int c = area_indices_1.second.first;
          int d = area_indices_1.second.second;
          int e = area_indices_2.first.first;
          int f = area_indices_2.first.second;
          int g = area_indices_2.second.first;
          int h = area_indices_2.second.second;

        std::map<char, char> eval_map {
          {'a', a},
          {'b', b},
          {'c', c},
          {'d', d},
          {'e', e},
          {'f', f},
          {'g', g},
          {'h', h}
        };
        auto eval_res_temp = evaluateTree (tree_AB, eval_map);
        dep_dof[{{a, b, c, d}, {e, f, g, h}}] = std::move(eval_res_temp);
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
  }

  {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    auto area_indices_2 = area_indices_1;
    std::pair<char, char> metric_indices = {0, 0};

    do {
      do {
        do {
            int a = area_indices_1.first.first;
            int b = area_indices_1.first.second;
            int c = area_indices_1.second.first;
            int d = area_indices_1.second.second;
            int e = area_indices_2.first.first;
            int f = area_indices_2.first.second;
            int g = area_indices_2.second.first;
            int h = area_indices_2.second.second;
            int i = metric_indices.first;
            int j = metric_indices.second;

          std::map<char, char> eval_map {
            {'a', a},
            {'b', b},
            {'c', c},
            {'d', d},
            {'e', e},
            {'f', f},
            {'g', g},
            {'h', h},
            {'i', i},
            {'j', j}
          };
          auto eval_res_temp = evaluateTree (tree_ABI, eval_map);
          dep_dof[{{a, b, c, d}, {e, f, g, h}, {i, j}}] = std::move(eval_res_temp);
        } while (nextIndexPairSymmetric(metric_indices));
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
  }

  {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};

    do {
      for (char p = 0; p < 4; ++p) { 
        auto area_indices_2 = area_indices_1;
        do {
          for (char q = 0; q < 4; ++q) {
            if (area_indices_1 == area_indices_2 && q < p) {
              continue;
            }
            int a = area_indices_1.first.first;
            int b = area_indices_1.first.second;
            int c = area_indices_1.second.first;
            int d = area_indices_1.second.second;
            int e = area_indices_2.first.first;
            int f = area_indices_2.first.second;
            int g = area_indices_2.second.first;
            int h = area_indices_2.second.second;

            std::map<char, char> eval_map {
              {'a', a},
              {'b', b},
              {'c', c},
              {'d', d},
              {'e', e},
              {'f', f},
              {'g', g},
              {'h', h},
              {'p', p},
              {'q', q}
            };
            auto eval_res_temp = evaluateTree (tree_ApBq, eval_map);
            dep_dof[{{a, b, c, d}, {e, f, g, h}, {p}, {q}}] = std::move(eval_res_temp);
          }
        } while (nextAreaIndices(area_indices_2));
      }
    } while (nextAreaIndices(area_indices_1));
  }

  {
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices_1 {0, 0};

    do {
      do {
        auto area_indices_2 = area_indices_1;
        auto metric_indices_2 = metric_indices_1;
        do {
          do {
              int a = area_indices_1.first.first;
              int b = area_indices_1.first.second;
              int c = area_indices_1.second.first;
              int d = area_indices_1.second.second;
              int e = area_indices_2.first.first;
              int f = area_indices_2.first.second;
              int g = area_indices_2.second.first;
              int h = area_indices_2.second.second;
              int i = metric_indices_1.first;
              int j = metric_indices_1.second;
              int k = metric_indices_2.first;
              int l = metric_indices_2.second;

            std::map<char, char> eval_map {
              {'a', a},
              {'b', b},
              {'c', c},
              {'d', d},
              {'e', e},
              {'f', f},
              {'g', g},
              {'h', h},
              {'i', i},
              {'j', j},
              {'k', k},
              {'l', l}
            };
            auto eval_res_temp = evaluateTree (tree_AIBJ, eval_map);
            dep_dof[{{a, b, c, d}, {e, f, g, h}, {i, j}, {k, l}}] = std::move(eval_res_temp);
          } while (nextIndexPairSymmetric(metric_indices_2));
        } while (nextAreaIndices(area_indices_2));
      } while (nextIndexPairSymmetric(metric_indices_1));
    } while (nextAreaIndices(area_indices_1));
  }

  std::cout << "[";
  std::for_each(dep_dof.cbegin(), dep_dof.cend(),
      [first=true] (auto const & p) mutable {
        auto indices = p.first;
        auto values = p.second;

        if (!first) std::cout << ",";
        first = false;

        std::cout << "([";
        std::for_each(indices.cbegin(), indices.cend(),
            [first=true] (auto const & ix) mutable {
            if (!first) std::cout << ",";
            first = false;
            std::cout << "[";
            std::for_each(ix.cbegin(), ix.cend(),
                [first=true] (auto const & i) mutable {
                  if (!first) std::cout << ",";
                  first = false;
                  std::cout << i;
                });
            std::cout << "]";
            });

        std::cout << "],[";

        std::for_each (values.cbegin(), values.cend(),
            [first=true] (auto const & q) mutable {
              if (!first) std::cout << ",";
              first = false;

              std::cout << "(";
              std::cout << q.first;
              std::cout << ",";
              std::cout << q.second.get_num();
              std::cout << "%";
              std::cout << q.second.get_den();
              std::cout << ")";
            });

        std::cout << "])";
      });
  std::cout << "]";

  return 0;
}
