cmake_minimum_required(VERSION 3.16)
project(MultiPacman)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_EXE_LINKER_FLAGS "-static")
set(SFML_STATIC_LIBRARIES ON)
find_package(SFML 3 COMPONENTS Network System REQUIRED)

set(GAME_SOURCES
    src/map.cpp
    src/Entity.cpp
    src/Player.cpp
    src/Ghost.cpp
    src/Dot.cpp
    src/Game.cpp
)

add_executable(multipacman
    src/main.cpp
    ${GAME_SOURCES}
    src/ConsoleRenderer.cpp
)

add_executable(multipacman_server
    src/main_server.cpp
)
target_link_libraries(multipacman_server PRIVATE SFML::Network SFML::System ws2_32)

add_executable(multipacman_client
    src/main_client.cpp
)
target_link_libraries(multipacman_client PRIVATE SFML::Network SFML::System ws2_32)
