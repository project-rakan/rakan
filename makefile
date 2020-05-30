pip:
	python3.7 -m pip install -r requirements.txt

cmake: rakan/cRakan
	cd rakan/cRakan/build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make all

build: cmake
	cd rakan && python3.7 setup.py build_ext --inplace

clean:
	cd rakan && rm -rf *.so  cRakan/wrapper.cpp tests/*.so build
	cd bladecaller && rm -rf htmlcov .coverage
	cd logs && rm -rf *.log bladecaller/db.sqlite3
	rm -rf bladecaller/api/__pycache__ bladecaller/api/tests/__pycache__ bladecaller/api/migrations/__pycache__
	rm -rf bladecaller/bladecaller/__pycache__ rakan/__pycache__ rakan/tests/__pycache__

test: build cmake
	cd rakan && cp ./*.so tests/
	cd rakan/cRakan/build && ./tst/rakan_tst 
	cd rakan/tests && python3.7 -m unittest
	cd bladecaller && coverage run --source='.' manage.py test -v3 && coverage html

migrations: build
	cd bladecaller && python3.7 manage.py makemigrations
	cd bladecaller && python3.7 manage.py migrate

bladecaller: build
	cd bladecaller && python3.7 manage.py loaddata seed.json
	cd bladecaller && python3.7 manage.py collectstatic --no-input
	cd bladecaller && python3.7 manage.py runserver