#pragma once

#include <pqxx/pqxx>
#include <nlohmann/json.hpp>
#include "PasswordChecker.hpp"

#include "ArgParser.hpp"

class Query {
    using json = nlohmann::json; 
public:
    Query(std::string user, std::string password, std::string host_ip, std::string port);

    // args: email, password
    // throw an error if email is not valid or user already exists
    std::string Register(const Command& args);
    
    // args: email, password
    // throw an error if email is not in the base or wrong password
    std::string Authorize(const Command& args);

    // args: token
    // throw an error if token is not valid
    std::string Feed(const Command& args);
private:
    pqxx::connection access_to_db;
    PasswordChecker checker_;

    void CreateTable();
    void PrepareConnection();

};
