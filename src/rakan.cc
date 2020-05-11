#include <stdlib.h>
#include <iostream>

#include "./Graph.h"
#include "./Node.h"
#include "./Reader.h"
#include "./Runner.h"

#include "./Queue.h"

using rakan::Runner;

using battledance::StartMapJobRequest;
using battledance::Queue;
using battledance::Task;
using std::cout;
using std::cerr;
using std::endl;

void print_seeding(Graph *g) {
  unordered_set<int> *set;
  for (int i = 0; i < g->GetNumDistricts(); i++) {
    cout << "district " << i << " = [";
    set = g->GetNodesInDistrict(i);
    for (auto& id : *set) {
      cout << id << ", ";
    }
    cout << "]" << endl;
  }
}

int main(int argc, char *argv[]) {
  if (argc != 2) {
    cerr << "usage: " << argv[0] << " num_steps" << endl;
  }

  Queue queue("amqp://norton:Norton00@bladecaller_queue", "rakan");
  Runner runner(&queue);

  while (1) {
    Task task = queue.GetNextTask();
    if (task.task_id == START_MAP) {
      StartMapJobRequest *request = reinterpret_cast<StartMapJobRequest *>(task.payload);

      // ideally would pull idx file from database
      FILE *file = fopen("../tst/iowa.idx", "rb");
      runner.LoadGraph(file);

      Graph *g = runner.GetGraph();
      g->SetAlpha(request->alpha);
      g->SetBeta(request->beta);
      g->SetGamma(request->gamma);
      g->SetEta(request->eta);

      runner.SeedDistricts();
      runner.PopulateGraphData();
      runner.Walk(atoi(argv[1]), request->guid);
    } else {
      // TODO
    }
  }

  return EXIT_SUCCESS;
}
