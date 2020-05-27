from rest_framework.response import Response
from rest_framework import status
from rest_framework.decorators import api_view

from bladecaller.settings import MIN_MAPS_FOR_PROBABILITY, DEFAULT_WALK_SIZE
from rakan import Engine

from api import models
from uuid import uuid4
from math import isnan

@api_view(['POST'])
def createGuid(request):
    state = request.data.get('state', 'XX').lower()
    jobType = request.data.get('jobType', 'XX').lower()
    uuid = uuid4()
    return Response({"guid": f"{state}-{jobType}-{str(uuid)}"})


@api_view(['POST'])
def getDistricting(request):
    # Check fields are there
    for field in ['state', 'mapId']:
        if field not in request.data:
            return Response({'msg': 'Missing a field', 'missing_field': field}, status=status.HTTP_400_BAD_REQUEST)

    mapId = request.data['mapId']
    state = request.data['state']

    # Check state is there
    if len(models.State.objects.filter(state=state)) == 0:
        return Response({'msg': 'Unable to find state', 'mapId': mapId, 'state': state}, status=status.HTTP_400_BAD_REQUEST)

    stateModel = models.State.objects.get(state=state)

    # Check mapID is there
    if len(models.GeneratedMap.objects.filter(id=mapId, state=stateModel)) == 0:
        return Response({'msg': 'Unable to find mapId', 'mapId': mapId, 'state': state}, status=status.HTTP_400_BAD_REQUEST)

    mapFound = models.GeneratedMap.objects.get(id=mapId, state=stateModel).mapContents

    # Return the map
    return Response({
        'state': request.data['state'],
        'map': [[precinct_id, district_id] for precinct_id, district_id in enumerate(mapFound)]
    })


@api_view(['POST'])
def scoreMap(request):
    # Check fields are there
    for field in ['id', 'state', 'map', 'alpha', 'beta', 'gamma', 'eta']:
        if field not in request.data:
            return Response({'msg': 'Missing a field', 'missing_field': field}, status=status.HTTP_400_BAD_REQUEST)

    jobId = request.data['id']
    state = request.data['state']
    districting = request.data['map']
    alpha = request.data['alpha']
    beta = request.data['beta']
    gamma = request.data['gamma']
    eta = request.data['eta']

    # Check state is there
    if len(models.State.objects.filter(state=state)) == 0:
        return Response({'msg': 'Unable to find state', 'id': jobId, 'state': state}, status=status.HTTP_400_BAD_REQUEST)

    stateModel = models.State.objects.get(state=state)

    # Verify districting is valid
    districting = [_[1] for _ in sorted(districting, key=lambda cell: cell[0])]
    if len(districting) != stateModel.precincts:
        return Response({'msg': 'Incorrect number of precincts', 'id': jobId, 'recieved': len(districting), 'expected': stateModel.precincts}, status=status.HTTP_400_BAD_REQUEST)
    elif max(districting) < stateModel.maxDistricts and min(districting) >= 0:
        return Response({
            'msg': 'Incorrect number of districts', 
            'id': jobId, 
            'recieved': {
                'min':  min(districting),
                'max': max(districting),
            }, 
            'expected': {
                'min': 0,
                'max': max(districting)
            }
        }, status=status.HTTP_400_BAD_REQUEST)

    # Assert the weights are floats
    for name, weight in [('alpha', alpha), ('beta', beta), ('gamma', gamma), ('eta', eta)]:
        if not isinstance(weight, (float, int)):
            return Response({'msg': f'Invalid weight: {name}', 'id': jobId, 'recieved': str(type(weight)), 'expected': 'float or int'}, status=status.HTTP_400_BAD_REQUEST)

    # Connect to single threaded engine (blocking & ids don't matter)

    engine = engine(stateModel.stateJsonLocation)
    engine.setDistricts(districting)
    scores = engine.getScores()

    # Calculate the weighted score using the weights + scores
    weightedScore = (
        scores['compactness'] * alpha +
        scores['population'] * beta +
        scores['political'] * gamma +
        scores['vra'] * eta
    )

    # Calculate the probability iff there exists enough entries in the db
    mapCount = len(models.GeneratedMap.objects.all())
    if mapCount >= MIN_MAPS_FOR_PROBABILITY:
        # Handle the random walk case:
        denom = sum([abs(alpha), abs(beta), abs(gamma), abs(eta)])
        if sum([abs(alpha), abs(beta), abs(gamma), abs(eta)]) == 0:
            denom = 1.
        elif sum([alpha, beta, gamma, eta]) == 0:
            denom = alpha = beta = gamma = eta = 1.

        probability = (
            models.GeneratedMap.objects.filter(compactness__lte=scores['compactness']) * alpha + 
            models.GeneratedMap.objects.filter(distribution__lte=scores['population']) * beta + 
            models.GeneratedMap.objects.filter(borderRespect__lte=scores['political']) * gamma + 
            models.GeneratedMap.objects.filter(vra__lte=scores['vra']) * eta
        ) / denom
    else:
        probability = float('nan')

    # Construct response
    response = Response({
        'id': jobId,
        'score': weightedScore,
        'probability': probability,
        'references': mapCount
    })

    # Store the user submitted one in the database
    models.GeneratedMap.objects.create(
        state=stateModel,
        mapContents=districting,
        compactness=scores['compactness'],
        distribution=scores['population'],
        borderRespect=scores['borderRespect'],
        vra=scores['vra'],
    )

    return response


