#include "find_logic.h"
#include "logic/logic_unit.h"
#include "logic/logic_infos.h"
#include "lbs/lbs_connector.h"
#include "lbs/lbs_logic_unit.h"
#include "basic/scoped_ptr.h"
#include "net/operator_code.h"
#include "net/error_comm.h"
#include "logic/logic_comm.h"
#include "config/config.h"
#include "common.h"

namespace findsvc_logic{

Findlogic*
Findlogic::instance_=NULL;

Findlogic::Findlogic(){
   if(!Init())
      assert(0);
}

Findlogic::~Findlogic(){
}

bool Findlogic::Init(){
    return true;
}

Findlogic*
Findlogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Findlogic();

    return instance_;
}



void Findlogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Findlogic::OnFindConnect(struct server *srv,const int socket){

    return true;
}



bool Findlogic::OnFindMessage(struct server *srv, const int socket, const void *msg,const int len){
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
		case FIND_BROAD:
			FindBroad(srv,socket,value);
			break;
		case FIND_BUILDING:
			FindBuilding(srv,socket,value);
			break;
		case FIND_BOOK:
			FindBook(srv,socket,value);
			break;
		case FIND_INTRO:
			FindIntro(srv,socket,value);
	}
    return true;
}

bool Findlogic::OnFindClose(struct server *srv,const int socket){

    return true;
}



bool Findlogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Findlogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Findlogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Findlogic::OnIniTimer(struct server *srv){

    return true;
}



bool Findlogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}

bool Findlogic::FindBroad(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
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
}

bool Findlogic::FindBuilding(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
      		const void* msg,const int len){
	scoped_ptr<netcomm_recv::RecomFind> recom(new netcomm_recv::RecomFind(netbase));
	int error_code = recom->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	std::string test_content = "{\"result\": {\"nearbuild\": [{\"basic\": {\"id\": 11,\"name\": \"四季青儿童服装市场\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"type\": 5}, \"location\": {\"address\": \"采荷支路8号(采荷大厦对面)\",\"city\": \"杭州市\",\"distance\": 1605.9234795394068,\"latitude\": 30.257029,\"longitude\": 120.202853}},{\"basic\": {\"id\": 3,\"name\": \"清真寺\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"type\": 1},\"location\": {\"address\": \"运河东路附近\",\"city\": \"杭州市\",\"distance\": 2461.4142861640266,\"latitude\": 30.280609,\"longitude\": 120.226083}},{\"basic\": {\"id\": 8,\"name\": \"杭州寿康永素食餐饮有限公司\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"type\": 3 },\"location\": {\"address\": \"上城区延安南路38号(近惠民路)\",\"city\": \"杭州市\",\"distance\": 4785.142388967215,\"latitude\": 30.247475,\"longitude\": 120.171456 } }, {\"basic\": {\"id\": 4,\"name\": \"博库书城(天目山店)\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"type\": 2}, \"location\": {\"address\": \"天目山路38号\",\"city\": \"杭州市\",\"distance\": 6219.8927696060855,\"latitude\": 30.278738,\"longitude\": 120.158669}}]},\"status\": 1}";
	base_logic::LogicComm::SendFull(socket,test_content.c_str(),test_content.length());
	return true;
}

bool Findlogic::FindBook(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
          		const void* msg,const int len){
	scoped_ptr<netcomm_recv::RecomFind> recom(new netcomm_recv::RecomFind(netbase));
	int error_code = recom->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	std::string test_content = "{\"result\": { \"hot\": [{\"id\": 11,\"name\": \"金刚经\", \"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5},{\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5},{\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5}],\"scriptures\": [{ \"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5},{\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5},{\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5 }],\"ritual\": [{\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5}, {\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5},{\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5}],\"theory\": [{\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5}, {\"id\": 11,\"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5},{\"id\": 11, \"name\": \"金刚经\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"《金刚经》是大乘佛教的重要经典。全称《能断金刚般若波罗蜜经》\",\"type\": 5} ] }, \"status\": 1}";
	base_logic::LogicComm::SendFull(socket,test_content.c_str(),test_content.length());
	return true;

}

bool Findlogic::FindIntro(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
          		const void* msg,const int len){
	scoped_ptr<netcomm_recv::RecomFind> recom(new netcomm_recv::RecomFind(netbase));
	int error_code = recom->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	std::string test_content = "{\"result\": {\"his\": [{\"id\": 11,\"name\": \"印度佛教史\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"印度佛教的发展史，可以分为八个阶段\",\"type\": 1},{\"id\": 11,\"name\": \"印度佛教史\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"印度佛教的发展史，可以分为八个阶段\",\"type\": 1}],\"thought\": [{\"id\": 11,\"name\": \"印度佛教史\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"印度佛教的发展史，可以分为八个阶段\",\"type\": 2},{\"id\": 11,\"name\": \"印度佛教史\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"印度佛教的发展史，可以分为八个阶段\",\"type\": 2}],\"art\": [{\"id\": 11,\"name\": \"印度佛教史\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"印度佛教的发展史，可以分为八个阶段\",\"type\": 3},{\"id\": 11,\"name\": \"印度佛教史\",\"pic\": \"http://tp1.sinaimg.cn/1215434384/50/5693231554/1\",\"summary\": \"印度佛教的发展史，可以分为八个阶段\",\"type\": 4}]},\"status\": 1}";
	base_logic::LogicComm::SendFull(socket,test_content.c_str(),test_content.length());
	return true;
}
}

