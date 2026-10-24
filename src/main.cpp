#include <iostream>
#include <string>
#include <utility>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;
using boost::asio::awaitable;
using boost::asio::co_spawn;
using boost::asio::detached;
using boost::asio::use_awaitable;
namespace this_coro = boost::asio::this_coro;

// Coroutine handling an individual client session
awaitable<void> session(tcp::socket socket) {
    try {
        auto client_endpoint = socket.remote_endpoint();
        std::cout << "[+] New client connected from: " << client_endpoint << std::endl;

        std::string welcome = "Welcome to the Boost.Asio TCP Echo Server (C++20 Coroutines)!\r\nType something and press Enter:\r\n";
        co_await boost::asio::async_write(socket, boost::asio::buffer(welcome), use_awaitable);

        char data[1024];
        while (true) {
            std::size_t length = co_await socket.async_read_some(
                boost::asio::buffer(data), use_awaitable);

            std::string message(data, length);
            std::cout << "[<] Received from " << client_endpoint << ": " << message;

            // Echo back the received message
            std::string reply = "Echo: " + message;
            co_await boost::asio::async_write(
                socket, boost::asio::buffer(reply), use_awaitable);
        }
    } catch (const boost::system::system_error& se) {
        if (se.code() == boost::asio::error::eof) {
            std::cout << "[-] Client disconnected normally." << std::endl;
        } else {
            std::cerr << "[-] Session error: " << se.what() << std::endl;
        }
    } catch (const std::exception& e) {
        std::cerr << "[-] Exception in session: " << e.what() << std::endl;
    }
}

// Coroutine listening for incoming TCP connections
awaitable<void> listener(unsigned short port) {
    auto executor = co_await this_coro::executor;
    tcp::acceptor acceptor(executor, tcp::endpoint(tcp::v4(), port));

    std::cout << "[*] Server listening on port " << port << " (C++20 Coroutines)..." << std::endl;

    while (true) {
        tcp::socket socket = co_await acceptor.async_accept(use_awaitable);
        // Spawn a new independent coroutine for each connected client
        co_spawn(executor, session(std::move(socket)), detached);
    }
}

int main(int argc, char* argv[]) {
    try {
        unsigned short port = 8080;
        if (argc > 1) {
            port = static_cast<unsigned short>(std::atoi(argv[1]));
        }

        boost::asio::io_context io_context;

        // Graceful shutdown on SIGINT and SIGTERM
        boost::asio::signal_set signals(io_context, SIGINT, SIGTERM);
        signals.async_wait([&](const boost::system::error_code&, int) {
            std::cout << "\n[*] Shutting down server..." << std::endl;
            io_context.stop();
        });

        // Spawn the listening coroutine on the io_context
        co_spawn(io_context, listener(port), detached);

        // Run the event loop
        io_context.run();
    } catch (const std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
