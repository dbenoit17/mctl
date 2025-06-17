# A generic Makefile that naively compiles all C files
# to intermediate objects and links them together.
#
# By default, intermediate objects are created in
# a _build directory.
#
# This Makefile also supports a `test` target, which will
# compile objects in a _test directory and use
# mctl/generate_tests.sh to create a test driver from
# test definitions in the C sources.
#
# Tests should be defined in blocks guarded as follows:
#
# | #ifdef _MCTL_MODE_TEST
# | <test code ...>
# | #endif // _MCTL_MODE_TEST
#
# To use this library, you will also have to
# guard main() with the same macro:
#
# | #ifndef _MCTL_MODE_TEST
# | int main(void) {
# | #else
# | int _main(void) {
# | #endif // _MCTL_MODE_TEST

# Path to the test library
MCTL = ..

# Name of the main application
MAIN = main

all: $(MAIN)

# The following section builds intermediate object files
# in a _build directory.
OBJDIR ?= _build
SRCDIR ?= .
SRCS += $(shell ls *.c)
OBJS += $(addprefix $(OBJDIR)/,$(SRCS:.c=.o))
$(OBJDIR):
	@mkdir -p $(OBJDIR)
$(OBJS): $(OBJDIR)/%.o: %.c $(OBJDIR)
	$(CC) $(CFLAGS) $(IFLAGS) -c -o $@ $<

# Build the main application.
# Intermediate objects are linked from _build.
$(MAIN): $(OBJS)
	$(CC) $(CFLAGS) $(IFLAGS) $(LDFLAGS) $(OBJS) -o $(MAIN)

# Build the tests using mctl
TEST_GENERATOR = $(MCTL)/generate_tests.sh
TEST_DRIVER = _test/_test_main_.c
TEST_MAIN = test.main

# Run the test generator and output the driver file.
$(TEST_DRIVER): $(TEST_GENERATOR) *.c
	bash $(TEST_GENERATOR) *.c > $(TEST_DRIVER)

# This target builds the test driver.  Do not invoke the target directly.
# Use `make test`, which will call this after setting up the test parameters.
$(TEST_MAIN): $(OBJS) $(TEST_DRIVER)
	$(CC) $(CFLAGS) $(IFLAGS) $(LDFLAGS) $(OBJS) $(TEST_DRIVER) -o $(TEST_MAIN)

# This is the Makefile frontend target for building the tests.
# We set the following parameters:
#    CFLAGS=-D_MCTL_MODE_TEST 	 : Includes the test functions in the build.
#    OBJDIR=_test		 : Override object directory to _test instead of _build.
#				   This eliminates object dependency conflicts.
#    IFLAGS="$(IFLAGS) -I$(MCTL)": Include the location of the testing.h header.
.PHONY: test
test:
	$(MAKE) $(TEST_MAIN) CFLAGS=-D_MCTL_MODE_TEST OBJDIR=_test IFLAGS="$(IFLAGS) -I$(MCTL)"
.PHONY: run_test
run_test: test
	./$(TEST_MAIN)

clean:
	-rm -rf _build _test $(TEST_MAIN) $(MAIN)

# Format
.PHONY: clang_format
clang_format:
	clang-format --style=LLVM -i *.c *.h ../*.h
