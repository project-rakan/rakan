from rest_framework.response import Response
from rest_framework.decorators import api_view

from uuid import uuid4

# Create your views here.

# API to handle:

"""
Bladecaller
# CreateGuid Request/Response
# GetDistricting Request/Response
# GetStateInfo Request/Response

Rakan
# ScoreMap (Request/Response)
# StartMapJob (Request/Update)
"""

@api_view(['POST'])
def createGuid(request):
    if request.method == 'POST':
        state = request.data.get('state', 'XX').lower()
        jobType = request.data.get('jobType', 'XX').lower()
        uuid = uuid4()
        return Response({"guid": f"{state}-{jobType}-{str(uuid)}"})
    return Response({"data": None})
