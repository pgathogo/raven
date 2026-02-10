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
    gender = models.ForeignKey(Gender, models.DO_NOTHING, null=True, blank=True)
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
    agency = models.ForeignKey(Agent, models.DO_NOTHING, blank=True, null=True)
    client_group = models.ForeignKey(ClientGroup,models.DO_NOTHING,  blank=True, null=True)
    account_manager = models.ForeignKey(SalesPerson,models.DO_NOTHING,  blank=True, null=True)
    revenue_type = models.CharField(max_length=1, blank=True, null=True, choices=REVENUE_TYPE, default='C')
    discount_percent = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    agency_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    sales_rep_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    interest_rate = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    late_fee = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    grace_period = models.IntegerField(null=True)
    bill_cycle = models.CharField(max_length=1, blank=True, null=True, choices=BILL_CYCLE)
    erp_code = models.CharField(max_length=50, null=True, blank=True)

class Brand(models.Model):
    brand_name = models.CharField(max_length=255)
    client = models.ForeignKey(Client, models.DO_NOTHING)
    brand_manager = models.ForeignKey(SalesPerson,models.DO_NOTHING,  null=True, blank=True)

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
    spot_duration = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    real_duration = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    client = models.ForeignKey(Client, models.DO_NOTHING)
    brand = models.ForeignKey(Brand, models.DO_NOTHING, blank=True, null=True)

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


class Setup(models.Model):
    station_name = models.CharField(max_length=255, null=True, blank=True)
    station_logo = models.CharField(max_length=255, null=True, blank=True)
    address1 = models.CharField(max_length=255, null=True, blank=True)
    address2 = models.CharField(max_length=255, null=True, blank=True)
    agency_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    agency_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    sales_rep_comm = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    sales_rep_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    billing_cycle = models.CharField(max_length=1, blank=True, null=True, choices=BILL_CYCLE)
    billing_type = models.CharField(max_length=1, blank=True, null=True, choices=ORDER_BILLING_TYPE)
    billing_basis = models.CharField(max_length=3, blank=True, null=True, choices=BILLING_BASIS)
    late_fee = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    interest_rate = models.DecimalField(max_digits=5, decimal_places=2, null=True)
    pay_grace_period = models.IntegerField(null=True)
    revenue_type = models.CharField(max_length=1, blank=True, null=True, choices=REVENUE_TYPE);
    order_approval_levels = models.IntegerField(null=True)
    order_number_sequence = models.IntegerField(null=True);
    order_approved_before_booking = models.BooleanField(default=False)
    break_time_interval = models.IntegerField(default=15)
    break_duration = models.IntegerField(default=120)
    break_max_spots = models.IntegerField(default=4)
    audio_path = models.CharField(max_length=255, null=True, blank=True)
    comm_audio_path = models.CharField(max_length=255, null=True, blank=True)
    convert_to_ogg = models.BooleanField(null=True, blank=True)
    editor_filepath = models.CharField(max_length=255, null=True, blank=True)
    playlist_template_filepath = models.CharField(max_length=300, null=True, blank=True)
    playlist_output_path = models.CharField(max_length=300, null=True, blank=True)
    playlist_backup_path = models.CharField(max_length=300, null=True, blank=True)
    report_viewer_path = models.CharField(max_length=300, null=True, blank=True)
    report_runner_path = models.CharField(max_length=300, null=True, blank=True)
    magicsoft_logfile_path = models.CharField(max_length=300, null=True, blank=True)


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

