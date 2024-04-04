#pragma once

#include <vector>
#include <string>

class Command {
public:
    size_t GetArgSize() const;

    std::string GetArg(size_t command_num) const;
    std::string GetCommand() const;

    bool IsValid() const;
    
    void AddArg(const std::string& arg);
    void Parse(const std::string& to_parse);
private:
    std::vector<std::string> args_;
};