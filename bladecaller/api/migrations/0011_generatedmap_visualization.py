# Generated by Django 3.0.6 on 2020-05-29 04:30

from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0010_auto_20200528_2035'),
    ]

    operations = [
        migrations.AddField(
            model_name='generatedmap',
            name='visualization',
            field=models.ImageField(default=None, upload_to=''),
            preserve_default=False,
        ),
    ]