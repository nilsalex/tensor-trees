#include <algorithm>
#include <sstream>

#include "Scalar.hxx"
#include "Algorithms.hxx"

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

Forest<Node> copyForest (Forest<Node> const & forest) {
  Forest<Node> new_forest;
  new_forest.reserve(forest.size());

  std::for_each(forest.cbegin(), forest.cend(),
    [&new_forest] (auto const & t) {
      new_forest.emplace_back(std::make_unique<Tree<Node>>());
      new_forest.back()->node = t->node->clone();
      new_forest.back()->forest = copyForest (t->forest);
    });

  return new_forest;
}

void multiplyForest (Forest<Node> & forest, mpq_class factor) {
  std::for_each(forest.begin(), forest.end(),
    [&factor] (auto & t) {
      t->node->multiplyForest(t->forest, factor);
    });
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

void applyTensorSymmetries(Forest<Node> & forest, int parity) {
  std::vector<Tree<Node>*> erase_vec;
  std::for_each(forest.begin(), forest.end(),
    [parity,&erase_vec,&forest] (auto & t) {
      if (t->node->applyTensorSymmetries(t->forest, parity) == 0) {
        erase_vec.push_back(t.get());
      }
    });

  forest.erase(std::remove_if(forest.begin(), forest.end(),
    [&erase_vec] (auto & t) {
      return (std::find(erase_vec.begin(), erase_vec.end(), t.get()) != erase_vec.end());
    }), forest.end());
}

void exchangeTensorIndices (Forest<Node> & forest, std::map<char, char> const & exchange_map) {
  std::for_each(forest.begin(), forest.end(),
    [&exchange_map] (auto & t) {
      t->node->exchangeTensorIndices(t->forest, exchange_map);
    });
}

void symmetrizeForest (Forest<Node> & forest, std::map<char, char> const & exchange_map, int parity) {
  multiplyForest(forest, mpq_class(1, 2));

  Forest<Node> new_forest = copyForest(forest);
  forest.reserve(2 * forest.size());

  if (parity < 0) {
    multiplyForest(new_forest, mpq_class(-1, 1));
  }

 exchangeTensorIndices (new_forest, exchange_map);

  std::for_each(new_forest.begin(), new_forest.end(),
    [&forest] (auto & t) {
      forest.push_back(std::make_unique<Tree<Node>>());
      forest.back().swap(t);
    });
}

bool isForestSorted (Forest<Node> const & forest) {
  if (std::find_if(forest.cbegin(), forest.cend(),
    [] (auto const & t) {
      return !(t->node->isLeastNode(t->forest));
    }) != forest.cend()) {
    return false;
    }
  else {
   return (std::find_if(forest.cbegin(), forest.cend(),
     [] (auto const & t) {
       return !(isForestSorted (t->forest));
     }) == forest.cend());
  }
}

Forest<Node> splitUnsortedTrees (Forest<Node> const & forest) {
  Forest<Node> ret;
  ret.reserve(forest.size());
  std::for_each(forest.begin(), forest.end(),
    [&ret] (auto & t) {
      Forest<Node> tmp_forest = t->node->splitUnsortedTrees (t);
      std::for_each(tmp_forest.begin(), tmp_forest.end(),
        [&ret] (auto & t) {
          ret.emplace_back(std::make_unique<Tree<Node>>());
          std::swap(ret.back(), t);
        });
    });

  return ret;
}

void sortForest (Forest<Node> & forest) {
  std::for_each(forest.begin(), forest.end(),
    [] (auto & t) {
      sortForest (t->forest);
    });

  std::sort(forest.begin(), forest.end(),
    [] (auto & a, auto & b) {
      return (*(a->node) < b->node.get());
    });
}

void sortForestNoRepeat (Forest<Node> & forest) {
  std::sort(forest.begin(), forest.end(),
    [] (auto & a, auto & b) {
      return (*(a->node) < b->node.get());
    });
}

void mergeForest (Forest<Node> & forest) {
  if (forest.empty() || forest.size() == 1) {
    return;
  }

  sortForestNoRepeat(forest);

  Forest<Node> ret;
  
  auto tree = std::make_unique<Tree<Node>>();
  tree->node = forest.front()->node->clone();
  tree->forest = Forest<Node>();

  std::for_each(forest.front()->forest.begin(), forest.front()->forest.end(),
    [&tree] (auto const & t) {
      tree->forest.emplace_back(std::make_unique<Tree<Node>>());
      tree->forest.back()->node = t->node->clone();
      tree->forest.back()->forest = copyForest(t->forest);
    });

  auto last_it = forest.begin();
  for (auto it = forest.begin() + 1; it != forest.end(); ++it) {
    if ((*last_it)->node->order() == 100 && (*it)->node->order() == 100 && tree->node->order() == 100 && 
          static_cast<Scalar *>((*last_it)->node.get())->get_variable() == static_cast<Scalar *>((*it)->node.get())->get_variable()) {
      std::unique_ptr<Node> new_node = static_cast<Scalar *>(tree->node.get())->addOther(static_cast<Scalar *>((*it)->node.get()));
      std::swap(tree->node, new_node);

    } else if (*((*last_it)->node) == (*it)->node.get()) {
      std::for_each((*it)->forest.begin(), (*it)->forest.end(),
        [&tree] (auto const & t) {
          tree->forest.emplace_back(std::make_unique<Tree<Node>>());
          tree->forest.back()->node = t->node->clone();
          tree->forest.back()->forest = copyForest(t->forest);
          });
    } else {
      ret.emplace_back(std::make_unique<Tree<Node>>());
      std::swap(ret.back(), tree);

      tree->node = (*it)->node->clone();
      tree->forest = Forest<Node>();

      std::for_each((*it)->forest.begin(), (*it)->forest.end(),
        [&tree] (auto const & t) {
          tree->forest.emplace_back(std::make_unique<Tree<Node>>());
          tree->forest.back()->node = t->node->clone();
          tree->forest.back()->forest = copyForest(t->forest);
          });
    }
    last_it = it;
  }

  ret.emplace_back(std::make_unique<Tree<Node>>());
  std::swap(ret.back(), tree);

  forest.clear();
  std::for_each(ret.begin(), ret.end(),
    [&forest] (auto & t) {
      forest.emplace_back(std::make_unique<Tree<Node>>());
      forest.back()->node = t->node->clone();
      forest.back()->forest = copyForest(t->forest);
    });

  forest.shrink_to_fit();

  std::for_each(forest.begin(), forest.end(), [] (auto & t) { mergeForest(t->forest); });
}

void shrinkForest (Forest<Node> & forest) {
  forest.shrink_to_fit();
  std::for_each(forest.begin(), forest.end(),
    [] (auto & t) {
      t->forest.shrink_to_fit();
    });
}

void removeEmptyBranches (Forest<Node> & forest) {
  std::for_each(forest.begin(), forest.end(),
    [] (auto & t) {
      removeEmptyBranches (t->forest);
    });

  forest.erase(std::remove_if(forest.begin(), forest.end(),
    [] (auto & t) {
      return ((t->node->order() == 100 && static_cast<Scalar *>(t->node.get())->isZero()) || (t->forest.size() == 0 && t->node->order() != 100));
    }), forest.end());
}
