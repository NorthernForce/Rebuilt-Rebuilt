#include <gtest/gtest.h>
#include <logging/Logger.h>
#include <memory>

namespace nfr
{

// Test class that has a Log specialization (not a member method)
struct TestStruct
{
    int value;
    std::string name;
};

// Specialization of Log for TestStruct
template <>
void Log(const LogContext& logContext, const TestStruct& t)
{
    logContext["value"] << t.value;
    logContext["name"] << t.name;
}

// Test class with member Log method
struct TestWithMemberLog
{
    int data;
    
    void Log(const LogContext& logContext) const
    {
        logContext << data;
    }
};

class LoggingTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
        // Test setup if needed
    }
};

TEST_F(LoggingTest, LoggingWithSpecialization)
{
    TestStruct test{42, "hello"};
    
    // This should work - direct logging of object with Log specialization
    EXPECT_NO_THROW({
        nfr::logger["test"] << test;
    });
}

TEST_F(LoggingTest, LoggingWithMemberMethod)
{
    TestWithMemberLog test{100};
    
    // This should work - object with member Log method
    EXPECT_NO_THROW({
        nfr::logger["test"] << test;
    });
}

TEST_F(LoggingTest, LoggingSharedPtrWithMemberMethod)
{
    auto test = std::make_shared<TestWithMemberLog>(200);
    
    // This should work - shared_ptr to object with member Log method
    EXPECT_NO_THROW({
        nfr::logger["test"] << test;
    });
}

TEST_F(LoggingTest, LoggingSharedPtrWithSpecialization)
{
    auto test = std::make_shared<TestStruct>(TestStruct{300, "world"});
    
    // This currently fails but should work after the fix
    // shared_ptr to object with Log specialization
    EXPECT_NO_THROW({
        nfr::logger["test"] << test;
    });
}

TEST_F(LoggingTest, LoggingUniquePtrWithSpecialization)
{
    auto test = std::make_unique<TestStruct>(TestStruct{400, "unique"});
    
    // This currently fails but should work after the fix
    // unique_ptr to object with Log specialization
    EXPECT_NO_THROW({
        nfr::logger["test"] << test;
    });
}

TEST_F(LoggingTest, LoggingRawPtrWithSpecialization)
{
    TestStruct test{500, "raw"};
    TestStruct* testPtr = &test;
    
    // This currently fails but should work after the fix
    // raw pointer to object with Log specialization
    EXPECT_NO_THROW({
        nfr::logger["test"] << testPtr;
    });
}

} // namespace nfr