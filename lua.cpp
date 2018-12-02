#include "ext.h"
#include "main.h"

static char g_GetUserNameBuffer[MAX_USER_NAME*2+2];

int C_CreateBullet(double proc_type, double chr_obj_no, double chara_type, double blt_type, double obj_type, double posx, double posy, double vecx, double vecy, double addx, double addy, double hit_range, double extdata1, double extdata2)
{
	type_blt blt;
	ZeroMemory(&blt, sizeof(type_blt));
	blt.proc_type = (BYTE)proc_type;
	if (blt.proc_type == BLT_PROC_TYPE_SCR_STAGE)
		blt.chr_obj_no = (short)DEF_STAGE_OBJ_NO;
	else
		blt.chr_obj_no = (short)chr_obj_no;
	blt.adx = (char)addx;
	blt.ady = (char)addy;
	blt.chara_type = (BYTE)chara_type;
	if (proc_type == BLT_PROC_TYPE_SCR_CHARA && blt_type >= MAX_CHARA_BULLET_TYPE)
	{
		MessageBox(g_hWnd, L"不正なblt_type",L"script error", MB_OK);
		return -1;
	}

	blt.bullet_type = (BYTE)blt_type;
	blt.obj_type = (E_OBJ_TYPE)((OBJ_TYPE_BLT)|(int)obj_type);
	blt.ax = (short)posx;
	blt.ay =(short) posy;
	blt.vx = (short)vecx;
	blt.vy = (short)vecy;
	blt.hit_range = (BYTE)hit_range;
	blt.extdata1 = (DWORD)extdata1;
	blt.extdata2 = (DWORD)extdata2;
	blt.obj_state = OBJ_STATE_MAIN_ACTIVE;

	g_pCriticalSection->EnterCriticalSection_Object(L'-');
	int res = g_pSyncMain->AddObject( &blt );
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

bool C_Shot(int chr_obj_no, int proc_type, int chr_id, int blt_type, int vec_angle, int power, int frame, int indicator_angle, int indicator_short)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'E');
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(chr_obj_no);
	if (!sess)
	{
		AddMessageLog(L"!C_Shot chr_obj_no out range");
		g_pSyncMain->SetPhase(GAME_MAIN_PHASE_TURNEND);
		g_pCriticalSection->LeaveCriticalSection_Session();
		return true;	// 発射終了
	}
	bool ret = g_pPPMain->ShootingCommand(sess ,proc_type, chr_id, blt_type, vec_angle, (int)(power*BLT_POS_FACT_F), chr_obj_no, frame, indicator_angle, indicator_short);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return ret;
}

