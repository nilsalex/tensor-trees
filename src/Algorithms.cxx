#include <algorithm>
#include <sstream>

#include <iostream>

#include "Indices.hxx"
#include "Tensor.hxx"
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

std::string printTreeMaple (std::unique_ptr<Tree<Node>> const & tree) {
  std::stringstream ss;
  auto it = tree->firstLeaf();

  bool first = true;
  while (it != nullptr) {
    if (first) {
      first = false;
    } else {
      ss << "+ ";
    }
    ss << printBranchMaple (it->getBranch());
    ss << std::endl;
    
    it = it->nextLeaf();
  }

  return ss.str();
}

std::string printBranchMaple (std::vector <Node *> const & branch) {
  assert (branch.size() > 1);
  std::stringstream ss;
  
  ss << branch.back()->printMaple();

  for (size_t counter = 0; counter < branch.size() - 1; ++counter) {
    ss << " * ";
    ss << branch[counter]->printMaple();
  }

  return ss.str();
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

void exchangeSymmetrizeTree (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map, int parity) {
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

void multiExchangeSymmetrizeTree (std::unique_ptr<Tree<Node>> & tree, std::vector<std::pair<std::map<char, char>, int>> const & exchange_map_set) {
  multiplyTree (tree, mpq_class(1, exchange_map_set.size() + 1));

  auto tree_copy = copyTree (tree);

  std::for_each (exchange_map_set.cbegin(), exchange_map_set.cend(),
    [&tree_copy,&tree] (auto const & p) {
      auto new_tree = copyTree (tree_copy);
      auto exchange_map = p.first;
      int parity = p.second;
      mpq_class factor = (parity < 0 ? -1 : 1);
      multiplyTree (new_tree, factor);
      exchangeTensorIndices (new_tree, exchange_map);
      applyTensorSymmetries (new_tree);
      removeEmptyBranches (new_tree);
      sortTreeAndMerge (tree, new_tree);
      removeEmptyBranches (tree);
    });
}

void redefineScalarsSym (std::unique_ptr<Tree<Node>> & tree) {
  auto leaf_it = tree->firstLeaf();
  std::map<size_t, std::pair<size_t, mpq_class>> substitution_map;
  size_t var_counter = 0;
  
  while (leaf_it != nullptr) {
    Scalar * scalar = static_cast<Scalar *>(leaf_it->node.get());
    auto first = scalar->getCoefficientMap()->begin();
    auto it = substitution_map.find(first->first);
    if (it == substitution_map.end()) {
      std::unique_ptr<Node> new_scalar = std::make_unique<Scalar>(++var_counter, first->second);
      std::swap(new_scalar, leaf_it->node);
      substitution_map[first->first] = std::make_pair(var_counter, first->second);
    } else {
      std::unique_ptr<Node> new_scalar = std::make_unique<Scalar>(it->second.first, first->second);
      std::swap(new_scalar, leaf_it->node);
    }

    leaf_it = leaf_it->nextLeaf();
  }
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

void sortTree (std::unique_ptr<Tree<Node>> & tree) {
  sortForest (tree->forest);

  std::for_each(tree->forest.begin(), tree->forest.end(),
    [] (auto & t) {
      sortTree (t);
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

//  sortForest(dst->forest);
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

std::map<size_t, mpq_class> evaluateTree (std::unique_ptr<Tree<Node>> const & tree, std::map<char, char> const & eval_map, mpq_class prefactor) {
  std::map<size_t, mpq_class> ret;

  if (tree->isLeaf()) {
    std::map<size_t, mpq_class> const * leaf_map = tree->node->getCoefficientMap();
    std::for_each(leaf_map->cbegin(), leaf_map->cend(),
      [&ret,&prefactor] (auto const & p) {
        ret.insert(std::make_pair(p.first, prefactor * p.second));
      });
  } else {
    if (!tree->isRoot()) {
      prefactor *= static_cast<Tensor const *>(tree->node.get())->evaluate(eval_map);
      if (prefactor == 0) {
        return ret;
      }
    } 

    std::for_each(tree->forest.cbegin(), tree->forest.cend(),
      [&ret, &eval_map, &prefactor] (auto const & t) {
        auto _map = evaluateTree (t, eval_map, prefactor);
        std::for_each(_map.cbegin(), _map.cend(),
          [&ret] (auto const & a) {
            auto it = ret.find(a.first);
            if (it == ret.end()) {
              ret.insert(std::make_pair(a.first, a.second));
            } else {
              it->second += a.second;
              if (it->second == 0) {
                ret.erase(it);
              }
            }
          });
      });
  }

  return ret;
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

std::map<size_t, size_t> getVariableMap (std::unique_ptr<Tree<Node>> const & tree) {
  auto variables = getVariableSet(tree);
  std::map<size_t, size_t> ret;
  std::for_each(variables.cbegin(), variables.cend(),
    [&ret,n=0] (auto const & v) mutable {
      ret[v] = n++;
    });
  return ret;
}

std::set<size_t> getVariableSet (std::unique_ptr<Tree<Node>> const & tree) {
  std::set<size_t> ret;

  auto leaf_it = tree->firstLeaf();

  while (leaf_it != nullptr) {
    ret.merge (leaf_it->node->getVariableSet());
    leaf_it = leaf_it->nextLeaf();
  }

  return ret;
}

void shrinkForest (Forest<Node> & forest) {
  forest.shrink_to_fit();
  std::for_each(forest.begin(), forest.end(),
    [] (auto & t) {
      t->forest.shrink_to_fit();
    });
}

void substituteVariables (std::unique_ptr<Tree<Node>> & tree, std::map<size_t, size_t> const & subs_map) {
  auto leaf_it = tree->firstLeaf();

  while (leaf_it != nullptr) {
    leaf_it->node->substituteVariables (subs_map);
    leaf_it = leaf_it->nextLeaf();
  }
}

void setVariablesToZero (std::unique_ptr<Tree<Node>> & tree, std::set<size_t> const & variables) {
  auto leaf_it = tree->firstLeaf();

  while (leaf_it != nullptr) {
    leaf_it->node->removeVariables(variables);
    leaf_it = leaf_it->nextLeaf();
  }
}

void evaluateNumerical (std::unique_ptr<Tree<Node>> & tree, std::function< void (std::unique_ptr<Tree<Node>> const &, std::set<std::map<size_t, mpq_class>> &)> fun) {
  std::set<std::map<size_t, mpq_class>> eval_res_set;
  
  fun (tree, eval_res_set);

  std::set<size_t> var_set = getVariableSet (tree);
  std::map<size_t, size_t> var_map;
  std::set<size_t> erase_set = var_set;
  std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> eval_res_mat;
  std::set<std::pair<std::pair<size_t, size_t>, mpq_class>> eval_res_mat_mapped;

  std::for_each(eval_res_set.cbegin(), eval_res_set.cend(),
    [row_counter=0,&eval_res_mat] (auto const & m) mutable {
      std::for_each(m.cbegin(), m.cend(),
        [&row_counter,&eval_res_mat] (auto const & p) {
          eval_res_mat.insert(std::make_pair(std::make_pair(row_counter, p.first), p.second));
        }); 
      ++row_counter; 
    }); 

  eval_res_set.clear();

  std::for_each(eval_res_mat.cbegin(), eval_res_mat.cend(),
    [&eval_res_mat_mapped,&var_map,&erase_set,var=0] (auto const & v) mutable {
      erase_set.erase(v.first.second);
      auto it = var_map.find(v.first.second);
      if (it == var_map.end()) {
        var_map.insert(std::make_pair(v.first.second, var));
        ++var;
      }
      eval_res_mat_mapped.insert(std::make_pair(std::make_pair(v.first.first, var_map.at(v.first.second)), v.second));
    });

  eval_res_mat.clear();

  std::cout << "These equations contain " << var_map.size() << " variables." << std::endl;
  std::cout << "That is, " << erase_set.size() << " variables do not contribute at all." << std::endl;
  std::cout << std::endl;
  std::cout << "Computing LU decomposition of the linear system using Eigen subroutines ..." << std::endl;

  auto erase_set_2_mapped = findDependentVariables (eval_res_mat_mapped, eval_res_mat_mapped.rbegin()->first.first + 1, var_map.size());
  std::cout << "... done! " << erase_set_2_mapped.size() << " variables are dependent." << std::endl;

  eval_res_mat_mapped.clear();

  std::map<size_t, size_t> var_rmap;
  std::transform (var_map.cbegin(), var_map.cend(), std::inserter(var_rmap, var_rmap.begin()),
    [] (auto const & p) {
      return std::make_pair(p.second, p.first);
    });

  std::set<size_t> erase_set_2;
  std::transform(erase_set_2_mapped.cbegin(), erase_set_2_mapped.cend(), std::inserter(erase_set_2, erase_set_2.begin()),
    [&var_rmap] (auto const & v) {
      return var_rmap.at(v);
    });

  erase_set_2_mapped.clear();

  erase_set.merge(erase_set_2);
  erase_set_2.clear();

  std::cout << "Removing all " << erase_set.size() << " variables from the ansatz." << std::endl;

  setVariablesToZero (tree, erase_set);
  removeEmptyBranches (tree);

  var_set = getVariableSet (tree);

  var_map.clear();

  std::transform (var_set.cbegin(), var_set.cend(), std::inserter (var_map, var_map.begin()),
    [counter=0] (auto const & v) mutable {
      return std::make_pair (v, ++counter);
    });

  substituteVariables (tree, var_map);
}

bool compareTrees (std::unique_ptr<Tree<Node>> const & tree1, std::unique_ptr<Tree<Node>> const & tree2) {
  bool nodes_equal = false;
  if (tree1->isEmpty() != tree2->isEmpty()) {
    nodes_equal = false;
  } else if (tree1->isEmpty() && tree2->isEmpty()) {
    nodes_equal = true;
  } else {
    nodes_equal = tree1->node->equals(tree2->node.get());
  }

  return (nodes_equal && std::equal(tree1->forest.cbegin(), tree1->forest.cend(),
                                    tree2->forest.cbegin(), tree2->forest.cend(),
                                    [] (auto const & t1, auto const & t2) {
                                      return compareTrees (t1, t2);
                                    }));
}
