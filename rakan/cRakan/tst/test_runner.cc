#include <inttypes.h>

#include "../src/Runner.h"
#include "../src/Graph.h"
#include "../src/Node.h"

#include "gtest/gtest.h"

namespace rakan {

Runner* GenerateIowa() {
  Runner *r = new Runner(99, 4);
  r->add_node(0, 0, 139, 2);
  r->add_node(1, 1, 398, 8);
  r->add_node(2, 2, 36, 6);
  r->add_node(3, 3, 371, 5);
  r->add_node(4, 4, 240, 3);
  r->add_node(5, 5, 736, 11);
  r->add_node(6, 6, 3712, 277);
  r->add_node(7, 7, 658, 25);
  r->add_node(8, 8, 303, 5);
  r->add_node(9, 9, 1486, 28);
  r->add_node(10, 10, 431, 17);
  r->add_node(11, 11, 1384, 37);
  r->add_node(12, 12, 573, 9);
  r->add_node(13, 13, 3, 0);
  r->add_node(14, 14, 284, 3);
  r->add_node(15, 15, 1, 0);
  r->add_node(16, 16, 1043, 82);
  r->add_node(17, 17, 456, 9);
  r->add_node(18, 18, 1182, 32);
  r->add_node(19, 19, 322, 5);
  r->add_node(20, 20, 261, 6);
  r->add_node(21, 21, 281, 2);
  r->add_node(22, 22, 2105, 139);
  r->add_node(23, 23, 390, 15);
  r->add_node(24, 24, 1671, 52);
  r->add_node(25, 25, 674, 11);
  r->add_node(26, 26, 369, 29);
  r->add_node(27, 27, 45, 1);
  r->add_node(28, 28, 1808, 184);
  r->add_node(29, 29, 583, 8);
  r->add_node(30, 30, 126, 4);
  r->add_node(31, 31, 487, 10);
  r->add_node(32, 32, 476, 6);
  r->add_node(33, 33, 908, 15);
  r->add_node(34, 34, 273, 14);
  r->add_node(35, 35, 255, 4);
  r->add_node(36, 36, 149, 3);
  r->add_node(37, 37, 556, 10);
  r->add_node(38, 38, 891, 26);
  r->add_node(39, 39, 524, 25);
  r->add_node(40, 40, 15, 0);
  r->add_node(41, 41, 525, 13);
  r->add_node(42, 42, 534, 8);
  r->add_node(43, 43, 4063, 590);
  r->add_node(44, 44, 545, 10);
  r->add_node(45, 45, 335, 13);
  r->add_node(46, 46, 634, 16);
  r->add_node(47, 47, 737, 9);
  r->add_node(48, 48, 713, 10);
  r->add_node(49, 49, 877, 19);
  r->add_node(50, 50, 563, 11);
  r->add_node(51, 51, 2650, 229);
  r->add_node(52, 52, 581, 11);
  r->add_node(53, 53, 546, 8);
  r->add_node(54, 54, 354, 5);
  r->add_node(55, 55, 764, 14);
  r->add_node(56, 56, 5094, 489);
  r->add_node(57, 57, 579, 48);
  r->add_node(58, 58, 12, 0);
  r->add_node(59, 59, 132, 3);
  r->add_node(60, 60, 709, 15);
  r->add_node(61, 61, 507, 9);
  r->add_node(62, 62, 9349, 266);
  r->add_node(63, 63, 1110, 104);
  r->add_node(64, 64, 802, 13);
  r->add_node(65, 65, 582, 8);
  r->add_node(66, 66, 362, 11);
  r->add_node(67, 67, 995, 28);
  r->add_node(68, 68, 707, 13);
  r->add_node(69, 69, 1118, 194);
  r->add_node(70, 70, 677, 11);
  r->add_node(71, 71, 352, 11);
  r->add_node(72, 72, 2398, 86);
  r->add_node(73, 73, 1362, 39);
  r->add_node(74, 74, 25, 1);
  r->add_node(75, 75, 656, 15);
  r->add_node(76, 76, 2240, 185);
  r->add_node(77, 77, 2193, 223);
  r->add_node(78, 78, 640, 9);
  r->add_node(79, 79, 496, 14);
  r->add_node(80, 80, 622, 3);
  r->add_node(81, 81, 1833, 61);
  r->add_node(82, 82, 1762, 71);
  r->add_node(83, 83, 13, 0);
  r->add_node(84, 84, 3006, 135);
  r->add_node(85, 85, 559, 14);
  r->add_node(86, 86, 89, 2);
  r->add_node(87, 87, 576, 12);
  r->add_node(88, 88, 253, 5);
  r->add_node(89, 89, 630, 38);
  r->add_node(90, 90, 5615, 213);
  r->add_node(91, 91, 661, 17);
  r->add_node(92, 92, 294, 6);
  r->add_node(93, 93, 533, 12);
  r->add_node(94, 94, 411, 16);
  r->add_node(95, 95, 827, 18);
  r->add_node(96, 96, 475, 13);
  r->add_node(97, 97, 331, 7);
  r->add_node(98, 98, 10, 0);
  r->add_edge(6, 9);
  r->add_edge(19, 92);
  r->add_edge(45, 98);
  r->add_edge(10, 17);
  r->add_edge(86, 87);
  r->add_edge(17, 74);
  r->add_edge(22, 81);
  r->add_edge(17, 20);
  r->add_edge(42, 82);
  r->add_edge(49, 63);
  r->add_edge(10, 46);
  r->add_edge(49, 78);
  r->add_edge(17, 46);
  r->add_edge(33, 65);
  r->add_edge(39, 98);
  r->add_edge(29, 70);
  r->add_edge(45, 75);
  r->add_edge(94, 97);
  r->add_edge(47, 56);
  r->add_edge(27, 56);
  r->add_edge(9, 21);
  r->add_edge(10, 80);
  r->add_edge(25, 88);
  r->add_edge(42, 77);
  r->add_edge(47, 53);
  r->add_edge(20, 75);
  r->add_edge(16, 65);
  r->add_edge(5, 47);
  r->add_edge(73, 75);
  r->add_edge(20, 70);
  r->add_edge(37, 85);
  r->add_edge(0, 4);
  r->add_edge(24, 76);
  r->add_edge(47, 78);
  r->add_edge(23, 82);
  r->add_edge(37, 41);
  r->add_edge(53, 89);
  r->add_edge(79, 86);
  r->add_edge(10, 75);
  r->add_edge(50, 89);
  r->add_edge(31, 73);
  r->add_edge(4, 82);
  r->add_edge(29, 73);
  r->add_edge(10, 70);
  r->add_edge(74, 83);
  r->add_edge(22, 48);
  r->add_edge(6, 85);
  r->add_edge(14, 77);
  r->add_edge(2, 32);
  r->add_edge(59, 71);
  r->add_edge(12, 80);
  r->add_edge(74, 96);
  r->add_edge(17, 80);
  r->add_edge(64, 72);
  r->add_edge(18, 65);
  r->add_edge(0, 1);
  r->add_edge(7, 84);
  r->add_edge(33, 34);
  r->add_edge(13, 23);
  r->add_edge(19, 60);
  r->add_edge(34, 41);
  r->add_edge(15, 52);
  r->add_edge(40, 54);
  r->add_edge(72, 86);
  r->add_edge(11, 41);
  r->add_edge(23, 66);
  r->add_edge(24, 36);
  r->add_edge(13, 38);
  r->add_edge(49, 84);
  r->add_edge(54, 73);
  r->add_edge(75, 93);
  r->add_edge(77, 82);
  r->add_edge(8, 18);
  r->add_edge(60, 76);
  r->add_edge(43, 57);
  r->add_edge(11, 16);
  r->add_edge(45, 54);
  r->add_edge(17, 83);
  r->add_edge(40, 45);
  r->add_edge(40, 98);
  r->add_edge(67, 89);
  r->add_edge(1, 87);
  r->add_edge(2, 95);
  r->add_edge(0, 14);
  r->add_edge(16, 94);
  r->add_edge(17, 96);
  r->add_edge(68, 72);
  r->add_edge(1, 68);
  r->add_edge(8, 9);
  r->add_edge(16, 97);
  r->add_edge(31, 54);
  r->add_edge(12, 36);
  r->add_edge(43, 91);
  r->add_edge(68, 86);
  r->add_edge(18, 95);
  r->add_edge(0, 87);
  r->add_edge(39, 41);
  r->add_edge(76, 84);
  r->add_edge(21, 27);
  r->add_edge(12, 13);
  r->add_edge(0, 24);
  r->add_edge(7, 24);
  r->add_edge(6, 32);
  r->add_edge(5, 56);
  r->add_edge(8, 32);
  r->add_edge(49, 62);
  r->add_edge(68, 77);
  r->add_edge(13, 82);
  r->add_edge(30, 48);
  r->add_edge(34, 98);
  r->add_edge(46, 66);
  r->add_edge(63, 85);
  r->add_edge(22, 52);
  r->add_edge(3, 89);
  r->add_edge(37, 63);
  r->add_edge(6, 11);
  r->add_edge(14, 68);
  r->add_edge(19, 90);
  r->add_edge(20, 31);
  r->add_edge(38, 60);
  r->add_edge(24, 90);
  r->add_edge(21, 30);
  r->add_edge(19, 79);
  r->add_edge(20, 71);
  r->add_edge(23, 96);
  r->add_edge(61, 78);
  r->add_edge(9, 56);
  r->add_edge(7, 93);
  r->add_edge(15, 69);
  r->add_edge(34, 37);
  r->add_edge(54, 94);
  r->add_edge(26, 79);
  r->add_edge(18, 32);
  r->add_edge(3, 67);
  r->add_edge(52, 56);
  r->add_edge(35, 64);
  r->add_edge(57, 69);
  r->add_edge(14, 82);
  r->add_edge(53, 61);
  r->add_edge(59, 70);
  r->add_edge(20, 73);
  r->add_edge(10, 20);
  r->add_edge(39, 84);
  r->add_edge(23, 80);
  r->add_edge(53, 91);
  r->add_edge(61, 62);
  r->add_edge(60, 87);
  r->add_edge(50, 91);
  r->add_edge(58, 67);
  r->add_edge(43, 55);
  r->add_edge(45, 93);
  r->add_edge(12, 75);
  r->add_edge(15, 81);
  r->add_edge(1, 86);
  r->add_edge(33, 44);
  r->add_edge(24, 60);
  r->add_edge(41, 84);
  r->add_edge(30, 52);
  r->add_edge(11, 34);
  r->add_edge(19, 58);
  r->add_edge(1, 14);
  r->add_edge(16, 98);
  r->add_edge(58, 90);
  r->add_edge(14, 38);
  r->add_edge(4, 13);
  r->add_edge(71, 83);
  r->add_edge(1, 72);
  r->add_edge(51, 57);
  r->add_edge(54, 75);
  r->add_edge(43, 50);
  r->add_edge(55, 88);
  r->add_edge(27, 32);
  r->add_edge(15, 22);
  r->add_edge(4, 38);
  r->add_edge(28, 43);
  r->add_edge(7, 76);
  r->add_edge(8, 33);
  r->add_edge(49, 61);
  r->add_edge(47, 85);
  r->add_edge(63, 78);
  r->add_edge(26, 92);
  r->add_edge(51, 91);
  r->add_edge(7, 36);
  r->add_edge(16, 33);
  r->add_edge(11, 33);
  r->add_edge(32, 95);
  r->add_edge(41, 63);
  r->add_edge(49, 76);
  r->add_edge(45, 73);
  r->add_edge(63, 84);
  r->add_edge(35, 68);
  r->add_edge(53, 78);
  r->add_edge(62, 76);
  r->add_edge(70, 83);
  r->add_edge(9, 27);
  r->add_edge(17, 70);
  r->add_edge(6, 8);
  r->add_edge(21, 32);
  r->add_edge(62, 67);
  r->add_edge(76, 90);
  r->add_edge(64, 77);
  r->add_edge(23, 46);
  r->add_edge(21, 95);
  r->add_edge(27, 30);
  r->add_edge(15, 51);
  r->add_edge(39, 93);
  r->add_edge(10, 73);
  r->add_edge(0, 60);
  r->add_edge(18, 33);
  r->add_edge(5, 9);
  r->add_edge(75, 80);
  r->add_edge(3, 25);
  r->add_edge(27, 52);
  r->add_edge(18, 44);
  r->add_edge(29, 71);
  r->add_edge(5, 6);
  r->add_edge(19, 26);
  r->add_edge(28, 55);
  r->add_edge(25, 89);
  r->add_edge(20, 29);
  r->add_edge(58, 62);
  r->add_edge(67, 92);
  r->add_edge(5, 85);
  r->add_edge(46, 96);
  r->add_edge(61, 67);
  r->add_edge(12, 93);
  r->add_edge(50, 53);
  r->add_edge(25, 67);
  r->add_edge(65, 97);
  r->add_edge(34, 39);
  r->add_edge(70, 71);
  r->add_edge(36, 38);
  r->add_edge(79, 87);
  r->add_edge(26, 58);
  r->add_edge(25, 50);
  r->add_edge(24, 38);
  r->add_edge(13, 36);
  r->add_edge(29, 31);
  r->add_edge(41, 98);
  r->add_edge(10, 12);
  r->add_edge(50, 88);
  r->add_edge(59, 83);
  r->add_edge(5, 78);
  r->add_edge(40, 94);
  r->add_edge(4, 23);
  r->add_edge(70, 74);
  r->add_edge(93, 98);
  r->add_edge(48, 52);
  r->add_edge(64, 68);
  r->add_edge(19, 87);
  r->add_edge(3, 58);
  r->add_edge(11, 18);
  r->add_edge(61, 89);
  r->add_edge(6, 37);
  r->add_edge(36, 93);
  r->add_edge(9, 32);
  r->add_edge(15, 56);
  r->add_edge(46, 80);
  r->add_edge(11, 37);
  r->add_edge(26, 87);
  r->add_edge(34, 40);
  r->add_edge(8, 11);
  r->add_edge(16, 40);
  r->add_edge(69, 81);
  r->add_edge(4, 14);
  r->add_edge(1, 79);
  r->add_edge(35, 72);
  r->add_edge(0, 38);
  r->add_edge(49, 85);
  r->add_edge(51, 56);
  r->add_edge(3, 92);
  r->add_edge(51, 69);
  r->add_edge(66, 96);
  r->add_edge(49, 90);
  r->add_edge(47, 51);
  r->add_edge(28, 57);
  r->add_edge(42, 66);
  r->add_edge(47, 91);
  r->add_edge(54, 98);
  r->add_edge(44, 65);
  r->add_edge(62, 90);
  r->add_edge(23, 42);
  r->add_edge(40, 97);
  r->add_edge(2, 21);
  r->add_edge(7, 39);
  r->add_edge(13, 80);
  r->add_edge(5, 51);
  r->add_edge(16, 34);
  r->add_edge(58, 92);
  r->add_edge(44, 95);
  r->add_edge(57, 91);
  r->add_edge(88, 89);
  r->add_edge(78, 85);
  r->add_edge(43, 88);
  r->add_edge(60, 90);
  return r;
}

Runner* Generate4x4Map(bool set_districts) {
  Runner *r = new Runner(16, 4);
  for (uint32_t i = 0; i < 17; i++) {
    r->add_node(i, 0, 0, 0);
  }
  r->add_edge(0, 1);
  r->add_edge(1, 2);
  r->add_edge(2, 3);
  r->add_edge(0, 4);
  r->add_edge(1, 5);
  r->add_edge(2, 6);
  r->add_edge(3, 7);
  r->add_edge(4, 5);
  r->add_edge(5, 6);
  r->add_edge(6, 7);
  r->add_edge(4, 8);
  r->add_edge(5, 9);
  r->add_edge(6, 10);
  r->add_edge(7, 11);
  r->add_edge(8, 9);
  r->add_edge(9, 10);
  r->add_edge(10, 11);
  r->add_edge(8, 12);
  r->add_edge(9, 13);
  r->add_edge(10, 14);
  r->add_edge(11, 15);
  r->add_edge(12, 13);
  r->add_edge(13, 14);
  r->add_edge(14, 15);

  if (set_districts) {
    vector<uint32_t> districts(16);

    // First quadrant in district 0
    districts[0] = 0;
    districts[1] = 0;
    districts[4] = 0;
    districts[5] = 0;

    // Second quadrant in district 1
    districts[2] = 1;
    districts[3] = 1;
    districts[6] = 1;
    districts[7] = 1;

    // Third quadrant in district 2
    districts[8] = 2;
    districts[9] = 2;
    districts[12] = 2;
    districts[13] = 2;

    // Fourth quadrant in district 3
    districts[10] = 3;
    districts[11] = 3;
    districts[14] = 3;
    districts[15] = 3;

    r->set_districts(districts);
  }

  return r;
}

Runner* GenerateHMap(bool set_districts) {
  Runner *r = new Runner(7, 3);
  for (uint32_t i = 0; i < 7; i++) {
    r->add_node(i, 0, 0, 0);
  }
  r->add_edge(0, 1);
  r->add_edge(1, 2);
  r->add_edge(1, 3);
  r->add_edge(4, 5);
  r->add_edge(5, 6);
  r->add_edge(3, 5);

  if (set_districts) {
    vector<uint32_t> districts(7);
    districts[0] = 0;
    districts[1] = 0;
    districts[2] = 0;
    districts[3] = 1;
    districts[4] = 2;
    districts[5] = 2;
    districts[6] = 2;
    r->set_districts(districts);
  }

  return r;
}

Runner* GenerateHourglassMap(bool set_districts) {
  Runner *r = new Runner(7, 3);
  for (uint32_t i = 0; i < 7; i++) {
    r->add_node(i, 0, 0, 0);
  }
  r->add_edge(0, 1);
  r->add_edge(0, 3);
  r->add_edge(1, 2);
  r->add_edge(1, 3);
  r->add_edge(2, 3);
  r->add_edge(3, 4);
  r->add_edge(3, 5);
  r->add_edge(3, 6);
  r->add_edge(4, 5);
  r->add_edge(5, 6);

  if (set_districts) {
    vector<uint32_t> districts(7);
    districts[0] = 0;
    districts[1] = 1;
    districts[2] = 1;
    districts[3] = 0;
    districts[4] = 0;
    districts[5] = 2;
    districts[6] = 2;
    r->set_districts(districts);
  }

  return r;
}

TEST(Test_Runner, TestRedistrict4x4Map) {
  Runner *r = Generate4x4Map(true);
  Graph *g = r->GetGraph();
  Node *victim_node, *idle_node;
  unordered_set<Edge, EdgeHash> *crossing_edges;
  unordered_set<Edge, EdgeHash> expected_crossing_edges;
  expected_crossing_edges.emplace(1, 2);
  expected_crossing_edges.emplace(5, 6);
  expected_crossing_edges.emplace(4, 8);
  expected_crossing_edges.emplace(5, 9);
  expected_crossing_edges.emplace(6, 10);
  expected_crossing_edges.emplace(7, 11);
  expected_crossing_edges.emplace(9, 10);
  expected_crossing_edges.emplace(13, 14);

  crossing_edges = g->GetCrossingEdges();
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());

