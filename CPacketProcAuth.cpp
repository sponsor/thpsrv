#include "CPacketProcAuth.h"
#include "ext.h"

#if TRIAL
#include "hash.h"
#endif

WCHAR *c_arAuthResultTable[10] =
{
	L"成功",L"パスワード不一致",L"使用済みユーザ名",L"ユーザー名が不正",L"ゲーム中は入れない",L"タイムアウト",L"ハッシュ値があわない",L"ファイル送信失敗",L"ファイル送信終了",L"ファイル送信不可"
};

//> public >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
BOOL CPacketProcAuth::PacketProc(BYTE *data, ptype_session sess)
{
	E_TYPE_AUTH_RESULT auth_result = AUTH_RESULT_SUCCESS;
	BOOL ret = FALSE;
	WCHAR	msg[128];
	WCHAR	name[MAX_USER_NAME+1],pass[MAX_SRV_PASS+1];
	WORD	nEnd = PEND;
	int cLen,index,uLen;
	short	res=0;

	// 認証結果チェック
	AddMessageLog(L"CPacketProcAuth::PacketProc");

	// パケットサイズ取得
	SafeMemCopy(&res, &data[0], 2, sizeof(short));
	BYTE bUsers = 0;
	BYTE bHeader = 0;
	
	switch (data[PACKET_HEADER_INDEX])
	{
	case PK_USER_AUTH:
		index = 3;

		// ユーザ名の長さ確認
		if (data[index] > (MAX_USER_NAME*sizeof(WCHAR)) || data[index] < (MIN_USER_NAME*sizeof(WCHAR)) )
			auth_result = AUTH_RESULT_INVALID_USER_NAME;
		else
		{
			// User Name
			uLen = DecodeSizeStrFromRaw( &name[0], (BYTE*)&data[index], MAX_USER_NAME*sizeof(WCHAR)+1, res-index);

			if (p_pNWSess->IsUniqueUserName(name))
			{
				index += uLen+1;	// 文字列+文字サイズ情報バイト(1バイト)
				// Server Pass
				cLen = DecodeSizeStrFromRaw( &pass[0], (BYTE*)&data[index], MAX_SRV_PASS*sizeof(WCHAR)+1, res-index);

				// サーバパスワードと比較
				ret = (wcscmp(pass, g_pSrvPassWord) == 0);
				if (ret)
				{
					// 認証成功した場合、サーバはゲーム中か確認
					if (g_eGamePhase == GAME_PHASE_ROOM)
					{
						switch (GetRoomPhase())
						{
						case GAME_PHASE_ROOM:
							auth_result = AUTH_RESULT_SUCCESS;		// 認証成功
							break;
						case GAME_PHASE_LOAD:
							auth_result = AUTH_RESULT_GAME_LOAD;
							break;
						case GAME_PHASE_MAIN:
							auth_result = AUTH_RESULT_GAME_PHASE;
							break;
						default:
							auth_result = AUTH_RESULT_GAME_INVALID;
							break;
						}
					}
					else
						auth_result = AUTH_RESULT_GAME_PHASE;
				}
				else	auth_result = AUTH_RESULT_INVALID_PWD;	// 認証失敗
			}
			else
			{
				// 既にあるユーザ名
				auth_result = AUTH_RESULT_NO_UNIQUE_USER;
			}
		}

		// 認証結果チェック
		SafePrintf(msg, 128, L"ユーザー認証中結果:%s - OBJNo[%d]/PWD:[%s]/NAME:[%s]",c_arAuthResultTable[(int)auth_result],  sess->sess_index, pass, name);
		AddMessageLog(&msg[0]);

#if SCR_HASH_CHECK
		// 認証成功なら名前をとりあえず覚えておく
		if (auth_result == AUTH_RESULT_SUCCESS)
		{
			sess->frame_count = 0;
			SafeMemCopy(sess->name, name, uLen, MAX_USER_NAME*sizeof(WCHAR));
			sess->name_len = uLen;
			sess->extdata1 = 0;
			sess->extdata2 = 0;
			sess->game_ready = 1;

//			CheckNextHash(sess);
			// 認証結果パケット作成
			BYTE	pktdata[MAX_PACKET_SIZE];	// パケットデータ
			INT		packetSize = 0;
			if ((packetSize = PacketMaker::MakePacketData_ReqHashList(sess, pktdata)) == 0)
				return FALSE;
			// 認証結果パケット作成
			ret = AddPacket(sess, pktdata, packetSize);
		}
		else
		{
			BYTE	pktdata[MAX_PACKET_SIZE];	// パケットデータ
			INT		packetSize = 0;
			// 認証結果パケット作成
			if ((packetSize = PacketMaker::MakePacketData_Authentication(sess, pktdata, auth_result)) == 0)
				return FALSE;
			// 認証結果パケット作成
			ret = AddPacket(sess, pktdata, packetSize);
			sess->extdata1 = 0;
			sess->extdata2 = 0;
			sess->game_ready = 0;
		}
#else
		{
			BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
			INT		packetSize = 0;
			// 認証成功なら// ルームイン
			if (auth_result == AUTH_RESULT_SUCCESS)
			{
				sess->connect_state = CONN_STATE_AUTHED;
				sess->obj_state = (E_TYPE_OBJ_STATE)OBJ_STATE_ROOM;
				SafeMemCopy(sess->name, name, uLen, MAX_USER_NAME*sizeof(WCHAR));
				sess->name_len = uLen;
				InitSession(sess);
			}

			// 認証結果パケット作成
			if ((packetSize = PacketMaker::MakePacketData_Authentication(sess, pktdata, auth_result)) == 0)
				return FALSE;

			// 認証結果パケット作成
			ret = AddPacket(sess, pktdata, packetSize);
		}
#endif
///////////////////////////////////////////////////////		
		break;
	case PK_CHK_FILE_HASH:
		ret = PacketProcRetHashCheck(sess, data);
		break;
	case PK_RET_HASH:
//		ret = PacketProcRetHash(sess, data);
		ret = PacketProcRetHashCheck(sess, data);
		break;
	case PK_CMD_CONFIRM:
		ret = PacketProcConfirm(sess, data);
		break;
	case PK_REQ_FILE_HASH:
		ret = PacketReqFileHash(sess, data);
		break;
	case PK_REQ_FILE_DATA:
		ret = PacketReqFileData(sess, data);
		break;
	default:
		return FALSE;
	}
	
	if (ret)
		UpdateWMCopyData();
	return ret;
}

