#ifndef SERVER_CLIENTFORSERVER_H
#define SERVER_CLIENTFORSERVER_H

#include "BaseSession.h"
#include "Fsm.h"
#include <mysql_connection.h>
#include <string>

class ClientForServer : public BaseSession {
private:
    sql::Connection *conn;
    sql::Driver *driver;
    std::string schema;
public:
    ClientForServer(CLIENT_DESC id) {
        state = AUTHORIZATION;
        fd = id;
    }

    sql::Connection *get_connection() {
        return this->conn;
    }

    STATE get_state() {
        return this->state;
    }

    void set_state(STATE st) {
        this->state = st;
    }

    void set_connection(sql::Connection *conn) {
        this->conn = conn;
    }

    std::string &set_schema() {
        return this->schema;
    }

    ~ClientForServer() {
        delete conn;
    }

};

#endif //SERVER_CLIENTFORSERVER_H
