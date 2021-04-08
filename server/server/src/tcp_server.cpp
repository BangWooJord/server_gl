#include "../headers/tcp_server.h"

tcp::socket& tcp_connection::getSocket()
{
    return connection_socket;
}

void tcp_connection::start()
{
    boost::system::error_code err;
    boost::asio::async_read(connection_socket, msg_buf,
        boost::asio::transfer_at_least(1),
        boost::bind(&tcp_connection::handle_read, shared_from_this(),
            boost::asio::placeholders::error));
}

tcp_connection::tcp_connection(boost::asio::io_context& io_context)
    : connection_socket(io_context)
{
}

void tcp_connection::handle_read(const boost::system::error_code& err) {
    if (!err) {
        std::istream is(&msg_buf);
        std::getline(is, string_msg);
        if (!string_msg.empty()) {
            std::cout << "Message recieved from: " << connection_socket.local_endpoint().address() << ", message: " << string_msg << std::endl;
        }
        start();
    }
    else if (err == boost::asio::error::eof)
        std::cerr << "Client disconnected" << std::endl;
    else if (boost::asio::error::connection_reset == err)
        std::cerr << "Client disconnected forcefully" << std::endl;
    else std::cerr << "Read caught an error: " << err.message() << std::endl;
}

// TCP_SERVER

tcp_server::tcp_server(boost::asio::io_context& io_context, const int port)
	: srv_io_context(io_context),
	srv_acceptor(io_context, tcp::endpoint(boost::asio::ip::make_address("127.0.0.1"), port))
{
	start_accept();
}
void tcp_server::start_accept()
{
	tcp_connection::pointer new_connection =
		tcp_connection::create(srv_io_context);

	srv_acceptor.async_accept(new_connection->getSocket(),
		boost::bind(&tcp_server::handle_accept, this, new_connection,
			boost::asio::placeholders::error));
}
void tcp_server::handle_accept(tcp_connection::pointer new_connection,
	const boost::system::error_code& error)
{
	if (!error)
	{
		new_connection->start();
	}
	else std::cerr << "Error in handle_accept: " << error.message();


	start_accept();
}