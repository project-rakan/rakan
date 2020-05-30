# distutils: language = c++

from cython.operator import dereference, preincrement, address

from libcpp.vector cimport vector as cvector
from libcpp.string cimport string as cstring
from libc.stdint cimport uint32_t as cuint32_t
from libcpp cimport bool as cbool

from wrapper cimport Runner as cRunner

import pickle
import io

cdef class Engine:
    cdef cRunner* _runner
    cpdef int _districts
    cpdef int _precincts
    cpdef cbool _redistricted;
    cpdef cstring _filepath;

    def __cinit__(self, binLocation):
        "Create an new engine with the initialization data in the binLocation"

        with io.open(binLocation, 'rb') as handle:
            data = pickle.loads(handle.read())

        self._runner = new cRunner(self._precincts, self._districts)

        # first pass to add all nodes
        for prec in data['precincts']:
            self._addNode(prec['nodeID'], prec['county'], prec['minPopulation'], prec['majPopulation'])
        
        #second pass to add all edges
        for brother, sister in data['edges']:
            self._addEdge(brother, sister)

        self._redistricted = False

    def __dealloc__(self):
        del self._runner

    def _addNode(self, int id, int county, int minorityPopulation, int majorityPoplation):
        if not dereference(self._runner).add_node(id, county, minorityPopulation, majorityPoplation):
            raise ValueError("Unable to add Node")

    def _addEdge(self, int id1, int id2):
        if not dereference(self._runner).add_edge(id1, id2):
            raise ValueError(f"Incorrect ids: {id1} {id2}")

    def setDistricts(self, cvector[cuint32_t] districts):
        if not dereference(self._runner).set_districts(districts):
            raise ValueError()

    def getMaps(self):
        if self._redistricted:
            return dereference(self._runner).getMaps()
        raise Exception("Map is not redistricted")

    def getScores(self):
        if self._redistricted:
            return dereference(self._runner).getScores()
        raise Exception("Map is not redistricted")

    def seed(self):
        self._redistricted = True
        return dereference(self._runner).seed()

    def walk(self, int stepsToTake, double alpha, double beta, double gamma, double eta):
        if self._redistricted:
            return dereference(self._runner).Walk(stepsToTake, alpha, beta, gamma, eta)
        raise Exception("Map is not redistricted")

    @property
    def districts(self):
        graph = dereference(self._runner).GetGraph()

        districts = []
        for node_id in range(self._precincts):
            district = dereference(graph.GetNode(node_id)).GetDistrict()
            districts.append(district)

        return districts

    def visualize(self, output = "output.jpg"):
        "Creates an image with the file name output"
        



# dereference(self._runner)