void CPacketProcAuth::InitSession(ptype_session sess)
{
	bool bMaster = true;
	g_pCriticalSection->EnterCriticalSection_Session(L'6');
	int nSearchIndex = 0;
	for(ptype_session pSess=p_pNWSess->GetSessionFirst(&nSearchIndex);
		pSess;
		pSess=p_pNWSess->GetSessionNext(&nSearchIndex))
	{
		// 認証済みか
		if (pSess->connect_state != CONN_STATE_AUTHED)	continue;
		if (pSess == sess)	continue;						// 飛ばすセッションか
		bMaster = false;
	}
	sess->master = bMaster;

	sess->lx = WIN_WIDTH/2;
	sess->ly = ROOM_CHARA_BASE_MAX_MOVE_H/2;
	sess->ax = 0;
	sess->ay = 0;
	sess->vx = 0;
	sess->vy = 0;
	sess->dir = USER_DIRECTION_RIGHT;
	sess->angle = CHARA_ANGLE_RIGHT;
	sess->chara_type = g_nDefaultCharaID;
	sess->obj_type = OBJ_TYPE_CHARA;
	
	g_pCriticalSection->LeaveCriticalSection_Session();
}

BOOL CPacketProcAuth::DisconnectSession(ptype_session sess)
{
	BOOL ret = FALSE;
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT		packetSize = 0;

	// 認証済み以外は処理しない
	if (sess->connect_state != CONN_STATE_AUTHED)
		return ret;
	// 切断を周知
	packetSize = PacketMaker::MakePacketData_UserDiconnect(sess, pktdata);
	// 切断パケットを全ユーザに送信
	if (packetSize)
		ret = AddPacketAllUser(sess, pktdata, packetSize);
	return ret;
}

//> private >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>

BOOL CPacketProcAuth::PacketProcConfirm(ptype_session sess, BYTE* data)
{
	BYTE		pktdata[MAX_PACKET_SIZE];	// パケットデータ
	INT packetSize = 0;
	int index = 3;
	
	if ( (data[index] != sess->sess_index)
		|| (sess->obj_state != OBJ_STATE_RESULT_CONFIRMING))
		return DisconnectSession(sess);

	sess->obj_state = OBJ_STATE_ROOM_READY;
	packetSize = PacketMaker::MakePacketData_Confirmed(sess, pktdata);
	return AddPacket(sess, pktdata, packetSize);
}

