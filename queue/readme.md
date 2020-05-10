# Rakan.BattleDance
_(Sense the theme yet?)_

In `Queue.h`, contains a class called `Queue`. Leverage it to communicate to the RabbitMQ.

An example program is:
```cpp
int main() {
    // Sample program demonstrating how to use it. Use the constructor defined below. 
    // Do not alter its input values, unless if you've changed the docker-compose file.
    battledance::Queue queue("amqp://guest:guest@bladecaller_queue", "rakan");
    battledance::Task task = queue.GetNextTask();
    battledance::MapScoreResponse response;
    const char * dummyGuid = "1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567";
    strcpy(response.guid, dummyGuid);
    response.score = 72359.34;
    response.probability = 0.34;
    std::cout << "submitting map score" << std::endl;
    queue.SubmitMapScore(response);
}
```