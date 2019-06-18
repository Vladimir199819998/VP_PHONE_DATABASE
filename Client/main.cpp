#include "Client.h"

int main() {
    Client* cl = new Client;
    cl->loop();
    delete cl;
    return 0;
}