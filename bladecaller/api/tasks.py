from celery import shared_task
from api.models import Job, GeneratedMap
from rakan import Engine

@shared_task
def performMetropolisHastingsWalk(jobId: int):
    job = Job.objects.get(id=jobId)
    engine = Engine(job.state.stateEngineData)
    engine.walk(job.steps, job.alpha, job.beta, job.gamma, job.eta)

    maps = engine.getMaps()
    scores = engine.getScores()

    # Create geopandas of the state

    for map_, score_ in zip(maps, scores):
        # Visualize the redistricting
        mapModel = GeneratedMap.objects.create(
            state=job.state,
            mapContents=map_,
            compactness=score_[b'compact'],
            vra=score_[b'vra'],
            distribution=score_[b'distribution'],
            borderRespect=score_[b'border'],
        )
        job.generatedMaps.add(mapModel)

    job.finished = True
    job.save()