# Generated by Django 3.0.6 on 2020-05-30 05:35

from django.db import migrations


class Migration(migrations.Migration):

    dependencies = [
        ('datamerger', '0010_vtdblock_vtd_state_id'),
    ]

    operations = [
        migrations.RenameField(
            model_name='vtdblock',
            old_name='vtd_state_id',
            new_name='engine_id',
        ),
    ]
