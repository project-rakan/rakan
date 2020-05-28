from django.contrib.gis.geos import GEOSGeometry
from django.core.management.base import BaseCommand
from datamerger.models import VTDBlock, DistrictBlock
from api.models import State

from bladecaller.settings import DEBUG

from progress.bar import IncrementalBar

import pandas as pd
import geopandas as gpd

import os
import io

CONGRESS_TARGET = "https://www2.census.gov/geo/tiger/GENZ2018/shp/cb_2018_us_cd116_20m.zip"
CENSUS_TARGET = "http://censusdata.ire.org/{fips}/all_140_in_{fips}.P3.csv"
VTD_TARGET = "https://www2.census.gov/geo/tiger/TIGER2012/VTD/tl_2012_{fips}_vtd10.zip"
TRACT_TARGET = "https://www2.census.gov/geo/tiger/GENZ2018/shp/cb_2018_{fips}_tract_500k.zip"

DATA_LOCATION = os.path.join(os.getenv('RAKAN_LOCATION'), 'data')
STATE_TABLE = os.path.join(os.getenv('RAKAN_LOCATION'), 'configs', 'stateKeys.csv')

CONGRESS_ZIP_NAME = os.path.join(DATA_LOCATION, "116_congress.zip")
CONGRESS_DIR_NAME = os.path.join(DATA_LOCATION, "116_congress")

STATE_DIR_NAME = os.path.join(DATA_LOCATION, "{state_name}")
STATE_CENSUS_FILE_NAME = os.path.join(STATE_DIR_NAME, "census.csv")

STATE_VTD_DIR_NAME = os.path.join(STATE_DIR_NAME, "vtd")
STATE_VTD_ZIP_NAME = os.path.join(STATE_DIR_NAME, "vtd.zip")

STATE_TRACT_DIR_NAME = os.path.join(STATE_DIR_NAME, "tract")
STATE_TRACT_ZIP_NAME = os.path.join(STATE_DIR_NAME, "tract.zip")

HIDE_OUTPUT = ""

