#include <cstdint>
#include <unordered_map>
#include <sstream>

#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include <unistd.h>


#include <string>

#include <iostream>

#define START_MAP 0
#define JUDGE_MAP 1
#define BLOCKING_RETRY_DURATION 0.1
#define XAYAH_QUEUE "xayah"
#define BLADECALLER_DATABASE "bladecaller"

#ifndef BATTLEDANCE_QUEUE_H_
#define BATTLEDANCE_QUEUE_H_

/** Structs */

using namespace std;

namespace battledance {

typedef struct event_base* eventBasePtr;

typedef struct StartMapJobRequestStruct {
    char state[2];
    char guid[128];
    double alpha;
    double beta;
    double gamma;
    double eta;
} StartMapJobRequest;

typedef struct MapScoreRequestStruct {
    char guid[128];
    char state[2];
    unordered_map<int32_t, int32_t> map;
    double alpha;
    double beta;
    double gamma;
    double eta;
} MapScoreRequest;

typedef struct MapScoreResponseStruct {
    char guid[128];
    double score;
    double compactness;
    double distribution;
    double borderRespect;
    double vra;
    double alpha;
    double beta;
    double gamma;
    double eta;
} MapScoreResponse;

// Being sent for beta release
typedef struct MapJobUpdateStruct {
    char guid[128];
    char state[2];
    unordered_map<int32_t, int32_t> map;
    double alpha;
    double beta;
    double gamma;
    double eta;
} MapJobUpdate;

typedef struct TaskStruct {
    int8_t task_id;
    void * payload;
} Task;

typedef struct QueueConnectionRequirementStructs {
    eventBasePtr evbase;
    AMQP::LibEventHandler* handler;
    AMQP::TcpConnection* connection;
    AMQP::TcpChannel* channel;
} QueueConnectionRequirement;

/** Classes */

class Queue {
  public:
    /** Connect to rabbitmq here */
    Queue(const string &address, const string &identifier)
    : address_(address),
      identifier_(identifier)
    { };

    /** Retrieve the next task. Blocks. */
    Task GetNextTask() {
        // access to the event loop
        bool done = false;

        // Moment of truth...
        QueueConnectionRequirement * queueConnection = setUpConnection();
        eventBasePtr evbase = queueConnection->evbase;
        AMQP::TcpChannel * channel = queueConnection->channel;
        AMQP::TcpConnection * connection = queueConnection->connection;

        auto identifier = identifier_;

        // Setup the call backs to do: connect to the queue and get one message
        channel->declareQueue(identifier_, AMQP::passive)
        .onSuccess([&connection, &channel, &done, &identifier](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
            channel->get(identifier, AMQP::noack)
            .onReceived([&connection, &done, &identifier](const AMQP::Message &msg, uint64_t tag, bool redelivered) {
                auto body = std::string(msg.body());
                body.resize(msg.bodySize());
                done = true;
                connection->close();
            });
        });

        // fire off the async commands
        event_base_dispatch(evbase);
        event_base_free(evbase);

        // Wait until it's finished
        while (!done) {
            sleep(BLOCKING_RETRY_DURATION);
        }

        tearDownConnection(queueConnection);

        // TODO: parse the payload

        StartMapJobRequest * x = new StartMapJobRequest();
        strcpy(x->guid, "ia-test-bb02f343-087c-4bb6-b1ed-57d827793491");
        // x->guid;
        strcpy(x->state, "IA");
        x->alpha = 0;
        x->beta = 0;
        x->gamma = 0;
        x->eta = 0;

        Task task = Task();
        task.payload = x;
        task.task_id = START_MAP;
        return task;
    }

