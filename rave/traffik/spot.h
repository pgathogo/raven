#ifndef SPOT_H
#define SPOT_H

#include "../framework/baseentity.h"
#include "typeexclusion.h"

class Client;
class VoiceOver;
class SpotVoiceOver;
class SpotTypeExclusion;

namespace TRAFFIK{
    class SpotAudio;
}

namespace AUDIO{
    class Audio;
}

namespace TRAFFIK {

    class Spot : public BaseEntity
    {
    public:
        Spot();
        Spot(const Client* client);
        ~Spot() override;

        std::string tableName() const override;
        void setTableName(const std::string table_name) override;
        std::unique_ptr<BaseEntity> mapFields(StringMap*) override;

        std::vector<std::string> tableViewColumns() const override;
        std::vector<std::string> tableViewValues() override;
        QStringList tableHeaders() const override;

        std::string searchColumn() const override;
        void populateEntity() override;

        std::unique_ptr<BaseEntity> cloneAsUnique() override;
        void afterMapping(BaseEntity& entity) override;

        StringField* name() const;
        void set_name(const std::string n);
        DecimalField* spot_duration() const;
        void set_spot_duration(double dur);
        DecimalField* real_duration() const;
        void set_real_duration(double dur);
        ForeignKeyField* client() const;
        void set_client(int client_id);
        ForeignKeyField* brand() const;
        void set_brand(int brand_id);

        StringField* daypart1() const;
        void set_daypart1(std::string dp);

        StringField* daypart2() const;
        void set_daypart2(std::string dp);

        StringField* daypart3() const;
        void set_daypart3(std::string dp);

        StringField* daypart4() const;
        void set_daypart4(std::string dp);

        StringField* daypart5() const;
        void set_daypart5(std::string dp);

        StringField* daypart6() const;
        void set_daypart6(std::string dp);

        StringField* daypart7() const;
        void set_daypart7(std::string dp);

        SpotVoiceOver& voice_over();
        SpotTypeExclusion& type_exclusion();

        SpotAudio& spot_audio();

    private:
        StringField* m_name;
        DecimalField* m_spot_duration;
        DecimalField* m_real_duration;
        ForeignKeyField* m_client;
        ForeignKeyField* m_brand;

        StringField* m_daypart1;
        StringField* m_daypart2;
        StringField* m_daypart3;
        StringField* m_daypart4;
        StringField* m_daypart5;
        StringField* m_daypart6;
        StringField* m_daypart7;

        QStringList mHeader;
        std::string mTableName;

        std::unique_ptr<VoiceOver> m_voice_over;
        std::unique_ptr<SpotVoiceOver> m_spot_voice_over;

        std::unique_ptr<TRAFFIK::TypeExclusion> m_type_ex;
        std::unique_ptr<SpotTypeExclusion> m_spot_type_ex;

        std::unique_ptr<AUDIO::Audio> m_audio;
        std::unique_ptr<TRAFFIK::SpotAudio> m_spot_audio;

    };
}

#endif // SPOT_H
