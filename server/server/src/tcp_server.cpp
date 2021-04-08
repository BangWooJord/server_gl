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
            std::cout << "\rMessage recieved from: " << connection_socket.local_endpoint().address() << ", message: " << string_msg << std::endl << "Server>";
        }
        start();
    }
    else if (err == boost::asio::error::eof)
        std::cerr << "\rClient disconnected" << std::endl << "Server>";
    else if (boost::asio::error::connection_reset == err)
        std::cerr << "\rClient disconnected forcefully" << std::endl << "Server>";
    else std::cerr << "\rRead caught an error: " << err.message() << std::endl << "Server>";
}
void tcp_connection::connectionStop() {
    this->connection_socket.close();
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
        start_accept();
    }
    else std::cerr << "\rError in handle_accept: " << error.message() << std::endl << "Server>";
}
void tcp_server::server_stop() {
    srv_io_context.post(boost::bind(&tcp_server::handle_stop, this));
}
void tcp_server::handle_stop() {
    std::string exit_flag = "1";
    tcp::socket sock(srv_io_context);
    boost::system::error_code err;
    boost::asio::write(sock, boost::asio::buffer(exit_flag, 1), err);
    srv_acceptor.close();
    srv_io_context.stop();
}