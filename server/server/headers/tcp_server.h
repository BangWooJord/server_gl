#ifndef TCP_SERVER_H
#define TCP_SERVER_H
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
using boost::asio::ip::tcp;

class tcp_connection
	: public boost::enable_shared_from_this<tcp_connection> {
private:
	tcp::socket connection_socket;
	std::string message = "Hello From Server!";
	enum { max_length = 1024 };
	char connection_data[max_length];
public:
	typedef boost::shared_ptr<tcp_connection> pointer;
	tcp_connection(boost::asio::io_service& io_service) : connection_socket(io_service) {}
	  // creating the pointer
	  static pointer create(boost::asio::io_service& io_service)
	  {
		  return pointer(new tcp_connection(io_service));
	  }
	  tcp::socket& socket()
	  {
		  return connection_socket;
	  }

	tcp_connection(tcp::socket socket);
	void start();
	void read();
	void write();
};

class tcp_server {
private:
	tcp::acceptor server_acceptor;
public:
	tcp_server();
	tcp_server(boost::asio::io_context& context, const int port);
	void server_listen();
	void accept(boost::shared_ptr<tcp_connection> new_connection,
		const boost::system::error_code& error);
};

#endif // !TCP_SERVER_H
