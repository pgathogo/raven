#include <QFile>
#include <QFileInfo>

#include "playlist.h"

namespace PIXELPLAN {

PlaylistItem::PlaylistItem(const QString name)
    : m_item_name{name}
{}


Playlist::Playlist(const QString input_filepath, const QString output_filepath)
    :m_playlist_file{input_filepath}
    ,m_output_filepath{output_filepath}
    ,m_dom_document{nullptr}
{
    m_dom_document = std::make_unique<QDomDocument>("playlist");

}

bool Playlist::attribute_has_zero_value(std::string attr)
{
    std::vector<std::string> tags_with_zero = {
        "PARENTID", "LAST_RESULT_PLAYED", "FILE_SIZE", "MARK_IN", "MARK_OUT",
        "TOTAL_DURATION", "VIDEO_WIDTH", "VIDEO_HEIGHT", "TFF", "SWAP_FIELD",
        "INTERLACED", "INTERLACED_FORCED", "ASPECT_RATIO", "ASPECT_RATIO_FORCED",
        "FRAME_RATE", "MATERIAL_AVAILABILITY", "COLOR_ADJUSTEMENT", "COLOR_SPACE",
        "SOUND_GAIN", "FILE_ID", "REQUEST_ANALYZE_INSTRUCTION"
    };

    auto it = std::find(tags_with_zero.begin(), tags_with_zero.end(), attr);

    return (it != tags_with_zero.end() ? true : false);
}

bool Playlist::attribute_has_negative_one(std::string attr)
{
    std::vector<std::string> tag_with_negative_one = {"CHANNEL", "EVENT_END", "ASSIGNED_LOGO_ID", "PROGRAM_TS_NUM_SELECTED"};
    auto it = std::find(tag_with_negative_one.begin(), tag_with_negative_one.end(), attr);
    return (it != tag_with_negative_one.end() ? true : false);
}

PlaylistItemAttributes Playlist::attributes()
{
    PlaylistItemAttributes pli_attrib;

    int tag_count = static_cast<int>(PlaylistElement::TagType::Count);
    PlaylistElement pl_elem;

    for(int i=0; i < tag_count-1; ++i) {
        pl_elem.m_tag_type = static_cast<PlaylistElement::TagType>(i);

        if (attribute_has_zero_value(pl_elem.to_string())) {
            pli_attrib[pl_elem.to_string()] = "0";
        } else if (attribute_has_negative_one(pl_elem.to_string())) {
            pli_attrib[pl_elem.to_string()] = "-1";
        } else {
            pli_attrib[pl_elem.to_string()] = "";
        }
    }

    return pli_attrib;
}

Playlist::~Playlist()
{
}


ErrorOr Playlist::set_playlist_content()
{
    // Check if playlist_file exists
    if (!QFileInfo::exists(m_playlist_file))
        return std::make_tuple(false, "File does not exists");

    QFile playlist_file(m_playlist_file);
    if(!playlist_file.open(QIODevice::ReadOnly))
        return std::make_tuple(false, "Unable to open file for reading!");

    if (!m_dom_document->setContent(&playlist_file)) {
        playlist_file.close();
        return std::make_tuple(false, "Unable to set playlist content! Check the XML file.");
    }

    playlist_file.close();
    return std::make_tuple(true, "ok");

}

QDomNode Playlist::find_node(QString node_name)
{
    QDomElement root_elem = m_dom_document->firstChildElement();
    QDomNodeList nodes = root_elem.elementsByTagName(node_name);
    if (nodes.isEmpty())
        return QDomNode();   // Null node - caller to check for this

    return nodes.at(0);
}

void Playlist::update_playlist(std::vector<QDomElement>& playlist_items)
{
    QDomElement root_element = m_dom_document->firstChildElement();
    QDomNode node_to_update = find_node("DB_INFO");

    if (node_to_update.isNull())
        return;

    for (auto playlistitem : playlist_items)
    {
        root_element.insertBefore(playlistitem, node_to_update);
    }
}

ErrorOr Playlist::save_playlist()
{

    QFile output_file(m_output_filepath);
    if (!output_file.open(QIODevice::WriteOnly | QIODevice::Truncate)) {
        QString msg = "Failed to open file for writting ...";
        return std::make_tuple(false, msg);
    }


    QTextStream stream(&output_file);
    m_dom_document->save(stream, 4);
    output_file.close();

    return std::make_tuple(true, "ok");

}

void Playlist::create_playlist_item(PlaylistItemAttributes attributes)
{
    QDomElement playlist_item = m_dom_document->createElement("PlaylistDB");

    make_attribute_elements(attributes, playlist_item);

    m_playlist_items.push_back(playlist_item);

    update_playlist(m_playlist_items);
}


void Playlist::make_attribute_elements(PlaylistItemAttributes& attribs, QDomElement& parent)
{
    int tag_count = static_cast<int>(PlaylistElement::TagType::Count);
    PlaylistElement ple;

    for(int i=0; i < tag_count-1; ++i)
    {
        ple.m_tag_type = static_cast<PlaylistElement::TagType>(i);
        QString tag_name = QString::fromStdString(ple.to_string());
        QDomElement tag = m_dom_document->createElement(tag_name);
        if (!attribs[ple.to_string()].empty()) {
            QDomText tag_text = m_dom_document->createTextNode(QString::fromStdString(attribs[ple.to_string()]));
            tag.appendChild(tag_text);
        }

        parent.appendChild(tag);
    }

}


}
