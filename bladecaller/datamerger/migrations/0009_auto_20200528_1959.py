# Generated by Django 3.0.6 on 2020-05-28 19:59

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('datamerger', '0008_auto_20200528_1809'),
    ]

    operations = [
        migrations.AlterField(
            model_name='districtblock',
            name='district_id',
            field=models.IntegerField(),
        ),
    ]
