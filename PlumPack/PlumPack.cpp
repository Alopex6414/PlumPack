/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumPack.cpp
* @brief	This Program is PlumPack DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2017-12-16	v1.00a	alopex	Create Project
*/
#include "PlumPack.h"

//PlumPack������Ϸ��Դ���/���(Pack/UnPack)

//------------------------------------------------------------------
// @Function:	 CPlumPack()
// @Purpose: CPlumPack���캯��
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumPack::CPlumPack()
{
}

//------------------------------------------------------------------
// @Function:	 ~CPlumPack()
// @Purpose: CPlumPack��������
// @Since: v1.00a
// @Para: None
// @Return: None
//------------------------------------------------------------------
CPlumPack::~CPlumPack()
{
}

//-----------------------------------------------------------------------------
// @Function:	 PlumPackFile(const char* pSrcArr[], int nArrSize, char* pDest)
// @Purpose: CPlumPack�������
// @Since: v1.00a
// @Para: const char* pSrcArr[]			//���Դ�ļ�������
// @Para: int nArrSize					//���Դ�ļ�����
// @Para: const char* pDest				//���Ŀ���ļ���
// @Return: None
//-----------------------------------------------------------------------------
void CPlumPack::PlumPackFileA(const char* pSrcArr[], int nArrSize, const char* pDest)
{
	HANDLE hFileSrc;
	HANDLE hFileDest;
	HANDLE hFileMsg;

	//��Ŀ���ļ�
	hFileDest = CreateFileA(pDest, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileDest == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFileDest);
		return;
	}

	//����Ŀ���ļ���Ϣ
	char* pTemp;
	char* pDestName;
	int nCount;

	pTemp = strrchr((char*)pDest, '\\');
	if (pTemp != NULL)//���ҳɹ�
	{
		nCount = strlen(++pTemp);
		pDestName = (char*)malloc((nCount + 1)*sizeof(char));
		memset(pDestName, 0, (nCount + 1));
		strcpy_s(pDestName, (nCount + 1), pTemp);
	}
	else//����ʧ��
	{
		nCount = strlen(pDest);
		pDestName = (char*)malloc((nCount + 1)*sizeof(char));
		memset(pDestName, 0, (nCount + 1));
		strcpy_s(pDestName, (nCount + 1), pTemp);
	}

	//д�����ļ���Ϣ
	int i = 0;
	PlumPackInfo sPackMsg;
	DWORD dwRealWritePackMsg;

	memset(&sPackMsg, 0, sizeof(sPackMsg));
	for (i = 0, pTemp = pDestName; i < sizeof(sPackMsg.cFileName) && *pTemp != '\0'; ++i, ++pTemp)
	{
		*(sPackMsg.cFileName + i) = *pTemp;
	}
	memcpy(&(sPackMsg.cCodeAuthor), "alopex", sizeof("alopex"));
	sPackMsg.dwFileAllNum = nArrSize;
	//sPackMsg.dwFileAllSize = 0;
	WriteFile(hFileDest, &sPackMsg, sizeof(sPackMsg), &dwRealWritePackMsg, NULL);

	if (pDestName) free(pDestName);

	//Ԥ����Ӧ�ֽ�����ַ
	DWORD* pFileAddressArr = NULL;

	pFileAddressArr = new DWORD[nArrSize];
	memset(pFileAddressArr, 0, nArrSize*sizeof(DWORD));
	WriteFile(hFileDest, pFileAddressArr, nArrSize*sizeof(DWORD), &dwRealWritePackMsg, NULL);

	//ѭ����
	for (int i = 0; i < nArrSize; ++i)
	{
		//��ȡ��ǰ�ļ�ָ��λ��
		*(pFileAddressArr + i) = SetFilePointer(hFileDest, NULL, NULL, FILE_CURRENT);

		//��Դ�ļ�
		hFileSrc = CreateFileA(*(pSrcArr + i), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFileSrc == INVALID_HANDLE_VALUE)
		{
			CloseHandle(hFileSrc);
			CloseHandle(hFileDest);
			return;
		}

		//�����ļ�
		char* pTemp;
		char* pMsg;
		int nCount;

		pTemp = (char*)(*(pSrcArr + i));
		nCount = strlen(pTemp);
		pMsg = (char*)malloc((nCount + 1)*sizeof(char));
		strcpy_s(pMsg, (nCount + 1), pTemp);
		pTemp = strrchr(pMsg, '.');
		*pTemp = '.';
		*(pTemp + 1) = 'm';
		*(pTemp + 2) = 's';
		*(pTemp + 3) = 'g';
		*(pTemp + 4) = '\0';

		//���ļ���Ϣ
		hFileMsg = CreateFileA(pMsg, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

		if (hFileMsg == INVALID_HANDLE_VALUE)
		{
			if (pMsg) free(pMsg);
			CloseHandle(hFileMsg);
			CloseHandle(hFileSrc);
			CloseHandle(hFileDest);
			return;
		}

		//д��Msg
		PlumFileInfo sMsg;
		DWORD dwMsgRead;
		DWORD dwMsgWrite;

		ReadFile(hFileMsg, &sMsg, sizeof(sMsg), &dwMsgRead, NULL);
		WriteFile(hFileDest, &sMsg, sizeof(sMsg), &dwMsgWrite, NULL);

		//д���ļ�
		unsigned char* pSrcArr = NULL;
		unsigned char* pDestArr = NULL;
		int nSrcSize = CRYPTARRAYSIZE;
		int nDestSize = CRYPTARRAYSIZE;
		int nReadSize = 0;
		int nWriteSize = 0;
		DWORD dwRealRead = 0;
		DWORD dwRealWrite = 0;

		pSrcArr = (unsigned char*)malloc(nSrcSize*sizeof(unsigned char));
		pDestArr = (unsigned char*)malloc(nDestSize*sizeof(unsigned char));

		for (;;)
		{
			memset(pSrcArr, 0, nSrcSize);
			memset(pDestArr, 0, nDestSize);

			ReadFile(hFileSrc, pSrcArr, nSrcSize, &dwRealRead, NULL);
			if (dwRealRead == 0) break;

			memcpy(pDestArr, pSrcArr, dwRealRead);

			WriteFile(hFileDest, pDestArr, dwRealRead, &dwRealWrite, NULL);
			if (dwRealWrite == 0) break;
		}

		if (pMsg) free(pMsg);
		if (pSrcArr) free(pSrcArr);
		if (pDestArr) free(pDestArr);

		CloseHandle(hFileMsg);
		hFileMsg = NULL;
		CloseHandle(hFileSrc);
		hFileSrc = NULL;
	}

	//����д���ļ���ַ��Ϣ
	SetFilePointer(hFileDest, sizeof(sPackMsg), NULL, FILE_BEGIN);
	WriteFile(hFileDest, pFileAddressArr, nArrSize*sizeof(DWORD), &dwRealWritePackMsg, NULL);

	delete[] pFileAddressArr;

	CloseHandle(hFileDest);
	hFileDest = NULL;
}

