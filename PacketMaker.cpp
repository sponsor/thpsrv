#include "PacketMaker.h"
#include "ext.h"
#include "common.h"

INT PacketMaker::MakePacketData_SYN(BYTE* msg)
{
	WORD datIndex = 2;		// 追加するデータの位置
	if (!msg)	return 0;

	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_SYN);
	// 1
	datIndex += SetByteData(&msg[datIndex], 1);
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
	return datIndex;
}

INT PacketMaker::AddEndMarker(int datIndex, BYTE* msg)
{
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
	return datIndex;
}

//> パケットヘッダ作成
// header1			: 1バイト目ヘッダ
// header2			: 2バイト目ヘッダ
// info_count		: 以下のオブジェクトの情報数(0の場合追加しない)
// msg			: out パケット
INT PacketMaker::MakePacketHeadData(BYTE header1, BYTE header2, int info_count , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], header1);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], header2);
	// 情報数
	if (info_count)
		datIndex += SetByteData(&msg[datIndex], (BYTE)info_count);
	return datIndex;
}
//< パケットヘッダ作成

//> 認証結果パケット 
INT PacketMaker::MakePacketData_Authentication(ptype_session sess, BYTE* msg, E_TYPE_AUTH_RESULT auth,int group, int id)
{
	int datIndex = 2;
	if (!sess || !msg)
		return 0;

	datIndex += SetByteData(&msg[datIndex], PK_USER_AUTH);
	// 認証結果
	datIndex += SetByteData(&msg[datIndex], (BYTE)auth);
	// 認証成功の場合、情報を追加
	if (auth == AUTH_RESULT_SUCCESS)
	{
		// ユーザNo
		datIndex += SetByteData(&msg[datIndex], sess->sess_index);
		// 状態(ゲーム準備状態など)
		datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
		// マスター
		datIndex += SetByteData(&msg[datIndex], sess->master);
		// チームNo
		datIndex += SetByteData(&msg[datIndex], sess->team_no);
		// 人数
		datIndex += SetByteData(&msg[datIndex], (BYTE)g_nMaxLoginNum);
//		// ユーザー数
//		datIndex += SetByteData(&msg[datIndex], (BYTE)authed_user_count);
	}
	else if (auth == AUTH_RESULT_INVALID_HASH)
	{
		// 認証結果AUTH_RESULT_INVALID_HASH
		datIndex += SetByteData(&msg[datIndex], (BYTE)group);
		// 認証結果AUTH_RESULT_INVALID_HASH
		datIndex += SetWordData(&msg[datIndex], (WORD)id);
	}

	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< 認証結果パケット

INT PacketMaker::MakePacketData_ReqHashList(ptype_session sess, BYTE* msg)
{
	int datIndex = 2;
	if (!sess || !msg)
		return 0;

	datIndex += SetByteData(&msg[datIndex], PK_REQ_HASH);
	int nCharaScrCount = g_mapCharaScrInfo.size();
	datIndex += SetWordData(&msg[datIndex], (WORD)nCharaScrCount);
	for (std::map < int, TCHARA_SCR_INFO >::iterator it = g_mapCharaScrInfo.begin();
		it != g_mapCharaScrInfo.end();
		++it)
	{
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it).second.ID);
	}
	int nStageScrCount = g_mapStageScrInfo.size();
	datIndex += SetWordData(&msg[datIndex], (WORD)nStageScrCount);
	for (std::map < int, TSTAGE_SCR_INFO >::iterator it = g_mapStageScrInfo.begin();
		it != g_mapStageScrInfo.end();
		++it)
	{
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it).second.ID);
	}

	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
	return datIndex;
}

//> ユーザー情報パケット
INT PacketMaker::MakePacketData_RoomInfoIn(ptype_session sess,BYTE* msg)
{
	WORD datIndex = 2;		// 追加するデータの位置
	if (!msg || !sess)
		return 0;

	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_NEW);
	// オブジェクトNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// キャラタイプ
	datIndex += SetByteData(&msg[datIndex], sess->chara_type );
	// キャラ名長
	datIndex += SetByteData(&msg[datIndex], sess->name_len);
	// キャラ名
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	// 状態(ゲーム準備状態など)
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
	// マスター
	datIndex += SetByteData(&msg[datIndex], sess->master);
	// チームNo
	datIndex += SetByteData(&msg[datIndex], sess->team_no);
	// 準備状態
	datIndex += SetByteData(&msg[datIndex], sess->game_ready);

	// 座標値X
	datIndex += SetShortData(&msg[datIndex], sess->lx);
	// 座標値Y
	datIndex += SetShortData(&msg[datIndex], sess->ly);
	// 移動値X
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// 移動値Y
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// 方向
	datIndex += SetByteData(&msg[datIndex], sess->dir);

	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);
#ifdef _DEBUG
	if ( datIndex > MAX_PACKET_SIZE)
	{
		AddMessageLog(L"PacketMaker::MakePacket_AddObj_CellObjectsパケットサイズ超過");
		return 0;
	}