  // Node 1 currently in district 0
  victim_node = g->GetNode(1);
  idle_node = g->GetNode(2);
  ASSERT_EQ(victim_node->GetDistrict(), 0);

  // Redistrict node 1 -> node 2
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 1);
  ASSERT_EQ(g->GetPerimNodeNeighbors(0, 1), nullptr);

  Edge e1(1, 2);
  expected_crossing_edges.erase(e1);
  expected_crossing_edges.emplace(0, 1);
  expected_crossing_edges.emplace(1, 5);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  unordered_set<Edge, EdgeHash>::iterator itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 3);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);

  // Node 9 currently in district 2
  victim_node = g->GetNode(9);
  idle_node = g->GetNode(5);
  ASSERT_EQ(victim_node->GetDistrict(), 2);

  // Redistrict node 9 -> node 5
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 0);
  ASSERT_EQ(g->GetPerimNodeNeighbors(2, 9), nullptr);

  Edge e2(5, 9);
  expected_crossing_edges.erase(e2);
  expected_crossing_edges.emplace(8, 9);
  expected_crossing_edges.emplace(9, 13);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 3);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);

  // Node 13 currently in district 2
  victim_node = g->GetNode(13);
  idle_node = g->GetNode(9);
  ASSERT_EQ(victim_node->GetDistrict(), 2);

  // Redistrict node 13 -> node 9
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 0);
  ASSERT_EQ(g->GetPerimNodeNeighbors(2, 13), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(2, 12), nullptr);

  Edge e3(9, 13);
  expected_crossing_edges.erase(e3);
  expected_crossing_edges.emplace(12, 13);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);

  // Node 7 currently in district 1
  victim_node = g->GetNode(7);
  idle_node = g->GetNode(11);
  ASSERT_EQ(victim_node->GetDistrict(), 1);

  // Redistrict node 7 -> node 11
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 3);
  ASSERT_EQ(g->GetPerimNodeNeighbors(1, 7), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(3, 7), nullptr);

  Edge e4(7, 11);
  expected_crossing_edges.erase(e4);
  expected_crossing_edges.emplace(3, 7);
  expected_crossing_edges.emplace(6, 7);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 5);

  // Redistrict node 7 -> node 3
  idle_node = g->GetNode(3);
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 1);
  ASSERT_EQ(g->GetPerimNodeNeighbors(3, 7), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(1, 7), nullptr);
  ASSERT_EQ(g->GetPerimNodeNeighbors(3, 7), nullptr);

  Edge e5(3, 7);
  Edge e6(6, 7);
  expected_crossing_edges.erase(e5);
  expected_crossing_edges.erase(e6);
  expected_crossing_edges.emplace(7, 11);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 5);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(3)->size(), 4);
}