@api_view(['POST'])
def startMapJob(request):
    # Check fields are there
    for field in ['id', 'state', 'alpha', 'beta', 'gamma', 'eta']:
        if field not in request.data:
            return Response({'msg': 'Missing a field', 'missing_field': field}, status=status.HTTP_400_BAD_REQUEST)

    jobId = request.data['id']
    state = request.data['state']
    steps = request.data.get('steps')

    # Check jobId is not in use
    if models.Job.objects.filter(jobId=jobId):
        return Response({'msg': 'jobId in use, please use a new one', 'id': jobId}, status=status.HTTP_400_BAD_REQUEST)

    # Check state is there
    if len(models.State.objects.filter(state=state)) == 0:
        return Response({'msg': 'Unable to find state', 'id': jobId, 'state': state}, status=status.HTTP_400_BAD_REQUEST)

    # Check if steps is there, and set a default if it's invalid
    if not isinstance(steps, int):
        steps = DEFAULT_WALK_SIZE

    stateModel = models.State.objects.get(state=state)

    alpha = request.data['alpha']
    beta = request.data['beta']
    gamma = request.data['gamma']
    eta = request.data['eta']

    # Assert the weights are floats
    for name, weight in [('alpha', alpha), ('beta', beta), ('gamma', gamma), ('eta', eta)]:
        if not isinstance(weight, (float, int)):
            return Response({'msg': f'Invalid weight: {name}', 'id': jobId, 'recieved': str(type(weight)), 'expected': 'float or int'}, status=status.HTTP_400_BAD_REQUEST)

    # Start the job
    job = models.Job.objects.create(
        jobId=jobId,
        state=stateModel,
        steps=steps,
        alpha=alpha,
        beta=beta,
        gamma=gamma,
        eta=eta,
    )

    # Return status code
    return Response({
        'id': jobId,
        'state': state,
        'steps': steps,
        'msg': 'Queued up the task',
        'alpha': alpha,
        'beta': beta,
        'gamma': gamma,
        'eta': eta,
    }, status=status.HTTP_201_CREATED)


@api_view(['POST'])
def updateMapJob(request):
    # Check fields are there
    for field in ['id', 'state']: # weights ignored because we already have them
        if field not in request.data:
            return Response({'msg': 'Missing a field', 'missing_field': field}, status=status.HTTP_400_BAD_REQUEST)

    jobId = request.data['id']
    state = request.data['state']

    # Check state is there
    if len(models.State.objects.filter(state=state)) == 0:
        return Response({'msg': 'Unable to find state', 'id': jobId, 'state': state}, status=status.HTTP_400_BAD_REQUEST)

    stateModel = models.State.objects.get(state=state)

    # Check jobID is valid
    if len(models.Job.objects.filter(id=jobId, state=stateModel)) == 0:
        return Response({'msg': 'Unable to find jobId', 'id': jobId, 'state': state}, status=status.HTTP_400_BAD_REQUEST)

    job = models.Job.objects.get(id=jobId)
    finished = job.finished

    # Check if maps are available
    if len(job.generatedMaps.all()) == 0:
        return Response({'msg': 'No maps available yet', 'id': jobId, 'state': state}, status=status.HTTP_204_NO_CONTENT)

    mapFound = job.generatedMaps.all().order_by('-added')[0]

    # Calculate our scores
    weightedScore = (
        mapFound.compactness * job.alpha +
        mapFound.distribution * job.beta +
        mapFound.borderRespect * job.gamma +
        mapFound.vra * job.eta
    )

    # Calculate the probability iff there exists enough entries in the db
    mapCount = len(models.GeneratedMap.objects.all())
    if mapCount >= MIN_MAPS_FOR_PROBABILITY:
        # Handle the random walk case:
        alpha, beta, gamma, eta = job.alpha, job.beta, job.gamma, job.eta
        denom = sum([abs(job.alpha), abs(job.beta), abs(job.gamma), abs(job.eta)])
        if sum([abs(job.alpha), abs(job.beta), abs(job.gamma), abs(job.eta)]) == 0:
            denom = 1.
        elif sum([job.alpha, job.beta, job.gamma, job.eta]) == 0:
            denom = alpha = beta = gamma = eta = 1.

        probability = (
            models.GeneratedMap.objects.filter(compactness__lte=mapFound.compactness) * alpha + 
            models.GeneratedMap.objects.filter(distribution__lte=mapFound.distribution) * beta + 
            models.GeneratedMap.objects.filter(borderRespect__lte=mapFound.borderRespect) * gamma + 
            models.GeneratedMap.objects.filter(vra__lte=mapFound.vra) * eta
        ) / denom
    else:
        probability = float('nan')

    return Response({
        'id': jobId,
        'mapId': mapFound.id,
        'state': mapFound.state.state,
        'updates': [[precinct_id, district_id] for precinct_id, district_id in enumerate(mapFound.mapContents)],
        'score': weightedScore,
        'probability': probability,
        'references': mapCount
    }, status=(status.HTTP_200_OK if finished else status.HTTP_206_PARTIAL_CONTENT))