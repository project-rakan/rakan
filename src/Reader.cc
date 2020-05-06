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

uint32_t RidOfZeros(uint32_t x) {
  uint32_t copy = x;
  char *byte = (char *) &x;

  for (int i = 0; i < 4; i++) {
    if (*byte == 0x0) {
      copy = copy >> 8;
    } else {
      break;
    }
    byte++;
  }

  return copy;
}

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

uint16_t Reader::ReadNode(const uint32_t offset, NodeRecord& record, Node *node) const {
  size_t res;
  uint32_t i, temp;

  if (file_ == nullptr) {
    return INVALID_FILE;
  }

  if (fseek(file_, offset + record.node_pos, SEEK_SET) != 0) {
    return SEEK_FAILED;
  }

  // Read node id.
  res = fread(&node->id_, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->id_ = RidOfZeros(ntohl(node->id_));

  // Read area.
  res = fread(&node->area_, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->area_ = RidOfZeros(ntohl(node->area_));

  for (i = 0; i < record.num_neighbors; i++) {
    // Read neighbor.
    res = fread(&temp, sizeof(uint32_t), 1, file_);
    if (res != 1) {
      return READ_FAILED;
    }
    node->neighbors_->insert(RidOfZeros(ntohl(temp)));
  }

  // Read total population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetTotalPop(RidOfZeros(ntohl(temp)));

  // Read AA population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAAPop(RidOfZeros(ntohl(temp)));

  // Read AI population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAIPop(RidOfZeros(ntohl(temp)));

  // Read AS population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetASPop(RidOfZeros(ntohl(temp)));

  // Read CA popuation.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetCAPop(RidOfZeros(ntohl(temp)));

  // Read other population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetOtherPop(RidOfZeros(ntohl(temp)));

  return SUCCESS;
}

}     // namespace rakan
