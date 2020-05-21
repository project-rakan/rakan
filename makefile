pip:
	python3.7 -m pip install -r requirements.txt

build: rakan/cRakan
	cd rakan && python3.7 setup.py build_ext --inplace

cmake: rakan/cRakan
	cd rakan/cRakan/build && cmake .. -DCMAKE_BUILD_TYPE=Debug -G "Unix Makefiles" && make all

clean:
	cd rakan && rm -rf *.so  cRakan/wrapper.cpp tests/*.so build

test: build cmake
	cd rakan && cp ./*.so tests/
	cd rakan/tests && python3.7 -m unittest
	cd rakan/cRakan/build && ./tst/rakan_tst 
