#include "Server.h"

Server::Server() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(0);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    if (bind(this->get_desc(), (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("bind");
        exit(1);
    }
    int opt = 1;
    listen(fd, SOMAXCONN);
    fcntl(fd, F_SETFL, O_NONBLOCK);
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));
    listening = true;
    timeout.tv_sec = 1000;
    timeout.tv_usec = 0;
    memset(buffer, 0, sizeof(buffer));
}

void Server::loop() {
    while (listening) {
        fd_set readset;
        FD_ZERO(&readset);
        FD_SET(fd, &readset);

        for (vector<ClientForServer *>::iterator it = clients.begin(); it != clients.end(); it++) {
            FD_SET((*it)->get_desc(), &readset);
        }

//        auto max_client = max_element(clients.begin(), clients.end(), [](const ClientForServer* cl1, const ClientForServer* cl2) {
//            return cl1->get_desc() > cl2->get_desc();
//        });
//        int max_cl = ((*max_client)->get_desc()) ? (*max_client)->get_desc() : 0;
        int mx = max(fd, 10);
        int res = select(mx + 1, &readset, NULL, NULL, &timeout);
        if (res <= 0) {
            cout << "Connection timed out ...";
            listening = false;
            exit(EXIT_SUCCESS);
        }
        if (FD_ISSET(fd, &readset)) {
            int client_socket = accept(fd, NULL, NULL);
            if (client_socket < 0) {
                perror("accept");
            }
            bool found = false;
            for (vector<ClientForServer *>::iterator it = clients.begin(); it != clients.end(); it++) {
                if ((*it)->get_desc() == client_socket) {
                    found = true;
                }
            }
            if (!found) {
                ClientForServer *newClient = new ClientForServer(client_socket);
                clients.push_back(newClient);
            }
            fcntl(client_socket, F_SETFL, O_NONBLOCK);
        }
        for (vector<ClientForServer *>::iterator it = clients.begin(); it != clients.end(); it++) {
            if (FD_ISSET((*it)->get_desc(), &readset)) {
                switch ((*it)->get_state()) {
                    case AUTHORIZATION: {
                        string name;
                        string password;
                        string schema;
                        recv((*it)->get_desc(), buffer, sizeof(buffer), 0);
                        stringstream ss(buffer);
                        memset(buffer, 0, sizeof(buffer));
                        ss >> name >> password >> schema;
                        bool result = authorize(**it, name, password, schema);
                        if (result) {
                            char message[] = "Authorized successfully!";
                            (*it)->set_state(REQUESTING);
                            send((*it)->get_desc(), message, sizeof(message), 0);
                        }
                        break;
                    }
                    case REQUESTING: {
                        int option;
                        recv((*it)->get_desc(), buffer, sizeof(buffer), 0);
                        stringstream ss(buffer);
                        memset(buffer, 0, sizeof(buffer));
                        ss >> option;
                        switch (option) {
                            case 1: {
                                string name;
                                string last_name;
                                string phone_number;
                                ss >> name >> last_name >> phone_number;
                                add_user(**it, name, last_name, phone_number);
                                break;
                            }
                            case 2: {
                                break;
                            }
                            case 3: {
                                break;
                            }
                        }
                        break;
                    }
                    case CLOSE:
                        FD_CLR((*it)->get_desc(), &readset);
                        close((*it)->get_desc());
                        delete *it;
                        clients.erase(it);
                        break;
                }
            }
        }
    }
}


bool Server::authorize(ClientForServer &client, const string &name, const string &last_name, const string &schema) {
    sql::Connection *conn;
    sql::Driver *driver;
    driver = get_driver_instance();
    conn = driver->connect("tcp://127.0.0.1:3306", name, last_name);
    if (conn) {
        client.set_connection(conn);
        client.get_connection()->setSchema(schema);
        return true;
    }
    return false;
}

void Server::add_user(ClientForServer &client, const string &name, const string &last_name, const string &phone) {
    sql::Statement *stmt;
    stmt = client.get_connection()->createStatement();
    bool res = stmt->execute(
            "INSERT INTO `phone_book` (name, last_name, phone) values (\"" + name + "\", \"" + last_name + "\" ,\"" + phone +
            "\");");
    if (res) {
        char message[] = "User has been added successfully!\n";
        send(client.get_desc(), message, sizeof(message), 0);
    } else {
        char fail[] = "Could not insert data. Try again. Check fields for data type";
        send(client.get_desc(), fail, sizeof(fail), 0);
    }
}


Server::~Server() {
    for (vector<ClientForServer *>::iterator it = clients.begin(); it != clients.end(); it++) {
        delete *it;
    }
}
