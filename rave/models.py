# RadioWaves - Raven models
# source /d/virtualenvs/raven/script/activate
# python manage.py makemigrations
# python manage.py migrate
#
from datetime import datetime
from django.db import models

class ClientGroup(models.Model):
    name = models.CharField(max_length=200)
    description = models.TextField()
