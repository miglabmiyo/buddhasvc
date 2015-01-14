#include "usr_logic.h"
#include "user_basic_info.h"
#include "db_comm.h"
#include "logic/logic_unit.h"
#include "logic/logic_infos.h"
#include "lbs/lbs_connector.h"
#include "lbs/lbs_logic_unit.h"
#include "basic/scoped_ptr.h"
#include "net/operator_code.h"
#include "net/error_comm.h"
#include "pushmsg/basic_push_info.h"
#include "logic/logic_comm.h"
#include "config/config.h"

#include <string>

namespace usrsvc_logic{

Usrlogic*
Usrlogic::instance_=NULL;

Usrlogic::Usrlogic(){
   if(!Init())
      assert(0);
}

Usrlogic::~Usrlogic(){
}

bool Usrlogic::Init(){
	Test();
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

	/*std::string latitude = "120.21937542";
	std::string longitude = "30.25924446";
	std::string city;
	std::string district;
	std::string province;
	std::string street;
	std::string host = "218.74.28.80";
	r = engine->GeocoderForAddress(longitude
			,latitude
						,city,district,province,street);*/
    return true;
}

Usrlogic*
Usrlogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Usrlogic();

    return instance_;
}


void Usrlogic::Test(){
	/*std::string error_str;
	int error_code;
	std::string http_str = "imei=A000&addr=115.192.191.236&type=buddha_quicklogin\n";
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_HTTP,&http_str));
	base_logic::Value* value = serializer.get()->Deserialize(&error_code,&error_str);
	int i = 0;
	i = i+1;*/
	double l = base_lbs::CalcGEO::CalcGEODistance( 120.138157,30.304746,
			 108.952601,34.288142);

	//netcomm_recv::NetBase*  value = (netcomm_recv::NetBase*)(serializer.get()->Deserialize(&error_code,&error_str));
	//scoped_ptr<netcomm_recv::HeadPacket> packet(new netcomm_recv::HeadPacket(value));
}

void Usrlogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Usrlogic::OnUsrConnect(struct server *srv,const int socket){

    return true;
}



bool Usrlogic::OnUsrMessage(struct server *srv, const int socket, const void *msg,const int len){

	const char* packet_stream = (char*)(msg);
	std::string http_str(packet_stream,len);
	std::string error_str;
	int error_code = 0;
	/*packet::HttpPacket packet(packet_stream,len-1);
	std::string type;
	packet.GetPacketType(type);
	if (type=="buddha_quicklogin")
		OnQuickLogin(srv,socket,msg,len);
	else if(type=="buddha_thirdlogin")
		OnThirdLogin(srv,socket,msg,len);
	else if(type=="buddha_bdbindpush")
		OnBDBindPush(srv,socket,msg,len);*/
	//base_logic::HttpValueSerializer serializer
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
		case QUICK_LOGIN:
			OnQuickLogin(srv,socket,value);
			break;
		case THIRID_LOGIN:
			OnThirdLogin(srv,socket,value);
			break;
		//case FIND_COMM:
			//OnFindRecomm(srv,socket,value);
			break;
	}

    return true;
}

bool Usrlogic::OnUsrClose(struct server *srv,const int socket){

    return true;
}



bool Usrlogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Usrlogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Usrlogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Usrlogic::OnIniTimer(struct server *srv){

    return true;
}



bool Usrlogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}

bool Usrlogic::OnQuickLogin(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::QucikLogin> login(new netcomm_recv::QucikLogin(netbase));
	scoped_ptr<base_logic::LBSInfos> lbs_info;
	usrsvc_logic::UserInfo userinfo;
	std::string token;
	bool r = false;
	int error_code = login->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//获取地址
	lbs_info.reset(base_logic::LogicUnit::GetGeocderAndAddress((netcomm_recv::HeadPacket*)login.get()));
	if(r)
		login.get()->Reset();
	userinfo.set_imei(login->imei());
	userinfo.set_machine(login->machine());
	//存储用户信息
	usrsvc_logic::DBComm::OnQuickRegister(userinfo,lbs_info.get());
	base_logic::LogicUnit::CreateToken(userinfo.uid(),token);
	scoped_ptr<netcomm_send::Login> qlogin(new netcomm_send::Login());


	qlogin->set_userinfo_address(userinfo.city());
	qlogin->set_userinfo_token(token);
	qlogin->set_userinfo_uid(userinfo.uid());
	qlogin->set_userinfo_nickname(userinfo.nickname());
	qlogin->set_userinfo_source(userinfo.type());
	qlogin->set_useromfo_head(userinfo.head());
	send_message(socket,(netcomm_send::HeadPacket*)qlogin.get());
	return true;
}

