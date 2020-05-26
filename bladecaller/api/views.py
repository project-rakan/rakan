from rest_framework.response import Response
from rest_framework import status
from rest_framework.decorators import api_view

from api import models

from uuid import uuid4

# Create your views here.

# API to handle:

"""
Bladecaller
# GetDistricting Request/Response

Rakan
# ScoreMap (Request/Response)
# StartMapJob (Request/Update)
"""

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

    # Check mapID is there
    if len(models.GeneratedMap.objects.filter(id=mapId, state=state)) == 0:
        return Response({'msg': 'Unable to find mapId', mapId: mapId, state: state}, status=status.HTTP_400_BAD_REQUEST)

    # Return the map
    return Response({
        'state': request.data['state'],
        'map': [[precinct_id, district_id] for precinct_id, district_id in enumerate(models.GeneratedMap.objects.get(id=mapId, state=state).mapContents)]
    })
    