#ifndef BREAKCREATEFORM_H
#define BREAKCREATEFORM_H

#include <memory>
#include <QDialog>

namespace Ui {
class BreakCreateForm;
}

class EntityDataModel;
class BreakLayoutLine;
class Schedule;

class BreakCreateForm : public QDialog
{
    Q_OBJECT

public:
    explicit BreakCreateForm(QWidget *parent = nullptr);
    ~BreakCreateForm();

    void setDefaults();

    template<typename T>
    struct Vectored{
        Vectored():vec{}{}
        Vectored& operator<<(T* t)
        {
            vec.push_back(t);
            return *this;
        }
        void clear(){
            vec.clear();
        }
        std::vector<T*> vec;
    };

private slots:
    void breakLayoutSelected(const QModelIndex& index);
    void closeForm();
    void createBreaks();

private:
    Ui::BreakCreateForm *ui;
    std::unique_ptr<EntityDataModel> edmBreakLayout;
    std::unique_ptr<EntityDataModel> edmBreakLine;
};

#endif // BREAKCREATEFORM_H
