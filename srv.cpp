#include "pch.hpp"
#include <memory>

constexpr int port = 10055;

void listen(uvw::Loop &loop) {
    std::shared_ptr<uvw::TCPHandle> tcp = loop.resource<uvw::TCPHandle>();

    tcp->once<uvw::ListenEvent>([](const uvw::ListenEvent &, uvw::TCPHandle &srv) {
        std::shared_ptr<uvw::TCPHandle> client = srv.loop().resource<uvw::TCPHandle>();

        client->on<uvw::CloseEvent>([ptr = srv.shared_from_this()](const uvw::CloseEvent &, uvw::TCPHandle &) { ptr->close(); });
        client->on<uvw::EndEvent>([](const uvw::EndEvent &, uvw::TCPHandle &client) { client.close(); });
        client->on<uvw::DataEvent>([](const uvw::DataEvent &data, uvw::TCPHandle &client) {
            std::cout << std::string_view(data.data.get(), data.length) << std::endl;
        });

        srv.accept(*client);
        client->read();
    });

    tcp->bind("0.0.0.0", port);
    tcp->listen();
}

void listen_udp(uvw::Loop &loop) {
    std::shared_ptr<uvw::UDPHandle> udp = loop.resource<uvw::UDPHandle>();

    udp->on<uvw::UDPDataEvent>([](const uvw::UDPDataEvent &data, uvw::UDPHandle &srv){
        std::cout << "dataev: " << data.sender.ip << ":" << data.sender.port << std::endl;
        std::cout << "recv: " << std::string_view(data.data.get(), data.length) << std::endl;

        auto echo_data = std::make_unique<char[]>(data.length);
        std::copy(data.data.get(), data.data.get() + data.length, echo_data.get());
        srv.send(data.sender, std::move(echo_data), data.length);
    });

    udp->bind("0.0.0.0", port);
    udp->recv();
}

int main() {
    auto loop = uvw::Loop::getDefault();
    listen_udp(*loop);
    loop->run();
}