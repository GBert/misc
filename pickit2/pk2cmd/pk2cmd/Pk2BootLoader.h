#pragma once
#include "PICkitFunctions.h"

#define MAX_BLLINE_LEN 80

class Pk2BootLoader
{
public:
	Pk2BootLoader(void);
	~Pk2BootLoader(void);
	bool ReadHexAndDownload(_TCHAR* fileName, CPICkitFunctions* picFuncs, int unitIndex);
	bool ReadHexAndVerify(_TCHAR* fileName, CPICkitFunctions* picFuncs);
	int ParseHex(_TCHAR* characters, int length);
};
