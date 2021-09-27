#pragma once

#include <QDialog>
#include <QDate>

namespace Ui {
class SaveAs;
}

class QComboBox;

struct Result{
    QDate dest_date;
    std::map<int, int> dest_map;
};

class SaveAs : public QDialog
{
    Q_OBJECT

public:


    explicit SaveAs(const std::vector<int>, QWidget *parent = nullptr);
    ~SaveAs();

    void setup_selection_table();
    void add_combo_items(QComboBox*);

    Result save_result();

public slots:
    void save();
    void cancel();

private:
    std::vector<int> m_selected_hours;
    Ui::SaveAs *ui;

    Result m_result;

};

