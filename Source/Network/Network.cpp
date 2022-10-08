#include "Network.hpp"
#include <iostream>

namespace Network {
    AddressResolve::AddressResolve(std::string address, const std::string port = "80") {
        io_service ioService{};
        boost::asio::ip::udp::resolver resolver(ioService);
        boost::asio::ip::udp::resolver ::query query(address, port);
        this->mIterator = resolver.resolve(query);
    }

    const std::string AddressResolve::operator() () const {
        boost::asio::ip::udp::endpoint endPoint = mIterator->endpoint();
        return endPoint.address().to_string();
    }

    const boost::asio::ip::network_v4 AddressResolve::GetNetwork() const {
        boost::asio::ip::udp::endpoint endPoint = mIterator->endpoint();
        boost::asio::ip::network_v4 network = boost::asio::ip::make_network_v4(endPoint.address().to_string() + "/32");
        return network;
    }

    const std::vector<std::string> AddressResolve::GetHostsName() const {
        std::vector<std::string> hosts;
        io_service ioService;
        boost::asio::ip::udp::resolver resolver(ioService);
        auto itr = resolver.resolve(mIterator->endpoint());
        boost::asio::ip::udp::resolver::iterator end;
        for (int i = 1; itr != end; itr++, i++)
            hosts.push_back(itr->host_name());
        return hosts;
    }

    const bool Network::IpAccess::Check(const std::string address, const boost::asio::ip::port_type port = 80) const {
        io_service ioService{};
        boost::asio::ip::udp::socket sock(ioService);
        try {
            auto resolve = Network::AddressResolve(address);
            auto ipv4 = boost::asio::ip::address::from_string(resolve());
            boost::asio::ip::udp::endpoint endPoint(ipv4, port);
            sock.connect(endPoint);
        }
        catch (...) {
            return false;
        }
        return sock.is_open();
    }

    const std::string SubnetMask::GetMask(const std::string from, const std::string to) noexcept {
        std::string result{};
        auto begin = boost::asio::ip::address_v4::from_string(from);
        auto end = boost::asio::ip::address_v4::from_string(to);
        bool edge = false;
        for (int i = 0; i < 4; ++i) {
            for(auto b = (unsigned char)128; b >= (unsigned char)1; b /= 2) {
                if (!edge && ((begin.to_bytes()[i]&b) == (end.to_bytes()[i]&b)) )
                    mBytes[i] = b;
                else {
                    edge = true;
                    mBytes[i] = (unsigned char)(mBytes[i]&~b);
                }
            }
        }
        for (int j = 0; j < 4; ++j)
             mBytes[j] = (mBytes[j] == 1 ? 255 : 256-(int)mBytes[j]);
        return std::to_string(mBytes[0]) + '.' + std::to_string(mBytes[1]) + '.' +
            std::to_string(mBytes[2]) + '.' +std::to_string(mBytes[3]);
    }
} // Network

/*
int main() {
    auto ipAccess = std::make_shared<Network::IpAccess>();
    auto address = "5.255.255.80";
    std::cout << ipAccess->Check(address) << std::endl;
    if(ipAccess->Check(address)) {
        auto r = Network::AddressResolve(address);
        std::cout << r() << std::endl;
        std::cout << "Broadcast: " << r.GetNetwork().broadcast().to_string() << std::endl;
        std::cout << "HOST: " << r.GetHostsName().at(0) << std::endl;
    }
    Network::SubnetMask sm;
    std::cout << sm.GetMask("64.233.165.113", "64.233.165.113") << std::endl;

    std::cin.get();
    return 0;
}
 */