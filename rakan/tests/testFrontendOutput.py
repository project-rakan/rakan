import json
import unittest
import os

class testFrontendOutput(unittest.TestCase):
    def testStateJSON(self):
        "Goes through the entire json file and checks that all fields are the same as expected"
        location = os.path.join(os.getenv('RAKAN_STATEFILES'), 'IA.json')
        with open(location) as f:
            actual = json.load(f)

        with open('expected/IA.json') as f:
            expected = json.load(f)
        
        
        self.assertEqual(actual["state"], expected['state'], 'IA')
        self.assertEqual(actual["maxDistricts"], expected["maxDistricts"], 4)
        self.assertEqual(actual["fips"], expected["fips"], 19)

        actPrecincts = actual["precincts"]
        expPrecincts = expected["precincts"]
        self.assertEqual(len(actPrecincts), len(expPrecincts), 99)
        
        for i in range(99):
            act = actPrecincts[i]
            exp = expPrecincts[i]
            self.assertEqual(act['name'][1:7], exp['name'][1:7]) # check that the name starts with "county"
            self.assertEqual(act['id'], exp['id'])
            self.assertEqual(len(act['vertices']), len(exp['vertices']))
            for j in range(len(exp['vertices'])):
                actV = act['vertices'][j]
                expV = exp['vertices'][j]
                self.assertAlmostEqual(actV['lat'], expV['lat'])
                self.assertAlmostEqual(actV['lng'], expV['lng'])

    def testDistrictsJSON(self):
        location = os.path.join(os.getenv('RAKAN_STATEFILES'), 'IA.districts.json')
        with open(location) as f:
            actual = json.load(f)
        
        with open('expected/IA.districts.json') as f:
            expected = json.load(f)

        self.assertEqual(actual, expected) # The entire data structure should be the same

if __name__ == '__main__':
    unittest.main()