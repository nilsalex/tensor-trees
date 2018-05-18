#pragma once
#include <map>
#include <set>
#include <gmpxx.h>

#include "Tree.hxx"
#include "Node.hxx"

typedef std::map<size_t, mpq_class> coefficient_vector;

struct vec_compare {
  bool operator () (coefficient_vector const & v1, coefficient_vector const & v2);
};

typedef std::set<coefficient_vector, vec_compare> coefficient_matrix;

std::string printTree (std::unique_ptr<Tree<Node>> const & tree);

std::string printForest (Forest<Node> const & f, size_t depth = 0);

std::unique_ptr<Tree<Node>> copyTree (std::unique_ptr<Tree<Node>> const & tree);

void multiplyTree (std::unique_ptr<Tree<Node>> & tree, mpq_class const & factor);

void exchangeTensorIndices (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map);

void applyTensorSymmetries (std::unique_ptr<Tree<Node>> & tree, int parity = 1);

void symmetrizeTree (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map, int parity = 1);

void redefineScalarsSym (std::unique_ptr<Tree<Node>> & tree);

void sortBranch (std::vector<Node *> & branch);

void sortTreeAndMerge (std::unique_ptr<Tree<Node>> & dst, std::unique_ptr<Tree<Node>> const & src);

void insertBranch (std::unique_ptr<Tree<Node>> & dst, std::vector<Node *> & branch, size_t const node_number = 0);

void removeEmptyBranches (std::unique_ptr<Tree<Node>> & tree);

void sortForest (Forest<Node> & forest);

void sortTree (std::unique_ptr<Tree<Node>> & tree);

bool isTreeSorted (std::unique_ptr<Tree<Node>> const & tree);

std::set<size_t> getVariableSet (std::unique_ptr<Tree<Node>> const & tree);

std::map<size_t, size_t> getVariableMap (std::unique_ptr<Tree<Node>> const & tree);

coefficient_matrix getCoefficientMatrix (std::unique_ptr<Tree<Node>> const & tree);

void substituteVariables (std::unique_ptr<Tree<Node>> & tree, std::map<size_t, size_t> const & subs_map);

std::map<size_t, mpq_class> evaluateTree (std::unique_ptr<Tree<Node>> const & tree, std::map<char, char> const & eval_map, mpq_class prefactor = 1);

void shrinkForest (Forest<Node> & forest);

void setVariablesToZero (std::unique_ptr<Tree<Node>> & tree, std::set<size_t> const & variables);

// ********** old **********

void redefineVariables (Forest<Node> & forest);
