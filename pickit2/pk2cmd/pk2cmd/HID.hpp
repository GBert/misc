// Borland C++ Builder
// Copyright (c) 1995, 2002 by Borland Software Corporation
// All rights reserved

// (DO NOT EDIT: machine generated header) 'Hid.pas' rev: 6.00

#ifndef HidHPP
#define HidHPP

#pragma delphiheader begin
#pragma option push -w-
#pragma option push -Vx
#include <Windows.hpp>	// Pascal unit
#include <SysInit.hpp>	// Pascal unit
#include <System.hpp>	// Pascal unit

//-- user supplied -----------------------------------------------------------

namespace Hid
{
//-- type declarations -------------------------------------------------------
typedef Word *PUsage;

typedef Word TUsage;

typedef unsigned THIDPKeyboardModifierState;

#pragma option push -b-
enum THIDVariantFlags { Range, NotRange };
#pragma option pop

struct HIDD_CONFIGURATION;
typedef HIDD_CONFIGURATION *PHIDDConfiguration;

#pragma pack(push, 4)
struct HIDD_CONFIGURATION
{
	void *cookie;
	unsigned size;
	unsigned RingBufferSize;
} ;
#pragma pack(pop)

typedef HIDD_CONFIGURATION  THIDDConfiguration;

struct HIDD_ATTRIBUTES;
typedef HIDD_ATTRIBUTES *PHIDDAttributes;

#pragma pack(push, 4)
struct HIDD_ATTRIBUTES
{
	unsigned Size;
	Word VendorID;
	Word ProductID;
	Word VersionNumber;
} ;
#pragma pack(pop)

typedef HIDD_ATTRIBUTES  THIDDAttributes;

typedef void *PHIDPPreparsedData;

typedef unsigned *PHIDPReportType;

typedef unsigned THIDPReportType;

typedef unsigned *PHIDPKeyboardDirection;

typedef unsigned THIDPKeyboardDirection;

struct USAGE_AND_PAGE;
typedef USAGE_AND_PAGE *PUsageAndPage;

#pragma pack(push, 2)
struct USAGE_AND_PAGE
{
	Word Usage;
	Word UsagePage;
} ;
#pragma pack(pop)

typedef USAGE_AND_PAGE  TUsageAndPage;

struct HIDP_BUTTON_CAPS;
typedef HIDP_BUTTON_CAPS *PHIDPButtonCaps;

#pragma pack(push, 4)
struct HIDP_BUTTON_CAPS
{
	Word UsagePage;
	Byte ReportID;
	Byte IsAlias;
	Word BitField;
	Word LinkCollection;
	Word LinkUsage;
	Word LinkUsagePage;
	Byte IsRange;
	Byte IsStringRange;
	Byte IsDesignatorRange;
	Byte IsAbsolute;
	unsigned Reserved[10];
	union
	{
		struct 
		{
			Word Usage;
			Word Reserved1;
			Word StringIndex;
			Word Reserved2;
			Word DesignatorIndex;
			Word Reserved3;
			Word DataIndex;
			Word Reserved4;
			
		};
		struct 
		{
			Word UsageMin;
			Word UsageMax;
			Word StringMin;
			Word StringMax;
			Word DesignatorMin;
			Word DesignatorMax;
			Word DataIndexMin;
			Word DataIndexMax;
			
		};
		
	};
} ;
#pragma pack(pop)

typedef HIDP_BUTTON_CAPS  THIDPButtonCaps;

struct HIDP_VALUE_CAPS;
typedef HIDP_VALUE_CAPS *PHIDPValueCaps;

#pragma pack(push, 4)
struct HIDP_VALUE_CAPS
{
	Word UsagePage;
	Byte ReportID;
	Byte IsAlias;
	Word BitField;
	Word LinkCollection;
	Word LinkUsage;
	Word LinkUsagePage;
	Byte IsRange;
	Byte IsStringRange;
	Byte IsDesignatorRange;
	Byte IsAbsolute;
	Byte HasNull;
	Byte Reserved;
	Word BitSize;
	Word ReportCount;
	Word Reserved_[5];
	unsigned UnitsExp;
	unsigned Units;
	int LogicalMin;
	int LogicalMax;
	int PhysicalMin;
	int PhysicalMax;
	union
	{
		struct 
		{
			Word Usage;
			Word Reserved1;
			Word StringIndex;
			Word Reserved2;
			Word DesignatorIndex;
			Word Reserved3;
			Word DataIndex;
			Word Reserved4;
			
		};
		struct 
		{
			Word UsageMin;
			Word UsageMax;
			Word StringMin;
			Word StringMax;
			Word DesignatorMin;
			Word DesignatorMax;
			Word DataIndexMin;
			Word DataIndexMax;
			
		};
		
	};
} ;
#pragma pack(pop)

typedef HIDP_VALUE_CAPS  THIDPValueCaps;

struct HIDP_LINK_COLLECTION_NODE;
typedef HIDP_LINK_COLLECTION_NODE *PHIDPLinkCollectionNode;

#pragma pack(push, 4)
struct HIDP_LINK_COLLECTION_NODE
{
	Word LinkUsage;
	Word LinkUsagePage;
	Word Parent;
	Word NumberOfChildren;
	Word NextSibling;
	Word FirstChild;
	Byte CollectionType;
	Byte IsAlias;
	Word Reserved;
	void *UserContext;
} ;
#pragma pack(pop)

typedef HIDP_LINK_COLLECTION_NODE  THIDPLinkCollectionNode;

typedef char *PHIDPReportDescriptor;

struct HIDP_CAPS;
typedef HIDP_CAPS *PHIDPCaps;

#pragma pack(push, 2)
struct HIDP_CAPS
{
	Word Usage;
	Word UsagePage;
	Word InputReportByteLength;
	Word OutputReportByteLength;
	Word FeatureReportByteLength;
	Word Reserved[17];
	Word NumberLinkCollectionNodes;
	Word NumberInputButtonCaps;
	Word NumberInputValueCaps;
	Word NumberInputDataIndices;
	Word NumberOutputButtonCaps;
	Word NumberOutputValueCaps;
	Word NumberOutputDataIndices;
	Word NumberFeatureButtonCaps;
	Word NumberFeatureValueCaps;
	Word NumberFeatureDataIndices;
} ;
#pragma pack(pop)

typedef HIDP_CAPS  THIDPCaps;

struct HIDP_DATA;
typedef HIDP_DATA *PHIDPData;

#pragma pack(push, 4)
struct HIDP_DATA
{
	Word DataIndex;
	Word Reserved;
	union
	{
		struct 
		{
			Byte On_;
			
		};
		struct 
		{
			unsigned RawValue;
			
		};
		
	};
} ;
#pragma pack(pop)

typedef HIDP_DATA  THIDPData;

struct HIDP_UNKNOWN_TOKEN;
typedef HIDP_UNKNOWN_TOKEN *PHIDPUnknownToken;

#pragma pack(push, 4)
struct HIDP_UNKNOWN_TOKEN
{
	Byte Token;
	Byte Reserved[3];
	unsigned BitField;
} ;
#pragma pack(pop)

typedef HIDP_UNKNOWN_TOKEN  THIDPUnknownToken;

struct HIDP_EXTENDED_ATTRIBUTES;
typedef HIDP_EXTENDED_ATTRIBUTES *PHIDPExtendedAttributes;

#pragma pack(push, 4)
struct HIDP_EXTENDED_ATTRIBUTES
{
	Byte NumGlobalUnknowns;
	Byte Reserved[3];
	HIDP_UNKNOWN_TOKEN *GlobalUnknowns;
	unsigned Data[1];
} ;
#pragma pack(pop)

typedef HIDP_EXTENDED_ATTRIBUTES  THIDPExtendedAttributes;

typedef bool __stdcall (*THIDPInsertScanCodes)(void * Context, char * NewScanCodes, unsigned Length);

typedef unsigned __stdcall (*THidD_Hello)(char * Buffer, unsigned BufferLength);

typedef void __stdcall (*THidD_GetHidGuid)(GUID &HidGuid);

typedef BOOL __stdcall (*THidD_GetPreparsedData)(unsigned HidDeviceObject, void * &PreparsedData);

typedef BOOL __stdcall (*THidD_FreePreparsedData)(void * PreparsedData);

typedef BOOL __stdcall (*THidD_GetConfiguration)(unsigned HidDeviceObject, HIDD_CONFIGURATION &HidConfig, int Size);

typedef BOOL __stdcall (*THidD_SetConfiguration)(unsigned HidDeviceObject, const HIDD_CONFIGURATION &HidConfig, int Size);

typedef BOOL __stdcall (*THidD_FlushQueue)(unsigned HidDeviceObject);

typedef BOOL __stdcall (*THidD_GetFeature)(unsigned HidDeviceObject, void *Report, int Size);

typedef BOOL __stdcall (*THidD_SetFeature)(unsigned HidDeviceObject, void *Report, int Size);

typedef BOOL __stdcall (*THidD_GetNumInputBuffers)(unsigned HidDeviceObject, int &NumBufs);

typedef BOOL __stdcall (*THidD_SetNumInputBuffers)(unsigned HidDeviceObject, int NumBufs);

typedef BOOL __stdcall (*THidD_GetAttributes)(unsigned HidDeviceObject, HIDD_ATTRIBUTES &HidAttrs);

typedef BOOL __stdcall (*THidD_GetManufacturerString)(unsigned HidDeviceObject, wchar_t * Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetProductString)(unsigned HidDeviceObject, wchar_t * Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetSerialNumberString)(unsigned HidDeviceObject, wchar_t * Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetPhysicalDescriptor)(unsigned HidDeviceObject, void *Buffer, int BufferLength);

typedef BOOL __stdcall (*THidD_GetIndexedString)(unsigned HidDeviceObject, int Index, wchar_t * Buffer, int BufferLength);

typedef int __stdcall (*THidP_GetCaps)(void * PreparsedData, HIDP_CAPS &Capabilities);

typedef int __stdcall (*THidP_GetLinkCollectionNodes)(PHIDPLinkCollectionNode LinkCollectionNodes, unsigned &LinkCollectionNodesLength, void * PreparsedData);

typedef int __stdcall (*THidP_GetSpecificButtonCaps)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, PHIDPButtonCaps ButtonCaps, Word &ButtonCapsLength, void * PreparsedData);

