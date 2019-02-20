#pragma once
#include "Tensor.hxx"

class Epsilon : public Tensor {
 private:
  char i1, i2, i3, i4;

 public:
  Epsilon () = default;
  Epsilon(char const i1, char const i2, char const i3, char const i4) : i1(i1), i2(i2), i3(i3), i4(i4) {}
  ~Epsilon() = default;
};