TEST(Test_Runner, TestRedistrictHourglassMap) {
  Runner *r = GenerateHourglassMap(true);
  Graph *g = r->GetGraph();
  Node *victim_node, *idle_node;
  unordered_set<Edge, EdgeHash> *crossing_edges;
  unordered_set<Edge, EdgeHash> expected_crossing_edges;
  expected_crossing_edges.emplace(0, 1);
  expected_crossing_edges.emplace(1, 3);
  expected_crossing_edges.emplace(2, 3);
  expected_crossing_edges.emplace(3, 5);
  expected_crossing_edges.emplace(3, 6);
  expected_crossing_edges.emplace(4, 5);
  crossing_edges = g->GetCrossingEdges();
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());

  // Node 3 currently in district 0, try to move it into district 1
  // Should NOT be allowed because district 0 will be disconnected
  victim_node = g->GetNode(3);
  idle_node = g->GetNode(1);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 0);

  // Node 1 currently in district 1
  victim_node = g->GetNode(1);
  idle_node = g->GetNode(3);

  // Redistrict node 1 -> node 3
  ASSERT_TRUE(r->IsValidRedistricting(victim_node, idle_node));
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 0);
  ASSERT_EQ(g->GetPerimNodeNeighbors(1, 1), nullptr);
  ASSERT_EQ(g->GetPerimNodeNeighbors(0, 0), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(0, 1), nullptr);

  Edge e1(0, 1);
  Edge e2(1, 3);
  expected_crossing_edges.erase(e1);
  expected_crossing_edges.erase(e2);
  expected_crossing_edges.emplace(1, 2);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  unordered_set<Edge, EdgeHash>::iterator itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 4);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 1);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);

  // Node 2 currently in district 1, try to move it into district 0
  // Should NOT be allowed because district 1 will be empty
  victim_node = g->GetNode(2);
  idle_node = g->GetNode(1);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 1);

  // Node 3 currently in district 0, try to move it into district 2
  // Should NOT be allowed because district 0 will be disconnected
  victim_node = g->GetNode(3);
  idle_node = g->GetNode(5);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 0);

  // Node 4 currently in district 0
  victim_node = g->GetNode(4);
  idle_node = g->GetNode(5);

  // Redistrict node 4 -> node 5
  ASSERT_TRUE(r->IsValidRedistricting(victim_node, idle_node));
  r->Redistrict(victim_node, idle_node);
  ASSERT_EQ(victim_node->GetDistrict(), 2);
  ASSERT_EQ(g->GetPerimNodeNeighbors(0, 4), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(2, 4), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(0, 3), nullptr);
  ASSERT_NE(g->GetPerimNodeNeighbors(2, 5), nullptr);

  Edge e3(4, 5);
  expected_crossing_edges.erase(e3);
  expected_crossing_edges.emplace(3, 4);
  ASSERT_EQ(crossing_edges->size(), expected_crossing_edges.size());
  itr = expected_crossing_edges.begin();
  for (uint32_t i = 0; i < expected_crossing_edges.size(); i++) {
    Edge expected = *itr;
    ASSERT_NE(crossing_edges->find(expected), crossing_edges->end());
    Edge actual = *crossing_edges->find(expected);
    ASSERT_EQ(expected, expected);
    itr++;
  }
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 3);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 1);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 3);

  // Node 5 currently in district 2, try to move it into district 0
  // Should NOT be allowed because district 2 will be disconnected
  victim_node = g->GetNode(5);
  idle_node = g->GetNode(3);
  ASSERT_FALSE(r->IsValidRedistricting(victim_node, idle_node));
  ASSERT_EQ(victim_node->GetDistrict(), 2);
}

