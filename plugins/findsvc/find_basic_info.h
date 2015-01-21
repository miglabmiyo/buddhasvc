/*
 * find_basic_info.h
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#ifndef FIND_BASIC_INFO_H_
#define FIND_BASIC_INFO_H_


#include "basic/basictypes.h"
#include <string>
#include <list>
namespace findsvc_logic{


class FindBookInfo{
public:
	explicit FindBookInfo();
	FindBookInfo(const FindBookInfo& bookinfo);
	FindBookInfo& operator = (const FindBookInfo& bookinfo);
public:

	void set_id(const int64 id){data_->id_ = id;}
	void set_type(const int32 type){data_->type_ = type;}
	void set_attr(const int32 attr){data_->attr_ = attr;}
	void set_pic(const std::string& pic){data_->pic_ = pic;}
	void set_name(const std::string& name){data_->name_ = name;}
	void set_summary(const std::string& summary){data_->summary_ = summary;}

	const int64 id() const   {return data_->id_;}
	const int32 type() const {return data_->type_;}
	const int32 attr() const {return data_->attr_;}
	const std::string& pic() const {return data_->pic_;}
	const std::string& name() const {return data_->name_;}
	const std::string& summary() const {return data_->summary_;}

private:
	class Data{
	public:
		Data()
			:refcount_(1)
			,id_(0)
			,type_(0)
			,attr_(0){}
	public:
		int64       id_;
		int32       type_;
		int32       attr_;
		std::string name_;
		std::string pic_;
		std::string summary_;
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	private:
		int refcount_;
	};
	Data*       data_;
};
}


#endif /* BUILDING_BASIC_INFO_H_ */
