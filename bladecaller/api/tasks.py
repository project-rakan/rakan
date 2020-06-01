from celery import shared_task
from api.models import Job, GeneratedMap
from rakan import Engine

from django.db import IntegrityError

import os
import matplotlib.pyplot as plt
from bladecaller.settings import RAKAN_STATE_VISUALIZATIONS

import hashlib

@shared_task
def performMetropolisHastingsWalk(jobId: int):
    job = Job.objects.get(id=jobId)
    engine = Engine(job.state.stateEngineData)
    engine.seed()

    maps = engine.getMaps()
    scores = engine.getScores()

    # Generate the seed
    generatedMap = maps[0]
    mapHash = hashlib.md5(" ".join([str(_) for _ in generatedMap]).encode()).hexdigest()
    try:
        generatedMap = maps[0]
        mapModel = GeneratedMap.objects.create(
            state=job.state,
            mapContents=generatedMap,
            md5Hash=mapHash,
            compactness=scores[0][b'compact'],
            vra=scores[0][b'vra'],
            distribution=scores[0][b'distribution'],
            borderRespect=scores[0][b'border'],
        )
    except IntegrityError as e:
        mapModel = GeneratedMap.objects.get(
            md5Hash=mapHash
        )
    
    job.generatedMaps.add(mapModel)

    engine.walk(job.steps, job.alpha, job.beta, job.gamma, job.eta)

    maps = engine.getMaps()[1:]
    scores = engine.getScores()[1:]

    for map_, score_ in zip(maps, scores):
        mapHash = hashlib.md5(" ".join([str(_) for _ in map_]).encode()).hexdigest()
        try:
            mapModel = GeneratedMap.objects.create(
                state=job.state,
                mapContents=map_,
                md5Hash=mapHash,
                compactness=score_[b'compact'],
                vra=score_[b'vra'],
                distribution=score_[b'distribution'],
                borderRespect=score_[b'border'],
            )
        except IntegrityError:
            mapModel = GeneratedMap.objects.get(
                md5Hash=mapHash,
            )
        job.generatedMaps.add(mapModel)

    job.finished = True
    job.save()

@shared_task
def visualizeMap(generatedMapId: int):
    generatedMap = GeneratedMap.objects.get(id=generatedMapId)
    df = generatedMap.df
    df['coords'] = df['geometry'].apply(lambda x: x.representative_point().coords[:])
    df['coords'] = [coords[0] for coords in df['coords']]
    df.boundary.plot(
        label=True,
        figsize=(15,10),
    )
    df.plot(column='district')
    filepath = os.path.join(RAKAN_STATE_VISUALIZATIONS, f"{generatedMap.id}.png")
    for idx, row in df.iterrows():
        plt.annotate(s=idx, xy=row['coords'], horizontalalignment='center')
    plt.savefig(filepath)
    plt.close()
    generatedMap.visualization = f"/images/{generatedMap.id}.png"
    generatedMap.save()
