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

SALUTATION = (
        ('MISS', 'Miss.'),
        ('MS', 'Ms.'),
        ('MRS', 'Mrs.'),
        ('MR', 'Mr.'),
        )

REVENUE_TYPE = (
        ('C', 'Cash'),
        ('T', 'Trade'),
        )

BILL_CYCLE = (
        ('W', 'Weekly'),
        ('B', 'Bi-Weekly'),
        ('M', 'Monthly'),
        )

class Client(models.Model):
    name = models.CharField(max_length=255)
    bill_name = models.CharField(max_length=255, null=True, blank=True)
    address1 = models.CharField(max_length=255, null=True, blank=True)
    address2 = models.CharField(max_length=255, null=True, blank=True)
    town = models.CharField(max_length=200, null=True, blank=True)
    postal_no = models.CharField(max_length=100, null=True, blank=True)
    postal_code = models.CharField(max_length=10, null=True, blank=True)
    telephone = models.CharField(max_length=15, null=True, blank=True)
    client_email = models.CharField(max_length=100, null=True, blank=True)
    client_mobile = models.CharField(max_length=15, null=True, blank=True)
    contact_name = models.CharField(max_length=255, null=True, blank=True)
    contact_salute = models.CharField(max_length=5, choices=SALUTATION)
    contact_email = models.CharField(max_length=100, null=True, blank=True)
    contact_mobile = models.CharField(max_length=15, null=True, blank=True)
    agency = models.ForeignKey(Agent, blank=True, null=True)
    client_group = models.ForeignKey(ClientGroup, blank=True, null=True)
    account_manager = models.ForeignKey(SalesPerson, blank=True, null=True)
    revenue_type = models.CharField(max_length=1, blank=True, null=True, choices=REVENUE_TYPE, default='C')
    discount_percent = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    agency_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    sales_rep_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    interest_rate = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    late_fee = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    grace_period = models.IntegerField(null=True)
    bill_cycle = models.CharField(max_length=1, blank=True, null=True, choices=BILL_CYCLE)

class Brand(models.Model):
    brand_name = models.CharField(max_length=255)
    client = models.ForeignKey(Client)
    brand_manager = models.ForeignKey(SalesPerson, null=True, blank=True)

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

class Spot(Daypart):
    name = models.CharField(max_length=255)
    spot_duration = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    real_duration = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    client = models.ForeignKey(Client)
    brand = models.ForeignKey(Brand, blank=True, null=True)

class SpotVoiceOver(ManyToMany):
    pass

class SpotTypeExclusion(ManyToMany):
    pass

class TypeExclusion(Daypart):
    name = models.CharField(max_length=255)
    description = models.TextField(null=True, blank=True)

class VoiceOver(Person, Daypart):
    pass

BILL_METHOD = (
        ('FF', 'Flat Fee'),
        ('IP', 'Increment By Percentage'),
        )

class TimeBand(Daypart):
    name = models.CharField(max_length=255)
    bill_method = models.CharField(max_length=2, blank=True, null=True, choices=BILL_METHOD)
    locked = models.BooleanField(default=False)
    base_price = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    base_duration = models.IntegerField(null=True)
    ip_duration1 = models.IntegerField(null=True)
    ip_percent1 = models.IntegerField(null=True)
    ip_duration2 = models.IntegerField(null=True)
    ip_percent2 = models.IntegerField(null=True)
    ip_duration3 = models.IntegerField(null=True)
    ip_percent3 = models.IntegerField(null=True)
    ip_duration4 = models.IntegerField(null=True)
    ip_percent4 = models.IntegerField(null=True)
    ip_duration5 = models.IntegerField(null=True)
    ip_percent5 = models.IntegerField(null=True)
    ip_duration6 = models.IntegerField(null=True)
    ip_percent6 = models.IntegerField(null=True)
    ip_duration7 = models.IntegerField(null=True)
    ip_percent7 = models.IntegerField(null=True)
    ip_duration8 = models.IntegerField(null=True)
    ip_percent8 = models.IntegerField(null=True)
    ip_duration9 = models.IntegerField(null=True)
    ip_percent9 = models.IntegerField(null=True)
    ip_duration10 = models.IntegerField(null=True)
    ip_percent10 = models.IntegerField(null=True)
    ip_duration11 = models.IntegerField(null=True)
    ip_percent11 = models.IntegerField(null=True)
    ip_duration12 = models.IntegerField(null=True)
    ip_percent12 = models.IntegerField(null=True)

