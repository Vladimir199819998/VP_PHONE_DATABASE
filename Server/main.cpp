#include "Server.h"
int main() {
    Server* serv = new Server;
    serv->loop();
    delete serv;
    return 0;
}