#pragma once

#include <QDialog>
#include "viewbreaksmodel.h"

class QMdiArea;
class QTreeView;
class QDateEdit;

class EntityDataModel;
class ViewBreaksModel;

namespace PIXELPLAN
{

class ViewBreaksForm : public QDialog
{
    Q_OBJECT

public:
    explicit ViewBreaksForm(QWidget* parent = nullptr);

    void view_breaks(const QDate&, const QDate&);
    void setMdiArea(QMdiArea*);

private slots:
    void on_view_breaks(bool);
    void on_expand_all(Qt::CheckState);
private:
    void setup_ui();
    void prepare_tree(QTreeView*);
    void build_tree_view(OrderedMap&);

    std::unique_ptr<EntityDataModel> m_edm_breaks;
    QMdiArea* m_mdi_area;
    QTreeView* m_breaks;
    ViewBreaksModel* m_breaks_model;

    QDateEdit* m_dtFrom;
    QDateEdit* m_dtTo;

};






}