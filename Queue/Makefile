CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
COV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lboost_unit_test_framework -lpthread

PROGRAM = queue_program
TEST_PROGRAM = queue_tests
BENCH_PROGRAM = queue_bench
REPORT_DIR = coverage_report
LINT_FILES = Queue.h Queue.cpp Interface.cpp test_queue.cpp bench_queue.cpp

.PHONY: all program test bench coverage lint clean

all: program

program: Queue.cpp Interface.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(PROGRAM)

test: test_queue.cpp Queue.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_PROGRAM) $(LDFLAGS)
	./$(TEST_PROGRAM) --catch_system_errors=no

bench: bench_queue.cpp Queue.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(BENCH_PROGRAM) $(LDFLAGS)
	@echo "=== ЗАПУСК БЕНЧМАРКОВ QUEUE ==="
	./$(BENCH_PROGRAM) --catch_system_errors=no

coverage: clean_cov
	$(CXX) $(CXXFLAGS) $(COV_FLAGS) test_queue.cpp Queue.cpp -o $(TEST_PROGRAM)_cov $(LDFLAGS)
	@./$(TEST_PROGRAM)_cov --catch_system_errors=no 2>/dev/null || echo "Тесты завершены"
	@sleep 0.1
	@lcov --capture --directory . --output-file coverage.info --no-external --ignore-errors inconsistent 2>/dev/null || echo "lcov: сбор данных"
	@lcov --remove coverage.info '/usr/*' '*/boost/*' --output-file coverage.info 2>/dev/null || true
	@genhtml coverage.info --output-directory $(REPORT_DIR) --title "Queue Coverage Report" 2>/dev/null || echo "genhtml: генерация"
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
	@clang-tidy Queue.cpp --checks='-*,bugprone*,clang-analyzer*' -- -std=c++17 2>/dev/null || true

lint-std:
	@clang-tidy $(LINT_FILES) --checks='clang-analyzer-*,bugprone*,modernize*,performance*' -- -std=c++17

clean_cov:
	rm -f $(TEST_PROGRAM)_cov *.gcda *.gcno coverage.info $(BENCH_PROGRAM)
	rm -rf $(REPORT_DIR)

clean:
	rm -f $(PROGRAM) $(TEST_PROGRAM) $(BENCH_PROGRAM) $(TEST_PROGRAM)_cov *.gcda *.gcno coverage.info
	rm -rf $(REPORT_DIR) queue.txt queue.bin
