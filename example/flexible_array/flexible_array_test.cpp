#include <gtest/gtest.h>

#include "flexible_array.hpp"

template <typename Value>
std::vector<Value> ToVector(FlexibleArray::FlexibleArray<Value>& values)
{
    std::vector<Value> vector;
    vector.reserve(values.Size());
    values.To(std::back_inserter(vector));
    return vector;
}

TEST(flexible_array, insert)
{
    FlexibleArray::FlexibleArray<int> values{};

    EXPECT_EQ(values.Size(), 0);

    values.PushBack(1);
    EXPECT_EQ(values.Size(), 1);
    EXPECT_EQ(values[0], 1);

    values.Insert(2, 0);
    EXPECT_EQ(values.Size(), 2);
    EXPECT_EQ(values[0], 2);
    EXPECT_EQ(values[1], 1);

    values.Insert(3, 1);

    EXPECT_EQ(values.Size(), 3);
    EXPECT_EQ(values[0], 2);
    EXPECT_EQ(values[1], 3);
    EXPECT_EQ(values[2], 1);
}

TEST(flexible_array, expose)
{
    FlexibleArray::FlexibleArray<int> values{};

    for (int i = 0; i < 10; i++) {
        values.PushBack(i);
    }

    EXPECT_EQ(values.Size(), 10);

    values.Remove(2, 8);

    EXPECT_EQ(values.Size(), 4);
    EXPECT_EQ(values[0], 0);
    EXPECT_EQ(values[1], 1);
    EXPECT_EQ(values[2], 8);
    EXPECT_EQ(values[3], 9);
}

TEST(flexible_array, to_std_vector)
{
    FlexibleArray::FlexibleArray<int> values{};
    values.PushBack(1);
    values.PushBack(2);
    values.PushBack(3);

    EXPECT_EQ(ToVector(values), std::vector<int>({1, 2, 3}));
}