class Order(models.Model):
    title = models.CharField(max_length=255)
    order_number = models.CharField(max_length=50, null=True, blank=True)
    client = models.ForeignKey(Client, models.DO_NOTHING)
    order_date = models.DateField()
    start_date = models.DateField()
    end_date = models.DateField()
    package = models.ForeignKey(OrderPackage, models.DO_NOTHING, null=True)
    revenue_type = models.CharField(max_length=1, blank=True, null=True, choices=REVENUE_TYPE)
    billing_type = models.CharField(max_length=1, blank=True, null=True, choices=ORDER_BILLING_TYPE)
    billing_period = models.CharField(max_length=1, blank=True, null=True, choices=BILL_CYCLE)
    account_rep = models.ForeignKey(SalesPerson,models.DO_NOTHING,  null=True, blank=True)
    brand = models.ForeignKey(Brand, models.DO_NOTHING, null=True, blank=True)
    agency = models.ForeignKey(Agent,models.DO_NOTHING,  null=True, blank=True)
    spots_ordered = models.IntegerField(null=True)
    spots_booked = models.IntegerField(null=True)
    spots_played = models.IntegerField(null=True)
    discount = models.DecimalField(max_digits=6, decimal_places=2, null=True)
    agency_comm = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    agency_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    sales_rep_comm = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    sales_rep_comm_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    trade_credit = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    trade_credit_type = models.CharField(max_length=1, blank=True, null=True, choices=COMM_TYPE)
    late_fee = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    grace_period = models.IntegerField(null=True)
    billing_basis = models.CharField(max_length=3, blank=True, null=True, choices=BILLING_BASIS)
    approval_count = models.IntegerField(null=True, default=0)
    add_login = models.CharField(max_length=15, null=True, blank=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)

class OrderApprover(models.Model):
    user_id = models.IntegerField(null=True)
    username = models.CharField(max_length=15, null=True, blank=True)
    user_title = models.CharField(max_length=255, null=True, blank=True)
    level = models.IntegerField(null=True)

class OrderApprovals(models.Model):
    order = models.ForeignKey(Order, models.DO_NOTHING)
    approver = models.ForeignKey(OrderApprover, models.DO_NOTHING)
    username = models.CharField(max_length=15, null=True, blank=True)
    level = models.IntegerField(null=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)


TIME_INTERVAL = (
        (15, '15 Mins'),
        (10, '10 Mins'),
        (5,   '5 Mins'),
        (1,   '1 Min'),
        )

BREAK_FILL_METHOD = (
        ('S', 'Sequential'),
        ('R', 'Random')
        )

PARENTAL_RATING = (
    ('G', 'General Audience'),
    ('PG', 'Parental Guidance'),
    ('PG-13', 'Parents Strongly Cautioned'),
    ('R', 'Restricted'),
    ('NC-17', 'Adults Only'),
)

