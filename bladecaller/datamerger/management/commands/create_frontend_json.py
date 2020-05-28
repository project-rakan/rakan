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


class Command(BaseCommand):
    help = "Download all the VTD into the database"

    def add_arguments(self, parser):
        parser.add_argument('--ignore-cache', type=bool, default=False)

    def handle(self, *args, **options):
        raise NotImplementedError("Abstract!")