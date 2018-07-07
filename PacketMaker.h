#ifndef H_PACKET_MAKER___
#define H_PACKET_MAKER___

#include "windows.h"
#include <cassert>
#include "../include/define.h"
#include "../include/types.h"
#include <vector>
#include "util.h"

namespace PacketMaker
{

	// 認証結果パケット
	// sess	: セッション
	// msg	: out作成パケット
	// bauth: 認証結果 0/0以外 :成功/エラー番号
	INT MakePacketData_Authentication(ptype_session sess, BYTE* msg, E_TYPE_AUTH_RESULT auth,int group = 0, int id = 0);

	INT MakePacketData_ReqHashList(ptype_session sess, BYTE* msg);

	// 入室パケット作成(1ユーザ情報用)
	INT MakePacketData_RoomInfoIn(ptype_session sess, BYTE* msg);
	// 入室パケットヘッダ作成(パケットヘッダ、ルーム情報ヘッダ、キャラ数)
	// msg	: パケットへのポインタ(先頭のサイズ情報は内部で飛ばす)
	INT MakePacketData_RoomInfoInHeader(BYTE nUserCount, BYTE* msg);
	// 入室パケット1ユーザ情報(キャラタイプなど…)
	// msg	: 情報を追加するパケットへの先頭ポインタ
	INT MakePacketData_RoomInfoInChara(ptype_session sess, BYTE* msg);
	// ルームマスター情報パケット作成
	INT MakePacketData_RoomInfoMaster(int obj_no, BYTE flag, BYTE* msg);

	// ゲーム準備状態変更パケット作成
	// obj_no		: ユーザNo
	// game_ready	: 準備OK/NG
	// msg			: out パケット
	INT MakePacketData_RoomInfoGameReady(BYTE obj_no, BYTE game_ready, BYTE* msg);

	// キャラ選択パケット作成
	// obj_no		: ユーザNo
	// chara_no	: 選択キャラNo
	// msg			: out パケット
	INT MakePacketData_RoomInfoCharaSelect(BYTE obj_no, BYTE chara_no, BYTE* msg);

	// キャラ移動情報パケット作成
	// sess			: ユーザ
	// msg			: out パケット
	INT MakePacketData_RoomInfoRoomCharaMove(ptype_session sess , BYTE* msg);

	// パケットフッタ作成(エンドマーカ、パケットサイズ)
	// wSize: フッタも含めないサイズ
	// msg	: パケットへのポインタ
	INT MakePacketData_SetFooter(WORD wSize, BYTE* msg);

	// チャットパケット
	// in_msg	: 受信したチャットパケット
	// sess		: 受信元セッション
	// msg		: out作成パケット
	INT MakePacketData_UserChat(BYTE *in_msg, ptype_session sess,BYTE *msg);

	// 切断パケット
	// sess	: 切断するセッション
	// msg	: out作成パケット
	INT MakePacketData_UserDiconnect(ptype_session sess, BYTE *msg);

	// アイテム選択パケット作成
	// item_index: アイテムインデックス
	// item_flg		: アイテムフラグ
	// msg			: out パケット
	INT MakePacketData_RoomInfoItemSelect(int index, DWORD item_flg, WORD cost, BYTE* msg);

	// チーム数パケット作成
	// team_count: チーム数
	// msg			: out パケット
	INT MakePacketData_RoomInfoTeamCount(int team_count, BYTE* msg);

	// ルール変更パケット作成
	// rule_flg: チーム数
	// msg			: out パケット
	INT MakePacketData_RoomInfoRule(BYTE rule_flg, BYTE* msg);

	// ルール変更パケット作成
	// stage_index: ステージインデックス
	// msg			: out パケット
	INT MakePacketData_RoomInfoStageSelect(int stage_index, BYTE* msg);

	INT MakePacketData_TeamRandomHeader(BYTE* msg, BYTE teams);
	INT MakePacketData_TeamRandomAddData(BYTE* msg, std::wstring& wstr);

