#ifndef CLIENT_CLIENT_H
#define CLIENT_CLIENT_H

#include "BaseSession.h"
#include "Options.h"
#include <iostream>
#include <cstring>
using namespace std;

class Client : public BaseSession
{
private:
    string username;
    string password;
    string schema;
    enum Options option;
    int user_option;
    char request[256];
    char response[256];
public:
    Client();
    void loop();
    void authorize();
    void send_requests();
    void close();
    virtual ~Client() {};
};




#endif //CLIENT_CLIENT_H
