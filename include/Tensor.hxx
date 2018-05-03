#pragma once
#include <gmpxx.h>
#include <string>

#include "Tree.hxx"
#include "Node.hxx"

class Tensor : public Node {
 private:
  size_t rank;
  std::string indices;
 
 public:
  Tensor(size_t const rank, std::string const & indices);

  size_t get_rank() const;
  std::string const & get_indices() const;

  void exchangeTensorIndices (Forest<Node> & forest, std::map<char, char> const & exchange_map);

  Forest<Node> splitUnsortedTrees (std::unique_ptr<Tree<Node>> const & t) const override;

  std::set<size_t> getVariableSet (Forest<Node> const & forest) const override;

  int sortIndices ();

  virtual mpq_class evaluate(std::map <char, size_t> const & eval_map) const = 0;
  virtual mpq_class symmetrize() = 0;
  void multiplyForest (Forest<Node> & forest, mpq_class factor) override;

  ~Tensor() = default;
};
