#pragma once
#include "Node.hxx"

class Scalar : public Node {
 private:
  std::map<size_t, mpq_class> map;

 public:
  Scalar () = default;

  Scalar (size_t variable, mpq_class fraction);

  Scalar (Scalar const & other);

  int order () const override;
  std::map<size_t, mpq_class> get() const;
  std::string print () const override;

  int applyTensorSymmetries (int parity) override;
  void exchangeTensorIndices (std::map<char, char> const & exchange_map);

  void multiply (mpq_class const & factor) override;

  std::unique_ptr<Node> clone () const override;

  std::map<size_t, mpq_class> evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const; 

  std::set<size_t> getVariableSet (Forest<Node> const &) const override;

  void addOther(Scalar const * other);

  bool lessThan (Node const * other) const override;
  bool equals (Node const * other) const override;
  bool isZero () const;

  void setVariablesToZero (Forest<Node> & forest, std::set<size_t> const & variables) override;
  void substituteVariables (Forest<Node> & forest, std::map<size_t, size_t> const & map) override;

  std::set<std::vector<mpq_class>> getCoefficientMatrix (Forest<Node> const & forest, std::map<size_t, size_t> const & variable_map);


  ~Scalar() = default;
};
