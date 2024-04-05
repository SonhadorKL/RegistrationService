#include "Command.hpp"

void Command::Parse(const std::string& to_parse) {
    std::string processing_word = "";
    for (size_t i = 0; i <= to_parse.size(); ++i) {
        if (to_parse[i] == ' ' || i == to_parse.size()) {
            if (processing_word == "") continue;
            AddArg(processing_word);
            processing_word = "";
            continue;
        }
        processing_word += to_parse[i];
    }
}

size_t Command::GetArgSize() const {
    return args_.size() - 1;
}

std::string Command::GetArg(size_t command_num) const{
    return args_.at(command_num + 1);
}

std::string Command::GetCommand() const {
    return args_.at(0);
}

bool Command::IsValid() const {
    return args_.size() > 0;
}

void Command::AddArg(const std::string& arg) {
    args_.push_back(arg);
}
