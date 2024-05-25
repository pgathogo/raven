#pragma once

#include <tuple>
#include <vector>
#include <string>

namespace FRAMEWORK {
    struct NodeData;
}

using LineTokens = std::tuple<std::string, std::string, std::string>;

class AudioLibrary
{
public:
    std::vector<FRAMEWORK::NodeData*> read_data_from_file(const std::string);
    std::vector<FRAMEWORK::NodeData*> read_data_from_db();
private:
    LineTokens line_tokenizer(std::string line);
};
