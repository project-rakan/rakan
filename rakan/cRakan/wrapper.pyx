# distutils: language = c++

from cython.operator import dereference, preincrement, address

from libcpp.string cimport string as cstring

from wrapper cimport Runner as cRunner
from wrapper cimport Graph as cGraph

cdef class Runner:
    cdef cRunner* _runner

    def __cinit__(self):
        self._runner = new cRunner()

    def __dealloc__(self):
        del self._runner

    def addNode(self, int id, int county, int minorityPopulation, int majorityPoplation):
        print('addNode')
        print(id, county, minorityPopulation, majorityPoplation)

    def addEdge(self, int id1, int id2):
        print('addEdge')

    def setDistricts(int districts):
        print('setDistricts')

    def populate(self):
        print('populate')

    def getMaps(self):
        print('getMaps')

    def getScores(self):
        print('getScores')

    def seed(self):
        print('seed')

    def walk(int stepsToTake, double alpha, double beta, double gamma, double eta):
        print('walk')


cdef class Graph:
    cdef cGraph* _graph

    def __cinit__(self):
        self._graph = new cGraph()

    def __dealloc__(self):
        del self._graph

    def getNumNodes(self):
        return dereference(self._graph).GetNumNodes()

    def addNode(self, node):
        pass

# dereference(self._runner)