bool Usrlogic::OnThirdLogin(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::ThirdLogin> login(new netcomm_recv::ThirdLogin(netbase));
	scoped_ptr<base_logic::LBSInfos> lbs_info;
	usrsvc_logic::UserInfo userinfo;
	std::string token;
	bool r = false;
	int error_code = login->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	//获取地址
	lbs_info.reset(base_logic::LogicUnit::GetGeocderAndAddress((netcomm_recv::HeadPacket*)login.get()));
	if(r)
		login.get()->Reset();
	userinfo.set_imei(login->imei());
	userinfo.set_machine(login->machine());
	userinfo.set_type(login->source());
	userinfo.set_nickname(login->nickanme());
	userinfo.set_sex(login->sex());
	userinfo.set_birthday(login->birthday());
	userinfo.set_head(login->head());
	userinfo.set_session(login->session());
	//存储用户信息
	usrsvc_logic::DBComm::OnThirdRegister(userinfo,lbs_info.get());
	base_logic::LogicUnit::CreateToken(userinfo.uid(),token);
	scoped_ptr<netcomm_send::Login> qlogin(new netcomm_send::Login());

	qlogin->set_userinfo_address(userinfo.city());
	qlogin->set_userinfo_token(token);
	qlogin->set_userinfo_uid(userinfo.uid());
	qlogin->set_userinfo_nickname(userinfo.nickname());
	qlogin->set_userinfo_source(userinfo.type());
	qlogin->set_useromfo_head(userinfo.head());
	send_message(socket,(netcomm_send::HeadPacket*)qlogin.get());
	return true;
}

bool Usrlogic::OnBDBindPush(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
   		const void* msg,const int len){
	scoped_ptr<netcomm_recv::BDBindPush> bind(new netcomm_recv::BDBindPush(netbase));
	/*
	 * const int64 platform,const int64 uid,const int64 channel,const int64 bd_userid,
			const std::string& pkg_name,const std::string& tag,const int64 appid,
			const int64 requestid,const int32 machine)
	 * */
	base_push::BaiduBindPushInfo bindinfo(bind->platform(),bind->uid(),bind->channel(),
			bind->bduserid(),bind->pkg_name(),bind->tag(),bind->appid(),bind->request(),
			bind->machine());


}

/*bool Usrlogic::OnFindRecomm(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int){
	scoped_ptr<netcomm_recv::RecomFind> recom(new netcomm_recv::RecomFind(netbase));
	int error_code = recom->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	std::string test_content = "{\"msg\": \"\",\"flag\": 1,\"result\":{\"msg\": {\"new\": 1, \"total\": 100},\"advert\": [{\"title\": \"推荐1\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\", \"weight \": 1},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2}],\"news\": {\"title\": \"安徽省佛教讲义圆满结束\",\"summary\": \"阅后即焚服务一度跻身APP store 前50名免费应用排行榜.\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\"},\"books\": [{\"name\": \"法华经\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"name\": \"金刚经\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 1},{\"name\": \"圆觉经\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 3}],\"activities\": [{\"title\": \"僧侣出游\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"恭迎舍利子\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 1},{\"title\": \"在外打坐\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 3}]},\"status\": 1}";
	base_logic::LogicComm::SendFull(socket,test_content.c_str(),test_content.length());
	return true;
}*/
bool Usrlogic::OnQuickLogin(struct server *srv,const int socket,const void* msg,const int len){
	std::string test_content = "{\"msg\": \"\",\"flag\": 1,\"result\": {\"userinfo\": {\"uid\": 100008,\"token\": \"2cbae21fd1d8018408582aaede703448\",\"address\": \"杭州\"},\"msg\": {\"new\": 1, \"total\": 100},\"advert\": [{\"title\": \"推荐1\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\", \"weight \": 1},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"推荐2\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2}],\"news\": {\"title\": \"安徽省佛教讲义圆满结束\",\"summary\": \"阅后即焚服务一度跻身APP store 前50名免费应用排行榜.\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\"},\"books\": [{\"name\": \"法华经\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"name\": \"金刚经\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 1},{\"name\": \"圆觉经\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 3}],\"activities\": [{\"title\": \"僧侣出游\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 2},{\"title\": \"恭迎舍利子\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 1},{\"title\": \"在外打坐\",\"pic\": \"http://face.miu.miyomate.com/book_0.jpg\",\"weight \": 3}]},\"status\": 1}";
	//SendFull(socket,test_content.c_str(),test_content.length());
	return true;
}

bool Usrlogic::OnThirdLogin(struct server *srv,const int socket,const void* msg,const int len){
	return true;
}

bool Usrlogic::OnBDBindPush(struct server *srv,const int socket,const void* msg,const int len){
	std::string test_content = "{\"msg\": \"\",\"result\": {},\"status\": 1}";
	//SendFull(socket,test_content.c_str(),test_content.length());
	return true;
}


}

