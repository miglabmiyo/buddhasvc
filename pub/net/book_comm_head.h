/*
 * book_comm_head.h
 *
 *  Created on: 2014年12月26日
 *      Author: kerry
 */

#ifndef BOOK_COMM_HEAD_H_
#define BOOK_COMM_HEAD_H_

#include "net/comm_head.h"
#include "basic/basictypes.h"
#include "basic/scoped_ptr.h"
#include "logic/base_values.h"
#include <list>
#include <string>
#include <sstream>

namespace netcomm_recv{

//书籍详情
class BookSummary: public LoginHeadPacket{
public:
	BookSummary(NetBase* m)
	:LoginHeadPacket(m){
		bool r = false;
		GETBIGINTTOINT(L"bookid",bookid_);
		if(!r) error_code_ = BOOK_ID_LACK;
	}

	const int64 bookid() {return this->bookid_;}
private:
	int64 bookid_;
};

//推荐书籍
class BookRecomm:public LoginHeadPacket{
public:
	BookRecomm(NetBase* m)
	:LoginHeadPacket(m){}
};

//类别搜索
class SearchType:public LoginHeadPacket{
public:
	SearchType(NetBase* m)
	:LoginHeadPacket(m){
		bool r = false;
		GETBIGINTTOINT(L"btype",btype_);
		if(!r) error_code_ = BTYPE_LACK;
	}
	const int64 btype() {return this->btype_;}
private:
	int64 btype_;
};

//获取用户书单
class BookList:public LoginHeadPacket{
public:
	BookList(NetBase* m)
	:LoginHeadPacket(m){
	}
};

//提交已购买书籍
class GetBook:public LoginHeadPacket{
public:
	GetBook(NetBase* m)
	:LoginHeadPacket(m){
		bool r = false;
		GETBIGINTTOINT(L"bookid",bookid_);
		if(!r) error_code_ = BOOK_ID_LACK;
	}
	const int64 bookid() {return this->bookid_;}
private:
	int64 bookid_;
};

//获取详细章节
class ChapterSummary:public GetBook{
public:
	ChapterSummary(NetBase* m)
	:GetBook(m){
		bool r = false;
		r = m_->GetString(L"booktoken",&booktoken_);
		if(!r) error_code_ = BOOK_TOKEN;
	}
private:
	std::string booktoken_;
};

}


namespace netcomm_send{
//书籍详情
class BookSummary:public HeadPacket{
public:
	BookSummary(){
		base_.reset(new netcomm_send::NetBase());
		book_summary_.reset(new base_logic::DictionaryValue());
		user_.reset(new base_logic::DictionaryValue());
		label_.reset(new base_logic::ListValue());
	}

	virtual ~BookSummary(){

	}

	netcomm_send::NetBase* release(){
		if(book_summary_->size()!=0)
			this->base_->Set(L"summary",book_summary_.release());
		if(user_->size()!=0)
			this->base_->Set(L"user",user_.release());
		if(!label_->empty())
			this->base_->Set(L"label",label_.release());

		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();

	}

	void set_book_summary(const std::string& author,const std::string& pubtime,
			const int32 chapter,const std::string& summary,const std::string& free,
			const std::string& full){
		if(!author.empty())
			book_summary_->SetString(L"author",author);
		if(!pubtime.empty())
			book_summary_->SetString(L"pubtime",pubtime);
		book_summary_->SetInteger(L"chapter",chapter);
		if(!free.empty())
			book_summary_->SetString(L"free",free);
		if(!full.empty())
			book_summary_->SetString(L"full",full);
		if(!summary.empty())
			book_summary_->SetString(L"summary",summary);
	}

	void set_user(const int32 issave){
		user_->SetInteger(L"issave",issave);
	}

	void set_label(const std::string& label){
		label_->Append(base_logic::Value::CreateStringValue(label));
	}

private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::DictionaryValue>       book_summary_;
	scoped_ptr<base_logic::DictionaryValue>       user_;
	scoped_ptr<base_logic::ListValue>             label_;
};


//书籍个体
class BookInfo:public base_logic::DictionaryValue{
public:
	BookInfo(){
	}
	virtual ~BookInfo(){}
public:
	inline void  set_search_info(const int64 id,const std::string& name,const std::string& pic,
			const std::string& summary,const int64 type){
		this->SetBigInteger(L"id",id);
		this->SetString(L"name",name);
		this->SetString(L"pic",pic);
		this->SetString(L"summary",summary);
		this->SetBigInteger(L"type",type);
	}
	inline void set_book_list(const int64 id,const int32 type,const std::string& name,const std::string& pic,
			const std::string& book_token){
		this->SetBigInteger(L"id",id);
		this->SetInteger(L"type",type);
		this->SetString(L"name",name);
		this->SetString(L"pic",pic);
		this->SetString(L"booktoken",book_token);
	}

