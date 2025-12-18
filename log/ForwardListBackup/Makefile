CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra
COV_FLAGS = -fprofile-arcs -ftest-coverage
LDFLAGS = -lboost_unit_test_framework -lpthread

PROGRAM = list_program
TEST_PROGRAM = list_tests
REPORT_DIR = coverage_report
LINT_FILES = ForwardList.h ForwardList.cpp Interface.cpp test.cpp

.PHONY: all program test coverage lint clean

all: program

program: ForwardList.cpp Interface.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(PROGRAM)

test: test.cpp ForwardList.cpp
	$(CXX) $(CXXFLAGS) $^ -o $(TEST_PROGRAM) $(LDFLAGS)
	./$(TEST_PROGRAM) --catch_system_errors=no

coverage: clean_cov
	$(CXX) $(CXXFLAGS) $(COV_FLAGS) test.cpp ForwardList.cpp -o $(TEST_PROGRAM)_cov $(LDFLAGS)
	./$(TEST_PROGRAM)_cov --catch_system_errors=no --result_code=no >/dev/null 2>&1 || true
	lcov --capture --directory . --output-file coverage.info --no-external --ignore-errors inconsistent >/dev/null 2>&1
	lcov --remove coverage.info '/usr/*' '*/boost/*' --output-file coverage.info >/dev/null 2>&1
	genhtml coverage.info --output-directory $(REPORT_DIR) --title "Coverage Report" >/dev/null 2>&1
	@echo "Отчёт: $(REPORT_DIR)/index.html"

# Линтер - базовая версия без конфига
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
		echo "clang-tidy не установлен. Установите: sudo pacman -S clang-tidy"; \
		exit 1; \
	fi

# Быстрая проверка только критических ошибок
lint-quick:
	@echo "=== БЫСТРАЯ ПРОВЕРКА ==="
	@clang-tidy ForwardList.cpp --checks='-*,bugprone*,clang-analyzer*' -- -std=c++17 2>/dev/null || true

# Стандартные проверки Google/LLVM
lint-std:
	@echo "=== СТАНДАРТНЫЕ ПРОВЕРКИ ==="
	@clang-tidy $(LINT_FILES) --checks='clang-analyzer-*,bugprone*,modernize*,performance*' -- -std=c++17

clean_cov:
	rm -f $(TEST_PROGRAM)_cov *.gcda *.gcno coverage.info
	rm -rf $(REPORT_DIR)

clean:
	rm -f $(PROGRAM) $(TEST_PROGRAM) $(TEST_PROGRAM)_cov *.gcda *.gcno coverage.info
	rm -rf $(REPORT_DIR) list.txt
