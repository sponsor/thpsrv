#include "main.h"
#include "ext.h"

BOOL recvall(int sock, char* pkt, int* recvsize, int bufsize);
int g_nSessionCount=0;
int g_bRestartFlg=FALSE;

DWORD __stdcall Thread_PacketReceiver(PVOID param)
{
	int		nTcpSock=0;
	const int c_nPacketRecvCount = 8;
	const int c_nPacketStockCount = 64;
	BYTE	arMsg[MAX_PACKET_SIZE*c_nPacketRecvCount];
	BYTE	arMsgto[c_nPacketStockCount][MAX_PACKET_SIZE];
	WORD arSize[c_nPacketStockCount];
	int nPacketProcIndex = 0;
	int nPacketStockIndex = 0;
	BOOL bStock = FALSE;

	int		nHighest_fd;
	int		nConnectSocket = 0;
	int		nPacketCount=0;
//	int		nIndex;
//	int		n;

	WORD	wRecvEnd;
	WORD	wEnd = PEND;
	fd_set	rfds, efds;
	int nDivIndex;
	int nRecvSize = 0;
	int nRecvStockSize = 0;
	BOOL bPacket;
	
	ptype_session	pObjSess;
	ptype_session	pConnectObjSess;

	g_nSessionCount = 0;
	CCriticalSection*	pCriticalSection = ((CCriticalSection*) param);
	CNetworkSession*	pNWSess = g_pNetSess;

	nTcpSock = pNWSess->GetSockListener();

//	int nUserCount = 0;
	int newUser = 0;

	while(TRUE)
	{
		FD_ZERO(&rfds);
		FD_ZERO(&efds);
		nHighest_fd = nTcpSock;
		FD_SET(nTcpSock, &rfds);

		//> 接続ユーザーのFD_SET
		pCriticalSection->EnterCriticalSection_Session(L'u');
		static int nSearchIndex = 0;
		for(pConnectObjSess=pNWSess->GetSessionFirst(&nSearchIndex);
			pConnectObjSess;
			pConnectObjSess=pNWSess->GetSessionNext(&nSearchIndex))
		{
			nConnectSocket = pConnectObjSess->sock;
			FD_SET(nConnectSocket,&rfds);
			FD_SET(nConnectSocket,&efds);//extra receive

			if (nConnectSocket>nHighest_fd)
				nHighest_fd = nConnectSocket;
		}
		pCriticalSection->LeaveCriticalSection_Session();

		// 受信待ち
		if (select(nHighest_fd+1,&rfds,NULL,&efds,(struct timeval *)0)<0)
		{
			if (errno!=/*EINTR*/0)
				continue;
		}

		if ( (newUser = NewUserSession(&rfds, nTcpSock, pNWSess, &pObjSess)) == -1)
			continue;
		g_nSessionCount += newUser;
#ifdef _DEBUG
		if (newUser)
		{
			WCHAR newuserlog[32];
			SafePrintf(newuserlog, 32, L"NewUser(usercount=%d)", g_nSessionCount);
			AddMessageLog(newuserlog);
		}

#endif
		//> 接続済みユーザとの処理
//		int nSearchIndex = 0;
		for(pObjSess=pNWSess->GetSessionFirst(&nSearchIndex);
			pObjSess;
			pObjSess=pNWSess->GetSessionNext(&nSearchIndex))
		{
			// 切断したいセッション
			if (pObjSess->connect_state==CONN_STATE_DESTROY)
			{
				WCHAR log[64];
				WCHAR name[MAX_USER_NAME+1];
				common::session::GetSessionName(pObjSess, name);
				SafePrintf(log, 64, L"disconnect:%s", name);
				AddMessageLog(log);
				DisconnectSession(pObjSess);
//				g_nSessionCount--;
				continue;
			}
			nConnectSocket = pObjSess->sock;

			//> 受信可能なユーザー
			if (FD_ISSET(nConnectSocket,&rfds))
			{
				memset(&arMsg[nRecvStockSize],0,  sizeof(arMsg)-nRecvStockSize);
				//> パケット受信
//				if((nRecvSize = recv(nConnectSocket, arMsg, MAX_PACKET_SIZE, 0))<=0)
				if (!recvall(nConnectSocket, (char*)arMsg, &nRecvSize, sizeof(arMsg)-nRecvStockSize))
				{
					// 突発的にアクセス中断
					WCHAR log[64];
					WCHAR name[MAX_USER_NAME+1];
					common::session::GetSessionName(pObjSess, name);
					SafePrintf(log, 64, L"abort disconnect in receive:%s", name);
					AddMessageLog(log);
					DisconnectSession(pObjSess);
					continue;
				}
				//< パケット受信
				
				//> パケットチェック
				// サイズ０受信
				if (nRecvSize == 0)
				{
					AddMessageLog(L"パケットサイズ0受信\n");
					continue;
				}

				// 合計受信サイズが0ならサイズ設定
				bStock = (nRecvStockSize>0);
				if (!bStock)
					memcpy(&arSize[nPacketStockIndex],&arMsg[0],2);	// パケットサイズ取得

				nPacketCount=0;					//packet count reset

				// パケットストックしてない
				if (!bStock)
				{
					// 1パケットのみ受信
					if( nRecvSize==arSize[nPacketStockIndex])
					{
						memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
						memcpy(arMsgto[nPacketStockIndex],&arMsg[0],arSize[nPacketStockIndex]);
						memcpy(&wRecvEnd,&arMsg[arSize[nPacketStockIndex] -2],2);
						if(wRecvEnd != wEnd)
						{
							WCHAR msglog[64];
							SafePrintf(msglog, 64, L"パケット末尾にエラー...size=%d\n", (int)arSize[0]);
							AddMessageLog(msglog);
							continue;
						}
						nRecvStockSize = 0;	// ストックなし
						nPacketCount=1;
						nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
					}
					else	// 複数パケット受信か1パケット以下受信
					{
						//> パケット分割
						nPacketCount=0;
						nDivIndex=0;
						nRecvStockSize = nRecvSize;
						for(;;)
						{
							if(nRecvStockSize <= 0)	break;
							if(arSize[nPacketStockIndex] > nRecvStockSize)	// 受信バッファよりパケットが大きい
							{
								// 受信バッファを先頭に移動する必要がある
								memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
								memcpy(arMsgto[nPacketStockIndex], &arMsg[nDivIndex], nRecvStockSize);
								break;
							}
							memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
							memcpy(&arMsgto[nPacketStockIndex][0],&arMsg[nDivIndex],arSize[nPacketStockIndex]);
							memcpy(&wRecvEnd,&arMsgto[nPacketStockIndex][ (int)(arSize[nPacketStockIndex] -2)],2);
							if(wRecvEnd != wEnd)
							{
								WCHAR msglog[MAX_MSG_BUFFER+1];
								SafePrintf(msglog, MAX_MSG_BUFFER+1, L"パケット末尾にエラー(分割中エラー)...size=%d/index=%d/stock=%d", arSize[nPacketStockIndex],nDivIndex,nRecvStockSize);
								nRecvStockSize = 0;
								break;
							}
							nDivIndex += arSize[nPacketStockIndex];
							nRecvStockSize -= arSize[nPacketStockIndex];
							nPacketCount++;
							nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
							// 次パケットサイズ
							memcpy(&arSize[nPacketStockIndex],&arMsg[nDivIndex],2);
						}
						//< パケット分割
					}
				}
				else	// パケットストック中
				{
					// ストック+受信サイズが1パケット分
					if( (nRecvStockSize+nRecvSize)==arSize[nPacketStockIndex])
					{
						memcpy(&arMsgto[nPacketStockIndex][nRecvStockSize],arMsg,arSize[nPacketStockIndex]-nRecvStockSize);
						memcpy(&wRecvEnd,&arMsgto[nPacketStockIndex][arSize[nPacketStockIndex] -2],2);
						if(wRecvEnd != wEnd)
						{
							WCHAR msglog[64];
							SafePrintf(msglog, 64, L"パケット末尾にエラー...size=%d\n", (int)arSize[0]);
							AddMessageLog(msglog);
							continue;
						}
						nRecvStockSize = 0;	// ストックなし
						nPacketCount=1;
						nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
					}
					else	// パケット分割
					{
						nPacketCount=0;
						nDivIndex=0;
						int nRecvStockIndex = nRecvStockSize;
						nRecvStockSize = nRecvSize;
						for (;;)
						{
							if(nRecvStockSize <= 0)	break;
							if ((arSize[nPacketStockIndex]-nRecvStockIndex) > (nRecvStockSize))	// 受信バッファよりパケットが大きい
							{
								// 受信バッファを先頭に移動する必要がある
								memcpy(&arMsgto[nPacketStockIndex][nRecvStockIndex], &arMsg[nDivIndex], nRecvStockSize);
								nRecvStockSize += nRecvStockIndex;
								break;
							}
//							memset(arMsgto[nPacketStockIndex],0,MAX_PACKET_SIZE);
							memcpy(&arMsgto[nPacketStockIndex][nRecvStockIndex],&arMsg[nDivIndex],arSize[nPacketStockIndex]-nRecvStockIndex);
							memcpy(&wRecvEnd,&arMsgto[nPacketStockIndex][ (int)(arSize[nPacketStockIndex] -2)],2);
							if(wRecvEnd != wEnd)
							{
								WCHAR msglog[MAX_MSG_BUFFER+1];
								SafePrintf(msglog, MAX_MSG_BUFFER+1, L"パケット末尾にエラー(分割中エラー)...size=%d/index=%d/stock=%d", arSize[nPacketStockIndex],nDivIndex,nRecvStockSize);
								nRecvStockSize = 0;
								break;
							}
							nDivIndex += arSize[nPacketStockIndex];
							nRecvStockIndex = 0;
							nRecvStockSize -= arSize[nPacketStockIndex];
							nPacketCount++;
							nPacketStockIndex = (nPacketStockIndex+1)%c_nPacketStockCount;
							// 次パケットサイズ
							memcpy(&arSize[nPacketStockIndex],&arMsg[nDivIndex],2);
						}
					}
				}
				//< パケットチェック

				bPacket = FALSE;
				pCriticalSection->EnterCriticalSection_Session(L'i');
				// パケット処理
				for(int n=0;n<nPacketCount;n++)
				{
					bPacket |= PacketProc(pNWSess, (BYTE*)arMsgto[nPacketProcIndex], pObjSess, arSize[nPacketProcIndex]);
					nPacketProcIndex = (nPacketProcIndex+1)%c_nPacketStockCount;
				}
				pCriticalSection->LeaveCriticalSection_Session();

				if (bPacket)
				{
					pCriticalSection->EnterCriticalSection_Packet(L'R');
					PacketEnqueue();
					pCriticalSection->LeaveCriticalSection_Packet();
				}
			}//if (FD_ISSET(csocket,&rfds)) end 接続初期化後のパケット受信終了

			// エラー
			/*g_nSessionCount += */ErrorUserSession(nConnectSocket, &efds, nRecvSize, pNWSess, pObjSess);
		
			// 接続ユーザが一人も居ない場合
			if (!g_pNetSess->CalcAuthedUserCount())
			{
				g_pPPRoom->Init(g_pNetSess, g_pPPPhase, g_pPPPhase->GetStageIndex(), g_bytIniRuleFlg, g_nActTimeLimit);
				g_pPPPhase = g_pPPRoom;
				UpdateWMCopyData();
			}
		}
	}// endless loop
	return 0;
}