class Command(BaseCommand):
    help = "Download all the VTD into the database"

    def add_arguments(self, parser):
        parser.add_argument('ignore-cache', type=bool, default=True)

    def handle(self, *args, **options):
        ignore_cache = options.get('ignore-cache', True)

        self.download116Congress(ignore_cache)
        self.downloadStates(ignore_cache)


    def download116Congress(self, ignore_cache: bool):

        if ignore_cache or not os.path.isfile(CONGRESS_ZIP_NAME):
            os.system(f"wget {CONGRESS_TARGET} -O {CONGRESS_ZIP_NAME} {HIDE_OUTPUT}")

        if ignore_cache or not os.path.isdir(CONGRESS_DIR_NAME):
            os.system(f"unzip {CONGRESS_ZIP_NAME} -d {CONGRESS_DIR_NAME} {HIDE_OUTPUT}")   

    def downloadStates(self, ignore_cache: bool):
        with io.open(STATE_TABLE) as handle:
            lines = handle.readlines()

        bar = IncrementalBar('Downloading states', max=len(lines) * 7) # .next() called 7 times per line

        for line in lines:
            line = line.strip()
            print(line)
            fips, state_code, state_name, geoid, state_ns = line.split(',')
            bar.next()
            self.downloadState(fips, state_code, state_name, state_ns, ignore_cache, bar) # 6 bar.next() calls
            self.mergeDatasets(fips, state_code, state_name, state_ns, ignore_cache, bar)

        bar.finish()

    def downloadState(self, fips, state_code, state_name, districts, ignore_cache: bool, bar):
        # Configure variables
        state_name = state_name.lower().replace(' ', '_')
        state_dir_name = STATE_DIR_NAME.format(state_name=state_name)
        
        state_census_file_name = STATE_CENSUS_FILE_NAME.format(state_name=state_name)

        state_vtd_dir_name = STATE_VTD_DIR_NAME.format(state_name=state_name)
        state_vtd_zip_name = STATE_VTD_ZIP_NAME.format(state_name=state_name)

        state_tract_dir_name = STATE_TRACT_DIR_NAME.format(state_name=state_name)
        state_tract_zip_name = STATE_TRACT_ZIP_NAME.format(state_name=state_name)

        # Download everything

        if not os.path.isdir(state_dir_name):
            os.mkdir(state_dir_name)

        bar.next()

        # Download Census files
        if ignore_cache or not os.path.isfile(state_census_file_name):
            os.system(f"wget {CENSUS_TARGET.format(fips=fips)} -O - | gunzip > {state_census_file_name} {HIDE_OUTPUT}")

        bar.next()

        # Download VTD Shape files
        if ignore_cache or not os.path.isfile(state_vtd_zip_name):
            os.system(f"wget -O {state_vtd_zip_name} {VTD_TARGET.format(fips=fips)} {HIDE_OUTPUT}")
        
        bar.next()
        
        if ignore_cache or not os.path.isdir(state_vtd_dir_name):
            if not os.path.isdir(state_vtd_dir_name):
                os.mkdir(state_vtd_dir_name)
            os.system(f"unzip {state_vtd_zip_name} -d {state_vtd_dir_name} {HIDE_OUTPUT}")

        bar.next()

        # Download Census Tract Shape files
        if ignore_cache or not os.path.isfile(state_tract_zip_name):
            os.system(f"wget -O {state_tract_zip_name} {TRACT_TARGET.format(fips=fips)} {HIDE_OUTPUT}")
        
        bar.next()
        
        if ignore_cache or not os.path.isdir(state_tract_dir_name):
            if not os.path.isdir(state_tract_dir_name):
                os.mkdir(state_tract_dir_name)
            os.system(f"unzip {state_tract_zip_name} -d {state_tract_dir_name} {HIDE_OUTPUT}")
        
        bar.next()

        # TODO: Downlaod voting data

    def mergeDatasets(self, fips, state_code, state_name, districts, ignore_cache: bool, bar):
        # Configure variables
        state_name = state_name.lower().replace(' ', '_')
        state_census_file_name = STATE_CENSUS_FILE_NAME.format(state_name=state_name)
        state_vtd_dir_name = STATE_VTD_DIR_NAME.format(state_name=state_name)
        state_tract_dir_name = STATE_TRACT_DIR_NAME.format(state_name=state_name)

        pass

def downloadStates():
    with io.open(STATE_TABLE) as handle:
        lines = handle.readlines()

    for line in lines:
        line = line.strip()
        fips, state_code, state_name, state_ns = line.split(',')

        if not os.path.isdir(state_name):
            print(f"Creating Directory for {state_name}")
            state_name = state_name.lower().replace(' ', '_')
            os.mkdir(f"{state_name}")
        else:
            print(f"Directory already exists for {state_name}")

        if not os.path.isfile(f"{state_name}/{state_name}.csv"):
            print(f"Downloading 2010 {state_name.title()} Census Data")
            os.system(f"wget -O - http://censusdata.ire.org/{fips}/all_140_in_{fips}.P3.csv | gunzip > {state_name}/{state_name}.csv")


        if not os.path.isdir(f"{state_name}/vtd/"):
            os.mkdir(f"{state_name}/vtd")
            print(f"Downloading 2010 {state_name.title()} Census VTD shapefiles")
            os.system(f"wget -O {state_name}/temp.zip https://www2.census.gov/geo/tiger/TIGER2012/VTD/tl_2012_{fips}_vtd10.zip")
            os.system(f"unzip {state_name}/temp.zip -d {state_name}/vtd/")
            os.system(f"rm {state_name}/temp.zip")

        if not os.path.isdir(f"{state_name}/tracts/"):
            os.mkdir(f"{state_name}/tracts")
            print(f"Downloading 2018 {state_name.title()} Census Tract shapefiles")
            os.system(f"wget -O {state_name}/temp.zip https://www2.census.gov/geo/tiger/GENZ2018/shp/cb_2018_{fips}_tract_500k.zip")
            os.system(f"unzip {state_name}/temp.zip -d {state_name}/tracts/")
            os.system(f"rm {state_name}/temp.zip")

        if not os.path.isdir(f"{state_name}/votes"):
            os.mkdir(f"{state_name}/votes")
