#define BOOST_TEST_MODULE CoverageExample
#include <boost/test/included/unit_test.hpp>

// Тестируемые функции
int add(int a, int b) {
    return a + b;
}

int multiply(int a, int b) {
    return a * b;
}

bool is_even(int n) {
    return n % 2 == 0;
}

// Тесты
BOOST_AUTO_TEST_CASE(test_arithmetic) {
    BOOST_TEST(add(2, 3) == 5);
    BOOST_TEST(add(-1, 1) == 0);
    
    BOOST_TEST(multiply(3, 4) == 12);
    BOOST_TEST(multiply(0, 5) == 0);
}

BOOST_AUTO_TEST_CASE(test_logic) {
    BOOST_TEST(is_even(4) == true);
    BOOST_TEST(is_even(7) == false);
    BOOST_TEST(is_even(0) == true);
}

// Запустите этот тест отдельно: make run_failing
BOOST_AUTO_TEST_CASE(test_expected_failure) {
    // Этот тест падает специально
    // BOOST_TEST(2 * 2 == 5);
}