bool C_RemoveBullet(double obj_no, int rm_type)
{
	bool res = false;
	g_pCriticalSection->EnterCriticalSection_Object(L'^');
	res = g_pSyncMain->RemoveObject((int)obj_no, (E_OBJ_RM_TYPE)rm_type);
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

bool C_IsRemovedObject(double obj_no)
{
	bool res = false;
	g_pCriticalSection->EnterCriticalSection_Object(L'H');
	res = g_pSyncMain->IsRemovedObject((int)obj_no);
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

//弾作ったキャラObjNo,弾タイプ
bool C_BombObject(int scr_id, int blt_type, int blt_chr_no, int blt_no, int pos_x, int pos_y,int erase)
{
	g_pCriticalSection->EnterCriticalSection_Object(L'\\');
	bool res = g_pSyncMain->BombObject(scr_id, blt_type, blt_chr_no, blt_no, pos_x, pos_y,erase);
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

bool C_UpdateBulletState(double obj_no, double state)
{
	DWORD dwNewState = OBJ_STATE_GAME|((OBJ_STATE_MAIN_WAIT_FLG|OBJ_STATE_MAIN_ACTIVE_FLG)&(DWORD)state);
	g_pCriticalSection->EnterCriticalSection_Object(L'!');
	bool res = g_pSyncMain->UpdateObjectState((int)obj_no, (E_TYPE_OBJ_STATE)dwNewState);
	if (res)
		g_pSyncMain->SetObjectLuaFlg((int)obj_no, PROC_FLG_OBJ_UPDATE_STATE, TRUE);
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}
bool C_UpdateBulletPosition(double obj_no, double px, double py, double vx, double vy, double adx, double ady)
{
	bool res = false;
	g_pCriticalSection->EnterCriticalSection_Object(L'"');
	if (res = g_pSyncMain->UpdateBulletPositoin((int)obj_no,px,py,vx,vy,adx,ady))
		g_pSyncMain->SetObjectLuaFlg((int)obj_no, PROC_FLG_OBJ_UPDATE_POS, TRUE);
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

bool C_UpdateBulletVector(double obj_no, double vx, double vy, double adx, double ady)
{
	bool res = false;
	g_pCriticalSection->EnterCriticalSection_Object(L'#');
	if (res = g_pSyncMain->UpdateBulletVector((int)obj_no,vx,vy,adx,ady))
		g_pSyncMain->SetObjectLuaFlg((int)obj_no, PROC_FLG_OBJ_UPDATE_VEC, TRUE);
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

bool C_DamageCharaHP(int assailant_no, int victim_no, int hp)
{
	if (!hp) return false;
	g_pCriticalSection->EnterCriticalSection_Session(L'R');
	bool res =  g_pSyncMain->DamageCharaHP(assailant_no, victim_no, hp);	
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_UpdateCharaStatus(int obj_no, int hp, int mv, int delay, int exp)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'T');
	bool res = g_pSyncMain->UpdateCharaStatus(obj_no, hp, mv, delay,exp);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_UpdateCharaPos(int obj_no, int x, int y)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'Y');
	bool res = g_pSyncMain->UpdateCharaPos(obj_no, x,y);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_SetBulletOptionData(int obj_no, int index, int data)
{
	g_pCriticalSection->EnterCriticalSection_Object(L'%');
	bool res = g_pSyncMain->SetBulletOptionData(obj_no, index, (DWORD)(data));
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

unsigned int C_GetBulletOptionData(int obj_no, int index)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'I');
	unsigned int res = g_pSyncMain->GetBulletOptionData(obj_no, index);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_SetCharaOptionData(int obj_no, int index, int data)
{
	g_pCriticalSection->EnterCriticalSection_Object(L'%');
	bool res = g_pSyncMain->SetCharaOptionData(obj_no, index, (DWORD)(data));
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

unsigned int C_GetCharaOptionData(int obj_no, int index)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'I');
	unsigned int res = g_pSyncMain->GetCharaOptionData(obj_no, index);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_SetBulletExtData1(int obj_no, unsigned long extdata1)
{
	g_pCriticalSection->EnterCriticalSection_Object(L'$');
	bool res = g_pSyncMain->SetBulletExtData1(obj_no, (DWORD)(extdata1));
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

bool C_SetBulletExtData2(int obj_no, unsigned long extdata2)
{
	g_pCriticalSection->EnterCriticalSection_Object(L'%');
	bool res = g_pSyncMain->SetBulletExtData2(obj_no, (DWORD)(extdata2));
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

unsigned int C_GetCharaExtData1(int obj_no)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'U');
	unsigned int res = g_pSyncMain->GetCharaExtData1(obj_no);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

unsigned int C_GetCharaExtData2(int obj_no)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'I');
	unsigned int res = g_pSyncMain->GetCharaExtData2(obj_no);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_SetCharaExtData1(int obj_no, unsigned long extdata1)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'O');
	bool res = g_pSyncMain->SetCharaExtData1(obj_no, (DWORD)(extdata1));
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_SetCharaExtData2(int obj_no, unsigned long extdata2)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'P');
	bool res = g_pSyncMain->SetCharaExtData2(obj_no, (DWORD)(extdata2));
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_AddCharaItem(int obj_no, int item_flg)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'@');
	bool res = g_pSyncMain->AddCharaItem(obj_no, (DWORD)(item_flg));
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

bool C_SetCharaState(int obj_no, int chr_stt, int val)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'[');
	bool res = g_pSyncMain->SetCharacterState(obj_no, chr_stt, val);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

int C_GetCharaState(int obj_no, int chr_stt)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'[');
	int res = g_pSyncMain->GetCharacterState(obj_no, chr_stt);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

unsigned long C_GetCharaItem(int obj_no, int item_index)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'[');
	unsigned long res = g_pSyncMain->GetCharacterItemInfo(obj_no, item_index);
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

// スクリプトを使用しているキャラNo、ステージステージXY、テクスチャXYWH
void C_PasteTextureOnStage(int scr_id, int sx,int sy, int tx,int ty,int tw,int th)
{
	g_pSyncMain->PasteTextureOnStage(scr_id, sx,sy,tx,ty,tw,th);
	return;
}

int C_GetRand(int min, int max)
{
	if (min>max)
		MessageBox(g_hWnd, L"C_GetRand() min > max", L"script error", MB_OK);
	return (genrand_int32() % (max-min)) + min;
}

