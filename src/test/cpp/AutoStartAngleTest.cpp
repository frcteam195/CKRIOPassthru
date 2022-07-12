#include "gtest/gtest.h"

#include "utils/AutoStartPositionAnglePair.hpp"

TEST(StartAngleTest, CheckAngleCorrect)
{
    AutoStartPositionAnglePair start_angle_1(-88.5);
    AutoStartPositionAnglePair start_angle_2(136.5);
    AutoStartPositionAnglePair start_angle_3(-156.0);

    EXPECT_DOUBLE_EQ(start_angle_1.get_red_start_angle(), -88.5);
    EXPECT_DOUBLE_EQ(start_angle_1.get_blue_start_angle(), 91.5);

    EXPECT_DOUBLE_EQ(start_angle_2.get_red_start_angle(), 136.5);
    EXPECT_DOUBLE_EQ(start_angle_2.get_blue_start_angle(), -43.5);

    EXPECT_DOUBLE_EQ(start_angle_3.get_red_start_angle(), -156.0);
    EXPECT_DOUBLE_EQ(start_angle_3.get_blue_start_angle(), 24.0);
}