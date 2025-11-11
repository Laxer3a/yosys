# Makefile for standalone RTLIL library - COMPLETE VERSION with BigInt and SHA1
# Extracted from Yosys

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -Wno-unused-parameter -I./include -I./libs
# Define _YOSYS_ to satisfy yosys_common.h requirement
CXXFLAGS += -D_YOSYS_
# Define namespace macros
CXXFLAGS += -DYOSYS_NAMESPACE_BEGIN='namespace Yosys {'
CXXFLAGS += -DYOSYS_NAMESPACE_END='}'
CXXFLAGS += -DYOSYS_NAMESPACE_PREFIX='Yosys::'
# Disable features not needed in standalone lib
CXXFLAGS += -DYOSYS_NO_IDS_REFCNT

AR := ar
ARFLAGS := rcs

# Kernel source files
KERNEL_SRCS := src/rtlil.cc src/log.cc src/binding.cc src/init.cc src/fmt.cc src/io.cc src/rtlil_bufnorm.cc
KERNEL_OBJS := $(KERNEL_SRCS:.cc=.o)

# BigInt library
BIGINT_SRCS := libs/bigint/BigUnsigned.cc libs/bigint/BigInteger.cc \
               libs/bigint/BigIntegerAlgorithms.cc libs/bigint/BigUnsignedInABase.cc \
               libs/bigint/BigIntegerUtils.cc
BIGINT_OBJS := $(BIGINT_SRCS:.cc=.o)

# SHA1 library
SHA1_SRCS := libs/sha1/sha1.cpp
SHA1_OBJS := $(SHA1_SRCS:.cpp=.o)

# All objects
OBJS := $(KERNEL_OBJS) $(BIGINT_OBJS) $(SHA1_OBJS)

# Library target
LIB := librtlil.a

# Test programs
TEST_SIMPLE := test/simple_example
TEST_COMPLEX := test/complex_example

.PHONY: all clean test

all: $(LIB)

$(LIB): $(OBJS)
	@echo "Creating static library $@"
	$(AR) $(ARFLAGS) $@ $^

src/%.o: src/%.cc
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

libs/bigint/%.o: libs/bigint/%.cc
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

libs/sha1/%.o: libs/sha1/%.cpp
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_SIMPLE): test/simple_example.cc $(LIB)
	@echo "Building simple test program"
	$(CXX) $(CXXFLAGS) $< -L. -lrtlil -o $@

$(TEST_COMPLEX): test/complex_example.cc $(LIB)
	@echo "Building complex test program"
	$(CXX) $(CXXFLAGS) $< -L. -lrtlil -o $@

test: $(TEST_SIMPLE)
	@echo "Running simple test..."
	./$(TEST_SIMPLE)

test-complex: $(TEST_COMPLEX)
	@echo "Running complex test..."
	./$(TEST_COMPLEX)

clean:
	rm -f $(KERNEL_OBJS) $(BIGINT_OBJS) $(SHA1_OBJS) $(LIB) $(TEST_SIMPLE) $(TEST_COMPLEX)
	@echo "Cleaned build artifacts"

# Dependencies
src/rtlil.o: src/rtlil.cc include/rtlil.h include/yosys.h include/log.h include/yosys_common.h
src/log.o: src/log.cc include/log.h include/yosys.h include/yosys_common.h
src/binding.o: src/binding.cc include/binding.h include/rtlil.h
src/fmt.o: src/fmt.cc include/fmt.h include/yosys.h
src/io.o: src/io.cc include/io.h include/yosys.h
