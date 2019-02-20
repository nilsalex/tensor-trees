#pragma once
#include "Tensor.hxx"

class Eta : public Tensor {
 private:
  char i1, i2;

 public:
  Eta() = default;
  Eta(char const i1, char const i2) : i1(i1), i2(i2) {}

  ~Eta() = default;
};
