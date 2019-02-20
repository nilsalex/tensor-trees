#pragma once
#include "Node.hxx"

class Scalar : public Node {
 private:
   size_t var;

 public:
  Scalar () = default;

  Scalar (size_t variable) : var(variable) {}

  ~Scalar() = default;
};
