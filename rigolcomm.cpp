/*
 *
 *  This program is Copyright (c) 2015 by Al Williams al.williams@awce.com
 *  All rights reserved.
 *
    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
    */
#include "rigolcomm.h"
#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <locale.h>
#include <QDebug>


// The intent of this file is to just read/write
// the device in a very raw way

static const int max_cmd_len = 255;
static const int max_resp_len = 56000000+10; // 10 byte header on newer scopes

RigolComm::RigolComm()
{
    buffer = NULL;
    data_size = 0;
    isConnected = false;
}

RigolComm::~RigolComm()
{
    if (buffer) free(buffer);
    isConnected = false;
}

int RigolComm::open(const char *device)
{
    strcpy(ip, device);
    if (vxi11_open_device(&clink, device, "scope"))
        return 0;

    buffer = (char*)malloc(max_resp_len + 1);
    isConnected = true;
    return buffer != NULL;
}

int RigolComm::unlock()
{
    return send(":KEY:LOCK DISABLE");
}

int RigolComm::close(void)
{
    vxi11_close_device(clink, ip);
    isConnected = false;
    if (buffer) free(buffer);
    buffer = NULL;
    return 0;
}

int RigolComm::send(const char *command)
{
    strcpy(lastCmd, command);
    return vxi11_send(clink, command, strlen(command));
}

int RigolComm::recv(void)
{
    data_size = vxi11_receive(clink, buffer, max_resp_len);
    if (data_size > 2)
    {
        buffer[data_size - 1] = 0;
        data_size -= 1;
    }

    else if (data_size < -15)
    {
        qDebug() << "invalid command:" << lastCmd;
    }

    return data_size;
}

float RigolComm::toFloat(void)
{
#if 0
    float rv=-1.0f;
    if (recv()>=0) sscanf(buffer,"%f",&rv);
    return rv;
#else
    float rv=-1.0f;
       // the problem here is sscanf uses the user's locale
       // But the Rigol ALWAYS uses a '.' for decimal separator
       struct lconv *lc=localeconv();
       char dp=*lc->decimal_point;
       if (recv()<0) return rv;
       if (dp!='.')
       {
           char *p;
           do
           {
               p=strchr(buffer,'.');
               if (p) *p=dp;  // convert to local decimal point
           } while (p);
       }
       sscanf(buffer,"%f",&rv);
       return rv;
#endif

}

float RigolComm::cmdFloat(const char *cmd)
{
    if (send(cmd)<0) return -1.0f;
    return toFloat();
}

float RigolComm::cmdFloatlt(const char *cmd)
{
    float rv=-1.0f;
    if (send(cmd)<0) return -rv;
    if (recv()>=0) sscanf(buffer+1,"%f",&rv);
    return rv;
}


int RigolComm::command(const char *cmd)
{
    if (send(cmd)<=0)
        if (strchr(cmd,'?'))
            return recv();
    return 0;
}
