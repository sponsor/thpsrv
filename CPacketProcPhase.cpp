#include "CPacketProcPhase.h"
#include "ext.h"


//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


//> public >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
//> 切断処理
BOOL CPacketProcPhase::DisconnectSession(ptype_session sess)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	// 認証済みユーザー以外は処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	// 切断したセッションがマスターだった場合
	if (sess->master)
	{
		m_pMasterSession = NULL;
		ptype_session pNewMasterSess = NULL;
		sess->master = 0;
		// 最初に見つかったセッションにマスター権限を移す
		//> マスター以外、全員が準備OKか確認
		int nSearchIndex = 0;
		for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
			pSess;
			pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
		{
			if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
			if (pSess == sess)	continue;
			pNewMasterSess = pSess;
			pNewMasterSess->master = 1;	// 準備状態を解除
			break;
		}

		if (pNewMasterSess)
		{
			packetSize = PacketMaker::MakePacketData_RoomInfoMaster(pNewMasterSess->sess_index, TRUE, pktdata);
			if (packetSize)
				ret = AddPacketAllUser(sess, pktdata, packetSize);
			// 変更
			if (pNewMasterSess->obj_state & OBJ_STATE_ROOM)
				m_pMasterSession = pNewMasterSess;
		}
	}

	return ret;
}
//< 切断処理