BOOL CPacketProcAuth::PacketProcRetHashCheck(ptype_session sess, BYTE* data)
{
	WCHAR msg[128];
	BYTE hash[MD5_LENGTH+1];
	memset(hash, 0, sizeof(BYTE)*(MD5_LENGTH+1));
	
	BYTE pkt[MAX_PACKET_SIZE];
	INT pktsize = 0;
	int index = 3;
	BOOL ret = FALSE;
	WORD wCharaCount = 0;
	memcpy(&wCharaCount, &data[index], sizeof(WORD));
	index += sizeof(WORD);
	WORD wScrID = 0;
	std::map < int, std::string > mapCharaHash;
	std::map < int, std::string > mapStageHash;

	for (int i=0;i<wCharaCount;++i)
	{
		memcpy(&wScrID, &data[index], sizeof(WORD));
		index += sizeof(WORD);

		memcpy(&hash, &data[index], 16);
		index += 16;
		char md5[MD5_LENGTH+1];
		memset(md5, 0, sizeof(char)*(MD5_LENGTH	+ 1));
	    for (int i=0; i<16; i++) {
		    sprintf(&md5[i*2], "%02X", hash[i]);
			md5[i*2] = tolower(md5[i*2]);
			md5[i*2+1] = tolower(md5[i*2+1]);
	    }
		mapCharaHash.insert(std::map< int , std::string>::value_type((int)wScrID, std::string(md5)));
	}

	WORD wStageCount = 0;
	memcpy(&wStageCount, &data[index], sizeof(WORD));
	index += sizeof(WORD);
	for (int i=0;i<wStageCount;++i)
	{
		memcpy(&wScrID, &data[index], sizeof(WORD));
		index += sizeof(WORD);
		memcpy(&hash, &data[index], 16);
		index += 16;
		char md5[MD5_LENGTH+1];
		memset(md5, 0, sizeof(char)*(MD5_LENGTH	+ 1));
	    for (int i=0; i<16; i++) {
		    sprintf(&md5[i*2], "%02X", hash[i]);
			md5[i*2] = tolower(md5[i*2]);
			md5[i*2+1] = tolower(md5[i*2+1]);
	    }
		mapStageHash.insert(std::map< int , std::string>::value_type((int)wScrID, std::string(md5)));
	}
	
	if (mapCharaHash.size() < g_mapCharaScrInfo.size())
	{
		pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_SCR_CHARA_COUNT);
		ret = AddPacket(sess, pkt, pktsize);
		return ret;
	}
	if (mapStageHash.size() < g_mapStageScrInfo.size())
	{
		pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_SCR_STAGE_COUNT);
		ret = AddPacket(sess, pkt, pktsize);
		return ret;
	}
	for (std::map < int, TCHARA_SCR_INFO >::iterator it = g_mapCharaScrInfo.begin();
		it != g_mapCharaScrInfo.end();
		++it)
	{
		std::map < int, std::string >::iterator itFind = mapCharaHash.find((*it).second.ID);
		if (itFind == mapCharaHash.end())
		{
			// 認証結果パケット作成
			SafePrintf(msg, 128, L"ユーザー認証の結果:スクリプト不足 - OBJNo[%d]", sess->sess_index);
			AddMessageLog(&msg[0]);
			pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_SCR_CHARA_NOT_FOUND,1, (*it).second.ID);
			ret = AddPacket(sess, pkt, pktsize);
			return ret;
		}
		// ハッシュ不一致
		if ( (*itFind).second.compare((*it).second.md5) != 0)
		{
			// 認証結果パケット作成
			SafePrintf(msg, 128, L"ユーザー認証の結果:ハッシュ値不一致 - OBJNo[%d]", sess->sess_index);
			AddMessageLog(&msg[0]);
			pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_HASH,1, (*it).second.ID);
			ret = AddPacket(sess, pkt, pktsize);
			return ret;
		}
	}
	for (std::map < int, TSTAGE_SCR_INFO >::iterator it = g_mapStageScrInfo.begin();
		it != g_mapStageScrInfo.end();
		++it)
	{
		std::map < int, std::string >::iterator itFind = mapStageHash.find((*it).second.ID);
		if (itFind == mapStageHash.end())
		{
			// 認証結果パケット作成
			SafePrintf(msg, 128, L"ユーザー認証の結果:スクリプト不足 - OBJNo[%d]", sess->sess_index);
			AddMessageLog(&msg[0]);
			pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_SCR_STAGE_NOT_FOUND,0, (*it).second.ID);
			ret = AddPacket(sess, pkt, pktsize);
			return ret;
		}
		// ハッシュ不一致
		if ( (*itFind).second.compare((*it).second.md5) != 0)
		{
			// 認証結果パケット作成
			SafePrintf(msg, 128, L"ユーザー認証の結果:ハッシュ値不一致 - OBJNo[%d]", sess->sess_index);
			AddMessageLog(&msg[0]);
			pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_HASH,0, (*it).second.ID);
			ret = AddPacket(sess, pkt, pktsize);
			return ret;
		}
	}
	// ハッシュ一致

	SafePrintf(msg, 128, L"ユーザー認証の結果:認証成功- OBJNo[%d]", sess->sess_index);
	AddMessageLog(&msg[0]);
	// ハッシュ値チェック終了
	InitSession(sess);
	sess->frame_count = 0;
	sess->extdata1 = 0;
	sess->extdata2 = 0;
	sess->game_ready = 0;
	sess->obj_state = (E_TYPE_OBJ_STATE)OBJ_STATE_ROOM;
	sess->connect_state = CONN_STATE_AUTHED;
	// 認証結果パケット作成
	if ((pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_SUCCESS)) == 0)
		return FALSE;
	ret = AddPacket(sess, pkt, pktsize);
	return ret;
}

