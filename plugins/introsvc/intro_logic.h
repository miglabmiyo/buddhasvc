#ifndef __BUDDHA__INTROSVC__INTRO_LOGIC___
#define __BUDDHA__INTROSVC__INTRO_LOGIC___
#include "intro_basic_info.h"
#include "logic/logic_infos.h"
#include "net/intro_comm_head.h"
#include "protocol/http_packet.h"
#include "common.h"

#define DEFAULT_CONFIG_PATH     "./plugins/introsvc/introsvc_config.xml"

namespace introsvc_logic{

class Intrologic{

public:
    Intrologic();
    virtual ~Intrologic();

private:
    static Intrologic    *instance_;

public:
    static Intrologic *GetInstance();
    static void FreeInstance();

public:
    bool OnIntroConnect (struct server *srv,const int socket);

    bool OnIntroMessage (struct server *srv, const int socket,const void *msg, const int len);

    bool OnIntroClose (struct server *srv,const int socket);

    bool OnBroadcastConnect(struct server *srv,const int socket,const void *data, const int len);

    bool OnBroadcastMessage (struct server *srv, const int socket, const void *msg, const int len);

    bool OnBroadcastClose (struct server *srv, const int socket);

    bool OnIniTimer (struct server *srv);

    bool OnTimeout (struct server *srv, char* id, int opcode, int time);

private:
    bool OnSearchIntroByType(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnIntroHisAndThr(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnIntroArt(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);
private:

    bool Init();
};


}

#endif