#endif
	return datIndex;
}
//< ユーザー情報パケット

// 入室パケットヘッダ作成(パケットヘッダ、ルーム情報ヘッダ、ユーザ数)
INT PacketMaker::MakePacketData_RoomInfoInHeader(BYTE nUserCount, BYTE* msg)
{
	WORD datIndex = 2;		// 追加するデータの位置
	if (!nUserCount || !msg)
		return 0;

	// header			:1
	// roomheader	:1
	// chara_count	:1

	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_IN);
	// ユーザ数
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], nUserCount);

	return datIndex;
}

//> 入室パケット1ユーザ情報(キャラタイプなど…)
//> msg	: 情報を追加するパケットへの先頭ポインタ
INT PacketMaker::MakePacketData_RoomInfoInChara(ptype_session sess, BYTE* msg)
{
	WORD datIndex = 0;		// 追加するデータの位置
	if (!msg || !sess)
		return 0;
	// chara_type...	:

	// オブジェクトNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// キャラタイプ
	datIndex += SetByteData(&msg[datIndex], sess->chara_type );
	// キャラ名長
	datIndex += SetByteData(&msg[datIndex], sess->name_len);
	// キャラ名
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	// 状態(ゲーム準備状態など)
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
	// マスター
	datIndex += SetByteData(&msg[datIndex], sess->master);
	// チームNo
	datIndex += SetByteData(&msg[datIndex], sess->team_no);
	// 準備状態
	datIndex += SetByteData(&msg[datIndex], sess->game_ready);

	// 座標値X
	datIndex += SetShortData(&msg[datIndex], sess->lx);
	// 座標値Y
	datIndex += SetShortData(&msg[datIndex], sess->ly);
	// 移動値X
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// 移動値Y
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// 方向
	datIndex += SetByteData(&msg[datIndex], sess->dir);

#ifdef _DEBUG
	if ( datIndex > MAX_PACKET_SIZE)
	{
		AddMessageLog(L"PacketMaker::MakePacket_AddObj_CellObjectsパケットサイズ超過");
		return 0;
	}
#endif
	return datIndex;
}
//< 入室パケット1ユーザ情報(キャラタイプなど…)
//< msg	: 情報を追加するパケットへの先頭ポインタ

//> チャットパケット
INT PacketMaker::MakePacketData_UserChat(BYTE *in_msg, ptype_session sess,BYTE *msg)
{
	int		datIndex = 2;
	if (!in_msg || !msg || !sess)
		return 0;
	//< IN
	// size			:	2
	// header		:	1
	//	chat_type	:	1
	//	msg_len	:	1
	//	msg			:	msg_len
	//> OUT
	// size			:	2
	// header		:	1
	// obj_no		:	1
	//	name_len	:	1
	//	name		:	name_len
	// user_no		:	1
	//	msg_len	:	1
	//	msg			:	msg_len

	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_CHAT);
	// チャットヘッダ
	datIndex += SetByteData(&msg[datIndex], in_msg[datIndex]);

	// ユーザNo (sess == NULLはサーバメッセージ)
	if (sess == NULL)
		datIndex += SetByteData(&msg[datIndex], g_nMaxLoginNum);
	else
		datIndex += SetByteData(&msg[datIndex], sess->sess_index);

	// NULLの場合はサーバメッセージ作成
	if (sess != NULL)
	{	// ユーザ名を取得
		// キャラ名長
		datIndex += SetByteData(&msg[datIndex], sess->name_len);
		// キャラ名
		datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	}

	// メッセージ文字列長
	BYTE	bytMessageLen = min(MAX_CHAT_MSG*sizeof(WCHAR), in_msg[4]);
	if (!bytMessageLen)
		return 0;
	datIndex += SetByteData(&msg[datIndex], bytMessageLen);
	// メッセージ
	datIndex += SetMultiByteData(&msg[datIndex], &in_msg[5], bytMessageLen, MAX_CHAT_MSG*sizeof(WCHAR));

	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

#ifdef _DEBUG
	WCHAR	logmsg[MAX_PACKET_SIZE];
	WCHAR	chatmsg[MAX_CHAT_MSG+1];
	WCHAR	username[MAX_USER_NAME+1];
	ZeroMemory(chatmsg, MAX_CHAT_MSG+2);
	ZeroMemory(username, MAX_USER_NAME+2);
	ZeroMemory(logmsg, sizeof(logmsg));
	// キャラ名
	SetMultiByteData((BYTE*)&username[0], (BYTE*)sess->name, sess->name_len, MAX_USER_NAME*sizeof(WCHAR));
	username[sess->name_len/sizeof(WCHAR)] = NULL;
	// Message
