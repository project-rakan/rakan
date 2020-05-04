#ifndef READERUTILS_H_
#define READERUTILS_H_

#include <inttypes.h>   // for int64_t
#include <arpa/inet.h>  // For htonl(), ntohl()

namespace rakan {

// The first four bytes of a valid index file.
extern const uint32_t kMagicNumber;

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

}         // namespace rakan

#endif    //  READERUTILS_H_
