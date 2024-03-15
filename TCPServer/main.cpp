#include <boost/asio.hpp>
#include <iostream>
#include <Networking/server.h>

int main()
{
    TCP::Server server(4000);

    server.OnJoin = [](std::shared_ptr<TCP::Sock>& server){
        std::cout << "User has joined the server: " << server->GetUsername() << std::endl;
    };

    server.OnLeave = [](std::shared_ptr<TCP::Sock>& server){
        std::cout << "User has left the server: " << server->GetUsername() << std::endl;
    };

    server.OnClientMessage = [&server](const std::string& message){
        server.Broadcast(message);
    };

    server.Run();

    return 0;
}