	// ロード命令パケット作成
	// team_count	: チーム数
	// rule				: ルールフラグ
	// msg				: out パケット
	INT MakePacketData_Load(int teams, BYTE rule, int stage, BYTE* msg);

	// ロード命令パケット（ヘッダ）作成
	// team_count	: チーム数
	// rule				: ルールフラグ
	// msg				: out パケット
	INT MakePacketHeadData_Load(int teams, BYTE rule, int stage, int chara_count, BYTE* msg);
	// ロード命令パケット（キャラ）作成
	// nCharaType	: キャラタイプ
	// blg				: 弾
	// msg			: out パケット
	INT AddPacketData_Load(int datIndex, ptype_session sess, BYTE* msg);

	// メイン開始パケット作成
	// pNWSess		: 接続情報
	// msg				: out パケット
	INT MakePacketData_MainStart(BYTE* msg);
	// メイン開始パケット（ヘッダ）作成
	// team_count	: チーム数
	// rule				: ルールフラグ
	// msg				: out パケット
	INT MakePacketHeadData_MainStart(int chara_count, BYTE* msg);
	// メイン開始パケット（キャラ）作成
	// datIndex	: index
	// sess			: キャラ
	// msg			: out パケット
	INT AddPacketData_MainStart(int datIndex, ptype_session sess, BYTE* msg);

	// 弾発射パケット作成
	// blt_count	: 作成する弾数
	// msg			: out パケット
	INT MakePacketData_MainInfoBulletShot(ptype_blt blt , BYTE* msg);

	// キャラ移動パケット作成
	// blt_count	: 更新する弾数
	// msg			: out パケット
	INT MakePacketData_MainInfoMoveChara(ptype_session sess , BYTE* msg);

	// アクティブ情報パケット作成
	// obj_no		: No
	// msg			: out パケット
	INT MakePacketData_MainInfoActive(ptype_session sess, BYTE* msg);

	// ターンエンド情報パケット作成
	// sess			: キャラ
	// msg			: out パケット
	INT MakePacketData_MainInfoTurnEnd(ptype_session sess, int wind, BYTE* msg);

	// オブジェクト削除パケット作成
	// obj_no	: オブジェクト番号
	// msg		: out パケット
	INT MakePacketData_MainInfoRemoveObject(E_OBJ_RM_TYPE rm_type, ptype_obj obj , BYTE* msg);

	// キャラ死亡パケット作成
	// obj_no	: オブジェクト番号
	// msg		: out パケット
	INT MakePacketData_MainInfoDeadChara(E_TYPE_PACKET_MAININFO_HEADER dead_type, int obj_no, BYTE* msg);

	// オブジェクト移動パケット(ヘッダ)作成
	// blt_count	: 更新する弾数
	// msg			: out パケット
	//	INT MakePacketHeadData_MainInfoMoveObject(int obj_count , BYTE* msg);
	// オブジェクト移動パケット(データ)作成
	// nCharaType	: キャラタイプ
	// blg				: 弾
	// msg			: out パケット
	INT AddPacketData_MainInfoMoveObject(int datIndex, ptype_obj obj , BYTE* msg);
	INT AddPacketData_MainInfoMoveBullet(int datIndex, ptype_blt blt , BYTE* msg);

	// 弾爆発パケット作成
	INT MakePacketHeader_MainInfoBombObject(int scr_id,int blt_type,int blt_chr_no, int blt_no, int pos_x, int pos_y, int erase, BYTE* msg);
	INT AddPacketData_MainInfoBombObject(int datIndex, int hit_chr_no, int power, BYTE* msg);

	// ステージに画像貼り付けパケット作成
	INT MakePacketData_MainInfoPasteImage(int chr_type, int stage_x, int stage_y, int image_x, int image_y, int image_w, int image_h, BYTE* msg);