int C_GetEntityCharacters()
{
	g_pCriticalSection->EnterCriticalSection_Session(L'q');
	int res = g_pSyncMain->GetEntityCharacters();
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

int C_GetLivingCharacters()
{
	g_pCriticalSection->EnterCriticalSection_Session(L'w');
	int res = g_pSyncMain->GetLivingCharacters();
	g_pCriticalSection->LeaveCriticalSection_Session();
	return res;
}

type_session C_GetCharacterAtVector(int index)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'e');
	type_session* sess = g_pSyncMain->GetCharacterAtVector(index);
	g_pCriticalSection->LeaveCriticalSection_Session();
	if (!sess)
		MessageBox(NULL, L"C_GetCharacterAtVector()", L"script error", MB_OK);
	return *sess;
}

type_session C_GetCharacterFromObjNo(int obj_no)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'r');
	ptype_session sess = g_pNetSess->GetSessionFromUserNo(obj_no);
	g_pCriticalSection->LeaveCriticalSection_Session();
	if (!sess)
		MessageBox(NULL, L"C_GetCharacterFromObjNo()", L"script error", MB_OK);
	return *sess;
}

type_blt C_GetBulletInfo(int obj_no)
{
	g_pCriticalSection->EnterCriticalSection_Object(L'&');
	type_blt* blt = g_pSyncMain->GetBulletInfo(obj_no);
	g_pCriticalSection->LeaveCriticalSection_Object();
//	if (!blt)
//		MessageBox(NULL, L"C_GetBulletInfo()", L"script error", MB_OK);
	return *blt;
}

int C_GetBulletAtkValue(int blt_no)
{
	int ret = g_pSyncMain->GetBulletAtkValue(blt_no);
	return ret;
}

