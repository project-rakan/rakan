#!/bin/bash
. /home/project/configs/.secrets.sh

# launch postgres using values in . .secrets.sh
service postgresql start

# insert credentials into the database if it hasn't already
su - postgres -c "createdb $DATABASE_TABLE"
su - postgres -c "psql -c \"CREATE USER $DATABASE_USER with encrypted password '$DATABASE_PASS'\""
su - postgres -c "psql -c \"GRANT ALL PRIVILEGES ON DATABASE $DATABASE_TABLE TO $DATABASE_USER;\""
su - postgres -c "psql -c \"ALTER USER $DATABASE_USER WITH SUPERUSER;\""

# Create migrations and whatnot
cd bladecaller
python3.7 manage.py makemigrations
python3.7 manage.py migrate
python3.7 manage.py collectstatic --noinput
cd ..

# run nginx if it isn't running already
service nginx start

# run the gunicorn if it hasn't already
cd bladecaller
gunicorn --bind 127.0.0.1:8000 --workers 3 bladecaller.wsgi &> /home/project/logs/gunicorn.log &
cd ..

# start a shell if it's dev mode
if [ $DEBUG_MODE = true ]; then
    /bin/bash
else
    exec $@
fi

