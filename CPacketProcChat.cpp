#include "CPacketProcChat.h"
#include "ext.h"


//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

//> ��M�p�P�b�g����
BOOL CPacketProcChat::PacketProc(BYTE *data, ptype_session sess)
{
	BOOL	ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// �p�P�b�g�f�[�^
	INT		packetSize = 0;

	// �F�؍ς݈ȊO�������Ȃ�
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;

	switch ( data[PACKET_HEADER_INDEX] )
	{
	case PK_USER_CHAT:
		// �`���b�g�p�P�b�g�쐬
		if (!(packetSize = PacketMaker::MakePacketData_UserChat(data, sess, pktdata)))
			return ret;
		// �`���b�g�w�b�_�ɂ���ăp�P�b�g���M��𕪊�
		switch ( data[PACKET_CHAT_HEADER_INDEX] )
		{
		case PK_USER_CHAT_ALL:						// �S����
			ret = AddPacketAllUser(NULL, pktdata, packetSize);
			break;
		case PK_USER_CHAT_TEAM:					// �`�[����
			ret = AddPacketTeamUser(sess->team_no, pktdata, packetSize);
			break;
		default:
//		case PK_USER_CHAT_WIS:					// �l��
			if (data[PACKET_CHAT_HEADER_INDEX] < g_nMaxLoginNum
			&& data[PACKET_CHAT_HEADER_INDEX] >= 0)
			{
				int nSendID = data[PACKET_CHAT_HEADER_INDEX];	// ���M�惆�[�U
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
//< ��M�p�P�b�g����

//> �ؒf����
BOOL CPacketProcChat::DisconnectSession(ptype_session sess)
{
	return FALSE;
}
//< �ؒf����
