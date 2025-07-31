#include <gtest/gtest.h>
#include <logging/Logger.h>
#include <logging/LogTypes.h>

#include <frc/geometry/Pose2d.h>
#include <frc/geometry/Pose3d.h>
#include <frc/geometry/Transform2d.h>
#include <frc/geometry/Transform3d.h>
#include <frc/geometry/Translation2d.h>
#include <frc/geometry/Translation3d.h>
#include <frc/geometry/Rotation2d.h>
#include <frc/geometry/Rotation3d.h>
#include <frc/geometry/Twist2d.h>
#include <frc/geometry/Twist3d.h>
#include <frc/kinematics/ChassisSpeeds.h>
#include <frc/kinematics/SwerveModulePosition.h>
#include <frc/kinematics/SwerveModuleState.h>
#include <units/acceleration.h>
#include <units/angle.h>
#include <units/angular_velocity.h>
#include <units/current.h>
#include <units/frequency.h>
#include <units/length.h>
#include <units/temperature.h>
#include <units/time.h>
#include <units/velocity.h>
#include <units/voltage.h>

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
    EXPECT_NO_THROW({ nfr::logger["test"] << test; });
}

TEST_F(LoggingTest, LoggingWithMemberMethod)
{
    TestWithMemberLog test{100};

    // This should work - object with member Log method
    EXPECT_NO_THROW({ nfr::logger["test"] << test; });
}

TEST_F(LoggingTest, LoggingSharedPtrWithMemberMethod)
{
    auto test = std::make_shared<TestWithMemberLog>(200);

    // This should work - shared_ptr to object with member Log method
    EXPECT_NO_THROW({ nfr::logger["test"] << test; });
}

TEST_F(LoggingTest, LoggingSharedPtrWithSpecialization)
{
    auto test = std::make_shared<TestStruct>(TestStruct{300, "world"});

    // This currently fails but should work after the fix
    // shared_ptr to object with Log specialization
    EXPECT_NO_THROW({ nfr::logger["test"] << test; });
}

TEST_F(LoggingTest, LoggingUniquePtrWithSpecialization)
{
    auto test = std::make_unique<TestStruct>(TestStruct{400, "unique"});

    // This currently fails but should work after the fix
    // unique_ptr to object with Log specialization
    EXPECT_NO_THROW({ nfr::logger["test"] << test; });
}

TEST_F(LoggingTest, LoggingRawPtrWithSpecialization)
{
    TestStruct test{500, "raw"};
    TestStruct* testPtr = &test;

    // This currently fails but should work after the fix
    // raw pointer to object with Log specialization
    EXPECT_NO_THROW({ nfr::logger["test"] << testPtr; });
}

// =============================================================================
// WPILib Geometry Types Tests
// =============================================================================

TEST_F(LoggingTest, LoggingPose2dDirect)
{
    frc::Pose2d pose{1.0_m, 2.0_m, frc::Rotation2d{45.0_deg}};
    EXPECT_NO_THROW({ nfr::logger["pose2d"] << pose; });
}

TEST_F(LoggingTest, LoggingPose2dSharedPtr)
{
    auto pose = std::make_shared<frc::Pose2d>(1.0_m, 2.0_m, frc::Rotation2d{45.0_deg});
    EXPECT_NO_THROW({ nfr::logger["pose2d_shared"] << pose; });
}

TEST_F(LoggingTest, LoggingPose2dUniquePtr)
{
    auto pose = std::make_unique<frc::Pose2d>(1.0_m, 2.0_m, frc::Rotation2d{45.0_deg});
    EXPECT_NO_THROW({ nfr::logger["pose2d_unique"] << pose; });
}

TEST_F(LoggingTest, LoggingPose2dRawPtr)
{
    frc::Pose2d pose{1.0_m, 2.0_m, frc::Rotation2d{45.0_deg}};
    frc::Pose2d* posePtr = &pose;
    EXPECT_NO_THROW({ nfr::logger["pose2d_raw"] << posePtr; });
}

TEST_F(LoggingTest, LoggingPose3dDirect)
{
    frc::Pose3d pose{1.0_m, 2.0_m, 3.0_m, frc::Rotation3d{0.1_rad, 0.2_rad, 0.3_rad}};
    EXPECT_NO_THROW({ nfr::logger["pose3d"] << pose; });
}

TEST_F(LoggingTest, LoggingPose3dSharedPtr)
{
    auto pose = std::make_shared<frc::Pose3d>(1.0_m, 2.0_m, 3.0_m, frc::Rotation3d{0.1_rad, 0.2_rad, 0.3_rad});
    EXPECT_NO_THROW({ nfr::logger["pose3d_shared"] << pose; });
}

