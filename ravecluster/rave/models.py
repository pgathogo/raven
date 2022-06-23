# RadioWavesCluster - RaveCluster models
# source /d/virtualenvs/raven/script/activate
# python manage.py makemigrations
# python manage.py migrate
#
from django.db import models
from django.utils import timezone

def strip_seconds(dt):
    return dt.replace(second=0, microsecond=0)

def now():
   return strip_seconds(timezone.now())

class Cluster(models.Model):
    cluster_name = models.CharField(max_length=200)
    notes = models.TextField(blank=True, null=True)

SERVER_TYPE = (
        ('DS', 'DB-SERVER'),
        ('AS','AUDIO-SERVER'),
        )

class Server(models.Model):
    server_name = models.CharField(max_length=200)
    server_type = models.CharField(max_length=2, choices=SERVER_TYPE)
    server_ip = models.CharField(max_length=24, blank=True, null=True)
    server_port= models.IntegerField(default=0)
    notes = models.TextField(blank=True, null=True)

class Database(models.Model):
    db_name = models.CharField(max_length=200)
    server = models.ForeignKey(Server, models.DO_NOTHING)
