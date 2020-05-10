  
/**
 *  Libevent.cpp
 *
 *  Test program to check AMQP functionality based on Libevent
 *
 *  @author Brent Dimmig <brentdimmig@gmail.com>
 */

/**
 *  Dependencies
 */
#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include <unistd.h>
#include <string>

#include "./Queue.h"


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


// /**
//  *  Main program
//  *  @return int (compile with -levent flag)
//  */
// int main()
// {
//     // access to the event loop
    
//     struct event_base* evbase = event_base_new();

//     // handler for libevent
//     AMQP::LibEventHandler handler(evbase);

//     // make a connection
//     AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://guest:guest@bladecaller_queue"));

//     // we need a channel too
//     AMQP::TcpChannel channel(&connection);

//     // create a temporary queue
//     channel.declareQueue("rakan", AMQP::passive)
//     .onSuccess([&connection](const std::string &name, uint32_t messagecount, uint32_t consumercount) {

//         // report the name of the temporary queue
//         std::cout << "declared queue " << name << std::endl;
//     });

//     channel.get("rakan", AMQP::noack)
//     .onReceived(
//         [&connection](const AMQP::Message &msg, uint64_t tag, bool redelivered)
//         {
//             auto body = std::string(msg.body());
//             body.resize(msg.bodySize());
//             std::cout << "Received: " << body << std::endl;
//         }
//     );

//     // run the loop
//     event_base_dispatch(evbase);
//     event_base_free(evbase);

//     connection.close();

//     // done
//     return 0;
// }

#if 0
// Compile with -lev flag
#include <iostream>
#include <amqpcpp.h>
#include <amqpcpp/linux_tcp.h>
#include <ev.h>
#include <amqpcpp/libevent.h>

#include "./Queue.h"
#include "./handle.h"

int main(void)
{
    // create an instance of your own tcp handler
    // auto *loop = EV_DEFAULT;
    // MyHandler myHandler(loop);

    auto evbase = event_base_new();
    AMQP::LibEventHandler myHandler(evbase);

    std::cout << "created handler" << std::endl;

    // address of the server
    AMQP::Address address("amqp://guest:guest@bladecaller_queue");

    std::cout << "created address" << std::endl;

    // // create a AMQP connection object
    AMQP::TcpConnection connection(&myHandler, address);

    std::cout << "created connection" << std::endl;

    // // and create a channel
    std::cout << "created channel" << std::endl;
    AMQP::TcpChannel channel(&connection);

    // // use the channel object to call the AMQP method you like
    auto queueName = "rakan";
    auto exchangeName = "";
    auto keyName = "";

    // channel.declareExchange(exchangeName, AMQP::fanout)
    //     .onSuccess
    //     (
    //         [&]()
    //         {
    //             std::cout << "Exchange created." << std::endl;
    //             channel.declareQueue(AMQP::exclusive)
    //             .onSuccess
    //             (
    //                 [&channel, queueName, keyName, exchangeName](const std::string &name,
    //                                      uint32_t messagecount,
    //                                      uint32_t consumercount)
    //                 {
    //                     std::cout << "Queue created." << std::endl;
    //                     channel.declareQueue(queueName).onSuccess([&channel](const std::string &name, uint32_t messageCount, uint32_t consumerCount) {
    //                         std::cout << "Queue '" << name << "' has been declared with " << messageCount << " messages and " << consumerCount << " consumers" << std::endl;
    //                         channel.publish("", "hello_world", "my first message from C++");
    //                     });
    //                     channel.bindQueue(exchangeName, queueName, keyName);
    //                     channel.consume(queueName, AMQP::noack);
    //                 }
    //             );
    //         }
    //     );


    channel.declareQueue(queueName, AMQP::passive)
    // channel.bindQueue(exchangeName, queueName, queueName)
    .onSuccess([&connection](const std::string &name,
                        uint32_t messagecount,
                        uint32_t consumercount) {
        std::cout << "Queue: " << name << std::endl;
    })
    .onError([](const char *message) {
        std::cout << "error: " << message << std::endl;
    });
    // start a queue
    // channel.declareQueue(AMQP::exclusive).onSuccess([&channel](const std::string &name, uint32_t messageCount, uint32_t consumerCount) {
    //     std::cout << "Queue '" << name << "' has been declared with " << messageCount << " messages and " << consumerCount << " consumers" << std::endl;
    //     channel.publish("", "hello_world", "my first message from C++");
    // });

    // publish a number of messages
    

    // commit the transactions, and set up callbacks that are called when
    // the transaction was successful or not
    // channel.commitTransaction()
    //     .onSuccess([]() {
    //         // all messages were successfully published
    //     })
    //     .onError([](const char *message) {
    //         // none of the messages were published
    //         // now we have to do it all over again
    //     });

    // event_base_dispatch(loop);
    // ev_run(loop, 0);
    // event_base_free(loop);
    
    // run the loop
    event_base_dispatch(evbase);

    event_base_free(evbase);

    // channel.publish(exchangeName, queueName, "Hello, world!");
    // channel.consume(queueName, AMQP::noack)
    //     .onReceived
    //     (
    //         [](const AMQP::Message &msg, uint64_t tag, bool redelivered)
    //         {
    //             std::cout << "Received: " << msg.body() << std::endl;
    //         }
    //     );

    return 0;
}
#endif