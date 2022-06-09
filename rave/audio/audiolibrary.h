#pragma once

#include <tuple>
#include <vector>

class NodeData;

using LineTokens = std::tuple<std::string, std::string, std::string>;

class AudioLibrary
{
public:
    std::vector<NodeData*> read_data_from_file(const std::string);
    std::vector<NodeData*> read_data_from_db();
private:
    LineTokens line_tokenizer(std::string line);
};
