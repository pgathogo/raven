#include <memory>
#include <fstream>
#include <QDebug>

#include "audiolibrary.h"
#include "../framework/tree.h"
#include "../framework/entitydatamodel.h"
#include "../framework/ravenexception.h".h"
#include "audiofolder.h"

std::vector<NodeData*> AudioLibrary::read_data_from_file(const std::string data_file)
{
    std::vector<NodeData*> data;

    std::string line;
    std::ifstream tree_file(data_file);

    if (tree_file.is_open())
    {
        while (std::getline(tree_file, line))
        {
           auto[id, name, parent_id] = line_tokenizer(line);

           auto node_data = new NodeData();
           node_data->id = std::stoi(id);
           node_data->parent_id = std::stoi(parent_id);
           node_data->name = name;
           data.push_back(node_data);
        }
    }else{
        qDebug() << "Unable to open file for reading!";
    }

    return data;
}

LineTokens AudioLibrary::line_tokenizer(std::string line)
{
     size_t pos = 0;
     std::string token;
     std::string delimeter = "|";

     std::vector<std::string> tokens;

     while((pos = line.find(delimeter)) != std::string::npos){
         token = line.substr(0, pos);
         tokens.push_back(token);
         line.erase(0, pos + delimeter.length());
     }

     TreeData tok_tuple = std::make_tuple(tokens.at(0), tokens.at(1), tokens.at(2));

     return tok_tuple;
}

std::vector<NodeData*> AudioLibrary::read_data_from_db()
{
    std::vector<NodeData*> data;

    EntityDataModel edm(std::make_shared<AudioFolder>());
    qDebug() << "5555";
    try{

    qDebug() << "7777";
        edm.all();
    } catch(DatabaseException& de) {
        std::cout << de.errorMessage() << '\n';
    }

    for(auto& [name, entity] : edm.modelEntities()){
       AudioFolder* audio_folder = dynamic_cast<AudioFolder*>(entity.get());
       auto node_data = new NodeData();
       node_data->id = audio_folder->id();
       node_data->parent_id = audio_folder->parent_id()->value();
       node_data->name = audio_folder->folder_name()->value();
       data.push_back(node_data);
    }
    return data;
}
