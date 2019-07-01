#include "Client.h"

Client::Client() {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        perror("socket");
        exit(0);
    }
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = htonl(INADDR_LOOPBACK);
    state = AUTHORIZATION;
    if (connect(fd, (struct sockaddr *) &addr, sizeof(addr)) < 0) {
        perror("connect");
        exit(1);
    }
}

void Client::loop() {
    for (;;) {
        switch (state) {
            case AUTHORIZATION:
                authorize();
                break;
            case REQUESTING:
                send_requests();
                break;
            case CLOSE:
                close();
                break;
        }
    }

}

void Client::authorize() {
    while (state == AUTHORIZATION) {
        string buffer;
        cout << "Enter your username: ";
        cin >> username;
        cout << "Enter your password: ";
        cin >> password;
        cout << "Set database schema: ";
        cin >> schema;
        buffer = username + " " + password + " " + schema;
        strcpy(request, buffer.c_str());
        send(fd, request, sizeof(request), 0);
        int bytes_read = recv(fd, response, sizeof(response), 0);
        if (bytes_read <= 0) {
            cout << "Lost connection with the server ... Terminating ...\n";
            exit(2);
        } else {
            state = REQUESTING;
            cout << response << endl;
            cout << "Now you are able to send requests. Good luck!\n\n";
            memset(response, 0, sizeof(response));
        }
    }
}

void Client::send_requests() {
    while (state == REQUESTING) {
        cout << "You are provided with the following options: \n";
        cout << ADD_USER << " : add user\n";
        cout << EDIT_USER << " : edit user\n";
        cout << DELETE_USER << " : delete user\n";
        cout << QUIT << " : quit\n";
        cin >> user_option;
        switch (user_option) {
            case 0: {
                cout << "Bye ...\n";
                state = CLOSE;
                break;
            }

            case 1: {
                string name;
                string last_name;
                string phone;
                cout << "Enter name: ";
                cin >> name;
                cout << "Enter last name: ";
                cin >> last_name;
                cout << "Enter phone number :";
                cin >> phone;
                string buf;
                buf += to_string(user_option) + " " + name + + " " + last_name + " " + phone;
                strcpy(request, buf.c_str());
                send(fd, request, strlen(request), 0);
                recv(fd, response, sizeof(response), 0);
                if (strlen(response) <= 0) {
                    perror("Cannot add a user, check out your data");
                } else {
                    cout << "User has been added successfully!\n";
                }
                break;
            }
            case 2: {
                cout << "Option is temporarily unavailable\n";
                break;
            }
            case 3: {
                cout << "Option is temporarily unavailable\n";
                break;
            }


        }
    }
}

void Client::close() {
    cout << "Closing the connection ... Good Bye...\n";
    exit(EXIT_SUCCESS);
}

