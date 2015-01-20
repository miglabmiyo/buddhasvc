#include "intro_logic.h"
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

namespace introsvc_logic{

Intrologic*
Intrologic::instance_=NULL;

Intrologic::Intrologic(){
   if(!Init())
      assert(0);
}

Intrologic::~Intrologic(){
	introsvc_logic::DBComm::Dest();
}

bool Intrologic::Init(){
	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL){
		return false;
	}
	r = config->LoadConfig(path);

	introsvc_logic::DBComm::Init(config->mysql_db_list_);
    return true;
}

Intrologic*
Intrologic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Intrologic();

    return instance_;
}



void Intrologic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Intrologic::OnIntroConnect(struct server *srv,const int socket){

    return true;
}



bool Intrologic::OnIntroMessage(struct server *srv, const int socket, const void *msg,const int len){
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
	 case INTRO_SEARCH_TYPE:
		 OnSearchIntroByType(srv,socket,value);
		 break;
	 case INTRO_HIS_THR_SUMMARY:
		 OnIntroHisAndThr(srv,socket,value);
		 break;
	 case INTOR_ART_SUMMARY:
		 OnIntroArt(srv,socket,value);
		 break;
	}
    return true;
}

bool Intrologic::OnIntroClose(struct server *srv,const int socket){

    return true;
}



bool Intrologic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Intrologic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Intrologic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Intrologic::OnIniTimer(struct server *srv){

    return true;
}



bool Intrologic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}

bool Intrologic::OnSearchIntroByType(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::IntroSearchType> src_type(new netcomm_recv::IntroSearchType(netbase));
	int error_code = src_type->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	std::list<introsvc_logic::IntroBasicInfo> list;
	//读取数据库
	introsvc_logic::DBComm::SearchIntroInfoByType(src_type->btype(),list);
	scoped_ptr<netcomm_send::IntroSearchType> ssrc_typr(new netcomm_send::IntroSearchType());
	while(list.size()>0){
		introsvc_logic::IntroBasicInfo basic_info = list.front();
		list.pop_front();
		scoped_ptr<netcomm_send::IntroInfo> info(new netcomm_send::IntroInfo());
		info->set_basic_intro(basic_info.id(),basic_info.type(),basic_info.name(),basic_info.pic(),basic_info.summary());
		ssrc_typr->SetIntroInfo(info.release());
	}

	//发送
	send_message(socket,(netcomm_send::HeadPacket*)ssrc_typr.get());

	return true;
}

bool Intrologic::Intrologic::OnIntroHisAndThr(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
   		const void* msg,const int len){
	scoped_ptr<netcomm_recv::IntroSummary> summary(new netcomm_recv::IntroSummary(netbase));
	introsvc_logic::IntroBasicInfo introinfo;
	std::list<introsvc_logic::IntroChapterInfo> list;
	int error_code = summary->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	bool r = false;
	introinfo.set_id(summary->introid());
	r = introsvc_logic::DBComm::GetIntroSummary(summary->introid(),introinfo);
	if(!r){
		send_error(error_code,socket);
		return false;
	}
	r = introsvc_logic::DBComm::GetIntroChapter(summary->introid(),list);

	//
	scoped_ptr<netcomm_send::IntroHisaThr> histhr_summary(new netcomm_send::IntroHisaThr());
	histhr_summary->SetIntroSummary(list.size(),introinfo.bigpic(),introinfo.content());
	while(list.size()>0){
		introsvc_logic::IntroChapterInfo chapter = list.front();
		list.pop_front();
		histhr_summary->SetChapterList(chapter.name(),chapter.chapter_url(),chapter.id());
	}

	//发送
	send_message(socket,(netcomm_send::HeadPacket*)histhr_summary.get());
	return true;
}

bool Intrologic::OnIntroArt(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len ){
	scoped_ptr<netcomm_recv::IntroSummary> summary(new netcomm_recv::IntroSummary(netbase));
	introsvc_logic::IntroBasicInfo introinfo;
	std::list<introsvc_logic::IntroChapterInfo> list;
	int error_code = summary->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	bool r = false;
	introinfo.set_id(summary->introid());
	r = introsvc_logic::DBComm::GetIntroSummary(summary->introid(),introinfo);
	if(!r){
		send_error(error_code,socket);
		return false;
	}

	//
	scoped_ptr<netcomm_send::IntroArt> intro_art(new netcomm_send::IntroArt());
	intro_art->SetSummary(introinfo.bigpic(),introinfo.content());
	//发送
	send_message(socket,(netcomm_send::HeadPacket*)intro_art.get());
	return true;
}

}

