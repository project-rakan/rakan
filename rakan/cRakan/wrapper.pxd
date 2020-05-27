# distutils: language=c++

from libcpp.string cimport string as cstring
from libcpp.unordered_map cimport unordered_map as cunordered_map
from libcpp.unordered_set cimport unordered_set as cunordered_set
from libcpp cimport bool as cbool
from libc.stdint cimport uint16_t as cuint16_t
from libc.stdint cimport uint32_t as cuint32_t
from libcpp.map cimport map as cmap

cdef extern from "src/Runner.cc" namespace "rakan":
    pass

cdef extern from "src/Runner.h" namespace "rakan":
    cdef cppclass Runner:
        Runner() except +;
        Runner(Graph &g) except +;
        cuint16_t SetDistricts(cunordered_map[cuint32_t, cuint32_t] &map) except +;
        cuint16_t SeedDistricts() except +;
        cuint16_t PopulateGraphData() except +;
        double ScoreCompactness() except +;
        double ScorePopulationDistribution() except +;
        double ScoreExistingBorders() except +;
        double ScoreVRA() except +;
        double LogScore() except +;
        double MetropolisHastings() except +;
        double Redistrict(Node &node, int new_district) except +;
        double Walk(int num_steps) except +;

cdef extern from "src/Graph.cc" namespace "rakan":
    pass

cdef extern from "src/Graph.h" namespace "rakan":
    cdef cppclass Graph:
        Graph();
        Graph(const cuint32_t &num_nodes, const cuint32_t &num_districts, const cuint32_t &state_pop);
        cbool ContainsNode(const Node& node) const;
        cbool ContainsEdge(const Node& node1, const Node& node2) const;
        cbool NodeExistsInDistrict(const Node& node, cuint32_t &district) const;
        cuint32_t GetNumNodes() const;
        cbool AddNode(Node &node);


cdef extern from "src/Node.cc" namespace "rakan":
    pass

cdef extern from "src/Node.h" namespace "rakan":
    cdef cppclass Node:
        Node();
