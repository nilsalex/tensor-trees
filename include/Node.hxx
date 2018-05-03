#pragma once
#include <gmpxx.h>
#include <map>
#include <set>

#include "Tree.hxx"

class Node {
 private:
 
 public:
  virtual std::map<size_t, mpq_class> evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const = 0; 
  virtual std::string print() const = 0;
  virtual int applyTensorSymmetries (Forest<Node> & forest, int parity) = 0;
  virtual void exchangeTensorIndices (Forest<Node> & forest, std::map<char, char> const & exchange_map) = 0;
  virtual void multiplyForest (Forest<Node> & forest, mpq_class factor) = 0;
  virtual Forest<Node> splitUnsortedTrees (std::unique_ptr<Tree<Node>> const & t) const = 0;
  virtual bool isLeastNode (Forest<Node> const & forest) const;
  virtual int order () const = 0;
  virtual std::set<size_t> getVariableSet (Forest<Node> const & forest) const = 0;

  virtual std::unique_ptr<Node> clone () const = 0;

  virtual bool lessThan(Node const * other) const = 0;
  virtual bool equals(Node const * other) const = 0;

  virtual bool operator< (Node const * other) const;

  virtual bool operator== (Node const * other) const;

  virtual bool operator> (Node const * other) const;

  virtual ~Node() = default;
};