unsigned int C_GetBulletExtData1(int obj_no)
{
	DWORD res = 0;
	g_pCriticalSection->EnterCriticalSection_Object(L'\'');
	ptype_blt blt = g_pSyncMain->GetBulletInfo(obj_no);
	if (blt)	res = blt->extdata1;
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

unsigned int C_GetBulletExtData2(int obj_no)
{
	DWORD res = 0;
	g_pCriticalSection->EnterCriticalSection_Object(L'(');
	ptype_blt blt = g_pSyncMain->GetBulletInfo(obj_no);
	if (blt)	res = blt->extdata2;
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

int C_GetWindValue()
{
	return g_pSyncMain->GetWind();
}

void C_SetWindValue(int wind, int dir)
{
	g_pSyncMain->SetWind(wind, dir);
}

void C_SetCameraFocusToBullet(int blt_no){}

void C_SetCameraFocusToChara(int obj_no){}

int C_GetStageWidth()
{
	return g_pSyncMain->m_pMainStage->GetStageWidth();
}

int C_GetStageHeight()
{
	return g_pSyncMain->m_pMainStage->GetStageHeight();
}

bool C_UpdateObjectType(int obj_no, int type)
{
	bool res = false;
	g_pCriticalSection->EnterCriticalSection_Object(L')');
	if (res = g_pSyncMain->UpdateObjType(obj_no, (E_OBJ_TYPE)type))
		g_pSyncMain->SetObjectLuaFlg(obj_no, PROC_FLG_OBJ_UPDATE_TYPE, TRUE);
	g_pCriticalSection->LeaveCriticalSection_Object();
	return res;
}

//> 描画情報設定はスルー
bool C_UpdateBulletAngle(double blt_no, double angle){	return true;	}
bool C_SetBulletTextureIndex(double blt_no, double index) {	return true;	};
bool C_SetCharaTexture(int chr_no, int left, int top, int right, int bottom) {	return true;	};

//> effect	サーバはスルー
int C_AddEffect(double chr_id, double left, double top, double right, double bottom, double x, double y, double age){	return -1;}
bool C_RemoveEffect(double effect_no){	return true;	}
bool C_SetEffectVector(double effect_no, double vx, double vy, double ax, double ay, double effect_time){	return true;	}
bool C_SetEffectAlpha(double effect_no, double alpha) {	return true;	}
bool C_SetEffectFade(double effect_no, double fade, double effect_time){	return true;	}
bool C_SetEffectFadeInOut(double effect_no, double fadeinout){	return true; }
bool C_SetEffectRotate(double effect_no, double rot){	return true;	}
bool C_SetEffectRotation(double effect_no, double rot, double effect_time){	return true;	}
bool C_SetEffectScale(double effect_no, double scalex, double scaley){	return true;	}
bool C_SetEffectScalling(double effect_no, double scalex, double scaley, double effect_time){	return true;	}
bool C_SetEffectAnimation(double effect_no, double atime, double acount, bool loop, double effect_time){	return true;	}
bool C_SetEffectTexture(int effect_no, double left, double top, double right, double bottom) { return true;	}


int C_AddBGEffect(double chr_id, double left, double top, double right, double bottom, double x, double y, double age){	return -1;}
bool C_RemoveBGEffect(double effect_no){	return true;	}
bool C_SetBGEffectVector(double effect_no, double vx, double vy, double ax, double ay, double effect_time){	return true;	}
bool C_SetBGEffectAlpha(double effect_no, double alpha) {	return true;	}
bool C_SetBGEffectFade(double effect_no, double fade, double effect_time){	return true;	}
bool C_SetBGEffectFadeInOut(double effect_no, double fadeinout){	return true; }
bool C_SetBGEffectRotate(double effect_no, double rot){	return true;	}
bool C_SetBGEffectRotation(double effect_no, double rot, double effect_time){	return true;	}
bool C_SetBGEffectScale(double effect_no, double scalex, double scaley){	return true;	}
bool C_SetBGEffectScalling(double effect_no, double scalex, double scaley, double effect_time){	return true;	}
bool C_SetBGEffectAnimation(double effect_no, double atime, double acount, bool loop, double effect_time){	return true;	}
bool C_SetBGEffectTexture(int effect_no, double left, double top, double right, double bottom) { return true;	}

void C_HideStage(){};
void C_ShowStage(){};

// sound	サーバはスルー
// リソースの登録
void C_RegistSoundSE(char* rc_name){}
// リソースの再生
void C_PlaySoundSE(char* rc_name, int loop, double fade){}

// クライアントのキャラNoを得る(サーバは-1を返す)
int C_GetMyCharaNo(){ return -1;};

// 文字列をログに追加
void C_AddMsgLog(char* str)
{
	int len = strlen(str);
	WCHAR *wstr = new wchar_t[len +1]();
	if (wstr)
	{
		if (MultiByteToWideChar(CP_THREAD_ACP, 0, str, len, wstr, len))
			AddMessageLog(wstr);
	}
	SafeDeleteArray(wstr);
}

int C_GetAngle(int x, int y)
{
	return GetAngle(x,y);
}

int C_GetScrIDFromChrNo(int obj_no)
{
	int ret = -1;
	g_pCriticalSection->EnterCriticalSection_Session(L't');
	ptype_session sess= g_pNetSess->GetSessionFromUserNo(obj_no);
	if (sess)
		ret = sess->scrinfo->ID;
	g_pCriticalSection->LeaveCriticalSection_Session();
	return ret;
}

int C_GetUserTeamNo(int obj_no)
{
	int ret = -1;
	g_pCriticalSection->EnterCriticalSection_Session(L't');
	ptype_session sess= g_pNetSess->GetSessionFromUserNo(obj_no);
	if (sess)
		ret = sess->team_no;
	g_pCriticalSection->LeaveCriticalSection_Session();
	return ret;
}

char* C_GetUserName(int obj_no)
{
	g_pCriticalSection->EnterCriticalSection_Session(L'y');
	ptype_session sess= g_pNetSess->GetSessionFromUserNo(obj_no);
	g_GetUserNameBuffer[0] = NULL;
	if (sess)
	{
		WCHAR wsName[MAX_USER_NAME+1];
		common::session::GetSessionName(sess, wsName);
		WideCharToMultiByte(CP_THREAD_ACP, 0, wsName, -1, g_GetUserNameBuffer, MAX_USER_NAME*2+2,NULL,NULL);
	}
	g_pCriticalSection->LeaveCriticalSection_Session();
	return g_GetUserNameBuffer;
}

void C_DbgOutputStr(char* str)
{
	MessageBoxA(NULL, str, "C_DbgOutputStr", MB_OK);
}

void C_DbgOutputNum(int num)
{
	WCHAR msg[64];
	SafePrintf(msg, 64, L"C_DbgOutputNum:%d", num);
	MessageBox(NULL, msg, L"C_DbgOutputNum", MB_OK);
}

void C_DbgAddLogNum(int num)
{
	WCHAR msg[64];
	SafePrintf(msg, 64, L"C_DbgOutputNum:%d", num);
	AddMessageLog(msg);
}

type_ground C_GetRandomStagePos()
{
	type_ground res = {0,0,false};
	POINT pnt;
	if (g_pSyncMain->GetRandomStagePos(&pnt))
	{
		res.x = pnt.x;
		res.y = pnt.y;
		res.ground = true;
	}
	return res;
}

type_ground C_GetGroundPos(int x, int y)
{
	type_ground res = {0,0,false};
	D3DXVECTOR2 vecPut;
	D3DXVECTOR2 vecGround = D3DXVECTOR2(0,0);
	D3DXVECTOR2 vecPos = D3DXVECTOR2((float)x,(float)y);

	if (g_pSyncMain->m_pMainStage->FindGround(&vecPut, &vecGround, &vecPos, CHARA_BODY_RANGE))
	{
		res.x = (int)vecPut.x;
		res.y = (int)vecPut.y;
		res.ground = true;
	}
	return res;
}
