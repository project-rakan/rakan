int main(int argc, char *argv[]) {
  if (argc != 6) {
    cerr << "Usage: ./" << argv[0] << " filepath alpha beta gamma eta" << endl;
  }

  // Open index file as a binary.
  FILE *file = fopen(argv[1], "rb");
  if (file == nullptr) {
    return EXIT_FAILURE;
  }

  // Initialize this engine.
  Graph *graph = rakan::LoadGraph(file);
  graph->SetAlpha(argv[1]);
  graph->SetBeta(argv[2]);
  graph->SetGamma(argv[3]);
  graph->SetEta(argv[4]);

  // Clean up and exit.
  fclose(file);
  return EXIT_SUCCESS;
}
