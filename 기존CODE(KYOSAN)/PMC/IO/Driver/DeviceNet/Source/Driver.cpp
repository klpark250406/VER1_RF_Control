#include <windows.h>
#include <stdio.h>
#include <direct.h>
#include <math.h>
#include <TString.h>
#include <stdlib.h>

#include <TDrvInterface.h>
#include <TSeqIF.h>
//#include <TUtilSync.h>
#include <TComSerial.h>
// #include "Devicenet.h"

#include "..\H\DeviceNet.h"

// #include ""
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
enum	{	NOTLOG	,	LOG		};
enum	{	BINARY	,	TEXT	};
enum	{	DI	,	DO  ,  AI  ,  AO  ,  SI  ,  SO  };
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
TCriticalSection csLock;
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
enum 	{FAULT = -1, EVENT = 1};
#define GETBIT(data, index) ((data & (1 << index)) >> index)

#define KYOSAN_SRFG_ID	9
#define KYOSAN_BRFG_ID 10
#define KYOSAN_SMAT_ID 11
#define KYOSAN_BMAT_ID 12
#define MFC1_ID		   20
#define MFC2_ID		   21
#define MFC3_ID		   22
#define MFC4_ID		   23
#define MFC5_ID		   24
#define MFC6_ID		   25
#define MFC7_ID		   26
#define MFC8_ID		   27
#define MFC9_ID		   28
#define MFC10_ID	   29

#define FRC_ID	    32


static	int			iComState  = 0;
static	int			logFileState;
static	int			logEnable = 1;

const WriteMask[] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
static  char		gcInterfaceName[256];

// DeviceNet Card(SST) Status
static	BOOL		bOnline		= FALSE;
static	BOOL		bStart		= FALSE;
static	BOOL		bOpen		= FALSE;
static	int			iMaxNode	= 0;
static  int         iMacId[64]  = {0};
static	char		chDeviceStatus[256] = {0};
static  int         gnLogSts    = 0;

char gsDrv_Name[32] = {0};

int gnRFCommSts[2] = {0};
double gdblErrorNum = 0.0;
double gdblErrorFuncNum = 0.0;
double gdblWarningNum = 0.0;
double gdblWarningFuncNum = 0.0;

// Memory Map : 30 -> max node number
IO_Map				InOutMap[64]		= {0};
// IO_Info				In_Info[30]			= {0};
// IO_Info				Out_Info[30]		= {0};

// DeviceNet Card(SST) Configuration
DNS_SCANNER_CFG		ScannerConfig		= {0};	// scanner configuration data structure
DNS_DEVICE_CFG		DeviceConfig[64]	= {0};	// device configuration data structure: 최대 연결 MacId = 64이므로,
DNS_STATUS			DeviceStatus[64]	= {0};

DWORD CardHandle;								// Interface Card Handle				
//HINSTANCE hLibInst;								// global library instance handle(SST)
//HINSTANCE hErrInst;								// error DLL module handle(SST)

HMODULE hLibInst;
HMODULE hErrInst;

HANDLE pFileHandle;


#define FILE_LIMIT		20		// 최대 FILE 생성 갯수.
#define FILE_SIZE		1048576	// 1Mbyte

char 	gszPath[512]= {0};
char	gszFullPath[512]={0};
// HANDLE 	pFileHandle;
char    gszFileName[128] = {0};
char    gszPath_origin[512] = {0};


union uValue
{
	float fValue;
	unsigned char ucValue[4];
};

union uValue1
{
	WORD 		 uwValue;
	short		 swValue;
	unsigned char ucValue[2];
};


union DATA
{
	WORD wDATA;
	short signed int iData;
	unsigned char ucData[2];
};

union DATA4
{
	int iData;
	unsigned char ucData[4];
};

union union_FRC
{
	WORD wDATA;
	short signed int iData;
	unsigned char ucData[2];
};

BOOL gbThread_Run = FALSE;
BOOL gbThread_End = FALSE;

HANDLE   hThread = NULL;
CRITICAL_SECTION mCS_Lock;


int gnAPCComSts = FALSE;
int gnFRCComSts = FALSE;

#define INF__NOT_USED		0
#define INF__PANEL_MODE		1
#define INF__SERVICE_MODE	2	// RS232 를 이용한 Firmware update 및 RFGen.의 자체 로그 백업용.
#define INF__RS232_MODE		3
#define INF__PROFIBUS_MODE	4	
#define INF__RS485_MODE		5
#define INF__REMOTE1_MODE	6
#define INF__REMOTE2_MODE	7
#define INF__REMOTE3_MODE	8
#define INF__USER1_MODE		9
#define INF__USER2_MODE		10
#define INF__DNET_MODE		11

#define Sleep_Time			10



// Kyosan
int gnKYOSAN_RFCommSts[2]		= {0};

int gnGenPowerSts_ON[2]		= {0};
int gnGenPowerSts_OFF[2]	= {0};
int gnGenFailSts[2]			= {0};
int gnGenRemoteMode[2]		= {0};
int gnGenACSag[2]			= {0};
int gnGenAbort[2]			= {0};
int gnGenDCShutDown[2]		= {0};
int gnGenArcing[2]			= {0};

int gnGenRefLimit[2]		= {0};
int gnGenVppLimit[2]		= {0};
int gnGenCurLimit[2]		= {0};
int gnGenVoltLimit[2]		= {0};
int gnGenFWDLimit[2]		= {0};
int gnGenSlaveNotReady[2]	= {0};
int gnGenInterlock[2]		= {0};

int gnGenFanFailure[2]		= {0};
int gnGenOverheat[2]		= {0};
int gnGenFuseAlarm[2]		= {0};
int gnGenCoverOpen[2]		= {0};
int gnGenAMPFailure[2]		= {0};
int gnGenDCFailure[2]		= {0};

int gnGenArcCNT[2]			= {0};
int gnGenFWDOverTime[2]		= {0};
int gnGenDCUnitFail[2]		= {0};
int gnGenDCOverTime[2]		= {0};
int gnGenRefOverTime[2]		= {0};
int gnGenVppOverTime[2]		= {0};
int gnGenCurOverTime[2]		= {0};
int gnGenVoltOverTime[2]	= {0};

int gnGenRefOverload[2]		= {0};
int gnGenVppOverload[2]		= {0};
int gnGenAbnormal_L[2]		= {0};
int gnGenAbnormal_H[2]		= {0};

int gnGenMatcherFail[2]		= {0};
int gnGenChopperFail[2]		= {0};
int gnGenMiddleAmpFail[2]	= {0};
int gnGenWaterShortage[2]	= {0};
int gnGenInterlockHW[2]		= {0};

int gnGenWaterleak[2]		= {0};

double gdblDeliveredPwr[2]	= {0.0};
double gdblFwdPwr[2]		= {0.0};
double gdblRefPwr[2]		= {0.0};
double gdbRampUpTime[2]			= {0.0};
double gdbRampDownTime[2]		= {0.0};
double gdbArcLevelRefelct[2]	= {0.0};
double gdbArcCountLimit[2]      = {0.0};
double gdblRFRestartTime[2]     = {0.0};
double gdblRFRecoverTime[2]     = {0.0};
double gdblRFRecognizeCNT[2]    = {0.0};
double gdblRFMaskTime[2]        = {0.0};
double gdblRFMaskDelayTime[2]   = {0.0};
double gdblRFOutDelayTime[2]    = {0.0};



int gnKYOSAN_MatCommSts[2]		= {0};


int gnblMATControl[2]			= {0};
int gnblMATRFSts[2]				= {0};
int gnblMATFail[2]				= {0};
int gnblMATMode[2]				= {0};
int gnblMATTuneCCW[2]			= {0};
int gnblMATTuneCW[2]			= {0};
int gnblMATLoadCCW[2]			= {0};
int gnblMATLoadCW[2]			= {0};

int gnblMATTuneCCWLimit[2]		= {0};
int gnblMATTuneCWLimit[2]		= {0};
int gnblMATLoadCCWLimit[2]		= {0};
int gnblMATLoadCWLimit[2]		= {0};

int gnblMATHWLimit[2]			= {0};
int gnblMATTimeOut[2]			= {0};

int gnblMATFANFail[2]			= {0};
int gnblMATWaterShortage[2]		= {0};
int gnblMATCoverOpen[2]			= {0};
int gnblMATWaterLeak1[2]		= {0};
int gnblMATWaterLeak2[2]		= {0};



double gdblCLPos[2]				= {0.0};
double gdblCTPos[2]				= {0.0};
double gdblVdc[2]				= {0.0};
double gdblVpp[2]				= {0.0};
double gdblPCWFlow[2]			= {0.0};



double gdblZ_Load[2]			= {0.0};
double gdblPhi_Load[2]			= {0.0};

// for FRC Global Param

int gnFRC1_DATA = 0;
int gnFRC2_DATA = 0;
int gnFRC3_DATA = 0;
int gdFRC_Total = 0;


unsigned short gnFRC_Status = 0;
double gdFRC_Flow1    = 0.0;
double gdFRC_Flow2    = 0.0;
double gdFRC_Flow3    = 0.0;
double gdFRC_MaxRate  = 1.0;
double gdFRC_MaxRateCalc =1.0;
float gfFRC_Ratio	  = 0.0;		// 2018.03.01


//----------------------------------------------
// for Brooks Mfc
int gnMfcCommSts[20]		= {0};
double gdblMfcFlowrate[20]	= {0.0};
double gdblMfcSetFlow[20]		= {0.0};
double gdblMfcTemp[20]		= {0.0};

////////////////// Functions Prototype  //////////////////////////
int	message_out(char *msg, BOOL status);
int Get_Bit_Sts (BYTE source , int pos);

int Drv_StartScan(void);
int Drv_Online(void);
int Drv_Open(void);
int GetProcAddresses(void);
int Drv_GetStatus(int macid);
int READ_RFGEN(int nMacId);
int READ_KYOSAN_RFGEN(int nMacId);
int READ_KYOSAN_MATCH(int nMacId);
int READ_FRC(int nMacId);
int READ_MFC(int nMacId);
void UNLOAD_DEVICENET();


int Make_Log(char* pstr_Msg1, char* pstr_Msg2, int nLvl);
void print_log(char	*pstr_Log, int nLevel);
int OutputLog(char *pstr_Msg);
int MakeFile();
int InitLog(char *csPath);
//////////////////////////////////////////////////////////////////


// VAT APC Valve
// Output Instance - 103
// Input  Instance - 104

// Huettinger RF Generator
// Output Instance - 100
// Input  Instance - 104

