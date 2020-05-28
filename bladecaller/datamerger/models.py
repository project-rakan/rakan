from django.db import models
from django.contrib.gis.db import models as gis_models

from api.models import State

# Create your models here.

class VTDBlock(models.Model):
    state = models.ForeignKey(State, on_delete=models.CASCADE)
    geoid = models.CharField(max_length=255, unique=True)
    geometry = gis_models.GeometryField()

    county = models.IntegerField()

    name = models.CharField(max_length=256)

    land = models.FloatField()
    water = models.FloatField()

    def __str__(self):
        return self.name

class TractBlock(models.Model):
    state = models.ForeignKey(State, on_delete=models.CASCADE)
    geometry = gis_models.GeometryField()

    geoid = models.CharField(max_length=256, unique=True)
    name = models.CharField(max_length=256)

    land = models.FloatField()
    water = models.FloatField()

    totalPop = models.IntegerField(default=0)
    whitePop = models.IntegerField(default=0)
    blackPop = models.IntegerField(default=0)
    nativeAPop = models.IntegerField(default=0)
    asianPop = models.IntegerField(default=0)
    pacisPop = models.IntegerField(default=0)
    otherPop = models.IntegerField(default=0)
    multiPop = models.IntegerField(default=0)

    def __str__(self):
        return self.name

class DistrictBlock(models.Model):
    state = models.ForeignKey(State, on_delete=models.CASCADE)
    district_id = models.IntegerField(unique=True)
    geometry = gis_models.GeometryField()