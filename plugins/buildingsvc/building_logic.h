#ifndef __BUDDHA__BUILDINGSVC__BUILDING_LOGIC___
#define __BUDDHA__BUILDINGSVC__BUILDING_LOGIC___
#include "building_basic_info.h"
#include "logic/logic_infos.h"
#include "net/building_comm_head.h"
#include "protocol/http_packet.h"
#include "common.h"

#define MAX_RANGE 10000
#define DEFAULT_CONFIG_PATH     "./plugins/buildingsvc/buildingsvc_config.xml"
namespace buildingsvc_logic{

class Buildinglogic{

public:
    Buildinglogic();
    virtual ~Buildinglogic();

private:
    static Buildinglogic    *instance_;

public:
    static Buildinglogic *GetInstance();
    static void FreeInstance();

public:
    bool OnBuildingConnect (struct server *srv,const int socket);

    bool OnBuildingMessage (struct server *srv, const int socket,const void *msg, const int len);

    bool OnBuildingClose (struct server *srv,const int socket);

    bool OnBroadcastConnect(struct server *srv,const int socket,const void *data, const int len);

    bool OnBroadcastMessage (struct server *srv, const int socket, const void *msg, const int len);

    bool OnBroadcastClose (struct server *srv, const int socket);

    bool OnIniTimer (struct server *srv);

    bool OnTimeout (struct server *srv, char* id, int opcode, int time);

private:
    bool OnRetrievalNearBuilding(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnSummaryBuildingInfo(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnSearchBuildingByType(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);
private:

    bool OnCalcBatchBuilding(const int socket,const int32 count,const int32 range,
    		std::list<base_logic::BuildingBasicInfo>& list,
    		scoped_ptr<base_logic::LBSInfos>& lbs_info);

    bool Init();
};


}

#endif

