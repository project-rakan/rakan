pip:
	python3.7 -m pip install -r requirements.txt

build: rakan/cRakan
	cd rakan && python3.7 setup.py build_ext --inplace

cmake: rakan/cRakan
	cd rakan/cRakan/build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make all

clean:
	cd rakan && rm -rf *.so  cRakan/wrapper.cpp tests/*.so build

docs: pip
	mkdocs serve -a 0.0.0.0:8080

test: build
	cd rakan && cp ./*.so tests/
	cd rakan/tests && python3.7 -m unittest