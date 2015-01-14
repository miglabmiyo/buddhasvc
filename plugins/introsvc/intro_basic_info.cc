/*
 * building_basic_info.cc
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#include "intro_basic_info.h"
namespace introsvc_logic{

IntroBasicInfo::IntroBasicInfo(){
	data_ = new Data();
}


IntroBasicInfo::IntroBasicInfo(const IntroBasicInfo& introinfo)
:data_(introinfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

IntroBasicInfo& IntroBasicInfo::operator =(const IntroBasicInfo& introinfo){
	if(introinfo.data_!=NULL){
		introinfo.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = introinfo.data_;
	return (*this);
}


IntroChapterInfo::IntroChapterInfo(){
	data_ = new Data();
}


IntroChapterInfo::IntroChapterInfo(const IntroChapterInfo& introinfo)
:data_(introinfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

IntroChapterInfo& IntroChapterInfo::operator =(const IntroChapterInfo& introinfo){
	if(introinfo.data_!=NULL){
		introinfo.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = introinfo.data_;
	return (*this);
}

}