BOOL CPacketProcAuth::PacketProcRetHash(ptype_session sess, BYTE* data)
{
	WCHAR msg[128];
	BYTE pkt[MAX_PACKET_SIZE];
	INT pktsize = 0;
	BOOL ret = FALSE;
	std::string srv_hash;

	int hash_group = data[3];
	WORD hash_id = 0;
	memcpy(&hash_id, &data[4], sizeof(WORD));
	BYTE hash_size = data[6];
	char cli_hash[128+1];
	ZeroMemory(cli_hash, sizeof(char)*129);
	SafeMemCopy(cli_hash, &data[7], sizeof(char)*hash_size, sizeof(char)*129);

	// サーバハッシュ値検索
	if (hash_group == 0)
	{
		std::map < int, TSTAGE_SCR_INFO >::iterator itfind = g_mapStageScrInfo.find(hash_id);
		if (itfind == g_mapStageScrInfo.end())
		{
			// 認証結果パケット作成
			pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_HASH);
			ret = AddPacket(sess, pkt, pktsize);
			return ret;
		}
#if	TRIAL
		switch (hash_id)
		{
		case 0:	srv_hash = HASH_STAGE00;	break;
		case 1:	srv_hash = HASH_STAGE01;	break;
		case 2:	srv_hash = HASH_STAGE02;	break;
		case 3:	srv_hash = HASH_STAGE03;	break;
		case 4:	srv_hash = HASH_STAGE04;	break;
		case 5:	srv_hash = HASH_STAGE05;	break;
		case 6:	srv_hash = HASH_STAGE06;	break;
		case 7:	srv_hash = HASH_STAGE07;	break;
#if	STAGE_TEST
		case 8:	srv_hash = HASH_STAGE08;	break;
#endif
		default:
			// ハッシュ値不一致
			if (srv_hash.compare(cli_hash) != 0)
			{
				// 認証結果パケット作成
				SafePrintf(msg, 128, L"ユーザー認証の結果:ハッシュ値不一致 - OBJNo[%d]", sess->sess_index);
				AddMessageLog(&msg[0]);
				pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_HASH, hash_group, hash_id);
				ret = AddPacket(sess, pkt, pktsize);
				return ret;
			}
		}
#else
		srv_hash = (*itfind).second.md5;
#endif
	}
	else
	{
		std::map < int, TCHARA_SCR_INFO >::iterator itfind = g_mapCharaScrInfo.find(hash_id);
		if (itfind == g_mapCharaScrInfo.end())
		{
			// 認証結果パケット作成
			pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_HASH);
			ret = AddPacket(sess, pkt, pktsize);
			return ret;
		}