COMM_TYPE = (
        ('F', 'Fixed'),
        ('P', 'Percentage'),
        )

class Setup(models.Model):
    station_name = models.CharField(max_length=255, null=True, blank=True)
    station_logo = models.CharField(max_length=255, null=True, blank=True)
    address1 = models.CharField(max_length=255, null=True, blank=True)
    address2 = models.CharField(max_length=255, null=True, blank=True)
    max_spot_per_break = models.IntegerField(null=True)
    agency_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    agency_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    sales_rep_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    sales_rep_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    billing_cycle = models.CharField(max_length=1, blank=True, null=True, choices=BILL_CYCLE)
    late_fee = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    interest_rate = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    pay_grace_period = models.IntegerField(null=True)
    order_approval_levels = models.IntegerField(null=True)

class Content(models.Model):
    name = models.CharField(max_length=255)
    display_name = models.CharField(max_length=255, null=True, blank=True)
    table_name = models.CharField(max_length=255, null=True, blank=True)
    code = models.CharField(max_length=255, null=True, blank=True)

class ContentAuth(ManyToMany):
    access_bit = models.CharField(max_length=8, default='00000000')

class OrderPackage(models.Model):
    name = models.CharField(max_length=255)
    spot_count = models.IntegerField(null=True)
    mention_count = models.IntegerField(null=True)

REVENUE_TYPE = (
        ('C', 'Cash'),
        ('T', 'Trade'),
        )

ORDER_BILLING_TYPE = (
        ('G', 'Gross'),
        ('N', 'Net'),
        )

BILLING_BASIS = (
        ('STD', 'Standard'),
        ('DAY', 'Daily'),
        ('PRD', 'Billing Period'),
        )
class Order(models.Model):
    title = models.CharField(max_length=255)
    order_number = models.IntegerField(null=True)
    client = models.ForeignKey(Client)
    order_date = models.DateField()
    start_date = models.DateField()
    end_date = models.DateField()
    package = models.ForeignKey(OrderPackage, null=True)
    revenue_type = models.CharField(max_length=1, blank=True, null=True, choices=REVENUE_TYPE)
    billing_type = models.CharField(max_length=1, blank=True, null=True, choices=ORDER_BILLING_TYPE)
    billing_period = models.CharField(max_length=1, blank=True, null=True, choices=BILL_CYCLE)
    account_rep = models.ForeignKey(SalesPerson, null=True, blank=True)
    brand = models.ForeignKey(Brand, null=True, blank=True)
    agency = models.ForeignKey(Agent, null=True, blank=True)
    spots_ordered = models.IntegerField(null=True)
    spots_booked = models.IntegerField(null=True)
    spots_played = models.IntegerField(null=True)
    discount = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    agency_comm = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    agency_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    sales_rep_comm = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    sales_rep_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    trade_credit = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    trade_credit_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    late_fee = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    grace_period = models.IntegerField(null=True)
    billing_basis = models.CharField(max_length=1, blank=True, null=True, choices=BILLING_BASIS)
    approval_count = models.IntegerField(null=True, default=0)
    add_login = models.CharField(max_length=15, null=True, blank=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)

class OrderApprover(models.Model):
    user_id = models.CharField(max_length=15)
    user_title = models.CharField(max_length=255, null=True, blank=True)
    level = models.IntegerField(null=True)

class OrderApprovals(models.Model):
    order = models.ForeignKey(Order)
    approver = models.ForeignKey(OrderApprover)
    level = models.IntegerField(null=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)

