/*
 * building_basic_info.cc
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#include "book_basic_info.h"
namespace booksvc_logic{


ChaptersInfo::ChaptersInfo(){
	data_ = new Data();
}


ChaptersInfo::ChaptersInfo(const ChaptersInfo& chaptersinfo)
:data_(chaptersinfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

ChaptersInfo& ChaptersInfo::operator =(const ChaptersInfo& chaptersinfo){
	if(chaptersinfo.data_!=NULL){
		chaptersinfo.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = chaptersinfo.data_;
	return (*this);
}


BookBasicInfo::BookBasicInfo(){
	data_ = new Data();
}


BookBasicInfo::BookBasicInfo(const BookBasicInfo& bookinfo)
:data_(bookinfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

BookBasicInfo& BookBasicInfo::operator =(const BookBasicInfo& bookinfo){
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


