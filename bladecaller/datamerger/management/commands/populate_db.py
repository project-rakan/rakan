from django.contrib.gis.geos import GEOSGeometry
from django.core.management.base import BaseCommand
from datamerger.models import VTDBlock, DistrictBlock, TractBlock
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
        parser.add_argument('--ignore-cache', type=bool, default=False)

    def handle(self, *args, **options):
        ignore_cache = options['ignore_cache']

        self.loadStates()                       # Create the states w/ only fips/name/max districts
        self.download116Congress(ignore_cache)  # Download the congressional districts
        self.install116Congress()               # Install the congressional districts into the db
        self.installStates(ignore_cache)        # Process all states
        self.cleanStates()                      # Break apart multipolygons/filter water precincts/adjust granularity

    def loadStates(self):
        with io.open(STATE_TABLE) as handle:
            lines = handle.readlines()

        states = []

        for line in lines:
            fips, state_code, state_name, geoid, state_ns, granularity = line.split(',')
            if not State.objects.filter(fips=fips):
                states.append(State(**{
                    'state': state_code,
                    'maxDistricts': state_ns,
                    'fips': fips,
                    'precincts': -1,
                    'granularity': granularity
                }))

        State.objects.bulk_create(states)

    def download116Congress(self, ignore_cache: bool):

        if ignore_cache or not os.path.isfile(CONGRESS_ZIP_NAME):
            os.system(f"wget {CONGRESS_TARGET} -O {CONGRESS_ZIP_NAME} {HIDE_OUTPUT}")

        if ignore_cache or not os.path.isdir(CONGRESS_DIR_NAME):
            os.system(f"unzip {CONGRESS_ZIP_NAME} -d {CONGRESS_DIR_NAME} {HIDE_OUTPUT}")

    def install116Congress(self):
        

        df = gpd.read_file(CONGRESS_DIR_NAME)
        df['GEOID'] = df['GEOID'].astype(int)
        df['STATEFP'] = df['STATEFP'].astype(int)

        districts = []

        for i, row in df.iterrows():
            if not DistrictBlock.objects.filter(district_id=row['GEOID']) and State.objects.filter(fips=row['STATEFP']):
                districts.append(DistrictBlock(**{
                    'state': State.objects.get(fips=row['STATEFP']),
                    'district_id': row['GEOID'],
                    'geometry': GEOSGeometry(row['geometry'].to_wkt())
                }))

        DistrictBlock.objects.bulk_create(districts)

    def installStates(self, ignore_cache: bool):
        with io.open(STATE_TABLE) as handle:
            lines = handle.readlines()

        bar = IncrementalBar(f'Processing {len(lines)} states', max=len(lines) * 18)      # .next() called 18x/state

        for line in lines:
            line = line.strip()
            fips, state_code, state_name, geoid, state_ns, granularity = line.split(',')
            args = (fips, state_code, state_name, state_ns, ignore_cache, bar)
            bar.next()                       # 1 bar.next() calls (1)
            self.downloadState(*args)        # 6 bar.next() calls (7)
            self.installVTD(*args)           # 3 bar.next() calls (10)
            self.installTract(*args)         # 5 bar.next() calls (15)
            self.mergeTractIntoVTD(*args)    # 1 bar.next() calls (16)
            self.mergeDistrictIntoVTD(*args) # 2 bar.next() calls (18)

        bar.finish()

    def downloadState(self, fips, state_code, state_name, districts, ignore_cache, bar):
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

    def installVTD(self, fips, state_code, state_name, districts, ignore_cache, bar):
        # Configure variables
        state_name = state_name.lower().replace(' ', '_')
        state_vtd_dir_name = STATE_VTD_DIR_NAME.format(state_name=state_name)

        df = gpd.read_file(state_vtd_dir_name)

        df['STATEFP10'] = df['STATEFP10'].astype(int)
        df['COUNTYFP10'] = df['COUNTYFP10'].astype(int)
        df['ALAND10'] = df['ALAND10'].astype(int)
        df['AWATER10'] = df['AWATER10'].astype(int)
        
        vtds = []

        bar.next()

        for i, row in df.iterrows():
            if not VTDBlock.objects.filter(geoid=row['GEOID10']) and State.objects.filter(fips=row['STATEFP10']):
                vtds.append(VTDBlock(**{
                    'state': State.objects.get(fips=row['STATEFP10']),
                    'geoid': row['GEOID10'],
                    'geometry': GEOSGeometry(row['geometry'].to_wkt()),
                    'county': row['COUNTYFP10'],
                    'name': row['NAME10'],
                    'land': row['ALAND10'],
                    'water': row['AWATER10'],
                }))

        bar.next()

        # Insert into VTD Table
        VTDBlock.objects.bulk_create(vtds)

        bar.next()

    def installTract(self, fips, state_code, state_name, state_ns, ignore_cache, bar):
        state_name = state_name.lower().replace(' ', '_')
        state_tract_dir_name = STATE_TRACT_DIR_NAME.format(state_name=state_name)
        state_census_file_name = STATE_CENSUS_FILE_NAME.format(state_name=state_name)

        df = gpd.read_file(state_tract_dir_name)
        census_df = pd.read_csv(state_census_file_name)
        
        bar.next()

        tracts = []

        for i, row in df.iterrows():
            if not TractBlock.objects.filter(geoid=row['GEOID']) and State.objects.filter(fips=row['STATEFP']):
                tracts.append(TractBlock(**{
                    'geoid': row['GEOID'],
                    'state': State.objects.get(fips=row['STATEFP']),
                    'geometry': GEOSGeometry(row['geometry'].to_wkt()),
                    'name': row['NAME'],
                    'land': row['ALAND'],
                    'water': row['AWATER'],
                }))

        bar.next()

        # Insert into Tract Table
        TractBlock.objects.bulk_create(tracts)

        bar.next()

        colsToAdd = ['GEOID', 'P003001', 'P003002', 'P003003', 'P003004',
                    'P003005','P003006', 'P003007', 'P003008']
        readableNames = ['GEOID', 'totalPop', 'whitePop', 'blackPop', 'nativeAPop',
                        'asianPop', 'pacisPop', 'otherPop', 'multiPop']

        census_df = census_df[colsToAdd]

        # Merge the demographic data with the main dataframe
        # Match census_df's types
        cols = [i for i in census_df.columns if i not in ["GEOID"]]
        for col in cols:
            census_df[col] = census_df[col].astype(int)
        census_df["GEOID"] = census_df["GEOID"].astype(str)

        # Some GEOID's that begin with 0 get shortened
        # Re-add the leading 0 before merge
        if len(census_df["GEOID"][0]) == 10:
            census_df["GEOID"] = census_df["GEOID"].map(lambda x: '0'+x)

        census_df.rename(columns=dict(zip(colsToAdd, readableNames)), inplace=True)

        bar.next()

        for i, row in census_df.iterrows():
            if TractBlock.objects.filter(geoid=row['GEOID']):
                tract = TractBlock.objects.get(geoid=row['GEOID'])
                for name in readableNames[1:]: # ignores GEOID
                    setattr(tract, name, row[name])
                tract.save()

        bar.next()

    def mergeTractIntoVTD(self, fips, state_code, state_name, state_ns, ignore_cache, bar):
        # Merge populations
        state = State.objects.get(fips=fips)
        for vtdblock in VTDBlock.objects.filter(state=state):
            for related_tract in vtdblock.overlapping_tracts:
                # this calculates interesection area
                overlap = -vtdblock.geometry.union(related_tract.geometry).area + vtdblock.geometry.area + related_tract.geometry.area
                partition = (overlap / related_tract.geometry.area)

                vtdblock.majorityPop += partition * related_tract.whitePop
                vtdblock.minorityPop += partition * related_tract.blackPop
                vtdblock.minorityPop += partition * related_tract.nativeAPop
                vtdblock.minorityPop += partition * related_tract.asianPop
                vtdblock.minorityPop += partition * related_tract.pacisPop
                vtdblock.minorityPop += partition * related_tract.otherPop
                vtdblock.minorityPop += partition * related_tract.multiPop
        
            vtdblock.majorityPop = round(vtdblock.majorityPop)
            vtdblock.minorityPop = round(vtdblock.minorityPop)
            vtdblock.save()

        bar.next()

    def mergeDistrictIntoVTD(self, fips, state_code, state_name, state_ns, ignore_cache, bar):
        tabledict = {}
        
        for district in DistrictBlock.objects.filter(state__fips=fips):
            for vtd in district.overlapping_vtds:
                intersect = vtd.geometry.intersection(district.geometry).area
                
                if vtd.geoid in tabledict:
                    if intersect > tabledict[vtd.geoid][1]:
                        tabledict[vtd.geoid] = (district, intersect)
                else:
                    tabledict[vtd.geoid] = (district, intersect)

        bar.next()

        for geoid, district_ in tabledict.items():
            district = district_[0]
            vtd = VTDBlock.objects.get(geoid=geoid)
            vtd.district = district
            vtd.save()

        bar.next()
        
    def filterWater(self, vtd):
        if 'river' in vtd.name.lower() and vtd.land == 0:
            vtd.delete()
            return True
        return False

    def splitMultiPolygon(self, vtd):
        import pdb; pdb.set_trace()
        if vtd.geometry.type != 'Polygon':
            largestPolygon = sorted(vtd.geometry, key=lambda _: -_.area)[0]
            vtd.geometry = largestPolygon
            vtd.save()

    def cleanStates(self):
        vtds = VTDBlock.objects.all()
        states = State.objects.all()

        bar = IncrementalBar(f'Cleaning {len(vtds)} VTDs', max=len(vtds) * 2)      # .next() called 2x/vtd

        for vtd in vtds:
            # filter water precincts
            deleted = self.filterWater(vtd)
            bar.next()
            # Break multi polygons
            if not deleted:
                self.splitMultiPolygon(vtd)
            bar.next()
        
        for state in states:
            # adjust for granularity
            pass
