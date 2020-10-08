#ifndef VOICEOVER_H
#define VOICEOVER_H

#include "../framework/baseentity.h"

class VoiceExclusion;
class TypeExclusion;

class VoiceOver : public BaseEntity
{
public:
    VoiceOver();
    ~VoiceOver() override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    std::vector<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

    StringField* name() const;
    void setName(std::string aName);

    StringField* mobileno() const;
    void setMobileNo(std::string mobileno);

    ForeignKeyField* gender() const;
    void setGender( int i);

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

    VoiceExclusion& voiceEx();

private:
    StringField* mName;
    StringField* mMobileNo;

    StringField* mDaypart1;
    StringField* mDaypart2;
    StringField* mDaypart3;
    StringField* mDaypart4;
    StringField* mDaypart5;
    StringField* mDaypart6;
    StringField* mDaypart7;
    ForeignKeyField* mGender;

    std::unique_ptr<VoiceExclusion> mVoiceEx;
    std::unique_ptr<TypeExclusion> mTypeEx;

    QStringList mHeader;
    std::string mTableName;
};

#endif // VOICEOVER_H