void THREAD_READ(void *pDummy)
{
	int nMacId_RF   = 0;
	int nMacId_MFC  = 0;

	gbThread_Run = TRUE;
	gbThread_End = FALSE;

	while(1)
	{
		if(gbThread_Run == FALSE) 	break;
		
		//////////////////////////////////////////////////////////////////////////
		_sleep(Sleep_Time);
		EnterCriticalSection(&mCS_Lock);
		_sleep(Sleep_Time);

		//FRC
		READ_FRC(32);
		
		//RF
		for(nMacId_RF = 9; nMacId_RF <=12; nMacId_RF++)	
		{
 			if		(nMacId_RF == 9  || nMacId_RF == 10)	READ_KYOSAN_RFGEN(nMacId_RF);
			else if	(nMacId_RF == 11 || nMacId_RF == 12)	READ_KYOSAN_MATCH(nMacId_RF);		
			
		}

  
		//MFC
		for(nMacId_MFC = MFC1_ID; nMacId_MFC <=MFC10_ID; nMacId_MFC++)	
		{
			READ_MFC(nMacId_MFC);
		}

	
		LeaveCriticalSection(&mCS_Lock);


	}

	_endthread();
}


//-------------------------------------------------------------------------//
//  Function    : message_out                                             //
//	Description	: Driver Message Output                                    //
//	Arguments	: char* msg, BOOL status                                   //
//	Return		: 1                                                        //
//-------------------------------------------------------------------------//
int	message_out(char *msg, BOOL status)
{
	char ErrString[256] = {0};
	char log_str[512] = {0};

	if( status == TRUE )
	{
		Make_Log("", msg, EVENT);
	}
	else 
	{
		DNE_CommandError( GetLastError(), ErrString, sizeof(ErrString) );	
		sprintf(log_str, "%s - %s", msg, ErrString);
		Make_Log("", log_str, FAULT);
	}

	return 1;
}

void print_out_with_errormsg(char *msg)
{
	char ErrString[256] = {0};
	DNE_CommandError( GetLastError(), ErrString, sizeof(ErrString) );
	printf("%s :::> %s\n", msg, ErrString);
}


//-------------------------------------------------------------------------//
//  Function    : Drv_StartScan()                                          //
//	Description	: 1. Load dnscan32.dll                                     //
//				: 2. Get the function pointer of dnscan32.dll              //
//              : 3. Load dnerr32.dll                                      //
//              : 4. Get the function pointer of dnerr32.dll               //
//              : 5. DNS_LoadDriver()                                      //
//              : 6. DNS_OpenCard()                                        //
//	Arguments	: void                                                     //
//	Return		: -1(failure), 1(succeess)                                 //
//-------------------------------------------------------------------------//

int Drv_StartScan(void)
{
	char ErrorString[256];
	
	if(DNS_StartScan(CardHandle) == FALSE)
	{
		sprintf(ErrorString, "[%s] DNS_StartScan : Failure", gcInterfaceName);
		message_out(ErrorString, FALSE);
		return -1;
	}
	else
	{
		sprintf(ErrorString, "[%s] DNS_StartScan : Success", gcInterfaceName);
		message_out(ErrorString, TRUE);
	}

	
	return 1;
}


//-------------------------------------------------------------------------//
//  Function    : Drv_GetDeviceStatus                                      //
//	Description	: return status of DeviceNet Scanner Card                  //
//	Arguments	: void                                                     //
//	Return		: -1(failure), 1(succeess)                                 //
//-------------------------------------------------------------------------//
int Drv_GetStatus(int macid)
{
	DNS_STATUS Device_Status;

	DNS_GetDeviceStatus(CardHandle, DeviceConfig[macid].MacId, &Device_Status);
	
	switch(Device_Status.StatusCode)
	{
		case DNS_NON_EXIST:
			strcpy(chDeviceStatus, "DNS_NON_EXIST");
			break;
					
		case DNS_IDLE:
			strcpy(chDeviceStatus, "DNS_IDLE");
			break;
					
		case DNS_ACTIVE:
			strcpy(chDeviceStatus, "DNS_ACTIVE");
			break;
					
		case DNS_TIMEOUT:
			strcpy(chDeviceStatus, "DNS_TIMEOUT");
			break;
					
		case DNS_FAULT_UCMM:
			strcpy(chDeviceStatus, "DNS_FAULT_UCMM");
			break;
					
		case DNS_MS_BUSY:
			strcpy(chDeviceStatus, "DNS_MS_BUSY");
			break;
					
		case DNS_FAULT_MS:
			strcpy(chDeviceStatus, "DNS_FAULT_MS");
			break;
					
		case DNS_BAD_VENDOR:
			strcpy(chDeviceStatus, "DNS_BAD_VENDOR");
			break;
					
		case DNS_FAULT_VENDOR:
			strcpy(chDeviceStatus, "DNS_FAULT_VENDOR");
			break;
					
		case DNS_BAD_TYPE:
			strcpy(chDeviceStatus, "DNS_BAD_TYPE");
			break;
					
		case DNS_FAULT_TYPE:
			strcpy(chDeviceStatus, "DNS_FAULT_TYPE");
			break;
					
		case DNS_BAD_CODE:
			strcpy(chDeviceStatus, "DNS_BAD_CODE");
			break;
					
		case DNS_FAULT_CODE:
			strcpy(chDeviceStatus, "DNS_FAULT_CODE");
			break;
					
		case DNS_BAD_IO1_INPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_BAD_IO1_INPUT_SIZE");
			break;
					
		case DNS_FAULT_IO1_INPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_FAULT_IO1_INPUT_SIZE");
			break;
					
		case DNS_BAD_IO1_OUTPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_BAD_IO1_OUTPUT_SIZE");
			break;
					
		case DNS_FAULT_IO1_OUTPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_FAULT_IO1_OUTPUT_SIZE");
			break;
					
		case DNS_BAD_IO2_INPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_BAD_IO2_INPUT_SIZE");
			break;
					
		case DNS_FAULT_IO2_INPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_FAULT_IO2_INPUT_SIZE");
			break;
					
		case DNS_BAD_IO2_OUTPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_BAD_IO2_OUTPUT_SIZE");
			break;
					
		case DNS_FAULT_IO2_OUTPUT_SIZE:
			strcpy(chDeviceStatus, "DNS_FAULT_IO2_OUTPUT_SIZE");
			break;
					
		case DNS_FAULT_SETTING_IO1_RATE:
			strcpy(chDeviceStatus, "DNS_FAULT_SETTING_IO1_RATE");
			break;
					
		case DNS_FAULT_SETTING_IO2_RATE:
			strcpy(chDeviceStatus, "DNS_FAULT_SETTING_IO2_RATE");
			break;
					
		case DNS_MS_SYNC:
			strcpy(chDeviceStatus, "DNS_MS_SYNC");
			break;
					
		default:
			break;
	}
	
	return 1;
}

//-------------------------------------------------------------------------//
// Procedure   : AddDevice                                                 //
// Dexcription : 1. Read DeviceNet Configuration File                      //
//             : 2. Set the Device Configuration (MacId, In/Out size)      //
//			   : 3. DNS_AddDevice()                                        //
//             : 4. Memory Allocation                                      //
//             : DI/O(2ch or 4ch or 8ch) -> size : 1byte                   //
// Arguments   : char* cfgFile                                             //
// Returns     : 1(success), -1(failure)                                   //
//-------------------------------------------------------------------------//

int Drv_AddDevice(char* cfgFile)
{
	char	ErrorString[256];
	
	int		moduleIndex=0;
	int		ai_max = 0, ao_max = 0, di_max = 0, do_max = 0;
	int		count = 0;

	char	sType1[256];
	char	sType2[256];
	
	WORD	macId, inSize, outSize;
	FILE	*fp;
	int		iNodeCount = 0;
	
	char	sMac_ID[256] = {0};

	if ( ( fp = fopen( cfgFile , "r" ) ) == NULL )
	{
		printf("[%s] Load Error >> Cannot Find DeviceNet PreConfig File\n", gsDrv_Name);
		message_out("Cannot Find DeviceNet PreConfig File", FALSE);
		Make_Log("", "0", FAULT);
		return 0;
	}
	count = GetPrivateProfileInt("NODE",     "COUNT",  0, cfgFile);
	
	iMaxNode = count;
		
	for(iNodeCount = 0; iNodeCount < count; iNodeCount++)
	{
		sprintf(sMac_ID, "MAC ID-%d", iNodeCount);
		macId   = GetPrivateProfileInt(sMac_ID,   "MAC_ID",   0, cfgFile);
		inSize  = GetPrivateProfileInt(sMac_ID,   "IN_SIZE",  0, cfgFile);
		outSize = GetPrivateProfileInt(sMac_ID,   "OUT_SIZE", 0, cfgFile);
		
		GetPrivateProfileString(sMac_ID,  "TYPE1",  "POLL", sType1, 256, cfgFile);
		GetPrivateProfileString(sMac_ID,  "TYPE2",  "NONE", sType2, 256, cfgFile);

		// setup device configuration and add slave to scan list
		DeviceConfig[macId].MacId			= macId;			
		iMacId[iNodeCount] = macId;

		// if vendor id, product code, and product type are 0 they will be ignored
		// a call to DNS_GetDevice will retrieve the VendorId, DeviceType, and ProductCode
		// read from the device
		DeviceConfig[macId].VendorId		= 0;				//ignore
		DeviceConfig[macId].ProductCode		= 0;				//ignore
		DeviceConfig[macId].DeviceType		= 0;				//ignore

		if((strcmp(sType1, "POLL") == 0) && (strcmp(sType2, "EXPLICIT") == 0))
		{
			DeviceConfig[macId].Flags		= SS_P^SS_EX;	//Polling
		}
		else if((strcmp(sType1, "POLL") == 0)&& (strcmp(sType2, "EXPLICIT") != 0 ))
		{
			DeviceConfig[macId].Flags		= SS_P;			//Polling
		}
		else if((strcmp(sType1, "POLL") != 0)&& (strcmp(sType2, "EXPLICIT") != 0 ))
		{
			DeviceConfig[macId].Flags		= SS_EX;		//Polling
		}

//		DeviceConfig[macId].Io1Interval		= 125;			//in-out 1 polling interval, mac id 별로 interval time 을 125ms 로 설정.

		// 2008.12.05 추가 변경건
		DeviceConfig[macId].Io1Interval		= 0;			//in-out 1 polling interval, interval time 에 관한 상태를 시스템에 맡김.
		
		// poll connection alone uses I/O connection 1
		// wInputSize와 wOutputSize는 SST configure tool에서 확인.

		//////////////////////////////////////////////////////////////////
		// Notice : Input Memory와 Output Memory가 겹쳐지지 않도록 주의한다.
		// Input  은 0x1000 ~ 영역에..
		// Output 은 0x2000 ~ 영역에.. 맵핑한다. 
		// 한개의 노드에서 잡을 수 있는 최대 영역은 512 byte이며 256개의 analog 채널을 갖는다.
		// default로 0x30 byte 만큼 잡아서 맵핑하도록 해본다. 즉, analog 48 채널 영역만큼을 확보하여본다.
		// 만일 최대 영역이 0x100 만큼 된다면 그에 맞도록 offset 값을 설정한다.
		//////////////////////////////////////////////////////////////////
		DeviceConfig[macId].Input1Size		= inSize;							//위의 MacId에 연결되어 있는 입력 장치들의 total size
		DeviceConfig[macId].Input1Offset	= 0x1000 + 0x100 * ( iNodeCount );	// 기존 0x50 에서 0x100 으로 수정 :  08.12.16
		DeviceConfig[macId].Output1Size		= outSize;
		DeviceConfig[macId].Output1Offset	= 0x2000 + 0x100 * ( iNodeCount );
		
		// Explicit의 size와 offset이 정해지지 않으면 제대로 configuration이 불가능.
		DeviceConfig[macId].ExplicitSize	= 100;
		DeviceConfig[macId].ExplicitOffset	= 0x3000 + 0x100 * ( iNodeCount );

		// add the device to the scan list
		if( DNS_AddDevice( CardHandle, &DeviceConfig[macId] ) == -1 )
		{
			printf("[%s] Load Error >> DNS_AddDevice MacID[%d] : Failure\n", gsDrv_Name, macId);
			sprintf(ErrorString, "[%s] DNS_AddDevice MacID[%d] : Failure", gcInterfaceName, macId);
			message_out( ErrorString, FALSE);
			return 0;
		}
		else
		{
//			printf("[%s] DNS_AddDevice MacID[%d] : Success\n", gsDrv_Name, macId);
			sprintf(ErrorString, "[%s] DNS_AddDevice MacID[%d] : Success", gcInterfaceName, macId);
			message_out( ErrorString, TRUE);
		}

		InOutMap[macId].pReadData		= (PBYTE)malloc(inSize);
		InOutMap[macId].pWriteData		= (PBYTE)malloc(outSize);

		memset(InOutMap[macId].pReadData,  0, inSize);
		memset(InOutMap[macId].pWriteData, 0, outSize);
	}
	

	fclose(fp);
	return 1;
}


