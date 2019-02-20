#include <vector>

#include "Tree.hxx"
#include "Tensor.hxx"
#include "Eta.hxx"
#include "Epsilon.hxx"
#include "Scalar.hxx"
#include "Builder.hxx"

constexpr size_t eta_size = 2;
constexpr size_t epsilon_size = 4;

std::vector<std::string> EtaBuilder::pickEta (std::string const & indices) {
  size_t len = indices.length();
  std::vector<std::string> ret;
  ret.reserve(len - 1);

  for (size_t counter = 1; counter < len; ++counter) {
    std::string tmp;
    tmp.reserve (len);
    tmp.push_back(indices[0]);
    tmp.push_back(indices[counter]);

    for (size_t counter_2 = 1; counter_2 < counter; ++counter_2) {
      tmp.push_back(indices[counter_2]);
    }

    for (size_t counter_2 = counter + 1; counter_2 < len; ++counter_2) {
      tmp.push_back(indices[counter_2]);
    }

    ret.push_back(tmp);
  }

  return ret;
}

std::vector<std::string> EpsilonBuilder::pickEpsilon (std::string const & indices) {
  size_t len = indices.length();
  std::vector<std::string> ret;
  ret.reserve((len * (len - 1) * (len - 2) * (len - 3)) / 24);

  for (size_t counter_1 = 0; counter_1 < len - 3; ++counter_1) {
    for (size_t counter_2 = counter_1 + 1; counter_2 < len - 2; ++counter_2) {
      for (size_t counter_3 = counter_2 + 1; counter_3 < len - 1; ++counter_3) {
        for (size_t counter_4 = counter_3 + 1; counter_4 < len; ++counter_4) {
          std::string tmp;
          tmp.reserve (len);
          tmp.push_back(indices[counter_1]);
          tmp.push_back(indices[counter_2]);
          tmp.push_back(indices[counter_3]);
          tmp.push_back(indices[counter_4]);
          for (size_t counter_5 = 0; counter_5 < len; ++counter_5) {
            if (counter_5 != counter_1 && counter_5 != counter_2 && counter_5 != counter_3 && counter_5 != counter_4) {
              tmp.push_back(indices[counter_5]);
            }
          }

          ret.push_back(tmp);
        }
      }
    }
  }
  
  return ret;
}

Tree * EtaBuilder::buildTree (std::string const & indices, Tree * parent) {
  int order = indices.size();
  if (order == 0) {
    auto tree = new Tree (1);
    tree->parent = parent;
    tree->forest.back() = new Tree(0);
    tree->forest.back()->node = static_cast<void *>(new int ((-1) * (++leaf_counter)));
    tree->forest.back()->parent = tree;
    return tree;
  } else {
    auto strings = pickEta (indices);
    auto tree = new Tree (order - 1);
    tree->parent = parent;
    std::transform (strings.cbegin(), strings.cend(), tree->forest.begin(),
      [this,&tree] (auto const & str) {
        auto subtree = buildTree (str.substr(eta_size), tree);
        subtree->node = static_cast<void *>(new int(1));//new Eta (str[0], str[1]);
        return subtree;
      });
    return tree;
  }
}

Tree * EpsilonBuilder::buildTree (std::string const & indices, Tree * parent) {
  int order = indices.size();
  int trees = (order * (order - 1) * (order - 2) * (order - 3)) / 24;
  auto tree = new Tree (trees);

  if (order > 3) {
  auto strings = pickEpsilon (indices);
  tree->parent = parent;
  tree->forest = Forest (trees);
  std::transform (strings.cbegin(), strings.cend(), tree->forest.begin(),
    [this,&tree] (auto const & str) {
      EtaBuilder etaBuilder (leaf_counter);
      auto subtree = etaBuilder.buildTree (str.substr(epsilon_size), tree);
      subtree->node = static_cast<void *>(new int(2));//new Epsilon (str[0], str[1], str[2], str[3]);
      leaf_counter = etaBuilder.get_leaf_counter();
      return subtree;
    });
  }
  return tree;
}

Tree * buildEpsilonEtaTree (std::string const & indices) {
  auto ret = new Tree (0);

  EpsilonBuilder epsilonBuilder;
  auto epsilonTree = epsilonBuilder.buildTree(indices);

  EtaBuilder etaBuilder(epsilonBuilder.get_leaf_counter());
  auto etaTree = etaBuilder.buildTree(indices);

  ret->forest.reserve(epsilonBuilder.get_leaf_counter() + etaBuilder.get_leaf_counter());

  std::for_each(epsilonTree->forest.begin(), epsilonTree->forest.end(),
    [&ret] (auto & t) {
      t->parent = ret;
      ret->forest.push_back(t);
    });
  std::for_each(etaTree->forest.begin(), etaTree->forest.end(),
    [&ret] (auto & t) {
      t->parent = ret;
      ret->forest.push_back(t);
    });

  return ret;
}
