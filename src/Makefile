.PHONY: all clean build install uninstall rebuild tests gcov_report lcov_report
SHELL = /bin/sh
OS = $(shell uname)
CC = gcc
CXX = g++
STD = -std=c++17
CFLAGS = -Wall -Werror -Wextra
RM     = rm -rf

ifeq ($(OS), Linux)
	GTEST_LIB = -lgtest -lpthread
else
	GTEST_LIB = -lgtest
endif

all: build

build:
	mkdir -p build && cd build && qmake6 .. && make

install: build
	cp -r build/3DViewer.app ~/Applications/3DViewer.app

uninstall:
	rm -rf ~/Applications/3DViewer.app
	rm -f ~/.3DViewer_preferences.json

VPATH += libs/matrix/
libs21_matrix.a:
	cd libs/matrix && $(CXX) $(STD) -c matrix.cc && ar rcs $@ *.o

LIBS += -Llibs/matrix/ -ls21_matrix
VPATH += model/ test_dir/

TEST_OUT = test.out

tests: $(TEST_OUT)
	./$(TEST_OUT)

INC_PATH = -I libs/matrix/ -I model/

$(TEST_OUT): tests.cc file_reader.cc transform_matrix.cc
	make libs21_matrix.a
	$(CXX) $(STD) $(CFLAGS) $(INC_PATH) $^ $(GTEST_LIB) $(LIBS) -o $@

GCOV_COMPILE_FLAGS  = -fprofile-arcs -ftest-coverage
GCOV_OUT		= gcov_report.out
GCOV_RESULT	= *.gcda *.gcno *.gcov
GCOV 			  = gcov
GCOV_FLAGS  = -kjr

$(GCOV_OUT): tests.cc file_reader.cc transform_matrix.cc
	$(RM) $(GCOV_RESULT)
	make libs21_matrix.a
	$(CXX) $(STD) $(GCOV_COMPILE_FLAGS) $(INC_PATH) model/file_reader.cc \
		$(GTEST_LIB) $(LIBS) -c
	$(CXX) $(STD) $(GCOV_COMPILE_FLAGS) $(INC_PATH) model/transform_matrix.cc \
		$(GTEST_LIB) $(LIBS) -c
	$(CXX) $(STD) $(INC_PATH) $< $(GTEST_LIB) $(LIBS) -c
	$(CXX) $(STD) $(GCOV_COMPILE_FLAGS) file_reader.o transform_matrix.o tests.o $(GTEST_LIB) $(LIBS) -o $@

gcov_report: $(GCOV_OUT) file_reader.cc transform_matrix.cc
	./$(GCOV_OUT)
	$(GCOV) $(GCOV_FLAGS) file_reader.cc transform_matrix.cc

LCOV          = lcov
LCOV_FLAGS    = --no-external -c -d .
COVERAGE_INFO = coverage.info

$(COVERAGE_INFO): $(GCOV_OUT)
	./$(GCOV_OUT)
	$(LCOV) $(LCOV_FLAGS) -o $(COVERAGE_INFO)

LCOV_REPORT_DIR = lcov_report_dir
GENHTML         = genhtml

$(LCOV_REPORT_DIR): $(COVERAGE_INFO)
	$(GENHTML) $^ -o $(LCOV_REPORT_DIR)

lcov_report: $(LCOV_REPORT_DIR)

clean:
	rm -rf $(LCOV_REPORT_DIR)
	rm -f $(GCOV_RESULT) *.info *.out *.o *.a
	rm -rf build
	rm -rf libs/matrix/*.{a,o}

dvi: info.pdf

ARCHIVE_DIR = 3DViewer_v2.0
ARCHIVE     = $(ARCHIVE_DIR).tar

dist: $(ARCHIVE)

$(ARCHIVE):
	mkdir -p $(ARCHIVE_DIR)
	cp -r model $(ARCHIVE_DIR)
	cp -r view $(ARCHIVE_DIR)
	cp -r controller $(ARCHIVE_DIR)
	cp -r test_dir $(ARCHIVE_DIR)
	cp -r libs $(ARCHIVE_DIR)
	cp -r info.tex $(ARCHIVE_DIR)
	cp -r info.pdf $(ARCHIVE_DIR)
	cp -r Makefile $(ARCHIVE_DIR)
	cp -r assets $(ARCHIVE_DIR)
	cp -r ../obj_examples $(ARCHIVE_DIR)
	tar cf $(ARCHIVE) $(ARCHIVE_DIR)
	rm -r $(ARCHIVE_DIR)

info.pdf: info.tex
	texi2pdf -c --quiet -o info.pdf info.tex

clang:
	clang-format -n model/* view/*.cc view/*.h test_dir/*.cc controller/*

SUPPRESSING = --suppress=unusedFunction --suppress=missingIncludeSystem \
			  --suppress=unmatchedSuppression --suppress=missingInclude --suppress=unusedStructMember
cppcheck:
	cppcheck --enable=all --language=c++ $(SUPPRESSING) *.cc *.h */*.cc */*.h
