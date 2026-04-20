cmake_minimum_required(VERSION 3.16)
project(MultiPacman)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(SFML_STATIC_LIBRARIES ON)

find_package(SFML 3 COMPONENTS Graphics Network System REQUIRED)

set_target_properties(freetype PROPERTIES
    IMPORTED_LOCATION "C:/msys64/ucrt64/lib/libfreetype.a"
    INTERFACE_LINK_LIBRARIES "bz2;png16;z;harfbuzz;graphite2;brotlidec;brotlicommon;rpcrt4;usp10;dwrite;glib-2.0;intl;pcre2-8;iconv;shlwapi;ole32"
)

include_directories(src src/model src/model/entity src/model/strategy src/network src/rendering src/ui)

set(GAME_SOURCES
    src/model/Map.cpp
    src/model/entity/Entity.cpp
    src/model/entity/Player.cpp
    src/model/entity/Ghost.cpp
    src/model/entity/Dot.cpp
    src/model/Game.cpp
    src/model/strategy/ChaserStrategy.cpp
    src/model/strategy/AmbusherStrategy.cpp
    src/model/strategy/PatrolStrategy.cpp
    src/model/strategy/RandomStrategy.cpp
    src/model/strategy/BotStrategy.cpp
)

add_executable(multipacman_server
    src/main_server.cpp
    src/network/GameServer.cpp
    src/network/ClientConnection.cpp
    ${GAME_SOURCES}
)
target_link_libraries(multipacman_server PRIVATE SFML::Network SFML::System ws2_32)

add_executable(multipacman_client
    src/main_client.cpp
    src/network/GameClient.cpp
    src/rendering/SfmlRenderer.cpp
    src/ui/MainMenuScreen.cpp
    src/ui/ConnectScreen.cpp
    src/ui/LobbyScreen.cpp
    src/ui/GameScreen.cpp
    ${GAME_SOURCES}
    src/rendering/ConsoleRenderer.cpp
)
target_link_libraries(multipacman_client PRIVATE SFML::Graphics SFML::Network SFML::System ws2_32)
target_link_options(multipacman_client PRIVATE -mwindows)

add_executable(multipacman_console
    src/main_client.cpp
    src/network/GameClient.cpp
    src/rendering/SfmlRenderer.cpp
    src/ui/MainMenuScreen.cpp
    src/ui/ConnectScreen.cpp
    src/ui/LobbyScreen.cpp
    src/ui/GameScreen.cpp
    ${GAME_SOURCES}
    src/rendering/ConsoleRenderer.cpp
)
target_link_libraries(multipacman_console PRIVATE SFML::Graphics SFML::Network SFML::System ws2_32)
target_compile_definitions(multipacman_console PRIVATE CONSOLE_MODE)

file(COPY ${CMAKE_SOURCE_DIR}/maps DESTINATION ${CMAKE_BINARY_DIR})