TEST_F(LoggingTest, LoggingPose3dUniquePtr)
{
    auto pose = std::make_unique<frc::Pose3d>(1.0_m, 2.0_m, 3.0_m, frc::Rotation3d{0.1_rad, 0.2_rad, 0.3_rad});
    EXPECT_NO_THROW({ nfr::logger["pose3d_unique"] << pose; });
}

TEST_F(LoggingTest, LoggingPose3dRawPtr)
{
    frc::Pose3d pose{1.0_m, 2.0_m, 3.0_m, frc::Rotation3d{0.1_rad, 0.2_rad, 0.3_rad}};
    frc::Pose3d* posePtr = &pose;
    EXPECT_NO_THROW({ nfr::logger["pose3d_raw"] << posePtr; });
}

TEST_F(LoggingTest, LoggingTranslation2dDirect)
{
    frc::Translation2d translation{1.5_m, 2.5_m};
    EXPECT_NO_THROW({ nfr::logger["translation2d"] << translation; });
}

TEST_F(LoggingTest, LoggingTranslation2dSharedPtr)
{
    auto translation = std::make_shared<frc::Translation2d>(1.5_m, 2.5_m);
    EXPECT_NO_THROW({ nfr::logger["translation2d_shared"] << translation; });
}

TEST_F(LoggingTest, LoggingTranslation3dDirect)
{
    frc::Translation3d translation{1.5_m, 2.5_m, 3.5_m};
    EXPECT_NO_THROW({ nfr::logger["translation3d"] << translation; });
}

TEST_F(LoggingTest, LoggingTranslation3dSharedPtr)
{
    auto translation = std::make_shared<frc::Translation3d>(1.5_m, 2.5_m, 3.5_m);
    EXPECT_NO_THROW({ nfr::logger["translation3d_shared"] << translation; });
}

TEST_F(LoggingTest, LoggingRotation2dDirect)
{
    frc::Rotation2d rotation{90.0_deg};
    EXPECT_NO_THROW({ nfr::logger["rotation2d"] << rotation; });
}

TEST_F(LoggingTest, LoggingRotation2dSharedPtr)
{
    auto rotation = std::make_shared<frc::Rotation2d>(90.0_deg);
    EXPECT_NO_THROW({ nfr::logger["rotation2d_shared"] << rotation; });
}

TEST_F(LoggingTest, LoggingRotation3dDirect)
{
    frc::Rotation3d rotation{0.1_rad, 0.2_rad, 0.3_rad};
    EXPECT_NO_THROW({ nfr::logger["rotation3d"] << rotation; });
}

TEST_F(LoggingTest, LoggingRotation3dSharedPtr)
{
    auto rotation = std::make_shared<frc::Rotation3d>(0.1_rad, 0.2_rad, 0.3_rad);
    EXPECT_NO_THROW({ nfr::logger["rotation3d_shared"] << rotation; });
}

TEST_F(LoggingTest, LoggingTransform2dDirect)
{
    frc::Transform2d transform{frc::Translation2d{1.0_m, 2.0_m}, frc::Rotation2d{45.0_deg}};
    EXPECT_NO_THROW({ nfr::logger["transform2d"] << transform; });
}

TEST_F(LoggingTest, LoggingTransform2dSharedPtr)
{
    auto transform = std::make_shared<frc::Transform2d>(frc::Translation2d{1.0_m, 2.0_m}, frc::Rotation2d{45.0_deg});
    EXPECT_NO_THROW({ nfr::logger["transform2d_shared"] << transform; });
}

TEST_F(LoggingTest, LoggingTransform3dDirect)
{
    frc::Transform3d transform{frc::Translation3d{1.0_m, 2.0_m, 3.0_m}, frc::Rotation3d{0.1_rad, 0.2_rad, 0.3_rad}};
    EXPECT_NO_THROW({ nfr::logger["transform3d"] << transform; });
}

TEST_F(LoggingTest, LoggingTransform3dSharedPtr)
{
    auto transform = std::make_shared<frc::Transform3d>(frc::Translation3d{1.0_m, 2.0_m, 3.0_m}, frc::Rotation3d{0.1_rad, 0.2_rad, 0.3_rad});
    EXPECT_NO_THROW({ nfr::logger["transform3d_shared"] << transform; });
}

TEST_F(LoggingTest, LoggingTwist2dDirect)
{
    frc::Twist2d twist{1.0_mps, 2.0_mps, 0.5_rad_per_s};
    EXPECT_NO_THROW({ nfr::logger["twist2d"] << twist; });
}

TEST_F(LoggingTest, LoggingTwist2dSharedPtr)
{
    auto twist = std::make_shared<frc::Twist2d>(1.0_mps, 2.0_mps, 0.5_rad_per_s);
    EXPECT_NO_THROW({ nfr::logger["twist2d_shared"] << twist; });
}

