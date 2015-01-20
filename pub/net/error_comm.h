/*
 * error_comm.h
 *
 *  Created on: 2014年12月5日
 *      Author: kerry
 */

#ifndef ERROR_COMM_H_
#define ERROR_COMM_H_

enum errorcode{
	STRUCT_ERROR = -100,
	TYPE_LACK = -1000,
	IMEI_LACK = -1101,
	MACHINE_LACK = -1102,
	SOUCE_LACK = -1103,
	NICKNAME_LACK = -1104,
	SEX_LACK = -1105,
	SESSION_LACK = -1106,
	PLATFORM_LACK = -1107,
	UID_LACK = -1108,
	TOKEN_LACK = -1109,
	BD_CHANNEL_LACK = -1110,
	BD_USERID_LACK = -1111,
	PKG_NAME_LACK = -1112,
	TAG_LACK = -1113,
	APPID_LACK = -1114,
	REQUEST_LACK = -1115,
	BTYPE_LACK = -1116,
	BUILD_ID_LACK = -1117,
	BOOK_ID_LACK = -1118,
	BOOK_TOKEN = -1119,
	ACTIVITY_ID_LACK = -1120,
	INTRO_ID_LACK = -1121,
	BOOK_NAME_LACK = -1122,
	BOOK_HASH_NAME_LACK = -1123,
	BOOK_INTRO_LACK = -1124,
	BOOK_COVER_LACK = -1125
};


#define STRUCT_ERROR_STR                  "请求结构错误"
#define TYPE_LACK_STR                     "登陆类别不存在"
#define IMEI_LACK_STR                     "缺少IMEI"
#define MACHINE_LACK_STR                  "缺少机型"
#define SOUCE_LACK_STR                    "缺少登陆来源"
#define NICKNAME_LACK_STR                 "缺少昵称"
#define SEX_LACK_STR                      "缺少性别"
#define SESSION_LACK_STR                  "缺少第三方凭据"
#define PLATFORM_LACK_STR                 "缺少第三方平台信息"
#define UID_LACK_STR                      "缺少用户ID"
#define TOKEN_LACK_STR                    "TOKEN不存在或错误"
#define BD_CHANNEL_LACK_STR               "缺少百度绑定信息"
#define BD_USER_LACK_STR                  "缺少百度ID"
#define PKG_NAME_LACK_STR                 "缺少包名"
#define TAG_LACK_STR                      "缺少标签"
#define APPID_LACK_STR                    "缺少APPID"
#define REQUEST_LACK_STR                  "缺少请求ID"
#define BTYPE_LACK_STR                    "缺少类别"
#define BUILD_ID_LACK_STR                 "缺少商家ID"
#define BOOK_ID_LACK_STR                  "缺少书籍ID"
#define BOOK_TOKEN_STR                    "缺少购买凭据"
#define ACTIVITY_ID_LACK_STR              "缺少活动ID"
#define INTRO_ID_LACK_STR                 "缺少介绍ID"
#define BOOK_NAME_LACK_STR                "缺少书名"
#define BOOK_HASH_NAME_LACK_STR           "缺少书名HASH"
#define BOOK_INTRO_LACK_STR               "缺少书介绍"
#define BOOK_COVER_LACK_STR               "缺少封面"


const char*
buddha_strerror(int error_code);
#endif /* ERROR_COMM_H_ */
