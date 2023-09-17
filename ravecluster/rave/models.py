# RadioWavesCluster - RaveCluster models
# source /d/home/virtualenvs/raven/script/activate
# python manage.py makemigrations
# python manage.py migrate

#  select * from pg_authid
#  select * from pg_shadow

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
        ('DBS', 'DB-SERVER'),
        ('ADS','AUDIO-SERVER'),
        )

class Server(models.Model):
    server_name = models.CharField(max_length=200)
    server_type = models.CharField(max_length=3, choices=SERVER_TYPE)
    server_ip = models.CharField(max_length=24, blank=True, null=True)
    port_no = models.IntegerField(default=5432)
    db_admin = models.CharField(max_length=50, blank=True, null=True)
    db_admin_password = models.CharField(max_length=255, blank=True, null=True) 
    cluster = models.ForeignKey(Cluster, models.DO_NOTHING)
    notes = models.TextField(blank=True, null=True)

class Database(models.Model):
    db_name = models.CharField(max_length=200)
    server = models.ForeignKey(Server, models.DO_NOTHING)
    db_port= models.IntegerField(default=0)
    

class Station(models.Model):
    station_name = models.CharField(max_length=200)
    db_name = models.CharField(max_length=30, blank=True, null=True)
    cluster = models.ForeignKey(Cluster, models.DO_NOTHING)


class AudioStore(models.Model):
    store_name = models.CharField(max_length=200)
    server = models.ForeignKey(Server, models.DO_NOTHING)
    station = models.ForeignKey(Station, models.DO_NOTHING)
    notes = models.TextField(blank=True, null=True)


class StorageDisk(models.Model):
    disk_name = models.CharField(max_length=100)
    audio_server = models.ForeignKey(Server, models.DO_NOTHING)
    capacity = models.DecimalField(max_digits=8, decimal_places=2, null=True)


class AudioFolder(models.Model):
    folder_name = models.CharField(max_length = 200)
    disk = models.ForeignKey(StorageDisk, models.DO_NOTHING, null=True)


class StationSetup(models.Model):
    station = models.ForeignKey(Station, models.DO_NOTHING)
    audio_store = models.ForeignKey(AudioStore, models.DO_NOTHING)
    audio_folder = models.ForeignKey(AudioFolder, models.DO_NOTHING)
    database = models.ForeignKey(Database, models.DO_NOTHING)

class ManyToMany(models.Model):
    parent_id = models.IntegerField()
    detail_id = models.IntegerField()
    class Meta:
        abstract = True

class Content(models.Model):
    name = models.CharField(max_length=255)
    display_name = models.CharField(max_length=255, null=True, blank=True)
    table_name = models.CharField(max_length=255, null=True, blank=True)
    code = models.CharField(max_length=255, null=True, blank=True)

class ContentAuth(ManyToMany):
    access_bit = models.CharField(max_length=8, default='00000000')

class UserAccess(models.Model):
    username = models.CharField(max_length=50)
    station = models.ForeignKey(Station, models.DO_NOTHING)

class UserConfig(models.Model):
    username = models.CharField(max_length=50)
    reset_password = models.IntegerField(default=0)
