#pragma once

#include <QDate>
#include <QDialog>
#include <QComboBox>
#include <QString>

class QDialogButtonBox;
class QLabel;
class QLineEdit;
class QPlainTextEdit;
class QPushButton;
class QSpinBox;

template<typename T>
class ChoiceField;

class PickListBrowser;

namespace AUDIO
{
   class Audio;
class AudioWaveWidget;

    class AudioForm : public QDialog
    {
        Q_OBJECT

    public:
        enum ArtistColumn{FirstName=0, LastName, FullName, ArtistType, Notes};

        explicit AudioForm(std::shared_ptr<AUDIO::Audio>, QWidget *parent = nullptr);

        // Editable fields
        QString title() const;
        void setTitle(const QString &title);

        QString artist() const;
        void setArtist(const QString &artist);

        QString trackClass() const;
        void setTrackClass(const QString &trackClass);
        void setClasses(const QStringList &classes);

        QString genre() const;
        void setGenre(const QString &genre);
        void setGenres(const QStringList &genres);

        int year() const;
        void setYear(int year);

        QString notes() const;
        void setNotes(const QString &notes);

        // Read-only info shown on the right
        void setFileName(const QString &fileName);
        void setFolder(const QString &folder);
        void setCreationDate(const QDate &date);

        void accept() override;

        template<typename T>
        void set_choice_field(T* cf, QComboBox* cbox)
        {
            cf->setValue(cbox->itemData(cbox->currentIndex()).toString().toStdString());
        }


    public slots:
        // void selectArtistRequested();
        void pick_artist();
        void genre_combo_changed(int);
        void show_markers();

    private:
        void setupUi();
        static QLabel *createValueLabel(const QString &text = QString());
        void populate_widgets();
        void populate_choice_combo(QComboBox*, const ChoiceField<std::string>*);
        int item_index(QComboBox*, const QString);
        void set_choice_field_default(QComboBox* cbox, const std::string value);
        QString get_audio_file();

        QLineEdit *m_titleEdit = nullptr;
        QLineEdit *m_artistEdit = nullptr;
        QPushButton *m_selectArtistButton = nullptr;
        QComboBox *m_classCombo = nullptr;
        QComboBox *m_genreCombo = nullptr;
        QSpinBox *m_yearSpin = nullptr;
        QPlainTextEdit *m_notesEdit = nullptr;

        QLabel *m_fileNameValue = nullptr;
        QLabel *m_folderValue = nullptr;
        QLabel *m_creationDateValue = nullptr;

        QDialogButtonBox *m_buttonBox = nullptr;

        std::shared_ptr<AUDIO::Audio> m_audio;
        std::unique_ptr<PickListBrowser> m_artist_picker;
        AudioWaveWidget* m_audio_wave_widget;
    };
}