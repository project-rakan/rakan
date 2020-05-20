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

COPY requirements.txt /tmp/requirements.txt
RUN python3.7 -m pip install -r /tmp/requirements.txt

WORKDIR "/home/project"

COPY . .

RUN touch "a.YOU ARE NOT MOUNTING YOUR DEVELOPMENT DRIVE.YOU ARE IN PRODUCTION MODE"
RUN touch "z.YOU ARE NOT MOUNTING YOUR DEVELOPMENT DRIVE.YOU ARE IN PRODUCTION MODE"

ENTRYPOINT [ "./entrypoint.sh" ]