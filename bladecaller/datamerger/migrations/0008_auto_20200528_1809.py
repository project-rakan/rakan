# Generated by Django 3.0.6 on 2020-05-28 18:09

from django.db import migrations, models
import django.db.models.deletion


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0009_state_granularity'),
        ('datamerger', '0007_vtdblock_district'),
    ]

    operations = [
        migrations.AlterField(
            model_name='vtdblock',
            name='state',
            field=models.ForeignKey(on_delete=django.db.models.deletion.CASCADE, related_name='vtds', to='api.State'),
        ),
    ]