TEST(Test_Runner, TestSeveredDistrictSimple) {
  // r has 7 precincts, 3 districts
  // draws out an H shape
  Runner *r = GenerateHMap(true);
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(0), 1));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(1), 1));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(2), 1));
  ASSERT_FALSE(r->IsDistrictSevered(r->GetGraph()->GetNode(3), 0));
  ASSERT_FALSE(r->IsDistrictSevered(r->GetGraph()->GetNode(3), 2));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(4), 0));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(5), 1));
  ASSERT_TRUE(r->IsDistrictSevered(r->GetGraph()->GetNode(6), 0));
}

TEST(Test_Runner, TestIsValidRedistricting) {
  // r has 16 precincts, 4 districts
  // draws out a 4x4 square with 4 quadrants
  Runner *r = Generate4x4Map(true);
  Graph *g = r->GetGraph();

  // Redistricting to self
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(0)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(2), g->GetNode(2)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(11), g->GetNode(11)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(13), g->GetNode(13)));

  // Same districts
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(1)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(4)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(1)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(4)));

  // Just crossing, no severedness
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(1), g->GetNode(2)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(6)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(4), g->GetNode(8)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(9)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(6), g->GetNode(10)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(7), g->GetNode(11)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(9), g->GetNode(10)));
  ASSERT_TRUE(r->IsValidRedistricting(g->GetNode(13), g->GetNode(14)));

  // No edge
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(2)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(1), g->GetNode(3)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(8)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(5), g->GetNode(10)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(6), g->GetNode(11)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(10), g->GetNode(13)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(8), g->GetNode(10)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(12), g->GetNode(14)));
  ASSERT_FALSE(r->IsValidRedistricting(g->GetNode(0), g->GetNode(15)));
}

