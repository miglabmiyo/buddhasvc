/*
 * building_basic_info.h
 *
 *  Created on: 2014年12月17日
 *      Author: kerry
 */

#ifndef BUILDING_BASIC_INFO_H_
#define BUILDING_BASIC_INFO_H_


#include "basic/basictypes.h"
#include <string>
#include <list>
namespace buildingsvc_logic{

class BuildingBasicInfo{
public:
	explicit BuildingBasicInfo();
	BuildingBasicInfo(const BuildingBasicInfo& buildinfo);
	BuildingBasicInfo& operator = (const BuildingBasicInfo& buildinfo);
public:
	void set_id(const int64 id){data_->id_ = id;}
	void set_type(const int32 type) {data_->type_ = type;}
	void set_latitude(const double latitude){data_->latitude_ = latitude;}
	void set_longitude(const double longitude){data_->longitude_ = longitude;}
	void set_pic(const std::string& pic){data_->pic_ = pic;}
	void set_address(const std::string& address) {data_->address_ = address;}
	void set_name(const std::string& name) {data_->name_ = name;}
	void set_city(const std::string& city) {data_->city_ = city;}
	void set_district(const std::string& district) {data_->district_ = district;}
	void set_distance(const double distance) {data_->distance_ = distance;}

	const int64 id() const {return data_->id_;}
	const int32 type() const {return data_->type_;}
	const double latitude() const {return data_->latitude_;}
	const double longitude() const {return data_->longitude_;}
	const double distance() const {return data_->distance_;}
	const std::string& pic() const {return data_->pic_;}
	const std::string& address() const {return data_->address_;}
	const std::string& name() const {return data_->name_;}
	const std::string& city() const {return data_->city_;}
	const std::string& district() const {return data_->district_;}
public:
	static bool cmp(BuildingBasicInfo& t_info,BuildingBasicInfo& r_info){
		return t_info.distance() < r_info.distance();
	}
private:
	class Data{
	public:
		Data()
			:refcount_(1)
			,id_(0)
			,type_(0)
			,latitude_(0)
			,longitude_(0)
			,distance_(0){}
	public:
		int64 id_;
		int32 type_;
		double latitude_;
		double longitude_;
		double distance_;
		std::string pic_;
		std::string address_;
		std::string name_;
		std::string city_;
		std::string district_;
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	private:
		int refcount_;
	};
	Data*       data_;
};
}


#endif /* BUILDING_BASIC_INFO_H_ */
