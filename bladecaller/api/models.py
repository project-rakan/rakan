from django.db import models
from django.contrib.postgres.fields import ArrayField
from django.db.models.signals import post_save
from django.dispatch import receiver

import os
import io
import pickle
from bladecaller.settings import MAP_ROOT

# Create your models here.

class State(models.Model):
    state = models.CharField(max_length=2, unique=True)
    maxDistricts = models.IntegerField()
    fips = models.IntegerField(unique=True)
    precincts = models.IntegerField()

    granularity = models.CharField(max_length=7, choices=(
        ('vtd', 'vtd'),
        ('county', 'county'),
    ), default='vtd')

    def __str__(self):
        return self.state

    @property
    def stateJsonLocation(self):
        return os.path.join(MAP_ROOT, f"{self.state}.json")

    def stateEngineData(self):
        path = os.path.join(MAP_ROOT, f'{self.state}.pkl')
        if not os.path.isfile(path):
            precinct_id_map = {vtd: i for i, vtd in enumerate(self.vtds.all())}
            precincts = []
            edges = {()}

            for vtd, i in precinct_id_map.items():
                precincts.append({
                    "nodeID": i,
                    "curDistrict": vtd.district.district_id,
                    "county": vtd.county,
                    "area": vtd.land,
                    "minPopulation": vtd.minorityPop,
                    "majPopulation": vtd.majorityPop
                })

                for neighbor in vtd.connected:
                    edges.add(tuple(sorted([precinct_id_map[neighbor], i])))

            payload = {
                'stCode': self.state,
                'numPrecincts': self.precincts,
                'numDistricts': self.maxDistricts,
                'precincts': precincts,
                'edges': edges
            }

            with io.open(path, 'wb') as handle:
                handle.write(pickle.dumps(payload))
        
        return io.open(path, 'rb')
        

class GeneratedMap(models.Model):
    added = models.DateTimeField(auto_now_add=True)
    state = models.ForeignKey(State, on_delete=models.CASCADE)

    mapContents = ArrayField(
        models.IntegerField(),
        unique=True
    )

    compactness = models.FloatField()
    distribution = models.FloatField()
    borderRespect = models.FloatField()
    vra = models.FloatField()


class Job(models.Model):
    added = models.DateTimeField(auto_now_add=True)
    jobId = models.CharField(max_length=256, unique=True)
    state = models.ForeignKey(State, on_delete=models.CASCADE)

    finished = models.BooleanField(default=False)

    generatedMaps = models.ManyToManyField(GeneratedMap, blank=True)

    steps = models.IntegerField(default=200)

    alpha = models.FloatField()
    beta = models.FloatField()
    gamma = models.FloatField()
    eta = models.FloatField()

    def __str__(self):
        return self.jobId


@receiver(post_save, sender=Job)
def queue_new_job(sender, **kwargs):
    from api import tasks # imported here to prevent circular imports

    job = kwargs.get('instance')
    # start an engine in a different process

    if kwargs['created'] and not job.finished:
        tasks.performMetropolisHastingsWalk.delay(job.id)
        
