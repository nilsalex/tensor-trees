#include <algorithm>
#include "Algorithms.hxx"

#include "Tensor.hxx"

Tensor::Tensor(size_t const rank, std::string const & indices) : rank(rank), indices(indices) { }

size_t Tensor::get_rank() const { return rank; }
std::string const & Tensor::get_indices() const { return indices; }

void Tensor::multiplyForest (Forest<Node> & forest, mpq_class factor) {
  ::multiplyForest(forest, factor);
}

int Tensor::sortIndices() {
  int ret = 1;
  bool swapped = true;
  do {
    swapped = false;
    for (size_t counter = 1; counter < indices.size(); ++counter) {
      if (indices[counter - 1] == indices[counter]) {
        ret = 0;
      } else if (indices[counter - 1] > indices[counter]) {
        std::swap(indices[counter - 1], indices[counter]);
        swapped = true;
        ret *= -1;
      }
    }
  } while (swapped);

  return ret;
}
void Tensor::exchangeTensorIndices (Forest<Node> & forest, std::map<char, char> const & exchange_map) {
  std::for_each(indices.begin(), indices.end(),
    [&exchange_map] (auto & i) {
      if (exchange_map.count(i) > 0) {
        i = exchange_map.at(i);
      }
    });
  ::exchangeTensorIndices (forest, exchange_map);
}

Forest<Node> Tensor::splitUnsortedTrees (std::unique_ptr<Tree<Node>> const & tree) const {
  Forest<Node> tmp_forest = ::splitUnsortedTrees (tree->forest);
  Forest<Node> ret;

  if (tree->node->isLeastNode(tmp_forest)) {
    ret.emplace_back(std::make_unique<Tree<Node>>());
    ret[0]->node = tree->node->clone();
    ret[0]->forest = copyForest(tmp_forest);
  } else {
    ret.reserve(tmp_forest.size());

    std::for_each(tmp_forest.cbegin(), tmp_forest.cend(),
      [&tree,&ret] (auto const & t) {
        ret.emplace_back(std::make_unique<Tree<Node>>());
        ret.back()->forest = Forest<Node>();
        ret.back()->forest.emplace_back(std::make_unique<Tree<Node>>());
        ret.back()->forest.back()->forest = copyForest(t->forest);

        if (!(*(tree->node) > t->node.get())) {
          ret.back()->node = tree->node->clone();
          ret.back()->forest.back()->node = t->node->clone();
        } else {
          ret.back()->node = t->node->clone();
          ret.back()->forest.back()->node = tree->node->clone();
        }
      });
  }

  return ret;
}

std::set<size_t> Tensor::getVariableSet (Forest<Node> const & forest) const {
  return ::getVariableSet (forest);
}
