CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
COV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lboost_unit_test_framework -lpthread

PROGRAM = list_program
TEST_PROGRAM = list_tests
REPORT_DIR = coverage_report
COV_INFO = coverage.info

.PHONY: all program test coverage lint clean

all: program

program: $(PROGRAM)

$(PROGRAM): ForwardList.h ForwardList.cpp Interface.cpp
	$(CXX) $(CXXFLAGS) ForwardList.cpp Interface.cpp -o $(PROGRAM)

test: $(TEST_PROGRAM)
	./$(TEST_PROGRAM) --catch_system_errors=no

$(TEST_PROGRAM): ForwardList.h ForwardList.cpp test.cpp
	$(CXX) $(CXXFLAGS) ForwardList.cpp test.cpp -o $(TEST_PROGRAM) $(LDFLAGS)

coverage: clean_cov
	$(CXX) $(CXXFLAGS) $(COV_FLAGS) ForwardList.cpp test.cpp -o $(TEST_PROGRAM)_cov $(LDFLAGS)
	./$(TEST_PROGRAM)_cov --catch_system_errors=no >/dev/null 2>&1 || true
	lcov --capture --directory . --output-file $(COV_INFO) --no-external --ignore-errors inconsistent >/dev/null 2>&1
	lcov --remove $(COV_INFO) '/usr/*' '*/boost/*' --output-file $(COV_INFO) >/dev/null 2>&1
	genhtml $(COV_INFO) --output-directory $(REPORT_DIR) --title "Coverage Report" >/dev/null 2>&1
	@echo "Отчёт: $(REPORT_DIR)/index.html"

lint:
	@echo "lint placeholder"

open:
	xdg-open $(REPORT_DIR)/index.html 2>/dev/null || firefox $(REPORT_DIR)/index.html 2>/dev/null || echo "open $(REPORT_DIR)/index.html"

run: program
	./$(PROGRAM)

clean:
	rm -f $(PROGRAM) $(TEST_PROGRAM) $(TEST_PROGRAM)_cov *.gcda *.gcno $(COV_INFO)
	rm -rf $(REPORT_DIR)

clean_cov:
	rm -f $(TEST_PROGRAM)_cov *.gcda *.gcno $(COV_INFO)
	rm -rf $(REPORT_DIR)
