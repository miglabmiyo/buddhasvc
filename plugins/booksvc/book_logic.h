#ifndef __BUDDHA__BOOKSVC__BOOK_LOGIC___
#define __BUDDHA__BOOKSVC__BOOK_LOGIC___
#include "book_basic_info.h"
#include "logic/logic_infos.h"
#include "net/book_comm_head.h"
#include "protocol/http_packet.h"
#include "common.h"

#define DEFAULT_CONFIG_PATH     "./plugins/booksvc/booksvc_config.xml"
namespace sendsvc_logic{

class Booklogic{

public:
    Booklogic();
    virtual ~Booklogic();

private:
    static Booklogic    *instance_;

public:
    static Booklogic *GetInstance();
    static void FreeInstance();

public:
    bool OnBookConnect (struct server *srv,const int socket);

    bool OnBookMessage (struct server *srv, const int socket,const void *msg, const int len);

    bool OnBookClose (struct server *srv,const int socket);

    bool OnBroadcastConnect(struct server *srv,const int socket,const void *data, const int len);

    bool OnBroadcastMessage (struct server *srv, const int socket, const void *msg, const int len);

    bool OnBroadcastClose (struct server *srv, const int socket);

    bool OnIniTimer (struct server *srv);

    bool OnTimeout (struct server *srv, char* id, int opcode, int time);

private:
    bool OnSearchBookByType(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnGetBookSummary(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnGetOwnBookList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnWantGetBook(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
    		const void* msg = NULL,const int len = 0);

    bool OnGetBookChapters(struct server *srv,const int socket,netcomm_recv::NetBase* netbase,
       		const void* msg = NULL,const int len = 0);

private:
   // template<typename ListType,typename RecvType,typename SendTpe>
    //bool OnGetTemplateList(struct server *srv,const int socket,netcomm_recv::NetBase* netbase);
private:

    bool Init();
};


}

#endif

