
#include "Networking/server.h"

using boost::asio::ip::tcp;

TCP::Server::Server(u_short _port) : port(_port)
, acceptor(io_context, tcp::endpoint(tcp::v4(), port))
, connections {} {}

int TCP::Server::Run()
{
    try
    {
        Accept();
        io_context.run();
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}

void TCP::Server::Broadcast(const std::string& message)
{
    for(auto& connection : connections)
    {
        connection->SendMessage(message);
    }
}

void TCP::Server::Accept()
{
    socket.emplace(io_context);

    acceptor.async_accept(*socket, [this](const boost::system::error_code& error)
    {
        std::shared_ptr<Sock> connection = TCP::Sock::Create(std::move(*socket));

        if(OnJoin)
        {
            OnJoin(connection);
        }

        connections.insert(connection);
        if(!error)
        {
            connection->Start(
                    [this](const std::string& message){
                        if(OnClientMessage) OnClientMessage(message);
                    },
                    [&, weak = std::weak_ptr(connection)]{
                        if(auto shared = weak.lock(); shared && connections.erase(shared)){
                            if(OnLeave) OnLeave(shared);
                        }
                    });
        }

        Accept();
    });

}