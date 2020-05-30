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

from progress.bar import IncrementalBar

env = os.getenv("RAKAN_STATEFILES")
OUTPUT_JSON_LOCATION = env + "/{code}"


class Command(BaseCommand):
    help = "Download all the VTD into the database"

    def add_arguments(self, parser):
        parser.add_argument('--ignore-cache', type=bool, default=False)

    def handle(self, *args, **options):
        bar = IncrementalBar("Outputting JSON for frontend API", max=State.objects.all().count() * 2)
        
        for state in State.objects.all():
            self.outputJSON(state)
            bar.next()
            self.toJSONDict(state)
            bar.next()

        bar.finish()

    def outputJSON(self, state):
        # Convert each precinct's POLYGON into a list of (x,y) coordinates
        stCode = state.state
        vtds = state.vtds.all().order_by('engine_id')
        maxDistricts = state.maxDistricts
        fips = state.fips

        coordLists = self.getPolyCoords([_.geometry for _ in vtds])

        precincts = []

        for prec in vtds:
            precName = prec.name
            vertices = [{"lat": lat, "lng": lng} for lng, lat in coordLists[prec.engine_id]]

            precinctEntry = {
                "name": precName,
                "id": prec.engine_id,
                "vertices": vertices,
            }

            precincts.append(precinctEntry)

        dictionary = {
            "state": stCode,
            "maxDistricts": maxDistricts,
            "fips": fips,
            "precincts": precincts
        }

        with io.open(f'{OUTPUT_JSON_LOCATION.format(code=stCode)}.json', "w") as outfile:
            outfile.write(json.dumps(dictionary))


    def getPolyCoords(self, geo):
        "Returns a tuple of x,y coords from a POLYGON in the form ((x1,y1),...,(xn,yn))"
        coordsList = []
        
        for i in range(len(geo)):
            coordsList.append(geo[i].coords[0])
        return coordsList

    def toJSONDict(self, state):
        mapping = []

        for prec in state.vtds.all().order_by('engine_id'):
            mapping.append([prec.engine_id, prec.district.district_id])

        output = {
            "state": state.state,
            "map": mapping
        }

        with open(f'{OUTPUT_JSON_LOCATION.format(code=state.state)}.districts.json', "w") as outfile:
            outfile.write(json.dumps(output))