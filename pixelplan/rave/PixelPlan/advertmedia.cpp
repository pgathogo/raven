#include <QFileInfo>

#include "advertmedia.h"
#include "client.h"

#include "../../../rave/framework/choicefield.h"
#include "../../../rave/framework/entitydatamodel.h"

#include "spot.h"

namespace PIXELPLAN
{

    AdvertMedia::AdvertMedia()
    {

        m_title = createField<StringField>("title","title");
        m_media_file = createField<StringField>("media_file","media_file");
        m_media_path = createField<StringField>("media_path","media path");
        m_duration = createField<IntegerField>("duration","duration");
        m_aspect_ratio = createField<StringField>("aspect_ratio","aspect_ratio");
        m_resolution = createField<StringField>("resolution","resolution");


        m_rating = createField<ChoiceField<std::string>>("rating","rating");
        m_rating->addChoice({"G", "General Audience"});
        m_rating->addChoice({"PG", "Parental Guidance"});
        m_rating->addChoice({"PG-13", "Parents Strongly Cautioned"});
        m_rating->addChoice({"R", "Restricted"});
        m_rating->addChoice({"NC-17", "Adults Only"});


        m_created_at = createField<DateTimeField>("created_at","created_at");

        m_client = createField<ForeignKeyField>("client_id", "Client",
                                                std::make_unique<Client>(), "name");

        m_spot = createField<ForeignKeyField>("spot_id", "Spot",
                                              std::make_unique<TRAFFIK::Spot>(), "name");


        m_src_filepath = createField<StringField>("src_filepath", "Src Filepath");
        m_src_filepath->setFormOnly(true);

        m_dest_filepath = createField<StringField>("dest_filepath", "Dest Filepath");
        m_dest_filepath->setFormOnly(true);

        m_dest_path = createField<StringField>("dest_path", "Dest Path");
        m_dest_path->setFormOnly(true);

        m_file_extension = createField<StringField>("file_extension", "File Extension");

        m_header << QString::fromStdString(m_title->fieldLabel());


        setTableName("rave_advertmedia");
    }


    AdvertMedia::~AdvertMedia()
    {

    }

    StringField* AdvertMedia::title() const
    {
        return m_title;
    }

    StringField* AdvertMedia::media_file() const
    {
        return m_media_file;
    }

    StringField* AdvertMedia::media_path() const
    {
        return m_media_path;
    }

    IntegerField* AdvertMedia::duration()
    {
        return m_duration;
    }

    StringField* AdvertMedia::aspect_ratio() const
    {
        return m_aspect_ratio;
    }

    StringField* AdvertMedia::resolution() const
    {
        return m_resolution;
    }

    ChoiceField<std::string>* AdvertMedia::rating() const
    {
        return m_rating;
    }

    DateTimeField* AdvertMedia::created_at()
    {
        return m_created_at;
    }

    ForeignKeyField* AdvertMedia::client()
    {
        return m_client;
    }

    ForeignKeyField* AdvertMedia::spot()
    {
        return m_spot;
    }

    void AdvertMedia::set_title(const std::string title)
    {
        m_title->setValue(title);
    }

    void AdvertMedia::set_media_file(const std::string filename)
    {
        m_media_file->setValue(filename);
    }

    void AdvertMedia::set_media_path(const std::string path)
    {
        m_media_path->setValue(path);
    }

    void AdvertMedia::set_duration(int dur)
    {
        m_duration->setValue(dur);
    }

    void AdvertMedia::set_aspect_ratio(const std::string aratio)
    {
        m_aspect_ratio->setValue(aratio);
    }

    void AdvertMedia::set_resolution(const std::string res)
    {
        m_resolution->setValue(res);
    }

    void AdvertMedia::set_rating(const std::string rate)
    {
        m_rating->setValue(rate);
    }

    void AdvertMedia::set_created_at(QDateTime cdate)
    {
        m_created_at->setValue(cdate);
    }

    void AdvertMedia::set_client(int client_id)
    {
        m_client->setValue(client_id);
    }

    void AdvertMedia::set_spot(int spot_id)
    {
        m_spot->setValue(spot_id);
    }

    StringField* AdvertMedia::src_filepath() const
    {
        return m_src_filepath;
    }

    StringField* AdvertMedia::dest_filepath() const
    {
        return m_dest_filepath;
    }

    void AdvertMedia::set_src_filepath(const std::string o_filepath)
    {
        m_src_filepath->setValue(o_filepath);
    }

    void AdvertMedia::set_dest_filepath(const std::string d_filepath)
    {
        m_dest_filepath->setValue(d_filepath);
    }

    StringField* AdvertMedia::file_extension() const
    {
        return m_file_extension;
    }

    void AdvertMedia::set_file_extension(const std::string ext)
    {
        m_file_extension->setValue(ext);
    }

    StringField* AdvertMedia::dest_path() const
    {
        return m_dest_path;
    }

    void AdvertMedia::set_dest_path(const std::string dpath)
    {
         m_dest_path->setValue(dpath);

    }


    std::string AdvertMedia::tableName() const
    {
        return m_table_name;
    }

    void AdvertMedia::setTableName(const std::string table_name)
    {
        m_table_name = table_name;
    }
    std::unique_ptr<BaseEntity> AdvertMedia::mapFields(StringMap* raw_entity)
    {
        return nullptr;
    }

    std::vector<std::string> AdvertMedia::tableViewColumns() const
    {
        title()->set_display_width(300);
        return tableViewCols<std::string>(
            title()->displayName() );
    }

    std::vector<std::string> AdvertMedia::tableViewValues()
    {
        return { title()->displayName()};

    }

    QStringList AdvertMedia::tableHeaders() const
    {
        return m_header;
    }

    std::string AdvertMedia::searchColumn() const
    {
        return title()->valueToString();
    }

    void AdvertMedia::populateEntity()
    {}

    std::shared_ptr<BaseEntity> AdvertMedia::cloneAsShared()
    {
        return std::make_shared<AdvertMedia>();
    }

    void AdvertMedia::afterMapping(BaseEntity& entity)
    {

    }

    ActionResult AdvertMedia::validate()
    {
        if (m_title->value() == "") {
            return std::make_tuple(ActionResultType::arERROR,
                                   "Media title is empty!");
        }

        // check if the filename already exists in the media store!
        if (QFileInfo::exists(QString::fromStdString(m_dest_filepath->value()))) {
            return std::make_tuple(ActionResultType::arERROR,
                                   "File with the same name already exists!");
        }

        return BaseEntity::validate();
    }

    std::string AdvertMedia::order_by() const
    {
        return "id DESC";
    }

 }
