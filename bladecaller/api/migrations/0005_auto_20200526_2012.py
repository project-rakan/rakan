# Generated by Django 3.0.6 on 2020-05-26 20:12

import django.contrib.postgres.fields
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0004_auto_20200526_1905'),
    ]

    operations = [
        migrations.AlterField(
            model_name='generatedmap',
            name='mapContents',
            field=django.contrib.postgres.fields.ArrayField(base_field=models.IntegerField(), size=None, unique=True),
        ),
    ]