//-----------------------------------------------------------------------------
// @Function:	 PlumUnPackFile(const char* pSrc, char* pDest)
// @Purpose: CPlumPack�������
// @Since: v1.00a
// @Para: const char* pSrc				//���Դ�ļ���
// @Para: char* pDest					//���Ŀ���ļ�·��(�����ļ�����!)
// @Return: None
//-----------------------------------------------------------------------------
void CPlumPack::PlumUnPackFileA(const char* pSrc, const char* pDest)
{
	HANDLE hFileSrc;

	//��Դ�ļ�
	hFileSrc = CreateFileA(pSrc, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);

	if (hFileSrc == INVALID_HANDLE_VALUE)
	{
		CloseHandle(hFileSrc);
		return;
	}

	//���������Ϣ
	PlumPackInfo sPackMsg;
	int	nFileAllNum;
	DWORD dwRealReadPackMsg;

	ReadFile(hFileSrc, &sPackMsg, sizeof(sPackMsg), &dwRealReadPackMsg, NULL);
	if (dwRealReadPackMsg == 0) return;

	nFileAllNum = sPackMsg.dwFileAllNum;//����ļ�����
	if (nFileAllNum == 0) return;

	//�洢�����ļ�ƫ�Ƶ�ַ
	char* pDestFileName = NULL;
	DWORD* pFileAddress = NULL;
	DWORD dwAddressNow = 0;
	DWORD dwFileReadSize = 0;

	pFileAddress = new DWORD[nFileAllNum];
	for (int i = 0; i < nFileAllNum; ++i)
	{
		ReadFile(hFileSrc, (pFileAddress + i), sizeof(DWORD), &dwRealReadPackMsg, NULL);
	}

	//ѭ����ȡ
	for (int i = 0; i < nFileAllNum; ++i)
	{
		//���ý���ļ���ַ
		SetFilePointer(hFileSrc, *(pFileAddress + i), NULL, FILE_BEGIN);

		//�����ļ���Ϣ
		PlumFileInfo sMsg; 
		char* pArray = NULL;
		int nDestSize = 0;
		int nFileNameSize = 0;
		ReadFile(hFileSrc, &sMsg, sizeof(sMsg), &dwRealReadPackMsg, NULL);
		dwFileReadSize = sMsg.dwCryptFileSize;
		nDestSize = strlen(pDest);
		nFileNameSize = strlen(sMsg.cFileName);
		pDestFileName = new char[nDestSize + nFileNameSize + 1];
		memset(pDestFileName, 0, (nDestSize + nFileNameSize + 1)*sizeof(char));
		memcpy(pDestFileName, pDest, nDestSize);
		memcpy(pDestFileName + nDestSize, sMsg.cFileName, nFileNameSize);

		dwAddressNow = SetFilePointer(hFileSrc, NULL, NULL, FILE_CURRENT);

		pArray = new char[dwFileReadSize];
		ReadFile(hFileSrc, pArray, dwFileReadSize, &dwRealReadPackMsg, NULL);

		dwAddressNow = SetFilePointer(hFileSrc, NULL, NULL, FILE_CURRENT);

		//����ļ�
		CPlumCrypt* pCrypt = new CPlumCrypt;

		pCrypt->PlumDeCryptFileInMemoryStoreInFileExA((const void*)dwAddressNow, sMsg, pDestFileName);

		delete[] pDestFileName;
		delete[] pArray;
		delete pCrypt;
	}


	delete[] pFileAddress;

	CloseHandle(hFileSrc);
}