class TVProgram(models.Model):
    title = models.CharField(max_length=255)
    description = models.TextField(null=True, blank=True)
    code = models.CharField(max_length=50, unique=True)
    CATEGORY = (
        ('NEWS', 'News'),
        ('SPORTS', 'Sports'),
        ('ENTERTAINMENT', 'Entertainment'),
        ('DOCUMENTARY', 'Documentary'),
        ('MOVIE', 'Movie'),
        ('SERIES', 'Series'),
        ('DRAMA', 'Drama'),
        ('TALK SHOW', 'Talk Show'),
    )
    category = models.CharField(max_length=50, choices=CATEGORY, null=True, blank=True)

    LANGUAGE = (
        ('ENGLISH', 'English'),
        ('SWAHILI', 'Swahili'),
        ('FRENCH', 'French'),
        ('SPANISH', 'Spanish'),
        ('FRENCH', 'French'),
        ('ARABIC', 'Arabic'),
        ('MANDARIN', 'Mandarin'),
        ('HINDI', 'Hindi'),
    )
    prog_language = models.CharField(max_length=50, choices=LANGUAGE, null=True, blank=True)

    AUDIENCE = (
        ('GENERAL', 'General'),
        ('CHILDREN', 'Children'),
        ('TEENS', 'Teens'),
        ('ADULTS', 'Adults'),
        ('SENIORS', 'Seniors'),
    )
    audience = models.CharField(max_length=50, choices=AUDIENCE, null=True, blank=True)

    broadcast_days = models.CharField(max_length=50, null=True, blank=True)  # e.g., "Mon,Tue,Wed,Thu,Fri,Sat,Sun"
    start_time = models.TimeField(null=True, blank=True)
    end_time = models.TimeField(null=True, blank=True)
    duration = models.IntegerField(null=True, blank=True)  # Duration in minutes

    # Personnel
    producer = models.CharField(max_length=255, null=True, blank=True)
    director = models.CharField(max_length=255, null=True, blank=True)
    writer = models.CharField(max_length=255, null=True, blank=True)
    presenter = models.CharField(max_length=255, null=True, blank=True)
    prog_cast = models.TextField(null=True, blank=True)  # Comma-separated list of cast members

    # Production Information
    PRODUCTION_TYPE = (
        ('LIVE', 'Live'),
        ('RECORDED', 'Recorded'),
        ('ANIMATED', 'Animated'),
        ('DOCUMENTARY', 'Documentary'),
        ('SYNDICATED', 'Syndicated'),
    )

    production_type = models.CharField(max_length=50, choices=PRODUCTION_TYPE, null=True, blank=True)
    production_company = models.CharField(max_length=255, null=True, blank=True)
    production_location = models.CharField(max_length=255, null=True, blank=True)

    # Broadcast Information
    broadcast_network = models.CharField(max_length=255, null=True, blank=True)
    season_number = models.IntegerField(null=True, blank=True)
    episode_number = models.IntegerField(null=True, blank=True)
    repeat_schedule = models.CharField(max_length=255, null=True, blank=True)  # e.g., "Mon 8 PM, Wed 9 PM"

    # Metadata
    tags = models.CharField(max_length=255, null=True, blank=True)  # Comma-separated tags for easy searching
    parental_rating = models.CharField(max_length=5, choices=PARENTAL_RATING, null=True, blank=True)
    cover_image = models.CharField(max_length=255, null=True, blank=True)
    script_file = models.CharField(max_length=255, null=True, blank=True)

    created_at = models.DateTimeField(auto_now_add=True)
    created_by = models.CharField(max_length=100, null=True, blank=True)


class AdvertMedia(models.Model):
    title = models.CharField(max_length=255)
    media_file = models.CharField(max_length=300, blank=True, null=True)
    media_path = models.CharField(max_length=300, blank=True, null=True)
    file_extension = models.CharField(max_length=10, blank=True, null=True)
    duration = models.IntegerField(null=True, blank=True)
    aspect_ratio = models.CharField(max_length=20, blank=True, null=True, help_text="Video aspect ratio, e.g., 16:9")
    resolution = models.CharField(max_length=50, blank=True, null=True, help_text="Resolution of the video, e.g., 1920x1080")
    rating = models.CharField(max_length=5, choices=PARENTAL_RATING, blank=True, null=True)
    created_at = models.DateTimeField(auto_now_add=True)
    client = models.ForeignKey(Client, models.DO_NOTHING, default=0)
    spot = models.ForeignKey(Spot, models.DO_NOTHING, default=0, null=True, blank=True)
    


class BreakLayout(models.Model):
    tvprogram = models.ForeignKey(TVProgram, models.DO_NOTHING, default=0)
    time_interval = models.IntegerField(choices=TIME_INTERVAL, default=15)
    week_days = models.CharField(max_length=7)
    break_fill_method = models.CharField(max_length=1, blank=True, null=True, choices=BREAK_FILL_METHOD);

class BreakLayoutLine(models.Model):
    break_layout = models.ForeignKey(BreakLayout, models.DO_NOTHING)
    week_day = models.IntegerField()
    break_time = models.TimeField(auto_now=False, auto_now_add=False)
    break_hour = models.IntegerField(default=0)
    duration = models.IntegerField()
    max_spots = models.IntegerField()
    break_fill_method = models.CharField(max_length=1, blank=True, null=True, choices=BREAK_FILL_METHOD);

ARTIST_TYPE = (
        ('F','FEMALE'),
        ('M','MALE'),
        ('G','GROUP'),
       )

