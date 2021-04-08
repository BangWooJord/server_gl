#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>
#include <thread>

using boost::asio::ip::tcp;

void write_handler(const boost::system::error_code& error,
    std::size_t bytes_transferred) {
    if (error) {
        std::cerr << "Write Handler caught error: " << error.message() << std::endl;
    }
}

int connect_to_srv(boost::asio::io_service& service, tcp::socket& socket, const std::string address) {

    const auto semicln = address.find(":");
    std::string host = address.substr(0, semicln);
    std::string port = address.substr(semicln + 1);

    boost::system::error_code err;

    tcp::resolver resolver(service);
    tcp::resolver::query q(tcp::v4(), host, port);
    tcp::resolver::iterator it = resolver.resolve(q, err);
    if (err) throw - 2;
    boost::asio::connect(socket, it, err);
    if (err) throw -1;

    return 0;
}


void printCommands() {
    std::cout << std::endl;
    std::cout << " - help \n \t\t This help message" << std::endl << std::endl;
    std::cout << " - connect \n \t\t Connect to the server. E.g: connect 127.0.0.1:54000" << std::endl << std::endl;
    std::cout << " - send \n \t\t Send message. E.g: send Hello" << std::endl << std::endl;
    std::cout << " - exit \n \t\t Exit this session" << std::endl << std::endl;
}

int main(int argc, char* argv[])
{
    boost::asio::io_service service;
    tcp::socket socket(service);
    std::string address;
    std::string input;
    boost::system::error_code err;
    bool is_connected = false;
    std::thread check_server_th([&]() {
        while (true) {
            if (is_connected) {
                boost::asio::streambuf exit_msg;
                boost::asio::read(socket, exit_msg, err);
                std::cout << "\rServer disconnected" << std::endl << "Client>";
                is_connected = false;
            }
        }
        });
    std::cerr << "Write 'help' to see the command list" << std::endl << std::endl;
    while (true) {
        std::cout << "Client> ";
        std::cin >> input;

        if (input == "help") printCommands();
        else if (input == "connect") {
            std::cin >> address;
            try {
                connect_to_srv(service, socket, address);
            }
            catch (int err) {
                std::cerr << "\rCouldn't establish server connection, error: " << err << std::endl;
                is_connected = false;
                continue;
            }
            is_connected = true;
        }
        else if (input == "send") {
            if (is_connected) {
                std::string msg;
                std::getline(std::cin, msg);
                boost::asio::async_write(socket, boost::asio::buffer(msg), boost::bind(
                    &write_handler,
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred));

                if (!err) std::cout << "\rMessage sent to " << address << std::endl;
                else std::cerr << "\rError sending message, error: " << err.message() << std::endl;
            }
            else std::cerr << "\rYou are not connected to the server" << std::endl;
        }
        else if (input == "exit") {
            break;
        }
        else {
            std::cerr << "\r'" + input + "'" << " is not recognized as a command. \n Try 'help' to get the command list\n\n";
        }
    }
    check_server_th.join();

    return 0;
}