#include "building_logic.h"
#include "db_comm.h"
#include "logic/logic_unit.h"
#include "lbs/lbs_connector.h"
#include "lbs/lbs_logic_unit.h"
#include "basic/scoped_ptr.h"
#include "net/operator_code.h"
#include "net/error_comm.h"
#include "pushmsg/basic_push_info.h"
#include "logic/logic_comm.h"
#include "config/config.h"
#include "common.h"

namespace buildingsvc_logic{

Buildinglogic*
Buildinglogic::instance_=NULL;

Buildinglogic::Buildinglogic(){
   if(!Init())
      assert(0);
}

Buildinglogic::~Buildinglogic(){
}

bool Buildinglogic::Init(){
	double l = base::BasicUtil::CalcGEODistance(30.304746, 120.138157,
			34.288142, 108.952601);
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
    return true;
}

Buildinglogic*
Buildinglogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Buildinglogic();

    return instance_;
}



void Buildinglogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Buildinglogic::OnBuildingConnect(struct server *srv,const int socket){

    return true;
}



bool Buildinglogic::OnBuildingMessage(struct server *srv, const int socket, const void *msg,const int len){
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
	 case BUILDING_NEAR:
		 OnRetrievalNearBuilding(srv,socket,value);
		 break;
	 case BUILDING_SERCH_TYPE:
		 OnSearchBuildingByType(srv,socket,value);
		 break;
	 case BUILDING_SUMMARY:
		 OnSummaryBuildingInfo(srv,socket,value);
		 break;
	}
    return true;
}

bool Buildinglogic::OnBuildingClose(struct server *srv,const int socket){

    return true;
}



bool Buildinglogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Buildinglogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Buildinglogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Buildinglogic::OnIniTimer(struct server *srv){

    return true;
}



bool Buildinglogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}

bool Buildinglogic::OnRetrievalNearBuilding(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
   		const void* msg,const int len){
	scoped_ptr<netcomm_recv::RetrievalNearBuilding> renear(new netcomm_recv::RetrievalNearBuilding(netbase));
	scoped_ptr<base_logic::LBSInfos> lbs_info;
	std::list<base_logic::BuildingBasicInfo> list;
	std::string token;
	bool r = false;
	int32 range = MAX_RANGE;
	int error_code = renear->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//检测是否有坐标 没有坐标通过IP换算坐标
	lbs_info.reset(base_logic::LogicUnit::GetGeocderAndAddress((netcomm_recv::HeadPacket*)renear.get()));

	//检索圈定区再圈定城市范围 检索出全部并显示所有的数据
	r = buildingsvc_logic::DBComm::RetrievalNearBuilding(lbs_info->district(),lbs_info->city(),list);
	OnCalcBatchBuilding(socket,20,range,list,lbs_info);
	//计算距离如果大于此范围则删除
	/*for(std::list<base_logic::BuildingBasicInfo>::iterator it = list.begin();
			it!=list.end();){
		//计算距离
		base_logic::BuildingBasicInfo buildinfo = (*it);
		//double l = base_lbs::CalcGEO::CalcGEODistance( 120.138157,30.304746,108.952601,34.288142);
		double l = base_lbs::CalcGEO::CalcGEODistance(buildinfo.longitude(),buildinfo.latitude(),
				lbs_info->longitude(),lbs_info->latitude());
		buildinfo.set_distance(l);
		if(l > range){
			it = list.erase(it);
		}else{
			it++;
		}
	}
	//list 重新排列组合 按照距离由远到进排列
	list.sort(base_logic::BuildingBasicInfo::cmp);
	//发送客户端
	scoped_ptr<netcomm_send::RetrievalNearBuilding> new_build(new netcomm_send::RetrievalNearBuilding());
	//list 控制个数 此处暂不控制
	while(list.size()>0){
		base_logic::BuildingBasicInfo buildinfo = list.front();
		list.pop_front();
		scoped_ptr<netcomm_send::BaseBuilding> build(new netcomm_send::BaseBuilding());
		build->set_basic(buildinfo.id(),buildinfo.name(),buildinfo.type(),buildinfo.pic());
		build->set_location(buildinfo.address(),buildinfo.latitude(),buildinfo.longitude(),
				buildinfo.city(),buildinfo.distance());
		new_build->SetBuildingInfo(build.release());
	}
	send_message(socket,(netcomm_send::HeadPacket*)new_build.get());*/
	return true;
}

