import unittest

class testAPIExist(unittest.TestCase):
    def setUp(self):
        from rakan import Engine
        self.runnerClass = Engine

    def test_documented_api(self):
        "Test that this class has the documented API"
        self.assertTrue(hasattr(self.runnerClass, "_addNode"))
        self.assertTrue(hasattr(self.runnerClass, "_addEdge"))
        self.assertTrue(hasattr(self.runnerClass, "setDistricts"))
        self.assertTrue(hasattr(self.runnerClass, "getMaps"))
        self.assertTrue(hasattr(self.runnerClass, "getScores"))
        self.assertTrue(hasattr(self.runnerClass, "seed"))
        self.assertTrue(hasattr(self.runnerClass, "walk"))
        self.assertTrue(hasattr(self.runnerClass, "districts"))
        self.assertTrue(hasattr(self.runnerClass, "visualize"))

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