#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "headers/tcp_server.h"
#include <thread>

using boost::asio::ip::tcp;

void printCommands() {
    std::cout << " - help \n \t\t This help message" << std::endl;
    std::cout << " - start \n \t\t Start server" << std::endl;
    std::cout << " - stop \n \t\t Stop server" << std::endl;
    std::cout << " - setport \n \t\t Set server port. E.g: setport 123" << std::endl;
    std::cout << " - exit \n \t\t Exit this session" << std::endl;
}

int main()
{
        std::cerr << "Write 'help' to see the command list" << std::endl << std::endl;

        int port;
        std::string input; 
        std::thread server_listen_th;
        while (true) {
            std::cout << "Server> ";
            std::cin >> input;

            if (input == "help") printCommands();
            else if (input == "start") {
                if (port) {
                    server_listen_th = std::thread([&]() {
                        boost::asio::io_context io_context;
                        tcp_server server(io_context, std::move(port));
                        io_context.run();
                        });
                    std::cout << "Server started on port " << port << std::endl;
                }
            }
            else if (input == "stop") {
            }
            else if (input == "setport") {
                std::cin >> port;
                if (std::cin.fail() || port < 1) {
                    std::cerr << "Input a (positive) number " << std::endl;
                    std::cin.clear();
                    std::cin.ignore();
                }
            }
            else if (input == "exit") {
                break;
            }
            else {
                std::cerr << "'" + input + "'" << " is not recognized as a command. \n Try 'help' to get the command list\n\n";
            }
        }
        server_listen_th.join();

    return 0;
}