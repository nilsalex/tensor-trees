#pragma once
#include <algorithm>
#include <memory>
#include <vector>

template <class T>
struct Tree;

template <class T>
using Forest = std::vector<std::unique_ptr<Tree<T>>>;

template <class T>
struct Tree {
  std::unique_ptr<T> node;
  Forest<T> forest;

  Tree () : node(), forest() {}

  Tree (size_t order) : node(), forest() {
    forest.reserve(order);
  }
};
