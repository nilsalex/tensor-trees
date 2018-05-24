#pragma once
#include <functional>
#include <map>
#include <set>
#include <gmpxx.h>

#include "Tree.hxx"
#include "Node.hxx"

std::string printTree (std::unique_ptr<Tree<Node>> const & tree);

std::string printTreeMaple (std::unique_ptr<Tree<Node>> const & tree);

std::string printBranchMaple (std::vector<Node *> const & branch);

std::string printForest (Forest<Node> const & f, size_t depth = 0);

std::unique_ptr<Tree<Node>> copyTree (std::unique_ptr<Tree<Node>> const & tree);

void multiplyTree (std::unique_ptr<Tree<Node>> & tree, mpq_class const & factor);

void exchangeTensorIndices (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map);

void applyTensorSymmetries (std::unique_ptr<Tree<Node>> & tree, int parity = 1);

void exchangeSymmetrizeTree (std::unique_ptr<Tree<Node>> & tree, std::map<char, char> const & exchange_map, int parity = 1);

void multiExchangeSymmetrizeTree (std::unique_ptr<Tree<Node>> & tree, std::vector<std::pair<std::map<char, char>, int>> const & exchange_map_set);

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

void substituteVariables (std::unique_ptr<Tree<Node>> & tree, std::map<size_t, size_t> const & subs_map);

std::map<size_t, mpq_class> evaluateTree (std::unique_ptr<Tree<Node>> const & tree, std::map<char, char> const & eval_map, mpq_class prefactor = 1);

void shrinkForest (Forest<Node> & forest);

void setVariablesToZero (std::unique_ptr<Tree<Node>> & tree, std::set<size_t> const & variables);

void evaluateNumerical (std::unique_ptr<Tree<Node>> & tree, std::function< void (std::unique_ptr<Tree<Node>> const &, std::set<std::map<size_t, mpq_class>> &)> fun);

bool compareTrees (std::unique_ptr<Tree<Node>> const & tree1, std::unique_ptr<Tree<Node>> const & tree2);

void contractTreeWithEta (std::unique_ptr<Tree<Node>> & tree, char i1, char i2);

Forest<Node> contractTreeWithEtaInner (std::unique_ptr<Tree<Node>> & tree, char i1, char i2);

void saveTree (std::unique_ptr<Tree<Node>> const & tree, std::string const & filename);

std::unique_ptr<Tree<Node>> loadTree (std::string const & filename);

bool checkSaveAndLoad (std::unique_ptr<Tree<Node>> const & tree);
