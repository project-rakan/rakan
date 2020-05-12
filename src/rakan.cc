#include <stdlib.h>
#include <iostream>

#include "./Runner.h"
#include "./Queue.h"

#define NUM_STEPS 20
#define QUEUE_ADDRESS "amqp://norton:Norton00@bladecaller_queue"
#define QUEUE_NAME "rakan"

using rakan::Runner;

using battledance::StartMapJobRequest;
using battledance::Queue;
using battledance::Task;

int main(int argc, char *argv[]) {
  Queue queue(QUEUE_ADDRESS, QUEUE_NAME);
  Runner *runner = new Runner(&queue);

  while (1) {
    Task task;
    StartMapJobRequest *request;
    
    task = queue.GetNextTask();
    if (task.task_id == NOOP) {
      sleep(1);
      continue;
    }

    request = reinterpret_cast<StartMapJobRequest *>(task.payload);
    runner->StartMapJob(request, NUM_STEPS);

    fclose(file);
    delete g;
  }

  return EXIT_SUCCESS;
}