BOOL recvall(int sock, char* pkt, int* recvsize, int bufsize)
{
	int nBufferSize = bufsize;
	*recvsize=0;
	int nRecvd = 0;
	//> パケット受信
	while (SOCKET_ERROR != (nRecvd = recv(sock, &pkt[*recvsize], nBufferSize, 0)))
	{
		// ソケットが閉じられた
		if (!nRecvd)
			return FALSE;
//		// 受信0
//		if (!nRecvd)
//			break;
		// 受信残りがある
		*recvsize += nRecvd;
		nBufferSize -= nRecvd;
	}
	DWORD dwErrCode = WSAGetLastError();
	// エラー
	if(dwErrCode && WSAEWOULDBLOCK != dwErrCode)
	{
#if ADD_WSAERROR_LOG
		if (g_bLogFile)
		{
			WCHAR pc[16];
			SafePrintf(pc, 16, L"LastErr:%d", dwErrCode);
			AddMessageLog(pc);
		}
#endif
		return FALSE;
	}
	//< パケット受信
	return TRUE;
}

BOOL PacketProc(CNetworkSession *pNWSess, BYTE* msg, ptype_session sess, INT msgsize)
{
	BOOL bAuthPacket = g_pPPAuth->PacketProc(msg, sess);
	BOOL bChatPacket = g_pPPChat->PacketProc(msg, sess);
	BOOL bPhasePacket = g_pPPPhase->PacketProc(msg, sess);	// Room,Load,Main,Return

	E_STATE_GAME_PHASE eGamePhase = g_pPPPhase->GetGamePhase();
	if (g_eGamePhase != eGamePhase)
	{
		g_eGamePhase = eGamePhase;
		switch (eGamePhase)
		{
		case GAME_PHASE_ROOM:
			bPhasePacket |= g_pPPRoom->Init(g_pNetSess, g_pPPPhase, g_nDefaultStageID, g_bytIniRuleFlg, g_nActTimeLimit);
			bPhasePacket |= g_pPPRoom->PacketProc(msg, sess);
			g_pPPPhase = g_pPPRoom;
			break;
		case GAME_PHASE_LOAD:
			bPhasePacket |= g_pPPLoad->Init(g_pNetSess, g_pPPPhase);
			bPhasePacket |= g_pPPLoad->PacketProc(msg, sess);
			g_pSyncLoad->Init(g_pNetSess);
			g_pPPPhase = g_pPPLoad;
			break;
		case GAME_PHASE_MAIN:
			bPhasePacket |= g_pPPMain->Init(g_pNetSess, g_pPPPhase);
			bPhasePacket |= g_pPPMain->PacketProc(msg, sess);
			g_pPPPhase = g_pPPMain;
			break;
		case GAME_PHASE_RESULT:
			bPhasePacket |= g_pPPResult->Init(g_pNetSess, g_pPPPhase);
			bPhasePacket |= g_pPPResult->PacketProc(msg, sess);
			break;
		case GAME_PHASE_RETURN:
			break;
		}
	}

	if (g_bLogFile)
	{
		int hd = msg[PACKET_HEADER_INDEX];
		WCHAR log[32];
		SafePrintf(log, 32, L"rcvhd:%d", hd);
		AddMessageLog(log);
	}

	return (bAuthPacket|bChatPacket|bPhasePacket);
}

