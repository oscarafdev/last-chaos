//---------------------------------------------------------------------->>
// Name : Define_TCPD.h
// Desc : Tarissuis' Coding Panda Developements Setting File
// Date : [01/05/2016]
//----------------------------------------------------------------------<<

#pragma once

#ifndef	__TPCD_H__
#define	__TPCD_H__

#ifndef _CLIENT_  //HAD TO CHANGE TO IFDEF INSTEAD OF IFNDEF
	#define TCPD_DEV					// add this to your preprocessor settings in case of ClientSRC
#endif

// Performace & Security Settings
#ifdef TCPD_DEV
	#define DC_FAULTY_PLAYERS			// Players that sent invalid packets are being disconnected
#endif

// Main Projects
#ifdef TCPD_DEV
	#define USABILITY_201605
	#define CHATITEMTAG_201605
	#define REBORN_201512
	//#define ARTIFACT_201605				// applies more control towards artifact objects and their owners
	// Allows a player to tag a specific item from his inventory in the chat for everyone else to see and display its info
	//#define TOGGLEBOOST_201601			// Automatically uses certain items when toggled
	//#define REMOVEONPK_201601			// Removed/Disables certain stuff while a player is in PK mode
	//#define UNBIND_201601				// Allows a player to unbind character bound weapons and armor -> UIMixNew.h/.cpp
	
#endif


// Reborn Settings
#ifdef REBORN_201512
	#define REBORN_201512_UPDATE_1
	#define REBORN_201512_NPC			179			// Reborn NPC - Dialogue gets completely overriden, can only be used for reborn system!
	#define REBORN_201512_PREMIUM		20819			// Item that lets a player keep affinity, SP, ... when reborning
	#define REBORN_201512_CLASSREQ		20968			// Item that is required to change class to another (only classchange, no reborn!)
#endif

// Unbind Settings
#ifdef UNBIND_201601
	#define UNBIND_201601_TOKEN			20991			// Item that is required to unbind a bound weapon or armor item
#endif

// Toggle Booster Settings
#ifdef TOGGLEBOOST_201601
	#define TOGGLEBOOST_201601_EXP		50058				// Item Index of the toggable EXP Booster
	#define TOGGLEBOOST_201601_SP		50077				// Item Index of the toggable SP Booster
	#define TOGGLEBOOST_201601_DROP		50078				// Item Index of the toggable DROP Booster
	#define TOGGLEBOOST_201601_GOLD		50087				// Item Index of the toggable GOLD Booster
#endif

#ifdef REMOVEONPK_201601
	#define REMOVEONPK_201601_TABLE		"t_pk_notallowed"	// Name of the table in the data db
	#define REMOVEONPK_201601_NOTIN		25				// Zone in which the system doesn't apply
#endif

//User Online Counter
#ifdef USABILITY_201605
	#define USABILITY_201605
#endif

#endif // __TPCD_H__