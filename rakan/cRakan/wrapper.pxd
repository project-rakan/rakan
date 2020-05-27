# distutils: language=c++

from libcpp.string cimport string as cstring
from libcpp.vector cimport vector as cvector
from libcpp cimport bool as cbool
from libc.stdint cimport uint32_t
from libcpp.map cimport map as cmap

cdef extern from "src/Runner.cc" namespace "rakan":
    pass

cdef extern from "src/Runner.h" namespace "rakan":
    cdef cppclass Runner:
        Runner() except +;
        Runner(uint32_t num_precincts, uint32_t num_districts) except +;
        cbool add_node(uint32_t node_id,uint32_t county, uint32_t majority_population, uint32_t minority_population) except +;
        cbool add_edge(uint32_t node_one, uint32_t node_two) except +;
        cbool set_districts(cvector[uint32_t]& districts) except +;
        cbool seed() except +;
        void populate() except +;
        double ScoreCompactness() except +;
        double ScorePopulationDistribution() except +;
        double ScoreExistingBorders() except +;
        double ScoreVRA() except +;
        double LogScore() except +;
        double Walk(uint32_t num_steps, double alpha, double beta, double gamma, double eta) except +;
        cvector[cvector[uint32_t]] getMaps() except +;
        cvector[cmap[cstring, double]] getScores() except +;