void PacketEnqueue()
{
	g_pPacketQueue->Enqueue(g_pPPAuth->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPChat->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPRoom->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPLoad->DequeuePacket());
	g_pPacketQueue->Enqueue(g_pPPMain->DequeuePacket());
}

// 切断処理
void DisconnectSession(ptype_session sess)
{
	// セッションクリア済み確認
	if (!g_pNetSess->GetFlag(sess->sess_index))
		return;
	g_pCriticalSection->EnterCriticalSection_Session(L'o');
//	int sess_no = sess->obj_no;

	BOOL bAuthPacket = g_pPPAuth->DisconnectSession(sess);
	BOOL bChatPacket = g_pPPChat->DisconnectSession(sess);
	BOOL bPhasePacket = g_pPPPhase->DisconnectSession(sess);	// Room,Load,Main,Return

	// ユーザリストから削除
	DeleteUserList(sess);
	g_pNetSess->ClearSession(sess);

	g_pCriticalSection->LeaveCriticalSection_Session();

	g_pCriticalSection->EnterCriticalSection_Packet(L'R');
	
	// 送信パケットありか
	if (bAuthPacket)
		g_pPacketQueue->Enqueue(g_pPPAuth->DequeuePacket());
	if (bChatPacket)
		g_pPacketQueue->Enqueue(g_pPPChat->DequeuePacket());
	if (bPhasePacket)
		g_pPacketQueue->Enqueue(g_pPPPhase->DequeuePacket());	// Room,Load,Main,Return

	g_nSessionCount--;

	WCHAR disc[40];
	SafePrintf(disc, 40, L"DiscSess(Count=%d)", g_nSessionCount);
	AddMessageLog(disc);
	g_pCriticalSection->LeaveCriticalSection_Packet();
	if (g_nSessionCount == 0)
	{
#ifdef _DEBUG
		Sleep(100);
#else
		MessageBox(g_hDlg, L"サーバをリセットします",L"接続人数0",MB_OK); 
#endif
		g_bRestartFlg = TRUE;
		SendMessage(g_hWnd, WM_NULL, NULL, NULL);
	}

	UpdateWMCopyData();
}

