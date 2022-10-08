#pragma once
#include <boost/asio.hpp>
#include <Windows.h>
#include <stdio.h>
#include <iphlpapi.h>

#pragma comment(lib, "IPHLPAPI.lib")

namespace Network {
    using io_service = boost::asio::io_service;

    class SubnetMask final {
    private:
        unsigned char mBytes[4];
    public:
        const std::string GetMask(const std::string from, const std::string to) noexcept;
    };

    class AddressResolve final {
    private:
        boost::asio::ip::udp::resolver::iterator mIterator;
    public:
        AddressResolve(const std::string address, const std::string port);

        const std::string operator() () const;

        const boost::asio::ip::network_v4 GetNetwork() const;

        const std::vector<std::string> GetHostsName() const;

        ~AddressResolve() = default;
    };

    class IpAccess final {
    public:
        IpAccess() = default;

        const bool Check(const std::string address, const boost::asio::ip::port_type port) const;

        ~IpAccess() = default;
    };

    static void PrintDeviceMac() {
        IP_ADAPTER_INFO *info = NULL, *pos;
        DWORD size = 0;
        GetAdaptersInfo(info, &size);
        info = (IP_ADAPTER_INFO *)malloc(size);
        GetAdaptersInfo(info, &size);
        for (pos=info; pos!=NULL; pos=pos->Next) {
            printf("\n%s\n\t", pos->Description);
            printf("%2.2x", pos->Address[0]);
            for (int i=1; i<pos->AddressLength; i++)
                printf(":%2.2x", pos->Address[i]);
        }
        free(info);
    }
} // Network