#!/bin/bash
. /home/project/configs/.secrets.sh

# launch postgres using values in . .secrets.sh
service postgresql start

# launch rabbitmq
rabbitmq-server -detached

# insert credentials into the database if it hasn't already
su - postgres -c "createdb $DATABASE_TABLE && psql -c \"CREATE EXTENSION postgis;\""
su - postgres -c "psql -c \"CREATE USER $DATABASE_USER with encrypted password '$DATABASE_PASS'\""
su - postgres -c "psql -c \"GRANT ALL PRIVILEGES ON DATABASE $DATABASE_TABLE TO $DATABASE_USER;\""
su - postgres -c "psql -c \"ALTER USER $DATABASE_USER WITH SUPERUSER;\""

# Start a build for the celery worker
make build

# Create migrations and whatnot
cd bladecaller
python3.7 manage.py makemigrations
python3.7 manage.py migrate
python3.7 manage.py populate_db
python3.7 manage.py create_frontend_json
python3.7 manage.py collectstatic --noinput
cd ..

# run nginx if it isn't running already
service nginx start

# Configure rabbit
rabbitmqctl add_user $RABBIT_USER $RABBIT_PASS
rabbitmqctl add_vhost $RABBIT_VHOST

# Launch celery
cd bladecaller
( celery -A bladecaller worker > $RAKAN_LOCATION/logs/celery.log ) &
cd ..

# start a shell if it's dev mode
if [ $DEBUG_MODE = true ]; then
    /bin/bash
else
    # run the gunicorn if it hasn't already
    cd bladecaller
    echo "Running Server..."
    gunicorn --bind 127.0.0.1:8000 --workers 3 bladecaller.wsgi > $RAKAN_LOCATION/logs/gunicorn.log
    cd ..
fi

