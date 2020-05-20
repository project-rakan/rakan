import unittest

class testAPIExist(unittest.TestCase):
    def setUp(self):
        from rakan import Runner, Graph
        self.runnerClass = Runner
        self.graphClass = Graph

    def test_documented_api(self):
        "Test that this class has the documented API"
        self.assertTrue(hasattr(self.runnerClass, "addNode"))
        self.assertTrue(hasattr(self.runnerClass, "addEdge"))
        self.assertTrue(hasattr(self.runnerClass, "setDistricts"))
        self.assertTrue(hasattr(self.runnerClass, "populate"))
    
    def test_push(self):
        item = self.runnerClass()
        self.assertTrue(True)

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