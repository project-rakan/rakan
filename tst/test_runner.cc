#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

TEST(Test_Runner, TestScores) {
  // sets of neighbors
  // neighbors0 is the set of neighbors for node0
  // neighbors1 is the set of neighbors for node1
  unordered_set<uint32_t> neighbors0;
  unordered_set<uint32_t> neighbors1;
  neighbors0.insert(1);
  neighbors1.insert(0);

  // graph with 2 
  Graph g(2, 2, 100);
  Node node0(0, 0, &neighbors0);
  Node node1(1, 1, &neighbors1);
  
  node0.SetTotalPop(50);
  node0.SetAAPop(25);
  node0.SetCAPop(25);

  node1.SetTotalPop(50);
  node1.SetAAPop(10);
  node1.SetCAPop(40);

  Runner runner;
  runner.SetGraph(&g);

  ASSERT_EQ(runner.ScoreVRA(), 1);
}

}
