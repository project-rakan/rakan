from django.db import models
from django.contrib.postgres.fields import ArrayField

# Create your models here.

class GeneratedMap(models.Model):
    state = models.CharField(max_length=2)

    mapContents = ArrayField(
        models.IntegerField(),
    )

    compactness = models.FloatField()
    distribution = models.FloatField()
    borderRespect = models.FloatField()
    vra = models.FloatField()


class Job(models.Model):
    type = models.CharField(max_length=256, choices=(
        ('score', 'score'),
        ('walk', 'walk'),
    ))

    maxSteps = models.IntegerField(default=200)
    jobId = models.CharField(max_length=256, unique=True)

    alpha = models.FloatField()
    beta = models.FloatField()
    gamma = models.FloatField()
    eta = models.FloatField()
