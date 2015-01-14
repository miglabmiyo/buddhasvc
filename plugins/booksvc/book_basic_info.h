/*
 * building_basic_info.h
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#ifndef BOOK_BASIC_INFO_H_
#define BOOK_BASIC_INFO_H_


#include "basic/basictypes.h"
#include <string>
#include <list>
namespace booksvc_logic{


class ChaptersInfo{
public:
	explicit ChaptersInfo();
	ChaptersInfo(const ChaptersInfo& chaptersinfo);
	ChaptersInfo& operator = (const ChaptersInfo& bookinfo);


	void set_id(const int64 id){data_->id_ = id;}
	void set_bookid(const int64 bookid){data_->bookid_ = bookid;}
	void set_chapter_url(const std::string& chapter_url){data_->chapter_url_ = chapter_url;}
	void set_chapter_name(const std::string& chapter_name) {data_->chapter_name_ = chapter_name;}

	const int64 id() const {return data_->id_;}
	const int64 bookid() const {return data_->bookid_;}
	const std::string& chapter_url() const {return data_->chapter_url_;}
	const std::string& chapter_name() const {return data_->chapter_name_;}
public:
private:
	class Data{
	public:
		Data():refcount_(0)
			,id_(0)
			,bookid_(0){}
	public:
		int64           id_;
		int64           bookid_;
		std::string     chapter_url_;
		std::string     chapter_name_;
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	private:
		int refcount_;
	};
	Data*       data_;
};
class BookBasicInfo{
public:
	explicit BookBasicInfo();
	BookBasicInfo(const BookBasicInfo& bookinfo);
	BookBasicInfo& operator = (const BookBasicInfo& bookinfo);
public:

	void set_id(const int64 id){data_->id_ = id;}
	void set_type(const int32 type){data_->type_ = type;}
	void set_chapter(const int32 chapter){data_->chapter_ = chapter;}
	void set_author(const std::string& author){data_->author_ = author;}
	void set_pubtime(const std::string& pubtime){data_->pubtime_ = pubtime;}
	void set_pic(const std::string& pic){data_->pic_ = pic;}
	void set_name(const std::string& name){data_->name_ = name;}
	void set_summary(const std::string& summary){data_->summary_ = summary;}
	void set_freeurl(const std::string& freeurl){data_->free_url_ = freeurl;}
	void set_fullurl(const std::string& fullurl) {data_->full_url_ = fullurl;}
	void set_booktoken(const std::string& book_token){data_->book_token_ = book_token;}

	const int64 id() const   {return data_->id_;}
	const int32 type() const {return data_->type_;}
	const int32 chapter() const {return data_->chapter_;}
	const std::string& author() const {return data_->author_;}
	const std::string& pubtime() const {return data_->pubtime_;}
	const std::string& pic() const {return data_->pic_;}
	const std::string& name() const {return data_->name_;}
	const std::string& summary() const {return data_->summary_;}
	const std::string& freeurl() const {return data_->free_url_;}
	const std::string& fullurl() const {return data_->full_url_;}
	const std::string& book_token() const {return data_->book_token_;}

private:
	class Data{
	public:
		Data()
			:refcount_(1)
			,id_(0)
			,type_(0)
			,chapter_(0){}
	public:
		int64       id_;
		int32       type_;
		int32       chapter_;
		std::string name_;
		std::string author_;
		std::string pubtime_;
		std::string pic_;
		std::string summary_;
		std::string free_url_;
		std::string full_url_;
		std::string book_token_;
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	private:
		int refcount_;
	};
	Data*       data_;
};
}


#endif /* BUILDING_BASIC_INFO_H_ */
