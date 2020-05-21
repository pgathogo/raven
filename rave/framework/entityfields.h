#ifndef ENTITYFIELDS_H
#define ENTITYFIELDS_H

#include<string>
#include<variant>
#include<QVariant>
#include<memory>

using FieldValueType = std::variant<std::string, int>;

class FormField;
class IntegerFormField;
class StringFormField;
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

        virtual QVariant value()=0;

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

        QVariant value() override;
        std::string displayName() const override;

        //IntegerFormField* widget();

        QLineEdit* widget();

    private:
        int mValue;
        QLineEdit* mWidget;
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
        QVariant value() override;

        QLineEdit* widget();
        void setWidget(QLineEdit* lineEdit);
        void setValueFromWidget();

        void printWidgetValue();
        std::string displayName() const override;
    private:
        std::string mValue;
        QLineEdit* mWidget;
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
        QVariant value() override;

        QTextEdit* widget();
        void setWidget(QTextEdit* textEdit);
        void setValueFromWidget();

        std::string displayName() const override;
    private:
        std::string mValue;
        QTextEdit* mWidget;
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
        QVariant value() override;

        void setIndex(int i);
        int index() const;

        void setCurrText(std::string txt);
        std::string currText() const;

        QComboBox* widget();
        void setWidget(QComboBox* textEdit);
        void setValueFromWidget();

        void cacheData();
        std::size_t cacheCount();
        std::string displayName() const override;

    private:
        int mValue;
        int mIndex;
        std::string mCurrText;
        QComboBox* mWidget;
        static EntityDataModel* mEDM;
        static bool hasData;
};


using IF = std::tuple<IntegerField, int>;
using SF = std::tuple<StringField, std::string>;

#endif // ENTITYFIELDS_H
