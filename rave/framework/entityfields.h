#ifndef ENTITYFIELDS_H
#define ENTITYFIELDS_H

#include<string>
#include<variant>
#include<QVariant>
#include<memory>

using FieldValueType = std::variant<std::string, int>;

class FormField;
class IntegerFormField;
class BaseEntity;
class EntityDataModel;

class QLineEdit;
class QTextEdit;
class QComboBox;

class Field{
    public:
        Field();
        Field(const std::string aName, std::string aLabel);
        virtual ~Field();
        std::string fieldName() const;
        std::string fieldLabel() const;
        std::string dbColumnName() const;
        void setDBColumnName(std::string aName);
        void setName(const std::string aName);
        void setLabel(const std::string aLabel);

        bool visible() const;
        void setVisible(bool value);

        bool formOnly() const;
        void setFormOnly(bool value);

        void setSearchable(bool value);
        bool searchable();

        //void setDisplayName(const std::string dispName);
        virtual std::string displayName() const = 0;

        void setMandatory(bool value);
        bool mandatory() const;

        virtual std::string valueToString()const = 0;
        virtual std::string dbValueFormatter()=0;

        virtual void stringToValue(std::string val)=0;

        //virtual QVariant value()=0;

    private:
        std::string mFieldName;
        std::string mFieldLabel;
        std::string mDBColumnName;
        bool mVisible;
        bool mFormOnly;
        bool mSearchable;
        std::string mDisplayName;
        bool mMandatory;
};

class IntegerField : public Field{
    public:
        typedef int type;
        IntegerField();
        ~IntegerField() override;
        IntegerField(std::string aName, std::string aLabel);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(int val);

        void stringToValue(std::string val) override;

        int value();
        std::string displayName() const override;

        //IntegerFormField* widget();

        //QLineEdit* widget();

    private:
        int mValue;
        //QLineEdit* mWidget;
};

class StringField : public Field{
    public:
        typedef std::string type;

        StringField();
        ~StringField() override;
        StringField(std::string aName, std::string aLabel);

        StringField(const StringField& sf);
        StringField& operator=(const StringField& sf);

        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        std::string value();

        std::string displayName() const override;
    private:
        std::string mValue;
        //QLineEdit* mWidget;
};

class TextField :public Field{
    public:
        typedef std::string type;
        TextField();
        ~TextField() override;
        TextField(std::string aName, std::string aLabel);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(std::string val);
        void stringToValue(std::string val) override;
        std::string value();

        //QTextEdit* widget();
        //void setWidget(QTextEdit* textEdit);
        //void setValueFromWidget();

        std::string displayName() const override;
    private:
        std::string mValue;
        //QTextEdit* mWidget;
};

class LookupField : public Field{
public:
        LookupField();
        ~LookupField() override;
        LookupField(std::string aName, std::string aLabel, EntityDataModel* edm);
        std::string valueToString() const override;
        std::string dbValueFormatter() override;

        void setValue(int val);
        void stringToValue(std::string val) override;
        int value();

        void setIndex(int i);
        int index() const;

        void setCurrText(std::string txt);
        std::string currText() const;

        void cacheData();
        std::size_t cacheCount();
        std::string displayName() const override;

        EntityDataModel* dataModel() const;

    private:
        int mValue;
        int mIndex;
        std::string mCurrText;
        static EntityDataModel* mEDM;
        static bool hasData;
};

/*
using Choice = std::tuple<std::string, std::string>;
class ChoiceField : public Field{
public:
        ChoiceField();
        ChoiceField(std::string aName, std::string aLabel);
        ~ChoiceField() override;

        std::string valueToString() const override;
        std::string dbValueFormatter() override;
        void stringToValue(std::string val) override;
        std::string value();
        std::string displayName() const override;

        void addChoice(Choice choice);
        void setValue(std::string value);
        void setIndex(int i);
        int index() const;

        void setCurrText(std::string text);
        std::string currText() const;

    private:
        //QComboBox* mWidget;
        std::vector<Choice> mChoices;
        std::string mValue;
        int mIndex;
        std::string mCurrText;
};
*/

class ForeignKeyField : public Field {
    public:
        ForeignKeyField(const std::string aName, const std::string aLabel,
                        std::unique_ptr<BaseEntity> fkEntity);
        ~ForeignKeyField() override;

        std::string valueToString() const override;
        std::string dbValueFormatter() override;
        void stringToValue(std::string val) override;
        void setValue(int val);
        int value();

        void cacheData();
        std::size_t cacheCount();
        std::string displayName() const override;
    private:
        std::unique_ptr<BaseEntity> mFKEntity;
        int mValue;
};


using IF = std::tuple<IntegerField, int>;
using SF = std::tuple<StringField, std::string>;

#endif // ENTITYFIELDS_H
