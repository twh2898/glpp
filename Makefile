
CC=/usr/bin/gcc
CXX=/usr/bin/g++
CONFIG=Debug
TARGET=all

all: init build

init:
	git submodule update --init --recursive
	cmake --no-warn-unused-cli \
		-DCMAKE_BUILD_TYPE=Debug \
		-DCMAKE_EXPORT_COMPILE_COMMANDS=TRUE \
		-DCMAKE_C_COMPILER=${CC} \
		-DCMAKE_CXX_COMPILER=${CXX} \
		-S${PWD} \
		-B${PWD}/build \
		-G Ninja

build:
	cmake --build ${PWD}/build --config ${CONFIG} --target ${TARGET} --

.PHONY: init build
