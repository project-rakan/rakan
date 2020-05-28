import unittest
import os

class TestVisualize(unittest.TestCase):

    def setUp(self):
        from rakan import Engine
        self.runnerClass = Engine
        self.runnerInstance = Engine(os.path.join(os.getenv('RAKAN_STATEFILES'), 'IA.json'))

    def test_apiExists(self):
        self.assertTrue(hasattr(self.runnerClass, 'visualize'))
        self.assertTrue(hasattr(self.runnerInstance, 'visualize'))

    def test_iowaOutput(self):
        self.runnerInstance.visualize("output.jpg")
        self.assertTrue(os.path.isfile("output.jpg"))