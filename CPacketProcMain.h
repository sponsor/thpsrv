#ifndef H_CLASS_PACKET_PROC_MAIN___
#define H_CLASS_PACKET_PROC_MAIN___

#include "windows.h"
#include "TCHAR.h"
#include <assert.h>
#include "util.h"
#include "../include/define.h"
#include "../include/types.h"
#include "PacketMaker.h"
#include "CNetworkSession.h"
#include "CPacketProcPhase.h"
#include <lua.hpp>
#include "tolua++.h"
#include "tolua_glue/thg_glue.h"
#include "LuaHelper.h"
#include "BaseVector.h"
#include "CSyncMain.h"

#define PACKET_MAIN_MV_INDEX				(3)

class CPacketProcMain	: public CPacketProcPhase
{
public:
	CPacketProcMain():CPacketProcPhase()
	{
		m_eGamePhase = GAME_PHASE_MAIN;
	};
	virtual ~CPacketProcMain()
	{
	};

	// ������
	virtual BOOL Init(CNetworkSession *pNWSess)
	{
		BOOL ret = CPacketProcPhase::Init(pNWSess);
		m_eGamePhase = GAME_PHASE_MAIN;
		return ret;
	};
	// ������
	virtual BOOL Init(CNetworkSession *pNWSess, CPacketProcPhase* pPrevPhase);

	// �p�P�b�g���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL PacketProc(BYTE *data, ptype_session sess);
	// �ؒf���� / return TRUE/FALSE:���M�p�P�b�g�L��/����
	virtual BOOL DisconnectSession(ptype_session sess);

	virtual INT MakeTriggerPacket(ptype_session sess, BYTE* data);
	// ���ˏ���
	virtual BOOL ShootingBullet(ptype_session sess, int nFrame);
	
	// ���˃R�}���h
	virtual bool ShootingCommand(ptype_session sess, int nProcType,int nCharaID, int nBltType,int nAngle,int nPower, int nChrObjNo, int nFrame, int indicator_angle, int indicator_short);

	// �A�˂��邩�m�F
	virtual BOOL IsDoubleShot(ptype_session sess);

	// ���˂̃p���[�ݒ�
	virtual void SetSaveShotPower(int value){	m_nSaveShotPower = value;	};

protected:
//	CNetworkSession	*p_pNWSess;
//	type_queue	m_tQueue;			// �p�P�b�g���p
/*
	E_STATE_GAME_PHASE m_eGamePhase;
	ptype_session m_pMasterSession;
	int m_nTeamCount;
	BYTE m_bytRuleFlg;
	int m_nStageIndex;
*/
	// �ꔭ�ڂ̏����L�^
	short m_sSaveShotAngle;				// �e�p�x
	int m_nSaveShotPower;					// �e����
	int m_nSaveBltType;						// �e�̎��
	int m_nSaveCharaType;					// �L�����̎��
	int m_nSaveProcType;					// �����̎��
	int m_nSaveCharaObjNo;				// �L�����I�u�W�F�N�g�ԍ�
	short m_sSaveIndicatorAngle;
	int m_nSaveIndicatorPower;
	TCHARA_SCR_INFO* m_pSaveScrInfo;

	BOOL LoadStage();
	BOOL PutCharacter(std::vector<int>* vecGrounds, ptype_session sess);
	BOOL SetMove(ptype_session sess, BYTE* data);
	BOOL SetFlip(ptype_session sess, BYTE* data);
	BOOL UpdatePos(ptype_session sess, BYTE* data);
	BOOL SetShot(ptype_session sess, BYTE* data);
	BOOL SetAck(ptype_session sess);
	BOOL TurnPass(ptype_session sess);
	BOOL OrderItem(ptype_session sess,BYTE* data);
	BOOL UseItem(ptype_session sess, DWORD item_flg);
	BOOL StealItem(ptype_session sess, DWORD item_flg);
	BOOL SetTrigger(ptype_session sess,BYTE* data);
	BOOL RcvTriggerEnd(ptype_session sess,BYTE* data);
	BOOL SetTurnPass(ptype_session sess,BYTE* data);
	BOOL SetShotPowerStart(ptype_session sess, BYTE* data);
};

#endif
