#ifndef CLIENT_FSM_H
#define CLIENT_FSM_H

typedef enum FSM {
    CONNECTION,
    AUTHORIZATION,
    REQUESTING,
    CLOSE,
} STATE;

#endif //CLIENT_FSM_H
