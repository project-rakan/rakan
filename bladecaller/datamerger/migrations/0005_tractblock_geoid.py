# Generated by Django 3.0.6 on 2020-05-28 08:35

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('datamerger', '0004_tractblock_name'),
    ]

    operations = [
        migrations.AddField(
            model_name='tractblock',
            name='geoid',
            field=models.CharField(default='', max_length=256, unique=True),
            preserve_default=False,
        ),
    ]