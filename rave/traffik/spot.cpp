#include "spot.h"
#include "client.h"
#include "brand.h"
#include "../framework/entitydatamodel.h"

#include "voiceover.h"
#include "spotvoiceover.h"
#include "spottypeexclusion.h"

#include "../audio/audio.h"
#include "spotaudio.h"

namespace TRAFFIK {

    Spot::Spot()
        :m_name{}
        ,m_spot_duration{}
        ,m_real_duration{}
        ,m_client{}
        ,m_brand{}
    {
        m_client = createField<ForeignKeyField>("client_id", "Client",
                                               std::make_unique<Client>(), "name");

        m_name = createField<StringField>("name","Spot Name");
        m_name->setMandatory(true);

        m_spot_duration = createField<DecimalField>("spot_duration", "Spot Duration");
        m_real_duration = createField<DecimalField>("real_duration", "Real Duration");

        m_brand = createField<ForeignKeyField>("brand_id", "Brand",
                                              std::make_unique<TRAFFIK::Brand>(),
                                              "brand_name");

        m_daypart1 = createField<StringField>("daypart1", "Daypart1");
        m_daypart1->setSearchable(false);

        m_daypart2 = createField<StringField>("daypart2", "Daypart2");
        m_daypart2->setSearchable(false);

        m_daypart3 = createField<StringField>("daypart3", "Daypart3");
        m_daypart3->setSearchable(false);

        m_daypart4 = createField<StringField>("daypart4", "Daypart4");
        m_daypart4->setSearchable(false);
        m_daypart5 = createField<StringField>("daypart5", "Daypart5");
        m_daypart5->setSearchable(false);
        m_daypart6 = createField<StringField>("daypart6", "Daypart6");
        m_daypart6->setSearchable(false);
        m_daypart7 = createField<StringField>("daypart7", "Daypart6");
        m_daypart7->setSearchable(false);

        mHeader << QString::fromStdString(m_name->fieldLabel())
                 <<QString::fromStdString(m_spot_duration->fieldLabel());

        m_voice_over = std::make_unique<VoiceOver>();
        m_spot_voice_over = std::make_unique<SpotVoiceOver>(this, m_voice_over.get());

        m_type_ex = std::make_unique<TRAFFIK::TypeExclusion>();
        m_spot_type_ex = std::make_unique<SpotTypeExclusion>(this, m_type_ex.get() );

        m_audio = std::make_unique<AUDIO::Audio>("");
        m_spot_audio = std::make_unique<SpotAudio>(this, m_audio.get());

        setTableName("rave_spot");
    }

    Spot::Spot(const Client* client)
        :m_name{}
        ,m_spot_duration{}
        ,m_real_duration{}
        ,m_client{}
        ,m_brand{}
    {
        m_client = createField<ForeignKeyField>("client_id", "Client",
                                               std::make_unique<Client>(), "name");
        m_client->setValue(client->id());

        m_name = createField<StringField>("name","Spot Name");
        m_name->setMandatory(true);

        m_spot_duration = createField<DecimalField>("spot_duration", "Spot Duration");
        m_real_duration = createField<DecimalField>("real_duration", "Real Duration");

        EntityDataModel edm;
        auto filter = std::make_tuple("client_id", "=", client->id());
        std::string fstr = edm.prepareFilter(filter);

        m_brand = createField<ForeignKeyField>("brand_id", "Brand",
                                              std::make_unique<TRAFFIK::Brand>(),
                                              "brand_name",
                                              fstr);

        m_daypart1 = createField<StringField>("daypart1", "Daypart1");
        m_daypart1->setSearchable(false);

        m_daypart2 = createField<StringField>("daypart2", "Daypart2");
        m_daypart2->setSearchable(false);

        m_daypart3 = createField<StringField>("daypart3", "Daypart3");
        m_daypart3->setSearchable(false);

        m_daypart4 = createField<StringField>("daypart4", "Daypart4");
        m_daypart4->setSearchable(false);
        m_daypart5 = createField<StringField>("daypart5", "Daypart5");
        m_daypart5->setSearchable(false);
        m_daypart6 = createField<StringField>("daypart6", "Daypart6");
        m_daypart6->setSearchable(false);
        m_daypart7 = createField<StringField>("daypart7", "Daypart6");
        m_daypart7->setSearchable(false);

        mHeader << QString::fromStdString(m_name->fieldLabel())
                 <<QString::fromStdString(m_spot_duration->fieldLabel());

        m_voice_over = std::make_unique<VoiceOver>();
        m_spot_voice_over = std::make_unique<SpotVoiceOver>(this, m_voice_over.get());

        m_type_ex = std::make_unique<TRAFFIK::TypeExclusion>();
        m_spot_type_ex = std::make_unique<SpotTypeExclusion>(this, m_type_ex.get() );

//        m_audio = std::make_unique<AUDIO::Audio>("");
//        m_spot_audio = std::make_unique<SpotAudio>(this, m_audio.get());

        setTableName("rave_spot");

    }

