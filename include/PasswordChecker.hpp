#pragma once

#include <nlohmann/json.hpp>
#include <fstream>


class PasswordChecker {
    using json = nlohmann::json; 
public:
    PasswordChecker(const std::string& path_to_config);

    // Check if given password is strong enough
    std::string Check(const std::string& password);

    ~PasswordChecker();

private:
    json password_requirements_;
    std::ifstream stream_to_json_;

    const std::string kDefaultStatus = "weak";


    std::string Preprocess(const std::string& password, int weak_words_count);

    // Checkers
    bool Length(const std::string& to_check, uint32_t min_cnt);
    bool Unique(const std::string& to_check, uint32_t min_cnt);
    bool Upper(const std::string& to_check, uint32_t min_cnt);
    bool Lower(const std::string& to_check, uint32_t min_cnt);
    bool Number(const std::string& to_check, uint32_t min_cnt);
    bool Symbol(const std::string& to_check, uint32_t min_cnt);
};