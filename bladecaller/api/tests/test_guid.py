from django.test import TestCase
from rest_framework.test import APITestCase

class GuidTests(APITestCase):
    
    def test_get_guid(self):
        response = self.client.get('/createGuid', format='json')
        self.assertEqual(response.status_code, 405)


    def test_post_guid(self):
        response = self.client.post('/createGuid', {}, format='json')
        self.assertEqual(response.status_code, 200)
        self.assertTrue('guid' in response.data.keys())
        self.assertTrue(response.data['guid'].startswith('xx-xx-'))

    def test_post_guid_with_state(self):
        response = self.client.post('/createGuid', {'state': 'wa'}, format='json')
        self.assertEqual(response.status_code, 200)
        self.assertTrue('guid' in response.data.keys())
        self.assertTrue(response.data['guid'].startswith('wa-xx-'))

    def test_post_guid_with_state_and_job(self):
        response = self.client.post('/createGuid', {'state': 'wa', 'jobType': 'load'}, format='json')
        self.assertEqual(response.status_code, 200)
        self.assertTrue('guid' in response.data.keys())
        self.assertTrue(response.data['guid'].startswith('wa-load-'))

    def test_guid_not_same(self):
        N = 100
        guids = set()
        for i in range(N):
            response = self.client.post('/createGuid', {}, format='json')
            guids.add(response.data['guid'])
        
        self.assertEqual(len(guids), N)

