#include "./Node.h"

#include <bits/stdc++.h>      // for std::unordered_set.insert
#include <inttypes.h>         // for uint32_t
#include <stdio.h>            // for FILE *, fread, fseek
#include <unordered_map>      // for std::unordered_map

using std::unordered_set;
using std::unordered_map;

namespace rakan {

Node::Node() {
  neighbors_ = new unordered_set<uint32_t>;
  demographics_ = new unordered_map<string, uint32_t>;
}

Node::Node(const uint32_t id) : id_(id) {
  neighbors_ = new unordered_set<uint32_t>;
  demographics_ = new unordered_map<string, uint32_t>;
}

Node::Node(const uint32_t id, const uint32_t district)
    : id_(id), district_(district) {
  neighbors_ = new unordered_set<uint32_t>;
  demographics_ = new unordered_map<string, uint32_t>;
}

Node::Node(const uint32_t id,
           const uint32_t district,
          unordered_set<uint32_t> *neighbors)
    : id_(id), district_(district), neighbors_(neighbors) {
  demographics_ = new unordered_map<string, uint32_t>;
}

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