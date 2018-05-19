#include "CNetworkSession.h"
#include "ext.h"

BOOL CNetworkSession::InitSock()
{
	WSADATA		wsaData;
	const char val = 1;

	WSAStartup( MAKEWORD(2,2) , &wsaData ) ;
	if ((sockListener = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0)
	{
		AddMessageLog(L"error in sock");
		return -1;
	}
	// 0:ブロッキングモード(デフォルト)  1:非ブロッキングモード
	unsigned long arg = 1L;
	if(0!=ioctlsocket(sockListener,FIONBIO,&arg))
		return FALSE; // エラー

	if (setsockopt(sockListener, SOL_SOCKET, SO_REUSEADDR, &val, sizeof(int)) < 0)
	{
		AddMessageLog(L"error in sockopt SO_REUSEADDR");
		return FALSE;
	}

	if (setsockopt(sockListener, SOL_SOCKET, SO_KEEPALIVE, &val, sizeof(int)) < 0)
	{
		AddMessageLog(L"error in sockopt SO_KEEPALIVE");
		return FALSE;
	}

	if (setsockopt(sockListener, SOL_SOCKET, SO_LINGER, &val, sizeof(int)) < 0)
	{
		AddMessageLog(L"error in sockopt SO_LINGER");
		return FALSE;
	}

	ZeroMemory(&svrAddr, sizeof(svrAddr));

	svrAddr.sin_family=AF_INET;
	svrAddr.sin_port=htons(g_nTcpPort);
	svrAddr.sin_addr.s_addr=htonl(INADDR_ANY);
	
	bind(sockListener,(SOCKADDR *)&svrAddr,sizeof(SOCKADDR_IN) ) ;
	
	listen(sockListener,5);

	return TRUE;
}

void CNetworkSession::InitSession(ptype_session psession)
{
	ZeroMemory(psession, sizeof(type_session));
}

void CNetworkSession::InitSesssionArray()
{
	for(int i=0;i<MAXLOGINUSER;i++)
	{
		ZeroMemory(&m_SessionArray[i].s, sizeof(m_SessionArray[i].s));
		m_SessionArray[i].flag=0;
		m_SessionArray[i].sockNo=i;
	}
	ZeroMemory(SockNoToUserNo, sizeof(int)*MAXLOGINUSER);

	m_dwUserCount = 0;
}

void CNetworkSession::ResetSession(WORD idx)
{
	if(idx>=g_nMaxLoginNum) return;
	m_SessionArray[idx].flag = 0;
	m_SessionArray[idx].s.connect_state = CONN_STATE_EMPTY;
	m_SessionArray[idx].s.HP_c = 0;
	m_SessionArray[idx].s.name[0] = NULL;
	m_SessionArray[idx].s.name_len = 0;
	m_SessionArray[idx].s.obj_state = OBJ_STATE_EMPTY;

//	BYTE ent = m_SessionArray[idx].s.entity;
//	m_SessionArray[idx].s.
//	ZeroMemory(&m_SessionArray[idx].s, sizeof(type_session));
//	m_SessionArray[idx].s.entity = ent;
}

// pFirstIndex = 検索用のindex
ptype_session CNetworkSession::GetSessionFirst(int* pFirstIndex)
{
//	if(connUserNum==0) return NULL;
	if (!m_dwUserCount) return NULL;
	*pFirstIndex = -1;
	for(int i=0;i<g_nMaxLoginNum;i++){
		if(m_SessionArray[i].flag==1){
//			m_dwArrayIndex=i;
//			m_dwArrayCount=1;
			*pFirstIndex = i;
			return &m_SessionArray[i].s;
		}
	}
	return NULL;
}

ptype_session CNetworkSession::GetSessionNext(int* pSearchIndex)
{
	int nIndex = *pSearchIndex+1;
	if (nIndex <= 0) return NULL;
//	if(connUserNum<=m_dwArrayCount) return NULL;

	for(;nIndex<g_nMaxLoginNum;nIndex++){
		if(m_SessionArray[nIndex].flag==1){
//			m_dwArrayIndex=i;
//			m_dwArrayCount+=1;
			*pSearchIndex = nIndex;
			return &m_SessionArray[nIndex].s;
		}
	}
	return NULL;
}

ptype_session CNetworkSession::CreateSession(int sock, INT addr, WORD port)
{
	if (sock<0) return NULL;

	int i=0;
	for(i=0;i<g_nMaxLoginNum;i++)
	{
		if(m_SessionArray[i].flag==0)
		{
			m_SessionArray[i].flag=1;
			ZeroMemory(&m_SessionArray[i].s, sizeof(type_session));
			break;
		}
	}
	// 全セッション使用中
	if (i>=g_nMaxLoginNum)
	{
		WCHAR log[32];
		SafePrintf(log, 32, L"Over MaxSessionUser:%d",i);
		AddMessageLog(log);
		return NULL;
	}

	ptype_session sess = &m_SessionArray[i].s;

	sess->sock		= sock;						// ソケットアドレスを記録
	sess->port			= port;
	sess->addr		= addr;
	sess->connect_state	= (BYTE)CONN_STATE_EMPTY;
//	sess->clientver       = 0;
//	sess->frame_count = 0;			// フレームカウンタ
	sess->obj_no = MAXUSERNUM;
	sess->sess_index = i;
	sess->obj_state = OBJ_STATE_EMPTY;	// オブジェクト状態
//	sess->bx = 0;
//	sess->bz = 0;
//	sess->master = 0;
//	sess->name[0] = NULL;

//	ZeroMemory(sess->items, sizeof(BYTE)*MAX_ITEM_COUNT);
//	sess->master = 0;

	m_dwUserCount++;
	WCHAR log[80];
	SafePrintf(log, 80, L"CreateSession::obj_no:%d/UserCount:%d",i, m_dwUserCount);
	AddMessageLog(log);
	return &m_SessionArray[i].s;
}

//> 一個切断
void CNetworkSession::ClearSession(ptype_session psession)
{
	if (!psession)	return;

	m_dwUserCount--;

	// セッションを切断
	shutdown(psession->sock,2);
	closesocket(psession->sock);

	WCHAR clrlog[64];
	SafePrintf(clrlog, 64, L"ClearSession ObjNo:%d", psession->sess_index);
	AddMessageLog(clrlog);

	// セッション編集
	ResetSession(psession->sess_index);
}
//< 一個切断

void CNetworkSession::ClearAllSession()
{
	for(int i=0;i<MAXLOGINUSER;i++)
	{
		if(m_SessionArray[i].flag)
		{
			ptype_session sess = &m_SessionArray[i].s;
			ClearSession(sess);
		}
	}
}

int CNetworkSession::SetConnectUserName(ptype_session session, char* msg, int len)
{
	if (!SafeMemCopy(&session->name[0], &msg[0], len, MAX_USER_NAME*sizeof(WCHAR)))
		return 0;

	session->name[len] = '\0';
	session->name_len = len;
	return 1;
}

// 既に存在するユーザ名か確認
BOOL CNetworkSession::IsUniqueUserName(WCHAR* name)
{
	int nameLen = wcslen(name)*sizeof(WCHAR);
	for(int i=0;i<g_nMaxLoginNum;i++)
	{
		if(m_SessionArray[i].flag)
		{
			ptype_session sess = &m_SessionArray[i].s;
			// ログイン中の人
			if (!sess->name[0])
				continue;
			// 文字列長で確認
			if (nameLen != sess->name_len)
				continue;
			// 文字列比較
			WCHAR sess_name[MAX_USER_NAME+1];
			common::session::GetSessionName(sess, sess_name);
			if (wcscmp(sess_name, name) == 0)
				return FALSE;
		}
	}

	return TRUE;
}
