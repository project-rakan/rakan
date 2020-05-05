#include "./Reader.h"

#include <bits/stdc++.h>      // for std::unordered_set
#include <endian.h>
#include <inttypes.h>         // for uint32_t
#include <stdio.h>            // for FILE *, fread, fseek
#include <unordered_map>      // for std::unordered_map
#include <vector>             // for std::vector

#include "./ErrorCodes.h"     // for error codes
#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class

using std::unordered_set;
using std::unordered_map;

namespace rakan {

uint16_t Reader::ReadHeader(Header *header) const {
  size_t res;

  if (file_ == nullptr) {
    return INVALID_FILE;
  }

  res = fread(header, sizeof(Header), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  header->ToHostFormat();

  return SUCCESS;
}

uint16_t Reader::ReadNodeRecord(const uint32_t offset, NodeRecord *record) const {
  int res;

  if (file_ == nullptr) {
    return INVALID_FILE;
  }

  if (fseek(file_, offset, SEEK_SET) != 0) {
    return SEEK_FAILED;
  }

  res = fread(record, sizeof(NodeRecord), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  record->ToHostFormat();

  return SUCCESS;
}

uint16_t Reader::ReadNode(NodeRecord& record, Node *node) const {
  size_t res;
  uint32_t i;
  unsigned char buf[4];

  if (file_ == nullptr) {
    return INVALID_FILE;
  }

  if (fseek(file_, record.node_pos, SEEK_SET) != 0) {
    return SEEK_FAILED;
  }

  // Read node id.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }

  node->id_ = ntohl(*(reinterpret_cast<uint32_t *> (buf)));

  // Read area.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->area_ = *(reinterpret_cast<uint32_t *> (buf));

  for (i = 0; i < record.num_neighbors; i++) {
    // Read neighbor.
    res = fread(buf, sizeof(uint32_t), 1, file_);
    if (res != 1) {
      return READ_FAILED;
    }
    node->neighbors_->insert(*(reinterpret_cast<uint32_t *> (buf)));
  }

  // Read total population.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetTotalPop(*(reinterpret_cast<uint32_t *> (buf)));

  // Read AA population.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAAPop(*(reinterpret_cast<uint32_t *> (buf)));

  // Read AI population.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAIPop(*(reinterpret_cast<uint32_t *> (buf)));

  // Read AS population.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetASPop(*(reinterpret_cast<uint32_t *> (buf)));

  // Read CA popuation.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetCAPop(*(reinterpret_cast<uint32_t *> (buf)));

  // Read other population.
  res = fread(buf, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetOtherPop(*(reinterpret_cast<uint32_t *> (buf)));

  return SUCCESS;
}

}     // namespace rakan
