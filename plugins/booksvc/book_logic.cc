#include "book_logic.h"
#include "db_comm.h"
#include "basic/scoped_ptr.h"
#include "net/operator_code.h"
#include "net/error_comm.h"
#include "logic/logic_unit.h"
#include "logic/logic_comm.h"
#include "config/config.h"
#include "common.h"

namespace sendsvc_logic{

Booklogic*
Booklogic::instance_=NULL;

Booklogic::Booklogic(){
   if(!Init())
      assert(0);
}

Booklogic::~Booklogic(){
	booksvc_logic::DBComm::Dest();
}

bool Booklogic::Init(){
	bool r = false;
	std::string path = DEFAULT_CONFIG_PATH;
	config::FileConfig* config = config::FileConfig::GetFileConfig();
	if(config==NULL){
		return false;
	}
	r = config->LoadConfig(path);

	booksvc_logic::DBComm::Init(config->mysql_db_list_);
    return true;
}

Booklogic*
Booklogic::GetInstance(){

    if(instance_==NULL)
        instance_ = new Booklogic();

    return instance_;
}



void Booklogic::FreeInstance(){
    delete instance_;
    instance_ = NULL;
}

bool Booklogic::OnBookConnect(struct server *srv,const int socket){

    return true;
}



bool Booklogic::OnBookMessage(struct server *srv, const int socket, const void *msg,const int len){
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
	 case BOOK_SEARCH_TYPE:
		 OnSearchBookByType(srv,socket,value);
		 break;
	 case BOOK_SUAMMARY:
		 OnGetBookSummary(srv,socket,value);
		 break;
	 case BOOK_LIST:
		 OnGetOwnBookList(srv,socket,value);
		 break;
	 case BOOK_WANT_GET:
		 OnWantGetBook(srv,socket,value);
		 break;
	 case BOOK_CHAPTER_LIST:
		 OnGetBookChapters(srv,socket,value);
		 break;

	}

    return true;
}

bool Booklogic::OnBookClose(struct server *srv,const int socket){

    return true;
}



bool Booklogic::OnBroadcastConnect(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}

bool Booklogic::OnBroadcastMessage(struct server *srv, const int socket, const void *msg,const int len){

    return true;
}



bool Booklogic::OnBroadcastClose(struct server *srv, const int socket){

    return true;
}

bool Booklogic::OnIniTimer(struct server *srv){

    return true;
}



bool Booklogic::OnTimeout(struct server *srv, char *id, int opcode, int time){

    return true;
}


bool Booklogic::OnSearchBookByType(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::SearchType> searchtype(new netcomm_recv::SearchType(netbase));
	std::list<booksvc_logic::BookBasicInfo> list;
	std::string token;
	bool r = false;
	int32 range = 30;
	int error_code = searchtype->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//查询
	booksvc_logic::DBComm::OnGetBooksByType(searchtype->btype(),list);

	//暂定最新和热门是一样的
	//发送客户端
	scoped_ptr<netcomm_send::BookSearch> booksearch(new netcomm_send::BookSearch());
	while(list.size()>0){
		booksvc_logic::BookBasicInfo bookinfo = list.front();
		list.pop_front();
		scoped_ptr<netcomm_send::BookInfo> sbook(new netcomm_send::BookInfo());
		sbook->set_search_info(bookinfo.id(),bookinfo.name(),bookinfo.pic(),bookinfo.summary(),bookinfo.type());
		booksearch->SetHotBookInfo(sbook.get());
		booksearch->SetNewBookInfo(sbook.release());
	}
	//发送
	send_message(socket,(netcomm_send::HeadPacket*)booksearch.get());
	return true;
}