//	_tcsncpy_s(chatmsg, bytMessageLen/sizeof(WCHAR), (WCHAR*)(&in_msg[5]), MAX_CHAT_MSG);
	SetMultiByteData((BYTE*)&chatmsg[0], &in_msg[5], bytMessageLen, MAX_CHAT_MSG*sizeof(WCHAR));
	chatmsg[bytMessageLen/sizeof(WCHAR)] = NULL;
	SafePrintf(logmsg, MAX_PACKET_SIZE, L"PK_USER_CHAT:%s:%s", username, chatmsg);
	AddMessageLog(logmsg, FALSE);
#endif

	return datIndex;
}
//< チャットパケット

//> パケットフッタ作成(エンドマーカ、パケットサイズ)
// wSize: フッタも含めないサイズ
// msg	: パケットへのポインタ
INT PacketMaker::MakePacketData_SetFooter(WORD wSize, BYTE* msg)
{
	int datIndex = wSize;
	// エンドマーカ
	datIndex += SetEndMarker(&msg[wSize]);
	SetWordData(&msg[0], datIndex);
	return 2;
}
//< パケットフッタ作成(エンドマーカ、パケットサイズ)
// ルームマスター情報パケット作成
INT PacketMaker::MakePacketData_RoomInfoMaster(int sess_index, BYTE flag, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_MASTER);
	// マスター情報を変更するユーザNo
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess_index);
	// マスター情報を変更値
	datIndex += SetByteData(&msg[datIndex], flag);
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> キャラ移動情報パケット作成
// sess			: ユーザ
// msg			: out パケット
INT PacketMaker::MakePacketData_RoomInfoRoomCharaMove(ptype_session sess , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_MOVE);
	// ユーザNo
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->sess_index);
	// 座標X
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->lx);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->ly);
	// 移動値X
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->vx);
	// 移動値Y
	datIndex += SetShortData(&msg[datIndex], (WORD)sess->vy);
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< キャラ移動情報パケット作成

INT PacketMaker::MakePacketData_RoomInfoGameReady(BYTE sess_index, BYTE game_ready, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_READY);
	// ユーザNo
	datIndex += SetByteData(&msg[datIndex], sess_index);
	// ゲーム状態変更
	datIndex += SetByteData(&msg[datIndex], game_ready);
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> キャラ選択パケット作成
// obj_no		: ユーザNo
// chara_no	: 選択キャラNo
// msg			: out パケット
INT PacketMaker::MakePacketData_RoomInfoCharaSelect(BYTE sess_index, BYTE chara_no, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_CHARA_SEL);
	// ユーザNo
	datIndex += SetByteData(&msg[datIndex], sess_index);
	// キャラNo
	datIndex += SetByteData(&msg[datIndex], chara_no);
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< キャラ選択パケット作成

//> 再入室パケット作成
INT PacketMaker::MakePacketData_RoomInfoReEnter(ptype_session sess, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RE_ENTER);
	// ユーザNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// 座標X
	datIndex += SetShortData(&msg[datIndex], sess->lx);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], sess->ly);
	// 移動値X
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// 移動値Y
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// 向き
	datIndex += SetByteData(&msg[datIndex], sess->dir);
	// master
	datIndex += SetByteData(&msg[datIndex], sess->master);
	// obj state
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< 再入室パケット作成

//> 切断パケット
// sess	: 切断するセッション
// msg	: out作成パケット
INT PacketMaker::MakePacketData_UserDiconnect(ptype_session sess, BYTE *msg)
{
	int		datIndex = 2;
	if (!msg || !sess)
		return 0;
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_DISCON);
	// 切断セッションのユーザNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index );	
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< 切断パケット


//> アイテム選択パケット作成
// item_index: アイテムインデックス
// item_flg		: アイテムフラグ
// msg			: out パケット
INT PacketMaker::MakePacketData_RoomInfoItemSelect(int index, DWORD item_flg, BYTE cost, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)
		return 0;
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_ITEM_SEL);
	// アイテムインデックス
	datIndex += SetByteData(&msg[datIndex], (BYTE)index );	
	// アイテムフラグ
	datIndex += SetDwordData(&msg[datIndex], item_flg );
	// 残りコスト
	datIndex += SetByteData(&msg[datIndex], cost );
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> アイテム選択パケット作成
// item_index: アイテムインデックス
// item_flg		: アイテムフラグ
// msg			: out パケット
INT PacketMaker::MakePacketData_MainInfoItemSelect(int index, DWORD item_flg, BYTE* msg, BOOL steal)
{
	int		datIndex = 2;
	if (!msg)
		return 0;

	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_ITEM_USE);
	// アイテムインデックス
	datIndex += SetByteData(&msg[datIndex], (BYTE)index );	
	// アイテムフラグ
	datIndex += SetDwordData(&msg[datIndex], item_flg );	
	// アイテムインデックス
	datIndex += SetByteData(&msg[datIndex], (BYTE)steal );	
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}


