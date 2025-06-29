#include "tvprogram.h"
#include "../../../rave/framework/choicefield.h"
#include "../../../rave/framework/entitydatamodel.h"

namespace PIXELPLAN
{
        TVProgram::TVProgram()
        {
            m_title = createField<StringField>("title","Title");
            m_description = createField<TextField>("description","description");

            m_code = createField<StringField>("code","code");
            m_code->setMandatory(true);

            m_category = createField<ChoiceField<std::string>>("category","category");
            m_category->addChoice({"NEWS", "News"});
            m_category->addChoice({"SPORTS", "Sports"});
            m_category->addChoice({"ENTERTAINMENT", "Entertainment"});
            m_category->addChoice({"DOCUMENTARY", "Documentary"});
            m_category->addChoice({"MOVIE", "Movie"});
            m_category->addChoice({"SERIES", "Series"});
            m_category->addChoice({"DRAMA", "Drama"});
            m_category->addChoice({"TALK SHOW", "Talk Show"});


            m_language = createField<ChoiceField<std::string>>("prog_language","language");
            m_language->addChoice({"ENGLISH", "English"});
            m_language->addChoice({"SWAHILI", "Swahili"});
            m_language->addChoice({"FRENCH", "French"});
            m_language->addChoice({"SPANISH", "Spanish"});
            m_language->addChoice({"FRENCH", "French"});
            m_language->addChoice({"ARABIC", "Arabic"});
            m_language->addChoice({"MANDARIN", "Mandarin"});
            m_language->addChoice({"HINDI", "Hindi"});

            m_audience = createField<ChoiceField<std::string>>("audience","audience");
            m_audience->addChoice({"GENERAL", "General"});
            m_audience->addChoice({"CHILDREN", "Children"});
            m_audience->addChoice({"TEENS", "Teens"});
            m_audience->addChoice({"ADULTS", "Adults"});
            m_audience->addChoice({"SENIORS", "Seniors"});

            m_broadcast_days = createField<StringField>("broadcast_days","broadcast_days");
            m_start_time = createField<TimeField>("start_time","start_time");
            m_end_time = createField<TimeField>("end_time","end_time");
            m_duration = createField<IntegerField>("duration", "Duration");
            m_producer = createField<StringField>("producer","producer");
            m_director = createField<StringField>("director","director");
            m_writer = createField<StringField>("writer","writer");
            m_presenter = createField<StringField>("presenter","presenter");
            m_cast = createField<TextField>("prog_cast","cast");

            m_production_type = createField<ChoiceField<std::string>>("production_type","production_type");
            m_production_type->addChoice({"LIVE", "Live"});
            m_production_type->addChoice({"RECORDED", "Recorded"});
            m_production_type->addChoice({"ANIMATED", "Animated"});
            m_production_type->addChoice({"DOCUMENTARY", "Documentary"});
            m_production_type->addChoice({"SYNDICATED", "Syndicated"});

            m_production_company = createField<StringField>("production_company","production_company");
            m_production_location = createField<StringField>("production_location","production_location");
            m_broadcast_network = createField<StringField>("broadcast_network","broadcast_network");
            m_season_number = createField<IntegerField>("season_number","season_number");
            m_episode_number = createField<IntegerField>("episode_number","episode_number");
            m_repeat_schedule = createField<StringField>("repeat_schedule","repeat_schedule");
            m_tags = createField<StringField>("tags","tags");

            m_parental_rating = createField<ChoiceField<std::string>>("parental_rating","parental_rating");
            m_parental_rating->addChoice({"G", "General Audience"});
            m_parental_rating->addChoice({"PG", "Parental Guidance"});
            m_parental_rating->addChoice({"PG-13", "Parents Strongly Cautioned"});
            m_parental_rating->addChoice({"R", "Restricted"});
            m_parental_rating->addChoice({"NC-17", "Adults Only"});

            m_cover_image = createField<StringField>("cover_image","cover_image");
            m_script_file = createField<StringField>("script_file","script_file");
            m_created_at = createField<DateTimeField>("created_at","created_at");

            m_header << QString::fromStdString(m_title->fieldLabel());

            setTableName("rave_tvprogram");

        }

        TVProgram::~TVProgram()
        {

        }

        StringField*  TVProgram::title() const
        {
            return m_title;
        }

        TextField*  TVProgram::description() const
        {
            return m_description;
        }

        StringField*  TVProgram::code() const
        {
            return m_code;
        }

        ChoiceField<std::string>*  TVProgram::category() const
        {
            return m_category;
        }

        ChoiceField<std::string>*  TVProgram::language() const
        {
            return m_language;
        }

        ChoiceField<std::string>*  TVProgram::audience() const
        {
            return m_audience;
        }

        StringField*  TVProgram::broadcast_days() const
        {
            return m_broadcast_days;
        }

        TimeField*  TVProgram::start_time() const
        {
            return m_start_time;
        }

        TimeField*  TVProgram::end_time() const
        {
            return m_end_time;
        }

        IntegerField* TVProgram::duration()
        {
            return m_duration;
        }

        StringField*  TVProgram::producer() const
        {
            return m_producer;
        }

        StringField*  TVProgram::director() const
        {
            return m_director;
        }

        StringField*  TVProgram::writer() const
        {
            return m_writer;
        }

        StringField*  TVProgram::presenter() const
        {
            return m_presenter;
        }

        TextField*  TVProgram::cast() const
        {
            return m_cast;
        }

