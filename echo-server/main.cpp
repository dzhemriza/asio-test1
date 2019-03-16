// main.cpp
#include <iostream>
#include <memory>
#include <atomic>
#include <boost/asio.hpp>

using boost::asio::ip::tcp;

class echo_session : public std::enable_shared_from_this<echo_session>
{
public:
  echo_session(tcp::socket socket, int session_id)
    : socket_(std::move(socket)),
      session_id_(session_id)
  {
    std::cout << "session created session_id=" << session_id << std::endl;
  }

  ~echo_session()
  {
    std::cout << "session destroyed session_id=" << session_id_ << std::endl;
  }

  void start()
  {
    read();
  }

  echo_session(const echo_session&) = delete;
  echo_session& operator=(const echo_session&) = delete;

private:
  void read()
  {
    auto self(shared_from_this());
    socket_.async_read_some(boost::asio::buffer(data_, max_len),
      [this, self](boost::system::error_code ec, std::size_t len)
      {
        if (!ec)
        {
          std::cout << "Bytes received of size=" << len << std::endl;
          write(len);
        }
      });
  }

  void write(std::size_t len)
  {
    auto self(shared_from_this());
    boost::asio::async_write(socket_, boost::asio::buffer(data_, len),
      [this, self](boost::system::error_code ec, std::size_t len)
      {
        if (!ec)
        {
          std::cout << "Bytes written size=" << len << std::endl;
          read();
        }
      });
  }

private:
  tcp::socket socket_;
  enum { max_len = 2048 };
  char data_[max_len];
  int session_id_;
};

class server
{
public:
  server(boost::asio::io_service& io_service, short port)
    : acceptor_(io_service, tcp::endpoint(tcp::v4(), port)),
      socket_(io_service),
      session_id_(0)

  {
    accept();
  }

  server(const server&) = delete;
  server& operator=(const server&) = delete;

private:
  void accept()
  {
    acceptor_.async_accept(socket_,
      [this](boost::system::error_code ec)
      {
        if (!ec)
        {
          std::make_shared<echo_session>(std::move(socket_), ++session_id_)->start();
        }
        else
        {
          std::cout << "error: server::accept ec=" << ec << std::endl;
        }

        accept(); // continue accepting
      });
  }

private:
  tcp::acceptor acceptor_;
  tcp::socket socket_;
  std::atomic<int> session_id_;
};

int main(int argc, char** argv)
{
  enum { port = 8043 };
  std::cout << "starting asio-test1 server on port=" << port << std::endl;

  try
  {
    boost::asio::io_service io_service;

    server s(io_service, port);

    io_service.run();
  }
  catch (std::exception& ex)
  {
    std::cout << "error: main: exception: ex=" << ex.what() << std::endl;
  }

  return 0;
}
