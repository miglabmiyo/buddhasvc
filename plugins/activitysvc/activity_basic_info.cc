/*
 * building_basic_info.cc
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#include "activity_basic_info.h"
namespace activitysvc_logic{

ActivityBasicInfo::ActivityBasicInfo(){
	data_ = new Data();
}


ActivityBasicInfo::ActivityBasicInfo(const ActivityBasicInfo& activityinfo)
:data_(activityinfo.data_){
	if(data_!=NULL){
		data_->AddRef();
	}
}

ActivityBasicInfo& ActivityBasicInfo::operator =(const ActivityBasicInfo& activityinfo){
	if(activityinfo.data_!=NULL){
		activityinfo.data_->AddRef();
	}

	if(data_!=NULL){
		data_->Release();
	}
	data_ = activityinfo.data_;
	return (*this);
}

}


