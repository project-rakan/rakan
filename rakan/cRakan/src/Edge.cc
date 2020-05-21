#include "./Edge.h"

#include <inttypes.h>         // for uint32_t

#include <unordered_set>      // for std::unordered_set
#include <unordered_map>      // for std::unordered_map

using std::unordered_set;
using std::unordered_map;

namespace rakan {

Edge::Edge(const uint32_t one,
            const uint32_t two,
            const uint32_t dist_one,
            const uint32_t dist_two) : node_one_(one), node_two_(two) {
  cross_ = (dist_one != dist_two);
}

Edge::Edge(const uint32_t one, const uint32_t two)
    : node_one_(one), node_two_(two) {
  cross_ = false;
}

bool Edge::operator==(const Edge& other) const {
  return (this->node_one_ == other.node_one_ &&
          this->node_two_ == other.node_two_ &&
          this->cross_ == other.cross_);
}

bool Edge::ChangeConnection(uint32_t one, uint32_t two, bool cross) {
    if (node_one_ == one &&
          node_two_ == two &&
          cross_ == cross) {
              return false;
          }
    node_one_ = one;
    node_two_ = two;
    cross_ = cross;
    return true;
}

}   // namespace rakan
