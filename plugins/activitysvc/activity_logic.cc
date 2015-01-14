#include "activity_logic.h"
#include "db_comm.h"
#include "activity_basic_info.h"
#include "logic/logic_unit.h"
#include "lbs/lbs_connector.h"
#include "lbs/lbs_logic_unit.h"
#include "basic/scoped_ptr.h"
#include "net/operator_code.h"
#include "net/error_comm.h"
#include "net/activtiy_comm_head.h"
#include "pushmsg/basic_push_info.h"
#include "logic/logic_comm.h"
#include "config/config.h"
#include "common.h"

namespace activitysvc_logic{

Activitylogic*
Activitylogic::instance_=NULL;

Activitylogic::Activitylogic(){
   if(!Init())
      assert(0);
}

Activitylogic::~Activitylogic(){
}

bool Activitylogic::Init(){
	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL){
		return false;
	}
	r = config->LoadConfig(path);
	base_lbs::LbsConnectorEngine::Create(base_lbs::IMPL_BAIDU);
	base_lbs::LbsConnector* engine = base_lbs::LbsConnectorEngine::GetLbsConnectorEngine();
	engine->Init(config->mysql_db_list_);
	activitysvc_logic::DBComm::Init(config->mysql_db_list_);
    return true;
}

Activitylogic*
Activitylogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Activitylogic();

    return instance_;
}



void Activitylogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Activitylogic::OnActivityConnect(struct server *srv,const int socket){

    return true;
}



bool Activitylogic::OnActivityMessage(struct server *srv, const int socket, const void *msg,const int len){
	const char* packet_stream = (char*)(msg);
	std::string http_str(packet_stream,len);
	std::string error_str;
	int error_code = 0;
	LOG_MSG2("%s",packet_stream);
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_HTTP,&http_str));
	netcomm_recv::NetBase*  value = (netcomm_recv::NetBase*)(serializer.get()->Deserialize(&error_code,&error_str));
	if(value==NULL){
		error_code = STRUCT_ERROR;
			//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	scoped_ptr<netcomm_recv::HeadPacket> packet(new netcomm_recv::HeadPacket(value));
	int32 type = packet->GetType();
	switch(type){
		case ACTIVITY_LIST:
			OnGetActivityList(srv,socket,value);
			break;
		case ACTIVITY_SUMMARY:
			OnGetActivitySummary(srv,socket,value);
			 break;
		}

	    return true;
    return true;
}

bool Activitylogic::OnActivityClose(struct server *srv,const int socket){

    return true;
}



bool Activitylogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Activitylogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Activitylogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Activitylogic::OnIniTimer(struct server *srv){

    return true;
}



bool Activitylogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}


bool Activitylogic::OnGetActivitySummary(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
		const void* msg,const int len){
	bool r = false;
	scoped_ptr<netcomm_recv::ActivtiySummary> activty_summary(new netcomm_recv::ActivtiySummary(netbase));
	int error_code = activty_summary->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	activitysvc_logic::ActivityBasicInfo activityinfo;
	activitysvc_logic::DBComm::OnGainActivitySummary(activty_summary->aid(),activityinfo);
	scoped_ptr<netcomm_send::ActivtiySummary> sactivtiy(new netcomm_send::ActivtiySummary());
	sactivtiy->set_activtiy(activityinfo.phone(),activityinfo.details());
	sactivtiy->set_location(activityinfo.address(),activityinfo.latitude(),activityinfo.longitude());
	send_message(socket,(netcomm_send::HeadPacket*)sactivtiy.get());
	return true;
}

bool Activitylogic::OnGetActivityList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
		const void* msg,const int len){
	bool r = false;
	int32 range = 10000;
	scoped_ptr<netcomm_recv::Activties> activty(new netcomm_recv::Activties(netbase));
	int error_code = activty->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	scoped_ptr<base_logic::LBSInfos> lbs_info;
	std::list<activitysvc_logic::ActivityBasicInfo> list;
	//根据地理位置获取

	//检测是否有坐标 没有坐标通过IP换算坐标
	lbs_info.reset(base_logic::LogicUnit::GetGeocderAndAddress((netcomm_recv::HeadPacket*)activty.get()));

	//根据城市 范围 获取活动
	activitysvc_logic::DBComm::OnGainActivityList(lbs_info->district(),lbs_info->city(),list);

	//计算距离如果大于此范围则删除
	for(std::list<activitysvc_logic::ActivityBasicInfo>::iterator it = list.begin();
				it!=list.end();){
		//计算距离
		activitysvc_logic::ActivityBasicInfo activitynfo = (*it);
		double l = base_lbs::CalcGEO::CalcGEODistance(activitynfo.longitude(),activitynfo.latitude(),
					lbs_info->longitude(),lbs_info->latitude());
		activitynfo.set_distance(l);
		if(/*l > range*/false){
			it = list.erase(it);
		}else{
			it++;
		}
	}
	//list 重新排列组合 按照距离由远到进排列
	list.sort(activitysvc_logic::ActivityBasicInfo::cmp);
	//发送客户端
	scoped_ptr<netcomm_send::Activties> sactivitiy(new netcomm_send::Activties());
	int32 recommcount =0;
	//list 控制个数 此处暂不控制
	while(list.size()>0){
		activitysvc_logic::ActivityBasicInfo activitynfo = list.front();
		sactivitiy->SetActivtiesInfo(activitynfo.id(),activitynfo.name(),activitynfo.pic(),0);
		list.pop_front();
		//
		if(++recommcount>=2)
			break;
	}

	while(list.size()>0){
		activitysvc_logic::ActivityBasicInfo activitynfo = list.front();
		sactivitiy->SetActivtiesInfo(activitynfo.id(),activitynfo.name(),activitynfo.pic(),1);
		list.pop_front();
	}

	send_message(socket,(netcomm_send::HeadPacket*)sactivitiy.get());
	return true;
}

}

