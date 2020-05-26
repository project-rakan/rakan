from django.db import models
from django.contrib.postgres.fields import ArrayField

# Create your models here.

class State(models.Model):
    state = models.CharField(max_length=2, unique=True)
    maxDistricts = models.IntegerField()
    fips = models.IntegerField(unique=True)
    precincts = models.IntegerField()

    def __str__(self):
        return self.state

class GeneratedMap(models.Model):
    state = models.ForeignKey(State, on_delete=models.CASCADE)

    mapContents = ArrayField(
        models.IntegerField(),
    )

    compactness = models.FloatField()
    distribution = models.FloatField()
    borderRespect = models.FloatField()
    vra = models.FloatField()


class Job(models.Model):
    jobId = models.CharField(max_length=256, unique=True)

    status = models.CharField(max_length=128, choices=(
        ('queued', 'queued'),
        ('running', 'running'),
        ('finished', 'finished')
    ))

    steps = models.IntegerField(default=200)

    alpha = models.FloatField()
    beta = models.FloatField()
    gamma = models.FloatField()
    eta = models.FloatField()

    def __str__(self):
        return self.jobId
