# Makefile for standalone RTLIL library
# Extracted from Yosys

CXX := g++
CXXFLAGS := -std=c++17 -O2 -Wall -Wextra -I./include
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

# Source files
SRCS := src/rtlil.cc src/log.cc src/binding.cc src/init.cc src/fmt.cc src/io.cc
OBJS := $(SRCS:.cc=.o)

# Library target
LIB := librtlil.a

# Test program
TEST_SRC := test/simple_example.cc
TEST_BIN := test/simple_example

.PHONY: all clean test

all: $(LIB)

$(LIB): $(OBJS)
	@echo "Creating static library $@"
	$(AR) $(ARFLAGS) $@ $^

%.o: %.cc
	@echo "Compiling $<"
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(TEST_BIN): $(TEST_SRC) $(LIB)
	@echo "Building test program"
	$(CXX) $(CXXFLAGS) $< -L. -lrtlil -o $@

test: $(TEST_BIN)
	@echo "Running test..."
	./$(TEST_BIN)

clean:
	rm -f $(OBJS) $(LIB) $(TEST_BIN)
	@echo "Cleaned build artifacts"

# Dependencies (simplified - in production use proper dependency generation)
src/rtlil.o: src/rtlil.cc include/rtlil.h include/yosys.h include/log.h include/yosys_common.h
src/log.o: src/log.cc include/log.h include/yosys.h include/yosys_common.h
src/binding.o: src/binding.cc include/binding.h include/rtlil.h
