#ifndef ADVERTMEDIA_H
#define ADVERTMEDIA_H

#include "../../../rave/framework/baseentity.h"

class StringField;
class IntegerField;
class DateTimeField;

template<typename T>
class ChoiceField;

namespace  PIXELPLAN {

     class AdvertMedia : public BaseEntity
    {
    public:
        AdvertMedia();
        ~AdvertMedia() override;

        StringField* title() const;
        StringField* media_file() const;
        StringField* media_path() const;
        IntegerField* duration();
        StringField* aspect_ratio() const;
        StringField* resolution() const;
        ChoiceField<std::string>* rating() const;
        DateTimeField* created_at();
        ForeignKeyField* client();
        ForeignKeyField* spot();

        StringField* src_filepath() const;
        StringField* dest_filepath() const;
        StringField* file_extension() const;
        StringField* dest_path() const;

        void set_title(const std::string);
        void set_media_file(const std::string);
        void set_media_path(const std::string);
        void set_duration(int);
        void set_aspect_ratio(const std::string);
        void set_resolution(const std::string);
        void set_rating(const std::string);
        void set_created_at(QDateTime);
        void set_client(int);
        void set_spot(int);

        void set_src_filepath(const std::string);
        void set_dest_filepath(const std::string);
        void set_file_extension(const std::string);
        void set_dest_path(const std::string);

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

        std::string order_by() const override;


    private:
        StringField* m_title;
        StringField* m_media_file;
        StringField* m_media_path;
        IntegerField* m_duration;
        StringField* m_aspect_ratio;
        StringField* m_resolution;
        ChoiceField<std::string>* m_rating;
        DateTimeField* m_created_at;
        ForeignKeyField* m_client;
        ForeignKeyField* m_spot;

        StringField* m_src_filepath;
        StringField* m_dest_filepath;
        StringField* m_file_extension;
        StringField* m_dest_path;

        QStringList m_header;
        std::string m_table_name;
    };

}


#endif
