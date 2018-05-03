#pragma once
#include "Node.hxx"
#include "Tensor.hxx"

class Epsilon : public Tensor {
 private:

 public:
  Epsilon(char const i1, char const i2, char const i3, char const i4);

  int order () const override;

  std::map<size_t, mpq_class> evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const; 
  mpq_class evaluate(std::map <char, size_t> const & eval_map) const override;
  mpq_class symmetrize() override;
  std::string print () const override;
  int applyTensorSymmetries (Forest<Node> & forest, int parity) override;

  bool lessThan(Node const * other) const override;
  bool equals(Node const * other) const override;

  std::unique_ptr<Node> clone () const override;

  ~Epsilon() = default;
};
