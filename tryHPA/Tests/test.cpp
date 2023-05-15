#include "pch.h"
#include "../../tryHPA/tryHPA/DrawFunctions.h"
#include "../../tryHPA/tryHPA/map.h"
#include "../../tryHPA/tryHPA/tiles.h"

TEST(TestCaseName, TestName) {
  EXPECT_EQ(1, 1);
  EXPECT_TRUE(true);
}

TEST(test_Node, test_create_1) {
	Node n(1, 1);
	ASSERT_NO_THROW();
}
TEST(test_Node, test_getID) {
	Node n(1, 1);
	ASSERT_EQ(1, n.getId());
}
TEST(test_Node, test_getClusterID) {
	Node n(1, 7);
	ASSERT_EQ(7, n.getClusterId());
}
TEST(test_Edge, test_create_1) {
	Node n1(1, 1);
	Node n2(10, 7);
	Edge e(n1, n2, 7.7, INTER);
	ASSERT_NO_THROW();
}