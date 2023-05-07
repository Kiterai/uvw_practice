#include "pch.hpp"
#include <memory>

constexpr int port = 10055;

void conn(uvw::Loop &loop) {
    // auto tcp = loop.resource<uvw::TCPHandle>();

    // tcp->on<uvw::ErrorEvent>([](const uvw::ErrorEvent &, uvw::TCPHandle &) { /* handle errors */ });

    // tcp->once<uvw::ConnectEvent>([](const uvw::ConnectEvent &, uvw::TCPHandle &tcp) {
    //     std::unique_ptr<char[]> dataWrite;

    //     std::string str = "hello";
    //     dataWrite = std::make_unique<char[]>(str.length());
    //     std::copy(str.begin(), str.end(), dataWrite.get());

    //     tcp.write(std::move(dataWrite), str.length());
    //     tcp.close();
    // });

    // tcp->connect(std::string{"127.0.0.1"}, 10055);

    std::shared_ptr<uvw::UDPHandle> udp = loop.resource<uvw::UDPHandle>();

    udp->once<uvw::SendEvent>([](const uvw::SendEvent &data, uvw::UDPHandle &cli){
        std::cout << "first sendev" << std::endl;
        std::cout << "sock: " << cli.sock().ip << ":" << cli.sock().port << std::endl;
    });

    udp->once<uvw::UDPDataEvent>([](const uvw::UDPDataEvent &data, uvw::UDPHandle &srv){
        std::cout << "dataev: " << data.sender.ip << ":" << data.sender.port << std::endl;
        std::cout << "recv: " << std::string_view(data.data.get(), data.length) << std::endl;
        srv.close();
    });

    udp->bind("127.0.0.1", 0);
    udp->recv();
    
    auto dataTrySend = std::unique_ptr<char[]>(new char[5]{'h', 'e', 'l', 'l', 'o'});
    udp->send("127.0.0.1", port, std::move(dataTrySend), 5);
}

int main() {
    auto loop = uvw::Loop::getDefault();
    conn(*loop);
    loop->run();
}
