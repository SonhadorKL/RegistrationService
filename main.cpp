#include "Client.hpp"

#include <iostream>
#include <Query.hpp>

int main() {
    Client client;
    std::string user, password, ip, port;
    try {
        client.GetConnection(user, password, ip, port);
        Query tasks(user, password, ip, port);
        std::cout << "Successful connection\n";
        bool is_continue = true;
        while (is_continue) {
            try {
                is_continue = client.GetNext(tasks);
            } catch(std::exception& ex) {
                std::cerr << ex.what();
            }
        }
    }
    catch (std::exception& ex) {
        std::cout << "Could not connect to data base\n";
    }
    return 0;
}
