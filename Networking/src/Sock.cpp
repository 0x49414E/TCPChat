#include "Networking/Sock.h"

TCP::Sock::Sock(tcp::socket&& socket)
: socket(std::move(socket))
{
    boost::system::error_code ec;
    std::stringstream name;
    name << socket.remote_endpoint();

    username = name.str();
};

void TCP::Sock::Handler(const boost::system::error_code& error, size_t bytes)
{
    if(error)
    {
        std::cout << "Error sending message..." << std::endl;
    } else{
        std::cout << "Sent " << bytes << " bytes" << std::endl;
    }
}

void TCP::Sock::async_read()
{
    boost::asio::async_read_until(socket,streambuf,"\n",[self = shared_from_this()](boost::system::error_code ec, size_t bytes){
        self->on_read(ec, bytes);
    });
}

void TCP::Sock::on_read(boost::system::error_code& ec, size_t bytes)
{
    if(ec)
    {
        socket.close(ec);

        errorHandler();
        return;
    }

    std::stringstream message;
    message << username << ": " << std::istream(&streambuf).rdbuf();

    messageHandler(message.str());
    async_read();
}

void TCP::Sock::SendMessage(const std::string& message)
{
    outgoingMessages.push(message);
    if(outgoingMessages.empty())
    {
        async_write();
    }
}

void TCP::Sock::async_write()
{
    boost::asio::async_write(socket,boost::asio::buffer(outgoingMessages.front()),[self = shared_from_this()](boost::system::error_code ec, size_t bytes){
        self->on_write(ec, bytes);
    });
}

void TCP::Sock::on_write(boost::system::error_code& ec, size_t bytes)
{
    if(ec)
    {
        socket.close(ec);
        errorHandler();
        return;
    }

    outgoingMessages.pop();

    if(!outgoingMessages.empty())
    {
        async_write();
    }
}


void TCP::Sock::Start(MessageHandler&& messageHandler, ErrorHandler&& errorHandler)
{
    this->messageHandler = std::move(messageHandler);
    this->errorHandler = std::move(errorHandler);
    async_read();
}