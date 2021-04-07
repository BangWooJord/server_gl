#include <iostream>
#include <boost/asio.hpp>
#include <boost/array.hpp>

using boost::asio::ip::tcp;

void printCommands() {
	std::cout << " - help \n \t\t This help message" << std::endl;
	std::cout << " - start \n \t\t Start server" << std::endl;
	std::cout << " - stop \n \t\t Stop server" << std::endl;
	std::cout << " - setport \n \t\t Set server port" << std::endl;
	std::cout << " - exit \n \t\t Exit this session" << std::endl;
}

int main()
{
	std::cerr << "Write 'help' to see the command list" << std::endl << std::endl;
	while (true) {
		std::string input;
		std::cout << "Server> ";
		std::cin >> input; 
		if (input == "help") printCommands();
	}
	return 0;
}