TEST(Test_Runner, TestSeedSimple) {
  // r has 2 precincts, 1 district
  Runner r(2, 1);
  r.add_node(0, 1, 0, 0);
  r.add_node(1, 1, 0, 0);
  r.add_edge(0, 1);
  r.seed();

  ASSERT_TRUE(r.IsDistrictConnected(0));
  Graph *g = r.GetGraph();
  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 2);
  ASSERT_EQ(g->GetNode(0)->GetDistrict(), 0);
  ASSERT_EQ(g->GetNode(1)->GetDistrict(), 0);
}

TEST(Test_Runner, TestSeedMedium) {
  // r has 6 precincts, 3 district
  // n0 <-> n1
  // n2 <-> n3
  // n4 <-> n5
  Runner r(6, 3);
  r.add_node(0, 1, 0, 0);
  r.add_node(1, 1, 0, 0);
  r.add_node(2, 1, 0, 0);
  r.add_node(3, 1, 0, 0);
  r.add_node(4, 1, 0, 0);
  r.add_node(5, 1, 0, 0);
  r.add_edge(0, 1);
  r.add_edge(2, 3);
  r.add_edge(4, 5);

  unordered_set<Node *> seed_nodes;
  Graph *g = r.GetGraph();
  seed_nodes.insert(g->GetNode(0));
  seed_nodes.insert(g->GetNode(2));
  seed_nodes.insert(g->GetNode(4));
  g->AddNodeToDistrict(0, 0);
  g->AddNodeToDistrict(2, 1);
  g->AddNodeToDistrict(4, 2);

  r.SpawnDistricts(&seed_nodes);

  ASSERT_TRUE(r.IsDistrictConnected(0));
  ASSERT_TRUE(r.IsDistrictConnected(1));
  ASSERT_TRUE(r.IsDistrictConnected(2));

  ASSERT_EQ(g->GetNodesInDistrict(0)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(1)->size(), 2);
  ASSERT_EQ(g->GetNodesInDistrict(2)->size(), 2);

  ASSERT_EQ(g->GetNode(0)->GetDistrict(), g->GetNode(1)->GetDistrict());
  ASSERT_EQ(g->GetNode(2)->GetDistrict(), g->GetNode(3)->GetDistrict());
  ASSERT_EQ(g->GetNode(4)->GetDistrict(), g->GetNode(5)->GetDistrict());
}