typedef int __stdcall (*THidP_GetSpecificValueCaps)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, PHIDPValueCaps ValueCaps, Word &ValueCapsLength, void * PreparsedData);

typedef int __stdcall (*THidP_GetData)(unsigned ReportType, PHIDPData DataList, unsigned &DataLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetData)(unsigned ReportType, PHIDPData DataList, unsigned &DataLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef unsigned __stdcall (*THidP_MaxDataListLength)(unsigned ReportType, void * PreparsedData);

typedef int __stdcall (*THidP_GetUsages)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetButtons)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetUsagesEx)(unsigned ReportType, Word LinkCollection, PUsageAndPage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetButtonsEx)(unsigned ReportType, Word LinkCollection, PUsageAndPage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetUsages)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetButtons)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage ButtonList, unsigned &ButtonLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_UnsetUsages)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage UsageList, unsigned &UsageLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_UnsetButtons)(unsigned ReportType, Word UsagePage, Word LinkCollection, PUsage ButtonList, unsigned &ButtonLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef unsigned __stdcall (*THidP_MaxUsageListLength)(unsigned ReportType, Word UsagePage, void * PreparsedData);

typedef unsigned __stdcall (*THidP_MaxButtonListLength)(unsigned ReportType, Word UsagePage, void * PreparsedData);

typedef int __stdcall (*THidP_GetUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, unsigned &UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetScaledUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, int &UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_GetUsageValueArray)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, char * UsageValue, Word UsageValueByteLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, unsigned UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetScaledUsageValue)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, int UsageValue, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_SetUsageValueArray)(unsigned ReportType, Word UsagePage, Word LinkCollection, Word Usage, char * UsageValue, Word UsageValueByteLength, void * PreparsedData, void *Report, unsigned ReportLength);

