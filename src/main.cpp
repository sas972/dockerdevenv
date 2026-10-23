#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

// Manages an active TCP client session
class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket)
        : socket_(std::move(socket)) {}

    void start() {
        std::cout << "[+] New client connected from: "
                  << socket_.remote_endpoint() << std::endl;
        do_write("Welcome to the Boost.Asio TCP Echo Server!\r\nType something and press Enter:\r\n");
    }

private:
    void do_read() {
        auto self(shared_from_this());
        socket_.async_read_some(
            boost::asio::buffer(data_, max_length),
            [this, self](boost::system::error_code ec, std::size_t length) {
                if (!ec) {
                    std::string message(data_, length);
                    std::cout << "[<] Received from " << socket_.remote_endpoint()
                              << ": " << message;

                    // Echo back the received message
                    std::string reply = "Echo: " + message;
                    do_write(reply);
                } else if (ec != boost::asio::error::eof) {
                    std::cerr << "[-] Read error: " << ec.message() << std::endl;
                } else {
                    std::cout << "[-] Client disconnected: "
                              << socket_.remote_endpoint() << std::endl;
                }
            });
    }

    void do_write(const std::string& msg) {
        auto self(shared_from_this());
        boost::asio::async_write(
            socket_,
            boost::asio::buffer(msg),
            [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                if (!ec) {
                    do_read();
                } else {
                    std::cerr << "[-] Write error: " << ec.message() << std::endl;
                }
            });
    }

    tcp::socket socket_;
    enum { max_length = 1024 };
    char data_[max_length];
};

// Server listening for incoming TCP connections
class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        std::cout << "[*] Server listening on port " << port << "..." << std::endl;
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                } else {
                    std::cerr << "[-] Accept error: " << ec.message() << std::endl;
                }
                do_accept();
            });
    }

    tcp::acceptor acceptor_;
};

int main(int argc, char* argv[]) {
    try {
        short port = 8080;
        if (argc > 1) {
            port = static_cast<short>(std::atoi(argv[1]));
        }

        boost::asio::io_context io_context;
        Server s(io_context, port);

        // Run the Asio event processing loop
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
