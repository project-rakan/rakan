from django.db import models
from django.contrib.gis.db import models as gis_models
from django.db.models import Q

from api.models import State



class VTDBlock(models.Model):
    state = models.ForeignKey(State, on_delete=models.CASCADE, related_name='vtds')
    geoid = models.CharField(max_length=255, unique=True)
    geometry = gis_models.GeometryField()

    engine_id = models.IntegerField(default=-1)

    county = models.IntegerField()

    name = models.CharField(max_length=256)

    land = models.FloatField()
    water = models.FloatField()

    minorityPop = models.IntegerField(default=0)
    majorityPop = models.IntegerField(default=0)

    district = models.ForeignKey('DistrictBlock', on_delete=models.SET_NULL, null=True)

    @property
    def overlapping_tracts(self):
        return TractBlock.objects.filter(geometry__bboverlaps=self.geometry)

    @property
    def connected(self):
        return VTDBlock.objects.filter(geometry__bboverlaps=self.geometry).filter(~Q(id = self.id))

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

    @property
    def connected(self):
        return TractBlock.objects.filter(geometry__bboverlaps=self.geometry).filter(~Q(id = self.id))

    def __str__(self):
        return self.name


class DistrictBlock(models.Model):
    state = models.ForeignKey(State, on_delete=models.CASCADE)
    district_id = models.IntegerField()
    geometry = gis_models.GeometryField()

    @property
    def overlapping_vtds(self):
        return VTDBlock.objects.filter(state=self.state, geometry__bboverlaps=self.geometry)

    @property
    def connected(self):
        return DistrictBlock.objects.filter(geometry__bboverlaps=self.geometry).filter(~Q(id = self.id))

    def __str__(self):
        return f"State {self.state.state} District #{self.district_id}"
