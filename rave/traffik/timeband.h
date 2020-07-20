#ifndef TIMEBAND_H
#define TIMEBAND_H

#include "../framework/baseentity.h"

class StringField;
class BooleanField;

template<typename T>
class ChoiceField;

class TimeBand : public BaseEntity
{
public:
    TimeBand();
    ~TimeBand() override;

    std::string tableName() const override;
    void setTableName(const std::string table_name) override;
    std::unique_ptr<BaseEntity> mapFields(StringMap* raw_entity) override;

    std::list<std::string> tableViewColumns() const override;
    std::vector<std::string> tableViewValues() override;
    QStringList tableHeaders() const override;

    std::string searchColumn() const override;
    void populateEntity() override;

    std::unique_ptr<BaseEntity> cloneAsUnique() override;
    void afterMapping(BaseEntity& entity) override;

    StringField* name() const;
    void setName(std::string val);

    BooleanField* locked();
    void setLocked(bool val);

    ChoiceField<std::string>* billMethod();
    void setBillMethod(const std::string val);

    IntegerField* baseDuration() const;
    void setBaseDuration(int val);
    DecimalField* basePrice() const;
    void setBasePrice(double val);
    StringField* daypart1() const;
    void setDaypart1(std::string val);
    StringField* daypart2() const;
    void setDaypart2(std::string val);
    StringField* daypart3() const;
    void setDaypart3(std::string val);
    StringField* daypart4() const;
    void setDaypart4(std::string val);
    StringField* daypart5() const;
    void setDaypart5(std::string val);
    StringField* daypart6() const;
    void setDaypart6(std::string val);
    StringField* daypart7() const;
    void setDaypart7(std::string val);
    IntegerField* iPDuration1() const;
    void setIPDuration1(int val);
    IntegerField* iPDuration10() const;
    void setIPDuration10(int val);
    IntegerField* iPDuration11() const;
    void setIPDuration11(int val);
    IntegerField* iPDuration12() const;
    void setIPDuration12(int val);
    IntegerField* iPDuration2() const;
    void setIPDuration2(int val);
    IntegerField* iPDuration3() const;
    void setIPDuration3(int val);
    IntegerField* iPDuration4() const;
    void setIPDuration4(int val);
    IntegerField* iPDuration5() const;
    void setIPDuration5(int val);
    IntegerField* iPDuration6() const;
    void setIPDuration6(int val);
    IntegerField* iPDuration7() const;
    void setIPDuration7(int val);
    IntegerField* iPDuration8() const;
    void setIPDuration8(int val);
    IntegerField* iPDuration9() const;
    void setIPDuration9(int val);
    IntegerField* percent1() const;
    void setPercent1(int val);
    IntegerField* percent10() const;
    void setPercent10(int val);
    IntegerField* percent11() const;
    void setPercent11(int val);
    IntegerField* percent12() const;
    void setPercent12(int val);
    IntegerField* percent2() const;
    void setPercent2(int val);
    IntegerField* percent3() const;
    void setPercent3(int val);
    IntegerField* percent4() const;
    void setPercent4(int val);
    IntegerField* percent5() const;
    void setPercent5(int val);
    IntegerField* percent6() const;
    void setPercent6(int val);
    IntegerField* percent7() const;
    void setPercent7(int val);
    IntegerField* percent8() const;
    void setPercent8(int val);
    IntegerField* percent9() const;
    void setPercent9(int val);
private:
    StringField* mName;
    BooleanField* mLocked;
    ChoiceField<std::string>* mBillMethod;

    DecimalField* mBasePrice;
    IntegerField* mBaseDuration;
    IntegerField* mIPDuration1;
    IntegerField* mPercent1;
    IntegerField* mIPDuration2;
    IntegerField* mPercent2;
    IntegerField* mIPDuration3;
    IntegerField* mPercent3;
    IntegerField* mIPDuration4;
    IntegerField* mPercent4;
    IntegerField* mIPDuration5;
    IntegerField* mPercent5;
    IntegerField* mIPDuration6;
    IntegerField* mPercent6;
    IntegerField* mIPDuration7;
    IntegerField* mPercent7;
    IntegerField* mIPDuration8;
    IntegerField* mPercent8;
    IntegerField* mIPDuration9;
    IntegerField* mPercent9;
    IntegerField* mIPDuration10;
    IntegerField* mPercent10;
    IntegerField* mIPDuration11;
    IntegerField* mPercent11;
    IntegerField* mIPDuration12;
    IntegerField* mPercent12;
    StringField* mDaypart1;
    StringField* mDaypart2;
    StringField* mDaypart3;
    StringField* mDaypart4;
    StringField* mDaypart5;
    StringField* mDaypart6;
    StringField* mDaypart7;

    QStringList mHeader;
    std::string mTableName;
};

#endif // TIMEBAND_H
