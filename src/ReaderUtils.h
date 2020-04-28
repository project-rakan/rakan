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

class CRC {
 public:
  // Default constructor to make a CRC object.
  CRC(void) {}

  // Fold the next byte into the CRC.
  //
  // Arguments:
  //  - byte: the byte to fold into this CRC
  //
  // Returns:
  //  - true iff folding successful
  //  - false otherwise
  bool FoldIntoBytes(uint8_t byte);

  // Returns the final state of the CRC.
  uint32_t GetFinalState();

 private:
  // Initialize the table_ to the appropriate values according to the
  // CRC32 algorithm.
  void Initialize(void);

  // This private member variable holds the CRC calculation state.
  uint32_t state_;

  // This bool indicates whether or not the CRC has been finalized.
  bool finalized_;

  // This indicates whether the static table_ has been initialized.
  static bool tableIsInitialized_;
};

}         // namespace rakan

#endif    //  READERUTILS_H_