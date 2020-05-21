# distutils: language = c++

from cython.operator import dereference, preincrement, address

from libcpp.string cimport string as cstring

from wrapper cimport Runner as cRunner
from wrapper cimport Graph as cGraph

cdef class Engine:
    cdef cRunner* _runner
    cdef cGraph* _graph

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
# dereference(self._runner)