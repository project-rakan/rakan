# distutils: language = c++

from cython.operator import dereference, preincrement, address

from libcpp.vector cimport vector as cvector
from libcpp.string cimport string as cstring
from libc.stdint cimport uint32_t as cuint32_t

from wrapper cimport Runner as cRunner

import pickle
import io

cdef class Engine:
    cdef cRunner* _runner
    cpdef int _districts
    cpdef int _precincts
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

    def __dealloc__(self):
        del self._runner

    def _addNode(self, int id, int county, int minorityPopulation, int majorityPoplation):
        dereference(self._runner).add_node(id, county, minorityPopulation, majorityPoplation);

    def _addEdge(self, int id1, int id2):
        dereference(self._runner).add_edge(id1, id2)

    def setDistricts(self, cvector[cuint32_t] districts):
        dereference(self._runner).set_districts(districts)

    def getMaps(self):
        return dereference(self._runner).getMaps()

    def getScores(self):
        return dereference(self._runner).getScores()

    def seed(self):
        return dereference(self._runner).seed()

    def walk(self, int stepsToTake, double alpha, double beta, double gamma, double eta):
        return dereference(self._runner).Walk(stepsToTake, alpha, beta, gamma, eta)

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