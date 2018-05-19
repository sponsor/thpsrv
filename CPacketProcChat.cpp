#include "CPacketProcChat.h"
#include "ext.h"


//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//> 受信パケット処理
BOOL CPacketProcChat::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	// 認証済み以外処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	switch ( data[PACKET_HEADER_INDEX] )
	{
	case PK_USER_CHAT:
		// チャットパケット作成
		if (!(packetSize = PacketMaker::MakePacketData_UserChat(data, sess, pktdata)))
			return ret;
		// チャットヘッダによってパケット送信先を分岐
		switch ( data[PACKET_CHAT_HEADER_INDEX] )
		{
		case PK_USER_CHAT_ALL:						// 全員へ
			ret = AddPacketAllUser(NULL, pktdata, packetSize);
			break;
		case PK_USER_CHAT_TEAM:					// チームへ
			ret = AddPacketTeamUser(sess->team_no, pktdata, packetSize);
			break;
		default:
//		case PK_USER_CHAT_WIS:					// 個人へ
			if (data[PACKET_CHAT_HEADER_INDEX] < g_nMaxLoginNum
			&& data[PACKET_CHAT_HEADER_INDEX] >= 0)
			{
				int nSendID = data[PACKET_CHAT_HEADER_INDEX];	// 送信先ユーザ
				ret = AddPacket(p_pNWSess->GetSessionFromIndex(nSendID) , pktdata, packetSize);
				if (ret)
					ret = AddPacket(sess , pktdata, packetSize);
			}
			break;
		}
		break;
	default:
		break;
	}

	return ret;
}
//< 受信パケット処理

//> 切断処理
BOOL CPacketProcChat::DisconnectSession(ptype_session sess)
{
	return FALSE;
}
//< 切断処理