// チーム数パケット作成
// team_count: チーム数
// msg			: out パケット
INT PacketMaker::MakePacketData_RoomInfoTeamCount(int team_count, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_TEAM_COUNT);
	// アイテムインデックス
	datIndex += SetByteData(&msg[datIndex], (BYTE)team_count );	
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}

//> ルール変更パケット作成
// rule_flg: チーム数
// msg			: out パケット
INT PacketMaker::MakePacketData_RoomInfoRule(BYTE rule_flg, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RULE);
	// アイテムインデックス
	datIndex += SetByteData(&msg[datIndex], rule_flg );	
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< ルール変更パケット作成

//> チーム数パケット作成
// team_count: チーム数
// msg			: out パケット
INT PacketMaker::MakePacketData_RoomInfoStageSelect(int stage_index, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_STAGE_SEL);
	// アイテムインデックス
	datIndex += SetByteData(&msg[datIndex], stage_index );	
	// エンドマーカ
	datIndex += SetEndMarker(&msg[datIndex]);
	SetWordData(&msg[0], datIndex);

	return datIndex;
}
//< チーム数パケット作成

//> ロード命令パケットヘッダ作成
// team_count	: チーム数
// rule				: ルールフラグ
// msg				: out パケット
INT PacketMaker::MakePacketHeadData_Load(int teams, BYTE rule, int stage, int chara_count, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;

	//> OUT
	// size			:	2
	// header		:	1
	// items			:	4*MAX_ITEM_COUNT
	// teams		:	1
	// stage			:	1
	// rule			:	1
	// user_count:	users
	//> users
	// obj_no		:	1
	// obj_type	:	1
	// team_no	:	1
	//< users
	//	msg			:	msg_len

	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_LOAD);
	// チーム数
	datIndex += SetByteData(&msg[datIndex], teams);
	// ステージ
	datIndex += SetByteData(&msg[datIndex], (BYTE)stage);
	// ルール
	datIndex += SetByteData(&msg[datIndex], rule);
	// キャラ数
	datIndex += SetByteData(&msg[datIndex], (BYTE)chara_count);
	return datIndex;
}
//< ロード命令パケットヘッダ作成

//> ロード命令パケット（キャラ）作成
// nCharaType	: キャラタイプ
// blg				: 弾
// msg			: out パケット
INT PacketMaker::AddPacketData_Load(int datIndex, ptype_session sess, BYTE* msg)
{
	// ObjNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// ObjType
	datIndex += SetByteData(&msg[datIndex], sess->chara_type);
	// Team No
	datIndex += SetByteData(&msg[datIndex], sess->team_no);

	return datIndex;
}
//< ロード命令パケット（キャラ）作成

//> メイン開始パケット（キャラ）作成
// datIndex	: index
// sess			: キャラ
// msg			: out パケット
INT PacketMaker::AddPacketData_MainStart(int datIndex, ptype_session sess, BYTE* msg)
{
	// ObjIndex
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// x座標
	datIndex += SetShortData(&msg[datIndex], sess->ax);
	// y座標
	datIndex += SetShortData(&msg[datIndex], sess->ay);
	// 方向
	datIndex += SetByteData(&msg[datIndex], sess->dir);
	// 角度
	datIndex += SetShortData(&msg[datIndex], sess->angle);
	// HP
	datIndex += SetShortData(&msg[datIndex], sess->HP_c);
	// MV
	datIndex += SetShortData(&msg[datIndex], sess->MV_c);
	// delay
	datIndex += SetShortData(&msg[datIndex], sess->delay);
	// exp_c
	datIndex += SetShortData(&msg[datIndex], sess->EXP_c);

	return datIndex;
}

//> メイン開始パケット（ヘッダ）作成
// msg				: out パケット
INT PacketMaker::MakePacketHeadData_MainStart(int chara_count, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_START);
	// キャラ数
	datIndex += SetByteData(&msg[datIndex], (BYTE)chara_count);

	return datIndex;
}
//< メイン開始パケット（ヘッダ）作成

// オブジェクト削除パケット作成
// obj_no	: オブジェクト番号
// msg		: out パケット
INT PacketMaker::MakePacketData_MainInfoRemoveObject(E_OBJ_RM_TYPE rm_type, ptype_obj obj ,BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_OBJECT_RM);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], obj->obj_no);
	// 消える種類
	datIndex += SetByteData(&msg[datIndex], (BYTE)rm_type);
	// 座標X
	datIndex += SetShortData(&msg[datIndex], obj->ax);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], obj->ay);
	// 移動値X
	datIndex += SetShortData(&msg[datIndex], obj->vx);
	// 移動値Y
	datIndex += SetShortData(&msg[datIndex], obj->vy);
	// frame
	datIndex += SetWordData(&msg[datIndex], obj->frame_count);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> キャラ移動パケット(データ)作成