    void SubmitMapScore(MapScoreResponse &mapScoreResponse) {
        // Moment of truth...
        QueueConnectionRequirement * queueConnection = setUpConnection();
        eventBasePtr evbase = queueConnection->evbase;
        AMQP::TcpChannel * channel = queueConnection->channel;
        AMQP::TcpConnection * connection = queueConnection->connection;

        // Assemble our string
        std::stringstream jsonPayload;
        jsonPayload << "{";
        jsonPayload << "\"guid\": \"" << mapScoreResponse.guid << "\",";
        jsonPayload << "\"score\": " << mapScoreResponse.score << ",";
        jsonPayload << "\"compactness\": " << mapScoreResponse.compactness << ",";
        jsonPayload << "\"distribution\": " << mapScoreResponse.distribution << ",";
        jsonPayload << "\"borderRespect\": " << mapScoreResponse.borderRespect << ",";
        jsonPayload << "\"vra\": " << mapScoreResponse.vra << ",";
        jsonPayload << "\"alpha\": " << mapScoreResponse.alpha << ",";
        jsonPayload << "\"beta\": " << mapScoreResponse.beta << ",";
        jsonPayload << "\"gamma\": " << mapScoreResponse.gamma << ",";
        jsonPayload << "\"eta\": " << mapScoreResponse.eta;
        jsonPayload << "}";

        // Setup the call back to communicate to bladecaller distribution scores, so bladecaller can calculate a probability
        channel->declareQueue(BLADECALLER_DATABASE, AMQP::passive)
        .onSuccess([&connection, &channel, &jsonPayload](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
            channel->publish("", BLADECALLER_DATABASE, jsonPayload.str());
            connection->close();
        });

        // fire off the async commands
        event_base_dispatch(evbase);
        event_base_free(evbase);

        tearDownConnection(queueConnection);
    }; // For sending a score

    void SubmitRunUpdate(MapJobUpdate &mapJobUpdate) {
        // Moment of truth...
        QueueConnectionRequirement * queueConnection = setUpConnection();
        eventBasePtr evbase = queueConnection->evbase;
        AMQP::TcpChannel * channel = queueConnection->channel;
        AMQP::TcpConnection * connection = queueConnection->connection;

        // Assemble our string

        std::stringstream jsonPayload;
        jsonPayload << "{";
        jsonPayload << "\"guid\": \"" << mapJobUpdate.guid << "\",";
        jsonPayload << "\"state\": \"" << mapJobUpdate.state << "\",";
        jsonPayload << "\"map\": [";
        int32_t i = 0;
        for (std::pair<int32_t, int32_t> element : mapJobUpdate.map) {
            jsonPayload << "[" << element.first << ", " << element.second << "]";
            if (++i != mapJobUpdate.map.size()) {
                jsonPayload << ",";
            }
        }
        jsonPayload << "],";
        jsonPayload << "\"alpha\": " << mapJobUpdate.alpha << ",";
        jsonPayload << "\"beta\": " << mapJobUpdate.beta << ",";
        jsonPayload << "\"gamma\": " << mapJobUpdate.gamma << ",";
        jsonPayload << "\"eta\": " << mapJobUpdate.eta;
        jsonPayload << "}";

        // Setup the call backs to do: connect to TWO queues and push TWO messages
        // channel->declareQueue(XAYAH_QUEUE, AMQP::passive)
        // .onSuccess([&connection, &channel, &jsonPayload](const std::string &name, uint32_t messagecount, uint32_t consumercount) {

        std::cout << "SENDING: " << jsonPayload.str() << std::endl;
        
        channel->declareQueue(BLADECALLER_DATABASE, AMQP::passive)
        .onSuccess([&connection, &channel, &jsonPayload](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
            // channel->publish("", XAYAH_QUEUE, jsonPayload.str());
            channel->publish("", BLADECALLER_DATABASE, jsonPayload.str());
            connection->close();
        });
        // });

        // fire off the async commands
        event_base_dispatch(evbase);
        event_base_free(evbase);

        tearDownConnection(queueConnection);
    }; // For submitting new changes

    /** Destroy connection */
    ~Queue() {
    }
  private:
    std::string address_;
    std::string identifier_;
    bool queueActive_;

    QueueConnectionRequirement* setUpConnection() {
        // handler for libevent
        auto *queueConnection = new QueueConnectionRequirement();
        
        queueConnection->evbase = event_base_new();
        queueConnection->handler = new AMQP::LibEventHandler(queueConnection->evbase);
        queueConnection->connection = new AMQP::TcpConnection(&*(queueConnection->handler), AMQP::Address(address_));
        queueConnection->channel = new AMQP::TcpChannel(&*(queueConnection->connection));

        return queueConnection;
    };

    void tearDownConnection(QueueConnectionRequirement* queueConnection) {
        delete queueConnection->handler;
        delete queueConnection->connection;
        delete queueConnection->channel;
        delete queueConnection;
    }

};

}

#endif