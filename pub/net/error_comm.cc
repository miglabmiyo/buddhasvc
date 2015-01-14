/*
 * error_comm.cc
 *
 *  Created on: 2014年12月5日
 *      Author: kerry
 */
#include "net/error_comm.h"

const char*
buddha_strerror(int error_code){
	switch (error_code) {
	  case STRUCT_ERROR:
		  return STRUCT_ERROR_STR;
	  case TYPE_LACK:
		  return TYPE_LACK_STR;
	  case IMEI_LACK:
		  return IMEI_LACK_STR;
	  case MACHINE_LACK:
		  return MACHINE_LACK_STR;
	  case SOUCE_LACK:
		  return SOUCE_LACK_STR;
	  case NICKNAME_LACK:
		  return NICKNAME_LACK_STR;
	  case SEX_LACK :
		  return SEX_LACK_STR;
	  case SESSION_LACK:
		  return SESSION_LACK_STR;
	  case PLATFORM_LACK:
		  return PLATFORM_LACK_STR;
	  case UID_LACK:
		  return UID_LACK_STR;
	  case TOKEN_LACK:
		  return TOKEN_LACK_STR;
	  case BD_CHANNEL_LACK:
		  return BD_CHANNEL_LACK_STR;
	  case BD_USERID_LACK:
		  return BD_USER_LACK_STR;
	  case PKG_NAME_LACK :
		  return PKG_NAME_LACK_STR;
	  case APPID_LACK:
		  return APPID_LACK_STR;
	  case REQUEST_LACK:
		  return REQUEST_LACK_STR;
	  case BTYPE_LACK:
		  return BTYPE_LACK_STR;
	  case BUILD_ID_LACK:
		  return BUILD_ID_LACK_STR;
	  case BOOK_ID_LACK:
		  return BOOK_ID_LACK_STR;
	  case BOOK_TOKEN:
		  return BOOK_TOKEN_STR;
	  case ACTIVITY_ID_LACK:
		  return ACTIVITY_ID_LACK_STR;
	  case ACTIVITY_ID_LACK:
		  return ACTIVITY_ID_LACK_STR;
	  case INTRO_ID_LACK:
		  return INTRO_ID_LACK_STR;
	}
}