#if	TRIAL
		switch (hash_id)
		{
		case 0:	srv_hash = HASH_CHARA00;	break;
		case 1:	srv_hash = HASH_CHARA01;	break;
		case 2:	srv_hash = HASH_CHARA02;	break;
		case 4:	srv_hash = HASH_CHARA04;	break;
#if	CHARA_TEST
//		case 3:	srv_hash = HASH_CHARA03;	break;
//		case 5:	srv_hash = HASH_CHARA05;	break;
//		case 7:	srv_hash = HASH_CHARA07;	break;
		case 6:	srv_hash = HASH_CHARA06;	break;
		case 9:	srv_hash = HASH_CHARA09;	break;
		case 8:	srv_hash = HASH_CHARA08;	break;
#endif
		default:
			// ハッシュ値不一致
			if (srv_hash.compare(cli_hash) != 0)
			{
				// 認証結果パケット作成
				SafePrintf(msg, 128, L"ユーザー認証の結果:ハッシュ値不一致 - OBJNo[%d]", sess->sess_index);
				AddMessageLog(&msg[0]);
				pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_HASH, hash_group, hash_id);
				ret = AddPacket(sess, pkt, pktsize);
				return ret;
			}
		}
#else
		srv_hash = (*itfind).second.md5;
#endif
	}

	// ハッシュ値不一致
	if (srv_hash.compare(cli_hash) != 0)
	{
		// 認証結果パケット作成
		SafePrintf(msg, 128, L"ユーザー認証の結果:ハッシュ値不一致 - OBJNo[%d]", sess->sess_index);
		AddMessageLog(&msg[0]);
		pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_INVALID_HASH, hash_group, hash_id);
		ret = AddPacket(sess, pkt, pktsize);
		return ret;
	}

	if (hash_group == 0)
		sess->extdata1++;
	else
		sess->extdata2++;

	// ハッシュ値確認
	if (CheckNextHash(sess))
	{
		SafePrintf(msg, 128, L"ユーザー認証の結果:認証成功- OBJNo[%d]", sess->sess_index);
		AddMessageLog(&msg[0]);
		// ハッシュ値チェック終了
		sess->frame_count = 0;
		sess->extdata1 = 0;
		sess->extdata2 = 0;
		if (sess->game_ready)
		{
			sess->game_ready = 0;
			sess->obj_state = (E_TYPE_OBJ_STATE)OBJ_STATE_ROOM;
			sess->connect_state = CONN_STATE_AUTHED;
			InitSession(sess);
			// 認証結果パケット作成
			if ((pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_SUCCESS)) == 0)
				return FALSE;
		}
		else
		{
			// 認証結果パケット作成
			if ((pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_FILESEND_END)) == 0)
				return FALSE;
		}
		// 認証結果パケット作成
		ret = AddPacket(sess, pkt, pktsize);
	}
	return TRUE;
}

BOOL CPacketProcAuth::CheckNextHash(ptype_session sess)
{
	BYTE pkt[MAX_PACKET_SIZE];
	INT pktsize = 0;
	int hash_group = 0;
	int hash_id = 0;

	if (sess->extdata1 >= g_mapStageScrInfo.size())
	{
		if (sess->extdata2 >= g_mapCharaScrInfo.size())
			return TRUE;
		else
		{
			hash_group = 1;
			std::map < int, TCHARA_SCR_INFO >::iterator it = g_mapCharaScrInfo.begin();
			for (int i=0;
				it != g_mapCharaScrInfo.end();
				it++,i++)
			{
				if (i>=(int)sess->extdata2)
					break;
			}
			hash_id = (*it).second.ID;
		}
	}
	else
	{
		hash_group = 0;
		std::map < int, TSTAGE_SCR_INFO >::iterator it = g_mapStageScrInfo.begin();
		for (int i=0;
			it != g_mapStageScrInfo.end();
			it++,i++)
		{
			if (i>=(int)sess->extdata1)
				break;
		}
		hash_id = (*it).second.ID;
	}
	pktsize = PacketMaker::MakePacketData_AuthReqHash(hash_group, hash_id, pkt);
	// 認証結果パケット作成
	AddPacket(sess, pkt, pktsize);

	return FALSE;
}

