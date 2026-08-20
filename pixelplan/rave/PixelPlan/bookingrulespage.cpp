#include <algorithm>

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGridLayout>
#include <QLabel>
#include <QFont>

#include "../../../rave/utils/tools.h"
#include "../../../rave/utils/togglebutton.h"

#include "bookingrulespage.h"
#include "traffikrules.h"

namespace PIXELPLAN
{

BookingRulesPage::BookingRulesPage(OrderBookingWizard* wiz)
    :m_wizard(wiz)
{
    setTitle("Booking Rules");
    setSubTitle("Choose rules to apply when making the booking");

    set_rule_config();
    setup_ui();

}

void BookingRulesPage::setup_ui()
{
    int BTN_WIDTH  = 10;
    int BTN_HEIGHT = 12;

    QVBoxLayout* main_layout = new QVBoxLayout();

    QHBoxLayout* hl_all_rules = new QHBoxLayout();
    QLabel* lbl_sel_all = new QLabel("Select All Rules");
    ToggleButton* tb_sel_all = new ToggleButton(BTN_WIDTH, BTN_HEIGHT);
    hl_all_rules->addWidget(lbl_sel_all);
    hl_all_rules->addWidget(tb_sel_all);
    hl_all_rules->addStretch();

    //--- Horizontal line
    main_layout->addLayout(hl_all_rules);
    auto top_hline = horizontal_line(this);
    main_layout->addWidget(top_hline);

    // -- Rules Grid -----
    QGridLayout* gl_rules = new QGridLayout();
    gl_rules->setVerticalSpacing(7);

    //for(int row=0; row <= RULE_COUNT-1; ++row) {
    int row = 0;
    for(auto& rule_config : m_rules_config) {
        QLabel* lbl = new QLabel(QString("%1.").arg(QString::number(row+1)));

        QFont font = lbl->font();
        font.setBold(true);
        lbl->setFont(font);

        ToggleButton* tb = new ToggleButton(BTN_WIDTH, BTN_HEIGHT);

        //QLabel* desc = new QLabel(rule_desc.at(row));

        QLabel* desc = new QLabel(rule_config.desc);
        gl_rules->addWidget(lbl, row, 0);
        gl_rules->addWidget(tb, row, 1, Qt::AlignLeft);
        gl_rules->addWidget(desc, row, 2, Qt::AlignLeft);
        gl_rules->setColumnStretch(2, 1);

        gl_rules->setRowMinimumHeight(row, 40);

        rule_config.btn = tb;

        row++;
    }

    main_layout->addLayout(gl_rules);

    // --- Horizontal line
    main_layout->addLayout(hl_all_rules);
    auto bottom_hline = horizontal_line(this);
    main_layout->addWidget(bottom_hline);

    // --- Save Rules
    QHBoxLayout* hl_save_rules = new QHBoxLayout();
    QLabel* lbl_save = new QLabel("Remember selected break rules");
    ToggleButton* tb_save_rules = new ToggleButton(BTN_WIDTH, BTN_HEIGHT);

    hl_save_rules->addWidget(lbl_save);
    hl_save_rules->addWidget(tb_save_rules);
    hl_save_rules->addStretch();

    main_layout->addLayout(hl_save_rules);

    setLayout(main_layout);

}

void BookingRulesPage::init_rules_state()
{
    auto rule_state = [&](const QString rule_name)->bool{

        auto it = std::find_if(m_rules_config.begin(),
                               m_rules_config.end(), [rule_name](const RuleConfig& rc) {
                                   return rc.name == rule_name;
                               });
        if (it != m_rules_config.end()) {
            return it->btn->isChecked();
        } else {
            return false;
        }
    };

    PIXELPLAN::FullBreakRule::enable_or_disable( rule_state("break_duration"));
    PIXELPLAN::TypeExclusionRule::enable_or_disable(rule_state("type_excl"));
    PIXELPLAN::VoiceExclusionRule::enable_or_disable(rule_state("voice_excl"));
    PIXELPLAN::TypeDaypartRule::enable_or_disable(rule_state("type_daypart"));
    PIXELPLAN::VoiceDaypartRule::enable_or_disable(rule_state("voice_daypart"));
    PIXELPLAN::SpotDaypartRule::enable_or_disable(rule_state("spot_daypart"));
    PIXELPLAN::SameClientRule::enable_or_disable(rule_state("same_client"));
    PIXELPLAN::OverrideSameClientRule::enable_or_disable(rule_state("diff_brand"));

}
void BookingRulesPage::set_rule_config()
{

    RuleConfig rc1{"break_duration","Enforce Break Duration"};
    RuleConfig rc2{"type_excl","Enforce Type Exclusion Rule"};
    RuleConfig rc3{"voice_excl","Enforce Voice Exclusion Rule"};
    RuleConfig rc4{"type_daypart","Enforce Type Daypart Rule"};
    RuleConfig rc5{"voice_daypart","Enforce Voice Daypart Rule"};
    RuleConfig rc6{"spot_daypart", "Enforce Spot Daypart Rule"};
    RuleConfig rc7{"same_client","Enforce No Same Client Rule"};
    RuleConfig rc8{"diff_brand","Override Same Client Rule Different Brands"};

    m_rules_config.push_back(rc1);
    m_rules_config.push_back(rc2);
    m_rules_config.push_back(rc3);
    m_rules_config.push_back(rc4);
    m_rules_config.push_back(rc5);
    m_rules_config.push_back(rc6);
    m_rules_config.push_back(rc7);
    m_rules_config.push_back(rc8);

}

} // namespace