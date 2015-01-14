#include "usr_init.h"
#include "usr_logic.h"
#include "common.h"
#include "plugins.h"


struct usrplugin{
    char* id;
    char* name;
    char* version;
    char* provider;
};


static void *OnUsrStart(){

    signal(SIGPIPE,SIG_IGN);
    struct usrplugin* usr=(struct usrplugin*)calloc(1,sizeof(struct usrplugin));

    usr->id="usr";

    usr->name="usr";

    usr->version="1.0";

    usr->provider="kerry";

    if(!usrsvc_logic::Usrlogic::GetInstance())
        assert(0);

    return usr ;

}

static handler_t OnUsrShutdown(struct server* srv,void* pd){

    usrsvc_logic::Usrlogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnUsrConnect(struct server *srv, int fd, void *data, int len){

    usrsvc_logic::Usrlogic::GetInstance()->OnUsrConnect(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnUsrMessage(struct server *srv, int fd, void *data, int len){

    usrsvc_logic::Usrlogic::GetInstance()->OnUsrMessage(srv,fd,data,len);

    return HANDLER_GO_ON;
}

static handler_t OnUsrClose(struct server *srv, int fd){

    usrsvc_logic::Usrlogic::GetInstance()->OnUsrClose(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len){


    return HANDLER_GO_ON;

}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data, int len){


    usrsvc_logic::Usrlogic::GetInstance()->OnBroadcastConnect(srv,fd,data,len);

    return HANDLER_GO_ON;

}

static handler_t OnBroadcastClose(struct server* srv, int fd){ 


    usrsvc_logic::Usrlogic::GetInstance()->OnBroadcastClose(srv,fd);

    return HANDLER_GO_ON;


}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data, int len){


    usrsvc_logic::Usrlogic::GetInstance()->OnBroadcastMessage(srv,fd,data,len);

    return HANDLER_GO_ON;


}

static handler_t OnIniTimer(struct server* srv){ 


    usrsvc_logic::Usrlogic::GetInstance()->OnIniTimer(srv);

    return HANDLER_GO_ON;


}

static handler_t OnTimeOut(struct server* srv,char* id, int opcode, int time){ 


    usrsvc_logic::Usrlogic::GetInstance()->OnTimeout(srv,id,opcode,time);

    return HANDLER_GO_ON;


}






int usr_plugin_init(struct plugin *pl){


    pl->init=OnUsrStart;

    pl->clean_up = OnUsrShutdown;

    pl->connection = OnUsrConnect;

    pl->connection_close = OnUsrClose;

    pl->connection_close_srv = OnBroadcastClose;

    pl->connection_srv = OnBroadcastConnect;

    pl->handler_init_time = OnIniTimer;

    pl->handler_read = OnUsrMessage;

    pl->handler_read_srv = OnBroadcastMessage;

    pl->handler_read_other = OnUnknow;

    pl->time_msg = OnTimeOut;

    pl->data = NULL;

    return 0;

}

