#include <gtest/gtest.h>
#include "../vector.h"

TEST(VectorTest, PushBackAndSize) {
    mystl::vector<int> v;
    v.push_back(5);
    v.push_back(6);
    v.push_back(7);
    EXPECT_EQ(v.size(), 3);
    EXPECT_EQ(v[0], 5);
    EXPECT_EQ(v[1], 6);
    EXPECT_EQ(v[2], 7);
}

TEST(VectorTest, ShrinkToFit) {
    mystl::vector<int> v;
    v.assign(10,5);
    int before = v.capacity();
    EXPECT_EQ(v.capacity(), 10);
    v.reserve(100);
    EXPECT_EQ(v.capacity(), 100);
    v.shrink_to_fit();
    EXPECT_EQ(v.capacity(), 10);
    EXPECT_EQ(v.size(), 10);
}

TEST(VectorTest, Insert) {
    mystl::vector<int> v;
    v.assign(10,5);
    v.insert(v.begin() + 5, 9);
    mystl::vector<int> y;
    y.assign(3,3);
    v.insert(v.end(), y.begin(), y.end());
}

