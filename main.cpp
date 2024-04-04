// #include "PasswordChecker.hpp"
// #include "ArgParser.hpp"

#include "Client.hpp"

#include <iostream>
#include <pqxx/pqxx>
#include <fstream>
#include <Query.hpp>

int main() {
    // std::ifstream ifs("/home/sonhador/internship/Projects/Registration/settings/password_requirements.json");
    // nlohmann::json jf = nlohmann::json::parse(ifs);

    // PasswordChecker check(jf);
    // std::cout << check.Check("fsdfds132sfD#gdffsdfdad") << '\n';
    Client client;
    Query t("postgres", "qwerty", "127.0.0.1", "5432");
    bool is_continue = true;
    while (is_continue) {
        try {
            is_continue = client.GetNext(t);
        } catch(std::exception& ex) {
            std::cerr << ex.what();
        }
    }
    return 0;
}
using namespace std;

// int main(int argc, char* argv[]) {
//    try {
//       Query t("postgres", "qwerty", "127.0.0.1", "5432");
      
//       t.Register("Lol");
//       // C.disconnect();
//    } catch (const std::exception &e) {
//       cerr << e.what() << std::endl;
//       return 1;
//    }
// }


// pqxx::connection c("user = postgres password = qwerty \
//       hostaddr = 127.0.0.1 port = 5432");
//       if (c.is_open()) {
//          cout << "Opened database successfully: " << c.dbname() << endl;
//          pqxx::work w(c);
//          try {
//             // w.exec("create table if not exists test (\
//             //    id int primary key,\
//             //    username varchar(50)\
//             // );");
//             pqxx::result r = w.exec("SELECT id FROM test");
//             for (auto const &row: r) {
//                for (auto const &field: row) {
//                   std::cout << field.c_str() << '\t';
//                }
//                std::cout << std::endl;
//             }
//             w.commit();         
//          } catch(const std::exception &e) {
//             cerr << e.what() << endl;
//          }
//       } else {
//          cout << "Can't open database" << endl;
//          return 1;
//       }