#pragma once
#include <gmpxx.h>
#include <map>
#include <set>

#include "Tree.hxx"

class Node;

class Node {
 private:
 
 public:
  virtual std::map<size_t, mpq_class> evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const = 0; 
  virtual std::string print() const = 0;
  virtual int applyTensorSymmetries (int parity) = 0;
  virtual void exchangeTensorIndices (std::map<char, char> const & exchange_map) = 0;
  virtual void multiply (mpq_class const & factor) = 0;
  virtual bool isLeastNode (Forest<Node> const & forest) const;
  virtual char order () const = 0;
  virtual std::set<size_t> getVariableSet () const = 0;
  virtual std::map<size_t, mpq_class> const * getCoefficientMap () const = 0;
  virtual void setVariablesToZero (Forest<Node> & forest, std::set<size_t> const & variables) = 0;
  virtual void substituteVariables (std::map<size_t, size_t> const & subs_map) = 0;

  virtual std::unique_ptr<Node> clone () const = 0;

  virtual bool lessThan(Node const * other) const = 0;
  virtual bool equals(Node const * other) const = 0;

  virtual bool operator< (Node const * other) const;
  virtual bool operator< (Node const & other) const;

  virtual bool operator== (Node const * other) const;
  virtual bool operator== (Node const & other) const;

  virtual bool operator> (Node const * other) const;
  virtual bool operator> (Node const & other) const;

  virtual ~Node() = default;
};

