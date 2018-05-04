#pragma once
#include <map>
#include <set>
#include <gmpxx.h>

#include "Tree.hxx"
#include "Node.hxx"

std::string printForest (Forest<Node> const & f, size_t depth = 0);

std::map<size_t, mpq_class> evaluateForest (Forest<Node> const & forest, std::map<char, size_t> const & eval_map, mpq_class prefactor);

void applyTensorSymmetries (Forest<Node> & forest, int parity = 1);

void exchangeTensorIndices (Forest<Node> & forest, std::map<char, char> const & exchange_map);

Forest<Node> copyForest (Forest<Node> const & forest);

void multiplyForest (Forest<Node> & forest, mpq_class factor);

void symmetrizeForest (Forest<Node> & forest, std::map<char, char> const & exchange_map, int parity);

Forest<Node> splitUnsortedTrees (Forest<Node> const & forest);

void sortForest (Forest<Node> & forest);

void sortForestNoRepeat (Forest<Node> & forest);

void mergeForest (Forest<Node> & forest);

void shrinkForest (Forest<Node> & forest);

bool isForestSorted (Forest<Node> const & forest);

void removeEmptyBranches (Forest<Node> & forest);

std::set<size_t> getVariableSet(Forest<Node> const & forest);

std::map<size_t, size_t> getVariableMap(Forest<Node> const & forest);

std::set<std::vector<mpq_class>> getCoefficientMatrix (Forest<Node> const & forest, std::map<size_t, size_t> const & variable_map);

bool hasOnlyScalarNodes (Forest<Node> const & forest);

void setVariablesToZero (Forest<Node> & forest, std::set<size_t> const & variables);

void substituteVariables (Forest<Node> & forest, std::map<size_t, size_t> const & variables);
