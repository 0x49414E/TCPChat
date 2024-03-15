
#ifndef TCPCHAT_SERVER_H
#define TCPCHAT_SERVER_H
#include <iostream>
#include <boost/asio.hpp>
#include <Networking/Sock.h>
#include <optional>
#include <unordered_set>

using boost::asio::ip::tcp;

namespace TCP
{
    class Server
    {
        using OnJoinHandler = std::function<void(std::shared_ptr<Sock>&)>;
        using OnLeaveHandler = std::function<void(std::shared_ptr<Sock>&)>;
        using OnClientMessageHandler = std::function<void(std::string)>;

    public:
        Server(u_short port);

        int Run();
        void Accept();
        void Broadcast(const std::string& message);
    public:
        OnJoinHandler OnJoin;
        OnLeaveHandler OnLeave;
        OnClientMessageHandler OnClientMessage;
    private:
        u_short port;
        boost::asio::io_context io_context;
        tcp::acceptor acceptor;
        std::optional<tcp::socket> socket;
        std::unordered_set<std::shared_ptr<Sock>> connections;
    };
}


#endif //TCPCHAT_SERVER_H
