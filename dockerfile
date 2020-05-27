FROM ubuntu:18.04

ENV DEBIAN_FRONTEND=noninteractive

# House keeping + development tools
RUN apt-get update --fix-missing
RUN apt-get install -y software-properties-common apt-utils
RUN add-apt-repository ppa:deadsnakes/ppa
RUN add-apt-repository ppa:ubuntugis/ppa
RUN apt-get install dpkg git -y
RUN apt-get update --fix-missing

# Download gcc
RUN apt-get install cmake gcc make g++ gdb -y

# Download Python
RUN apt-get install python3.7 python3-pip python3.7-dev -y

# Install GDAL
RUN apt-get install libgdal-dev gdal-bin -y

# Install the database
RUN apt-get install postgresql-10 -y
RUN apt-get install libpq-dev postgis -y
RUN ln -fs /usr/share/zoneinfo/America/Los_Angles /etc/localtime

# Install additional packages for scripts
RUN apt-get install wget unzip -y

# Install nginx + Gunicorn
RUN apt-get install nginx -y
RUN wget https://raw.githubusercontent.com/nginx/nginx/master/conf/uwsgi_params -O /etc/nginx/uwsgi_params
COPY ./configs/nginx.conf /etc/nginx/sites-enabled/nginx.conf
RUN rm /etc/nginx/sites-enabled/default

# Install python related components
COPY requirements.txt /tmp/requirements.txt
RUN python3.7 -m pip install -r /tmp/requirements.txt

# Install rabbitmq for celery
RUN apt-get install rabbitmq-server -y

# Download maps from bladecaller
ADD .gitignore /var/www/html/stateinfo/.gitignore
COPY configs/downloadMaps.sh /tmp/downloadMaps.sh
RUN chmod +x /tmp/downloadMaps.sh
RUN /tmp/downloadMaps.sh

WORKDIR "/home/project"

COPY . .

RUN touch "a.YOU ARE NOT MOUNTING YOUR DEVELOPMENT DRIVE.YOU ARE IN PRODUCTION MODE"
RUN touch "z.YOU ARE NOT MOUNTING YOUR DEVELOPMENT DRIVE.YOU ARE IN PRODUCTION MODE"

RUN chmod +x ./configs/docker-entrypoint.sh

CMD [ "./configs/docker-entrypoint.sh" ]

