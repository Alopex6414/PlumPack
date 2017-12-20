/*
*     COPYRIGHT NOTICE
*     Copyright(c) 2017, Team Shanghai Dream Equinox
*     All rights reserved.
*
* @file		PlumPack.h
* @brief	This Program is PlumPack DLL Project.
* @author	Alopex/Helium
* @version	v1.00a
* @date		2017-12-16	v1.00a	alopex	Create Project
*/
#pragma once

#ifndef __PLUMPACK_H_
#define __PLUMPACK_H_

//Include Windows Header File
#include <Windows.h>

//Include C/C++ Run Header File
#include <stdio.h>

//Include Crypt File
#include "PlumCipherA.h"
#include "PlumCrypt.h"

//Macro Definition
#ifdef	PLUMPACK_EXPORTS
#define PLUMPACK_API	__declspec(dllexport)
#else
#define PLUMPACK_API	__declspec(dllimport)
#endif

//Struct Definition
typedef struct
{
	char cFileName[24];
	char cCodeAuthor[8];
	DWORD dwFileAllNum;
	DWORD dwFileAllSize;
} PlumPackInfo;

//Class Definition
class PLUMPACK_API CPlumPack
{
private:


public:
	CPlumPack();
	virtual ~CPlumPack();

	virtual void PlumPackFileA(const char* pSrcArr[], int nArrSize, const char* pDest);
	virtual void PlumUnPackFileA(const char* pSrc, const char* pDest);
};

#endif