TEST(Test_Runner, TestSeedHMap) {
  Runner *r = GenerateHMap(false);
  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
}

TEST(Test_Runner, TestSeed4x4Map) {
  Runner *r = Generate4x4Map(false);
  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));
}

TEST(Test_Runner, TestSeedIowaMap) {
  Runner *r = GenerateIowa();
  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));
}

TEST(Test_Runner, TestWalkOneStep) {
  Runner *r = Generate4x4Map(false);
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));
  r->Walk(1, 0, 0, 0, 0);
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap(false);
  ASSERT_TRUE(r1->seed());
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  r1->Walk(1, 0, 0, 0, 0);
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());

  Runner *r2 = GenerateIowa();
  ASSERT_TRUE(r2->seed());
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));
  r2->Walk(1, 0, 0, 0, 0);
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));

  g = r2->GetGraph();
  all_maps = r2->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());
}

TEST(Test_Runner, TestWalkTwoSteps) {
  Runner *r = Generate4x4Map(false);
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));
  r->Walk(2, 0, 0, 0, 0);
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap(false);
  ASSERT_TRUE(r1->seed());
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  r1->Walk(2, 0, 0, 0, 0);
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());

  Runner *r2 = GenerateIowa();
  ASSERT_TRUE(r2->seed());
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));
  r2->Walk(2, 0, 0, 0, 0);
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));

  g = r2->GetGraph();
  all_maps = r2->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());
}

