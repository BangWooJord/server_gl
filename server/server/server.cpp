#include <iostream>
#include <string>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context)
    {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& getSocket()
    {
        return connection_socket;
    }

    void start()
    {
        boost::system::error_code err;
        boost::asio::async_read(connection_socket, msg_buf,
            boost::asio::transfer_at_least(1),
            boost::bind(&tcp_connection::handle_read, shared_from_this(), 
                boost::asio::placeholders::error));
    }

private:
    tcp_connection(boost::asio::io_context& io_context)
        : connection_socket(io_context)
    {
    }

    void handle_read(const boost::system::error_code& err) {
        if ((err == boost::asio::error::eof) ||
            (boost::asio::error::connection_reset == err)) {
            std::cerr << err.message() << std::endl;
        }
        else if(!err) {
            std::istream is(&msg_buf);
            std::getline(is, string_msg);
            if (!string_msg.empty()) {
                std::cout << "Message recieved from: " << connection_socket.local_endpoint().address() << ", message: " << string_msg << std::endl;
            }
            start();
        }
    }

    tcp::socket connection_socket;
    boost::asio::streambuf msg_buf;
    std::string string_msg;
};

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context, const int port)
        : srv_io_context(io_context),
        srv_acceptor(io_context, tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), port))
    {
        start_accept();
    }

private:
    void start_accept()
    {
        tcp_connection::pointer new_connection =
            tcp_connection::create(srv_io_context);

        srv_acceptor.async_accept(new_connection->getSocket(),
            boost::bind(&tcp_server::handle_accept, this, new_connection,
                boost::asio::placeholders::error));
    }

    void handle_accept(tcp_connection::pointer new_connection,
        const boost::system::error_code& error)
    {
        if (!error)
        {
            new_connection->start();
        }
        else std::cerr << "Error in handle_accept: " << error.message();
       

        start_accept();
    }

    boost::asio::io_context& srv_io_context;
    tcp::acceptor srv_acceptor;
};

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
                    boost::asio::io_context io_context;
                    tcp_server server(io_context, std::move(port));
                    io_context.run();
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

    return 0;
}