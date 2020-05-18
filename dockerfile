FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

# House keeping + development tools
RUN apt-get update --fix-missing
RUN apt-get install -y software-properties-common apt-utils
RUN add-apt-repository ppa:deadsnakes/ppa
RUN apt-get install dpkg git -y
RUN apt-get update --fix-missing

# Download gcc
RUN apt-get install cmake gcc make g++ gdb -y

# Download boost library
RUN apt-get install libboost-all-dev -y

# Download Python
RUN apt-get install python3.7 python3-pip python3.7-dev -y

WORKDIR "/home/project"

COPY . .

RUN mkdir build && cd build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make all

WORKDIR "/home/project/build/"

ENTRYPOINT [ "src/rakan_run" ]

