#include "building_init.h"
#include "building_logic.h"
#include "common.h"
#include "plugins.h"


struct buildingplugin{
    char* id;
    char* name;
    char* version;
    char* provider;
};


static void *OnBuildingStart(){

    signal(SIGPIPE,SIG_IGN);
    struct buildingplugin* building=(struct buildingplugin*)calloc(1,sizeof(struct buildingplugin));

    building->id="";

    building->name="";

    building->version="";

    building->provider="";

    if(!buildingsvc_logic::Buildinglogic::GetInstance())
        assert(0);

    return building ;

}

static handler_t OnBuildingShutdown(struct server* srv,void* pd){

    buildingsvc_logic::Buildinglogic::FreeInstance();

    return HANDLER_GO_ON;
}

static handler_t OnBuildingConnect(struct server *srv, int fd, void *data, int len){

    buildingsvc_logic::Buildinglogic::GetInstance()->OnBuildingConnect(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnBuildingMessage(struct server *srv, int fd, void *data, int len){

    buildingsvc_logic::Buildinglogic::GetInstance()->OnBuildingMessage(srv,fd,data,len);

    return HANDLER_GO_ON;
}

static handler_t OnBuildingClose(struct server *srv, int fd){

    buildingsvc_logic::Buildinglogic::GetInstance()->OnBuildingClose(srv,fd);

    return HANDLER_GO_ON;
}

static handler_t OnUnknow(struct server *srv, int fd, void *data, int len){


    return HANDLER_GO_ON;

}

static handler_t OnBroadcastConnect(struct server* srv, int fd, void *data, int len){


    buildingsvc_logic::Buildinglogic::GetInstance()->OnBroadcastConnect(srv,fd,data,len);

    return HANDLER_GO_ON;

}

static handler_t OnBroadcastClose(struct server* srv, int fd){ 


    buildingsvc_logic::Buildinglogic::GetInstance()->OnBroadcastClose(srv,fd);

    return HANDLER_GO_ON;


}

static handler_t OnBroadcastMessage(struct server* srv, int fd, void *data, int len){


    buildingsvc_logic::Buildinglogic::GetInstance()->OnBroadcastMessage(srv,fd,data,len);

    return HANDLER_GO_ON;


}

static handler_t OnIniTimer(struct server* srv){ 


    buildingsvc_logic::Buildinglogic::GetInstance()->OnIniTimer(srv);

    return HANDLER_GO_ON;


}

static handler_t OnTimeOut(struct server* srv,char* id, int opcode, int time){ 


    buildingsvc_logic::Buildinglogic::GetInstance()->OnTimeout(srv,id,opcode,time);

    return HANDLER_GO_ON;


}






int building_plugin_init(struct plugin *pl){


    pl->init=OnBuildingStart;

    pl->clean_up = OnBuildingShutdown;

    pl->connection = OnBuildingConnect;

    pl->connection_close = OnBuildingClose;

    pl->connection_close_srv = OnBroadcastClose;

    pl->connection_srv = OnBroadcastConnect;

    pl->handler_init_time = OnIniTimer;

    pl->handler_read = OnBuildingMessage;

    pl->handler_read_srv = OnBroadcastMessage;

    pl->handler_read_other = OnUnknow;

    pl->time_msg = OnTimeOut;

    pl->data = NULL;

    return 0;

}

