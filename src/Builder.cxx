#include <algorithm>

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

Forest<Node> EtaBuilder::buildForest (std::string const & indices) {
  int order = indices.size();
  if (order == 0) {
    auto forest = Forest<Node> (1);
    forest[0] = std::make_unique<Tree<Node>>(0);
    forest[0]->node = std::make_unique<Scalar>(++leaf_counter, 1);
    return forest;
  } else {
    auto strings = pickEta (indices);
    auto forest = Forest<Node> (order - 1);
    std::transform (strings.cbegin(), strings.cend(), forest.begin(),
      [this] (auto const & str) {
        auto tree = std::make_unique<Tree<Node>>(); 
        tree->node = std::make_unique<Eta>(str[0], str[1]);
        tree->forest = buildForest (str.substr(eta_size));
        return tree;
      });
    return forest;
  }
}

Forest<Node> EpsilonBuilder::buildForest (std::string const & indices) {
  int order = indices.size();

  int trees = (order * (order - 1) * (order - 2) * (order - 3)) / 24;
  auto strings = pickEpsilon (indices);
  auto forest = Forest<Node> (trees);
  std::transform (strings.cbegin(), strings.cend(), forest.begin(),
    [this] (auto const & str) {
      auto tree = std::make_unique<Tree<Node>>();
      tree->node = std::make_unique<Epsilon>(str[0], str[1], str[2], str[3]);
      EtaBuilder etaBuilder (leaf_counter);
      tree->forest = etaBuilder.buildForest(str.substr(epsilon_size));
      leaf_counter = etaBuilder.get_leaf_counter();
      return tree;
    });
  return forest;
}

std::unique_ptr<Forest<Node>> buildEpsilonEtaForest (std::string const & indices) {
  auto ret = std::make_unique<Forest<Node>>();

  EtaBuilder etaBuilder;
  auto etaForest = etaBuilder.buildForest(indices);

  EpsilonBuilder epsilonBuilder(etaBuilder.get_leaf_counter());
  auto epsilonForest = epsilonBuilder.buildForest(indices);

  ret->reserve(etaForest.size() + epsilonForest.size());
  std::for_each(etaForest.begin(), etaForest.end(),
    [&ret] (auto & t) {
      ret->push_back(std::make_unique<Tree<Node>>());
      ret->back().swap(t);
    });
  std::for_each(epsilonForest.begin(), epsilonForest.end(),
    [&ret] (auto & t) {
      ret->push_back(std::make_unique<Tree<Node>>());
      ret->back().swap(t);
    });

  return ret;
}
