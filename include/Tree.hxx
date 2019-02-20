#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <deque>

struct Tree;

using Forest = std::vector<Tree *>;

struct Tree {
  void * node;
  Tree * parent;
  Forest forest;

  Tree () = delete;
  Tree (int size) : forest(Forest(size)) {}
};
