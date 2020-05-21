# Statuses: 'idle', 'seeding', 'walking', 'starting'

class Engines():

    def Engines(self, maxThreads):
        self.threads = dict
        self.backlog = list # queue of guids

    def loadMap(self, guid, jsonLocation):
        pass

    def setDistricts(self, guid, districts):
        print('mock setDistricts')

    def getMaps(self, guid):
        print('mock getMaps')
        return [[self.districts - 1] * self.precincts]

    def getScores(self, guid):
        print('mock getScores')
        return[{'compactness': 0.1, 'vra': 0.8, 'population':0.6, 'political': 0.76}]

    def seed(self, guid):
        print('mock seed')

    def walk(self, guid, stepsToTake, alpha, beta, gamma, eta):
        print('mock walk')

    def newThread(self, guid):
        return False # if successful

    def terminateThread(self, guid):
        return False # if successful

    def getStatuses(): # dict{'guid':status}
        pass