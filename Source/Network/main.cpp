#include "Network.hpp"
#include <iostream>
#include <string>
#include <memory>

int main() {
    auto ipAccess = std::make_shared<Network::IpAccess>();
    while (true) {
        system("cls");
        std::string ip;
        Network::SubnetMask mask;
        std::cout << "Enter Ipv4 or DNS: \n";
        std::cin >> ip;
        auto checkAccess = ipAccess->Check(ip.c_str(), 80);
        std::cout << "Status: " << (checkAccess ? "Available\n" : "Unavailable\n");
        if (checkAccess) {
            Network::AddressResolve resolve(ip, "80");
            std::cout << "Ipv4: " + resolve() << std::endl;
            std::cout << "Broadcast: " + resolve.GetNetwork().broadcast().to_string() << std::endl;
            Network::PrintDeviceMac();

            for(auto& dnsName : resolve.GetHostsName())
                std::cout << "\nDNS: " + dnsName << std::endl;
            std::cout << "\nDNS Size: " + std::to_string(resolve.GetHostsName().size()) << std::endl;

            auto hostSize = resolve.GetNetwork().hosts().size();
            std::cout << "Hosts: " + std::to_string(hostSize) << std::endl;
            std::cout << "Mask: " << mask.GetMask(resolve(), resolve());
        }
        std::string beginIp;
        std::string endIp;
        std::cout << "\nEnter begin IPv4: "; std::cin >> beginIp;
        std::cout << "\nEnter end IPv4: ";    std::cin >> endIp;
        std::cout << "Mask: " << mask.GetMask(beginIp, endIp) << std::endl;
        system("pause");
        std::cin.get();
    }
    return 0;
}