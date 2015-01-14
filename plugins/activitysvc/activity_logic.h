#ifndef __BUDDHA__ACTIVITYSVC__ACTIVITY_LOGIC___
#define __BUDDHA__ACTIVITYSVC__ACTIVITY_LOGIC___
#include "activity_basic_info.h"
#include "logic/logic_infos.h"
#include "net/activtiy_comm_head.h"
#include "protocol/http_packet.h"
#include "common.h"

#define DEFAULT_CONFIG_PATH     "./plugins/activitysvc/activitysvc_config.xml"

namespace activitysvc_logic{

class Activitylogic{

public:
    Activitylogic();
    virtual ~Activitylogic();

private:
    static Activitylogic    *instance_;

public:
    static Activitylogic *GetInstance();
    static void FreeInstance();

public:
    bool OnActivityConnect (struct server *srv,const int socket);

    bool OnActivityMessage (struct server *srv, const int socket,const void *msg, const int len);

    bool OnActivityClose (struct server *srv,const int socket);

    bool OnBroadcastConnect(struct server *srv,const int socket,const void *data, const int len);

    bool OnBroadcastMessage (struct server *srv, const int socket, const void *msg, const int len);

    bool OnBroadcastClose (struct server *srv, const int socket);

    bool OnIniTimer (struct server *srv);

    bool OnTimeout (struct server *srv, char* id, int opcode, int time);
private:
    bool OnGetActivityList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnGetActivitySummary(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

private:

    bool Init();
};


}

#endif

