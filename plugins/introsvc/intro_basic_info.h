/*
 * intro_basic_info.h
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#ifndef INTRO_BASIC_INFO_H_
#define INTRO_BASIC_INFO_H_


#include "basic/basictypes.h"
#include <string>
#include <list>
namespace introsvc_logic{


class IntroChapterInfo{
public:
	explicit IntroChapterInfo();
	IntroChapterInfo(const IntroChapterInfo& introinfo);
	IntroChapterInfo& operator = (const IntroChapterInfo& introinfo);

	const int64 id() const {return data_->id_;}
	const int64 bid() const {return data_->bid_;}
	const std::string& name() const {return data_->name_;}
	const std::string& chapter_url() const {return data_->chapter_url_;}

	void set_id(const int64 id) {data_->id_ = id;}
	void set_bid(const int64 bid) {data_->bid_ = bid;}
	void set_name(const std::string& name){data_->name_ = name;}
	void set_chapter_url(const std::string& chapter_url){data_->chapter_url_ = chapter_url;}

private:
	class Data{
	public:
		Data()
			:refcount_(1)
			,id_(0)
			,bid_(0){}
	public:
		int64 id_;
		int64 bid_;
		std::string name_;
		std::string chapter_url_;
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	private:
		int refcount_;
	};
	Data*       data_;
};

class IntroBasicInfo{
public:
	explicit IntroBasicInfo();
	IntroBasicInfo(const IntroBasicInfo& introinfo);
	IntroBasicInfo& operator = (const IntroBasicInfo& introinfo);

	void set_id(const int64 id){data_->id_ = id;}
	void set_type(const int32 type){data_->type_ = type;}
	void set_pic(const std::string& pic){data_->pic_ = pic;}
	void set_big_pic(const std::string& bigpic){data_->bigpic_ = bigpic;}
	void set_name(const std::string& name){data_->name_ = name;}
	void set_summary(const std::string& summary){data_->summary_ = summary;}
	void set_content(const std::string& content){data_->content_ = content;}

	const int64 id() const {return data_->id_;}
	const int32 type() const {return data_->type_;}
	const std::string& pic() const {return data_->pic_;}
	const std::string& bigpic() const {return data_->bigpic_;}
	const std::string& name() const {return data_->name_;}
	const std::string& summary() const {return data_->summary_;}
	const std::string& content() const {return data_->content_;}

private:
	class Data{
	public:
		Data()
			:refcount_(1)
			,id_(0)
			,type_(0){}
	public:
		int64 id_;
		int32 type_;
		std::string name_;
		std::string bigpic_;
		std::string pic_;
		std::string summary_;
		std::string content_;
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	private:
		int refcount_;
	};
	Data*       data_;
};
}


#endif /* INTRO_BASIC_INFO_H_ */