	inline void set_chapter_list(const int64 id,const int64 bookid,const std::string& chapter_url,
			const  std::string& chapter_name){
		this->SetBigInteger(L"id",id);
		this->SetBigInteger(L"bookid",bookid);
		this->SetString(L"url",chapter_url);
		this->SetString(L"name",chapter_name);
	}
};

//推荐书籍
class BookRecomm:public HeadPacket{
public:

	BookRecomm(){
		base_.reset(new netcomm_send::NetBase());
		hot_book_.reset(new base_logic::ListValue());
		scrip_book_.reset(new base_logic::ListValue());
		ritual_book_.reset(new base_logic::ListValue());
		theory_book_.reset(new base_logic::ListValue());
	}

	virtual ~BookRecomm(){

	}

	inline void SetHotBookInfo(base_logic::DictionaryValue* build){
		hot_book_->Append(build);
	}

	inline void SetScripBookInfo(base_logic::DictionaryValue* build){
		scrip_book_->Append(build);
	}

	inline void SetRitualBookInfo(base_logic::DictionaryValue* build){
		ritual_book_->Append(build);
	}

	inline void SetTheoryBookInfo(base_logic::DictionaryValue* build){
		theory_book_->Append(build);
	}

	netcomm_send::NetBase* release(){
		if(!hot_book_->empty())
			this->base_->Set(L"hot",hot_book_.release());
		if(!scrip_book_->empty())
			this->base_->Set(L"scriptures",scrip_book_.release());
		if(!ritual_book_->empty())
			this->base_->Set(L"ritual",ritual_book_.release());
		if(!theory_book_->empty())
			this->base_->Set(L"theory",theory_book_.release());
		head_->Set("result",base_.release());

		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             hot_book_;
	scoped_ptr<base_logic::ListValue>             scrip_book_;
	scoped_ptr<base_logic::ListValue>             ritual_book_;
	scoped_ptr<base_logic::ListValue>             theory_book_;
};

//类别搜索
class BookSearch:public HeadPacket{
public:
	BookSearch(){
		base_.reset(new netcomm_send::NetBase());
		hot_book_.reset(new base_logic::ListValue());
		new_book_.reset(new base_logic::ListValue());
	}

	inline void SetHotBookInfo(base_logic::DictionaryValue* build){
		hot_book_->Append(build);
	}

	inline void SetNewBookInfo(base_logic::DictionaryValue* build){
		new_book_->Append(build);
	}


	netcomm_send::NetBase* release(){
		if(!hot_book_->empty())
			this->base_->Set(L"hot",hot_book_.release());
		if(!new_book_->empty())
			this->base_->Set(L"new",new_book_.release());

		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             hot_book_;
	scoped_ptr<base_logic::ListValue>             new_book_;
};

//书单
class BookList:public HeadPacket{
public:
	BookList(){
		base_.reset(new netcomm_send::NetBase());
		book_list_.reset(new base_logic::ListValue());
	}
	inline void SetBookList(base_logic::DictionaryValue* build){
		book_list_->Append(build);
	}

	netcomm_send::NetBase* release(){
		if(!book_list_->empty())
			this->base_->Set(L"list",book_list_.release());

		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             book_list_;
};

//提交
class GetBook:public HeadPacket{
public:
	GetBook(){
		base_.reset(new netcomm_send::NetBase());
	}
	~GetBook(){}

	inline void SetBookToken(const std::string& token){
		this->base_->SetString(L"book_token",token);
	}
	netcomm_send::NetBase* release(){
		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
};

//获取章节详情
class ChapterSummary:public HeadPacket{
public:
	ChapterSummary(){
		base_.reset(new netcomm_send::NetBase());
		chapter_list_.reset(new base_logic::ListValue());
	}
	inline void SetBookList(base_logic::DictionaryValue* build){
		chapter_list_->Append(build);
	}
	netcomm_send::NetBase* release(){
		if(!chapter_list_->empty())
			this->base_->Set(L"list",chapter_list_.release());

		head_->Set("result",base_.release());
		this->set_status(1);
		return head_.release();
	}
private:
	scoped_ptr<netcomm_send::NetBase>             base_;
	scoped_ptr<base_logic::ListValue>             chapter_list_;
};


}

#endif /* BOOK_COMM_HEAD_H_ */
