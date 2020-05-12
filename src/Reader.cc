#include "./Reader.h"

#include <arpa/inet.h>        // For ntohl()
#include <bits/stdc++.h>      // for std::unordered_set
#include <inttypes.h>         // for uint32_t
#include <stdio.h>            // for FILE *, fread, fseek
#include <unordered_map>      // for std::unordered_map

#include "./ReturnCodes.h"     // for return codes
#include "./Graph.h"          // for Graph class
#include "./Node.h"           // for Node class

using std::unordered_set;
using std::unordered_map;

namespace rakan {

const uint32_t kMagicNumber = 0xBEEFCAFE;
const uint32_t kHeaderSize = sizeof(uint32_t) * 4 + sizeof(char) * 2;
const uint32_t kNodeRecordSize = sizeof(uint32_t) * 2;

uint16_t Reader::ReadHeader(Header *header) {
  size_t res;

  if (file_ == nullptr) {
    return INVALID_FILE;
  }

  res = fread(&header->magic_number, sizeof(uint32_t), 1, file_);
  header->magic_number = htonl(header->magic_number);
  if (res != 1) {
    return READ_FAILED;
  }
  // header->magic_number = htonl(header->magic_number);
  res = fread(&header->checksum, sizeof(uint32_t), 1, file_);
  header->checksum = htonl(header->checksum);
  res = fread(&header->state, sizeof(char) * 2, 1, file_);
  res = fread(&header->num_nodes, sizeof(uint32_t), 1, file_);
  header->num_nodes = htonl(header->num_nodes);
  res = fread(&header->num_districts, sizeof(uint32_t), 1, file_);
  header->num_districts = htonl(header->num_districts);

  return SUCCESS;
}

uint16_t Reader::ReadNodeRecord(const uint32_t offset,
                                NodeRecord *record) {
  size_t res;

  if (file_ == nullptr) {
    return INVALID_FILE;
  }

  if (fseek(file_, offset, SEEK_SET) != 0) {
    return SEEK_FAILED;
  }

  res = fread(record, kNodeRecordSize, 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  record->num_neighbors = htonl(record->num_neighbors);
  record->node_pos = htonl(record->node_pos);

  return SUCCESS;
}

uint16_t Reader::ReadNode(const uint32_t offset,
                          const uint32_t num_neighbors,
                          Node *node) {
  size_t res;
  uint32_t i, temp;

  if (file_ == nullptr) {
    return INVALID_FILE;
  }

  if (fseek(file_, offset, SEEK_SET) != 0) {
    return SEEK_FAILED;
  }

  // Read node id.
  res = fread(&node->id_, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->id_ = htonl(node->id_);

  // Read area.
  res = fread(&node->area_, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->area_ = htonl(node->area_);

  for (i = 0; i < num_neighbors; i++) {
    // Read neighbor.
    res = fread(&temp, sizeof(uint32_t), 1, file_);
    if (res != 1) {
      return READ_FAILED;
    }
    node->neighbors_->insert(htonl(temp));
  }

  // Read total population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetTotalPop(htonl(temp));

  // Read AA population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAAPop(htonl(temp));

  // Read AI population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAIPop(htonl(temp));

  // Read AS population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetASPop(htonl(temp));

  // Read CA popuation.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetCAPop(htonl(temp));

  // Read other population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetOtherPop(htonl(temp));

  return SUCCESS;
}

// bool ValidateCheckSum(FILE *file, uint32_t checksum) {
//   boost::crc_32_type crc;
//   unsigned char byte;

//   if (file == nullptr || fseek(file, sizeof(Header), SEEK_SET) != 0) {
//     return false;
//   }

//   while (fread(&byte, 1, 1, file) != 0) {
//     crc.process_byte(byte);
//   }

//   return crc.checksum() == checksum;
// }

}     // namespace rakan
