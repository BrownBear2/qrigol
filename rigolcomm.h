#ifndef RIGOLCOMM_H
#define RIGOLCOMM_H

#include <vxi11_user.h>

class RigolComm
{
protected:
    int fd;
    char ip[16];
    int get_data_size(int rawsize);

public:
    char *buffer;
    char lastCmd[256];
    ssize_t data_size;
    bool isConnected;
    VXI11_CLINK* clink;

    RigolComm();
    ~RigolComm();
    int open(const char *device);
    int close(void);
    bool connected(void) { return isConnected; }
    int send(const char *command);
    int unlock(void);
    int recv(void);
    float toFloat(void);
    float cmdFloat(const char *cmd);
    float cmdFloatlt(const char *cmd);
    int command(const char *cmd);
};

#endif // RIGOLCOMM_H
