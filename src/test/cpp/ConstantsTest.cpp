#include <frc/geometry/Transform3d.h>
#include <gtest/gtest.h>

#include "constants/Constants.h"

using namespace nfr;

class ConstantsTest : public ::testing::Test
{
protected:
    void SetUp() override
    {
    }
    void TearDown() override
    {
    }
};

TEST_F(ConstantsTest, CameraConstantsExist)
{
    // Test that camera constants are properly defined
    EXPECT_NE(CameraConstants::kFrontLeftCameraName, nullptr);
    EXPECT_NE(CameraConstants::kCenterCameraName, nullptr);
    EXPECT_NE(CameraConstants::kFrontRightCameraName, nullptr);
    EXPECT_NE(CameraConstants::kCenterBackCameraName, nullptr);
}

TEST_F(ConstantsTest, CameraTransformsValid)
{
    // Test that camera transforms are accessible
    frc::Transform3d frontLeftTransform =
        CameraConstants::kFrontLeftCameraTransform;
    frc::Transform3d centerTransform = CameraConstants::kCenterCameraTransform;
    frc::Transform3d frontRightTransform =
        CameraConstants::kFrontRightCameraTransform;
    frc::Transform3d centerBackTransform =
        CameraConstants::kCenterBackCameraTransform;

    // Basic sanity checks - transforms should have valid components
    EXPECT_TRUE(std::isfinite(frontLeftTransform.Translation().X().value()));
    EXPECT_TRUE(std::isfinite(frontLeftTransform.Translation().Y().value()));
    EXPECT_TRUE(std::isfinite(frontLeftTransform.Translation().Z().value()));

    EXPECT_TRUE(std::isfinite(centerTransform.Translation().X().value()));
    EXPECT_TRUE(std::isfinite(centerTransform.Translation().Y().value()));
    EXPECT_TRUE(std::isfinite(centerTransform.Translation().Z().value()));

    EXPECT_TRUE(std::isfinite(frontRightTransform.Translation().X().value()));
    EXPECT_TRUE(std::isfinite(frontRightTransform.Translation().Y().value()));
    EXPECT_TRUE(std::isfinite(frontRightTransform.Translation().Z().value()));

    EXPECT_TRUE(std::isfinite(centerBackTransform.Translation().X().value()));
    EXPECT_TRUE(std::isfinite(centerBackTransform.Translation().Y().value()));
    EXPECT_TRUE(std::isfinite(centerBackTransform.Translation().Z().value()));
}

TEST_F(ConstantsTest, VisionConstantsValid)
{
    // Test that vision constants are reasonable
    EXPECT_GT(VisionConstants::kMaxEstimateAge.value(), 0.0);
    EXPECT_LT(VisionConstants::kMaxEstimateAge.value(), 1.0);

    EXPECT_GT(VisionConstants::kEstimateTimeout.value(), 0.0);
    EXPECT_LT(VisionConstants::kEstimateTimeout.value(), 5.0);
}