    Spot::~Spot()
    {
    }

    std::string Spot::tableName() const
    {
        return mTableName;
    }

    void Spot::setTableName(const std::string table_name)
    {
        mTableName = table_name;
    }

    std::unique_ptr<BaseEntity> Spot::mapFields(StringMap*)
    {
    }

    std::vector<std::string> Spot::tableViewColumns() const
    {
        std::list<std::string> cols;

        return tableViewCols<std::string>(
                    name()->displayName(),
                    spot_duration()->displayName()
                    );
    }

    std::vector<std::string> Spot::tableViewValues()
    {
        return  tableViewColumns();
    }

    QStringList Spot::tableHeaders() const
    {
        return mHeader;
    }

    std::string Spot::searchColumn() const
    {
        return name()->valueToString();
    }

    void Spot::populateEntity()
    {
    }

    std::unique_ptr<BaseEntity> Spot::cloneAsUnique()
    {
        qDebug() << "<<< Spot::cloneAsUnique >>>" ;
        return std::make_unique<Spot>();
    }

    void Spot::afterMapping(BaseEntity &entity)
    {
        m_spot_voice_over->setParentId(entity.id());
        m_spot_type_ex->setParentId(entity.id());
        m_spot_audio->setParentId(entity.id());
    }

    StringField* Spot::name() const
    {
        return m_name;
    }

    void Spot::set_name(const std::string n)
    {
        m_name->setValue(n);
    }

    DecimalField* Spot::spot_duration() const
    {
        return m_spot_duration;
    }

    void Spot::set_spot_duration(double dur)
    {
        m_spot_duration->setValue(dur);
    }

    DecimalField* Spot::real_duration() const
    {
        return m_real_duration;
    }

    void Spot::set_real_duration(double dur)
    {
        m_real_duration->setValue(dur);
    }

    ForeignKeyField* Spot::client() const
    {
        return m_client;
    }

    void Spot::set_client(int client_id)
    {
        m_client->setValue( client_id );
    }

    ForeignKeyField* Spot::brand() const
    {
        return m_brand;
    }

    void Spot::set_brand(int brand_id)
    {
        m_brand->setValue( brand_id );
    }


    StringField* Spot::daypart1() const
    {
        return m_daypart1;
    }
    void Spot::set_daypart1(std::string dp)
    {
        m_daypart1->setValue(dp);
    }

    StringField* Spot::daypart2() const
    {
        return m_daypart2;
    }
    void Spot::set_daypart2(std::string dp)
    {
        m_daypart2->setValue(dp);
    }

    StringField* Spot::daypart3() const
    {
        return m_daypart3;
    }
    void Spot::set_daypart3(std::string dp)
    {
        m_daypart3->setValue(dp);
    }

    StringField* Spot::daypart4() const
    {
        return m_daypart4;
    }
    void Spot::set_daypart4(std::string dp)
    {
        m_daypart4->setValue(dp);
    }

    StringField* Spot::daypart5() const
    {
        return m_daypart5;
    }
    void Spot::set_daypart5(std::string dp)
    {
        m_daypart5->setValue(dp);
    }

    StringField* Spot::daypart6() const
    {
        return m_daypart6;
    }
    void Spot::set_daypart6(std::string dp)
    {
        m_daypart6->setValue(dp);
    }

    StringField* Spot::daypart7() const
    {
        return m_daypart7;
    }
    void Spot::set_daypart7(std::string dp)
    {
        m_daypart7->setValue(dp);
    }

    SpotVoiceOver& Spot::voice_over()
    {
        return *m_spot_voice_over;
    }

    SpotTypeExclusion& Spot::type_exclusion()
    {
        return *m_spot_type_ex;
    }

    SpotAudio& Spot::spot_audio()
    {
        return *m_spot_audio;
    }

}
