#include <stdlib.h>
#include <iostream>

#include "./Graph.h"
#include "./Node.h"
#include "./Reader.h"
#include "./Runner.h"

#include "./Queue.h"

#define NUM_STEPS 20

using rakan::Runner;

using battledance::StartMapJobRequest;
using battledance::Queue;
using battledance::Task;

int main(int argc, char *argv[]) {
  Queue queue("amqp://norton:Norton00@bladecaller_queue", "rakan");
  Runner *runner = new Runner(&queue);

  int i = 0;
  while (1) {
    // StartMapJobRequest *x = new StartMapJobRequest();
    // strcpy(x->guid, "ia-test-bb02f343-087c-4bb6-b1ed-57d827793491");
    // strcpy(x->state, "IA");
    // x->alpha = 0;
    // x->beta = 0;
    // x->gamma = 0;
    // x->eta = 0;

    Task task = queue.GetNextTask();
    StartMapJobRequest *request = reinterpret_cast<StartMapJobRequest *>(task.payload);

    // ideally would pull idx file from database
    FILE *file = fopen("../tst/iowa.idx", "rb");
    runner->LoadGraph(file);

    Graph *g = runner->GetGraph();
    // g->SetAlpha(0);
    // g->SetBeta(0);
    // g->SetGamma(0);
    // g->SetEta(0);

    g->SetAlpha(request->alpha);
    g->SetBeta(request->beta);
    g->SetGamma(request->gamma);
    g->SetEta(request->eta);

    runner->SeedDistricts();
    runner->PopulateGraphData();
    runner->Walk(NUM_STEPS, request->guid);
    i++;
    fclose(file);
    delete g;
  }

  return EXIT_SUCCESS;
}
