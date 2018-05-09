#pragma once
#include <map>
#include <set>
#include <gmpxx.h>

#include "Tree.hxx"
#include "Node.hxx"

std::string printTree (std::unique_ptr<Tree<Node>> const & tree);

std::string printForest (Forest<Node> const & f, size_t depth = 0);

std::unique_ptr<Tree<Node>> copyTree (std::unique_ptr<Tree<Node>> const & tree);

void multiplyTree (std::unique_ptr<Tree<Node>> & tree, mpq_class const & factor);

void exchangeTensorIndices (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map);

void applyTensorSymmetries (std::unique_ptr<Tree<Node>> & tree, int parity = 1);

void symmetrizeTree (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map, int parity = 1);

void sortBranch (std::vector<Node *> & branch);

void sortTreeAndMerge (std::unique_ptr<Tree<Node>> & dst, std::unique_ptr<Tree<Node>> const & src);

void insertBranch (std::unique_ptr<Tree<Node>> & dst, std::vector<Node *> & branch, size_t const node_number = 0);

void removeEmptyBranches (std::unique_ptr<Tree<Node>> & tree);

void sortForest (Forest<Node> & forest);

bool isTreeSorted (std::unique_ptr<Tree<Node>> const & tree);

// ********** old **********

std::map<size_t, mpq_class> evaluateForest (Forest<Node> const & forest, std::map<char, size_t> const & eval_map, mpq_class prefactor);

void shrinkForest (Forest<Node> & forest);

std::set<size_t> getVariableSet(Forest<Node> const & forest);

std::map<size_t, size_t> getVariableMap(Forest<Node> const & forest);

std::set<std::vector<mpq_class>> getCoefficientMatrix (Forest<Node> const & forest, std::map<size_t, size_t> const & variable_map);

void setVariablesToZero (Forest<Node> & forest, std::set<size_t> const & variables);

void substituteVariables (Forest<Node> & forest, std::map<size_t, size_t> const & variables);

void redefineVariables (Forest<Node> & forest);
