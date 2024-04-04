#include "Client.hpp"

#include <iostream>
#include <Query.hpp>

int main() {
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
