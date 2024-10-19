BUILD_DIR_DEBUG=$(shell pwd)/build-x86-Debug
BUILD_DIR_RELEASE=$(shell pwd)/build-x86-Release

format:
	pre-commit run --all-files

$(shell pwd)/build-x86-%/Makefile: $(shell find -name CMakeLists.txt)
	@echo Configure $*
	cmake -S . -B build-x86-$* -C cmake_presets/x86-$*.cmake --graphviz=build-x86-$*/build-documentation/cmake.dot

configure_debug: $(BUILD_DIR_DEBUG)/Makefile

configure_release: $(BUILD_DIR_RELEASE)/Makefile

configure: configure_debug configure_release


cfe_debug:
	cd embedded_software/middleware/NASA-cFS/ && make BUILDTYPE=release OMIT_DEPRECATED=true prep O=$(BUILD_DIR_DEBUG)/cfe_integration/
	cd $(BUILD_DIR_DEBUG)/cfe_integration/ && make all

clean:
	rm -rf build*
	rm -rf output/

documentation: configure_release
	cd $(BUILD_DIR_RELEASE) && make documentation

build_debug: configure_debug
	cmake --build $(BUILD_DIR_DEBUG) --parallel $(shell $(nproc))

build_release: configure_release
	cmake --build $(BUILD_DIR_RELEASE) --parallel $(shell $(nproc))

test: build_release
	ctest --test-dir $(BUILD_DIR_RELEASE) -C Release -L "unit_tests" --output-on-failure

test_debug: build_debug
	ctest --test-dir $(BUILD_DIR_DEBUG) -C Debug -L "unit_tests" --output-on-failure

quality: test_debug
	ctest --test-dir $(BUILD_DIR_DEBUG) -C Debug -L "quality" --output-on-failure

performance: build_release
	ctest --test-dir $(BUILD_DIR_RELEASE) -C Release -L "performance" --output-on-failure

validate: quality test performance

packaging: configure_debug
	cd $(BUILD_DIR_RELEASE) && make package

all: packaging documentation

.PHONY: all clean test configure validate
