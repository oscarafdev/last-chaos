//---------------------------------------------------------------------->>
// Name : TCPD_Manager.h
// Desc : Tarissuis' Coding Panda Developements Main Manager File
// Date : [01/05/2016]
//----------------------------------------------------------------------<<

#pragma once

#ifndef	__TCPD_MANAGER_H__
#define	__TCPD_MANAGER_H__

#ifdef TCPD_DEV


class CTCPDManager
{
public:
	CTCPDManager();
	~CTCPDManager();

	void		RecvTCPDMsg(CNetworkMessage* istr);

	void		RecvEtcMsg(CNetworkMessage* istr);


#ifdef UNBIND_201601
private:
	void		RecvUnbindMsg(CNetworkMessage* istr);

public:
	void		SendUnbindMsg_Req(SWORD nMStoneTab, SWORD ubMStoneIdx, SLONG slMStone, SWORD nTargetTab, SWORD ubItemTgtIdx, SLONG slItemTgt);
#endif // UNBIND_201601


#ifdef USABILITY_201605
public:
	int			GetColorIndex(int nOldColor, CTString name);
#endif // USABILITY_201605

#ifdef CHATITEMTAG_201605
private:
	void		RecvChatItemTagMsg(CNetworkMessage* istr);
#endif // CHATITEMTAG_201605
};


#endif // TCPD_DEV

#endif // __TCPD_MANAGER_H__