bool Buildinglogic::OnSearchBuildingByType(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	bool r = false;
	scoped_ptr<netcomm_recv::SearchTypeBuilding> renear(new netcomm_recv::SearchTypeBuilding(netbase));
	scoped_ptr<base_logic::LBSInfos> lbs_info;
	std::list<base_logic::BuildingBasicInfo> list;
	int32 range = MAX_RANGE;
	int error_code = renear->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//检测是否有坐标 没有坐标通过IP换算坐标
	lbs_info.reset(base_logic::LogicUnit::GetGeocderAndAddress((netcomm_recv::HeadPacket*)renear.get()));

	//检索圈定区再圈定城市范围 检索出全部并显示所有的数据
	r = buildingsvc_logic::DBComm::SearchBuildingByType(renear->btype(),renear->from(),renear->count(),
			list);
	OnCalcBatchBuilding(socket,20,range,list,lbs_info);
	return true;

}


bool Buildinglogic::OnSummaryBuildingInfo(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	bool r = false;
	std::string phone;
	int32 collection = 0;
	std::string summary;
	scoped_ptr<netcomm_recv::GainBuildingSummary> renear(new netcomm_recv::GainBuildingSummary(netbase));
	int error_code = renear->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//获取数据
	r = buildingsvc_logic::DBComm::GainBuildingSummary(renear->uid(),renear->bid(),phone,collection,summary);
	//发送客户端
	scoped_ptr<netcomm_send::BuildingSummary> build_summary(new netcomm_send::BuildingSummary());
	scoped_ptr<netcomm_send::BuildingBaseSummary> base_summary(new netcomm_send::BuildingBaseSummary());
	scoped_ptr<netcomm_send::BuildAboutUser> user_about(new netcomm_send::BuildAboutUser());
	base_summary->set_phone(phone);
	base_summary->set_summary(summary);
	user_about->set_collect(collection);
	build_summary->set_summary(base_summary.release());
	build_summary->set_user(user_about.release());
	send_message(socket,(netcomm_send::HeadPacket*)build_summary.get());
	return true;
}

bool Buildinglogic::OnCalcBatchBuilding(const int socket,const int32 count,const int32 range,std::list<base_logic::BuildingBasicInfo>& list,
   		scoped_ptr<base_logic::LBSInfos>& lbs_info){
	//计算距离如果大于此范围则删除
	for(std::list<base_logic::BuildingBasicInfo>::iterator it = list.begin();
			it!=list.end();){
		//计算距离
		base_logic::BuildingBasicInfo buildinfo = (*it);
		//double l = base_lbs::CalcGEO::CalcGEODistance( 120.138157,30.304746,108.952601,34.288142);
		double l = base_lbs::CalcGEO::CalcGEODistance(buildinfo.longitude(),buildinfo.latitude(),
				lbs_info->longitude(),lbs_info->latitude());
		buildinfo.set_distance(l);
		if(l > range){
			it = list.erase(it);
		}else{
			it++;
		}
	}
	//list 重新排列组合 按照距离由远到进排列
	list.sort(base_logic::BuildingBasicInfo::cmp);
	//发送客户端
	scoped_ptr<netcomm_send::RetrievalNearBuilding> new_build(new netcomm_send::RetrievalNearBuilding());
	new_build->SetBuildingCount(list.size());
	//list 控制个数 此处暂不控制
	while(list.size()>0){
		base_logic::BuildingBasicInfo buildinfo = list.front();
		list.pop_front();
		scoped_ptr<netcomm_send::BaseBuilding> build(new netcomm_send::BaseBuilding());
		build->set_basic(buildinfo.id(),buildinfo.name(),buildinfo.type(),buildinfo.pic());
		build->set_location(buildinfo.address(),buildinfo.latitude(),buildinfo.longitude(),
				buildinfo.city(),buildinfo.distance());
		new_build->SetBuildingInfo(build.release());
	}
	send_message(socket,(netcomm_send::HeadPacket*)new_build.get());
	return true;
}

}

