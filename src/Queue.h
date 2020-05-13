#include <cstdint>
#include <unordered_map>
#include <sstream>

#include <event2/event.h>
#include <amqpcpp.h>
#include <amqpcpp/libevent.h>
#include <unistd.h>
#include <boost/algorithm/string.hpp>
#include <bits/stdc++.h> 

#include <string>

#include <iostream>

#define NOOP -1
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
    char *guid;
    char *state;
    unordered_map<int32_t, int32_t> *map;
    double alpha;
    double beta;
    double gamma;
    double eta;
    double compactness;
    double distribution;
    double borderRespect;
    double vra;
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
        bool createTask = false;

        // Moment of truth...
        QueueConnectionRequirement * queueConnection = setUpConnection();
        eventBasePtr evbase = queueConnection->evbase;
        AMQP::TcpChannel * channel = queueConnection->channel;
        AMQP::TcpConnection * connection = queueConnection->connection;

        channel->onError([&connection, &evbase](const char* message) {
            std::cerr << "Something went wrong: " << message << std::endl;
            event_base_loopbreak(evbase);
            connection->close();
        });

        auto identifier = identifier_;

        string body;
        // Setup the call backs to do: connect to the queue and get one message
        channel->declareQueue(identifier_, AMQP::passive)
        .onSuccess([&connection, &channel, &done, &identifier, &body, &createTask](const std::string &name, uint32_t messagecount, uint32_t consumercount) {
            channel->get(identifier, AMQP::noack)
            .onReceived([&connection, &done, &identifier, &body, &createTask](const AMQP::Message &msg, uint64_t tag, bool redelivered) {
                body = std::string(msg.body());
                body.resize(msg.bodySize());
                done = true;
                createTask = true;
                connection->close();
            })
            .onEmpty([&connection, &done]() {
                std::cout << "Queue is empty" << std::endl;
                done = true;
                connection->close();
            });
        })
        .onError([&connection, &done](const char* message) {
            done = true;
            connection->close();
        });

        // fire off the async commands
        event_base_dispatch(evbase);
        event_base_free(evbase);

        // Wait until it's finished
        while (!done) {
            sleep(BLOCKING_RETRY_DURATION);
        }

        tearDownConnection(queueConnection);

        // parse the payload
        Task task = Task();

        if (createTask) {
            StartMapJobRequestStruct * request = new StartMapJobRequestStruct;
            vector<string> pairs;
            boost::split(pairs, body, boost::is_any_of(","));
            for(uint32_t i = 0; i < pairs.size(); i++) {
                if (pairs[i] == "") {
                continue;
                }
                vector<string> key_val;
                boost::split(key_val,pairs[i],boost::is_any_of(":"));
                string result("");
                for(uint32_t j = 0; j <  key_val[1].length(); j++) {
                if (isalpha(key_val[1][j]) || key_val[1][j] == '.'
                || (key_val[1][j] >= 48 && key_val[1][j] <= 57)) {
                result += tolower(key_val[1][j]);
                }
                }
                string key("");
                for(uint32_t j = 0; j <  key_val[0].length(); j++) {
                if (isalpha(key_val[0][j])) {
                key += tolower(key_val[0][j]);
                }
                }
                if (key == "guid") {
                for (uint32_t k = 0; k < result.length(); k++) {
                request->guid[k] = result[k];
                }
                } else if (key == "state") {
                request->state[0] = result[0];
                request->state[1] = result[1];
                } else if (key == "alpha") {
                string::size_type sz;
                request->alpha = stod(result, &sz);
                } else if (key == "beta") {
                string::size_type sz;
                request->beta = stod(result, &sz);
                } else if (key == "gamma") {
                string::size_type sz;
                request->gamma = stod(result, &sz);
                } else if (key == "eta") {
                string::size_type sz;
                request->eta = stod(result, &sz);
                }
            }
            
            task.payload = request;
            task.task_id = START_MAP;
        } else {
            // Queue was empty
            task.payload = nullptr;
            task.task_id = NOOP;
        }
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
        jsonPayload << "\"guid\": \""; 
        for (int i = 0; i < 6; i++) {
            jsonPayload << mapJobUpdate.guid[i];
        }
        jsonPayload << "\",";
        jsonPayload << "\"state\": \"" << mapJobUpdate.state[0] << mapJobUpdate.state[1] << "\",";
        jsonPayload << "\"map\": [";
        int32_t i = 0;
        for (std::pair<int32_t, int32_t> element : *mapJobUpdate.map) {
            std::cout << "element.first = " << element.first << std::endl;
            std::cout << "element.second = " << element.second << std::endl;
            jsonPayload << "[" << element.first << ", " << element.second << "]";
            if (++i != mapJobUpdate.map->size()) {
                jsonPayload << ",";
            }
        }
        jsonPayload << "],";
        jsonPayload << "\"alpha\": " << mapJobUpdate.alpha << ",";
        jsonPayload << "\"beta\": " << mapJobUpdate.beta << ",";
        jsonPayload << "\"gamma\": " << mapJobUpdate.gamma << ",";
        jsonPayload << "\"eta\": " << mapJobUpdate.eta << ",";
        jsonPayload << "\"compactness\": " << mapJobUpdate.compactness << ",";
        jsonPayload << "\"distribution\": " << mapJobUpdate.distribution << ",";
        jsonPayload << "\"borderRespect\": " << mapJobUpdate.borderRespect << ",";
        jsonPayload << "\"vra\": " << mapJobUpdate.vra;
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