TEST_F(LoggingTest, LoggingTwist3dDirect)
{
    frc::Twist3d twist{1.0_mps, 2.0_mps, 3.0_mps, 0.1_rad_per_s, 0.2_rad_per_s, 0.3_rad_per_s};
    EXPECT_NO_THROW({ nfr::logger["twist3d"] << twist; });
}

TEST_F(LoggingTest, LoggingTwist3dSharedPtr)
{
    auto twist = std::make_shared<frc::Twist3d>(1.0_mps, 2.0_mps, 3.0_mps, 0.1_rad_per_s, 0.2_rad_per_s, 0.3_rad_per_s);
    EXPECT_NO_THROW({ nfr::logger["twist3d_shared"] << twist; });
}

// =============================================================================
// WPILib Kinematics Types Tests
// =============================================================================

TEST_F(LoggingTest, LoggingChassisSpeedsDirect)
{
    frc::ChassisSpeeds speeds{1.0_mps, 2.0_mps, 0.5_rad_per_s};
    EXPECT_NO_THROW({ nfr::logger["chassis_speeds"] << speeds; });
}

TEST_F(LoggingTest, LoggingChassisSpeedsSharedPtr)
{
    auto speeds = std::make_shared<frc::ChassisSpeeds>(1.0_mps, 2.0_mps, 0.5_rad_per_s);
    EXPECT_NO_THROW({ nfr::logger["chassis_speeds_shared"] << speeds; });
}

TEST_F(LoggingTest, LoggingChassisSpeedsUniquePtr)
{
    auto speeds = std::make_unique<frc::ChassisSpeeds>(1.0_mps, 2.0_mps, 0.5_rad_per_s);
    EXPECT_NO_THROW({ nfr::logger["chassis_speeds_unique"] << speeds; });
}

TEST_F(LoggingTest, LoggingSwerveModuleStateDirect)
{
    frc::SwerveModuleState state{2.0_mps, frc::Rotation2d{45.0_deg}};
    EXPECT_NO_THROW({ nfr::logger["swerve_state"] << state; });
}

TEST_F(LoggingTest, LoggingSwerveModuleStateSharedPtr)
{
    auto state = std::make_shared<frc::SwerveModuleState>(2.0_mps, frc::Rotation2d{45.0_deg});
    EXPECT_NO_THROW({ nfr::logger["swerve_state_shared"] << state; });
}

TEST_F(LoggingTest, LoggingSwerveModulePositionDirect)
{
    frc::SwerveModulePosition position{1.5_m, frc::Rotation2d{30.0_deg}};
    EXPECT_NO_THROW({ nfr::logger["swerve_position"] << position; });
}

TEST_F(LoggingTest, LoggingSwerveModulePositionSharedPtr)
{
    auto position = std::make_shared<frc::SwerveModulePosition>(1.5_m, frc::Rotation2d{30.0_deg});
    EXPECT_NO_THROW({ nfr::logger["swerve_position_shared"] << position; });
}

// =============================================================================
// Units Types Tests
// =============================================================================

TEST_F(LoggingTest, LoggingUnitsLengthDirect)
{
    units::meter_t length = 5.5_m;
    EXPECT_NO_THROW({ nfr::logger["length"] << length; });
}

TEST_F(LoggingTest, LoggingUnitsLengthSharedPtr)
{
    auto length = std::make_shared<units::meter_t>(5.5_m);
    EXPECT_NO_THROW({ nfr::logger["length_shared"] << length; });
}

TEST_F(LoggingTest, LoggingUnitsAngleDirect)
{
    units::degree_t angle = 180.0_deg;
    EXPECT_NO_THROW({ nfr::logger["angle"] << angle; });
}

TEST_F(LoggingTest, LoggingUnitsAngleSharedPtr)
{
    auto angle = std::make_shared<units::degree_t>(180.0_deg);
    EXPECT_NO_THROW({ nfr::logger["angle_shared"] << angle; });
}

TEST_F(LoggingTest, LoggingUnitsVelocityDirect)
{
    units::meters_per_second_t velocity = 10.0_mps;
    EXPECT_NO_THROW({ nfr::logger["velocity"] << velocity; });
}

TEST_F(LoggingTest, LoggingUnitsVelocitySharedPtr)
{
    auto velocity = std::make_shared<units::meters_per_second_t>(10.0_mps);
    EXPECT_NO_THROW({ nfr::logger["velocity_shared"] << velocity; });
}

TEST_F(LoggingTest, LoggingUnitsAngularVelocityDirect)
{
    units::radians_per_second_t angularVelocity = 3.14_rad_per_s;
    EXPECT_NO_THROW({ nfr::logger["angular_velocity"] << angularVelocity; });
}

