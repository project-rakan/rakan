from django.contrib.gis.geos import GEOSGeometry
from django.core.management.base import BaseCommand
from datamerger.models import VTDBlock, DistrictBlock, TractBlock
from api.models import State
from shapely.geometry import mapping

from bladecaller.settings import DEBUG

from progress.bar import IncrementalBar

import pandas as pd
import geopandas as gpd

import os
import io
import json


env = os.getenv("RAKAN_STATEFILES")
OUTPUT_JSON_LOCATION = env + "/{code}"


class Command(BaseCommand):
    help = "Download all the VTD into the database"

    def add_arguments(self, parser):
        parser.add_argument('--ignore-cache', type=bool, default=False)

    def handle(self, *args, **options):
        # raise NotImplementedError("Abstract!")
        
        djangoDF = pd.DataFrame(list(VTDBlock.objects.all().values()))
        
        for state in State.objects.all():
            self.toJSON(djangoDF[djangoDF['state_id'] == state.id], state)

    def toJSON(self, df1, state):

        df = df1.reset_index()
        # Convert each precinct's POLYGON into a list of (x,y) coordinates
        stCode = state.state
        maxDistricts = state.maxDistricts
        fips = state.fips



        coordLists = self.getPolyCoords(df.geometry)

        
        import pdb; pdb.set_trace()
        precincts = []
        for index, prec in df.iterrows():
            precName = prec['name']
            precID = index

            vertices = []
            for v in coordLists[index]:
                coord = {
                    "lat": float(v[1]),
                    "lng": float(v[0])
                }
                vertices.append(coord)

            precinctEntry = {
                "name": precName,
                "id": precID,
                "vertices": vertices,
            }
            precincts.append(precinctEntry)

        dictionary = {
            "state": stCode,
            "maxDistricts": maxDistricts,
            "fips": fips,
            "precincts": precincts
        }

        json_loc = OUTPUT_JSON_LOCATION.format(code=stCode)+'.json'

        with open(json_loc, "w") as outfile:
            return outfile.write(json.dumps(dictionary, indent = 4))

        toJSONDict(df, stCode)


    def getPolyCoords(self, geo):
        "Returns a tuple of x,y coords from a POLYGON in the form ((x1,y1),...,(xn,yn))"
        coordsList = []
        
        for i in range(len(geo)):
            coordsList.append(geo[i].coords[0])
        return coordsList

    def toJSONDict(df, stCode):
        mapping = []
        for index, prec in df.iterrows():
            mapping.append([int(index), prec['district']])
        output = {
            "state": stCode,
            "map": mapping
        }
        districtsLoc = OUTPUT_JSON_LOCATION.format(code=stCode)+'.districts.json'
        with open(districtsLoc, "w") as outfile:
            return outfile.write(json.dumps(output, indent = 4))