// msg				: out パケット
INT PacketMaker::MakePacketData_MainInfoMoveChara(ptype_session sess , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_CHARA_MV);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// 座標X
	datIndex += SetShortData(&msg[datIndex], sess->ax);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], sess->ay);
	// 座標X (Y方向に移動値がある、落下などするときXは0にする)
	if (sess->vy == 0)
		datIndex += SetShortData(&msg[datIndex], sess->vx);
	else
		datIndex += SetShortData(&msg[datIndex], 0);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	// 向いてる方向
	datIndex += SetByteData(&msg[datIndex], (BYTE)((ptype_session)sess)->dir);
	// キャラの傾き
	datIndex += SetShortData(&msg[datIndex], ((ptype_session)sess)->angle);
	// 現在の移動残り値
	datIndex += SetShortData(&msg[datIndex], sess->MV_c);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< キャラ更新パケット(データ)作成

//> 弾移動パケット(データ)作成
// blg				: 弾
// msg			: out パケット
INT PacketMaker::AddPacketData_MainInfoMoveBullet(int datIndex, ptype_blt blt , BYTE* msg)
{
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], blt->obj_no);
	// 座標X
	datIndex += SetShortData(&msg[datIndex], blt->ax);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], blt->ay);
	// 移動値X
	datIndex += SetShortData(&msg[datIndex], blt->vx);
	// 移動値Y
	datIndex += SetShortData(&msg[datIndex], blt->vy);
	// 加速値X
	datIndex += SetCharData(&msg[datIndex], blt->adx);
	// 加速値Y
	datIndex += SetCharData(&msg[datIndex], blt->ady);
	// frame
	datIndex += SetWordData(&msg[datIndex], blt->frame_count);
	WCHAR log[32];
	SafePrintf(log, 32, L"MvBlt:#%d(%d,%d,%d,%d)", blt->obj_no, blt->ax, blt->ay, blt->vx,blt->vy);
	AddMessageLog(log);
	return datIndex;
}

//> オブジェクト移動パケット(データ)作成
// blg				: 弾
// msg			: out パケット
INT PacketMaker::AddPacketData_MainInfoMoveObject(int datIndex, ptype_obj obj , BYTE* msg)
{
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], obj->obj_no);
	// 座標X
	datIndex += SetShortData(&msg[datIndex], obj->ax);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], obj->ay);
	// 移動値X
	datIndex += SetShortData(&msg[datIndex], obj->vx);
	// 移動値Y
	datIndex += SetShortData(&msg[datIndex], obj->vy);
	// frame
	datIndex += SetWordData(&msg[datIndex], (WORD)obj->frame_count);
	return datIndex;
}
// 弾発射パケット作成
// blt_count	: 作成する弾数
// msg			: out パケット
INT PacketMaker::MakePacketData_MainInfoBulletShot(ptype_blt blt , BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_BULLET_SHOT);
	// キャラオブジェクト番号
	datIndex += SetShortData(&msg[datIndex], blt->chr_obj_no);
	// キャラ
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->chara_type);
	// 弾の種類
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->bullet_type);
	// 弾番号
	datIndex += SetShortData(&msg[datIndex], blt->obj_no);
	// 処理タイプ
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->proc_type);
	// 弾タイプ
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->obj_type);
	// 状態
	datIndex += SetDwordData(&msg[datIndex], (DWORD)blt->obj_state);
	// 弾座標X
	datIndex += SetShortData(&msg[datIndex], blt->ax);
	// 弾座標Y
	datIndex += SetShortData(&msg[datIndex], blt->ay);
	// 弾移動X
	datIndex += SetShortData(&msg[datIndex], blt->vx);
	// 弾移動Y
	datIndex += SetShortData(&msg[datIndex], blt->vy);
	// 弾加速X
	datIndex += SetCharData(&msg[datIndex], blt->adx);
	// 弾加速Y
	datIndex += SetCharData(&msg[datIndex], blt->ady);
	// extdata1
	datIndex += SetDwordData(&msg[datIndex], blt->extdata1);
	// extdata2
	datIndex += SetDwordData(&msg[datIndex], blt->extdata2);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//> キャラ死亡パケット作成
// obj_no	: オブジェクト番号
// msg		: out パケット
INT PacketMaker::MakePacketData_MainInfoDeadChara(E_TYPE_PACKET_MAININFO_HEADER dead_type, int obj_no, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], dead_type);
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], (short)obj_no);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> アクティブ情報パケット作成
// obj_no		: No
// msg			: out パケット
INT PacketMaker::MakePacketData_MainInfoActive(ptype_session sess, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_ACTIVE);
	// アクティブ情報
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// 現在ターン数
	datIndex += SetWordData(&msg[datIndex], sess->turn_count);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< アクティブ情報パケット作成