int ErrorUserSession(int nConnectSocket, fd_set *efds, int nRecvSize, CNetworkSession *pNWSess, ptype_session sess)
{
	// エラー
	if (FD_ISSET(nConnectSocket,efds))
	{
		AddMessageLog(L"disconnect in efds");
//		return -1;

		if(nRecvSize==1)
			AddMessageLog(L"DISCONNECT DATA SAVE OK----------");
		else
			AddMessageLog(L"DISCONNECT DATA SAVE FAIL--------");

		DisconnectSession(sess);
		return -1;
	}//end for linked list search loop

	return 0;
}

int NewUserSession(fd_set *rfds, int nTcpSock, CNetworkSession *pNWSess, pptype_session ppSess)
{
	int ret = 0;
	//> 新規のユーザの受信があったか
	//  セッション作成
	if (FD_ISSET(nTcpSock, rfds))
	{
		int				    nTempSock;
		struct sockaddr_in 	tAddr;
		int					nAddr_len = sizeof(tAddr);

		// クライアントからの接続要求を受け入れ，通信路を確保する
		if ((nTempSock = accept(nTcpSock,(struct sockaddr *)&tAddr,&nAddr_len))<0)
		{
	        if(errno == WSAEINTR)
	            ret = -1;              /* retry accept : back to while() */
	        else
				AddMessageLog(L"error with accept");
			ret = -1;
		}
		
		*ppSess = NULL;
		//> ユーザー情報同期
		g_pCriticalSection->EnterCriticalSection_Session(L'p');
		if (pNWSess->CalcAuthedUserCount() < g_nMaxLoginNum)
		{
			// ｾｯｼｮﾝ生成
			*ppSess = pNWSess->CreateSession(nTempSock, ntohl(tAddr.sin_addr.s_addr), ntohs(tAddr.sin_port));
		}
		//< ユーザー情報同期
		g_pCriticalSection->LeaveCriticalSection_Session();
		// 失敗なら切断
		if (!(*ppSess))
		{
			AddMessageLog(L"error with session create");
			shutdown(nTempSock, 2);
			closesocket(nTempSock);
			ret = -1;
		}
		ret++;
	}
	
	return ret;
}

