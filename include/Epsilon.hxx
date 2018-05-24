#pragma once
#include <boost/serialization/access.hpp>

#include "Node.hxx"
#include "Tensor.hxx"

class Epsilon : public Tensor {
 private:
  char i1, i2, i3, i4;

  friend class boost::serialization::access;

 public:
  Epsilon () = default;
  Epsilon(char const i1, char const i2, char const i3, char const i4);

  char order () const override;

  void exchangeTensorIndices (std::map<char, char> const & exchange_map) override;
  int sortIndices () override;

  int evaluate(std::map <char, char> const & eval_map) const override;
  mpq_class symmetrize() override;
  bool containsIndex (char i) const override;
  std::string print () const override;
  std::string printMaple () const override;
  int applyTensorSymmetries (int parity) override;

  bool lessThan(Node const * other) const override;
  bool equals(Node const * other) const override;

  std::unique_ptr<Node> clone () const override;

  template<class Archive>
  void serialize (Archive & ar, unsigned int const version);

  ~Epsilon() = default;
};
