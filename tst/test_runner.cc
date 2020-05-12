#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

TEST(Test_Runner, TestVRA) {
  // make a graph of 3 nodes, 1 district, 
  unordered_set<uint32_t> neighbors0;
  unordered_set<uint32_t> neighbors1;
  neighbors0.insert(1);
  neighbors1.insert(0);

  Graph g(2, 2, 100);
  Node node0(0, 0, &neighbors0);
  Node node1(1, 1, &neighbors1);
  
  // node0 total pop = 50
  // 50/50 split between majority and minority pops 
  node0.SetTotalPop(50);
  node0.SetAAPop(25); // african american
  node0.SetCAPop(25); // caucasian

  // node1 total pop = 50
  // 40/10 split
  node1.SetTotalPop(50);
  node1.SetAAPop(10); // african american
  node1.SetCAPop(40); // caucasian

  // Runner runner;
  // runner.LoadGraph(&g);

  // ASSERT_EQ(g.GetNumNodes(), 1);
}

}
