#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>
#include "headers/tcp_server.h"

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
	while (true) {
		std::cout << "Server> ";
		std::cin >> input; 

		if (input == "help") printCommands();
		else if (input == "start") {
			if (port) {
				boost::asio::io_context context;
				tcp_server server(context, port);
				context.run();
			}
		}
		else if (input == "stop"){
		}
		else if (input == "setport"){
			std::cin >> port;
			if (std::cin.fail() || port < 1) {
				std::cerr << "Input a (positive) number " << std::endl;
				std::cin.clear();
				std::cin.ignore();
			}
		}
		else if (input == "exit"){
			break;
		}
		else {
			std::cerr << "'" + input + "'" << " is not recognized as a command. \n Try 'help' to get the command list\n\n";
		}
	}
	return 0;
}