#include "intro_init.h"
#include "intro_logic.h"
#include "common.h"
#include "plugins.h"


struct introplugin{
    char* id;
    char* name;
    char* version;
    char* provider;
};


static void *OnIntroStart(){

    signal(SIGPIPE,SIG_IGN);
    struct introplugin* intro=(struct introplugin*)calloc(1,sizeof(struct introplugin));

    intro->id="intro";

    intro->name="intro";

    intro->version="1.0.0";

    intro->provider="kerry";

    if(!introsvc_logic::Intrologic::GetInstance())
        assert(0);

    return intro ;

}

static handler_t OnIntroShutdown(struct server* srv,void* pd){

    introsvc_logic::Intrologic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnIntroConnect(struct server *srv, int fd, void *data, int len){

    introsvc_logic::Intrologic::GetInstance()->OnIntroConnect(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnIntroMessage(struct server *srv, int fd, void *data, int len){

    introsvc_logic::Intrologic::GetInstance()->OnIntroMessage(srv,fd,data,len);

    return HANDLER_GO_ON;
}

static handler_t OnIntroClose(struct server *srv, int fd){

    introsvc_logic::Intrologic::GetInstance()->OnIntroClose(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len){


    return HANDLER_GO_ON;

}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data, int len){


    introsvc_logic::Intrologic::GetInstance()->OnBroadcastConnect(srv,fd,data,len);

    return HANDLER_GO_ON;

}

static handler_t OnBroadcastClose(struct server* srv, int fd){ 


    introsvc_logic::Intrologic::GetInstance()->OnBroadcastClose(srv,fd);

    return HANDLER_GO_ON;


}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data, int len){


    introsvc_logic::Intrologic::GetInstance()->OnBroadcastMessage(srv,fd,data,len);

    return HANDLER_GO_ON;


}

static handler_t OnIniTimer(struct server* srv){ 


    introsvc_logic::Intrologic::GetInstance()->OnIniTimer(srv);

    return HANDLER_GO_ON;


}

static handler_t OnTimeOut(struct server* srv,char* id, int opcode, int time){ 


    introsvc_logic::Intrologic::GetInstance()->OnTimeout(srv,id,opcode,time);

    return HANDLER_GO_ON;


}






int intro_plugin_init(struct plugin *pl){


    pl->init=OnIntroStart;

    pl->clean_up = OnIntroShutdown;

    pl->connection = OnIntroConnect;

    pl->connection_close = OnIntroClose;

    pl->connection_close_srv = OnBroadcastClose;

    pl->connection_srv = OnBroadcastConnect;

    pl->handler_init_time = OnIniTimer;

    pl->handler_read = OnIntroMessage;

    pl->handler_read_srv = OnBroadcastMessage;

    pl->handler_read_other = OnUnknow;

    pl->time_msg = OnTimeOut;

    pl->data = NULL;

    return 0;

}