TEST(Test_Runner, TestWalkThreeSteps) {
  Runner *r = Generate4x4Map(false);
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));
  r->Walk(3, 0, 0, 0, 0);
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap(false);
  ASSERT_TRUE(r1->seed());
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  r1->Walk(3, 0, 0, 0, 0);
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());

  Runner *r2 = GenerateIowa();
  ASSERT_TRUE(r2->seed());
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));
  r2->Walk(3, 0, 0, 0, 0);
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));

  g = r2->GetGraph();
  all_maps = r2->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());
}

TEST(Test_Runner, TestWalkTenSteps) {
  Runner *r = Generate4x4Map(false);
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));
  r->Walk(10, 0, 0, 0, 0);
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap(false);
  ASSERT_TRUE(r1->seed());
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  r1->Walk(10, 0, 0, 0, 0);
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());

  Runner *r2 = GenerateIowa();
  ASSERT_TRUE(r2->seed());
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));
  r2->Walk(10, 0, 0, 0, 0);
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));

  g = r2->GetGraph();
  all_maps = r2->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());
}

TEST(Test_Runner, TestWalkHundredSteps) {
  Runner *r = Generate4x4Map(false);
  Graph *g;
  vector<vector<uint32_t>> all_maps;
  vector<uint32_t> map;
  uint32_t i;
  unordered_map<uint32_t, vector<uint32_t>> district_nodes;

  ASSERT_TRUE(r->seed());
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));
  r->Walk(100, 0, 0, 0, 0);
  ASSERT_TRUE(r->IsDistrictConnected(0));
  ASSERT_TRUE(r->IsDistrictConnected(1));
  ASSERT_TRUE(r->IsDistrictConnected(2));
  ASSERT_TRUE(r->IsDistrictConnected(3));

  g = r->GetGraph();
  all_maps = r->getMaps();
  map = all_maps[all_maps.size() - 1];

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());

  Runner *r1 = GenerateHourglassMap(false);
  ASSERT_TRUE(r1->seed());
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  r1->Walk(100, 0, 0, 0, 0);
  ASSERT_TRUE(r1->IsDistrictConnected(0));
  ASSERT_TRUE(r1->IsDistrictConnected(1));
  ASSERT_TRUE(r1->IsDistrictConnected(2));
  g = r1->GetGraph();
  all_maps = r1->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());

  Runner *r2 = GenerateIowa();
  ASSERT_TRUE(r2->seed());
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));
  r2->Walk(100, 0, 0, 0, 0);
  ASSERT_TRUE(r2->IsDistrictConnected(0));
  ASSERT_TRUE(r2->IsDistrictConnected(1));
  ASSERT_TRUE(r2->IsDistrictConnected(2));
  ASSERT_TRUE(r2->IsDistrictConnected(3));

  g = r2->GetGraph();
  all_maps = r2->getMaps();
  map = all_maps[all_maps.size() - 1];
  district_nodes.clear();

  for (i = 0; i < g->GetNumNodes(); i++) {
      district_nodes[map[i]].push_back(i);
  }

  ASSERT_EQ(district_nodes[0].size(), g->GetNodesInDistrict(0)->size());
  ASSERT_EQ(district_nodes[1].size(), g->GetNodesInDistrict(1)->size());
  ASSERT_EQ(district_nodes[2].size(), g->GetNodesInDistrict(2)->size());
  ASSERT_EQ(district_nodes[3].size(), g->GetNodesInDistrict(3)->size());
}

}     // namespace rakan
