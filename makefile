pip:
	python3.7 -m pip install -r requirements.txt

build: rakan/cRakan
	cd rakan && python3.7 setup.py build_ext --inplace

cmake: rakan/cRakan
	cd rakan/cRakan/build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make all

clean:
	cd rakan && rm -rf *.so  cRakan/wrapper.cpp tests/*.so build
	cd bladecaller && rm -rf htmlcov .coverage
	cd logs && rm -rf *.log
	rm -rf bladecaller/api/__pycache__ bladecaller/api/tests/__pycache__ bladecaller/api/migrations/__pycache__
	rm -rf bladecaller/bladecaller/__pycache__ rakan/__pycache__ rakan/tests/__pycache__

test: build cmake
	cd bladecaller && coverage run --source='.' manage.py test && coverage html
	cd rakan && cp ./*.so tests/
	cd rakan/tests && python3.7 -m unittest
	cd rakan/cRakan/build && ./tst/rakan_tst 

migrations:
	cd bladecaller && python3.7 manage.py makemigrations
	cd bladecaller && python3.7 manage.py migrate

bladecaller-dev:
	cd bladecaller && python3.7 manage.py test
	cd bladecaller && python3.7 manage.py loaddata seed.json
	cd bladecaller && python3.7 manage.py collectstatic --no-input
	cd bladecaller && python3.7 manage.py runserver