MOOD_TYPE = (
        ('low','LOW'),
        ('vlow','VERY LOW'),
        ('medium', 'MEDIUM'),
        ('high', 'HIGH'),
        ('vhigh', 'VERY HIGH'),
)

class Genre(models.Model):
    genre = models.CharField(max_length=255)

class Mood(models.Model):
    mood = models.CharField(max_length=100)
    intensity = models.CharField(max_length=8, choices=MOOD_TYPE)
    timbre = models.CharField(max_length=8, choices=MOOD_TYPE)
    pitch = models.CharField(max_length=8, choices=MOOD_TYPE)
    rhythm = models.CharField(max_length=8, choices=MOOD_TYPE)

class Folder(models.Model):
    folder_name = models.CharField(max_length=255)
    parent = models.IntegerField()

class Artist(models.Model):
    first_name = models.CharField(max_length=255, blank=True, null=True)
    surname = models.CharField(max_length=255, blank=True, null=True)
    fullname = models.CharField(max_length=255, blank=True, null=True)
    artist_type = models.CharField(max_length=1, choices=ARTIST_TYPE)
    notes = models.TextField(blank=True, null=True)

AUDIO_TYPE = (
        ('SONG', 'Song'),
        ('COMM', 'Commercial'),
        ('JING', 'Jingle'),
        ('DROP', 'Drop'),
        ('NBITE', 'News Bite')
        )

class Audio(Daypart):
    title = models.CharField(max_length=255)
    short_desc = models.CharField(max_length=255, null=True, blank=True)
    artist = models.ForeignKey(Artist,models.DO_NOTHING,  null=True)
    filepath = models.CharField(max_length=255)
    audio_type = models.CharField(max_length=20, choices=AUDIO_TYPE, default='SONG')
    file_extension = models.CharField(max_length=5, default='OGG')
    duration = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    start_marker = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    fade_in_marker = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    intro_marker = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    extro_marker = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    fade_out_marker = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    fade_delay_marker = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    end_marker = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    genre = models.ForeignKey(Genre, models.DO_NOTHING, blank=True, null=True)
    mood = models.ForeignKey(Mood, models.DO_NOTHING, blank=True, null=True)
    folder = models.ForeignKey(Folder, models.DO_NOTHING, blank=True, null=True)
    start_date = models.DateField(null=True)
    end_date = models.DateField(null=True)
    deleted = models.BooleanField(default=False)
    play_count = models.IntegerField(null=True)
    audio_year = models.IntegerField(null=True)
    creation_date = models.DateField(null=True)
    notes = models.TextField(blank=True, null=True)

PLAY_STATUS = (
        ('WAITING','WAITING'),
        ('CUED','CUED'),
        ('PLAYED','PLAYED'),
        ('CANCELLED','CANCELLED'),
        )

SCHEDULE_TYPE = (
        ('SONG','SONG'),
        ('COMM-BREAK','COMMERCIAL'),
        ('COMM-AUDIO','COMMERCIAL-AUDIO'),
        ('JINGLE','JINGLE'),
        )

BREAK_MODE = (
        ('SINGLE','SINGLE'),
        ('MIXED','MIXED'),
        )

BREAK_STATUS = (
        ('LOCKED','LOCKED'),
        ('OPEN','OPEN'),
        )

TRANSITION = (
        (0, 'None'),
        (1, 'Mix'),
        (2, 'Cut'),
        (3, 'Back'),
        (4, 'Center'),
        (5, 'Early'),
        (6, 'Synchro'),
        (7, 'Stop'),
        )