//> ターンエンド情報パケット作成
// act			: アクティブ
// delay			: ディレイ値
// msg			: out パケット
INT PacketMaker::MakePacketData_MainInfoTurnEnd(ptype_session sess, int wind, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_TURNEND);
	// ディレイ値
	datIndex += SetShortData(&msg[datIndex], sess->delay);
	// オブジェクト状態
	datIndex += SetDwordData(&msg[datIndex], (DWORD)sess->obj_state);
	// 移動値の初期化
	datIndex += SetShortData(&msg[datIndex], sess->MV_m);
	// 風向き
	datIndex += SetCharData(&msg[datIndex], (char)wind);
	// ターン数
	datIndex += SetWordData(&msg[datIndex], (WORD)sess->turn_count);
	// 生存ターン数
	datIndex += SetWordData(&msg[datIndex], (WORD)sess->live_count);
	// EXP_c
	datIndex += SetShortData(&msg[datIndex], sess->EXP_c);	
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< アクティブ情報パケット作成

//> 弾爆発パケット作成
// obj_no
// pos_x	: 座標X
// pos_y	: 座標Y
// msg		: out パケット
INT PacketMaker::MakePacketHeader_MainInfoBombObject(int scr_id,int blt_type,int blt_chr_no, int blt_no, int pos_x, int pos_y, int erase, BYTE* msg)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);	//3
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_OBJECT_BOMB);	//4
	// scr_id
	datIndex += SetByteData(&msg[datIndex], (BYTE)scr_id);//5
	// blt_type
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt_type);//6
	// blt_chr_no
	datIndex += SetShortData(&msg[datIndex], (short)blt_chr_no);//8
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)blt_no);//10
	// 座標X
	datIndex += SetShortData(&msg[datIndex], (short)pos_x);//12
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], (short)pos_y);//14
	// erase
	datIndex += SetByteData(&msg[datIndex], (BYTE)erase);//15
	// hit chara count 0 clear
	datIndex += SetByteData(&msg[datIndex], 0);
	
	return datIndex;
}
const int c_nhit_chara_count = 15;
INT PacketMaker::AddPacketData_MainInfoBombObject(int datIndex, int hit_chr_no, int power, BYTE* msg)
{
	msg[c_nhit_chara_count]++;	// 数
	// ObjNo
	datIndex += SetShortData(&msg[datIndex], (short)hit_chr_no);
	// power(0-100)
	datIndex += SetByteData(&msg[datIndex], (BYTE)power);
	return datIndex;
}
//< 弾爆発パケット作成

//> ステージに画像貼り付けパケット作成
INT PacketMaker::MakePacketData_MainInfoPasteImage(int chr_type, int stage_x, int stage_y, int image_x, int image_y, int image_w, int image_h, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_PASTE_IMAGE);
	// chr_type
	datIndex += SetByteData(&msg[datIndex], (BYTE)chr_type);
	// 座標X
	datIndex += SetShortData(&msg[datIndex], (short)stage_x);
	// 座標Y
	datIndex += SetShortData(&msg[datIndex], (short)stage_y);
	// 画像範囲X
	datIndex += SetShortData(&msg[datIndex], (short)image_x);
	// 画像範囲Y
	datIndex += SetShortData(&msg[datIndex], (short)image_y);
	// 画像範囲W
	datIndex += SetShortData(&msg[datIndex], (short)image_w);
	// 画像範囲H
	datIndex += SetShortData(&msg[datIndex], (short)image_h);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< ステージに画像貼り付けパケット作成

//> キャラステータス更新パケット作成
// sess		: キャラ
// msg		: out パケット
INT PacketMaker::MakePacketData_MainInfoUpdateCharacterStatus(ptype_session sess , BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_CHARA_UPDATE_STATUS);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// HP
	datIndex += SetShortData(&msg[datIndex], sess->HP_c);
	// MV max
	datIndex += SetShortData(&msg[datIndex], sess->MV_m);
	// MV
	datIndex += SetShortData(&msg[datIndex], sess->MV_c);
	// delay
	datIndex += SetShortData(&msg[datIndex], sess->delay);
	// exp_c
	datIndex += SetShortData(&msg[datIndex], sess->EXP_c);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< キャラステータス更新パケット作成

//> メインゲーム終了パケット作成
// msg		: out パケット
INT PacketMaker::MakePacketData_MainInfoGameEnd(std::vector<ptype_session>* vecCharacters, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;

	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_GAMEEND);

	int nCountDatIndex = datIndex;
	int nUserCount=0;
	datIndex++;

	for (std::vector<ptype_session>::iterator it = vecCharacters->begin();
		it != vecCharacters->end();
		it++)
	{
		// obj_no
		datIndex += SetShortData(&msg[datIndex], (short)(*it)->obj_no);
		// No
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it)->frame_count);
		// live_count
		datIndex += SetWordData(&msg[datIndex], (WORD)(*it)->live_count);
		nUserCount++;
	}

	// UserCount
	SetByteData(&msg[nCountDatIndex], (BYTE)nUserCount);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< メインゲーム終了パケット作成

