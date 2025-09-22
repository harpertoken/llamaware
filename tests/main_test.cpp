#include <gtest/gtest.h>
#include "version.h"

// Basic test to verify the testing framework works
TEST(BasicTest, SanityCheck) {
    EXPECT_EQ(1 + 1, 2);
}

// Test for version functionality
TEST(VersionTest, VersionCommand) {
    const char* version = Version::get_version();
    EXPECT_STREQ(version, LLAMAWARE_VERSION_STRING);
}

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}