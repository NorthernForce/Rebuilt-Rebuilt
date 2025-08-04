#include <tuning/Tunable.h>
#include <tuning/TuningManager.h>
#include <units/length.h>
#include <units/velocity.h>

#include "gtest/gtest.h"

class TuningTest : public ::testing::Test
{
  protected:
    void SetUp() override
    {
        // Clear any existing tunables
        nfr::TuningManager::GetInstance().Clear();
    }

    void TearDown() override
    {
        nfr::TuningManager::GetInstance().Clear();
    }
};

TEST_F(TuningTest, BasicDoubleTest)
{
    nfr::TunableDouble testValue("testDouble", 5.0);

    // Test implicit conversion
    double value = testValue;
    EXPECT_DOUBLE_EQ(5.0, value);

    // Test Get() method
    EXPECT_DOUBLE_EQ(5.0, testValue.Get());

    // Test setting values
    testValue.Set(10.0);
    EXPECT_DOUBLE_EQ(10.0, testValue.Get());

    // Test assignment operator
    testValue = 15.0;
    EXPECT_DOUBLE_EQ(15.0, testValue.Get());
}

TEST_F(TuningTest, BasicIntTest)
{
    nfr::TunableInt testValue("testInt", 42);

    // Test implicit conversion
    int value = testValue;
    EXPECT_EQ(42, value);

    // Test setting values
    testValue.Set(100);
    EXPECT_EQ(100, testValue.Get());

    // Test assignment operator
    testValue = 200;
    EXPECT_EQ(200, testValue.Get());
}

TEST_F(TuningTest, BasicBoolTest)
{
    nfr::TunableBool testValue("testBool", true);

    // Test implicit conversion
    bool value = testValue;
    EXPECT_TRUE(value);

    // Test setting values
    testValue.Set(false);
    EXPECT_FALSE(testValue.Get());

    // Test assignment operator
    testValue = true;
    EXPECT_TRUE(testValue.Get());
}

TEST_F(TuningTest, UnitsTest)
{
    nfr::Tunable<units::meters_per_second_t> speedTunable("testSpeed", 3.0_mps);

    // Test implicit conversion
    units::meters_per_second_t speed = speedTunable;
    EXPECT_DOUBLE_EQ(3.0, speed.value());

    // Test setting values
    speedTunable.Set(5.0_mps);
    EXPECT_DOUBLE_EQ(5.0, speedTunable.Get().value());

    // Test assignment operator
    speedTunable = 7.0_mps;
    EXPECT_DOUBLE_EQ(7.0, speedTunable.Get().value());
}

TEST_F(TuningTest, CopyConstructorTest)
{
    nfr::TunableDouble original("original", 10.0);
    nfr::TunableDouble copy(original);

    EXPECT_DOUBLE_EQ(10.0, copy.Get());
    EXPECT_EQ("original", copy.GetKey());
}

TEST_F(TuningTest, AssignmentOperatorTest)
{
    nfr::TunableDouble first("first", 10.0);
    nfr::TunableDouble second("second", 20.0);

    second = first;

    EXPECT_DOUBLE_EQ(10.0, second.Get());
    EXPECT_EQ("first", second.GetKey());
}

TEST_F(TuningTest, ArithmeticOperationsTest)
{
    nfr::TunableDouble a("a", 10.0);
    nfr::TunableDouble b("b", 5.0);

    // Test that tunables can be used in arithmetic operations via implicit
    // conversion
    double aVal = a;
    double bVal = b;

    double sum = aVal + bVal;
    EXPECT_DOUBLE_EQ(15.0, sum);

    double product = aVal * bVal;
    EXPECT_DOUBLE_EQ(50.0, product);

    double difference = aVal - bVal;
    EXPECT_DOUBLE_EQ(5.0, difference);

    double quotient = aVal / bVal;
    EXPECT_DOUBLE_EQ(2.0, quotient);

    // Test direct usage in arithmetic (implicit conversion)
    double directSum = a + b;
    EXPECT_DOUBLE_EQ(15.0, directSum);
}

TEST_F(TuningTest, ComparisonOperationsTest)
{
    nfr::TunableDouble a("a", 10.0);
    nfr::TunableDouble b("b", 5.0);

    // Test comparison operations via implicit conversion
    double aVal = a;
    double bVal = b;

    EXPECT_TRUE(aVal > bVal);
    EXPECT_FALSE(aVal < bVal);
    EXPECT_TRUE(aVal >= bVal);
    EXPECT_FALSE(aVal <= bVal);
    EXPECT_FALSE(aVal == bVal);
    EXPECT_TRUE(aVal != bVal);

    // Test direct comparisons (implicit conversion)
    EXPECT_TRUE(a > 5.0);
    EXPECT_FALSE(a < 5.0);
    EXPECT_TRUE(10.0 == a);
}

TEST_F(TuningTest, UnitsArithmeticTest)
{
    nfr::Tunable<units::meters_per_second_t> speed("speed", 10.0_mps);
    nfr::Tunable<units::second_t> time("time", 2.0_s);

    // Test units arithmetic - extract values first for arithmetic
    units::meters_per_second_t speedValue = speed.Get();
    units::second_t timeValue = time.Get();
    units::meter_t distance = speedValue * timeValue;
    EXPECT_DOUBLE_EQ(20.0, distance.value());
}

TEST_F(TuningTest, TuningManagerTest)
{
    nfr::TunableDouble testValue("managerTest", 1.0);

    // Register for updates
    bool updateCalled = false;
    nfr::TuningManager::GetInstance().RegisterTunable(
        [&testValue, &updateCalled]()
        {
            testValue.Update();
            updateCalled = true;
        });

    // Call update all
    nfr::TuningManager::GetInstance().UpdateAll();

    EXPECT_TRUE(updateCalled);
}

// Test that tunables work in function calls
double TestFunction(double value)
{
    return value * 2.0;
}

TEST_F(TuningTest, FunctionCallTest)
{
    nfr::TunableDouble testValue("functionTest", 5.0);

    // Should work seamlessly as function parameter
    double result = TestFunction(testValue);
    EXPECT_DOUBLE_EQ(10.0, result);
}

// Test with const tunables
TEST_F(TuningTest, ConstTunableTest)
{
    const nfr::TunableDouble constTunable("constTest", 3.14);

    // Should be able to read from const tunable
    double value = constTunable;
    EXPECT_DOUBLE_EQ(3.14, value);

    double getValue = constTunable.Get();
    EXPECT_DOUBLE_EQ(3.14, getValue);
}