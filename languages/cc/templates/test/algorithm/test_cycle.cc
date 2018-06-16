#define MAIN

#include <gtest/gtest.h>
#include "common.cc"
#include "data-structure/graph.cc"
#include "algorithm/traverse.cc"
#include "algorithm/cycle.cc"
#include "../graph_utils.h"

GRAPH_TEST(CycleTest);

using namespace graph;

TYPED_TEST(g_CycleTest, SimpleTest) {
    /*
     * 0 -> 1 -> 2 -> 3 -> 4
     *        <------   -> 5
     */
    auto g = this->mkGraph({{0, 1}, {1, 2}, {2, 3}, {3, 4}, {3, 1}, {3, 5}});
    auto cycle_opt = cycle(g, 0);
    EXPECT_TRUE(cycle_opt);
    auto cycle = cycle_opt.value();
    EXPECT_EQ(3, cycle.size());
    EXPECT_EQ(std::make_tuple(1, 2), cycle[0]);
    EXPECT_EQ(std::make_tuple(2, 3), cycle[1]);
    EXPECT_EQ(std::make_tuple(3, 1), cycle[2]);
}
TYPED_TEST(g_CycleTest, NoCycleTest) {
    /*
     * 0 -> 1 -> 2 -> 3 -> 4
     *                  -> 5
     */
    auto g = this->mkGraph({{0, 1}, {1, 2}, {2, 3}, {3, 4}, {3, 5}});
    auto cycle_opt = cycle(g, 0);
    EXPECT_FALSE(cycle_opt);
}
