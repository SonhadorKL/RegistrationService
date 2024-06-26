#include "Query.hpp"

#include <fstream>
#include <random>
#include <regex>

#include "Config.h"
#include "PasswordChecker.hpp"
#include "jwt-cpp/jwt.h"

// Check if string is a valid email
bool IsEmailValid(const std::string& email_to_check) {
  std::regex email_checker("[\\w\\d]+@\\w+.\\w+");
  return std::regex_match(email_to_check.c_str(), email_checker);
}

// Generate new unique id
size_t GetNextId(pqxx::work& req) {
  std::random_device rand;
  std::mt19937_64 eng(rand());
  std::uniform_int_distribution<long long> distr;
  size_t next_id = std::abs(distr(eng));
  while (!req.exec_prepared("get_user_by_id", next_id)[0][0].is_null()) {
    next_id = std::abs(distr(eng));
  }
  return next_id;
}

Query::Query(std::string user, std::string password, std::string host_ip,
             std::string port)
    : access_to_db("user = " + user + " password = " + password +
                   " hostaddr = " + host_ip + " port = " + port),
      checker_(kPathToRequirements) {
  CreateTable();
  PrepareConnection();
}

std::string Query::Register(const Command& args) {
  std::string email = args.GetArg(0);
  std::string password = args.GetArg(1);

  if (!IsEmailValid(email)) {
    throw std::invalid_argument("Invalid email address: " + email + "\n");
  }

  // Check if user exists
  pqxx::work req(access_to_db);
  if (req.exec_prepared("get_user_by_email", email).size() > 0) {
    throw std::invalid_argument("User with email: " + email +
                                " already exists.\n");
  }

  // Check if password is good enough
  std::string check_status = checker_.Check(password);
  if (check_status == kDefaultPasswordStatus) {
    throw std::invalid_argument("The password is too weak.\n");
  }

  // Add new user
  size_t next_id = GetNextId(req);
  req.exec_prepared("insert_user", next_id, email, password);
  req.commit();

  // Return user_id and password_status
  return "user_id: " + std::to_string(next_id) + "\n" +
         "password_status: " + check_status;
}

std::string Query::Authorize(const Command& args) {
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

  // Prepare token
  auto token = jwt::create()
                   .set_type("JWT")
                   .set_payload_claim("user_id", jwt::claim(user_id))
                   .sign(jwt::algorithm::hs256{"secret"});

  return "Token: " + token;
}

std::string Query::Feed(const Command& args) {
  std::string token = args.GetArg(0);
  std::invalid_argument ex_invalid_token("Token is invalid\n");

  // Check if token is valid
  std::string user_id;
  try {
    auto decoded_token = jwt::decode(token);
    user_id = decoded_token.get_payload_json()["user_id"].to_str();
  } catch (std::exception&) {
    throw ex_invalid_token;
  }

  // Check if id is valid
  pqxx::work req(access_to_db);
  auto user_result = req.exec_prepared("get_user_by_id", user_id);
  if (user_result.size() == 0) {
    throw ex_invalid_token;
  }

  // Success
  return "Code: 200";
}

void Query::CreateTable() {
  pqxx::work req(access_to_db);
  try {
    req.exec(
        "create table if not exists users_ (user_id_ bigint primary key, "
        "email_ text, password_ text)");
    req.commit();
  } catch (std::exception&) {
    // pass
  }
}

void Query::PrepareConnection() {
  std::ifstream ifs(kPathToCommands);
  json commands = json::parse(ifs);
  for (auto& [command_name, request] : commands.items()) {
    access_to_db.prepare(command_name, request);
  }
}