bool Booklogic::OnGetBookSummary(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
		const void* msg,const int len){

	scoped_ptr<netcomm_recv::BookSummary> booksummary(new netcomm_recv::BookSummary(netbase));
	bool r = false;
	int32 issave;
	std::string token;
	booksvc_logic::BookBasicInfo bookinfo;
	int error_code = booksummary->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//
	r = booksvc_logic::DBComm::OnGetBookSummary(booksummary->uid(),booksummary->bookid(),issave,bookinfo);
	if(!r){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	scoped_ptr<netcomm_send::BookSummary> summary(new netcomm_send::BookSummary());
	summary->set_book_summary(bookinfo.author(),bookinfo.pubtime(),bookinfo.chapter(),
			bookinfo.summary(),bookinfo.freeurl(),bookinfo.fullurl());
	summary->set_user(issave);
	summary->set_label("哲史");
	send_message(socket,(netcomm_send::HeadPacket*)summary.get());
	return true;
}

bool Booklogic::OnGetOwnBookList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg,const int len){
	scoped_ptr<netcomm_recv::BookList> booklist(new netcomm_recv::BookList(netbase));
	bool r = false;
	int32 issave;
	std::string token;
	std::list<booksvc_logic::BookBasicInfo> list;
	int error_code = booklist->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//查询书单
	r = booksvc_logic::DBComm::OnGetBookList(booklist->uid(),list);
	scoped_ptr<netcomm_send::BookList> bookl(new netcomm_send::BookList());
	while(list.size()>0){
		booksvc_logic::BookBasicInfo bookinfo = list.front();
		list.pop_front();
		scoped_ptr<netcomm_send::BookInfo> sbook(new netcomm_send::BookInfo());
		sbook->set_book_list(bookinfo.id(),bookinfo.type(),bookinfo.name(),bookinfo.pic(),
				bookinfo.book_token());
		bookl->SetBookList(sbook.release());
	}
	send_message(socket,(netcomm_send::HeadPacket*)bookl.get());
	return true;
}

bool Booklogic::OnWantGetBook(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
		const void* msg,const int len){
	scoped_ptr<netcomm_recv::GetBook> book(new netcomm_recv::GetBook(netbase));
	int error_code = book->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}
	bool r = false;
	int32 status = 0;
	std::string booktoken;
	base_logic::LogicUnit::CreateToken(book->uid(),booktoken);
	//
	r = booksvc_logic::DBComm::OnWantGetBook(book->uid(),book->bookid(),booktoken);
	scoped_ptr<netcomm_send::GetBook> send_book(new netcomm_send::GetBook());
	send_book->SetBookToken(booktoken);
	send_message(socket,(netcomm_send::HeadPacket*)send_book.get());
	return true;
}

bool  Booklogic::OnGetBookChapters(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg,const int len){
	scoped_ptr<netcomm_recv::ChapterSummary> chapterlist(new netcomm_recv::ChapterSummary(netbase));
	bool r = false;
	int32 issave;
	std::string token;
	std::list<booksvc_logic::ChaptersInfo> list;
	int error_code = chapterlist->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//查询书单
	r = booksvc_logic::DBComm::OnGetBookChapters(/*chapterlist->bookid()*/6,list);
	scoped_ptr<netcomm_send::ChapterSummary> bookl(new netcomm_send::ChapterSummary());
	while(list.size()>0){
		booksvc_logic::ChaptersInfo chapterinfo = list.front();
		list.pop_front();
		scoped_ptr<netcomm_send::BookInfo> sbook(new netcomm_send::BookInfo());
		sbook->set_chapter_list(chapterinfo.id(),chapterinfo.bookid(),chapterinfo.chapter_url(),
				chapterinfo.chapter_name());
		bookl->SetBookList(sbook.release());
	}
	send_message(socket,(netcomm_send::HeadPacket*)bookl.get());
	return true;
}

/*
template<typename ListType,typename RecvType,typename SendTpe>
bool Booklogic::OnGetTemplateList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase){
	scoped_ptr<RecvType> booklist(new RecvType(netbase));
	bool r = false;
	std::list<ListType> list;
	int error_code = booklist->GetResult();
	if(error_code!=0){
		//发送错误数据
		send_error(error_code,socket);
		return false;
	}

	//查询书单
	r = booksvc_logic::DBComm::OnGetBookList(booklist->uid(),list);
	scoped_ptr<SendTpe> bookl(new netcomm_send::SendTpe());
	while(list.size()>0){
		booksvc_logic::BookBasicInfo bookinfo = list.front();
		list.pop_front();
		scoped_ptr<netcomm_send::BookInfo> sbook(new netcomm_send::BookInfo());
		sbook->set_book_list(bookinfo.id(),bookinfo.type(),bookinfo.name(),bookinfo.pic(),
				bookinfo.fullurl(),bookinfo.freeurl());
		bookl->SetBookList(sbook.release());
	}
	send_message(socket,(netcomm_send::HeadPacket*)bookl.get());
	return true;
}*/

}

