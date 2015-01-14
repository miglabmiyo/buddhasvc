/*

 * logic_infos.h
 *
 *  Created on: 2014年12月2日
 *      Author: kerry
 */
#ifndef _BUDDHA_LOGIC_INFOS_H__
#define _BUDDHA_LOGIC_INFOS_H__
#include "basic/basictypes.h"
#include <string>

namespace base_logic{

class LBSInfos{
public:
	explicit LBSInfos();
	explicit LBSInfos(const std::string& host,const double& latitude,
			const double& longitude,const std::string& city,const std::string& district,
			const std::string& province,std::string& street);

	LBSInfos(const LBSInfos& lbs_basic_info);
	LBSInfos& operator = (const LBSInfos& lbs_basic_info);

	const std::string& host() const {return data_->host_;}
	const double latitude() const {return data_->latitude_;}
	const double longitude() const {return data_->longitude_;}
	const std::string& city() const {return data_->city_;}
	const std::string& district() const {return data_->district_;}
	const std::string& province() const {return data_->province_;}
	const std::string& street() const {return data_->street_;}


private:
	class Data{
	public:
		Data():refcount_(1)
		,latitude_(0.0)
		,longitude_(0.0){}

		Data(const std::string& host,const double latitude,
				const double longitude,const std::string& city,const std::string& district,
				const std::string& province,std::string& street)
		:refcount_(1)
		,latitude_(latitude)
		,longitude_(longitude)
		,city_(city)
		,district_(district)
		,province_(province)
		,street_(street){}
		void AddRef(){refcount_ ++;}
		void Release(){if (!--refcount_)delete this;}
	public:
		const std::string host_;
		const double latitude_;
		const double longitude_;
		const std::string city_;
		const std::string district_;
		const std::string province_;
		const std::string street_;
	private:
		int refcount_;
	};
	Data*       data_;
};

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

#endif




