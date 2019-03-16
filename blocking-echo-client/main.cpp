// main.cpp
#include <iostream>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

int main(int argc, char** argv)
{
  try
  {
    enum { port = 8043, max_len = 2048, times = 1000000 };
    std::cout << "blocking-echo-client..." << std::endl;
    std::cout << "connecting to localhost:" << port << "..." << std::endl;

    boost::asio::io_service io_service;

    tcp::socket socket(io_service);

    tcp::resolver resolver(io_service);
    boost::asio::connect(socket,
                         resolver.resolve({"localhost", std::to_string(port)}));
    std::cout << "connecting to localhost:" << port << "...ok" << std::endl;

    const std::string request = "*** THIS IS TEST REQUEST ECHO DATA ***";

    for (size_t i = 0; i < times; ++i)
    {
      std::cout << "iteration#" << i << std::endl;

      std::cout << "write to socket..." << std::endl;
      boost::asio::write(socket,
                         boost::asio::buffer(request.c_str(), request.size()));
      std::cout << "write to socket...ok" << std::endl;

      // read the echo back
      std::cout << "read from socket..." << std::endl;
      char response[max_len] = {0};

      boost::system::error_code error;
      size_t replay_size = socket.read_some(boost::asio::buffer(response, max_len), error);
      std::cout << "read from socket...ok" << std::endl;

      if (error)
      {
        std::cout << "error: reading from socket, error=" << error << std::endl;
      }

      std::cout << "Reply: ";
      std::cout.write(response, replay_size);
      std::cout << std::endl;
    }
  }
  catch (std::exception& ex)
  {
    std::cout << "error: main: exception: ex=" << ex.what() << std::endl;
  }

  return 0;
}