//-------------------------------------------------------------------------//
//  Function    : Drv_Open()                                               //
//	Description	: 1. Load dnscan32.dll                                     //
//				: 2. Get the function pointer of dnscan32.dll              //
//              : 3. Load dnerr32.dll                                      //
//              : 4. Get the function pointer of dnerr32.dll               //
//              : 5. DNS_LoadDriver()                                      //
//              : 6. DNS_OpenCard()                                        //
//	Arguments	: void                                                     //
//	Return		: -1(failure), 1(succeess)                                 //
//-------------------------------------------------------------------------//
int Drv_Open(void)
{
	char msg[256] = {0};

	hLibInst = LoadLibrary("dnscan32.dll");
	if( hLibInst == NULL )
    {
		printf("[%s] Load Error >> LoadLibrary Failed - dnscan32.dll\n", gsDrv_Name);
		message_out("LoadLibrary Failed - dnscan32.dll", FALSE);
		return -1;
	}
	if( GetProcAddresses() == -1 )
    {
		printf("[%s] Load Error >> LoadLibrary Failed-get proc addresses for dnscan32 function calls\n", gsDrv_Name);
		message_out("LoadLibrary Failed-get proc addresses for dnscan32 function calls", FALSE);
        FreeLibrary( hLibInst );        
		return -1;
	}
	
	hErrInst = LoadLibrary("dnerr32.dll");
	if( hErrInst == NULL )
    {
		FreeLibrary( hLibInst );
        printf("[%s] Load Error >> LoadLibrary Failed-load dnerr32 DLL\n", gsDrv_Name);
        message_out("LoadLibrary Failed-load dnerr32 DLL", FAULT);

		return -1;
	}
    if ( (DNE_CommandError = ( dneCommandError )GetProcAddress( hErrInst, "DNE_CommandError" )) == FALSE)
    {
		printf("[%s] Load Error >> LoadLibrary Failed-get proc address for error library\n", gsDrv_Name);    
		message_out("LoadLibrary Failed-get proc address for error library", FALSE);    

        FreeLibrary( hLibInst );        
        FreeLibrary( hErrInst );        
        return -1;
    }
	if( DNS_LoadDriver( TEXT("ssdn32.dll") ) == FALSE )
    {
		printf("[%s] Load Error >> DNS_LoadDriver : Failure\n", gsDrv_Name);
		sprintf(msg, "[%s] DNS_LoadDriver : Failure", gcInterfaceName);
		message_out(msg, FALSE);
        FreeLibrary( hLibInst );        
        FreeLibrary( hErrInst );        
        return -1;
    }


	if( DNS_OpenCard( &CardHandle, gcInterfaceName, NULL, 0 ) == FALSE )
	{
		print_out_with_errormsg("DEVNET Load Error >> DNS_OpenCard : Failure");
		sprintf(msg, "[%s] DNS_OpenCard : Failure", gcInterfaceName);
		message_out(msg, FALSE);	
		DNS_FreeDriver();
		FreeLibrary( hLibInst );        
		FreeLibrary( hErrInst );	
		return -1;	
	}

	
	bOpen = TRUE;
	
	return 1;
}


//-------------------------------------------------------------------------//
//  Function    : Drv_Online                                               //
//	Description	: DNS_Online()   										   //
//	Arguments	: void			                                           //
//	Return		: -1(failure), 1(succeess)                                 //
//-------------------------------------------------------------------------//
int Drv_Online(void)
{
	char msg[256] = {0};
	// setup the scanner parameters and go online    
	// no server parameters selected.
	
	ScannerConfig.MacId				= 0;		//SST DeviceNet Card MacID
	ScannerConfig.BaudRate			= DN_125K;	//Baud Rate = 500K
	ScannerConfig.Io1Interval		= 0;		// 2008.05 추가 : polling cycle 을 하드웨어 맞추도록 설정함.
	
	if(DNS_Online(CardHandle, &ScannerConfig) == FALSE)
	{
		print_out_with_errormsg("[DEVNET] Load Error >> DNS_Online");
		sprintf("[%s] Load Error >> DNS_Online : Failure", gsDrv_Name);
		message_out(msg, FALSE);
		return -1;
	}

	return 1;
}





