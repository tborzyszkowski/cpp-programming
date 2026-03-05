#include <gtest/gtest.h>
#include <stdexcept>
#include "Calculator.h"

// ============================================================
// Testy jednostkowe dla klasy Calculator
// Framework: Google Test (gtest)
// ============================================================
// Kompilacja z Google Test (cmake):
//   CMakeLists.txt – patrz plik CMakeLists.txt w tym katalogu
// Lub ręcznie (zakładając zainstalowany gtest):
//   g++ -std=c++17 -o tests calculator_tests.cpp -lgtest -lgtest_main -pthread
// ============================================================

// Test fixture – reużywalny kontekst dla grupy testów
class CalculatorTest : public ::testing::Test {
protected:
    Calculator calc;    // obiekt dostępny we wszystkich testach tej grupy

    // SetUp wywołane przed każdym testem
    void SetUp() override {
        // można tu inicjalizować dane testowe
    }

    // TearDown wywołane po każdym teście
    void TearDown() override {}
};

// --- Testy dodawania ---
TEST_F(CalculatorTest, AddPositiveNumbers) {
    EXPECT_DOUBLE_EQ(calc.add(2.0, 3.0), 5.0);
}

TEST_F(CalculatorTest, AddNegativeNumbers) {
    EXPECT_DOUBLE_EQ(calc.add(-4.0, -6.0), -10.0);
}

TEST_F(CalculatorTest, AddZero) {
    EXPECT_DOUBLE_EQ(calc.add(5.0, 0.0), 5.0);
}

// --- Testy odejmowania ---
TEST_F(CalculatorTest, SubtractBasic) {
    EXPECT_DOUBLE_EQ(calc.subtract(10.0, 4.0), 6.0);
}

TEST_F(CalculatorTest, SubtractNegativeResult) {
    EXPECT_DOUBLE_EQ(calc.subtract(3.0, 7.0), -4.0);
}

// --- Testy mnożenia ---
TEST_F(CalculatorTest, MultiplyByZero) {
    EXPECT_DOUBLE_EQ(calc.multiply(99.9, 0.0), 0.0);
}

TEST_F(CalculatorTest, MultiplyNegatives) {
    EXPECT_DOUBLE_EQ(calc.multiply(-3.0, -4.0), 12.0);
}

// --- Testy dzielenia ---
TEST_F(CalculatorTest, DivideBasic) {
    EXPECT_DOUBLE_EQ(calc.divide(10.0, 2.0), 5.0);
}

TEST_F(CalculatorTest, DivideByZeroThrows) {
    // ASSERT_THROW sprawdza czy rzucony jest właściwy wyjątek
    ASSERT_THROW(calc.divide(10.0, 0.0), std::invalid_argument);
}

// --- Testy isEven ---
TEST_F(CalculatorTest, EvenNumber) {
    EXPECT_TRUE(calc.isEven(4));
}

TEST_F(CalculatorTest, OddNumber) {
    EXPECT_FALSE(calc.isEven(7));
}

TEST_F(CalculatorTest, ZeroIsEven) {
    EXPECT_TRUE(calc.isEven(0));
}

// --- Testy silni ---
TEST_F(CalculatorTest, FactorialZero) {
    EXPECT_EQ(calc.factorial(0), 1LL);
}

TEST_F(CalculatorTest, FactorialOne) {
    EXPECT_EQ(calc.factorial(1), 1LL);
}

TEST_F(CalculatorTest, FactorialFive) {
    EXPECT_EQ(calc.factorial(5), 120LL);
}

TEST_F(CalculatorTest, FactorialNegativeThrows) {
    ASSERT_THROW(calc.factorial(-1), std::invalid_argument);
}

// Prosta funkcja main (alternatywa do linkowania -lgtest_main)
// int main(int argc, char** argv) {
//     ::testing::InitGoogleTest(&argc, argv);
//     return RUN_ALL_TESTS();
// }
