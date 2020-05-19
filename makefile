pip:
	python3.7 -m pip install -r requirements.txt

build: rakan/cRakan
	cd rakan && python3.7 setup.py build_ext --inplace

clean:
	cd rakan && rm -rf *.so  cRakan/wrapper.cpp tests/*.so build

docs: pip
	mkdocs serve -a 0.0.0.0:8080