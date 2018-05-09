#include <algorithm>
#include <sstream>

#include <iostream>

#include "Scalar.hxx"
#include "LinearAlgebra.hxx"
#include "Algorithms.hxx"

std::string printTree (std::unique_ptr<Tree<Node>> const & tree) {
  if (tree->isEmpty()) {
    return printForest (tree->forest, 0);
  } else {
    std::stringstream ss;
    ss << tree->node->print();
    ss << "\n";
    ss << printForest (tree->forest, 1);
    return ss.str();
  }
}

std::string printForest (Forest<Node> const & f, size_t depth) {
  std::stringstream ss;
  std::for_each (f.cbegin(), f.cend(),
    [depth, &ss] (auto const & t) {
      for (size_t counter = 0; counter < depth; ++counter) {
        ss << "\t";
      }
      ss << t->node->print();
      ss << "\n";
      ss << printForest (t->forest, depth + 1);
    });
  return ss.str();
}

std::unique_ptr<Tree<Node>> copyTree (std::unique_ptr<Tree<Node>> const & tree) {
  auto new_tree = std::make_unique<Tree<Node>>();
  if (!tree->isEmpty()) {
    new_tree->node = tree->node->clone();
  }
  new_tree->forest = Forest<Node>();
  new_tree->forest.reserve(tree->forest.size());

  std::for_each(tree->forest.cbegin(), tree->forest.cend(),
    [&new_tree,&tree] (auto const & t) {
      new_tree->forest.emplace_back(std::move(copyTree(t)));
      new_tree->forest.back()->parent = new_tree.get();
    });

  return new_tree;
}

void multiplyTree (std::unique_ptr<Tree<Node>> & tree, mpq_class const & factor) {
  auto it = tree->firstLeaf();

  while (it != nullptr) {
    it->node->multiply(factor);
    it = it->nextLeaf();
  }
}

void exchangeTensorIndices (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map) {
  if (!tree->isEmpty()) {
    tree->node->exchangeTensorIndices(exchange_map);
  }

  std::for_each(tree->forest.begin(), tree->forest.end(),
    [&exchange_map] (auto & t) {
      exchangeTensorIndices (t, exchange_map);
    });
}

void symmetrizeTree (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map, int parity) {
  multiplyTree(tree, mpq_class(1, 2));

  auto new_tree = copyTree(tree);

  mpq_class factor = (parity < 0 ? -1 : 1);
  multiplyTree (new_tree, factor);

  exchangeTensorIndices (new_tree, exchange_map);

  applyTensorSymmetries (new_tree);
  removeEmptyBranches(new_tree);

  sortTreeAndMerge (tree, new_tree);
  removeEmptyBranches(tree);
}

void sortBranch (std::vector<Node *> & branch) {
  std::sort (branch.begin(), branch.end(),
    [] (auto & n, auto & m) {
      return (*n) < m;
    });
}

void sortForest (Forest<Node> & forest) {
  std::sort (forest.begin(), forest.end(),
    [] (auto const & n, auto const & m) {
      return *(n->node) < m->node.get();
    });
}

void sortTreeAndMerge (std::unique_ptr<Tree<Node>> & dst, std::unique_ptr<Tree<Node>> const & src) {
  auto src_it = src->firstLeaf();

  while (src_it != nullptr) {
    auto branch = src_it->getBranch();
    sortBranch (branch);

    insertBranch (dst, branch);

    src_it = src_it->nextLeaf();
  }
}

void insertBranch (std::unique_ptr<Tree<Node>> & dst, std::vector<Node *> & branch, size_t const node_number) {
  if (branch.size() == node_number) {
    return;
  }

  auto node_it = std::find_if (dst->forest.begin(), dst->forest.end(),
    [&branch,node_number] (auto & t) {
      if (t->node == nullptr) {
        return false;
      } else if (t->node->order() == 100 && branch[node_number]->order() == 100) {
        return true;
      } else if (t->node->equals(branch[node_number])){
        return true;
      } else {
        return false;
      }
    });

  if (node_it == dst->forest.end()) {
    dst->forest.push_back (std::make_unique<Tree<Node>>());
    dst->forest.back()->node = branch[node_number]->clone();
    dst->forest.back()->parent = dst.get();
    insertBranch(dst->forest.back(), branch, node_number + 1);
  } else if ((*node_it)->node->order() == 100 && branch[node_number]->order() == 100) {
    static_cast<Scalar *>((*node_it)->node.get())->addOther(static_cast<Scalar *>(branch[node_number]));
    insertBranch (*node_it, branch, node_number + 1);
  } else {
    insertBranch (*node_it, branch, node_number + 1);
  }

  sortForest(dst->forest);
}

void removeEmptyBranches (std::unique_ptr<Tree<Node>> & tree) {
  std::for_each(tree->forest.begin(), tree->forest.end(),
    [] (auto & t) {
      removeEmptyBranches (t);
    });

  tree->forest.erase(std::remove_if(tree->forest.begin(), tree->forest.end(),
    [] (auto & t) {

      if (!t->isEmpty()) {
        if (t->node->order() == 100 && static_cast<Scalar *>(t->node.get())->isZero()) {
          return true;
        } else if (t->isLeaf() && t->node->order() != 100) {
          return true;
        }
      } else {
        if (t->isLeaf()) {
          return true;
        }
      }

  return false;

  }), tree->forest.end());
}

