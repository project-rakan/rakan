#include "./Node.h"

#include <bits/stdc++.h>      // for std::unordered_set.insert
#include <inttypes.h>         // for uint32_t
#include <stdio.h>            // for FILE *, fread, fseek
#include <unordered_map>      // for std::unordered_map

using std::unordered_set;
using std::unordered_map;

namespace rakan {

bool Node::operator==(const Node& other) const {
  return (this->id_ == other.id_ &&
          this->district_ == other.district_ &&
          this->neighbors_ == other.neighbors_ &&
          this->demographics_ == other.demographics_);
}

bool Node::AddNeighbor(Node& other) {
  return (this->neighbors_->insert(other.id_).second &&
          other.neighbors_->insert(this->id_).second);
}

}   // namespace rakan