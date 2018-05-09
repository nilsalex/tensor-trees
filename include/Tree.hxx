#pragma once
#include <algorithm>
#include <cassert>
#include <memory>
#include <vector>

template <class T>
struct Tree;

template <class T>
using Forest = std::vector<std::unique_ptr<Tree<T>>>;

template <class T>
struct Tree {
  std::unique_ptr<T> node;
  Tree<T> * parent;
  Forest<T> forest;

  Tree () = default;

  Tree (size_t order) : Tree() { forest.reserve(order); };

  bool isValid () const;
  bool isLeaf () const;
  bool isEmpty () const;
  bool isRoot () const;

  Tree<T> * firstChild ();
  Tree<T> const * firstChild () const;

  Tree<T> * firstLeaf ();
  Tree<T> const * firstLeaf () const;

  Tree<T> * lastLeaf ();

  Tree<T> * nextSibling ();
  Tree<T> const * nextSibling () const;

  Tree<T> * firstInNextBranch ();
  Tree<T> const * firstInNextBranch () const;

  Tree<T> * nextLeaf ();
  Tree<T> const * nextLeaf () const;

  std::vector<T *> getBranch ();
  std::vector<T const *> getBranch () const;

//  void insertBranch (std::vector<std::unique_ptr<T>> && nodes, size_t const node_number = 0);
};

template <class T>
bool Tree<T>::isValid () const {
  return std::all_of(forest.cbegin(), forest.cend(),
    [this] (auto const & t) {
      return (this == t->parent && t->isValid());
    });
}

template <class T>
bool Tree<T>::isLeaf () const {
  return forest.empty();
}

template <class T>
bool Tree<T>::isEmpty () const {
  return (node.get() == nullptr);
}

template <class T>
bool Tree<T>::isRoot () const {
  return (parent == nullptr);
}

/*
template <class T>
void Tree<T>::insertBranch (std::vector<std::unique_ptr<T>> && nodes, size_t const node_number) {
  insertBranchIntoForest (forest, std::move(nodes), node_number, parent);
}

template <class T>
void insertBranchIntoForest (Forest<T> & forest, std::vector<std::unique_ptr<T>> && nodes, size_t const node_number, Tree<T> * parent) {
  if (nodes.size() == node_number) {
    return;
  }

  auto node_it = std::find_if(forest.cbegin(), forest.cend(),
    [&nodes,node_number] (auto const & t) {
      return *(nodes[node_number]) == *(t->node);
    });

  if (node_it != forest.cend()) {
    (*node_it)->insertBranch(std::move(nodes), node_number + 1);
  } else {
    forest.push_back(std::make_unique<Tree<T>>());
    forest.back()->node = std::unique_ptr<T>(std::move(nodes[node_number]));
    forest.back()->parent = parent;
    forest.back()->insertBranch(std::move(nodes), node_number + 1);
  }
}
*/

template <class T>
Tree<T> * Tree<T>::firstChild() {
  if (isLeaf()) {
    return nullptr;
  } else {
    return forest.front().get();
  }
}

template <class T>
Tree<T> const * Tree<T>::firstChild() const {
  if (isLeaf()) {
    return nullptr;
  } else {
    return forest.front().get();
  }
}

template <class T>
Tree<T> * Tree<T>::firstLeaf () {
  if (isLeaf()) {
    return this;
  } else {
    return firstChild()->firstLeaf();
  }
}

template <class T>
Tree<T> const * Tree<T>::firstLeaf () const {
  if (isLeaf()) {
    return this;
  } else {
    return firstChild()->firstLeaf();
  }
}

template <class T>
Tree<T> * Tree<T>::nextSibling () {
  if (isRoot()) {
    return nullptr;
  } else {
    auto this_it = std::find_if(parent->forest.begin(), parent->forest.end(),
      [this] (auto & t) {
        return (this == t.get());
      });
    if (++this_it == parent->forest.end()) {
      return nullptr;
    } else {
      return this_it->get();
    }
  }
}

template <class T>
Tree<T> const * Tree<T>::nextSibling () const {
  if (isRoot()) {
    return nullptr;
  } else {
    auto this_it = std::find_if(parent->forest.cbegin(), parent->forest.cend(),
      [this] (auto const & t) {
        return (this == t.get());
      });
    if (++this_it == parent->forest.cend()) {
      return nullptr;
    } else {
      return this_it->get();
    }
  }
}

template <class T>
Tree<T> * Tree<T>::firstInNextBranch () {
  if (isRoot()) {
    return nullptr;
  } else {
    auto next_sibling = nextSibling();
    if (next_sibling == nullptr) {
      return parent->firstInNextBranch();
    } else {
      return next_sibling;
    }
  }
}

template <class T>
Tree<T> const * Tree<T>::firstInNextBranch () const {
  if (isRoot()) {
    return nullptr;
  } else {
    auto next_sibling = nextSibling();
    if (next_sibling == nullptr) {
      return parent->firstInNextBranch();
    } else {
      return next_sibling;
    }
  }
}

template <class T>
Tree<T> * Tree<T>::nextLeaf () {
  auto first_in_next_branch = firstInNextBranch();
  if (first_in_next_branch == nullptr) {
    return nullptr;
  } else {
    return first_in_next_branch->firstLeaf();
  }
}

template <class T>
Tree<T> const * Tree<T>::nextLeaf () const {
  auto first_in_next_branch = firstInNextBranch();
  if (first_in_next_branch == nullptr) {
    return nullptr;
  } else {
    return first_in_next_branch->firstLeaf();
  }
}

template <class T>
std::vector<T *> Tree<T>::getBranch () {
  std::vector<T *> branch;
  if (!isRoot()) {
    branch = parent->getBranch();
  }

  if (!isEmpty()) {
    branch.emplace_back(node.get());
  }

  return branch;
}

template <class T>
std::vector<T const *> Tree<T>::getBranch () const {
  std::vector<T const *> branch;
  if (!isRoot()) {
    branch = parent->getBranch();
  }

  if (!isEmpty()) {
    branch.emplace_back(node.get());
  }

  return branch;
}

template <class T>
Tree<T> * Tree<T>::lastLeaf () {
  auto ret = this;
  for (auto ptr = this; ptr != nullptr; ptr = ptr->nextLeaf()) {
    ret = ptr;
  }

  return ret;
}
