#ifndef VOICEOVER_H
#define VOICEOVER_H

#include "../framework/baseentity.h"

class VoiceExclusion;

class VoiceOver : public BaseEntity
{
public:
    VoiceOver();
    ~VoiceOver() override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;

    BaseEntity* copy() const override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* e) override;

    QList<QStandardItem*> tableViewColumns() override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    StringField* name() const;
    void setName(std::string aName);

    StringField* mobileno() const;
    void setMobileNo(std::string mobileno);

    LookupField* gender() const;
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
    LookupField* mGender;
    VoiceExclusion* mVoiceEx;

    QStringList mHeader;
    std::string mTableName;

};

#endif // VOICEOVER_H
