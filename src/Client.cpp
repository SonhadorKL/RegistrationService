#include "Client.hpp"

#include "Command.hpp"

#include <iostream>

void Client::GetConnection(std::string &user, std::string &password, std::string &host_ip, std::string &port) {
    std::cout << "Please, set connection\n";
    
    std::cout << "Username: ";
    std::cin >> user;

    std::cout << "Password: ";
    std::cin >> password;

    std::cout << "IP: ";
    std::cin >> host_ip;

    std::cout << "Port: ";
    std::cin >> port;
}

bool Client::GetNext(Query &to_request)
{
    std::string user_input;
    std::getline(std::cin, user_input);
    
    Command processing_command;
    processing_command.Parse(user_input);

    if (!processing_command.IsValid()) {
        return true;
    }

    std::string result_of_command;
    if (processing_command.GetCommand() == "/register") {
        CheckCommandArgs(processing_command, 2);
        result_of_command = to_request.Register(processing_command);

    } else if (processing_command.GetCommand() == "/authorize") {
        CheckCommandArgs(processing_command, 2);
        result_of_command = to_request.Authorize(processing_command);

    } else if (processing_command.GetCommand() == "/feed") {
        CheckCommandArgs(processing_command, 1);
        result_of_command = to_request.Feed(processing_command);

    } else if (processing_command.GetCommand() == "/exit") {
        return false;
    } else {
        throw std::invalid_argument("Don't know the command " + processing_command.GetCommand() + '\n');
    }
    std::cout << result_of_command << '\n';
    return true;
}

void Client::CheckCommandArgs(Command& processing, int arg_count) {
    if (processing.GetArgSize() != arg_count) {
        throw std::invalid_argument("Invalid argument count: "
            + processing.GetCommand() + " expected " + std::to_string(arg_count)
            + " args - got " + std::to_string(processing.GetArgSize()) + " \n");
    }
}
