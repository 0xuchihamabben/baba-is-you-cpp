all:
	mkdir -p build
	cmake -B build .
	cd build && make -j8
	./build/BabaIsYou
