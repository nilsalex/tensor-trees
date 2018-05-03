#include "Node.hxx"

bool Node::isLeastNode (Forest<Node> const & forest) const {
  bool least = true;

  for (auto it = forest.cbegin(); it != forest.cend(); ++it) {
    least = !(*this > (*it)->node.get());
  }

  return least;
}

bool Node::operator< (Node const * other) const {
  if (order() < other->order()) {
    return true;
  } else if (order() > other->order()) {
    return false;
  } else {
    return this->lessThan(other);
  }
}

bool Node::operator== (Node const * other) const {
  if (order() != other->order()) {
    return false;
  } else {
    return this->equals(other);
  }
}

bool Node::operator> (Node const * other) const {
  return !(*this == other || *this < other);
}
