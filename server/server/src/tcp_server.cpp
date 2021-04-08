#include "../headers/tcp_server.h"

tcp_connection::tcp_connection(tcp::socket socket) 
	:connection_socket(std::move(socket))
{

}
void tcp_connection::start() {
	connection_socket.async_read_some(
		boost::asio::buffer(connection_data, max_length),
		boost::bind(&tcp_connection::read,
			shared_from_this()));

	connection_socket.async_write_some(
		boost::asio::buffer(message, max_length),
		boost::bind(&tcp_connection::write,
			shared_from_this()));
}
void tcp_connection::read() {
}
void tcp_connection::write() {
}

// TCP_SERVER
tcp_server::tcp_server(boost::asio::io_context& context, const int port)
	:server_acceptor(context, tcp::endpoint(tcp::v4(), port))
{
	server_listen();
}

void tcp_server::server_listen() {
	// socket
	tcp_connection::pointer connection
		= tcp_connection::create();

	// asynchronous accept operation and wait for a new connection.
	server_acceptor.async_accept(connection->socket(),
		boost::bind(&tcp_server::accept, this, connection,
			boost::asio::placeholders::error));
}
void tcp_server::accept(boost::shared_ptr<tcp_connection> new_connection,
	const boost::system::error_code& error) {
	if (!error) new_connection->start();
	server_listen();
}