from django.contrib import admin
from api import models

# Register your models here.

admin.site.register(models.State)
admin.site.register(models.GeneratedMap)