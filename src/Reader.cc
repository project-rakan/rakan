#include "./Reader.h"

#include <iostream>

#include <arpa/inet.h>        // For ntohl()
#include <bits/stdc++.h>      // for std::unordered_set
#include <inttypes.h>         // for uint32_t
#include <stdio.h>            // for FILE *, fread, fseek
#include <unordered_map>      // for std::unordered_map

#include "./ErrorCodes.h"     // for error codes
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

  res = fread(header, sizeof(Header), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  header->magic_number = ToHostFormat(header->magic_number);
  header->checksum = ToHostFormat(header->checksum);
  header->num_nodes = ToHostFormat(header->num_nodes);
  header->num_districts = ToHostFormat(header->num_districts);

  return SUCCESS;
}

uint16_t Reader::ReadNodeRecord(const uint32_t offset,
                                NodeRecord *record) {
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
  // std::cout << std::hex << "record->num_neighbors = " << record->num_neighbors << endl;
  // std::cout << std::hex << "ecord->node_pos = " << record->node_pos << endl;
  record->num_neighbors = ToHostFormat(record->num_neighbors);
  record->node_pos = ToHostFormat(record->node_pos);

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
  node->id_ = ToHostFormat(node->id_);

  // Read area.
  res = fread(&node->area_, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->area_ = ToHostFormat(node->area_);

  for (i = 0; i < num_neighbors; i++) {
    // Read neighbor.
    res = fread(&temp, sizeof(uint32_t), 1, file_);
    if (res != 1) {
      return READ_FAILED;
    }
    node->neighbors_->insert(ToHostFormat(temp));
  }

  // Read total population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetTotalPop(ToHostFormat(temp));

  // Read AA population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAAPop(ToHostFormat(temp));

  // Read AI population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetAIPop(ToHostFormat(temp));

  // Read AS population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetASPop(ToHostFormat(temp));

  // Read CA popuation.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetCAPop(ToHostFormat(temp));

  // Read other population.
  res = fread(&temp, sizeof(uint32_t), 1, file_);
  if (res != 1) {
    return READ_FAILED;
  }
  node->SetOtherPop(ToHostFormat(temp));

  return SUCCESS;
}

uint32_t Reader::ToHostFormat(uint32_t x) {
  uint32_t ret, i;
  char *byte = (char *) &x;
  x = htonl(x);
  ret = x;

  // Read all 4 bytes.
  for (i = 0; i < 4; i++) {
    // If the leading byte is 0, remove it.
    if (*byte == 0) {
      ret = ret >> 8;
    } else {
      break;
    }
    byte++;
  }

  return ret;
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
