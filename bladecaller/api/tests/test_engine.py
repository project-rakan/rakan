from django.test import TestCase
from rest_framework.test import APITestCase


# Create your tests here.

class EngineTests(APITestCase):
    
    def setUp(self):
        from api.models import State
        self.ex_state = State.objects.create(
            state='EX',
            maxDistricts=2,
            fips=0,
            precincts=4
        )

    def test_get_startMap(self):
        response = self.client.get('/startjob', format='json')
        self.assertEqual(response.status_code, 405)

    def test_post_startMap_missing_id(self):
        response = self.client.post('/startjob', {}, format='json')
        self.assertEqual(response.status_code, 400)
        self.assertTrue('msg' in response.data.keys())
        self.assertTrue('missing_field' in response.data.keys())
        self.assertTrue('missing' in response.data['msg'].lower())
        self.assertEqual(response.data['missing_field'], 'id')

    def test_post_startMap_missing_state(self):
        guid = '0'
        response = self.client.post('/startjob', {'id': guid}, format='json')
        self.assertEqual(response.status_code, 400)
        self.assertTrue('msg' in response.data.keys())
        self.assertTrue('missing_field' in response.data.keys())
        self.assertTrue('missing' in response.data['msg'].lower())
        self.assertEqual(response.data['missing_field'], 'state')

    def test_post_startMap_missing_weights(self):
        guid = '0'
        response = self.client.post('/startjob', {'id': guid, 'state': 'EX'}, format='json')
        self.assertEqual(response.status_code, 400)
        self.assertTrue('msg' in response.data.keys())
        self.assertTrue('missing_field' in response.data.keys())
        self.assertTrue('missing' in response.data['msg'].lower())
        self.assertEqual(response.data['missing_field'], 'alpha')

    def test_post_startMap_duplicate_job(self):
        from api.models import Job
        guid = '0'
        job = Job.objects.create(jobId=guid, state=self.ex_state, steps=1, alpha=0.0, beta=0.0, gamma=0.0, eta=0.0)

        response = self.client.post('/startjob', {
            'id': guid,
            'state': 'EX',
            'alpha': 0,
            'beta': 0,
            'gamma': 0,
            'eta': 0
        }, format='json')
        self.assertEqual(response.status_code, 400)

    def test_post_startMap_illegal_state_job(self):
        from api.models import Job
        guid = '0'

        response = self.client.post('/startjob', {
            'id': guid,
            'state': 'NOT A STATE :o',
            'alpha': 0,
            'beta': 0,
            'gamma': 0,
            'eta': 0
        }, format='json')
        self.assertEqual(response.status_code, 400)
    
    def test_post_startMap_illegal_weight(self):
        from api.models import Job
        guid = '0'

        response = self.client.post('/startjob', {
            'id': guid,
            'state': 'NOT A STATE :o',
            'alpha': 'a',
            'beta': 0,
            'gamma': 0,
            'eta': 0
        }, format='json')
        self.assertEqual(response.status_code, 400)

    def test_post_startMap_fires_signal(self):
        guid = '0'
        response = self.client.post('/startjob', {
            'id': guid,
            'state': 'EX',
            'alpha': 0,
            'beta': 0,
            'gamma': 0,
            'eta': 0
        }, format='json')
        self.assertEqual(response.status_code, 201)

    def test_post_startMap_worker_role(self):
        from api.models import Job, GeneratedMap, State
        from api.tasks import performMetropolisHastingsWalk
        guid = '0'
        job = Job.objects.create(jobId=guid, state=self.ex_state, steps=1, alpha=0.0, beta=0.0, gamma=0.0, eta=0.0)
        performMetropolisHastingsWalk(guid)
        self.assertEqual(len(GeneratedMap.objects.all()), 1)

    def test_post_startMap_worker_role(self):
        from api.models import Job, GeneratedMap, State
        from api.tasks import performMetropolisHastingsWalk
        guid = '0'
        job = Job.objects.create(jobId=guid, state=self.ex_state, steps=1, alpha=0.0, beta=0.0, gamma=0.0, eta=0.0)
        performMetropolisHastingsWalk(guid)
        self.assertEqual(len(GeneratedMap.objects.all()), 1)