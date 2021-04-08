#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
using boost::asio::ip::tcp;

class tcp_connection
    : public boost::enable_shared_from_this<tcp_connection>
{
public:
    typedef boost::shared_ptr<tcp_connection> pointer;

    static pointer create(boost::asio::io_context& io_context) {
        return pointer(new tcp_connection(io_context));
    }

    tcp::socket& getSocket();

    void start();

private:
    tcp_connection(boost::asio::io_context& io_context);

    void handle_read(const boost::system::error_code& err);

    tcp::socket connection_socket;
    boost::asio::streambuf msg_buf;
    std::string string_msg;
};

class tcp_server
{
public:
    tcp_server(boost::asio::io_context& io_context, const int port);

private:
    void start_accept();

    void handle_accept(tcp_connection::pointer new_connection,
        const boost::system::error_code& error);

    void server_stop();
    boost::asio::io_context& srv_io_context;
    tcp::acceptor srv_acceptor;
};


#endif // !TCP_SERVER_H
