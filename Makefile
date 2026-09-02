
MAKEFLAGS += --silent

.PHONY: build
build:
	cmake -B build -G Ninja
	cmake --build build

.PHONY: clean
clean:
	rm -rf build
