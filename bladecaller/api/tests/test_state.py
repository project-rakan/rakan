from django.test import TestCase
from api.models import State

import pickle

class StateTests(TestCase):
    fixtures = ['iowa_washington_fixture.json']

    def test_generatesPayloadWithKeys(self):
        iowa = State.objects.get(fips=19)
        handle = iowa.stateEngineData()
        payload = pickle.loads(handle.read())

        self.assertTrue(isinstance(payload, dict))
        self.assertTrue('edges' in payload)
        self.assertTrue('precincts' in payload)
        self.assertTrue('stCode' in payload)
        self.assertTrue('numPrecincts' in payload)
        self.assertTrue('numDistricts' in payload)

    def test_generatesPayloadWithCorrectStateValues(self):
        iowa = State.objects.get(fips=19)
        handle = iowa.stateEngineData()
        payload = pickle.loads(handle.read())

        self.assertEqual(payload['stCode'], iowa.state)
        self.assertEqual(payload['numPrecincts'], iowa.vtds.all().count())
        self.assertEqual(payload['numDistricts'], iowa.maxDistricts)

