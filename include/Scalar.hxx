#pragma once
#include "Node.hxx"

class Scalar : public Node {
 private:
  size_t variable;
  mpq_class fraction;

 public:
  Scalar (size_t variable, mpq_class fraction);

  int order () const override;
  int get_variable() const;
  std::string print () const override;

  int applyTensorSymmetries (Forest<Node> & forest, int parity) override;
  void exchangeTensorIndices (Forest<Node> & forest, std::map<char, char> const & exchange_map);

  void multiplyForest (Forest<Node> &, mpq_class factor) override;

  std::unique_ptr<Node> clone () const override;

  std::map<size_t, mpq_class> evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const; 

  Forest<Node> splitUnsortedTrees (std::unique_ptr<Tree<Node>> const & t) const override;

  std::set<size_t> getVariableSet (Forest<Node> const &) const override;

  std::unique_ptr<Node> addOther(Scalar const * other);

  bool lessThan (Node const * other) const override;
  bool equals (Node const * other) const override;
  bool isZero () const;

  static std::set<std::vector<mpq_class>> getCoefficientMatrix (Forest<Node> const & forest, std::map<size_t, size_t> const & variable_map);


  ~Scalar() = default;
};
