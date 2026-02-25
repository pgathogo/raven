#ifndef CTSGENERATOR_H
#define CTSGENERATOR_H

#include <QDate>
#include <QDialog>
#include <QJsonObject>

#include "xlsxdocument.h"
#include "xlsxrichstring.h"
#include "xlsxformat.h"

#include "bookedcommercialtvm.h"

class FormatSelectorWidget;
class QTabWidget;
class QDialogButtonBox;

class EntityDataModel;

enum class CTSElementType {Header, Time, Break, Spot, Duration};

struct CTSElement {
    CTSElementType element_type;
    QXlsx::RichString element;
    int header_space{0};
    int footer_space{0};
};

namespace PIXELPLAN {


    class CTSGenerator : public QDialog {

        Q_OBJECT

    public:
        CTSGenerator(QDate, BookedAdverts&, BreakTitles&, QWidget* parent = nullptr);

        QJsonObject toJson() const;
        void fromJson(const QJsonObject& json);

        bool save_to_file(const QString&) const;
        bool load_from_file(const QString&);

        FormatSelectorWidget* header_settings() const { return m_header_widget; }
        FormatSelectorWidget* time_settings()  const { return m_time_widget; }
        FormatSelectorWidget* break_settings()  const { return m_break_widget; }
        FormatSelectorWidget* spot_settings()  const { return m_spot_widget; }
        FormatSelectorWidget* duration_settings()  const { return m_duration_widget; }

    private slots:
        void onSaveClicked();
        void onLoadClicked();
        void onCancelClicked();

    private:
        void setup_ui();

        CTSElement make_cts_element(CTSElementType, const QString&);
        QXlsx::RichString xls_string(FormatSelectorWidget*, const QString&);
        bool generate_cts();

         QDate m_date;
         BookedAdverts m_booked_adverts;
         BreakTitles m_break_titles;
         QTabWidget* m_tabwidget;

         FormatSelectorWidget* m_header_widget;
         FormatSelectorWidget* m_time_widget;
         FormatSelectorWidget* m_break_widget;
         FormatSelectorWidget* m_spot_widget;
         FormatSelectorWidget* m_duration_widget;

         QDialogButtonBox* m_buttonbox;

         std::unique_ptr<EntityDataModel> m_edm_setup;
         std::shared_ptr<RavenSetup> m_setup;

    };



}

#endif
