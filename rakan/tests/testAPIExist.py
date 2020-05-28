import unittest

class testAPIExist(unittest.TestCase):
    def setUp(self):
        from rakan import Engine
        self.engineClass = Engine

    def test_documented_api(self):
        "Test that this class has the documented API"
        self.assertTrue(hasattr(self.engineClass, "_addNode"))
        self.assertTrue(hasattr(self.engineClass, "_addEdge"))
        self.assertTrue(hasattr(self.engineClass, "setDistricts"))
        self.assertTrue(hasattr(self.engineClass, "getMaps"))
        self.assertTrue(hasattr(self.engineClass, "getScores"))
        self.assertTrue(hasattr(self.engineClass, "seed"))
        self.assertTrue(hasattr(self.engineClass, "walk"))
        self.assertTrue(hasattr(self.engineClass, "districts"))
        self.assertTrue(hasattr(self.engineClass, "visualize"))

    def test_initialize_from_json(self):
        engine = self.engineClass("./testData/iowa.runner.json")

    def test_canLoad(self):
        "Test that this class can be instantiated"
        self.runnerClass("/var/www/html/stateinfo/IA.json")

"""
    def setUp(self):
        from cRakan import Queue
        self.item_class = Queue

    def test_documented_api(self):
        "Test that this class has the documented API"
        self.assertTrue(hasattr(self.item_class, "push"))
        self.assertTrue(hasattr(self.item_class, "pushFront"))
        self.assertTrue(hasattr(self.item_class, "pop"))
        self.assertTrue(hasattr(self.item_class, "size"))
    
    def test_push(self):
        item = self.item_class()
        self.assertEqual(item.size(), 0)
        item.push(b"test")
        self.assertEqual(item.size(), 1)
        self.assertEqual(item.pop(), b"test")
        self.assertEqual(item.size(), 0)

    def test_pushFront(self):
        item = self.item_class()
        self.assertEqual(item.size(), 0)
        item.push(b"test2")
        self.assertEqual(item.size(), 1)
        item.pushFront(b"test")
        self.assertEqual(item.size(), 2)
        self.assertEqual(item.pop(), b"test")
        self.assertEqual(item.size(), 1)
        self.assertEqual(item.pop(), b"test2")
        self.assertEqual(item.size(), 0)

    def test_push_empty(self):
        item = self.item_class()
        self.assertEqual(item.size(), 0)
        self.assertRaises(RuntimeError, item.pop)
"""