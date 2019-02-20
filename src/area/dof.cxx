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
  auto tree_ABC = loadTree ("ansatz_ABC.prs");
  auto tree_ABCI = loadTree ("ansatz_ABCI.prs");
  auto tree_ABpCq = loadTree ("ansatz_ABpCq.prs");
  auto tree_ABICJ = loadTree ("ansatz_ABICJ.prs");
  auto tree_ApBqCI = loadTree ("ansatz_ApBqCI.prs");
  int dof_A = getVariableSet(tree_A).size();
  int dof_AI = getVariableSet(tree_AI).size();
  int dof_AB = getVariableSet(tree_AB).size();
  int dof_ABI = getVariableSet(tree_ABI).size();
  int dof_ApBq = getVariableSet(tree_ApBq).size();
  int dof_AIBJ = getVariableSet(tree_AIBJ).size();
  int dof_ABC = getVariableSet(tree_ABC).size();
  int dof_ABCI = getVariableSet(tree_ABCI).size();
  int dof_ABpCq = getVariableSet(tree_ABpCq).size();
  int dof_ABICJ = getVariableSet(tree_ABICJ).size();
  int dof_ApBqCI = getVariableSet(tree_ApBqCI).size();

  shiftVariables(tree_A, 1);
  shiftVariables(tree_AI, 1 + dof_A);
  shiftVariables(tree_AB, 1 + dof_A + dof_AI);
  shiftVariables(tree_ABI, 1 + dof_A + dof_AI + dof_AB);
  shiftVariables(tree_ApBq, 1 + dof_A + dof_AI + dof_AB + dof_ABI);
  shiftVariables(tree_AIBJ, 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq);
  shiftVariables(tree_ABC, 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ);
  shiftVariables(tree_ABCI, 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ + dof_ABC);
  shiftVariables(tree_ABpCq, 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ + dof_ABC + dof_ABCI);
  shiftVariables(tree_ABICJ, 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ + dof_ABC + dof_ABCI + dof_ABpCq);
  shiftVariables(tree_ApBqCI, 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ + dof_ABC + dof_ABCI + dof_ABpCq + dof_ABICJ);

//  int dof_total = 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ;

  /*
  std::cout << 1 + dof_A << std::endl;
  std::cout << 1 + dof_A + dof_AI << std::endl;
  std::cout << 1 + dof_A + dof_AI + dof_AB << std::endl;
  std::cout << 1 + dof_A + dof_AI + dof_AB + dof_ABI << std::endl;
  std::cout << 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq << std::endl;
  std::cout << 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ << std::endl;
  std::cout << 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ + dof_ABC << std::endl;
  std::cout << 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ + dof_ABC + dof_ABICJ << std::endl;
  std::cout << 1 + dof_A + dof_AI + dof_AB + dof_ABI + dof_ApBq + dof_AIBJ + dof_ABC + dof_ABICJ + dof_ApBqCI << std::endl;
  */

