#ifndef SPOTAUDIOFORM_H
#define SPOTAUDIOFORM_H

#include <QDialog>
#include "../../../rave/framework/baseentitydetaildlg.h"
#include "spotaudio.h"

namespace Ui {
class SpotAudioForm;
}

class SpotAudioForm : public BaseEntityDetailDlg
{
    Q_OBJECT

public:
    explicit SpotAudioForm(TRAFFIK::SpotAudio* spot_audio, QDialog* parent = nullptr);
    ~SpotAudioForm() override;

    ActionResult saveRecord() override;
    std::string windowTitle() override;
    void populateEntityFields() override;
    void populateFormWidgets() override;

private:
    Ui::SpotAudioForm *ui;
    TRAFFIK::SpotAudio* m_spot_audio;
};

#endif // SPOTAUDIOFORM_H
