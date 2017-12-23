#define MAIN

#include <gtest/gtest.h>
#include "common.cc"

struct EvenIntFunctions {
    using State = std::tuple<size_t, std::vector<int>>;

    bool is_begin(const State &state) const {
        return get<0>(state) == 0;
    }
    bool is_end(const State &state) const {
        return get<0>(state) == get<1>(state).size();
    }
    bool is_valid(const State &state) const {
        return (get<1>(state)[get<0>(state)] % 2) == 0;
    }
    const int &get_value(const State &state) const {
        return get<1>(state)[get<0>(state)];
    }

    int &get_value(State &state) {
        return get<1>(state)[get<0>(state)];
    }

    void next(State &state) const {
        get<0>(state) += 1;
    }
    void previous(State &state) const {
        get<0>(state) -= 1;
    }
};

using IteratorForTest = BaseIterator<EvenIntFunctions>;

TEST(BaseIteratorTest, ConstructorTest) {
    std::vector<int> data = {1, 0, 3, 4};

    IteratorForTest it1(std::make_tuple(1, data));
    EXPECT_EQ(1, get<0>(it1.get_state()));
    EXPECT_EQ(0, *it1);

    IteratorForTest it2(std::make_tuple(0, data));
    EXPECT_EQ(1, get<0>(it2.get_state()));
    EXPECT_EQ(0, *it2);
}

TEST(BaseIteratorTest, IncrementTest) {
    std::vector<int> data = {1, 0, 3, 4, 5};

    IteratorForTest it1(std::make_tuple(1, data));
    ++it1;
    EXPECT_EQ(3, get<0>(it1.get_state()));
    EXPECT_EQ(4, *it1);

    IteratorForTest it2(std::make_tuple(4, data));
    ++it2;
    EXPECT_EQ(5, get<0>(it2.get_state()));
    EXPECT_TRUE(it2.is_end());
}

TEST(BaseIteratorTest, DecrementTest) {
    std::vector<int> data = {1, 0, 3, 4, 5};

    IteratorForTest it1(std::make_tuple(3, data));
    --it1;
    EXPECT_EQ(1, get<0>(it1.get_state()));
    EXPECT_EQ(0, *it1);

    IteratorForTest it2(std::make_tuple(1, data));
    --it2;
    EXPECT_EQ(0, get<0>(it2.get_state()));
    EXPECT_TRUE(it2.is_begin());
}



TEST(BaseIteratorTest, DereferenceTest) {
    std::vector<int> data = {1, 0, 3, 4, 5};

    IteratorForTest it1(std::make_tuple(3, data));
    *it1 = 6;
    EXPECT_EQ(3, get<0>(it1.get_state()));
    EXPECT_EQ(6, *it1);
}
