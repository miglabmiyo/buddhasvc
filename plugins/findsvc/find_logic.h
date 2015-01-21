#ifndef __BUDDHA__FINDSVC__FIND_LOGIC___
#define __BUDDHA__FINDSVC__FIND_LOGIC___
#include "net/find_comm_head.h"
#include "common.h"

#define DEFAULT_CONFIG_PATH     "./plugins/findsvc/findsvc_config.xml"
namespace findsvc_logic{

class Findlogic{

public:
    Findlogic();
    virtual ~Findlogic();

private:
    static Findlogic    *instance_;

public:
    static Findlogic *GetInstance();
    static void FreeInstance();

public:
    bool OnFindConnect (struct server *srv,const int socket);

    bool OnFindMessage (struct server *srv, const int socket,const void *msg, const int len);

    bool OnFindClose (struct server *srv,const int socket);

    bool OnBroadcastConnect(struct server *srv,const int socket,const void *data, const int len);

    bool OnBroadcastMessage (struct server *srv, const int socket, const void *msg, const int len);

    bool OnBroadcastClose (struct server *srv, const int socket);

    bool OnIniTimer (struct server *srv);

    bool OnTimeout (struct server *srv, char* id, int opcode, int time);

    bool FindBroad(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg = NULL,const int len = 0);

    bool FindBuilding(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg = NULL,const int len = 0);

    bool FindBook(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
          		const void* msg = NULL,const int len = 0);

    bool FindIntro(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
            		const void* msg = NULL,const int len = 0);
private:
    bool Init();
};


}

#endif

