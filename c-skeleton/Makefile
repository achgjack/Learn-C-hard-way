# compiler option, $(OPTFLAGS) is the user-specific options
# make OPTFLAGS=-pthread
CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)

# linking options, $(OPTLIBS) is the user-specific linking options
LIBS=-ldl $(OPTLIBS)

# ?=
# an optional variable that will only have this value if the person
# running the Makefile didn't already give a PREFIX setting
# make PREFIX=/tmp
PREFIX?=/root/opt

# dynamically creates the SOURCES variable by doing a wildcard search
# for all *.c files in src/ directory. You have to give both src/**/*.c
# and src/*.c so that make will include the files in src and the files
# below it
SOURCES=$(wildcard src/**/*.c src/*.c)

# object files, change all %.c extensions to %.o
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

# find all of the test source
TEST_SRC=$(wildcard tests/*_tests.c)

# get all the TEST targets
# stripping away the .c extension
TESTS=$(patsubst %.c,%,$(TEST_SRC))

# the ultimate target is build/libYOUR_LIBRARY.a
TARGET=build/libYOUR_LIBRARY.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))


# the target build
all: $(TARGET) $(SO_TARGET) tests

dev: CFLAGS=-g -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

$(TARGET): CFLAGS += -fPIC
$(TARGET): build $(OBJECTS)
	ar rcs $@ $(OBJECTS)
	ranlib $@

$(SO_TARGET) : $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
	@mkdir -p build
	@mkdir -p bin

# the unit tests
.PHONY: tests
tests: CFLAGS += $(TARGET)
tests: $(TESTS)
	sh ./tests/runtests.sh

# the cleaner
clean:
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc" -exec rm{}\;
	rm -rf `find . -name "*.dSYM" -print`

# the install
install: all
	install -d $(DESTDIR)/$(PREFIX)/lib/
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

# the checker
check:
	@echo Files with potentially dangerour functions.
	@egrep '[^_.>a-zA-Z0-9](str(n?cpy|n?cat|xfrm|n?dup|str|pbrk|tok|>)\
			|stpn?cpy|a?sn?printf|byte_)'$(SOURCES) || true

