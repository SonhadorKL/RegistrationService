#pragma once

#include "Command.hpp"
#include "Query.hpp"

class Client {
public:
    /* Set parameters of connection */
    void GetConnection(std::string& user, std::string& password, std::string& host_ip, std::string& port);

    /* Process one command from std::cin. Throws an exception if something go wrong */
    bool GetNext(Query& to_request);
    
private:

    /* Check if command has the right amount of arguments overwise throw an exception */
    void CheckCommandArgs(Command& processing, int arg_count);
};