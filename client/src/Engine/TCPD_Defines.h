//---------------------------------------------------------------------->>
// Name : TCPD_Defines.h
// Desc : Tarissuis' Coding Panda Developements Defines File
// Date : [01/05/2016]
//----------------------------------------------------------------------<<

#pragma once

#ifndef __TCPD_DEFINES_H__
#define __TCPD_DEFINES_H__

#include "Define_TCPD.h"

#ifdef TCPD_DEV

namespace TCPD
{
	enum eMsgTCPD
	{
		MSG_TCPD_GENERAL,
		MSG_REBORN_201512,
		MSG_UNBIND_201601,
		MSG_ALBERWAR_201601,
		MSG_PVPWARFARE_201602,
		MSG_PVPWARFARE_201602_HELPER,
		MSG_CHATITEMTAG_201605,
	};
	
	enum eMsgTCPDEtc
	{
		MSG_TCPD_ETC_DEBUG_ON,
		MSG_TCPD_ETC_DEBUG_OFF,
		MSG_TCPD_ETC_ARTIFACT_NOTICE_3MIN,
		MSG_TCPD_ETC_ARTIFACT_NOTICE_1MIN,
		MSG_TCPD_ETC_ARTIFACT_NOTICE_TIMEOUT,
		MSG_TCPD_ETC_ARTIFACT_NOTICE_REMAINING,
	};

  #ifdef REBORN_201512
	enum eMsgReborn2015
	{
		MSG_REBORN_REQ_REBORN,
		MSG_REBORN_REQ_STATS,
		MSG_REBORN_ERR_WRONGLEVEL,
		MSG_REBORN_ERR_NOMONEY,
		MSG_REBORN_ERR_MAXREBORN,
		MSG_REBORN_ERR_NOINVENSPACE,
	  #ifdef REBORN_201512_UPDATE_1
		MSG_REBORN_REQ_CLASSCHANGE,
		MSG_REBORN_ERR_CLASSCHANGE_NSLEVEL,
	  #endif
	};
  #endif // REBORN_201512

  #ifdef UNBIND_201601
	enum eMsgUnbind2016
	{
		MSG_UNBIND_SENDTOSERVER,
		MSG_UNBIND_RECVFROMSERVER,
	};
  #endif // UNBIND_201601


	enum eMsgChatItemItag2016
	{
		MSG_CHATITEMTAG_RECV,
	};
}


#endif // TCPD_DEV

#endif // __TCPD_DEFINES_H__