//> 状態更新パケット作成
//	state	: 状態
// msg		: out パケット
INT PacketMaker::MakePacketData_MainInfoUpdateObjectState(int obj_no, E_TYPE_OBJ_STATE state, WORD frame, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_OBJECT_UPDATE_STATE);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)obj_no);
	// state
	datIndex += SetDwordData(&msg[datIndex], (DWORD)state);
	// frame
	datIndex += SetDwordData(&msg[datIndex], (DWORD)frame);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< 状態更新パケット作成

//> キャラ状態更新パケット
INT PacketMaker::MakePacketData_MainInfoUpdateCharaState(ptype_session sess, int nStateIndex, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_CHARA_UPDATE_STATE);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);
	// index
	datIndex += SetByteData(&msg[datIndex], (BYTE)nStateIndex);
	// state
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->chara_state[nStateIndex]);
	// angle
	datIndex += SetShortData(&msg[datIndex], sess->angle);
	// x
	datIndex += SetShortData(&msg[datIndex], sess->ax);
	// y
	datIndex += SetShortData(&msg[datIndex], sess->ay);
	// dir
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->dir);
	// vx
	datIndex += SetShortData(&msg[datIndex], sess->vx);
	// vy
	datIndex += SetShortData(&msg[datIndex], sess->vy);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< キャラ状態更新パケット

//> オブジェクトタイプ更新パケット作成
//	type : タイプ
// msg		: out パケット
INT PacketMaker::MakePacketData_MainInfoUpdateObjectType(type_blt* blt, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_BULLET_UPDATE_TYPE);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], blt->obj_no);
	// type
	datIndex += SetByteData(&msg[datIndex], (BYTE)blt->obj_type);
	// x
	datIndex += SetShortData(&msg[datIndex], blt->ax);
	// y
	datIndex += SetShortData(&msg[datIndex], blt->ay);
	// vx
	datIndex += SetShortData(&msg[datIndex], blt->vx);
	// vy
	datIndex += SetShortData(&msg[datIndex], blt->vy);
	// adx
	datIndex += SetCharData(&msg[datIndex], blt->adx);
	// adx
	datIndex += SetCharData(&msg[datIndex], blt->ady);
	// frame
	datIndex += SetWordData(&msg[datIndex], blt->frame_count);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< オブジェクトタイプ更新パケット作成

//> キャラ状態更新パケット
INT PacketMaker::MakePacketData_MainInfoTrigger(int nCharaIndex, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_BULLET_TRIGGER);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)nCharaIndex);
	// スクリプト/アイテムのタイプ
	datIndex += SetByteData(&msg[datIndex], (BYTE)nProcType);
	// 演出のタイプ
	datIndex += SetByteData(&msg[datIndex], (BYTE)nBltType);
	// 角度
	datIndex += SetShortData(&msg[datIndex], (short)nShotAngle);
	// パワー
	datIndex += SetShortData(&msg[datIndex], (short)nShotPower);
	// Indicator角度
	datIndex += SetShortData(&msg[datIndex], (short)nShotIndicatorAngle);
	// Indicatorパワー
	datIndex += SetShortData(&msg[datIndex], (short)nShotIndicatorPower);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< キャラ状態更新パケット

INT PacketMaker::MakePacketData_MainInfoReqTriggerEnd(int nCharaIndex, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_REQ_TRIGGER_END);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)nCharaIndex);

	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> 弾発射要求パケット
INT PacketMaker::MakePacketData_MainInfoReqShot(int objno, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_REQ_MAININFO_BULLET_SHOT);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)objno);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< 弾発射要求パケット

//> 弾発射拒否パケット
INT PacketMaker::MakePacketData_MainInfoRejShot(int objno, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_REJ_MAININFO_BULLET_SHOT);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], (short)objno);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< 弾発射拒否パケット

//> 結果パケット
INT PacketMaker::MakePacketData_Confirmed(ptype_session sess, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_CONFIRMED);
	// ユーザNo
	datIndex += SetByteData(&msg[datIndex], sess->sess_index);	
	// obj_state
	datIndex += SetDwordData(&msg[datIndex], sess->obj_state);
	// master
	datIndex += SetByteData(&msg[datIndex], sess->master);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< 結果パケット

//> 風向設定パケット
INT PacketMaker::MakePacketData_MainInfoSetWind(int wind, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_WIND);
	// 風向き
	datIndex += SetCharData(&msg[datIndex], (char)wind);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< 風向設定パケット

//> アイテム追加パケット
INT PacketMaker::MakePacketData_MainInfoAddItem(int obj_no, int slot, DWORD item_flg, BYTE* msg,BOOL bSteal)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO_ADD_ITEM);
	// オブジェクト番号
	datIndex += SetShortData(&msg[datIndex], (short)obj_no);
	// スロット番号
	datIndex += SetByteData(&msg[datIndex], (BYTE)slot);
	// アイテムフラグ
	datIndex += SetDwordData(&msg[datIndex], item_flg);
	// アイテムスティール
	datIndex += SetByteData(&msg[datIndex], (BYTE)bSteal);

	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< アイテム追加パケット

