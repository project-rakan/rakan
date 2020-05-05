#ifndef READERUTILS_H_
#define READERUTILS_H_

#include <arpa/inet.h>      // For htonl(), ntohl()
// #include <boost\crc.hpp>    // for crc32 checksum
#include <inttypes.h>       // for int64_t
#include <stdio.h>          // for FILE *, fread, fseek

namespace rakan {

// The first four bytes of a valid index file.
const uint32_t kMagicNumber = 0xBEEFCAFE;

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
