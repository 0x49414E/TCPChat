#ifndef TCPCHAT_SOCK_H
#define TCPCHAT_SOCK_H
#include <iostream>
#include <boost/asio.hpp>
#include <memory>
#include <queue>

using boost::asio::ip::tcp;

namespace TCP
{
    using MessageHandler = std::function<void(std::string)>;
    using ErrorHandler = std::function<void()>;

class Sock : public std::enable_shared_from_this<Sock> {
public:
        static std::shared_ptr<Sock> Create(tcp::socket&& socket)
        {
            return std::make_shared<TCP::Sock>(std::move(socket));
        };
        Sock(tcp::socket&& socket);
        tcp::socket& getSocket(){
            return socket;
        }
        static void Handler(const boost::system::error_code& error, size_t bytes);
        void Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler);
        void SendMessage(const std::string& message);
        inline const std::string& GetUsername() const { return username; }

    private:
        void async_read();
        void on_read(boost::system::error_code& ec, size_t bytes);

        void async_write();
        void on_write(boost::system::error_code& ec, size_t bytes);

    private:
        tcp::socket socket;
        std::string message {"HELLO.\nCONNECTED TO THE SERVER.\n"};
        std::string username;

        std::queue<std::string> outgoingMessages;
        boost::asio::streambuf streambuf {65536};

        MessageHandler messageHandler;
        ErrorHandler errorHandler;
    };
}

#endif //TCPCHAT_SOCK_H