bool isTreeSorted (std::unique_ptr<Tree<Node>> const & tree) {
  return (std::is_sorted(tree->forest.cbegin(), tree->forest.cend(),
    [] (auto const & a, auto const & b) {
      return *(a->node) < b->node.get();
    }) && std::all_of(tree->forest.cbegin(), tree->forest.cend(),
      [] (auto const & t) {
        return isTreeSorted (t);
      }));
}

std::map<size_t, mpq_class> evaluateForest (Forest<Node> const & forest, std::map<char, size_t> const & eval_map, mpq_class prefactor) {
  if (forest.size() == 0) {
    return std::map<size_t, mpq_class> {};
  } else {
    std::map<size_t, mpq_class> ret;
    std::for_each(forest.cbegin(), forest.cend(),
      [&ret, &eval_map, prefactor] (auto const & tp) {
        auto _map = tp->node->evaluateTree(*tp, eval_map, prefactor);
        std::for_each(_map.cbegin(), _map.cend(),
          [&ret] (auto const & a) {
            if (ret.count(a.first) == 0) {
              ret[a.first] = a.second;
            } else {
              ret[a.first] += a.second;
            }
          });
      });
    return ret;
  }
}

void applyTensorSymmetries(std::unique_ptr<Tree<Node>> & tree, int parity) {
  std::vector<Tree<Node>*> erase_vec;
  std::for_each(tree->forest.begin(), tree->forest.end(),
    [parity,&erase_vec] (auto & t) {
      int new_parity = t->node->applyTensorSymmetries(parity);
      if (new_parity == 0) {
        erase_vec.push_back(t.get());
      } else {
        applyTensorSymmetries (t, new_parity);
      }
    });

  tree->forest.erase(std::remove_if(tree->forest.begin(), tree->forest.end(),
    [&erase_vec] (auto & t) {
      return (std::find(erase_vec.begin(), erase_vec.end(), t.get()) != erase_vec.end());
    }), tree->forest.end());
}

void shrinkForest (Forest<Node> & forest) {
  forest.shrink_to_fit();
  std::for_each(forest.begin(), forest.end(),
    [] (auto & t) {
      t->forest.shrink_to_fit();
    });
}

std::set<size_t> getVariableSet(Forest<Node> const & forest) {
  std::set<size_t> ret;
  std::for_each(forest.cbegin(), forest.cend(),
    [&ret] (auto const & t) {
      auto tmp_map = t->node->getVariableSet(t->forest);
      ret.merge(tmp_map);
    });
  return ret;
}

std::map<size_t, size_t> getVariableMap(Forest<Node> const & forest) {
  auto variables = getVariableSet(forest);
  std::map<size_t, size_t> ret;
  std::for_each(variables.cbegin(), variables.cend(),
    [&ret,n=0] (auto const & v) mutable {
      ret[v] = n++;
    });
  return ret;
}

std::set<std::vector<mpq_class>> getCoefficientMatrix (Forest<Node> const & forest, std::map<size_t, size_t> const & variable_map) {
  std::set<std::vector<mpq_class>> ret;

  std::for_each(forest.cbegin(), forest.cend(),
    [&ret,&variable_map] (auto const & t) {
      auto tmp_mat = getCoefficientMatrix (t->forest, variable_map);
      ret.merge(tmp_mat);
    });
  return ret;
}

void setVariablesToZero (Forest<Node> & forest, std::set<size_t> const & variables) {
  std::for_each(forest.cbegin(), forest.cend(),
    [&variables] (auto & t) {
      t->node->setVariablesToZero (t->forest, variables);
    });
}

void substituteVariables (Forest<Node> & forest, std::map<size_t, size_t> const & map) {
  std::for_each(forest.cbegin(), forest.cend(),
    [&map] (auto & t) {
      t->node->substituteVariables (t->forest, map);
    });
}
/*
void redefineVariables (Forest<Node> & forest) {
  // get mapping 
  // variable number -> consecutive number from 1 to number of variables

  auto variable_map = getVariableMap(forest);


  // construct inverse mapping

  std::map<size_t, size_t> reverse_variable_map;
  std::for_each(variable_map.cbegin(), variable_map.cend(),
    [&reverse_variable_map] (auto const & p) {
      reverse_variable_map[p.second] = p.first;
    });

  // get matrix of coefficients in front of variables
  // using variable mapping

  auto coeff_mat = getCoefficientMatrix(forest, variable_map);

  // find a set of dependent variables (using the consecutive numbering)

  auto _dependent_variables = findDependentVariables (coeff_mat);

  // transform to initial numbering

  std::set<size_t> dependent_variables;
  std::for_each(_dependent_variables.cbegin(), _dependent_variables.cend(),
    [&dependent_variables,&reverse_variable_map] (auto const & i) {
      dependent_variables.insert(reverse_variable_map.at(i));
    });

  // set dependent variables to zero

  setVariablesToZero (forest, dependent_variables);
  removeEmptyBranches(forest);
  shrinkForest(forest);

  std::map<size_t, size_t> substitution_map;
  std::for_each(variable_map.cbegin(), variable_map.cend(),
    [&substitution_map,&_dependent_variables,n=0] (auto const & p) mutable {
      if (_dependent_variables.find(p.second) == _dependent_variables.end()) {
        substitution_map[p.first] = ++n;
      }
    });

  substituteVariables(forest, substitution_map);

  std::for_each(dependent_variables.cbegin(), dependent_variables.cend(),
    [] (auto const & v) {
      std::cout << v << " " << std::endl;
    });

  std::for_each(substitution_map.cbegin(), substitution_map.cend(),
    [] (auto const & p) {
      std::cout << p.first << " --> " << p.second << std::endl;
    });
}
*/