	// 状態更新パケット作成
	//	state	: 状態
	// msg		: out パケット
	INT MakePacketData_MainInfoUpdateObjectState(int obj_no, E_TYPE_OBJ_STATE state, WORD frame, BYTE* msg);

	// オブジェクトタイプ更新パケット作成
	//	type_obj : object
	// msg		: out パケット
	INT MakePacketData_MainInfoUpdateObjectType(type_blt* blt, BYTE* msg);

	// キャラステータス更新パケット作成
	// sess		: キャラ
	// msg		: out パケット
	INT MakePacketData_MainInfoUpdateCharacterStatus(ptype_session sess , BYTE* msg);

	// メインゲーム終了パケット作成
	// sess		: キャラ
	// msg		: out パケット
	INT MakePacketData_MainInfoGameEnd(std::vector<ptype_session>* vecCharacters, BYTE* msg);

	// キャラ状態更新パケット
	INT MakePacketData_MainInfoUpdateCharaState(ptype_session sess, int nStateIndex, BYTE* msg);

	INT MakePacketData_MainInfoItemSelect(int index, DWORD item_flg, BYTE* msg, BOOL steal);

	// トリガーパケット
	INT MakePacketData_MainInfoTrigger(int nCharaIndex, int nProcType, int nBltType, int nShotAngle, int nShotPower, int nShotIndicatorAngle, int nShotIndicatorPower, BYTE* msg);
	// 
	INT MakePacketData_MainInfoReqTriggerEnd(int nCharaIndex, BYTE* msg);
	// 弾発射要求
	INT MakePacketData_MainInfoReqShot(int objno, BYTE* msg);
	// 弾発射拒否
	INT MakePacketData_MainInfoRejShot(int objno, BYTE* msg);

	INT MakePacketData_Confirmed(ptype_session sess, BYTE* msg);

	// 結果画面からロビーへ戻ったときのパケット
	INT MakePacketData_RoomInfoReEnter(ptype_session sess, BYTE* msg);

	// ハッシュ値要求
	INT MakePacketData_AuthReqHash(int hash_group, int hash_id , BYTE* msg);

	// 自ターンパス拒否
	INT MakePacketData_MainInfoRejTurnPass(ptype_session sess, BYTE* msg);

	// ロード完了要求
	INT MakePacketData_LoadReqLoadComplete(ptype_session sess, BYTE* msg);

	// 制限ターン数パケット
	INT MakePacketData_RoomInfoTurnLimit(int turn, BYTE* msg);

	// 制限時間パケット
	INT MakePacketData_RoomInfoActTimeLimit(int time, BYTE* msg);


	// 風向設定パケット
	INT MakePacketData_MainInfoSetWind(int wind, BYTE* msg);

	// アイテム追加パケット
	INT MakePacketData_MainInfoAddItem(int obj_no, int slot, DWORD item_flg, BYTE* msg,BOOL bSteal);

	// ファイルハッシュ送信パケット
	INT MakePacket_FileInfoSendHash(BOOL bCharaScr, int id, int fileno, char* md5, WCHAR* path, BYTE* msg);
	// ファイルデータ送信パケット
	INT MakePacket_FileInfoSendData(BOOL bCharaScr, int id, int fileno, BOOL eof, int dat_index, int size, BYTE* buffer, BYTE* msg);
	// ファイルデータ送信終了パケット
	INT MakePacket_FileInfoSendClose(BOOL bCharaScr, int id, BYTE* msg);



	// キャラヘッダパケット作成
	// header1			: 1バイト目ヘッダ
	// header2			: 2バイト目ヘッダ
	// info_count		: 以下のオブジェクトの情報数(0の場合追加しない)
	// msg			: out パケット
	INT MakePacketHeadData(BYTE header1, BYTE header2, int info_count , BYTE* msg);
	INT AddEndMarker(int datIndex, BYTE* msg);
	INT MakePacketData_SYN(BYTE* msg);
};



#endif