#include "BoostServerMain.h"

#include <boost/beast/core.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/asio.hpp>
#include <memory>
#include <string>
#include <iostream>

namespace beast = boost::beast;
namespace websocket = beast::websocket;
namespace net = boost::asio;
using tcp = boost::asio::ip::tcp;

// Classe Session
class Session : public std::enable_shared_from_this<Session> {
public:
    explicit Session(tcp::socket socket)
        : ws_(std::move(socket)) {}

    // Lance la session WebSocket
    void run() {
        ws_.async_accept([self = shared_from_this()](beast::error_code ec) {
            self->on_accept(ec);
            });
    }

    // Ferme proprement le WebSocket
    void close() {
        ws_.async_close(websocket::close_code::normal,
            [self = shared_from_this()](beast::error_code ec) {
                if (ec) {
                    std::cerr << "Erreur lors de la fermeture du socket : " << ec.message() << std::endl;
                }
                else {
                    std::cout << "Socket fermé proprement." << std::endl;
                }
            });
    }

private:
    websocket::stream<tcp::socket> ws_;

    // Callback d'acceptation
    void on_accept(beast::error_code ec) {
        if (ec) {
            std::cerr << "Erreur lors de l'acceptation : " << ec.message() << std::endl;
            return;
        }
        // Démarre la lecture après acceptation
        do_read();
    }

    // Lecture des messages
    void do_read() {
        ws_.async_read(buffer_,
            [self = shared_from_this()](beast::error_code ec, std::size_t bytes_transferred) {
                boost::ignore_unused(bytes_transferred);
                if (ec) {
                    std::cerr << "Erreur lors de la lecture : " << ec.message() << std::endl;
                    return;
                }
                // Traitement des données reçues ici
                std::cout << beast::buffers_to_string(self->buffer_.data()) << std::endl;

                // Efface le buffer après traitement
                self->buffer_.consume(self->buffer_.size());

                // Redémarre la lecture ou ferme la session si nécessaire
                self->do_read(); // Ou appelez `self->close();` ici pour fermer
            });
    }

    beast::flat_buffer buffer_;
};


class WebSocketServer {
public:
    WebSocketServer(net::io_context& ioc, tcp::endpoint endpoint)
        : acceptor_(ioc, endpoint) {
        accept();
    }

private:
    void accept() {
        acceptor_.async_accept([this](beast::error_code ec, tcp::socket socket) {
            if (!ec) std::make_shared<Session>(std::move(socket))->run();
            accept(); // Accepter les prochaines connexions
            });
    }

    tcp::acceptor acceptor_;
};

int main() {
    try {
        net::io_context ioc;
        tcp::endpoint endpoint(tcp::v4(), 5000);
        WebSocketServer server(ioc, endpoint);
        std::cout << "Serveur WebSocket en écoute sur ws://localhost:5000" << std::endl;
        ioc.stop();
        ioc.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Exception : " << e.what() << std::endl;
    }
}