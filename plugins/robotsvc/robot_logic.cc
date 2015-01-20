#include "robot_logic.h"
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

namespace robotsvc_logic{

Robotlogic*
Robotlogic::instance_=NULL;

Robotlogic::Robotlogic(){
   if(!Init())
      assert(0);
}

Robotlogic::~Robotlogic(){
}

bool Robotlogic::Init(){
	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL){
		return false;
	}
	r = config->LoadConfig(path);

	robotsvc_logic::DBComm::Init(config->mysql_db_list_);
    return true;
}

Robotlogic*
Robotlogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Robotlogic();

    return instance_;
}



void Robotlogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Robotlogic::OnRobotConnect(struct server *srv,const int socket){

    return true;
}



bool Robotlogic::OnRobotMessage(struct server *srv, const int socket, const void *msg,const int len){
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
	 case ROBOT_BOOK_COLLECTION:
		 OnBookCollection(srv,socket,value);
		 break;
	}
    return true;
}

bool Robotlogic::OnRobotClose(struct server *srv,const int socket){

    return true;
}



bool Robotlogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Robotlogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Robotlogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Robotlogic::OnIniTimer(struct server *srv){

    return true;
}



bool Robotlogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}

bool Robotlogic::OnBookInfo(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
	scoped_ptr<netcomm_recv::BookInfo> bookinfo(new netcomm_recv::BookInfo(netbase));
	int error_code = bookinfo->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	std::list<BookInfo> list;
	std::string dst_bookinfo_list;
	std::string bookinfo_list = bookinfo->bookinfo_list();
	//urlcode解码
	bool r = base::BasicUtil::UrlDecode(bookinfo_list,dst_bookinfo_list);
	//获取内容解析章节信息
	if(r){
		ResplveTemplateList<BookInfo>(list,bookinfo_list);
		if(list.size()>0)
			robotsvc_logic::DBComm::CollectionBookInfo(list);
	}

	//回复
	scoped_ptr<netcomm_send::BookCollection> qcollection(new netcomm_send::BookCollection());
	//发送
	send_message(socket,(netcomm_send::HeadPacket*)qcollection.get());
	return true;
}

bool Robotlogic::OnBookCollection(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
		const void* msg,const int len){
	scoped_ptr<netcomm_recv::BookCollection> book_collection(new netcomm_recv::BookCollection(netbase));
	int error_code = book_collection->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	std::list<BookChapterCollection> list;
	std::string book_chapter_list = book_collection->book_chapter_list();

	//获取内容解析章节信息
	ResplveTemplateList<BookChapterCollection>(list,book_chapter_list);
	if(list.size()>0)
		robotsvc_logic::DBComm::CollectionBookChapter(list);

	//回复
	scoped_ptr<netcomm_send::BookCollection> qcollection(new netcomm_send::BookCollection());
	//发送
	send_message(socket,(netcomm_send::HeadPacket*)qcollection.get());
	return true;
}

template <typename ELEMENT>
bool Robotlogic::ResplveTemplateList(std::list<ELEMENT>& list,std::string& str){
	std::string error_str;
	int error_code = 0;
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_JSON,&str));
	base_logic::Value* value = serializer->Deserialize(&error_code,&error_str);
	//遍历
	base_logic::ListValue* value_list =(base_logic::ListValue*)value;
	int32 size = value_list->GetSize();
	int32 i = 0;
	while(i<size){
		base_logic::DictionaryValue* chapter_elemnet;
		//base_logic::Value* value = (base_logic::Value*)chapter_value;
		//value_list->Remove(0,&value);
		value_list->GetDictionary(i,&chapter_elemnet);
		ELEMENT book_element(chapter_elemnet);
		if(book_element.isadd())
			list.push_back(book_element);
		i++;
	}
	if(value_list){
		delete value_list;
		value_list = NULL;
	}
	return true;
}
/*
bool Robotlogic::ResolveChapterList(std::list<BookChapterCollection>& list,std::string& str){
	std::string error_str;
	int error_code = 0;
	scoped_ptr<base_logic::ValueSerializer> serializer(base_logic::ValueSerializer::Create(base_logic::IMPL_JSON,&str));
	base_logic::Value* value = serializer->Deserialize(&error_code,&error_str);

	//遍历
	base_logic::ListValue* value_list =(base_logic::ListValue*)value;
	int32 size = value_list->GetSize();
	int32 i = 0;
	while(i<size){
		base_logic::DictionaryValue* chapter_value;
		//base_logic::Value* value = (base_logic::Value*)chapter_value;
		//value_list->Remove(0,&value);
		value_list->GetDictionary(i,&chapter_value);
		BookChapterCollection book_chapter(chapter_value);
		list.push_back(book_chapter);
		i++;
	}
	if(value_list){
		delete value_list;
		value_list = NULL;
	}
	return true;
}*/


}

