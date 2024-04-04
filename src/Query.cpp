#include "Query.hpp"
#include "Config.h"


#include <fstream>
#include <PasswordChecker.hpp>
#include <regex>


// Check if string is a valid email
bool IsEmailValid(const std::string& email_to_check) {
    std::regex email_checker("[\\w\\d]+@\\w+.\\w+");
    return std::regex_match(email_to_check.c_str(), email_checker);
}

// TODO: make it smarter!!!
// Generate new unique id
size_t GetNextId(pqxx::work& req) {
    size_t next_id = 1;
    auto id_max = req.exec_prepared("get_max_id");
    if (!id_max[0][0].is_null()) {
        next_id = id_max[0][0].as<size_t>();
        ++next_id; 
    }
    return next_id;
}

Query::Query(std::string user, std::string password, std::string host_ip, std::string port) : 
        access_to_db("user = " + user + " password = " + password + " hostaddr = " + host_ip + " port = " + port),
        checker_(kPathToRequirements) {
    PrepareConnection();
}

std::string Query::Register(const Command &args) {
    std::string email = args.GetArg(0);
    std::string password = args.GetArg(1);

    if (!IsEmailValid(email)) {
        throw std::invalid_argument("Invalid email address: " + email + "\n");
    }

    // Check if user exists
    pqxx::work req(access_to_db);
    if (req.exec_prepared("get_user_by_email", email).size() > 0) {
        throw std::invalid_argument("User with email: " + email + " already exists.\n");
    }

    // Check if password is good enough
    std::string check_status = checker_.Check(password);
    if (check_status == "weak") {
        throw std::invalid_argument("The password is too weak.\n");
    }

    // Add new user
    size_t next_id = GetNextId(req);
    req.exec_prepared("insert_user", next_id, email, password);
    req.commit();

    // Return user_id and password_status
    return "user_id: " + std::to_string(next_id) + "\n"
        +  "password_status: " + check_status;
}

/*  
    // TODO: make tokens harder!
    args: email, password
    throw an error if email is not in the base or wrong password
*/
std::string Query::Authorize(const Command &args) {
    std::string email = args.GetArg(0);
    std::string password = args.GetArg(1);

    // Check if user exists
    pqxx::work req(access_to_db);
    auto user_result = req.exec_prepared("get_user_by_email", email); 
    if (user_result.size() == 0) {
        throw std::invalid_argument("There is no user with such email: " + email);
    }

    // Check if password is correct
    std::string user_id = user_result[0][0].as<std::string>();
    std::string user_password = user_result[0][2].as<std::string>();
    if (password != user_password) {
        throw std::invalid_argument("Wrong password\n");
    }

    return "Token: " + user_id;
}

std::string Query::Feed(const Command &args) {
    std::string token = args.GetArg(0);

    // Check if token is valid
    pqxx::work req(access_to_db);
    auto user_result = req.exec_prepared("get_user_by_id", token); 
    if (user_result.size() == 0) {
        throw std::invalid_argument("Token invalid\n");
    }

    // Success
    return "Code: 200";
}

void Query::PrepareConnection() {
    std::ifstream ifs(kPathToCommands);
    json commands = json::parse(ifs);
    for (auto& [command_name, request] : commands.items()) {
        access_to_db.prepare(command_name, request);
    }
}