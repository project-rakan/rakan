#include <stdlib.h>
#include <iostream>

#include "./Graph.h"
#include "./Node.h"
#include "./Reader.h"
#include "./Runner.h"

using rakan::Runner;

using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {
  if (argc != 6) {
    cerr << "Usage: " << argv[0] << " filepath alpha beta gamma eta" << endl;
  }

  // Open index file as a binary.
  FILE *file = fopen(argv[1], "rb");
  if (file == nullptr) {
    return EXIT_FAILURE;
  }

  // Initialize this engine.
  Runner runner;
  runner.LoadGraph(file);

  float alpha, beta, gamma, eta;
  sscanf(argv[2], "%f", &alpha);
  sscanf(argv[3], "%f", &beta);
  sscanf(argv[4], "%f", &gamma);
  sscanf(argv[5], "%f", &eta);

  runner.GetGraph()->SetAlpha(alpha);
  runner.GetGraph()->SetBeta(beta);
  runner.GetGraph()->SetGamma(gamma);
  runner.GetGraph()->SetEta(eta);

  // Clean up and exit.
  fclose(file);
  return EXIT_SUCCESS;
}
