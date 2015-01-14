#ifndef __BUDDHA__USRSVC__USR_LOGIC___
#define __BUDDHA__USRSVC__USR_LOGIC___
#include "net/user_comm_head.h"
#include "protocol/http_packet.h"
#include "common.h"

#define DEFAULT_CONFIG_PATH     "./plugins/usrsvc/usrsvc_config.xml"

namespace usrsvc_logic{

class Usrlogic{

public:
    Usrlogic();
    virtual ~Usrlogic();

private:
    static Usrlogic    *instance_;

public:
    static Usrlogic *GetInstance();
    static void FreeInstance();

public:
    bool OnUsrConnect (struct server *srv,const int socket);

    bool OnUsrMessage (struct server *srv, const int socket,const void *msg, const int len);

    bool OnUsrClose (struct server *srv,const int socket);

    bool OnBroadcastConnect(struct server *srv,const int socket,const void *data, const int len);

    bool OnBroadcastMessage (struct server *srv, const int socket, const void *msg, const int len);

    bool OnBroadcastClose (struct server *srv, const int socket);

    bool OnIniTimer (struct server *srv);

    bool OnTimeout (struct server *srv, char* id, int opcode, int time);
private:
    bool OnQuickLogin(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnThirdLogin(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnBDBindPush(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    //bool OnFindRecomm(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       	//	const void* msg = NULL,const int len = 0);

private:
    bool OnQuickLogin(struct server *srv,const int socket,const void* msg,const int len);

    bool OnThirdLogin(struct server *srv,const int socket,const void* msg,const int len);

    bool OnBDBindPush(struct server *srv,const int socket,const void* msg,const int len);

    bool Init();
private:
    void Test();
};


}

#endif

