#ifndef BREAKLAYOUT_H
#define BREAKLAYOUT_H

#include "../../../rave/framework/baseentity.h"

using Mon = int;
using Tue = int;
using Wed = int;
using Thu = int;
using Fri = int;
using Sat = int;
using Sun = int;

using WeekDayBits = std::tuple<Mon, Tue, Wed, Thu, Fri, Sat, Sun>;

template<typename T>
class ChoiceField;

class BreakLayout : public BaseEntity
{
public:
    BreakLayout();
    ~BreakLayout() override;

    StringField* name() const;
    void setName(const std::string nm);

    TextField* description() const;
    void setDescription(const std::string desc);

    StringField* weekDays() const;
    void setWeekDays(const std::string wkdays);

    ChoiceField<int>* timeInterval();
    void setTimeInterval(int ti);

    ChoiceField<std::string>* break_fill_method();
    void set_break_fill_method(const std::string);

    IntegerField* monBit() const;
    void setMonBit(int bit);

    IntegerField* tueBit() const;
    void setTueBit(int bit);

    IntegerField* wedBit() const;
    void setWedBit(int bit);

    IntegerField* thuBit() const;
    void setThuBit(int bit);

    IntegerField* friBit() const;
    void setFriBit(int bit);

    IntegerField* satBit() const;
    void setSatBit(int bit);

    IntegerField* sunBit() const;
    void setSunBit(int bit);

    WeekDayBits getAccessBit(std::string abit);
    std::string access_bit_tostring();

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

private:
    StringField* mName;
    TextField* mDescription;
    StringField* mWeekDays;
    ChoiceField<int>* mTimeInterval;
    ChoiceField<std::string>* m_break_fill_method;

    IntegerField* mMonBit;
    IntegerField* mTueBit;
    IntegerField* mWedBit;
    IntegerField* mThuBit;
    IntegerField* mFriBit;
    IntegerField* mSatBit;
    IntegerField* mSunBit;

    QStringList mHeader;
    std::string mTableName;
};

#endif // BREAKLAYOUT_H
