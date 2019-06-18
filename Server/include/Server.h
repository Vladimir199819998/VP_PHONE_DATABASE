#ifndef SERVER_SERVER_H
#define SERVER_SERVER_H
#include <vector>
#include <algorithm>
#include <utility>
#include <sstream>
#include <cstring>
#include <mysql_connection.h>
#include <driver.h>
#include <statement.h>
#include <resultset.h>
#include <fcntl.h>
#include "Fsm.h"
#include "BaseSession.h"
#include "Options.h"
#include "ClientForServer.h"

using namespace std;


class Server : public BaseSession
{
private:
    vector<ClientForServer*> clients;
    bool listening;
    struct timeval timeout;
    char buffer[1024];
public:
    Server();
    void loop();
    bool authorize(ClientForServer& client, const string& name, const string& last_name, const string& schema);
    void process_request();
    void add_user(ClientForServer& client, const string& name, const string& last_name, const string& phone);
    void edit_user();
    void delete_user();
    virtual ~Server();
};


#endif //SERVER_SERVER_H