typedef int __stdcall (*THidP_UsageListDifference)(PUsage PreviousUsageList, PUsage CurrentUsageList, PUsage BreakUsageList, PUsage MakeUsageList, unsigned UsageListLength);

typedef int __stdcall (*THidP_TranslateUsagesToI8042ScanCodes)(PUsage ChangedUsageList, unsigned UsageListLength, unsigned KeyAction, unsigned &ModifierState, THIDPInsertScanCodes InsertCodesProcedure, void * InsertCodesContext);

//-- var, const, procedure ---------------------------------------------------
static const int HIDP_STATUS_SUCCESS = 0x110000;
static const int HIDP_STATUS_NULL = 0x80110001;
static const int HIDP_STATUS_INVALID_PREPARSED_DATA = 0xc0110001;
static const int HIDP_STATUS_INVALID_REPORT_TYPE = 0xc0110002;
static const int HIDP_STATUS_INVALID_REPORT_LENGTH = 0xc0110003;
static const int HIDP_STATUS_USAGE_NOT_FOUND = 0xc0110004;
static const int HIDP_STATUS_VALUE_OUT_OF_RANGE = 0xc0110005;
static const int HIDP_STATUS_BAD_LOG_PHY_VALUES = 0xc0110006;
static const int HIDP_STATUS_BUFFER_TOO_SMALL = 0xc0110007;
static const int HIDP_STATUS_INTERNAL_ERROR = 0xc0110008;
static const int HIDP_STATUS_I8042_TRANS_UNKNOWN = 0xc0110009;
static const int HIDP_STATUS_INCOMPATIBLE_REPORT_ID = 0xc011000a;
static const int HIDP_STATUS_NOT_VALUE_ARRAY = 0xc011000b;
static const int HIDP_STATUS_IS_VALUE_ARRAY = 0xc011000c;
static const int HIDP_STATUS_DATA_INDEX_NOT_FOUND = 0xc011000d;
static const int HIDP_STATUS_DATA_INDEX_OUT_OF_RANGE = 0xc011000e;
static const int HIDP_STATUS_BUTTON_NOT_PRESSED = 0xc011000f;
static const int HIDP_STATUS_REPORT_DOES_NOT_EXIST = 0xc0110010;
static const int HIDP_STATUS_NOT_IMPLEMENTED = 0xc0110020;
static const int HIDP_STATUS_I8242_TRANS_UNKNOWN = 0xc0110009;
static const Shortint HIDP_LINK_COLLECTION_ROOT = 0xffffffff;
static const Shortint HIDP_LINK_COLLECTION_UNSPECIFIED = 0x0;
static const Shortint kmsLeftControl = 0x1;
static const Shortint kmsLeftShift = 0x2;
static const Shortint kmsLeftAlt = 0x4;
static const Shortint kmsLeftGUI = 0x8;
static const Shortint kmsRightControl = 0x10;
static const Shortint kmsRightShift = 0x20;
static const Shortint kmsRightAlt = 0x40;
static const Byte kmsRightGUI = 0x80;
static const Word kmsCapsLock = 0x100;
static const Word kmsScollLock = 0x200;
static const Word kmsNumLock = 0x400;
static const Shortint lcnIsAlias = 0x1;
static const Shortint HidP_Input = 0x0;
static const Shortint HidP_Output = 0x1;
static const Shortint HidP_Feature = 0x2;
static const Shortint HidP_Keyboard_Break = 0x0;
static const Shortint HidP_Keyboard_Make = 0x1;
extern PACKAGE THidD_Hello HidD_Hello;
extern PACKAGE THidD_GetHidGuid HidD_GetHidGuid;
extern PACKAGE THidD_GetPreparsedData HidD_GetPreparsedData;
extern PACKAGE THidD_FreePreparsedData HidD_FreePreparsedData;
extern PACKAGE THidD_GetConfiguration HidD_GetConfiguration;
extern PACKAGE THidD_SetConfiguration HidD_SetConfiguration;
extern PACKAGE THidD_FlushQueue HidD_FlushQueue;
extern PACKAGE THidD_GetFeature HidD_GetFeature;
extern PACKAGE THidD_SetFeature HidD_SetFeature;
extern PACKAGE THidD_GetNumInputBuffers HidD_GetNumInputBuffers;
extern PACKAGE THidD_SetNumInputBuffers HidD_SetNumInputBuffers;
extern PACKAGE THidD_GetAttributes HidD_GetAttributes;
extern PACKAGE THidD_GetManufacturerString HidD_GetManufacturerString;
extern PACKAGE THidD_GetProductString HidD_GetProductString;
extern PACKAGE THidD_GetSerialNumberString HidD_GetSerialNumberString;
extern PACKAGE THidD_GetPhysicalDescriptor HidD_GetPhysicalDescriptor;
extern PACKAGE THidD_GetIndexedString HidD_GetIndexedString;
extern PACKAGE THidP_GetCaps HidP_GetCaps;
extern PACKAGE THidP_GetLinkCollectionNodes HidP_GetLinkCollectionNodes;
extern PACKAGE THidP_GetSpecificButtonCaps HidP_GetSpecificButtonCaps;
extern PACKAGE THidP_GetSpecificValueCaps HidP_GetSpecificValueCaps;
extern PACKAGE THidP_GetData HidP_GetData;
extern PACKAGE THidP_SetData HidP_SetData;
extern PACKAGE THidP_MaxDataListLength HidP_MaxDataListLength;
extern PACKAGE THidP_GetUsages HidP_GetUsages;
extern PACKAGE THidP_GetButtons HidP_GetButtons;
extern PACKAGE THidP_GetUsagesEx HidP_GetUsagesEx;
extern PACKAGE THidP_GetButtonsEx HidP_GetButtonsEx;
extern PACKAGE THidP_SetUsages HidP_SetUsages;
extern PACKAGE THidP_SetButtons HidP_SetButtons;
extern PACKAGE THidP_UnsetUsages HidP_UnsetUsages;
extern PACKAGE THidP_UnsetButtons HidP_UnsetButtons;
extern PACKAGE THidP_MaxUsageListLength HidP_MaxUsageListLength;
extern PACKAGE THidP_MaxButtonListLength HidP_MaxButtonListLength;
extern PACKAGE THidP_GetUsageValue HidP_GetUsageValue;
extern PACKAGE THidP_GetScaledUsageValue HidP_GetScaledUsageValue;
extern PACKAGE THidP_GetUsageValueArray HidP_GetUsageValueArray;
extern PACKAGE THidP_SetUsageValue HidP_SetUsageValue;
extern PACKAGE THidP_SetScaledUsageValue HidP_SetScaledUsageValue;
extern PACKAGE THidP_SetUsageValueArray HidP_SetUsageValueArray;
extern PACKAGE THidP_UsageListDifference HidP_UsageListDifference;
extern PACKAGE THidP_TranslateUsagesToI8042ScanCodes HidP_TranslateUsagesToI8042ScanCodes;
#define HidModuleName "HID.dll"
extern PACKAGE int __fastcall HidP_GetButtonCaps_(unsigned ReportType, PHIDPButtonCaps ButtonCaps, Word &ButtonCapsLength, void * PreparsedData);
extern PACKAGE int __fastcall HidP_GetValueCaps_(unsigned ReportType, PHIDPValueCaps ValueCaps, Word &ValueCapsLength, void * PreparsedData);
extern PACKAGE bool __fastcall HidP_IsSameUsageAndPage_(USAGE_AND_PAGE u1, USAGE_AND_PAGE u2);
extern PACKAGE bool __fastcall IsHidLoaded(void);
extern PACKAGE bool __fastcall LoadHid(void);
extern PACKAGE void __fastcall UnloadHid(void);

}	/* namespace Hid */
using namespace Hid;
#pragma option pop	// -w-
#pragma option pop	// -Vx

#pragma delphiheader end.
//-- end unit ----------------------------------------------------------------
#endif	// Hid
