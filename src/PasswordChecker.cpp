#include "PasswordChecker.hpp"

#include <iostream>
#include <set>
#include <algorithm>

PasswordChecker::PasswordChecker(const std::string& path_to_config) : stream_to_json_(path_to_config) {
    password_requirements_ = nlohmann::json::parse(stream_to_json_);
} 

std::string PasswordChecker::Check(const std::string& password) {
    std::string best_status = kDefaultStatus;
    int current_priority = -1;
    for (auto& [status, characteristic] : password_requirements_.items()) {
        const std::string& preprocessed_password = Preprocess(password, characteristic["special"]);
        
        bool type_is_valid = true;
        type_is_valid = type_is_valid 
            && Length(preprocessed_password, characteristic["character_count"])
            && Unique(preprocessed_password, characteristic["unique_count"])
            && Upper(preprocessed_password, characteristic["upper"])
            && Lower(preprocessed_password, characteristic["lower"])
            && Number(preprocessed_password, characteristic["number"])
            && Symbol(preprocessed_password, characteristic["symbol"]);
        
        if (type_is_valid && current_priority < characteristic["priority"]) {
            best_status = status;
            current_priority = characteristic["priority"];
        }
    }
    return best_status;
}

PasswordChecker::~PasswordChecker() {
    stream_to_json_.close();
}

std::string PasswordChecker::Preprocess(const std::string& password, int weak_words_count) {
    // TODO: implement checker if password contains
    // the most common passwords and remove them
    return password;
}

bool PasswordChecker::Length(const std::string &to_check, uint32_t min_cnt) {
    return to_check.length() >= min_cnt;
}

bool PasswordChecker::Unique(const std::string &to_check, uint32_t min_cnt) {
    return std::set<char>(to_check.begin(), to_check.end()).size() >= min_cnt;
}

bool PasswordChecker::Upper(const std::string &to_check, uint32_t min_cnt) {
    return std::count_if(to_check.begin(), to_check.end(), [](char ch) {return std::isupper(ch); }) >= min_cnt;
}

bool PasswordChecker::Lower(const std::string &to_check, uint32_t min_cnt) {
    return std::count_if(to_check.begin(), to_check.end(), [](char ch) {return std::islower(ch); }) >= min_cnt;
}

bool PasswordChecker::Number(const std::string &to_check, uint32_t min_cnt) {
    return std::count_if(to_check.begin(), to_check.end(), [](char ch) {return std::isdigit(ch); }) >= min_cnt;
}

bool PasswordChecker::Symbol(const std::string &to_check, uint32_t min_cnt) {
    return std::count_if(to_check.begin(), to_check.end(), [](char ch) {return !std::isalnum(ch); }) >= min_cnt;
}
