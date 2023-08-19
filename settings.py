import os

BASE_DIR = os.path.dirname(os.path.abspath(__file__))

# example) SQLite
#DATABASES = {
    #'default': {
        #'ENGINE': 'django.db.backends.sqlite3',
        #'NAME': os.path.join(BASE_DIR, 'db.sqlite3'),
    #}
#}

from django.urls import re_path as url
urlpatterns = [
    url(r'^$', lambda x: x)
]

ROOT_URLCONF = 'settings'

USE_TZ=False

DATABASES = {
    'default': {
        'ENGINE': 'django.db.backends.postgresql_psycopg2',
        'NAME': 'raven',
        'USER': 'postgres',
        'PASSWORD': 'abc123',
        'HOST': 'localhost',
        'PORT': '5432',
    }
}

INSTALLED_APPS = (
    'rave',
)

SECRET_KEY = 'sashsc!s60#f(2_%5yc6!h8lfzt!o1@xut3k=7xfr1+=)*hyy^fn!'