//-------------------------------------------------------------------------//
//  Function    : Get_Bit_Sts                                              //
//	Description	: return Bit Status of Source[BYTE]                        //
//	Arguments	: BYTE source, int pos                                     //
//	Return		: -1(failure), 1(succeess)                                 //
//-------------------------------------------------------------------------//
int Get_Bit_Sts (BYTE source , int pos)
{
	int result;
	
	BYTE mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};
	BYTE Temp;

	Temp   = (int)(source & mask[pos]);	//source = 0010 1011, mask[3] = 0000 1000, Temp = 0000 1000
	Temp   = Temp >> pos;               //pos만큼 각 비트가 오른쪽으로 이동. 이동된 공간은 모두 0으로 채워짐.
	result = Temp;

	return result;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL OnInitDevice( int ID1 , int ID2 , int ID3 , int ID4 , int ID5 , int ID6 , int ID7 , int ID8 , int ID9 , int ID10, TCHAR *szParm , void **ppDrvData /*OUT*/ ) {
	_tprintf(">>>>> [%d],[%d],[%d],[%d],[%d],[%d],[%d],[%d],[%d],[%d],[%s]\n", ID1 , ID2 , ID3 , ID4 , ID5 , ID6 , ID7 , ID8 , ID9 , ID10, szParm);
// 	printf(">>>>> [%d],[%d],[%d],[%d],[%d],[%d],[%d],[%d],[%d],[%d],[%s]\n", ID1 , ID2 , ID3 , ID4 , ID5 , ID6 , ID7 , ID8 , ID9 , ID10, szParm);
	int ret			= 0;
	WORD MacId		= 0;
	WORD InputSize	= 0;
	WORD OutputSize	= 0;
	int cnt;
	
	char str_msg[256] = {0};

	char sz_name1[128] = {0};
	char sz_name2[128] = {0};

	sprintf(gsDrv_Name, "%s", szParm);



	GetPrivateProfileString("INTERFACE", "CARD0", "DN3-PCI-0000", sz_name1, 256, "IO\\Driver\\DeviceNet\\DEVICENET.INI");
	GetPrivateProfileString("INTERFACE", "CARD1", "DN3-PCI-0000", sz_name2, 256, "IO\\Driver\\DeviceNet\\DEVICENET.INI");

	if		(ID1 == 0)	sprintf(gcInterfaceName, sz_name1);
	else if	(ID1 == 1)	sprintf(gcInterfaceName, sz_name2);
	
	printf("DEVICENET CARD [ %s ] IS DETECTED.\n", gcInterfaceName);

// 	CreateDirectory("LOG//", NULL);
// 	CreateDirectory("LOG//DRV//", NULL);
// 
// 	sprintf(str_msg, "LOG//DRV//%s.LOG", szParm);
// 	InitLog(str_msg);
// 	MakeFile();


	ret = Drv_Open();
	if( ret > 0 )
	{
		message_out("IO DeviceNet Driver success to open",			TRUE);
	}
	else
	{
		message_out("IO DeviceNet Driver fail to open",				FALSE);
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////////
	ret = Drv_AddDevice("IO\\Driver\\DeviceNet\\DEVICENET.INI");
	if( ret > 0 )
	{
		message_out("IO DeviceNet Driver success to add device",	TRUE);
	}
	else
	{
		message_out("IO DeviceNet Driver fail to add device",		FALSE);
		return FALSE;
	}
	
	/////////////////////////////////////////////////////////////////////
	ret = Drv_Online();
	if( ret > 0 )
	{
		message_out("IO DeviceNet Driver success to online",		TRUE);
	}
	else
	{
		message_out("IO DeviceNet Driver fail to online",			FALSE);
		return FALSE;
	}

	/////////////////////////////////////////////////////////////////////
	ret = Drv_StartScan();
	if( ret > 0 )
	{
		message_out("IO DeviceNet Driver success to start scanning",TRUE);
	}
	else
	{
		message_out("IO DeviceNet Driver fail to start scanning",	FALSE);
		return FALSE;
	}


	Drv_GetStatus(0);
	printf("[DEVNET] ScanCard Drv_GetStatus = %s\n", chDeviceStatus);

	for(cnt = 0; cnt < iMaxNode; cnt++)
	{
		Drv_GetStatus(iMacId[cnt]);
//		printf("[DEVNET] MAC ID [ %d ] Drv_GetStatus = %s\n", iMacId[cnt], chDeviceStatus);
	}

	//-----------------------------------------------------------------//
	// UnHandled Exception Handler                                     //
	//-----------------------------------------------------------------//

	_sleep(500);

 	InitializeCriticalSection(&mCS_Lock);

	hThread = (HANDLE)_beginthread(THREAD_READ, 0, NULL);
	if(hThread == NULL)
	{
 		DeleteCriticalSection(&mCS_Lock);
		UNLOAD_DEVICENET();
		return FALSE;
	}

	return TRUE;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
BOOL OnKillDevice( void* pDrvData , int ID1 , int ID2 , int ID3 , int ID4 , int ID5 , int ID6 , int ID7 , int ID8 , int ID9 , int ID10, TCHAR *szParm ) {
	int index = 0;
	
	
 	EnterCriticalSection(&mCS_Lock);
	
	_sleep(100);
	
	InOutMap[9].pWriteData[0]  = 0;	// Rf : Off, Filament : Off, DNetActive : Off
	InOutMap[10].pWriteData[0] = 0;	// Rf : Off, Filament : Off, DNetActive : Off
	DNS_WriteDeviceIo( CardHandle, DeviceConfig[9].MacId,  DNS_OUTPUT1, InOutMap[9].pWriteData,  DeviceConfig[9].Output1Size  ); _sleep(50);
	DNS_WriteDeviceIo( CardHandle, DeviceConfig[10].MacId, DNS_OUTPUT1, InOutMap[10].pWriteData, DeviceConfig[10].Output1Size ); _sleep(50);
	
 	LeaveCriticalSection(&mCS_Lock);
	
	gbThread_Run = FALSE;
	WaitForSingleObject(hThread, INFINITE);
	CloseHandle(hThread);
	DeleteCriticalSection(&mCS_Lock);
	
	if(CardHandle)
	{
		DNS_CloseCard(CardHandle);
	}
	
	DNS_FreeDriver();
	if(hLibInst)
	{
		FreeLibrary(hLibInst);
		printf("hLibInst is free\n");
	}
	
	if(hErrInst)
	{
		FreeLibrary(hErrInst);
		printf("hErrInst is free\n");
	}
	
	for(index = 0; index < 64; index++)
	{
		free(InOutMap[index].pReadData);
		free(InOutMap[index].pWriteData);
	}
	
	message_out("IO DeviceNet success to free Memory Map Allocation", TRUE);
	return TRUE;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//-----------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
int OnReadDigital( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, int* Result ) {
//	TAutoCriticalSection Lock(csLock);
	int		Data_Bit	= 0;
	int		Data		= 0;
	int		HighByte	= 0;
	int		LowByte		= 0;

	int		TempValue   = 0;

	char Data_result[256] = {0};
	char msg[256] = {0};
	
	int  nData = 0;
	int nIndex = 0;

	// Instance 106 : 30bytes

	if(ID2 == 99)
	{
		*Result = TRUE;

 		EnterCriticalSection(&mCS_Lock);
		
		_sleep(Sleep_Time);

		DNS_GetDeviceStatus(CardHandle, DeviceConfig[ID1].MacId, &DeviceStatus[ID1]);

 		LeaveCriticalSection(&mCS_Lock);
		
		if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
		{
			if(ID1 == KYOSAN_SRFG_ID)	nIndex = 0;
			else					nIndex = 1;

			if(DeviceStatus[ID1].StatusCode != DNS_ACTIVE)	gnKYOSAN_RFCommSts[nIndex] = FALSE;
			else											gnKYOSAN_RFCommSts[nIndex] = TRUE;			
			
			return gnKYOSAN_RFCommSts[nIndex];
		}
		else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
		{
			if(ID1 == KYOSAN_SMAT_ID)	nIndex = 0;
			else					nIndex = 1;
			
			if(DeviceStatus[ID1].StatusCode != DNS_ACTIVE)	gnKYOSAN_MatCommSts[nIndex] = FALSE;
			else											gnKYOSAN_MatCommSts[nIndex] = TRUE;			
						
			return gnKYOSAN_MatCommSts[nIndex];
		}
		else if(ID1 >= MFC1_ID && ID1 <= MFC10_ID)
		{
			nIndex = ID1 - MFC1_ID;
			if(DeviceStatus[ID1].StatusCode != DNS_ACTIVE)	gnMfcCommSts[nIndex] = FALSE;
			else											gnMfcCommSts[nIndex] = TRUE;	
		
			return gnMfcCommSts[nIndex];
		}
		else if( ID1 == 32 )
        {
            if(DeviceStatus[ID1].StatusCode != DNS_ACTIVE)  gnFRCComSts = FALSE;
            else                                            gnFRCComSts = TRUE;
			
            return gnFRCComSts;
        }
		
	}


	if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
	{
		if	(ID1 == KYOSAN_SRFG_ID)	nIndex = 0;
		else						nIndex = 1;
		
		if		(ID2 == 22	&& ID3 == 0)	nData = gnGenPowerSts_ON[nIndex];
		else if	(ID2 == 22	&& ID3 == 1)	nData = gnGenPowerSts_OFF[nIndex];
		else if (ID2 == 22	&& ID3 == 2)	nData = gnGenFailSts[nIndex];
		else if (ID2 == 22	&& ID3 == 3)	nData = gnGenRemoteMode[nIndex];
		else if (ID2 == 22	&& ID3 == 4)	nData = gnGenACSag[nIndex];
		else if	(ID2 == 22	&& ID3 == 5)	nData = gnGenAbort[nIndex];
		else if (ID2 == 22	&& ID3 == 6)	nData = gnGenDCShutDown[nIndex];
		else if (ID2 == 22	&& ID3 == 7)	nData = gnGenArcing[nIndex];

		else if	(ID2 == 23	&& ID3 == 0)	nData = gnGenRefLimit[nIndex];
		else if	(ID2 == 23	&& ID3 == 1)	nData = gnGenVppLimit[nIndex];
		else if (ID2 == 23	&& ID3 == 2)	nData = gnGenCurLimit[nIndex];
		else if (ID2 == 23	&& ID3 == 3)	nData = gnGenVoltLimit[nIndex];
		else if (ID2 == 23	&& ID3 == 4)	nData = gnGenFWDLimit[nIndex];
		else if	(ID2 == 23  && ID3 == 5)	nData = gnGenSlaveNotReady[nIndex];
		else if (ID2 == 23	&& ID3 == 6)	nData = gnGenInterlock[nIndex];

		else if	(ID2 == 24	&& ID3 == 0)	nData = gnGenFanFailure[nIndex];
		else if	(ID2 == 24	&& ID3 == 1)	nData = gnGenOverheat[nIndex];
		else if (ID2 == 24	&& ID3 == 2)	nData = gnGenFuseAlarm[nIndex];
		else if (ID2 == 24	&& ID3 == 3)	nData = gnGenCoverOpen[nIndex];
		else if (ID2 == 24	&& ID3 == 4)	nData = gnGenAMPFailure[nIndex];
		else if	(ID2 == 24	&& ID3 == 5)	nData = gnGenDCFailure[nIndex];

		else if	(ID2 == 25	&& ID3 == 0)	nData = gnGenArcCNT[nIndex];
		else if	(ID2 == 25	&& ID3 == 1)	nData = gnGenFWDOverTime[nIndex];
		else if (ID2 == 25	&& ID3 == 2)	nData = gnGenDCUnitFail[nIndex];
		else if (ID2 == 25	&& ID3 == 3)	nData = gnGenDCOverTime[nIndex];
		else if (ID2 == 25	&& ID3 == 4)	nData = gnGenRefOverTime[nIndex];
		else if	(ID2 == 25	&& ID3 == 5)	nData = gnGenVppOverTime[nIndex];
		else if (ID2 == 25	&& ID3 == 6)	nData = gnGenCurOverTime[nIndex];
		else if (ID2 == 25	&& ID3 == 7)	nData = gnGenVoltOverTime[nIndex];


		else if	(ID2 == 26	&& ID3 == 0)	nData = gnGenRefOverload[nIndex];
		else if	(ID2 == 26	&& ID3 == 1)	nData = gnGenVppOverload[nIndex];
		else if (ID2 == 26	&& ID3 == 6)	nData = gnGenAbnormal_L[nIndex];
		else if (ID2 == 26	&& ID3 == 7)	nData = gnGenAbnormal_H[nIndex];

		else if (ID2 == 27	&& ID3 == 0)	nData = gnGenMatcherFail[nIndex];
		else if	(ID2 == 27	&& ID3 == 1)	nData = gnGenChopperFail[nIndex];
		else if (ID2 == 27	&& ID3 == 2)	nData = gnGenMiddleAmpFail[nIndex];
		else if (ID2 == 27	&& ID3 == 3)	nData = gnGenWaterShortage[nIndex];
		else if (ID2 == 27	&& ID3 == 5)	nData = gnGenInterlockHW[nIndex];

		else if (ID2 == 28	&& ID3 == 2)	nData = gnGenWaterleak[nIndex];


		*Result = TRUE;
	}
	else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
	{
		if	(ID1 == KYOSAN_SMAT_ID)	nIndex = 0;
		else						nIndex = 1;
 
		if		(ID2 == 8	&& ID3 == 0)	nData = gnblMATControl[nIndex];	
		else if	(ID2 == 8	&& ID3 == 1)	nData = gnblMATRFSts[nIndex];	
		else if (ID2 == 8	&& ID3 == 2)	nData = gnblMATFail[nIndex];		
		else if (ID2 == 8	&& ID3 == 3)	nData = gnblMATMode[nIndex];		
		else if (ID2 == 8	&& ID3 == 4)	nData = gnblMATTuneCCW[nIndex];	
		else if	(ID2 == 8	&& ID3 == 5)	nData = gnblMATTuneCW[nIndex];	
		else if (ID2 == 8	&& ID3 == 6)	nData = gnblMATLoadCCW[nIndex];	
		else if (ID2 == 8	&& ID3 == 7)	nData = gnblMATLoadCW[nIndex];	

		else if	(ID2 == 9	&& ID3 == 4)	nData = gnblMATTuneCCWLimit[nIndex];	
		else if (ID2 == 9	&& ID3 == 5)	nData = gnblMATTuneCWLimit[nIndex];	
		else if (ID2 == 9	&& ID3 == 6)	nData = gnblMATLoadCCWLimit[nIndex];	
		else if (ID2 == 9	&& ID3 == 7)	nData = gnblMATLoadCWLimit[nIndex];	

		else if	(ID2 == 10	&& ID3 == 1)	nData = gnblMATHWLimit[nIndex];	
		else if (ID2 == 10	&& ID3 == 2)	nData = gnblMATTimeOut[nIndex];	

		else if	(ID2 == 11	&& ID3 == 0)	nData = gnblMATFANFail[nIndex];		
		else if (ID2 == 11	&& ID3 == 1)	nData = gnblMATWaterShortage[nIndex];
		else if (ID2 == 11	&& ID3 == 3)	nData = gnblMATCoverOpen[nIndex];	
		else if (ID2 == 11	&& ID3 == 4)	nData = gnblMATWaterLeak1[nIndex];	
		else if (ID2 == 11	&& ID3 == 5)	nData = gnblMATWaterLeak2[nIndex];	

		*Result = TRUE;
	}
	else if(FRC_ID == ID1)		// 2018.03.01
	{
		if(0 == ID2) nData = InOutMap[ID1].pReadData[0];
		else if(1 == ID2) nData = InOutMap[ID1].pReadData[1];
		*Result = TRUE;
	}
	else
	{
		*Result = FALSE;
		nData = 0;
	}

    return nData;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnWriteDigital( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, int SetValue , int* Result ) {
// 	TAutoCriticalSection Lock(csLock);
	int			HighByte	= 0;
	int			LowByte		= 0;
	int			iWriteIndex = 0;
	int			iWritePoint = 0;

	char		msg[256]	= {0};

	BYTE		Temp		= 0;
	BYTE		mask[8]		= {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

	if(ID2 == 999)
	{
		if(SetValue == 1)	gnLogSts = 1;
		else			gnLogSts = 0;
		
		*Result = TRUE;
		return;
	}

	
	if(ID1 == 15)
	{
		if(gnAPCComSts == FALSE)
		{
			*Result = FALSE;
			return;
		}
	}
	else if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
	{
		if(gnKYOSAN_RFCommSts[ID1 - KYOSAN_SRFG_ID] == FALSE)
		{
			*Result = FALSE;
			return;
		}
	}

	_sleep(Sleep_Time);

	iWriteIndex = ID1;			// Mac Address
	iWritePoint = ID2;			// Byte Position of Output Instance 105

	if(ID1 == 15)
	{
		if		(ID2 == 5)	// APC Setpoint Type : Position, Pressure
		{
			Temp = SetValue;
		}
		else if	(ID2 == 0)	// APC Control Type : None, FullClose, FullOpen, Control, Hold
		{
			if(SetValue == 0)
			{
				*Result = TRUE;
				return;
			}
			else if(SetValue > 0)
			{
				Temp = SetValue - 1;
			}
		}

		if(Temp < 0)	Temp = 0;
	}
	else if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
	{
		if(ID2 == 4) 
		{
			if(ID3 >= 0 && ID3 <= 7)
			{
				Temp = InOutMap[iWriteIndex].pWriteData[ID2];
				
				if(SetValue == 0)
				{	
					if( Get_Bit_Sts(Temp, ID3) == 1 )	Temp = Temp - (BYTE)mask[ID3];
				}
				else if	(SetValue == 1)
				{
					if( Get_Bit_Sts(Temp, ID3) == 0 )	Temp = Temp + (BYTE)mask[ID3];
				}
			}
		}
	}
	else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
	{
		if(ID2 == 2) 
		{
			if(ID3 >= 0 && ID3 <= 3)
			{
				Temp = InOutMap[iWriteIndex].pWriteData[ID2];
				
				if(SetValue == 0)
				{	
					if( Get_Bit_Sts(Temp, ID3) == 1 )	Temp = Temp - (BYTE)mask[ID3];
				}
				else if	(SetValue == 1)
				{
					if( Get_Bit_Sts(Temp, ID3) == 0 )	Temp = Temp + (BYTE)mask[ID3];
				}
			}
		}
	}

	InOutMap[iWriteIndex].pWriteData[iWritePoint] = Temp;

	if( DNS_WriteDeviceIo( CardHandle, DeviceConfig[iWriteIndex].MacId, DNS_OUTPUT1, InOutMap[iWriteIndex].pWriteData, DeviceConfig[iWriteIndex].Output1Size ) == FALSE )
	{	
		*Result = FALSE;
	}
	else
	{
		*Result = TRUE;
	}


	return;	
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
double OnReadAnalog( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, int* Result ) {
// 	TAutoCriticalSection Lock(csLock); 
	int		Data_Bit	= 0;
	int		Data		= 0;
	int		HighByte	= 0;
	int		LowByte		= 0;
	int		TempValue   = 0;
	
	char	Data_result[256] = {0};
	char	msg[256] = {0};
	
	int		nData = 0;
	double	dData = 0.0;
	
	int		nIndex = 0;
	
	if(ID1 == FRC_ID )
	{

		if		( ID2 == 1 )	dData = gdFRC_Flow1;
		else if	( ID2 == 2 )	dData = gdFRC_Flow2;
		else if	( ID2 == 3 )	dData = gdFRC_Flow3;
		else if	( ID2 == 4 )	dData = gfFRC_Ratio;
		
		*Result = TRUE;
	}
	else if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
	{
		if (ID1 == KYOSAN_SRFG_ID) nIndex = 0;
		else					   nIndex = 1;
		
		if		(ID2 == 0)	dData = gdblDeliveredPwr[nIndex];
		else if	(ID2 == 2)	dData = gdblFwdPwr[nIndex];
		else if (ID2 == 4)	dData = gdblRefPwr[nIndex];
		else if (ID2 == 6)  dData = gdbRampUpTime[nIndex];
		else if (ID2 == 8)  dData = gdbRampDownTime[nIndex];
		else if (ID2 == 10) dData = gdbArcLevelRefelct[nIndex];
		else if (ID2 == 12)	dData = gdbArcCountLimit[nIndex];
		else if (ID2 == 14) dData = gdblRFRestartTime[nIndex];
		else if (ID2 == 16) dData = gdblRFRecoverTime[nIndex];
		else if (ID2 == 17) dData = gdblRFRecognizeCNT[nIndex];
		else if (ID2 == 19) dData = gdblRFMaskTime[nIndex];
		else if (ID2 == 20) dData = gdblRFMaskDelayTime[nIndex];
		else if (ID2 == 21) dData = gdblRFOutDelayTime[nIndex];
		
		//printf("RF Generator ID2 :%d  data : %02.0f \n", ID2, dData);
		
		*Result = TRUE;
		
	}
	else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
	{
		if (ID1 == KYOSAN_SMAT_ID) nIndex = 0;
		else					   nIndex = 1;
		
		if		(ID2 == 0)	dData = gdblCLPos[nIndex];
		else if (ID2 == 1)	dData = gdblCTPos[nIndex];
		else if (ID2 == 2)	dData = gdblVdc[nIndex];
		else if (ID2 == 4)	dData = gdblVpp[nIndex];
		else if (ID2 == 6)	dData = gdblPCWFlow[nIndex];
		
		*Result = TRUE;
		
		//printf("RF Matcher ID2 :%d  data : %02.0f \n", ID2, dData);
	}

	else if(ID1 >= MFC1_ID && ID1 <= MFC10_ID)
	{
		if		(ID1 == MFC1_ID)			nIndex = 0;
		else if	(ID1 == MFC2_ID)			nIndex = 1;
		else if	(ID1 == MFC3_ID)			nIndex = 2;
		else if	(ID1 == MFC4_ID)			nIndex = 3;
		else if	(ID1 == MFC5_ID)			nIndex = 4;
		else if	(ID1 == MFC6_ID)			nIndex = 5;
		else if	(ID1 == MFC7_ID)			nIndex = 6;
		else if	(ID1 == MFC8_ID)			nIndex = 7;
		else if	(ID1 == MFC9_ID)			nIndex = 8;
		else if	(ID1 == MFC10_ID)			nIndex = 9;

		if		(ID2 == 1)	dData = (gdblMfcFlowrate[nIndex] * ID3 / 32767);
		else if (ID2 == 3)	dData = (gdblMfcSetFlow[nIndex] * ID3 / 32767);

		
		*Result = TRUE;
		
		//printf("RF Matcher ID2 :%d  data : %02.0f \n", ID2, dData);
	}
	
	else
	{
		*Result = FALSE;
		dData = 0.0;
	}
	
	return dData;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//-------------------------------------------------------------------------//
// Function    : DeviceNet_Set                                             //
// Description : 1. Set Status of Each Node                                //
//			   : 2. Digital Channel                                        //
//			   : 3. Analog Channel                                         //
//             : 4. Get_Bit_Sts                                            //
//			   : 4. DNS_WriteDeviceIo                                      //
//			   : 5. FreeLibrary                                            //
// Arguments   : CDeviceNet_Info, CChannel_Info     					   //
// Returns     : 1(success), -1(failure)                                   //
//-------------------------------------------------------------------------//
// ID3 : MIN
// ID4 : MAX

void OnWriteAnalog( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, double SetValue , int* Result ) {
// 	TAutoCriticalSection Lock(csLock);
	int			HighByte = 0;
	int			LowByte = 0;
	int			Data = 0;
	int			iWriteIndex;	
	int			iWritePoint;

	char		msg[256] = {0};
	
	BYTE		Temp = 0;
	BYTE		mask[8] = {0x01, 0x02, 0x04, 0x08, 0x10, 0x20, 0x40, 0x80};

	union DATA	 uTempValue;
	union uValue sData;					// 2018.03.01
	float fSetRatio;					// 2018.03.01

	if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
	{
		if(gnKYOSAN_RFCommSts[ID1 - KYOSAN_SRFG_ID] == FALSE)
		{
			*Result = FALSE;
			return;
		}
	}
	else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
	{/*
 		if(gnKYOSAN_MatCommSts[ID1 - KYOSAN_SMAT_ID] == FALSE)
 		{
 			printf("gnKYOSAN_MatCommSts[ID1 - KYOSAN_SMAT_ID] = false");
 			*Result = FALSE;
 			return;
 		}
		*/
	}
	else if(ID1 >= MFC1_ID && ID1 <= MFC10_ID)
	{
		if(gnMfcCommSts[ID1 - MFC1_ID] == FALSE)
		{
			*Result = FALSE;
			return;
		}
	}
	else if( ID1 == 32 ) 
	{
        if(gnFRCComSts == FALSE) 
		{
			*Result = FALSE;	return;
		}
    }

	do {	// 2018.03.01
 		EnterCriticalSection(&mCS_Lock);

		_sleep(Sleep_Time);
		
		// was not calculated range
		iWriteIndex	= ID1;
		iWritePoint = ID2;
		
		if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
		{
			
			if	(ID2 == 15 || ID2 == 18 || ID2 == 19 || ID2 == 20)
			{
				
				InOutMap[iWriteIndex].pWriteData[iWritePoint] = (int)SetValue;
			}
			else
			{
				uTempValue.iData = (int)SetValue;
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]	= uTempValue.ucData[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]	= uTempValue.ucData[1];
			}
		}
		else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
		{
			// Tuning Cap. Position, Load Cap. Position
			InOutMap[iWriteIndex].pWriteData[iWritePoint] = (int)SetValue;

	//		printf("ID1 = %d, ID2 = %d, data = %f, uTempValue = %d\n", ID1, ID2, data, uTempValue.iData);
		}
		else if(ID1 >= MFC1_ID && ID1 <= MFC10_ID)
		{
			// for Brooks MFC
			if(ID3 != 0)
			{
				uTempValue.iData = (int)(SetValue * 32767 / ID3);
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]	= uTempValue.ucData[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]	= uTempValue.ucData[1];
		
				//printf("iData = %d, ucData = %02c%02c\n",uTempValue.iData,uTempValue.ucData[0],uTempValue.ucData[1]);
		
			}
		}
		
		else if( ID1 == 32 ) 
		{
			// FRC Analog Out
			if( ID2 == 0 ) 
			{
				iWritePoint = 1;

				if(SetValue <= 0) fSetRatio = 20;
				else fSetRatio = (float) (100-SetValue)/SetValue;
				if(fSetRatio > 20) fSetRatio = 20;
				else if(fSetRatio < (float) 1/20) fSetRatio = (float) 1/20;

				if(fSetRatio >= 1) InOutMap[iWriteIndex].pWriteData[0] = 0x02;
				else
				{
					InOutMap[iWriteIndex].pWriteData[0] = 0x01;
					fSetRatio = 1/fSetRatio;
				}

				sData.fValue = fSetRatio;
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0] = sData.ucValue[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1] = sData.ucValue[1];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 2] = sData.ucValue[2];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 3] = sData.ucValue[3];
				printf("FRC Ratio Set %.5f \n", sData.fValue);


				/*
				gnFRC1_DATA = (int)(SetValue * 24576.0 / 100.0);
				uTempValue.iData = gnFRC1_DATA;
			//	printf("FRC1 = [%d], [%d]\n", uTempValue.ucData[0], uTempValue.ucData[1]);

				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]		= uTempValue.ucData[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]		= uTempValue.ucData[1];
				*/

			}
			else if( ID2 == 2 ) {
				iWritePoint = 1;

				if(SetValue >= 100) fSetRatio = 20;
				else fSetRatio = (float) SetValue/(100-SetValue);
				if(fSetRatio > 20) fSetRatio = 20;
				else if(fSetRatio < (float) 1/20) fSetRatio = (float) 1/20;

				if(fSetRatio >= 1) InOutMap[iWriteIndex].pWriteData[0] = 0x02;
				else
				{
					InOutMap[iWriteIndex].pWriteData[0] = 0x01;
					fSetRatio = 1/fSetRatio;
				}

				sData.fValue = fSetRatio;
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0] = sData.ucValue[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1] = sData.ucValue[1];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 2] = sData.ucValue[2];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 3] = sData.ucValue[3];
				printf("FRC Ratio Set %.5f \n", sData.fValue);

				/*
				gnFRC2_DATA = (int)(SetValue * 24576.0 / 100.0);
				uTempValue.iData = gnFRC2_DATA;
			
			//	printf("FRC2 = [%d], [%d]\n", uTempValue.ucData[0], uTempValue.ucData[1]);

				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]		= uTempValue.ucData[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]		= uTempValue.ucData[1];
				*/

			}
	//         else if( ID2 == 4 ) {
	// 			gnFRC3_DATA = (int)(SetValue * 24576.0 / 100.0);
	// 			
	// 			gdFRC_Total = gnFRC1_DATA + gnFRC3_DATA + gnFRC3_DATA;
	// 			
	// 			if( gdFRC_Total != 24576.0 )	gnFRC3_DATA = 24576 - (gnFRC1_DATA + gnFRC2_DATA );
	//           
	// 			if( (gnFRC1_DATA + gnFRC2_DATA) == 0 ) gnFRC3_DATA = (int)(SetValue * 24576.0 / 100.0);
	// 			uTempValue.iData = gnFRC3_DATA;
	// 
	// 
	// 		//	printf(" SET FRC1 =[%d] FRC2 =[%d] FRC3 =[%d]\n",gnFRC1_DATA,gnFRC2_DATA,gnFRC3_DATA);
	// 		//	printf("FRC3 = [%d], [%d]\n", uTempValue.ucData[0], uTempValue.ucData[1]);
	// 
	// 			InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]		= uTempValue.ucData[0];
	// 			InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]		= uTempValue.ucData[1];
	// 
	//         }

			else if(ID2 == 6 ) 
			{
				gdFRC_MaxRate = SetValue;
				gdFRC_MaxRateCalc = SetValue/1000;

				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]		= uTempValue.ucData[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]		= uTempValue.ucData[1];
			}
			else if(ID2 == 8 )		// 2018.03.01
			{
				iWritePoint = 1;

				fSetRatio = (float) SetValue;
				if(fSetRatio >= 1) InOutMap[iWriteIndex].pWriteData[0] = 0x02;
				else
				{
					InOutMap[iWriteIndex].pWriteData[0] = 0x01;
					fSetRatio = 1/fSetRatio;
				}
				
				sData.fValue = fSetRatio;

				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0] = sData.ucValue[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1] = sData.ucValue[1];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 2] = sData.ucValue[2];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 3] = sData.ucValue[3];
				printf("FRC Ratio Set %.5f \n", sData.fValue);
				printf("Write Info : MacID=%d \n", DeviceConfig[iWriteIndex].MacId);

				/*
				// ( DWORD CardHandle, WORD DeviceId, BYTE Service, WORD ClassId, WORD InstanceId, void *ServiceData, WORD Size );
				DNS_SendDeviceExplicit(CardHandle, DeviceConfig[iWriteIndex].MacId, 0x10, 0x04, 12, InOutMap[iWriteIndex].pWriteData, 5);
				*Result = TRUE;
				break;
				*/
			}
			else if(ID2 == 9 )		// 2018.03.01
			{
				iWritePoint = 0;
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0] = (int) SetValue;
				printf("FRC Ratio Switch = %d \n", (int) SetValue);
			}
			else		// 2018.03.01
			{
				sprintf(msg, "Invalid FRC Control I/O Write");
				Make_Log("", msg, FAULT);
				*Result = FALSE;
				break;
			}
			
		 }
		
		if( DNS_WriteDeviceIo( CardHandle, DeviceConfig[iWriteIndex].MacId, DNS_OUTPUT1, InOutMap[iWriteIndex].pWriteData, DeviceConfig[iWriteIndex].Output1Size ) == FALSE )
		{
			sprintf(msg, "[ %02d ] AO - WP [ %02d ] Ch_Num [ %03d ] : Set [ %.0f ]", iWriteIndex, iWritePoint, 0, SetValue);
			Make_Log("", msg, FAULT);

			*Result = FALSE;
		}
		else
		{

			*Result = TRUE;
		}
		
 
	} while(0);

	LeaveCriticalSection(&mCS_Lock);
	return;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnReadString( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, TCHAR* pszRtn , int* Result ) {
// 	TAutoCriticalSection Lock(csLock); 


	
	*Result = TRUE;
// 	if ( !(*Result) ) Error_Message( SI , ID1 , ID2 , ID3 , ID4 );

}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
void OnWriteString( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, TCHAR* SetValue , int* Result ) {
// 	TCriticalSection csLock;
//	if ( !(*Result) ) Error_Message( SO , ID1 , ID2 , ID3 , ID4 );
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------------------





int READ_KYOSAN_RFGEN(int MacId)
{
	char szLog[512] = {0};

	char sModel[32] = {0};

	int iReadIndex;
	int iReadPoint;
	int nIndex;

	DWORD dwTime1, dwTime2;

	union DATA   uTempValu2;

	dwTime1 = GetTickCount();

	iReadIndex = MacId;
	if		(MacId == KYOSAN_SRFG_ID)	nIndex = 0;
	else if	(MacId == KYOSAN_BRFG_ID)	nIndex = 1;


	if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1, InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
	{
		// Exception
		gnKYOSAN_RFCommSts[MacId - KYOSAN_SRFG_ID] = FALSE;
		return - 1;
	}
	else
	{
		gnKYOSAN_RFCommSts[MacId - KYOSAN_SRFG_ID] = TRUE;
	}
	
	iReadPoint = 22;		//  Status Add 22
	gnGenPowerSts_ON[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenPowerSts_OFF[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenFailSts[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnGenRemoteMode[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnGenACSag[nIndex]			= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnGenAbort[nIndex]			= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	gnGenDCShutDown[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	gnGenArcing[nIndex]			= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);
	
	iReadPoint = 23;		// Status Add 23
	gnGenRefLimit[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenVppLimit[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenCurLimit[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnGenVoltLimit[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnGenFWDLimit[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnGenSlaveNotReady[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	gnGenInterlock[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	
	iReadPoint = 24;		// Status Add 24
	gnGenFanFailure[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenOverheat[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenFuseAlarm[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnGenCoverOpen[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnGenAMPFailure[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnGenDCFailure[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	
	iReadPoint = 25;		//  Status Add 25
	gnGenArcCNT[nIndex]			= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenFWDOverTime[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenDCUnitFail[nIndex]	    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnGenDCOverTime[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnGenRefOverTime[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnGenVppOverTime[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	gnGenCurOverTime[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	gnGenVoltOverTime[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);
	
	iReadPoint = 26;		//  Status Add 26
	gnGenRefOverload[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenVppOverload[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenAbnormal_L[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	gnGenAbnormal_H[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);

	iReadPoint = 27;		//  Status Add 27
	gnGenMatcherFail[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenChopperFail[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenMiddleAmpFail[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnGenWaterShortage[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnGenInterlockHW[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);

	iReadPoint = 28;		//  Status Add 28
	gnGenWaterleak[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);



	iReadPoint = 0;		// Delivered power
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblDeliveredPwr[nIndex] = uTempValu2.iData;


	iReadPoint = 2;		// Forward power 
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblFwdPwr[nIndex]	= uTempValu2.iData;
	
	
	iReadPoint = 4;		// Reflect power
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblRefPwr[nIndex]	= uTempValu2.iData;


	iReadPoint = 6;		// Ramp Up Time Set
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdbRampUpTime[nIndex]	= uTempValu2.iData;


	iReadPoint = 8;		// Ramp Down Time Set
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdbRampDownTime[nIndex]	= uTempValu2.iData;

	iReadPoint = 10;	// ARC LEVEL Limit
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdbArcLevelRefelct[nIndex] = uTempValu2.iData;

	iReadPoint = 12;	// Arc Count Set
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdbArcCountLimit[nIndex] = uTempValu2.iData;

	iReadPoint = 14;	// ReStart Time Set
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblRFRestartTime[nIndex] = uTempValu2.iData;

	iReadPoint = 16;	// Recover Start Set		// 1Byte
	gdblRFRecoverTime[nIndex] = InOutMap[iReadIndex].pReadData[iReadPoint];
	
	iReadPoint = 17;	// Recognize Count Set
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblRFRecognizeCNT[nIndex] = uTempValu2.iData;

	iReadPoint = 19;	// Mask Time Set		// 1Byte
	gdblRFMaskTime[nIndex] = InOutMap[iReadIndex].pReadData[iReadPoint];
	
	iReadPoint = 20;	// Mask Delay Time  Set		// 1Byte
	gdblRFMaskDelayTime[nIndex] = InOutMap[iReadIndex].pReadData[iReadPoint];
	
	iReadPoint = 21;	// Out Delay Time  Set		// 1Byte
	gdblRFOutDelayTime[nIndex] = InOutMap[iReadIndex].pReadData[iReadPoint];
	


	dwTime2 = GetTickCount() - dwTime1;

	return 1;
}

int READ_KYOSAN_MATCH(int MacId)
{
	char szLog[512] = {0};

	int iReadIndex;
	int iReadPoint;
	int nIndex;

	DWORD dwTime1, dwTime2;

	union DATA  uTempValu2;

	dwTime1 = GetTickCount();

	iReadIndex = MacId;
	if		(MacId == KYOSAN_SMAT_ID)	nIndex = 0;
	else if	(MacId == KYOSAN_BMAT_ID)	nIndex = 1;

	if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1, InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
	{
		// Exception
		gnKYOSAN_MatCommSts[MacId - KYOSAN_SMAT_ID] = FALSE;
		return - 1;
	}
	else
	{
		gnKYOSAN_MatCommSts[MacId - KYOSAN_SMAT_ID] = TRUE;
	}


	iReadPoint = 0;		// Load Cap. position, 0 ~ 1000
	gdblCLPos[nIndex] = InOutMap[iReadIndex].pReadData[iReadPoint];

	iReadPoint = 1;		// TuningCap. position, 0 ~ 1000
	gdblCTPos[nIndex] = InOutMap[iReadIndex].pReadData[iReadPoint];
	
	iReadPoint = 2;		// Vdc Peak-to-Peak voltage, unit : v
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblVdc[nIndex] = uTempValu2.iData;

	iReadPoint = 4;		// Vpp Peak-to-Peak voltage, unit : v
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblVpp[nIndex]	= uTempValu2.iData;

	iReadPoint = 6;		// Matcher Water Flow
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblPCWFlow[nIndex]	= uTempValu2.iData * 0.1;

	iReadPoint = 8;		//  Status Add 8
	gnblMATControl[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnblMATRFSts[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnblMATFail[nIndex]			= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnblMATMode[nIndex]			= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnblMATTuneCCW[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnblMATTuneCW[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	gnblMATLoadCCW[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	gnblMATLoadCW[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);

	iReadPoint = 9;		//  Status Add 9
	gnblMATTuneCCWLimit[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnblMATTuneCWLimit[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	gnblMATLoadCCWLimit[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	gnblMATLoadCWLimit[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);

	iReadPoint = 10;		//  Status Add 10
	gnblMATHWLimit[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnblMATTimeOut[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);

	iReadPoint = 11;		//  Status Add 11
	gnblMATFANFail[nIndex]		= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnblMATWaterShortage[nIndex]= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnblMATCoverOpen[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnblMATWaterLeak1[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnblMATWaterLeak2[nIndex]	= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);


//	if( nIndex ==0) 	printf("Bias Serial Num : %s \n",gcBMatSerial);



	dwTime2 = GetTickCount() - dwTime1;

	return 1;
}

int READ_FRC(int MacId)
{
	int iReadIndex;

	//----------------------------------------------------
	// 2018.03.01
	// union union_FRC sData;
	union uValue sData;					// 2018.03.01
	//----------------------------------------------------
	
	char szLog[512] = {0};
	
	int nTempValue = 0;
	int idx = 0;
	
	iReadIndex = MacId;
	

 	if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1, InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
 	{
 		gnFRCComSts = FALSE;

		printf("ERRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR \n");
 		return - 1;
 	}
 	else
 	{
 		gnFRCComSts = TRUE;
 	}

	//----------------------------------------------------
	// 2018.03.01
	sData.ucValue[0] = InOutMap[iReadIndex].pReadData[2];
	sData.ucValue[1] = InOutMap[iReadIndex].pReadData[3];
	sData.ucValue[2] = InOutMap[iReadIndex].pReadData[4];
	sData.ucValue[3] = InOutMap[iReadIndex].pReadData[5];

	gfFRC_Ratio = sData.fValue;
	gdFRC_Flow2 = gfFRC_Ratio / (1+gfFRC_Ratio) * 100;
	gdFRC_Flow1 = 1 / (1+gfFRC_Ratio) * 100;
	if(0x01 == InOutMap[iReadIndex].pReadData[1])
	{
		gdFRC_Flow3 = gdFRC_Flow1;
		gdFRC_Flow1 = gdFRC_Flow2;
		gdFRC_Flow2 = gdFRC_Flow3;
	}
	gdFRC_Flow3 = 0;

	//----------------------------------------------------



	/*
    gnFRC_Status = InOutMap[iReadIndex].pReadData[0];
    
    sData.ucData[0] = InOutMap[iReadIndex].pReadData[1 + 0];
	sData.ucData[1] = InOutMap[iReadIndex].pReadData[1 + 1];
 
	gdFRC_Flow1 = (double)(sData.iData * (3000/24576.0));//gdFRC_MaxRate / 24576.0); 2400 1973

//	printf("FRC_Flow1 = %fl, max:%fl Data = %d\n", gdFRC_Flow1,gdFRC_MaxRate, (unsigned int)sData.iData);
	
    sData.ucData[0] = InOutMap[iReadIndex].pReadData[3 + 0];
	sData.ucData[1] = InOutMap[iReadIndex].pReadData[3 + 1];
  
	gdFRC_Flow2 = (double)(sData.iData * (3000/24576.0));//gdFRC_MaxRate / 24576.0);

//	printf("FRC_Flow2 = %fl, max:%fl Data = %d\n", gdFRC_Flow2,gdFRC_MaxRate, (unsigned int)sData.iData);
	
//     sData.ucData[0] = InOutMap[iReadIndex].pReadData[5 + 0];
// 	sData.ucData[1] = InOutMap[iReadIndex].pReadData[5 + 1];
//   
// 	gdFRC_Flow3 = (double)(sData.iData * (3000/24576.0));//gdFRC_MaxRate / 24576.0);
// 
// 	//printf("FRC_Flow3 = %fl, Data = %d\n", gdFRC_Flow3, (unsigned int)sData.iData);
	*/
	
	return 1;
}


int READ_MFC(int MacId)
{
	int iReadIndex;
	union DATA sData;	
	
	char szLog[512] = {0};
	
	int nTempValue = 0;
	int idx = 0;

	iReadIndex = MacId;
	
	if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1, InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
	{
		gnMfcCommSts[MacId - MFC1_ID] = FALSE;
		return - 1;
	}
	else
	{
		gnMfcCommSts[MacId - MFC1_ID] = TRUE;
	}

	// flowrate
	sData.ucData[0] = InOutMap[iReadIndex].pReadData[1];
	sData.ucData[1] = InOutMap[iReadIndex].pReadData[2];
	gdblMfcFlowrate[MacId - MFC1_ID] = (double)sData.iData;

	// Set Flow
	sData.ucData[0] = InOutMap[iReadIndex].pReadData[3];
	sData.ucData[1] = InOutMap[iReadIndex].pReadData[4];
	gdblMfcSetFlow[MacId - MFC1_ID] = (double)sData.iData;

	return 1;
}

void UNLOAD_DEVICENET()
{
	int index = 0;

	if(bStart)
	{
		if (DNS_StopScan(CardHandle) == TRUE)	message_out("IO DeviceNet Driver success to stop scanning", TRUE);
		else									message_out("IO DeviceNet Driver fail to stop scanning",	FALSE);
		bStart = FALSE;
	}

	if(bOnline)
	{
		if(DNS_Offline(CardHandle) == TRUE)		message_out("IO DeviceNet Driver success to offline",		TRUE);
		else									message_out("IO DeviceNet Driver fail to stop offline",		FALSE);
		bOnline = FALSE;
	}
	
	if(bOpen)
	{
		if(DNS_CloseCard(CardHandle) == TRUE)	message_out("IO DeviceNet Driver success to close card",	TRUE);
		else									message_out("IO DeviceNet Driver fail to close card",		FALSE);

		if(DNS_FreeDriver() == TRUE)			message_out("IO DeviceNet Driver success to free driver",	TRUE);
		else									message_out("IO DeviceNet Driver fail to free driver",		FALSE);

		FreeLibrary(hLibInst);
		FreeLibrary(hErrInst);
		bOpen = FALSE;
	}

	for(index = 0; index < 64; index++)
	{
		free(InOutMap[index].pReadData);
		free(InOutMap[index].pWriteData);
	}

	return;
}



//-------------------------------------------------------------------------//
int InitLog(char *csPath)
{
	char sz_drive[128] = {0};
	char sz_dir[512]   = {0};
	char sz_ext[128]   = {0};
	char sz_fname[128] = {0};

	char sz_FullPath[1024] = {0};
	SYSTEMTIME st;

	GetLocalTime(&st);

	pFileHandle = INVALID_HANDLE_VALUE;


	if(strlen(gszPath_origin) < 1)
	{
		strcpy(gszPath_origin, csPath);
		strcpy(gszPath, csPath);
	}
	else
	{
		strcpy(gszPath, gszPath_origin);
	}

	

	_splitpath(gszPath, sz_drive, sz_dir, sz_fname, sz_ext);

	sprintf(gszPath, "%s%s%s%s", sz_drive, sz_dir, sz_fname, sz_ext);
	sprintf(gszFileName, "%00004d%002d%002d_%002d%002d%002d_%s", st.wYear,st.wMonth, st.wDay, st.wHour,st.wMinute, st.wSecond, sz_fname);
	sprintf(gszPath, "%s%s%s%s", sz_drive, sz_dir, gszFileName, sz_ext);
	sprintf(sz_FullPath, "%s%s", sz_drive, sz_dir);


	sprintf(gszFullPath, sz_FullPath);

	CreateDirectory(sz_FullPath, NULL);
	return 1;
}


//-------------------------------------------------------------------------//
int MakeFile()
{
	FILE *in;
	int flag  = 1;
	int count = 1;
	char msg[512] = {0};

	in = fopen(gszPath,"a+");
	if(in == NULL)	
	{
		flag = -1;
		pFileHandle = CreateFile(gszPath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		SetFilePointer(pFileHandle,0,0,FILE_END);
	}
	else
	{
		flag = -1;
		fclose(in);
	}


	if(pFileHandle != INVALID_HANDLE_VALUE)
	{
		CloseHandle(pFileHandle);
		pFileHandle = INVALID_HANDLE_VALUE;
		// file 생성 재시도.
		pFileHandle = CreateFile(gszPath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
		SetFilePointer(pFileHandle,0,0,FILE_END);
	}
	
	print_log("================================================================================", EVENT);
	sprintf(msg, "=================== %-*s ===================", 50, gszPath);
	print_log(msg, EVENT);
	print_log("================================================================================", EVENT);

	return 1;
}

//-------------------------------------------------------------------------//
int OutputLog(char *pstr_Msg)
{
	DWORD dwWritten;
	DWORD dwSize;

	if(strlen(pstr_Msg) < 1 || strlen(gszPath) < 1)
	{
		return -1;
	}

	pFileHandle = CreateFile(gszPath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	dwSize = GetFileSize(pFileHandle, NULL);
	if(dwSize > 50 * FILE_SIZE) // file size >? 50 mega bytes.
	{
		CloseHandle(pFileHandle);

		pFileHandle = INVALID_HANDLE_VALUE;
		_sleep(10);

		InitLog("");
		MakeFile();
		
		pFileHandle = CreateFile(gszPath, GENERIC_WRITE|GENERIC_READ, FILE_SHARE_READ|FILE_SHARE_WRITE, NULL, OPEN_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
	}

	if(pFileHandle != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(pFileHandle, 0, 0, FILE_END);

		WriteFile(pFileHandle, pstr_Msg,  strlen(pstr_Msg),  &dwWritten, NULL);
		CloseHandle(pFileHandle);
		pFileHandle = INVALID_HANDLE_VALUE;
	}

	return 1;
}

//-------------------------------------------------------------------------//
void print_log(char	*pstr_Log, int nLevel)
{
	char str_Time[512] = {0};
	SYSTEMTIME st;

	GetLocalTime(&st);

	if(nLevel == -1)
	{
		sprintf(str_Time, "%00004d/%02d/%02d %002d:%002d:%002d:%003d  [ERROR]  %s\r\n",	st.wYear, st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, pstr_Log);
	}
	else
	{
		sprintf(str_Time,	"%00004d/%02d/%02d %002d:%002d:%002d:%003d  [EVENT]  %s\r\n", st.wYear,st.wMonth, st.wDay, st.wHour, st.wMinute, st.wSecond, st.wMilliseconds, pstr_Log);
	}
	
	OutputLog(str_Time);
}

//-------------------------------------------------------------------------//
int Make_Log(char* pstr_Msg1, char* pstr_Msg2, int nLvl)
{
	char str_Msg[512] = {0};

	sprintf(str_Msg, "[%-*s]  %s", 10, pstr_Msg1, pstr_Msg2);
	print_log(str_Msg, nLvl);

	return 1;
}


int GetProcAddresses()
{
	 if ( (DNS_AddDevice = ( dnsAddDevice )
		GetProcAddress( hLibInst, "DNS_AddDevice" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_DeleteDevice = ( dnsDeleteDevice )
		GetProcAddress( hLibInst, "DNS_DeleteDevice" )) == NULL )
    {
        return -1;
    }
    if ( (DNS_CloseCard = ( dnsCloseCard )
		GetProcAddress( hLibInst, "DNS_CloseCard" )) == NULL)
	{
        return -1;
    }
    if ( (DNS_Driver = ( dnsDriver )
		GetProcAddress( hLibInst, "DNS_Driver" )) == NULL)
	{
        return -1;
    }
    if ( (DNS_FreeDriver = ( dnsFreeDriver )
		GetProcAddress( hLibInst, "DNS_FreeDriver" )) == NULL)
	{
        return -1;
    }
    if ( (DNS_GetBusStatus = ( dnsGetBusStatus )
		GetProcAddress( hLibInst, "DNS_GetBusStatus" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetClientEvent = ( dnsGetClientEvent )
		GetProcAddress( hLibInst, "DNS_GetClientEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetClientStatus = ( dnsGetClientStatus )
		GetProcAddress( hLibInst, "DNS_GetClientStatus" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetDevice = ( dnsGetDevice )
		GetProcAddress( hLibInst, "DNS_GetDevice" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetDeviceEvent = ( dnsGetDeviceEvent )
		GetProcAddress( hLibInst, "DNS_GetDeviceEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetDevicePath = ( dnsGetDevicePath )
		GetProcAddress( hLibInst, "DNS_GetDevicePath" )) == NULL)
    {
        return -1;
    }
	if ( (DNS_GetDeviceStatus = ( dnsGetDeviceStatus )
		GetProcAddress( hLibInst, "DNS_GetDeviceStatus" )) == NULL)
    {
        return -1;
    }
	if ( (DNS_GetModuleHeader = ( dnsGetModuleHeader )
		GetProcAddress( hLibInst, "DNS_GetModuleHeader" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetServerEvent = ( dnsGetServerEvent )
		GetProcAddress( hLibInst, "DNS_GetServerEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetServerPath = ( dnsGetServerPath )
		GetProcAddress( hLibInst, "DNS_GetServerPath" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_GetServerStatus = ( dnsGetServerStatus )
		GetProcAddress( hLibInst, "DNS_GetServerStatus" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_InitializePathBuffer = ( dnsInitializePathBuffer )
		GetProcAddress( hLibInst, "DNS_InitializePathBuffer")) == NULL)
    {
        return -1;
    }
    if ( (DNS_IoActive = ( dnsIoActive )
		GetProcAddress( hLibInst, "DNS_IoActive")) == NULL)
    {
        return -1;
    }
    if ( (DNS_IoIdle = ( dnsIoIdle )
		GetProcAddress( hLibInst, "DNS_IoIdle")) == NULL)
	{
        return -1;
    }
    if ( (DNS_LoadDriver = ( dnsLoadDriver )
		GetProcAddress( hLibInst, "DNS_LoadDriver")) == NULL)
    {
        return -1;
    }
    if ( (DNS_Log = ( dnsLog )GetProcAddress( hLibInst, "DNS_Log")) == NULL)
    {
        return -1;
    }
	if ( (DNS_Offline = ( dnsOffline )
		GetProcAddress( hLibInst, "DNS_Offline" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_Online = ( dnsOnline )
		GetProcAddress( hLibInst, "DNS_Online" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_OpenCard = ( dnsOpenCard )
		GetProcAddress( hLibInst, "DNS_OpenCard" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_ReadDeviceIo = ( dnsReadDeviceIo )
		GetProcAddress( hLibInst, "DNS_ReadDeviceIo" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_ReadServerIo = ( dnsReadServerIo )
		GetProcAddress( hLibInst, "DNS_ReadServerIo" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_ReceiveDeviceExplicit = ( dnsReceiveDeviceExplicit )
		GetProcAddress( hLibInst, "DNS_ReceiveDeviceExplicit" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_ReceiveServerExplicit = ( dnsReceiveServerExplicit )
		GetProcAddress( hLibInst, "DNS_ReceiveServerExplicit" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_RegisterBusStatusEvent = ( dnsRegisterBusStatusEvent )
		GetProcAddress( hLibInst, "DNS_RegisterBusStatusEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_RegisterClientEvent = ( dnsRegisterClientEvent )
		GetProcAddress( hLibInst, "DNS_RegisterClientEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_RegisterDeviceEvent = ( dnsRegisterDeviceEvent )
		GetProcAddress( hLibInst, "DNS_RegisterDeviceEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_RegisterServerEvent = ( dnsRegisterServerEvent )
		GetProcAddress( hLibInst, "DNS_RegisterServerEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_SendDeviceExplicit = ( dnsSendDeviceExplicit )
		GetProcAddress( hLibInst, "DNS_SendDeviceExplicit" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_SendServerExplicit = ( dnsSendServerExplicit )
		GetProcAddress( hLibInst, "DNS_SendServerExplicit" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_SetAccessTimeout = ( dnsSetAccessTimeout )
		GetProcAddress( hLibInst, "DNS_SetAccessTimeout")) == NULL)
    {
        return -1;
    }
    if ( (DNS_SetEventNotificationInterval = ( dnsSetEventNotificationInterval )
		GetProcAddress( hLibInst, "DNS_SetEventNotificationInterval" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_StartScan = ( dnsStartScan )
		GetProcAddress( hLibInst, "DNS_StartScan" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_StopScan = ( dnsStopScan )
		GetProcAddress( hLibInst, "DNS_StopScan" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_UnRegisterBusStatusEvent = ( dnsUnRegisterBusStatusEvent )
		GetProcAddress( hLibInst, "DNS_UnRegisterBusStatusEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_UnRegisterClientEvent = ( dnsUnRegisterClientEvent )
		GetProcAddress( hLibInst, "DNS_UnRegisterClientEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_UnRegisterDeviceEvent = ( dnsUnRegisterDeviceEvent )
		GetProcAddress( hLibInst, "DNS_UnRegisterDeviceEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_UnRegisterServerEvent = ( dnsUnRegisterServerEvent )
		GetProcAddress( hLibInst, "DNS_UnRegisterServerEvent" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_Version = ( dnsVersion )
		GetProcAddress( hLibInst, "DNS_Version" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_WriteDeviceIo = ( dnsWriteDeviceIo )
		GetProcAddress( hLibInst, "DNS_WriteDeviceIo" )) == NULL)
    {
        return -1;
    }
    if ( (DNS_WriteServerIo = ( dnsWriteServerIo )
		GetProcAddress( hLibInst, "DNS_WriteServerIo" )) == NULL)
    {
        return -1;
    }
    return 1;
}