//  std::cout << dof_total << std::endl;

  std::map<std::vector<std::vector<int>>, std::map<size_t, mpq_class>> dep_dof;

  dep_dof[{}] = std::map<size_t, mpq_class>({{1, mpq_class(1, 1)}});

  std::cerr << std::endl;
  std::cerr << "A" << std::endl;
  {
    int num = 21;
    int counter = 0;
    int progress = 0;
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
      ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
    } while (nextAreaIndices(area_indices));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "AI" << std::endl;
  {
    int num = 210;
    int counter = 0;
    int progress = 0;
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
        ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
      } while (nextIndexPairSymmetric(metric_indices));
    } while (nextAreaIndices(area_indices));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "AB" << std::endl;
  {
    int num = 11*21;
    int counter = 0;
    int progress = 0;
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
        ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "ABI" << std::endl;
  {
    int num = 21 * 210;
    int counter = 0;
    int progress = 0;
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
          ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
        } while (nextIndexPairSymmetric(metric_indices));
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "ApBq" << std::endl;
  {
    int num = 42*85;
    int counter = 0;
    int progress = 0;
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
            ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
          }
        } while (nextAreaIndices(area_indices_2));
      }
    } while (nextAreaIndices(area_indices_1));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "AIBJ" << std::endl;
  {
    int num = 105 * 211;
    int counter = 0;
    int progress = 0;
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
            ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
          } while (nextIndexPairSymmetric(metric_indices_2));
        } while (nextAreaIndices(area_indices_2));
      } while (nextIndexPairSymmetric(metric_indices_1));
    } while (nextAreaIndices(area_indices_1));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "ABC" << std::endl;
  {
    int num = 7*11*23;
    int counter = 0;
    int progress = 0;
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};

    do {
      auto area_indices_2 = area_indices_1;
      do {
        auto area_indices_3 = area_indices_2;
        do {
              int a = area_indices_1.first.first;
              int b = area_indices_1.first.second;
              int c = area_indices_1.second.first;
              int d = area_indices_1.second.second;
              int e = area_indices_2.first.first;
              int f = area_indices_2.first.second;
              int g = area_indices_2.second.first;
              int h = area_indices_2.second.second;
              int i = area_indices_3.first.first;
              int j = area_indices_3.first.second;
              int k = area_indices_3.second.first;
              int l = area_indices_3.second.second;

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
            auto eval_res_temp = evaluateTree (tree_ABC, eval_map);
            dep_dof[{{a, b, c, d}, {e, f, g, h}, {i, j, k, l}}] = std::move(eval_res_temp);
            ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
        } while (nextAreaIndices(area_indices_3));
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "ABCI" << std::endl;
  {
    int num = 11 * 21 * 210;
    int counter = 0;
    int progress = 0;
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_3 {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices {0, 0};
  
    do {
      auto area_indices_2 = area_indices_1;
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
           int i = area_indices_3.first.first;
           int j = area_indices_3.first.second;
           int k = area_indices_3.second.first;
           int l = area_indices_3.second.second;
           int p = metric_indices.first;
           int q = metric_indices.second;

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
              {'l', l},
              {'p', p},
              {'q', q}
            };
            auto eval_res_temp = evaluateTree (tree_ABCI, eval_map);
            dep_dof[{{a, b, c, d}, {e, f, g, h}, {i, j, k, l}, {p, q}}] = std::move(eval_res_temp);
            ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
          } while (nextIndexPairSymmetric(metric_indices));
        } while (nextAreaIndices(area_indices_3));
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
    if (counter != num)
      throw 0;
  }
  
  std::cerr << std::endl;
  std::cerr << "ABpCq" << std::endl;
  {
    int num = 42 * 85 * 21;
    int counter = 0;
    int progress = 0;
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};
  
    do {
      do {
        for (char p = 0; p < 4; ++p) {
          auto area_indices_3 = area_indices_2;
          do {
            for (char q = 0; q < 4; ++q) {
              if (area_indices_3 == area_indices_2 && q < p)
                continue;
             int a = area_indices_1.first.first;
             int b = area_indices_1.first.second;
             int c = area_indices_1.second.first;
             int d = area_indices_1.second.second;
             int e = area_indices_2.first.first;
             int f = area_indices_2.first.second;
             int g = area_indices_2.second.first;
             int h = area_indices_2.second.second;
             int i = area_indices_3.first.first;
             int j = area_indices_3.first.second;
             int k = area_indices_3.second.first;
             int l = area_indices_3.second.second;

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
                {'l', l},
                {'p', p},
                {'q', q}
              };
              auto eval_res_temp = evaluateTree (tree_ABpCq, eval_map);
              dep_dof[{{a, b, c, d}, {e, f, g, h}, {i, j, k, l}, {p}, {q}}] = std::move(eval_res_temp);
              ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
            }
          } while (nextAreaIndices(area_indices_3));
        }
      } while (nextAreaIndices(area_indices_2));
    } while (nextAreaIndices(area_indices_1));
    if (counter != num)
      throw 0;
  }

  std::cerr << std::endl;
  std::cerr << "ABICJ" << std::endl;
  {
    int num = 21*105*211;
    int counter = 0;
    int progress = 0;
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_2 {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices_1 {0, 0};

  do {
  do {
    do {
      auto area_indices_3 = area_indices_2;
      std::pair<char, char> metric_indices_2 {0, 0};
      do {
        do {
          if (area_indices_3 == area_indices_2 && metric_indices_2 < metric_indices_1)
            continue;
              int a = area_indices_1.first.first;
              int b = area_indices_1.first.second;
              int c = area_indices_1.second.first;
              int d = area_indices_1.second.second;
              int e = area_indices_2.first.first;
              int f = area_indices_2.first.second;
              int g = area_indices_2.second.first;
              int h = area_indices_2.second.second;
              int i = area_indices_3.first.first;
              int j = area_indices_3.first.second;
              int k = area_indices_3.second.first;
              int l = area_indices_3.second.second;
              int p = metric_indices_1.first;
              int q = metric_indices_1.second;
              int r = metric_indices_2.first;
              int s = metric_indices_2.second;

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
              {'l', l},
              {'p', p},
              {'q', q},
              {'r', r},
              {'s', s}
            };
            auto eval_res_temp = evaluateTree (tree_ABICJ, eval_map);
            dep_dof[{{a, b, c, d}, {e, f, g, h}, {i, j, k, l}, {p, q}, {r, s}}] = std::move(eval_res_temp);
            ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
        } while (nextIndexPairSymmetric (metric_indices_2));
      } while (nextAreaIndices(area_indices_3));
    } while (nextIndexPairSymmetric (metric_indices_1));
  } while (nextAreaIndices(area_indices_2));
  } while (nextAreaIndices(area_indices_1));
  if (counter != num)
    throw 0;
  }
  
  std::cerr << std::endl;
  std::cerr << "ApBqCI" << std::endl;
  {
    int num = 210*42*85;
    int counter = 0;
    int progress = 0;
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_1 {{0, 1}, {0, 1}};
    std::pair<std::pair<char, char>, std::pair<char, char>> area_indices_3 {{0, 1}, {0, 1}};
    std::pair<char, char> metric_indices {0, 0};

  do {
    for (char p = 0; p < 4; ++p) {
      auto area_indices_2 = area_indices_1;
      do {
        for (char q = 0; q < 4; ++q) {
          if (area_indices_1 == area_indices_2 && q < p)
            continue;

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
              int i = area_indices_3.first.first;
              int j = area_indices_3.first.second;
              int k = area_indices_3.second.first;
              int l = area_indices_3.second.second;
              int r = metric_indices.first;
              int s = metric_indices.second;

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
              {'l', l},
              {'p', p},
              {'q', q},
              {'r', r},
              {'s', s}
            };
            auto eval_res_temp = evaluateTree (tree_ApBqCI, eval_map);
            dep_dof[{{a, b, c, d}, {e, f, g, h}, {i, j, k, l}, {p}, {q}, {r, s}}] = std::move(eval_res_temp);
            ++counter;
          int _progress = (100 * counter) / num;
          if (_progress > progress) {
            progress = _progress;
            std::cerr << "\r";
            std::cerr << "progress : " << progress << " %" << std::flush;
          }
          } while (nextIndexPairSymmetric (metric_indices));
        } while (nextAreaIndices(area_indices_3));
      }
    } while (nextAreaIndices(area_indices_2));
  }
  } while (nextAreaIndices(area_indices_1));
  if (counter != num)
    throw 0;
  }

 // std::cout << "[";
  std::for_each(dep_dof.cbegin(), dep_dof.cend(),
      [] (auto const & p) mutable {
        auto indices = p.first;
        auto values = p.second;

        if (values.empty())
          return;

//        if (!first) std::cout << ",";
//        first = false;

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

        std::cout << "])" << std::endl;
      });
//  std::cout << "]";
  std::cout << std::endl;

  return 0;
}