//> 自ターンパス拒否
INT PacketMaker::MakePacketData_MainInfoRejTurnPass(ptype_session sess, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_MAININFO);
	// メイン情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_REJ_MAININFO_TURN_PASS);
	// obj_no
	datIndex += SetShortData(&msg[datIndex], sess->obj_no);

	return PacketMaker::AddEndMarker(datIndex, msg);
}

//< 自ターンパス拒否

// ロード完了要求
INT PacketMaker::MakePacketData_LoadReqLoadComplete(ptype_session sess, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_REQ_LOAD_COMPLETE);
	// obj_no
	datIndex += SetByteData(&msg[datIndex], (BYTE)sess->sess_index);
	// frame
	datIndex += SetWordData(&msg[datIndex], (WORD)sess->frame_count);

	return PacketMaker::AddEndMarker(datIndex, msg);
}

//> ハッシュ値要求パケット作成
// msg		: out パケット
INT PacketMaker::MakePacketData_AuthReqHash(int hash_group, int hash_id , BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_REQ_HASH);
	// group
	datIndex += SetByteData(&msg[datIndex], (BYTE)hash_group);
	// id
	datIndex += SetWordData(&msg[datIndex], (WORD)hash_id);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< キャラステータス更新パケット作成

//> 制限ターン数パケット
INT PacketMaker::MakePacketData_RoomInfoTurnLimit(int turn, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RULE_TURN_LIMIT);
	// ターン数
	datIndex += SetWordData(&msg[datIndex], (short)turn);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}
//< 制限ターン数パケット

// 制限時間パケット
INT PacketMaker::MakePacketData_RoomInfoActTimeLimit(int time, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_RULE_ACT_TIME_LIMIT);
	// ターン数
	datIndex += SetByteData(&msg[datIndex], (BYTE)time);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

// ファイルハッシュ送信パケット
INT PacketMaker::MakePacket_FileInfoSendHash(BOOL bCharaScr, int id, int fileno, char* md5, WCHAR* path, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO_OPEN);
	// キャラスクリプト
	datIndex += SetByteData(&msg[datIndex], (BYTE)bCharaScr);
	// スクリプトID
	datIndex += SetWordData(&msg[datIndex], (WORD)id);
	// FileNO
	datIndex += SetByteData(&msg[datIndex], (BYTE)fileno);
	// md5
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)md5, MD5_LENGTH, MD5_LENGTH);
	WORD wLen = wcslen(path);
	// path len
	datIndex += SetWordData(&msg[datIndex], wLen);
	// path
	datIndex += SetMultiByteData(&msg[datIndex], (BYTE*)path, sizeof(WCHAR)*wLen, _MAX_PATH*2);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

// ファイル送信パケット
INT PacketMaker::MakePacket_FileInfoSendData(BOOL bCharaScr, int id, int fileno ,BOOL eof , int dat_index, int size, BYTE* buffer, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO_SEND);
	// キャラスクリプト
	datIndex += SetByteData(&msg[datIndex], (BYTE)bCharaScr);
	// スクリプトID
	datIndex += SetWordData(&msg[datIndex], (WORD)id);
	// FileNO
	datIndex += SetByteData(&msg[datIndex], (BYTE)fileno);
	// EOF
	datIndex += SetByteData(&msg[datIndex], (BYTE)eof);
	// dat index
	datIndex += SetDwordData(&msg[datIndex], (DWORD)dat_index);
	// dat size
	datIndex += SetDwordData(&msg[datIndex], (DWORD)size);
	// dat buffer
	if (size)
		datIndex += SetMultiByteData(&msg[datIndex], buffer, size, MAX_SEND_BUFFER_SIZE);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

// ファイルデータ送信終了パケット
INT PacketMaker::MakePacket_FileInfoSendClose(BOOL bCharaScr, int id, BYTE* msg)
{
	int	datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_FILEINFO_CLOSE);
	// キャラスクリプト
	datIndex += SetByteData(&msg[datIndex], (BYTE)bCharaScr);
	// スクリプトID
	datIndex += SetWordData(&msg[datIndex], (WORD)id);
	
	return PacketMaker::AddEndMarker(datIndex, msg);
}

INT PacketMaker::MakePacketData_TeamRandomHeader(BYTE* msg, BYTE teams)
{
	int		datIndex = 2;
	if (!msg)	return 0;
	//	ヘッダーコード(固定位置)
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO);
	// ルーム情報ヘッダ
	datIndex += SetByteData(&msg[datIndex], PK_USER_ROOMINFO_TEAM_RAND);
	// チーム数
	datIndex += SetByteData(&msg[datIndex], teams);

	return datIndex;
}

INT PacketMaker::MakePacketData_TeamRandomAddData(BYTE* msg, std::wstring& wstr)
{
//	const int c_maxMsgSize = MAX_CHAT_MSG - 10;	// チーム①:
	INT datIndex = SetWStringData(msg, wstr);
	return datIndex;
}

