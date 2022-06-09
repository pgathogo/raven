#include <filesystem>
#include <QFile>

#include <QDebug>

#include "../framework/ravenexception.h"
#include "../framework/ravensetup.h"
#include "../framework/entitydatamodel.h"
#include "../framework/choicefield.h"
#include "../utils/tools.h"

#include "audioimporter.h"

#include "audio.h"
#include "artist.h"
#include "audiotool.h"
//#include "../audiolib/headers/cueeditor.h"
#include "../audiolib/headers/audioplayer.h"

namespace fs = std::filesystem;

namespace AUDIO{

    AudioImporter::AudioImporter(AUDIO::Audio* audio, QApplication* qapp)
        :m_audio{audio}
        ,m_qapp{qapp}
    {
        m_setup_edm = std::make_unique<EntityDataModel>(std::make_unique<RavenSetup>());
        m_setup_edm->all();
        m_setup = dynamic_cast<RavenSetup*>(m_setup_edm->firstEntity());

        m_audio_file = m_audio->audio_file();
        m_audio_file.set_creation_date(m_audio->creation_date()->value().toString("dd/MM/yyyy").toStdString());
        m_audio_file.set_audio_class(m_audio->audio_type()->displayName());
        m_audio_file.set_genre(m_audio->genre()->displayName());
        m_audio_file.set_year(m_audio->audio_year()->value());

        auto fk_artist = m_audio->artist()->unique_fk_entity();
        if (fk_artist != nullptr) {
            AUDIO::Artist* artist = dynamic_cast<AUDIO::Artist*>(fk_artist);
            m_audio_file.set_artist_name(artist->fullName()->value());
        }
    }

    void AudioImporter::create_audio_wave_file()
    {
         if (fs::exists(m_audio->audio_file().wave_file())){
             fs::path file_path(m_audio->audio_file().wave_file());
             fs::remove(file_path);
         }

         AudioTool audio_tool;
         if ( !audio_tool.generate_wave_file(
                  m_audio->audio_file().audio_file(),
                  m_audio->audio_file().wave_file() ) ) {
             throw AudioImportException("GEN_WAVE_FILE", "Error generating audio wave file.");
         }

        m_audio->set_audio_lib_path(m_setup->comm_audio_folder()->value());
        m_audio->set_file_path(m_setup->comm_audio_folder()->value());

    }

    void AudioImporter::import_audio()
    {
        create_audio_wave_file();
        open_cue_editor();
    }

    AudioFile AudioImporter::audio_file()
    {
        return m_audio_file;
    }

    void AudioImporter::open_cue_editor()
    {
        auto af = m_audio->audio_file();
        //af.set_ogg_filename(m_setup->comm_audio_folder()->value()+af.short_filename()+".ogg");

        auto cue_editor = std::make_unique<CueEditor>(af, m_qapp);
        if (cue_editor->editor() == 1) {
            m_audio->set_audio_file(af);
            //m_audio->set_marker(cue_editor->marker());
            m_marker = cue_editor->cue_marker();

            m_audio->set_duration(m_audio->audio_file().duration());

            m_audio_file.set_duration(m_audio->audio_file().duration());

            m_audio->setDBAction(DBAction::dbaCREATE);

        }
    }

    void AudioImporter::write_ogg_file(int audio_id)
    {
        AudioTool at;
        auto ogg_file = at.make_audio_filename(audio_id)+".ogg";
        auto dst_file = m_setup->audio_folder()->value()+ogg_file;

        try{
            at.copy_ogg_to_audiolib(m_audio_file.ogg_filename(), dst_file);
        } catch (fs::filesystem_error& fe) {
            throw fe;
        }
    }

    void AudioImporter::write_wave_file(int audio_id)
    {
        AudioTool at;
        auto wave_file = at.make_audio_filename(audio_id)+".png";

        auto dest_file = m_setup->audio_folder()->value()+wave_file;
        try{
            at.copy_wave_to_audiolib(m_audio_file.wave_filename(), dest_file);
        } catch(fs::filesystem_error& fe) {
        }

    }

    void AudioImporter::create_adf_file(int audio_id)
    {
        //qDebug() << "ADF File: "<< stoq(m_audio_file.adf_file());

        AudioTool at;
        auto ogg = at.make_audio_filename(audio_id);
        auto adf = ogg+".adf";
        auto ogg_file = ogg+".ogg";
        auto adf_file = m_setup->audio_folder()->value()+adf;

        m_audio_file.set_adf_file(adf_file);
        m_audio_file.set_audio_lib_path(m_setup->audio_folder()->value());
        m_audio_file.set_ogg_short_filename(ogg_file);
        m_audio_file.set_marker(m_marker);

        ADFRepository adf_repo;
        adf_repo.write(m_audio_file);

    }

    void AudioImporter::remove_ogg_temp_file()
    {
        qDebug() << stoq(m_audio_file.ogg_filename());
        QFile ogg_file(QString::fromStdString(m_audio_file.ogg_filename()));
        if (ogg_file.exists())
            ogg_file.remove();

    }

    void AudioImporter::remove_wave_temp_file()
    {
        QFile wave_file(QString::fromStdString(m_audio_file.wave_filename()));
        if (wave_file.exists())
            wave_file.remove();
    }

    CueMarker AudioImporter::marker()
    {
        return m_marker;
    }

}
