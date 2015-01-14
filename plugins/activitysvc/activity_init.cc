#include "activity_init.h"
#include "activity_logic.h"
#include "common.h"
#include "plugins.h"


struct activityplugin{
    char* id;
    char* name;
    char* version;
    char* provider;
};


static void *OnActivityStart(){

    signal(SIGPIPE,SIG_IGN);
    struct activityplugin* activity=(struct activityplugin*)calloc(1,sizeof(struct activityplugin));

    activity->id="activity";

    activity->name="activity";

    activity->version="1.0.0";

    activity->provider="kerry";

    if(!activitysvc_logic::Activitylogic::GetInstance())
        assert(0);

    return activity ;

}

static handler_t OnActivityShutdown(struct server* srv,void* pd){

    activitysvc_logic::Activitylogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnActivityConnect(struct server *srv, int fd, void *data, int len){

    activitysvc_logic::Activitylogic::GetInstance()->OnActivityConnect(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnActivityMessage(struct server *srv, int fd, void *data, int len){

    activitysvc_logic::Activitylogic::GetInstance()->OnActivityMessage(srv,fd,data,len);

    return HANDLER_GO_ON;
}

static handler_t OnActivityClose(struct server *srv, int fd){

    activitysvc_logic::Activitylogic::GetInstance()->OnActivityClose(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len){


    return HANDLER_GO_ON;

}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data, int len){


    activitysvc_logic::Activitylogic::GetInstance()->OnBroadcastConnect(srv,fd,data,len);

    return HANDLER_GO_ON;

}

static handler_t OnBroadcastClose(struct server* srv, int fd){ 


    activitysvc_logic::Activitylogic::GetInstance()->OnBroadcastClose(srv,fd);

    return HANDLER_GO_ON;


}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data, int len){


    activitysvc_logic::Activitylogic::GetInstance()->OnBroadcastMessage(srv,fd,data,len);

    return HANDLER_GO_ON;


}

static handler_t OnIniTimer(struct server* srv){ 


    activitysvc_logic::Activitylogic::GetInstance()->OnIniTimer(srv);

    return HANDLER_GO_ON;


}

static handler_t OnTimeOut(struct server* srv,char* id, int opcode, int time){ 


    activitysvc_logic::Activitylogic::GetInstance()->OnTimeout(srv,id,opcode,time);

    return HANDLER_GO_ON;


}






int activity_plugin_init(struct plugin *pl){


    pl->init=OnActivityStart;

    pl->clean_up = OnActivityShutdown;

    pl->connection = OnActivityConnect;

    pl->connection_close = OnActivityClose;

    pl->connection_close_srv = OnBroadcastClose;

    pl->connection_srv = OnBroadcastConnect;

    pl->handler_init_time = OnIniTimer;

    pl->handler_read = OnActivityMessage;

    pl->handler_read_srv = OnBroadcastMessage;

    pl->handler_read_other = OnUnknow;

    pl->time_msg = OnTimeOut;

    pl->data = NULL;

    return 0;

}

