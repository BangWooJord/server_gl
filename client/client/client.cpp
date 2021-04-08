#include <iostream>
#include <boost/array.hpp>
#include <boost/bind.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

void write_handler(const boost::system::error_code& error,
    std::size_t bytes_transferred) {

}

int connect_to_srv(const std::string& host, const std::string& port) {
    boost::asio::io_service service;

    tcp::socket socket(service);
    tcp::resolver resolver(service);
    tcp::resolver::query q(tcp::v4(), host, port);
    tcp::resolver::iterator it = resolver.resolve(q);
    boost::system::error_code err;
    // написать проверку на существование сервера
    boost::asio::connect(socket, it, err);
    if (err) throw -1;
    while (true) {
        std::cout << "Enter message" << std::endl;
        std::string msg;
        std::getline(std::cin, msg);
        boost::asio::async_write(socket, boost::asio::buffer(msg), boost::bind(
            &write_handler,
            boost::asio::placeholders::error,
            boost::asio::placeholders::bytes_transferred));

        if (!err) std::cout << "Message sent" << std::endl;
        else std::cerr << "Error sending message, error: " << err.message() << std::endl;

    }
    return 0;
}

int main(int argc, char* argv[])
{
    std::string host, port;
    std::cout << "Input host" << std::endl;
    std::cin >> host;
    std::cout << "Input port" << std::endl;
    std::cin >> port;

    try {
        connect_to_srv(host, port);
    }catch (int err) {
        std::cerr << "Error: " << err;
    }
    

    return 0;
}