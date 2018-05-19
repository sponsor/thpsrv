#include "CPacketProcPhase.h"
#include "ext.h"


//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


//> public >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//> �ؒf����
BOOL CPacketProcPhase::DisconnectSession(ptype_session sess)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;

	// �F�؍ς݃��[�U�[�ȊO�͏������Ȃ�
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	// �ؒf�����Z�b�V�������}�X�^�[�������ꍇ
	if (sess->master)
	{
		m_pMasterSession = NULL;
		ptype_session pNewMasterSess = NULL;
		sess->master = 0;
		// �ŏ��Ɍ��������Z�b�V�����Ƀ}�X�^�[�������ڂ�
		//> �}�X�^�[�ȊO�A�S��������OK���m�F
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pSess == sess)	continue;
			pNewMasterSess = pSess;
			pNewMasterSess->master = 1;	// ������Ԃ�����
			break;
		}

		if (pNewMasterSess)
		{
			packetSize = PacketMaker::MakePacketData_RoomInfoMaster(pNewMasterSess->sess_index, TRUE, pktdata);
			if (packetSize)
				ret = AddPacketAllUser(sess, pktdata, packetSize);
			// �ύX
			if (pNewMasterSess->obj_state & OBJ_STATE_ROOM)
				m_pMasterSession = pNewMasterSess;
		}
	}

	return ret;
}
//< �ؒf����
