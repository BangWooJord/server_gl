#include "../headers/tcp_server.h"

tcp_connection::tcp_connection(boost::asio::io_context& context)
	:socket(context) {
}
tcp::socket& tcp_connection::getSocket() {
	return this->socket;
}
boost::shared_ptr<tcp_connection> 
	tcp_connection::create_pointer(boost::asio::io_context& context) 
{
	return boost::shared_ptr<tcp_connection>(new tcp_connection(context));
}
void tcp_connection::start() {
	boost::asio::async_write(this->socket, boost::asio::buffer(message),
		boost::bind(&tcp_connection::write, shared_from_this()));
}
void tcp_connection::write() {

}

// TCP_SERVER
tcp_server::tcp_server(boost::asio::io_context& context, const int port)
	:server_port(port), server_context(context),
	server_acceptor(server_context, tcp::endpoint(tcp::v4(), server_port))
{
	server_listen();
}

void tcp_server::server_listen() {
	boost::shared_ptr<tcp_connection> new_connection =
		tcp_connection::create_pointer(server_context);

	server_acceptor.async_accept(new_connection->getSocket(),
		boost::bind(&tcp_server::accept, this, new_connection,
			boost::asio::placeholders::error));
}
void tcp_server::accept(boost::shared_ptr<tcp_connection> new_connection,
	const boost::system::error_code& error) {
	if (!error) new_connection->start();
	server_listen();
}