class Schedule(models.Model):
    schedule_date = models.DateField()
    schedule_time = models.TimeField(auto_now=False, auto_now_add=False, null=True)
    schedule_hour = models.IntegerField(default=0, null=True)
    audio = models.ForeignKey(Audio, models.DO_NOTHING, null=True)
    fade_in = models.IntegerField(default=0, null=True)
    fade_out = models.IntegerField(default=0, null=True)
    fade_delay = models.IntegerField(default=0, null=True)
    play_status = models.CharField(max_length=15, choices=PLAY_STATUS, blank=True, null=True)
    play_date = models.DateField(null=True)
    play_time = models.TimeField(null=True)
    auto_transition = models.IntegerField(default=0, null=True)
    live_transition = models.IntegerField(default=0, null=True)
    schedule_item_type = models.CharField(max_length=20, choices=SCHEDULE_TYPE, default='SONG')
    break_duration = models.IntegerField(default=0, null=True)
    break_start_win = models.IntegerField(default=0, null=True)
    break_end_win = models.IntegerField(default=0, null=True)
    break_max_spots = models.IntegerField(default=0, null=True)
    break_fill_method = models.CharField(max_length=1, blank=True, null=True, choices=BREAK_FILL_METHOD, default='S')
    booked_spots = models.IntegerField(default=0, null=True)
    break_duration_left = models.IntegerField(default=0, null=True)
    break_mode = models.CharField(max_length=8, choices=BREAK_MODE, default='MIXED', null=True, blank=True)
    break_status = models.CharField(max_length=6, choices=BREAK_STATUS, default='OPEN', null=True, blank=True)
    comment = models.CharField(max_length=255, blank=True, null=True)

class SpotAudio(models.Model):
    spot = models.ForeignKey(Spot, models.DO_NOTHING)
    audio = models.ForeignKey(Audio, models.DO_NOTHING)
    weight = models.IntegerField(default=100, null=True)
    seq_no = models.IntegerField(default=1, null=True)

class SpotMedia(models.Model):
    spot = models.ForeignKey(Spot, models.DO_NOTHING)
    media = models.ForeignKey(AdvertMedia, models.DO_NOTHING)
    weight = models.IntegerField(default=100, null=True)
    seq_no = models.IntegerField(default=1, null=True)

class BookingSegment(models.Model):
    order = models.ForeignKey(Order, models.DO_NOTHING, default=-1)
    booking_date = models.DateField(null=True)
    start_date = models.DateField(null=True)
    end_date = models.DateField(null=True)
    booking_count = models.IntegerField(default=0, null=True)
    segment_value = models.DecimalField(max_digits=16, decimal_places=2, null=True)
    comments =  models.TextField(null=True, blank=True)
    add_login = models.CharField(max_length=15, null=True, blank=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)

class ReasonVoidBooking(models.Model):
    reason = models.CharField(max_length=255)
    description = models.TextField(null=True, blank=True)

BOOKING_STATUS = (
        ('READY','READY'),
        ('PLAYED','PLAYED'),
        ('CANCELLED','CANCEL'),
        )

class OrderBooking(models.Model):
    schedule = models.ForeignKey(Schedule, models.DO_NOTHING)
    bookingsegment = models.ForeignKey(BookingSegment, models.DO_NOTHING, default=-1)
    booking_status = models.CharField(max_length=15, choices=BOOKING_STATUS, default='READY', null=True, blank=True)
    book_date = models.DateField(null=True)
    book_time = models.TimeField(auto_now=False, auto_now_add=False, null=True)
    book_hour = models.IntegerField(default=0, null=True)
    play_date = models.DateField(null=True)
    play_time = models.DateTimeField(default=now(), null=True, blank=True)
    spot = models.ForeignKey(Spot, models.DO_NOTHING, default=-1)
    booked_audio = models.ForeignKey(AdvertMedia, models.DO_NOTHING, null=True)
    played_audio = models.IntegerField(default=-1, null=True, blank=True)
    book_seq = models.IntegerField(default=1, null=True)
    order = models.ForeignKey(Order,models.DO_NOTHING,  null=True)
    void_reason = models.ForeignKey(ReasonVoidBooking, models.DO_NOTHING, null=True, blank=True)
    void_login = models.CharField(max_length=15, null=True, blank=True)
    void_dtime = models.DateTimeField(null=True, blank=True)
    comments = models.TextField(null=True, blank=True)
    add_login = models.CharField(max_length=15, null=True, blank=True)
    add_dtime = models.DateTimeField(default=now(), null=True, blank=True)