        ChoiceField<std::string>*  TVProgram::production_type() const
        {
            return m_production_type;
        }

        StringField*  TVProgram::production_company() const
        {
            return m_production_company;
        }

        StringField*  TVProgram::production_location() const
        {
            return m_production_location;
        }

        StringField*  TVProgram::broadcast_network() const
        {
            return m_broadcast_network;
        }

        IntegerField*  TVProgram::season_number() const
        {
            return m_season_number;
        }

        IntegerField*  TVProgram::episode_number() const
        {
            return m_episode_number;
        }

        StringField*  TVProgram::repeat_schedule() const
        {
            return m_repeat_schedule;
        }

        StringField*  TVProgram::tags() const
        {
            return m_tags;
        }
        ChoiceField<std::string>*  TVProgram::parental_rating() const
        {
            return m_parental_rating;
        }

        StringField*  TVProgram::cover_image() const
        {
            return m_cover_image;
        }

        StringField*  TVProgram::script_file() const
        {
            return m_script_file;
        }

        DateTimeField*  TVProgram::created_at() const
        {
            return m_created_at;
        }

        StringField*  TVProgram::created_by() const
        {
            return m_created_by;
        }

        //  Set methods

        void  TVProgram::set_title(const std::string title)
        {
            m_title->setValue(title);

        }

        void  TVProgram::set_description(const std::string desc)
        {
            m_description->setValue(desc);
        }

        void  TVProgram::set_code(const std::string code)
        {
            m_code->setValue(code);
        }

        void  TVProgram::set_category(const std::string cat)
        {
            m_category->setValue(cat);
        }

        void  TVProgram::set_language(const std::string lang)
        {
            m_language->setValue(lang);
        }

        void  TVProgram::set_audience(const std::string aud)
        {
            m_audience->setValue(aud);
        }

        void  TVProgram::set_broadcast_days(const std::string days)
        {
            m_broadcast_days->setValue(days);
        }

        void  TVProgram::set_start_time(QTime stime)
        {
            m_start_time->setValue(stime);
        }

        void  TVProgram::set_end_time(QTime etime)
        {
            m_end_time->setValue(etime);
        }

        void TVProgram::set_duration(int dur)
        {
            m_duration->setValue(dur);
        }

        void  TVProgram::set_producer(const std::string prod)
        {
            m_producer->setValue(prod);
        }

        void  TVProgram::set_director(const std::string direct)
        {
            m_director->setValue(direct);
        }

        void  TVProgram::set_writer(const std::string writer)
        {
            m_writer->setValue(writer);
        }

        void  TVProgram::set_presenter(const std::string present)
        {
            m_presenter->setValue(present);
        }

        void  TVProgram::set_cast(const std::string cast)
        {
            m_cast->setValue(cast);
        }

        void  TVProgram::set_production_type(const std::string prod)
        {
            m_production_type->setValue(prod);
        }

        void  TVProgram::set_production_company(const std::string prod_comp)
        {
            m_production_company->setValue(prod_comp);
        }

        void  TVProgram::set_production_location(const std::string prod_loc)
        {
            m_production_location->setValue(prod_loc);
        }

        void  TVProgram::set_broadcast_network(const std::string broad_net)
        {
            m_broadcast_network->setValue(broad_net);
        }

        void  TVProgram::set_season_number(int snum)
        {
            m_season_number->setValue(snum);
        }

        void  TVProgram::set_episode_number(int enumb)
        {
            m_episode_number->setValue(enumb);
        }

        void  TVProgram::set_repeat_schedule(const std::string rsched)
        {
            m_repeat_schedule->setValue(rsched);
        }

        void  TVProgram::set_tags(const std::string tags)
        {
            m_tags->setValue(tags);
        }

        void  TVProgram::parental_rating(const std::string prate)
        {
            m_parental_rating->setValue(prate);
        }

        void  TVProgram::set_cover_image(const std::string cimage)
        {
            m_cover_image->setValue(cimage);
        }

        void  TVProgram::set_script_file(const std::string sfile)
        {
            m_script_file->setValue(sfile);
        }

        void  TVProgram::set_created_at(QDateTime create_date)
        {
            m_created_at->setValue(create_date);
        }

        void  TVProgram::set_created_by(const std::string create_by)
        {
            m_created_by->setValue(create_by);
        }

        std::string TVProgram::tableName() const
        {
            return m_table_name;
        }

        void TVProgram::setTableName(const std::string table_name)
        {
            m_table_name = table_name;
        }

        std::unique_ptr<BaseEntity> TVProgram::mapFields(StringMap* raw_entity)
        {
            return nullptr;
        }

        std::vector<std::string> TVProgram::tableViewColumns() const
        {
            title()->set_display_width(300);

            return tableViewCols<std::string>(
                title()->displayName()
            );
        }

        std::vector<std::string> TVProgram::tableViewValues()
        {
            return {
                title()->displayName()
            };
        }

        QStringList TVProgram::tableHeaders() const
        {
            return m_header;
        }

        std::string TVProgram::searchColumn() const
        {
            return title()->valueToString();
        }

        void TVProgram::populateEntity()
        {
        }

        std::shared_ptr<BaseEntity> TVProgram::cloneAsShared()
        {
            return std::make_shared<TVProgram>();
        }

        void TVProgram::afterMapping(BaseEntity& entity)
        {
        }

        ActionResult TVProgram::validate()
        {
            return std::make_tuple(ActionResultType::arSUCCESS, "OK");

        }
}
