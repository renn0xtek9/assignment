BUILD_DIR_DEBUG=build-x86-Debug
BUILD_DIR_RELEASE=build-x86-Release

format:
	pre-commit run --all-files

build-x86-%/Makefile: $(shell find -name CMakeLists.txt)
	@echo Configure $*
	cmake -S . -B build-x86-$* -DCMAKE_BUILD_TYPE=$* -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DCMAKE_INSTALL_PREFIX=/usr/ --graphviz=build-x86-$*/cmake.dot

configure_debug: $(BUILD_DIR_DEBUG)/Makefile

configure_release: $(BUILD_DIR_RELEASE)/Makefile

configure: configure_debug configure_release

clean:
	rm -rf build*
	rm -rf output/

documentation: configure_debug
	cd $(BUILD_DIR_DEBUG) && make requirements_document
	cd $(BUILD_DIR_DEBUG) && make code_documentation
	cd $(BUILD_DIR_DEBUG) && make software_build_process_documentation
	cd $(BUILD_DIR_DEBUG) && make package

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

packaging: validate
	cd $(BUILD_DIR_RELEASE) && make package

all: packaging documentation

.PHONY: all clean test configure validate
