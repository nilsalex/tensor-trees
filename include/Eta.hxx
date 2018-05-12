#pragma once
#include "Node.hxx"
#include "Tensor.hxx"

class Eta : public Tensor {
 private:
  char i1, i2;

 public:
  Eta(char const i1, char const i2);

  char order () const override;

  void exchangeTensorIndices (std::map<char, char> const & exchange_map) override;
  int sortIndices () override;

  std::map<size_t, mpq_class> evaluateTree (Tree<Node> const & tree, std::map<char, size_t> const & eval_map, mpq_class prefactor) const; 
  mpq_class evaluate(std::map <char, size_t> const & eval_map) const override;
  mpq_class symmetrize() override;
  std::string print () const override;
  int applyTensorSymmetries (int parity) override;

  bool lessThan(Node const * other) const override;
  bool equals(Node const * other) const override;


  std::unique_ptr<Node> clone () const override;

  ~Eta() = default;
};
