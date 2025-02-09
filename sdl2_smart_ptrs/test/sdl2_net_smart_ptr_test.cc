#if (_CATCH_VERSION_MAJOR == 3)
  #include <catch2/catch_version_macros.hpp>                     // CATCH_VERSION_MAJOR
  #include <catch2/catch_test_macros.hpp>                        // TEST_CASE, SECTION, REQUIRE
#elif (_CATCH_VERSION_MAJOR == 2)
  #include <catch2/catch.hpp>
#endif

#include "sdl2_net_smart_ptr.hh"

#include <SDL.h>
#include <SDL_net.h>

#include <string>


static std::string collectErrorQuitSdlNet(const std::string& func_name) {
    std::string err { SDL_GetError() };
    SDL_ClearError();
    if (err.size() == 0) {
        err = "failure without setting SDL error";
    }
    SDLNet_Quit();
    SDL_Quit();
    return func_name + ": " + err;
}

using namespace sdl2_smart_ptr;

TEST_CASE("SDL_net allocations: SDLNet_SocketSet",
    "[sdl2_smart_ptr][SDL2][SDL_net][SDLNet_SocketSet]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdlNet("SDL_Init"));
    }

    if (SDLNet_Init() != 0) {
        FAIL(collectErrorQuitSdlNet("SDLNet_Init"));
    }

    // _SDLNet_SocketSet*
    SDLNet_SocketSet socketset {
        SDLNet_AllocSocketSet(4)
    };
    if (socketset == nullptr) {
        SKIP(collectErrorQuitSdlNet("SDLNet_AllocSocketSet"));
    }

    deleter::SocketSet dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(socketset);
    }
    SECTION("unique:: ctor")
    {
        unique::SocketSet up_socketset{socketset, dltr};
        REQUIRE(up_socketset.get() == socketset);
    }
    SECTION("make_unique")
    {
        auto up_socketset{ make_unique(socketset) };
        REQUIRE(up_socketset.get() == socketset);
    }
    SECTION("shared:: ctor")
    {
        shared::SocketSet sp_socketset{socketset, dltr};
        REQUIRE(sp_socketset.get() == socketset);
    }
    SECTION("make_shared")
    {
        auto sp_socketset{ make_shared(socketset) };
        REQUIRE(sp_socketset.get() == socketset);
    }

    SDLNet_Quit();
    SDL_Quit();
}

TEST_CASE("SDL_net allocations: TCPsocket",
    "[sdl2_smart_ptr][SDL2][SDL_net][TCPsocket]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdlNet("SDL_Init"));
    }

    if (SDLNet_Init() != 0) {
        FAIL(collectErrorQuitSdlNet("SDLNet_Init"));
    }

    IPaddress address;
    if (SDLNet_ResolveHost(&address,
                           nullptr,  // INADDR_ANY
                           0         // OS selects port
            ) != 0) {
        FAIL(collectErrorQuitSdlNet("SDLNet_ResolveHost"));
    }

    // _TCPsocket*
    TCPsocket tcpsocket {
        SDLNet_TCP_Open(&address)
    };
    if (tcpsocket == nullptr) {
        SKIP(collectErrorQuitSdlNet("SDLNet_TCP_Open"));
    }

    deleter::TcpSocket dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(tcpsocket);
    }
    SECTION("unique:: ctor")
    {
        unique::TcpSocket up_tcpsocket{tcpsocket, dltr};
        REQUIRE(up_tcpsocket.get() == tcpsocket);
    }
    SECTION("make_unique")
    {
        auto up_tcpsocket{ make_unique(tcpsocket) };
        REQUIRE(up_tcpsocket.get() == tcpsocket);
    }
    SECTION("shared:: ctor")
    {
        shared::TcpSocket sp_tcpsocket{tcpsocket, dltr};
        REQUIRE(sp_tcpsocket.get() == tcpsocket);
    }
    SECTION("make_shared")
    {
        auto sp_tcpsocket{ make_shared(tcpsocket) };
        REQUIRE(sp_tcpsocket.get() == tcpsocket);
    }

    SDLNet_Quit();
    SDL_Quit();
}

TEST_CASE("SDL_net allocations: UDPpacket",
    "[sdl2_smart_ptr][SDL2][SDL_net][UDPpacket]")
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        FAIL(collectErrorQuitSdlNet("SDL_Init"));
    }

    if (SDLNet_Init() != 0) {
        FAIL(collectErrorQuitSdlNet("SDLNet_Init"));
    }

    UDPpacket* udppacket {
        SDLNet_AllocPacket(100)
    };
    if (udppacket == nullptr) {
        SKIP(collectErrorQuitSdlNet("SDLNet_AllocPacket"));
    }

    deleter::UdpPacket dltr;

    SECTION("direct use of deleter after manual allocation")
    {
        dltr(udppacket);
    }
    SECTION("unique:: ctor")
    {
        unique::UdpPacket up_udppacket{udppacket, dltr};
        REQUIRE(up_udppacket.get() == udppacket);
    }
    SECTION("make_unique")
    {
        auto up_udppacket{ make_unique(udppacket) };
        REQUIRE(up_udppacket.get() == udppacket);
    }
    SECTION("shared:: ctor")
    {
        shared::UdpPacket sp_udppacket{udppacket, dltr};
        REQUIRE(sp_udppacket.get() == udppacket);
    }
    SECTION("make_shared")
    {
        auto sp_udppacket{ make_shared(udppacket) };
        REQUIRE(sp_udppacket.get() == udppacket);
    }

    SDLNet_Quit();
    SDL_Quit();
}