TEST_F(LoggingTest, LoggingUnitsAngularVelocitySharedPtr)
{
    auto angularVelocity = std::make_shared<units::radians_per_second_t>(3.14_rad_per_s);
    EXPECT_NO_THROW({ nfr::logger["angular_velocity_shared"] << angularVelocity; });
}

TEST_F(LoggingTest, LoggingUnitsVoltageDirect)
{
    units::volt_t voltage = 12.0_V;
    EXPECT_NO_THROW({ nfr::logger["voltage"] << voltage; });
}

TEST_F(LoggingTest, LoggingUnitsVoltageSharedPtr)
{
    auto voltage = std::make_shared<units::volt_t>(12.0_V);
    EXPECT_NO_THROW({ nfr::logger["voltage_shared"] << voltage; });
}

TEST_F(LoggingTest, LoggingUnitsCurrentDirect)
{
    units::ampere_t current = 20.0_A;
    EXPECT_NO_THROW({ nfr::logger["current"] << current; });
}

TEST_F(LoggingTest, LoggingUnitsCurrentSharedPtr)
{
    auto current = std::make_shared<units::ampere_t>(20.0_A);
    EXPECT_NO_THROW({ nfr::logger["current_shared"] << current; });
}

TEST_F(LoggingTest, LoggingUnitsTemperatureDirect)
{
    units::celsius_t temperature = 25.0_deg_C;
    EXPECT_NO_THROW({ nfr::logger["temperature"] << temperature; });
}

TEST_F(LoggingTest, LoggingUnitsTemperatureSharedPtr)
{
    auto temperature = std::make_shared<units::celsius_t>(25.0_deg_C);
    EXPECT_NO_THROW({ nfr::logger["temperature_shared"] << temperature; });
}

TEST_F(LoggingTest, LoggingUnitsAccelerationDirect)
{
    units::meters_per_second_squared_t acceleration = 9.81_mps_sq;
    EXPECT_NO_THROW({ nfr::logger["acceleration"] << acceleration; });
}

TEST_F(LoggingTest, LoggingUnitsAccelerationSharedPtr)
{
    auto acceleration = std::make_shared<units::meters_per_second_squared_t>(9.81_mps_sq);
    EXPECT_NO_THROW({ nfr::logger["acceleration_shared"] << acceleration; });
}

TEST_F(LoggingTest, LoggingUnitsTimeDirect)
{
    units::second_t time = 3.5_s;
    EXPECT_NO_THROW({ nfr::logger["time"] << time; });
}

TEST_F(LoggingTest, LoggingUnitsTimeSharedPtr)
{
    auto time = std::make_shared<units::second_t>(3.5_s);
    EXPECT_NO_THROW({ nfr::logger["time_shared"] << time; });
}

TEST_F(LoggingTest, LoggingUnitsFrequencyDirect)
{
    units::hertz_t frequency = 50.0_Hz;
    EXPECT_NO_THROW({ nfr::logger["frequency"] << frequency; });
}

TEST_F(LoggingTest, LoggingUnitsFrequencySharedPtr)
{
    auto frequency = std::make_shared<units::hertz_t>(50.0_Hz);
    EXPECT_NO_THROW({ nfr::logger["frequency_shared"] << frequency; });
}

// =============================================================================
// Mixed Pointer Types Tests (Comprehensive Coverage)
// =============================================================================

TEST_F(LoggingTest, LoggingMixedPointerTypes)
{
    // Test that all three pointer types work for same underlying type
    frc::Pose2d basePose{1.0_m, 2.0_m, frc::Rotation2d{45.0_deg}};
    auto sharedPose = std::make_shared<frc::Pose2d>(2.0_m, 3.0_m, frc::Rotation2d{90.0_deg});
    auto uniquePose = std::make_unique<frc::Pose2d>(3.0_m, 4.0_m, frc::Rotation2d{135.0_deg});
    frc::Pose2d* rawPose = &basePose;

    EXPECT_NO_THROW({
        nfr::logger["mixed"]["shared"] << sharedPose;
        nfr::logger["mixed"]["unique"] << uniquePose;
        nfr::logger["mixed"]["raw"] << rawPose;
    });
}

TEST_F(LoggingTest, LoggingNullPointers)
{
    // Test that null pointers are handled gracefully
    std::shared_ptr<frc::Pose2d> nullShared;
    std::unique_ptr<frc::Pose2d> nullUnique;
    frc::Pose2d* nullRaw = nullptr;

    EXPECT_NO_THROW({
        nfr::logger["null"]["shared"] << nullShared;
        nfr::logger["null"]["unique"] << nullUnique;
        nfr::logger["null"]["raw"] << nullRaw;
    });
}

}  // namespace nfr