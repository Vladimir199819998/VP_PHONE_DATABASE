#ifndef CLIENT_BASESESSION_H
#define CLIENT_BASESESSION_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <cstdio>
#include <unistd.h>
#include "Fsm.h"

typedef int CLIENT_DESC;

class BaseSession
{
protected:
    CLIENT_DESC fd;
    struct sockaddr_in addr;
    STATE state;
public:
    BaseSession() {
        state = CONNECTION;
    }
    CLIENT_DESC get_desc() const {
        return this->fd;
    }
    void set_desc(CLIENT_DESC desc) {
        this->fd = desc;
    }
    struct sockaddr_in& GetAddr() {
        return this->addr;
    }
    virtual ~BaseSession() {
        close(fd);
    }

};

#endif //CLIENT_BASESESSION_H
