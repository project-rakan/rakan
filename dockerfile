FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

# House keeping + development tools
RUN apt-get update --fix-missing
RUN apt-get clean
RUN apt-get update
RUN apt-get install dpkg git -y

# Download gcc
RUN apt-get install cmake gcc make g++ gdb -y

# Download boost library
RUN apt-get install libboost-all-dev -y

# Build and install SSL
RUN apt install libssl-dev -y

RUN apt-get install libev-dev libevent-dev -y

# Build and install AMQP
RUN git clone https://github.com/CopernicaMarketingSoftware/AMQP-CPP /tmp/amqp
RUN mkdir /tmp/amqp/build
WORKDIR /tmp/amqp/
RUN make
RUN make install

# Download Python
RUN apt-get install python -y

WORKDIR "/home/project"

COPY . .

RUN mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make all

WORKDIR "/home/project/build/"

ENTRYPOINT [ "src/rakan_run" ]

