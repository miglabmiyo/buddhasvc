/*
 * find_basic_info.cc
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#include "find_basic_info.h"
namespace findsvc_logic{



FindBookInfo::FindBookInfo(){
	data_ = new Data();
}


FindBookInfo::FindBookInfo(const FindBookInfo& bookinfo)
:data_(bookinfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

FindBookInfo& FindBookInfo::operator =(const FindBookInfo& bookinfo){
	if(bookinfo.data_!=NULL){
		bookinfo.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = bookinfo.data_;
	return (*this);
}


}


