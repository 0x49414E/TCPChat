#include <iostream>
#include <boost/asio.hpp>
#include <array>

using boost::asio::ip::tcp;

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver {io_context};

        tcp::resolver::results_type endpoints = resolver.resolve("127.0.0.1", "4000");
        tcp::socket socket {io_context};

        boost::asio::connect(socket, endpoints);

        for(;;)
        {
            std::array<char,128> buff {0};
            boost::system::error_code error;

            size_t len = socket.read_some(boost::asio::buffer(buff), error);

            if(error == boost::asio::error::eof)
            {
                break;
            }
            else if(error)
            {
                throw boost::system::system_error(error);
            }

            for(char& c : buff)
            {
                std::cout << c;
            }
        }
    }
    catch(std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        return -1;
    }
    return 0;
}
