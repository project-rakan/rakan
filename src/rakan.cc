#include <stdlib.h>
#include <iostream>

#include "./Graph.h"
#include "./Node.h"
#include "./Reader.h"
#include "./Runner.h"

#include "./Queue.h"

using rakan::Runner;

using battledance::Queue;
using battledance::Task;
using std::cerr;
using std::endl;

int main(int argc, char *argv[]) {
  Queue queue("amqp://guest:guest@bladecaller_queue", "rakan");

  while (1) {
    Task task = queue.GetNextTask();
  }

  return EXIT_SUCCESS;
}
