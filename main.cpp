#include "Client.hpp"

#include <iostream>
#include <Query.hpp>

int main() {
    Client client;
    std::string user, password, ip, port;
    client.GetConnection(user, password, ip, port);

    Query tasks(user, password, ip, port);
    bool is_continue = true;
    while (is_continue) {
        try {
            is_continue = client.GetNext(tasks);
        } catch(std::exception& ex) {
            std::cerr << ex.what();
        }
    }
    return 0;
}
