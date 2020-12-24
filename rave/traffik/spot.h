#ifndef SPOT_H
#define SPOT_H

#include "../framework/baseentity.h"

class Client;
class VoiceOver;
class SpotVoiceOver;
class TypeExclusion;
class SpotTypeExclusion;

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
    void setName(const std::string n);
    DecimalField* spotDuration() const;
    void setSpotDuration(double dur);
    DecimalField* realDuration() const;
    void setRealDuration(double dur);
    ForeignKeyField* client() const;
    void setClient(int client_id);
    ForeignKeyField* brand() const;
    void setBrand(int brand_id);

    StringField* daypart1() const;
    void setDaypart1(std::string dp);

    StringField* daypart2() const;
    void setDaypart2(std::string dp);

    StringField* daypart3() const;
    void setDaypart3(std::string dp);

    StringField* daypart4() const;
    void setDaypart4(std::string dp);

    StringField* daypart5() const;
    void setDaypart5(std::string dp);

    StringField* daypart6() const;
    void setDaypart6(std::string dp);

    StringField* daypart7() const;
    void setDaypart7(std::string dp);

    SpotVoiceOver& spotVoiceOver();
    SpotTypeExclusion& spotTypeExclusion();

private:
    StringField* m_name;
    DecimalField* mSpotDuration;
    DecimalField* mRealDuration;
    ForeignKeyField* m_client;
    ForeignKeyField* mBrand;

    StringField* mDaypart1;
    StringField* mDaypart2;
    StringField* mDaypart3;
    StringField* mDaypart4;
    StringField* mDaypart5;
    StringField* mDaypart6;
    StringField* mDaypart7;

    QStringList mHeader;
    std::string mTableName;

    std::unique_ptr<VoiceOver> mVoiceOver;
    std::unique_ptr<SpotVoiceOver> mSpotVoiceOver;

    std::unique_ptr<TypeExclusion> mTypeEx;
    std::unique_ptr<SpotTypeExclusion> mSpotTypeEx;


};

#endif // SPOT_H
