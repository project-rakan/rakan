#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "../src/ErrorCodes.h"
#include "../src/Reader.h"

using std::cout;
using std::cerr;
using std::endl;
using rakan::Header;
using rakan::Reader;

void PrintHeader(Header *header) {
  cout << "Header = {" << endl;
  cout << "    magic_number = " << std::hex << header->magic_number << endl;
  cout << "    checksum = " << std::hex << header->checksum << endl;
  cout << "    state = " << header->state[0] << header->state[1] << endl;
  cout << "    num_nodes = " << std::dec << header->num_nodes << endl;
  cout << "    num_districts = " << std::dec << header->num_districts << endl;
  cout << "}" << endl;
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "Usage: ./" << argv[0] << " filepath" << endl;
  }

  // Open index file as a binary.
  FILE *file = fopen(argv[1], "rb");
  if (file == nullptr) {
    perror("Cannot open file: ");
    return EXIT_FAILURE;
  }

  Reader reader(file);

  Header header;
  if (reader.ReadHeader(&header) != SUCCESS) {
    perror("ReadHeader failed");
    return EXIT_FAILURE;
  }
  PrintHeader(&header);

  fclose(file);
  return EXIT_SUCCESS;
}
