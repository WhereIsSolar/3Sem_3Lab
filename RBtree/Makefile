CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
COV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lboost_unit_test_framework -lpthread
BENCH_LDFLAGS = -lboost_unit_test_framework -lboost_chrono -lpthread

PROGRAM = rbtree_program
TEST_PROGRAM = rbtree_tests
BENCH_PROGRAM = rbtree_bench
REPORT_DIR = coverage_report
LINT_FILES = RBTree.h RBTree.cpp Interface.cpp test_rbtree.cpp bench_rbtree.cpp

.PHONY: all program test bench coverage lint lint-quick lint-std clean_cov clean

all: program

program: RBTree.cpp Interface.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(PROGRAM)

test: test_rbtree.cpp RBTree.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_PROGRAM) $(LDFLAGS)
	@echo "=== ЗАПУСК ТЕСТОВ ==="
	./$(TEST_PROGRAM) --catch_system_errors=no

bench: bench_rbtree.cpp RBTree.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BENCH_PROGRAM) $(BENCH_LDFLAGS)
	@echo "=== ЗАПУСК БЕНЧМАРКОВ ==="
	./$(BENCH_PROGRAM) --catch_system_errors=no

coverage: clean_cov
	$(CXX) $(CXXFLAGS) $(COV_FLAGS) test_rbtree.cpp RBTree.cpp -o $(TEST_PROGRAM)_cov $(LDFLAGS)
	@./$(TEST_PROGRAM)_cov --catch_system_errors=no 2>/dev/null || echo "Тесты завершены"
	@sleep 0.1
	@lcov --capture --directory . --output-file coverage.info --no-external --ignore-errors inconsistent 2>/dev/null || echo "lcov: сбор данных"
	@lcov --remove coverage.info '/usr/*' '*/boost/*' --output-file coverage.info 2>/dev/null || true
	@genhtml coverage.info --output-directory $(REPORT_DIR) --title "RBTree Coverage Report" 2>/dev/null || echo "genhtml: генерация"
	@echo "Отчёт: $(REPORT_DIR)/index.html"

lint:
	@echo "=== ПРОВЕРКА CLANG-TIDY ==="
	@if command -v clang-tidy >/dev/null 2>&1; then \
		for file in $(LINT_FILES); do \
			if [ -f "$$file" ]; then \
				echo "Анализ $$file..."; \
				clang-tidy $$file -- -std=c++17 $(CXXFLAGS); \
			fi; \
		done; \
	else \
		echo "clang-tidy не установлен."; \
	fi

lint-quick:
	@clang-tidy RBTree.cpp --checks='-*,bugprone*,clang-analyzer*' -- -std=c++17 2>/dev/null || true

lint-std:
	@clang-tidy $(LINT_FILES) --checks='clang-analyzer-*,bugprone*,modernize*,performance*' -- -std=c++17

clean_cov:
	rm -f $(TEST_PROGRAM)_cov *.gcda *.gcno coverage.info $(BENCH_PROGRAM)
	rm -rf $(REPORT_DIR)

clean:
	rm -f $(PROGRAM) $(TEST_PROGRAM) $(BENCH_PROGRAM) $(TEST_PROGRAM)_cov *.gcda *.gcno coverage.info
	rm -rf $(REPORT_DIR) rbtree.txt rbtree.bin
