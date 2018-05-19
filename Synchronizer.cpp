#include "main.h"
#include "ext.h"

// ���[�U�[�L�����N�^�[�𓯊�������X���b�h
DWORD __stdcall Thread_Synchronizer(LPVOID param)
{
//	WCHAR logmsg[MAX_LOG_BUFFER];
//	CSyncProc* pSyncProc = g_pSyncRoom;
	CCriticalSection*	pCriticalSection = ((CCriticalSection*) param);
	CNetworkSession*	pNWSess = g_pNetSess;

	timeBeginPeriod(1);
	for(;;)
	{
		//>> Frame move		
		// �w�莞�Ԉȏ�o�߂Ȃ�Q�[�������J�n
		static DWORD dwNextTick = FRAME_RATE;
		DWORD dwNowTick = 0;
		while(dwNextTick-3 > (dwNowTick=timeGetTime()))
			Sleep(1);
		while(dwNextTick > (dwNowTick=timeGetTime()))
			Sleep(0);
		dwNextTick = dwNowTick+FRAME_RATE;
		// ���݂̎��Ԃ��Ō�ɍX�V�������Ԃɂ���
		
		//> ��������
		BOOL bPacket = FALSE;
		switch (g_eGamePhase)
		{
		case GAME_PHASE_ROOM:
			pCriticalSection->EnterCriticalSection_Session(L'F');
			bPacket = g_pSyncRoom->Frame();
			pCriticalSection->LeaveCriticalSection_Session();
			if (bPacket)
			{
				g_pCriticalSection->EnterCriticalSection_Packet(L'Y');
				g_pPacketQueue->Enqueue(g_pSyncRoom->DequeuePacket());
				g_pCriticalSection->LeaveCriticalSection_Packet();
			}
			break;
		case GAME_PHASE_LOAD:
			bPacket = g_pSyncLoad->Frame();
			if (bPacket)
			{
				g_pCriticalSection->EnterCriticalSection_Packet(L'U');
				g_pPacketQueue->Enqueue(g_pSyncLoad->DequeuePacket());
				g_pCriticalSection->LeaveCriticalSection_Packet();
			}
			if (g_pSyncLoad->IsNextPhase())
			{
				g_pPPPhase->SetGamePhase(GAME_PHASE_MAIN);
				if (!g_pPPMain->Init(pNWSess, g_pPPLoad))
				{
					AddMessageLog(L"Main Packet Init error\n");
					return 0;
				}
				g_pPPPhase = g_pPPMain;
				g_eGamePhase = GAME_PHASE_MAIN;
				if (! g_pSyncMain->Init(pNWSess, g_pFiler, g_pPPMain))
					g_pSyncMain->GameEnd();

				g_pCriticalSection->EnterCriticalSection_Packet(L'I');
				g_pPacketQueue->Enqueue(g_pSyncMain->DequeuePacket());
				g_pCriticalSection->LeaveCriticalSection_Packet();
			}
			break;
		case GAME_PHASE_MAIN:
			bPacket = g_pSyncMain->Frame();
			if (bPacket)
			{
				g_pCriticalSection->EnterCriticalSection_Packet(L'O');
				g_pPacketQueue->Enqueue(g_pSyncMain->DequeuePacket());
				g_pCriticalSection->LeaveCriticalSection_Packet();
			}
			break;
		case GAME_PHASE_RESULT:

			break;
		}
		//< ��������
		Sleep(1);											// �K�� CPU �ɋx�݂�����
	}  // for ���[�v�I��
	timeEndPeriod(1);										// �^�C�}�[�̐��x��߂�
	return 0;
}
