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

    void set_defaults();

    template<typename T>
    struct Vectored{
        //Vectored():vec{}{}
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
    void break_layout_selected(const QModelIndex& index);
    void close_form();
    void create_breaks();

private:
    Ui::BreakCreateForm *ui;
    std::unique_ptr<EntityDataModel> m_edm_break_layout;
    std::unique_ptr<EntityDataModel> m_edm_break_line;
    bool m_breaks_created{false};
    bool write_breaks_to_db(const std::string sql);
    std::string make_break_sql(QDate from, QDate to);
};

#endif // BREAKCREATEFORM_H
