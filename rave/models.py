# RadioWaves - Raven models
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

class ValueList(models.Model):
    code = models.CharField(max_length=6, null=True, blank=True)
    value = models.CharField(max_length=100)

    class Meta:
        abstract = True

class Gender(ValueList):
    pass

class ManyToMany(models.Model):
    parent_id = models.IntegerField()
    detail_id = models.IntegerField()

    class Meta:
        abstract = True

class VoiceExclusion(ManyToMany):
    pass

class ClientGroup(models.Model):
    name = models.CharField(max_length=200)
    description = models.TextField(null=True, blank=True)
    add_login = models.CharField(max_length=15, null=True, blank=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)

class Person(models.Model):
    name = models.CharField(max_length=200)
    gender = models.ForeignKey(Gender, null=True, blank=True)
    mobile_no = models.CharField(max_length=15, null=True, blank=True)
    add_login = models.CharField(max_length=15, null=True, blank=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)

    class Meta:
        abstract = True

class SalesPerson(Person):
    pass

class Agent(models.Model):
    name = models.CharField(max_length=250)
    address = models.TextField(null=True, blank=True)
    town = models.CharField(max_length=200, null=True, blank=True)
    telephone = models.CharField(max_length=200, null=True, blank=True)
    email  = models.CharField(max_length=200, null=True, blank=True)
    contact_name = models.CharField(max_length=200, null=True, blank=True)
    contact_mobile_no = models.CharField(max_length=200, null=True, blank=True)

class Daypart(models.Model):
    daypart1 = models.CharField(max_length=96, null=True, blank=True)
    daypart2 = models.CharField(max_length=96, null=True, blank=True)
    daypart3 = models.CharField(max_length=96, null=True, blank=True)
    daypart4 = models.CharField(max_length=96, null=True, blank=True)
    daypart5 = models.CharField(max_length=96, null=True, blank=True)
    daypart6 = models.CharField(max_length=96, null=True, blank=True)
    daypart7 = models.CharField(max_length=96, null=True, blank=True)
    class Meta:
        abstract = True

class TypeExclusion(Daypart):
    name = models.CharField(max_length=255)
    description = models.TextField(null=True, blank=True)

class VoiceOver(Person, Daypart):
    pass
