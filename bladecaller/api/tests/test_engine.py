from django.test import TransactionTestCase
from django.test.utils import override_settings
from rest_framework.test import APITestCase

import time

# Create your tests here.

class EngineTests(APITestCase):
    fixtures = ['iowa_fixture.json']

    def setUp(self):
        from api.models import State
        self.ex_state = State.objects.get(fips=19)

    def tearDown(self):
        from api.models import GeneratedMap
        for map in GeneratedMap.objects.all():
            map.delete()

    def test_get_startMap(self):
        response = self.client.get('/create-job/', format='json')
        self.assertEqual(response.status_code, 405)


    def test_post_startMap_missing_id(self):
        response = self.client.post('/create-job/', {}, format='json')
        self.assertEqual(response.status_code, 400)
        self.assertTrue('msg' in response.data.keys())
        self.assertTrue('missing_field' in response.data.keys())
        self.assertTrue('missing' in response.data['msg'].lower())
        self.assertEqual(response.data['missing_field'], 'id')

    def test_post_startMap_missing_state(self):
        guid = '0'
        response = self.client.post('/create-job/', {'id': guid}, format='json')
        self.assertEqual(response.status_code, 400)
        self.assertTrue('msg' in response.data.keys())
        self.assertTrue('missing_field' in response.data.keys())
        self.assertTrue('missing' in response.data['msg'].lower())
        self.assertEqual(response.data['missing_field'], 'state')

    def test_post_startMap_missing_weights(self):
        guid = '0'
        response = self.client.post('/create-job/', {'id': guid, 'state': 'EX'}, format='json')
        self.assertEqual(response.status_code, 400)
        self.assertTrue('msg' in response.data.keys())
        self.assertTrue('missing_field' in response.data.keys())
        self.assertTrue('missing' in response.data['msg'].lower())
        self.assertEqual(response.data['missing_field'], 'alpha')

    @override_settings(CELERY_TASK_ALWAYS_EAGER=True)
    def test_post_startMap_duplicate_job(self):
        from api.models import Job
        guid = '0'
        job = Job.objects.create(jobId=guid, state=self.ex_state, steps=1, alpha=0.0, beta=0.0, gamma=0.0, eta=0.0)

        response = self.client.post('/create-job/', {
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

        response = self.client.post('/create-job/', {
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

        response = self.client.post('/create-job/', {
            'id': guid,
            'state': 'EX',
            'alpha': 'a',
            'beta': 0,
            'gamma': 0,
            'eta': 0
        }, format='json')
        self.assertEqual(response.status_code, 400)

    @override_settings(CELERY_TASK_ALWAYS_EAGER=True)
    def test_post_startMap_worker_role(self):
        from api.models import Job, GeneratedMap, State
        from api.tasks import performMetropolisHastingsWalk
        guid = '0'
        job = Job.objects.create(jobId=guid, state=self.ex_state, steps=1, alpha=0.0, beta=0.0, gamma=0.0, eta=0.0, finished=True)
        performMetropolisHastingsWalk(job.id)
        self.assertEqual(len(Job.objects.all()), 1)
        self.assertEqual(Job.objects.all()[0].jobId, guid)
        self.assertEqual(Job.objects.all()[0].state.state, 'IA')
        self.assertEqual(Job.objects.all()[0].alpha, 0.)
        self.assertEqual(Job.objects.all()[0].beta, 0.)
        self.assertEqual(Job.objects.all()[0].gamma, 0.)
        self.assertEqual(Job.objects.all()[0].eta, 0.)
        self.assertTrue(Job.objects.all()[0].finished)
