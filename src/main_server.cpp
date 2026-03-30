#include <cstdlib>
#include <ctime>
#include "GameServer.h"
#include "Protocol.h"

int main() {
    srand(time(nullptr));

    GameServer server(SERVER_PORT);
    server.run();

    return 0;
}
