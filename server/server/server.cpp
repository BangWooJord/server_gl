#include <iostream>
#include <string>
#include <boost/asio.hpp>
#include "headers/tcp_server.h"
#include <thread>

using boost::asio::ip::tcp;

void printCommands() {
    std::cout << std::endl;
    std::cout << " - help \n \t\t This help message" << std::endl << std::endl;
    std::cout << " - start \n \t\t Start server" << std::endl << std::endl;
    std::cout << " - stop \n \t\t Stop server" << std::endl << std::endl;
    std::cout << " - setport \n \t\t Set server port. E.g: setport 123" << std::endl << std::endl;
    std::cout << " - exit \n \t\t Exit this session" << std::endl << std::endl;
}

int main()
{
        std::cerr << "Write 'help' to see the command list" << std::endl << std::endl;

        int port = NULL;
        std::string input; 
        std::thread server_listen_th;
        tcp_server *server = nullptr;
        while (true) {
            std::cout << "Server> ";
            std::cin >> input;

            if (input == "help") printCommands();
            else if (input == "start") {
                if (server_listen_th.joinable()) {
                    std::cerr << "Server is currently running, stop it first" << std::endl;
                    continue;
                }
                if (port) {
                    server_listen_th = std::thread([&]() {
                        boost::asio::io_context io_context;
                        server = new tcp_server(io_context, std::move(port));
                        io_context.run();
                        });
                    std::cout << "Server started on port " << port << std::endl;
                }
                else std::cerr << "Set port first" << std::endl;
            }
            else if (input == "stop") {
                if (server != nullptr) {
                    server->server_stop();
                    server = nullptr;
                    server_listen_th.detach();
                }
                else std::cerr << "Can't close non existing server" << std::endl;
            }
            else if (input == "setport") {
                std::cin >> port;
                if (std::cin.fail() || port < 1) {
                    std::cerr << "Input a (positive) number " << std::endl;
                    std::cin.clear();
                    std::cin.ignore();
                }
                else std::cout << "\nPort succesfully set to " << port << std::endl << std::endl;
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