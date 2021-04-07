#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <boost/asio.hpp>
#include <boost/bind/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
using boost::asio::ip::tcp;

class tcp_connection
	: public boost::enable_shared_from_this<tcp_connection> {
private:
	tcp::socket socket;
	std::string message;
public:
	tcp_connection(boost::asio::io_context& context);
	tcp::socket& getSocket();
	void start();
	void write();
	static boost::shared_ptr<tcp_connection>
		create_pointer(boost::asio::io_context& context);
};

class tcp_server {
private:
	boost::asio::io_context& server_context;
	tcp::acceptor server_acceptor;
	int server_port;
public:
	tcp_server();
	tcp_server(boost::asio::io_context& context, const int port);
	void server_listen();
	void accept(boost::shared_ptr<tcp_connection> new_connection,
		const boost::system::error_code& error);
};

#endif // !TCP_SERVER_H
