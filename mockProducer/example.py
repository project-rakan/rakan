import pika
import time
import json

while True:
    try:
        connection = pika.BlockingConnection(pika.ConnectionParameters('bladecaller_queue'))
    except pika.exceptions.AMQPConnectionError as err:
        print('rabbitmq connection failed; retrying in 1 second...')
        time.sleep(1)
    else:
        break

channel = connection.channel()
# channel.exchange_declare(exchange='rakan', exchange_type='fanout')
result = channel.queue_declare(queue='rakan')

# channel.queue_bind(exchange='rakan', queue=result.method.queue)

for i in range(100):
    # make a lot of noise
    channel.basic_publish(exchange='',
                        routing_key='rakan',
                        body=json.dumps({
                            "key": "value"
                        }))
    print(f" [x] Sent json payload #{i}")

i = 100

while True:
    for j in range(10):
        channel.basic_publish(exchange='',
                            routing_key='rakan',
                            body=json.dumps({
                                "key": "value"
                            }))
        print(f" [x] Sent json payload #{i}")

        i += 1
    
    time.sleep(2)

connection.close()

"""
// Okay, so the C++ code appears as so:

#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include <unistd.h>
#include <string>


/**
 *  Main program
 *  @return int (compile with -levent flag)
 */
int main()
{
    // access to the event loop
    auto evbase = event_base_new();

    // handler for libevent
    AMQP::LibEventHandler handler(evbase);

    // make a connection
    AMQP::TcpConnection connection(&handler, AMQP::Address("amqp://guest:guest@bladecaller_queue"));

    // we need a channel too
    AMQP::TcpChannel channel(&connection);

    // create a temporary queue
    channel.declareQueue("rakan", AMQP::passive)
    .onSuccess([&connection](const std::string &name, uint32_t messagecount, uint32_t consumercount) {

        // report the name of the temporary queue
        std::cout << "declared queue " << name << std::endl;
    });

    channel.get("rakan", AMQP::noack)
    .onReceived(
        [&connection](const AMQP::Message &msg, uint64_t tag, bool redelivered)
        {
            auto body = std::string(msg.body());
            body.resize(msg.bodySize());
            std::cout << "Received: " << body << std::endl;
            connection.close();
        }
    );

    // run the loop
    event_base_dispatch(evbase);
    event_base_free(evbase);

    // done
    return 0;
}
"""