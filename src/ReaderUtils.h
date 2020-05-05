#ifndef READERUTILS_H_
#define READERUTILS_H_

#include <arpa/inet.h>      // For htonl(), ntohl()
// #include <boost\crc.hpp>    // for crc32 checksum
#include <inttypes.h>       // for int64_t
#include <stdio.h>          // for FILE *, fread, fseek

#include "Reader.h"         // for Header

namespace rakan {

// The first four bytes of a valid index file.
const uint32_t kMagicNumber = 0xBEEFCAFE;

// Macro to convert from network to host byte order.
#define ntohll(x) \
  ( ((uint64_t) (ntohl((uint32_t)((x << 32) >> 32))) << 32) |   \
    ntohl(((uint32_t)(x >> 32))) )

// Macro to convert from host to network byte order.
#define htonll(x) (ntohll(x))

// Macro for disabling copy constructors and assignment operators.
#define DISALLOW_COPY_AND_ASSIGN(TypeName) \
  TypeName(const TypeName&) = delete;      \
  void operator=(const TypeName&) = delete

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

}         // namespace rakan

#endif    //  READERUTILS_H_
