#pragma once
#include "PICkitFunctions.h"

#define MAX_LINE_LEN 80

class CImportExportHex
{
public:
	CImportExportHex(void);
	~CImportExportHex(void);

	bool ImportHexFile(_TCHAR* filePath, CPICkitFunctions* picFuncs);
	bool ImportBINFile(_TCHAR* filePath, CPICkitFunctions* picFuncs);
	int ParseHex(_TCHAR* characters, int length);
	bool ExportHexFile(_TCHAR* filePath, CPICkitFunctions* picFuncs);
	bool ExportBINFile(_TCHAR* filePath, CPICkitFunctions* picFuncs);
	unsigned char computeChecksum(_TCHAR* fileLine);

protected:
	

};
