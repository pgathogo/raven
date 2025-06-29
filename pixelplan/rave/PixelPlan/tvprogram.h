#ifndef TVPROGRAM_H
#define TVPROGRAM_H

#include "../../../rave/framework/baseentity.h"

class StringField;
class TextField;
class TimeField;

template<typename T>
class ChoiceField;

using namespace std;

namespace PIXELPLAN
{

    class TVProgram : public BaseEntity
    {
    public:
        TVProgram();
        ~TVProgram() override;

        StringField* title() const;
        TextField* description() const;
        StringField* code() const;
        ChoiceField<std::string>* category() const;
        ChoiceField<std::string>* language() const;
        ChoiceField<std::string>* audience() const;
        StringField* broadcast_days() const;
        TimeField* start_time() const;
        TimeField* end_time() const;
        IntegerField* duration();

        StringField* producer() const;
        StringField* director() const;
        StringField* writer() const;
        StringField* presenter() const;
        TextField* cast() const;

        ChoiceField<std::string>* production_type() const;
        StringField* production_company() const;
        StringField* production_location() const;

        StringField* broadcast_network() const;
        IntegerField* season_number() const;
        IntegerField* episode_number() const;
        StringField* repeat_schedule() const;

        StringField* tags() const;
        ChoiceField<std::string>* parental_rating() const;
        StringField* cover_image() const;
        StringField* script_file() const;

        DateTimeField* created_at() const;
        StringField* created_by() const;

        void set_title(const std::string);
        void set_description(const std::string);
        void set_code(const std::string);
        void set_category(const std::string);
        void set_language(const std::string);
        void set_audience(const std::string);
        void set_broadcast_days(const std::string);
        void set_start_time(QTime);
        void set_end_time(QTime);
        void set_duration(int);

        void set_producer(const std::string);
        void set_director(const std::string);
        void set_writer(const std::string);
        void set_presenter(const std::string);
        void set_cast(const std::string);

        void set_production_type(const std::string);
        void set_production_company(const std::string);
        void set_production_location(const std::string);

        void set_broadcast_network(const std::string);
        void set_season_number(int);
        void set_episode_number(int);
        void set_repeat_schedule(const std::string);

        void set_tags(const std::string);
        void parental_rating(const std::string);
        void set_cover_image(const std::string);
        void set_script_file(const std::string);

        void set_created_at(QDateTime);
        void set_created_by(const std::string);

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string searchColumn() const override;
        void populateEntity() override;

        std::shared_ptr<BaseEntity> cloneAsShared() override;
        void afterMapping(BaseEntity& entity) override;

        [[nodiscard]] ActionResult validate() override;

    private:
        StringField* m_title;
        TextField* m_description;
        StringField* m_code;
        ChoiceField<std::string>* m_category;
        ChoiceField<std::string>* m_language;
        ChoiceField<std::string>* m_audience;
        StringField* m_broadcast_days;
        TimeField* m_start_time;
        TimeField* m_end_time;
        IntegerField* m_duration;

        StringField* m_producer;
        StringField* m_director;
        StringField* m_writer;
        StringField* m_presenter;
        TextField* m_cast;

        ChoiceField<std::string>* m_production_type;
        StringField* m_production_company;
        StringField* m_production_location;

        StringField* m_broadcast_network;
        IntegerField* m_season_number;
        IntegerField* m_episode_number;
        StringField* m_repeat_schedule;

        StringField* m_tags;
        ChoiceField<std::string>* m_parental_rating;
        StringField* m_cover_image;
        StringField* m_script_file;

        DateTimeField* m_created_at;
        StringField* m_created_by;

        QStringList m_header;
        std::string m_table_name;

    };

}

#endif
