from django.db import models
from django.contrib.postgres.fields import ArrayField
from django.db.models.signals import post_save
from django.dispatch import receiver

import os
import io
import pickle

import geopandas as gpd
from shapely import wkt

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

    @property
    def stateEngineData(self):
        return os.path.join(MAP_ROOT, f'{self.state}.pkl')

    @property
    def df(self):
        table = {'geoid': [], 'geometry': [], 'district': [], 'engine_id': []}
        for vtd in self.vtds.all():
            table['geometry'].append(wkt.loads(vtd.geometry.wkt))
            table['geoid'].append(vtd.geoid)
            table['district'].append(vtd.district.district_id)
            table['engine_id'].append(vtd.engine_id)

        return gpd.GeoDataFrame(table).set_index('engine_id', drop=True)
        

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

    visualization = models.ImageField()

    @property
    def df(self):
        df = self.state.df
        df['district'] = self.mapContents
        return df

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

@receiver(post_save, sender=GeneratedMap)
def create_visualization(sender, **kwargs):
    from api import tasks # to prevent circular imports
    generatedMap = kwargs.get('instance')

    if kwargs['created']:
        tasks.visualizeMap(generatedMap.id)

@receiver(post_save, sender=Job)
def queue_new_job(sender, **kwargs):
    from api import tasks # imported here to prevent circular imports

    job = kwargs.get('instance')
    # start an engine in a different process

    if kwargs['created'] and not job.finished:
        tasks.performMetropolisHastingsWalk.delay(job.id)
        