BOOL CPacketProcAuth::PacketReqFileHash(ptype_session sess, BYTE* data)
{
	sess->game_ready = 0;
	BOOL ret = TRUE;
	BYTE pkt[MAX_PACKET_SIZE];
	INT pktsize = 0;
	WORD id = 0;
	int nIndex = 3;
	BOOL bScrChara = data[nIndex];
	nIndex++;
	memcpy(&id, &data[nIndex], sizeof(WORD));
	nIndex += sizeof(WORD);
	BYTE fileno = data[nIndex];
	nIndex++;
	WCHAR hash_path[_MAX_PATH*2+1];
	WCHAR* phash_path = NULL;
	char md5[MD5_LENGTH+1];
	md5[MD5_LENGTH] = NULL;
	int retHash = 0;
	g_pCriticalSection->EnterCriticalSection_Session(L'N');
	if (bScrChara)
		retHash = common::scr::GetCharaFileHash(g_pLuah, id, fileno, md5, hash_path, &g_mapCharaScrInfo, g_pCriticalSection);
	else
		retHash = common::scr::GetStageFileHash(g_pLuah, id, fileno, md5, hash_path, &g_mapStageScrInfo, g_pCriticalSection);
	g_pCriticalSection->LeaveCriticalSection_Session();

	switch (retHash)
	{
	case -2:
		pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_FILESEND_DISABLE);
		return AddPacket(sess, pkt, pktsize);
	case -1:
		if (g_bLogFile)
		{
			WCHAR log[32];
			SafePrintf(log, 32, L"ERROR:FILESEND_ERROR(FileNO:%d)",  fileno);
			AddMessageLog(log);
		}
		pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_FILESEND_ERROR);
		return AddPacket(sess, pkt, pktsize);
	case 0:	// 送信終了
		pktsize = PacketMaker::MakePacket_FileInfoSendClose(bScrChara, id, pkt);
		ret = AddPacket(sess, pkt, pktsize);

		if (bScrChara)
			sess->extdata2++;
		else
			sess->extdata1++;
		if (CheckNextHash(sess))
		{
			pktsize = PacketMaker::MakePacketData_Authentication(sess, pkt, AUTH_RESULT_FILESEND_END);
			ret |= AddPacket(sess, pkt, pktsize);
		}
		break;
	case 1:
		OutputDebugStr(hash_path);
		OutputDebugStr(L"\n");
		pktsize = PacketMaker::MakePacket_FileInfoSendHash(bScrChara, id, fileno, md5, hash_path, pkt);
		return AddPacket(sess, pkt, pktsize);
	}

	return ret;
}

BOOL CPacketProcAuth::PacketSendFileData(ptype_session sess, WCHAR* path, BOOL bCharaScrInfo, int id, int nFileNo, int nDatIndex)
{
	FILE* pf = NULL;
	BYTE pkt[MAX_PACKET_SIZE];
	INT pktsize = 0;
	BOOL bEof = FALSE;
	g_pCriticalSection->EnterCriticalSection_Session(L'M');
	while ((pf = _wfopen(path, L"rb")) == NULL)
		continue;

	BYTE buffer[MAX_SEND_BUFFER_SIZE];
	int nMaxReadSize = MAX_SEND_BUFFER_SIZE;

	// 範囲読み込み
	int nRead = 0;
	fseek(pf, nDatIndex, SEEK_SET);
	if ((nRead = fread(buffer, 1,nMaxReadSize, pf)) < nMaxReadSize)
	{
		if (feof(pf) == 0)
		{
			g_pCriticalSection->LeaveCriticalSection_Session();
			return FALSE;
		}
		else
			bEof = TRUE;
	}
	fclose(pf);
	g_pCriticalSection->LeaveCriticalSection_Session();

	pktsize = PacketMaker::MakePacket_FileInfoSendData(bCharaScrInfo, id, nFileNo, bEof, nDatIndex, nRead, buffer, pkt);
	if (pktsize)
		return AddPacket(sess, pkt, pktsize);
	return TRUE;
}

BOOL CPacketProcAuth::PacketReqFileData(ptype_session sess, BYTE* data)
{
	BOOL ret = FALSE;
	WORD id = 0;
	int nIndex = 3;
	BOOL bCharaScr = data[nIndex];
	nIndex++;
	memcpy(&id, &data[nIndex], sizeof(WORD));
	nIndex += sizeof(WORD);
	BYTE fileno = data[nIndex];
	nIndex++;
	int nDatIndex;
	memcpy(&nDatIndex, &data[nIndex], sizeof(int));
//	nIndex += sizeof(DWORD);
	WCHAR path[_MAX_PATH*2+1];
	// キャラスクリプト
	if (bCharaScr)
		ret = common::scr::GetCharaFilePath(path, g_pLuah, id, fileno, &g_mapCharaScrInfo, g_pCriticalSection);
	else
		ret = common::scr::GetStageFilePath(path, g_pLuah, id, fileno, &g_mapStageScrInfo, g_pCriticalSection);

	if (ret)
	{
		// ファイルのデータ送信
		if (!PacketSendFileData(sess,path, bCharaScr, id, fileno, nDatIndex))
			return DisconnectSession(sess);
	}
	return TRUE;
}

