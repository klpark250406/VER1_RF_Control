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

#define KYOSAN_SRFG_ID	15
#define KYOSAN_BRFG_ID  16
#define KYOSAN_SMAT_ID  13
#define KYOSAN_BMAT_ID  14
#define MFC1_ID		   1
#define MFC2_ID		   2
#define MFC3_ID		   3
#define MFC4_ID		   4
#define MFC5_ID		   5
#define MFC6_ID		   6
#define MFC7_ID		   7
#define MFC8_ID		   8
#define MFC9_ID		   9
#define MFC10_ID	   10

#define FRC_ID		   11
#define APC_ID		   12


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


//==========================================================================
// RFPT Generator Read START ===============================================
int gnKYOSAN_RFCommSts[2]	= {0};		// RFPT Generator Commsts

double gdblDeliveredPwr[2]	= {0.0};	// RFPT Input byte0~1     Set Point Power rb (readback)
double gdblRFFreqIn[2]      = {0.0};	// RFPT Input byte2~3     RF Frequency readback (kHz)
double gdblFwdPwr[2]		= {0.0};	// RFPT Input byte4~5     Forward Power In
double gdblRefPwr[2]		= {0.0};	// RFPT Input byte6~7     Reflected Power In
double gdblDeliveryPwr[2]   = {0.0};    // RFPT Input byte8~9     Delivery Power In (W)
double gdbRampUpTime[2]		= {0.0};	// RFPT Input byte19~20   Ramp Up rb
double gdbRampDownTime[2]	= {0.0};	// RFPT Input byte21~22   Ramp Down rb
int gnGenPulseOnOffRb[2]     = {0};     // RFPT Input byte23 bit0 : Pulse On/Off readback
int gnGenPulseMSRb[2]        = {0};     // RFPT Input byte23 bit1 : Master(0)/Slave(1) readback
double gdblPulseFreqRb[2]   = {0.0};    // RFPT Input byte24~25	  Pulse Frequency readback (Hz)
double gdblPulseDutyRb[2]   = {0.0};    // RFPT Input byte26~27	  Pulse Duty readback (0.1%)

// LED Status (byte28) -	RFPT Gen 5장 byte28 LED Status 비트필드
int gnGenACOn[2]             = {0};    // bit0 : AC On
int gnGenInterlockSts[2]     = {0};    // bit1 : Interlock Failure
int gnGenAlarmSts[2]         = {0};    // bit2 : Alarm 발생 (종합)
int gnGenOverTempSts[2]      = {0};    // bit3 : Over Temp
int gnGenPwrLimitSts[2]      = {0};    // bit4 : Power Limit
int gnGenRFOnSts[2]          = {0};    // bit5 : RF On/Off
int gnGenRemoteIn[2]         = {0};    // bit6 : RF Remot in Off 0 / on 1 


// System State (byte29) -	RFPT Gen 5장 byte29 System State 비트필드
int gnGenSysPwrMode[2]       = {0};    // bit0 : Power Mode (0=Fwd, 1=Load)
int gnGenSysRampMode[2]      = {0};    // bit1 : Ramp Mode
int gnGenSysPulseMode[2]     = {0};    // bit2 : Pulse Mode (0=CW, 1=Pulse)
int gnGenSysCEXMode[2]       = {0};    // bit3 : CEX Mode
int gnGenSysCEXLock[2]       = {0};    // bit4 : CEX Lock
int gnGenSysFreqTuning[2]    = {0};    // bit5 : Freq Tuning Mode
int gnGenSysTuningOnOff[2]   = {0};    // bit6 : Tuning on/off
int gnGenSysDCBias[2]        = {0};    // bit7 : DC Bias Mode

// Alarm 16bit (byte30=Low, byte31=High) - RFPT Gen 9장 알람 상세
int gnGenAlm_AUX3V3[2]      = {0};    // bit0  : AUX 3.3V 이상
int gnGenAlm_AUX5V[2]       = {0};    // bit1  : AUX 5V 이상
int gnGenAlm_AUX24V[2]      = {0};    // bit2  : AUX 24V 이상
int gnGenAlm_ACL12[2]       = {0};    // bit3  : AC Phase L1-L2 이상
int gnGenAlm_ACL23[2]       = {0};    // bit4  : AC Phase L2-L3 이상
int gnGenAlm_ACL31[2]       = {0};    // bit5  : AC Phase L3-L1 이상
int gnGenAlm_PFC[2]         = {0};    // bit6  : PFC Fail
int gnGenAlm_MaxPwr[2]      = {0};    // bit7  : Max Power Limit
int gnGenAlm_GateDrv[2]     = {0};    // bit8  : Gate Drv Amp 이상
int gnGenAlm_Fan[2]         = {0};    // bit9  : Fan Fail
int gnGenAlm_OverTemp[2]    = {0};    // bit10 : Over Temp
int gnGenAlm_TopIntlk[2]    = {0};    // bit11 : Top Interlock
int gnGenAlm_BotIntlk[2]    = {0};    // bit12 : Bottom Interlock
int gnGenAlm_RFIntlk[2]     = {0};    // bit13 : RF Interlock
int gnGenAlm_UserIntlk[2]   = {0};    // bit14 : User Interlock
int gnGenAlm_UnderFWD[2]    = {0};    // bit15 : Under FWD Power
// RFPT Generator Read END =================================================
//==========================================================================


//==========================================================================
// RFPT Matcher Read START =================================================
int gnKYOSAN_MatCommSts[2]	= {0};	// RFPT Matcher Commsts

double gdblCLPos[2]			= {0.0};  // byte0~1 : Actual Load Position
double gdblCTPos[2]			= {0.0};  // byte2~3 : Actual Tune Position

double gdblLoadPreset[2]    = {0.0};  // byte4~5 : Actual Load Preset (*0.1 %)
double gdblTunePreset[2]    = {0.0};  // byte6~7 : Actual Tune Preset (*0.1 %)

double gdblVdc[2]			= {0.0};  // byte8~9 : VDC In

// Status (byte10) -	RFPT Matcher 5장 byte10 상태 비트필드
int gnblMATMatcherMode[2]    = {0};    // bit0~3 : Matcher Mode (0=Manual,1=Auto,2=Preset)
int gnblMATRemoteIn[2]       = {0};    // bit4   : Remote In
int gnblMATRFState[2]        = {0};    // bit5   : RF State

// Alarm (byte11) -		RFPT Matcher 9장 알람 상세
int gnblMATAlarm[2]          = {0};    // bit7 : 종합 Alarm
int gnblMATLeak[2]           = {0};    // bit3 : 누수
int gnblMATConn[2]           = {0};    // bit2 : 커넥터 이상
int gnblMATCover[2]          = {0};    // bit1 : 커버 열림
int gnblMATFan[2]            = {0};    // bit0 : 팬 이상

// byte12~21 임피던스/전압/전류	RFPT Matcher 5장 byte12~21
double gdblVSWR[2]           = {0.0};  // byte12~13 : VSWR (수신값*0.01)
double gdblInputR[2]         = {0.0};  // byte14~15 : Input R (*0.1 옴)
double gdblInputX[2]         = {0.0};  // byte16~17 : Input X (*0.1 옴, 부호있음)
double gdblVrms[2]           = {0.0};  // byte18~19 : Vrms (*0.1 V)
double gdblIrms[2]           = {0.0};  // byte20~21 : Irms (*0.1 A)
// RFPT Matcher Read END ===================================================
//==========================================================================
double gdblPCWFlow[2]			= {0.0};




double gdblZ_Load[2]			= {0.0};
double gdblPhi_Load[2]			= {0.0};


int gnAPCException = 0;
int gnAPCAcessMode = 0;
int gnAPCDeviceSts = 0;
int gnAPCValveSts[8] = {0};
int gnAPCSetType = 0;


double gdblAPCSensor1 = 0.0;
double gdblAPCSensor2 = 0.0;
double gdblAPCPosition = 0.0;
double gdblAPCPressure = 0.0;


unsigned short gnFRC1_Status = 0;
double gdAI_FRC_FLOW_Ratio_SetChk[9] = {0.0};  // AI_FRC_FLOW_Ratio_SetChk_CH01

double gdAI_FRC_FLOW_Ratio[9]  = {0.0};		// AI_FRC_FLOW_Ratio_CH01
double gdFRC_Total_Flow_Ratio = 0.0;

double gdAI_FRC_FLOW_SCCM_SetChk[9] = {0.0};  // AI_FRC_FLOW_SCCM_SetChk_CH01
double gdAI_FRC_FLOW_SCCM[9] = {0.0};  // AI_FRC_FLOW_SCCM_CH01
double gdFRC_Total_Flow_SCCM = 0.0;


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
int GetAPCSlaveSts(int value);
int READ_RFGEN(int nMacId);
int READ_KYOSAN_RFGEN(int nMacId);
int READ_KYOSAN_MATCH(int nMacId);
int READ_FRC(int nMacId);
int READ_MFC(int nMacId);
int READ_APCVLV(int nMacId);
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
		READ_FRC(FRC_ID);

        //APC
		READ_APCVLV(APC_ID);
		//RF
		for(nMacId_RF = 13; nMacId_RF <=16; nMacId_RF++)	
		{
 			if		(nMacId_RF == 15  || nMacId_RF == 16)	READ_KYOSAN_RFGEN(nMacId_RF);
			else if	(nMacId_RF == 13 || nMacId_RF == 14)	READ_KYOSAN_MATCH(nMacId_RF);		
			
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
		sprintf(msg, "[%s] Load Error >> DNS_Online : Failure", gsDrv_Name);
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
BOOL OnKillDevice( void* pDrvData , int ID1 , int ID2 , int ID3 , int ID4 , int ID5 , int ID6 , int ID7 , int ID8 , int ID9 , int ID10, TCHAR *szParm ) 
{
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
int OnReadDigital( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, int* Result ) 
{
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
		else if( ID1 == FRC_ID ) // 2026.06.20 HORIBA MUCUBE
        {
            if(DeviceStatus[ID1].StatusCode != DNS_ACTIVE)  gnFRCComSts = FALSE;
            else                                            gnFRCComSts = TRUE;
			
            return gnFRCComSts;
        }
		else if(ID1 == APC_ID)
			{
				if(DeviceStatus[ID1].StatusCode != DNS_ACTIVE)	gnAPCComSts = FALSE;
				else											gnAPCComSts = TRUE;
				
				return gnAPCComSts;
			}
		
	}


	if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
	{
		if	(ID1 == KYOSAN_SRFG_ID)	nIndex = 0;
		else						nIndex = 1;
		
		// ------------------------------------------------------------------
		// byte23 Pulse Control rb 비트 
		// RFPT Gen 사양서 챕터11 Input offset 23
		// CFG: eDI_SRF_PulseOnOffRb(ID2=23,ID3=0), eDI_SRF_PulseMSRb(ID2=23,ID3=1)
		// ------------------------------------------------------------------
		if		(ID2 == 23 && ID3 == 0)	nData = gnGenPulseOnOffRb[nIndex];
		else if (ID2 == 23 && ID3 == 1)	nData = gnGenPulseMSRb[nIndex];
		
		// ------------------------------------------------------------------
		// byte28 LED Status 비트 - : RFPT Gen 사양서 챕터5 byte28
		// ------------------------------------------------------------------
		else if	(ID2 == 28 && ID3 == 0)	nData = gnGenACOn[nIndex];
		else if	(ID2 == 28 && ID3 == 1)	nData = gnGenInterlockSts[nIndex];
		else if	(ID2 == 28 && ID3 == 2)	nData = gnGenAlarmSts[nIndex];
		else if	(ID2 == 28 && ID3 == 3)	nData = gnGenOverTempSts[nIndex];
		else if	(ID2 == 28 && ID3 == 4)	nData = gnGenPwrLimitSts[nIndex];
		else if	(ID2 == 28 && ID3 == 5)	nData = gnGenRFOnSts[nIndex];
		else if	(ID2 == 28 && ID3 == 6)	nData = gnGenRemoteIn[nIndex];
		
		
		// ------------------------------------------------------------------
		// byte29 System State 비트 : RFPT Gen 사양서 챕터5 byte29
		// ------------------------------------------------------------------
		else if	(ID2 == 29 && ID3 == 0)	nData = gnGenSysPwrMode[nIndex];
		else if	(ID2 == 29 && ID3 == 1)	nData = gnGenSysRampMode[nIndex];
		else if	(ID2 == 29 && ID3 == 2)	nData = gnGenSysPulseMode[nIndex];
		else if	(ID2 == 29 && ID3 == 3)	nData = gnGenSysCEXMode[nIndex];
		else if	(ID2 == 29 && ID3 == 4)	nData = gnGenSysCEXLock[nIndex];
		else if	(ID2 == 29 && ID3 == 5)	nData = gnGenSysFreqTuning[nIndex];
		else if	(ID2 == 29 && ID3 == 6)	nData = gnGenSysTuningOnOff[nIndex];
		else if	(ID2 == 29 && ID3 == 7)	nData = gnGenSysDCBias[nIndex];
		
		// ------------------------------------------------------------------
		// byte30 Alarm 하위 바이트 (bit0~7) : RFPT Gen 사양서 챕터9
		// ------------------------------------------------------------------
		else if	(ID2 == 30 && ID3 == 0)	nData = gnGenAlm_AUX3V3[nIndex];
		else if	(ID2 == 30 && ID3 == 1)	nData = gnGenAlm_AUX5V[nIndex];
		else if	(ID2 == 30 && ID3 == 2)	nData = gnGenAlm_AUX24V[nIndex];
		else if	(ID2 == 30 && ID3 == 3)	nData = gnGenAlm_ACL12[nIndex];
		else if	(ID2 == 30 && ID3 == 4)	nData = gnGenAlm_ACL23[nIndex];
		else if	(ID2 == 30 && ID3 == 5)	nData = gnGenAlm_ACL31[nIndex];
		else if	(ID2 == 30 && ID3 == 6)	nData = gnGenAlm_PFC[nIndex];
		else if	(ID2 == 30 && ID3 == 7)	nData = gnGenAlm_MaxPwr[nIndex];
		
		// ------------------------------------------------------------------
		// byte31 Alarm 상위 바이트 (bit8~15) : RFPT Gen 사양서 챕터9
		// ------------------------------------------------------------------
		else if	(ID2 == 31 && ID3 == 0)	nData = gnGenAlm_GateDrv[nIndex];
		else if	(ID2 == 31 && ID3 == 1)	nData = gnGenAlm_Fan[nIndex];
		else if	(ID2 == 31 && ID3 == 2)	nData = gnGenAlm_OverTemp[nIndex];
		else if	(ID2 == 31 && ID3 == 3)	nData = gnGenAlm_TopIntlk[nIndex];
		else if	(ID2 == 31 && ID3 == 4)	nData = gnGenAlm_BotIntlk[nIndex];
		else if	(ID2 == 31 && ID3 == 5)	nData = gnGenAlm_RFIntlk[nIndex];
		else if	(ID2 == 31 && ID3 == 6)	nData = gnGenAlm_UserIntlk[nIndex];
		else if	(ID2 == 31 && ID3 == 7)	nData = gnGenAlm_UnderFWD[nIndex];


		*Result = TRUE;
	}


	else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
	{
		if	(ID1 == KYOSAN_SMAT_ID)	nIndex = 0;
		else						nIndex = 1;

		// ------------------------------------------------------------------
		// byte10 Status 비트 : RFPT Mat 사양서 챕터5 byte10
		// ------------------------------------------------------------------
		if		(ID2 == 10 && ID3 == 0)	nData = gnblMATMatcherMode[nIndex]; // bit0~3 Mode값
		else if	(ID2 == 10 && ID3 == 4)	nData = gnblMATRemoteIn[nIndex];    // bit4 Remote In
		else if	(ID2 == 10 && ID3 == 5)	nData = gnblMATRFState[nIndex];     // bit5 RF State

		// ------------------------------------------------------------------
		// byte11 Alarm 비트 : RFPT Mat 사양서 챕터5 byte11
		// ------------------------------------------------------------------
		else if	(ID2 == 11 && ID3 == 0)	nData = gnblMATFan[nIndex];
		else if	(ID2 == 11 && ID3 == 1)	nData = gnblMATCover[nIndex];
		else if	(ID2 == 11 && ID3 == 2)	nData = gnblMATConn[nIndex];
		else if	(ID2 == 11 && ID3 == 3)	nData = gnblMATLeak[nIndex];
		else if	(ID2 == 11 && ID3 == 7)	nData = gnblMATAlarm[nIndex];

		*Result = TRUE;
	}


	else if(ID1 == APC_ID)
	{
		if		(ID2 == 0)		nData = gnAPCException;
		else if	(ID2 == 7)		nData = gnAPCAcessMode;
		else if	(ID2 == 8)		nData = gnAPCDeviceSts;
		else if (ID2 == 9)		nData = gnAPCValveSts[0];
		else if (ID2 == 10)		nData = gnAPCValveSts[1];
		else if (ID2 == 11)		nData = gnAPCValveSts[2];
		else if (ID2 == 12)		nData = gnAPCValveSts[3];
		else if (ID2 == 13)		nData = gnAPCValveSts[4];
		else if (ID2 == 14)		nData = gnAPCValveSts[5];
		else if (ID2 == 15)		nData = gnAPCValveSts[6];
		else if (ID2 == 16)		nData = gnAPCValveSts[7];
		else if (ID2 == 99)		nData = gnAPCComSts;

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
void OnWriteDigital( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, int SetValue , int* Result ) 
{
	
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

	
	if(ID1 == APC_ID)
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

	if(ID1 == APC_ID)
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
		// ------------------------------------------------------------------
		// byte4  : Control 비트필드   : RFPT Gen 챕터5 byte4
		// byte10 : Pulse Control 비트필드 : RFPT Gen 챕터5 byte10 
		// byte19 : Tuning Control 비트필드 : RFPT Gen 챕터5 byte19 // 미사용 함으로, 조건문에서 삭제. 
		// 3가지 바이트 모두 동일한 비트 토글 방식 적용
		// ------------------------------------------------------------------
		if(ID2 == 4 || ID2 == 10 )  
		{
			if(ID3 >= 0 && ID3 <= 7)
			{
				Temp = InOutMap[iWriteIndex].pWriteData[ID2];
				
				if(SetValue == 0)
				{	
					if( Get_Bit_Sts(Temp, ID3) == 1 )	Temp = Temp - (BYTE)mask[ID3];
				}
				else if(SetValue == 1)
				{
					if( Get_Bit_Sts(Temp, ID3) == 0 )	Temp = Temp + (BYTE)mask[ID3];
				}
			}
		}
	}
	
	else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
	{
		// ------------------------------------------------------------------
		// RFPT Matcher 출력 byte8 처리
		// ------------------------------------------------------------------
		if(ID2 == 8)
		{
			Temp = InOutMap[iWriteIndex].pWriteData[ID2];

			if(ID3 >= 4 && ID3 <= 5)
			{
				// ID3 4=AlarmRST(bit4), ID3 5=RemoteOut(bit5)
				// RFPT Mat 사양서 챕터5 byte8 bit4=Alarm RST, bit5=Remote Out
				if(SetValue == 0)
				{
					if( Get_Bit_Sts(Temp, ID3) == 1 )	Temp = Temp - (BYTE)mask[ID3];
				}
				else if(SetValue == 1)
				{
					if( Get_Bit_Sts(Temp, ID3) == 0 )	Temp = Temp + (BYTE)mask[ID3];
				}
			}
			else if(ID3 == 0)
			{
				// ID3 0 = MatcherMode (bit0~3), 0~3 범위의 MODE 값 직접 설정
				// RFPT Mat 사양서 챕터5 byte8 bit0~3 Matcher Mode (0=Manual,1=Auto,2=Preset)
				// bit0~3을 마스크 후 SetValue(0/1/2)를 하위 4bit에 set
				Temp = (Temp & 0xF0) | ((BYTE)SetValue & 0x0F);
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
double OnReadAnalog( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, int* Result ) 
{
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
        switch(ID2)
		{
			case 1:
			case 2:
			case 3:
				dData = gdAI_FRC_FLOW_Ratio_SetChk[ID2-1]; break;
				
			case 11:
			case 12: 
			case 13: 
				dData = gdAI_FRC_FLOW_Ratio[(ID2%10)-1]; break;	
				
			case 20: 
				dData = gdFRC_Total_Flow_Ratio; break;  
				
			case 31:
			case 32:
			case 33: 
				dData = gdAI_FRC_FLOW_SCCM_SetChk[(ID2%30)-1]; break;
				
			case 41:
			case 42:
			case 43: 
				dData = gdAI_FRC_FLOW_SCCM[(ID2%40)-1]; break;
			
			case 50: 
				dData = gdFRC_Total_Flow_SCCM;  break;  
				
			default: 
				*Result = FALSE; 
				dData = 0.0;     
				break;
		}
	}
	else if(ID1 == APC_ID)	// APC : default 4 byts per 1 channel
	{
		if		(ID2 == 1)	dData = gdblAPCSensor1*0.2;// * 0.1;	// for 1 Torr	: 0 ~ 10000
		else if	(ID2 == 3)	dData = gdblAPCSensor2*0.01;// * 0.025;	// for 0.25 Torr : 0 ~ 10000
		else if	(ID2 == 5)  dData = gdblAPCPosition;
		
		*Result = TRUE;
	}

	else if(ID1 == KYOSAN_SRFG_ID || ID1 == KYOSAN_BRFG_ID)
	{
		if (ID1 == KYOSAN_SRFG_ID) nIndex = 0;
		else					   nIndex = 1;
		
		if		(ID2 == 0)	dData = gdblDeliveredPwr[nIndex];	// byte0~1 : Set Point Power readback : RFPT Gen 챕터11 Input offset 0~1
		else if	(ID2 == 2)	dData = gdblRFFreqIn[nIndex];		// byte2~3 : RF Frequency In readback : RFPT Gen 챕터11 Input offset 2~3
		else if	(ID2 == 4)	dData = gdblFwdPwr[nIndex];			// byte4~5 : Forward Power In -: RFPT Gen 챕터11 Input offset 4~5
		else if	(ID2 == 6)	dData = gdblRefPwr[nIndex];			// byte6~7 : Reflected Power In -: RFPT Gen 챕터11 Input offset 6~7
		else if	(ID2 == 8)	dData = gdblDeliveryPwr[nIndex];	// byte8~9 : Delivery Power In : RFPT Gen 챕터11 Input offset 8~9
		// ------------------------------------------------------------------
		// byte18 : Mode readback (nibble) 
		// RFPT Gen 사양서 챕터11 Input offset 18
		// 1byte 직접 반환 (nibble: 하위4bit=RegMode/상위4bit=RampMode)
		// ------------------------------------------------------------------
		else if	(ID2 == 18)	dData = (double)InOutMap[ID1].pReadData[18];  // byte18
		else if	(ID2 == 19)	dData = gdbRampUpTime[nIndex];		// byte19~20 : Ramp Up Time rb : RFPT Gen 챕터11 Input offset 19~20
		else if	(ID2 == 21)	dData = gdbRampDownTime[nIndex];	// byte21~22 : Ramp Down Time rb : RFPT Gen 챕터11 Input offset 21~22
		else if	(ID2 == 24)	dData = gdblPulseFreqRb[nIndex];	// byte24~25 : Pulse Frequency rb : RFPT Gen 챕터11 Input offset 24~25
		else if	(ID2 == 26)	dData = gdblPulseDutyRb[nIndex];	// byte26~27 : Pulse Duty rb : RFPT Gen 챕터11 Input offset 26~27
		
		*Result = TRUE;
	}

	else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
	{
		if (ID1 == KYOSAN_SMAT_ID) nIndex = 0;
		else					   nIndex = 1;
		
		if		(ID2 == 0)	dData = gdblCLPos[nIndex];		// byte0~1 : Actual Load Position : RFPT Mat 챕터11 Input offset 0~1 (*0.1 %)
		else if	(ID2 == 2)	dData = gdblCTPos[nIndex];		// byte2~3 : Actual Tune Position : RFPT Mat 챕터11 Input offset 2~3 (*0.1 %)
		else if	(ID2 == 4)	dData = gdblLoadPreset[nIndex]; // byte4~5 : Actual Load Preset : RFPT Mat 챕터11 Input offset 4~5
		else if	(ID2 == 6)	dData = gdblTunePreset[nIndex]; // byte6~7 : Actual Tune Preset : RFPT Mat 챕터11 Input offset 6~7
		else if	(ID2 == 8)	dData = gdblVdc[nIndex];		// byte8~9 : VDC In : RFPT Mat 챕터11 Input offset 8~9 (1V 단위)
		else if	(ID2 == 12)	dData = gdblVSWR[nIndex];		// byte12~13 : VSWR : RFPT Mat 챕터11 Input offset 12~13 (*0.01)
		else if	(ID2 == 14)	dData = gdblInputR[nIndex];		// byte14~15 : Input R : RFPT Mat 챕터11 Input offset 14~15 (*0.1 옴)
		else if	(ID2 == 16)	dData = gdblInputX[nIndex];		// byte16~17 : Input X : RFPT Mat 챕터11 Input offset 16~17 (*0.1 옴)
		else if	(ID2 == 18)	dData = gdblVrms[nIndex];		// byte18~19 : Vrms : RFPT Mat 챕터11 Input offset 18~19 (*0.1 V)
		else if	(ID2 == 20)	dData = gdblIrms[nIndex];		// byte20~21 : Irms : RFPT Mat 챕터11 Input offset 20~21 (*0.1 A)
		
		*Result = TRUE;
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

void OnWriteAnalog( void* pDrvData , void* , int ID1, int ID2, int ID3, int ID4, double SetValue , int* Result ) 
{
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
	union uValue  uTempfValue4;			// 2026.06.20

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
	else if( ID1 == FRC_ID ) 
	{
        if(gnFRCComSts == FALSE) 
		{
			*Result = FALSE;	return;
		}
    }
	
		else if(ID1 == APC_ID)
	{
		if(gnAPCComSts == FALSE)
		{
			*Result = FALSE;
			return;
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
			// ------------------------------------------------------------------
			// RFPT Gen 출력은 byte5(Mode nibble) 제외 모두 2byte uint16 LE 방식
			// RFPT Gen 사양서 챕터11 Output Map - 모든 바이트값 uint16 LE
			// ------------------------------------------------------------------
			if(ID2 == 5)
			{
				// Mode nibble byte - 1byte 직접 set
				// RFPT Gen 사양서 챕터11 Output offset 5, nibble byte
				// 하위4bit=RegMode(0~3), 상위4bit=RampMode(0~2)
				InOutMap[iWriteIndex].pWriteData[iWritePoint] = (int)SetValue;
			}
			else
			{
				// 나머지 모든 AO 항목 : 2byte uint16 LE 처리
				// RFPT Gen 사양서 챕터11 Output Map - uint16 LE
				uTempValue.iData = (int)SetValue;
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0] = uTempValue.ucData[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1] = uTempValue.ucData[1];
			}
		}

		else if(ID1 == KYOSAN_SMAT_ID || ID1 == KYOSAN_BMAT_ID)
		{
			// ------------------------------------------------------------------
			// RFPT Matcher 위치/프리셋 모두 uint16 LE ×10 변환 후 2byte 쓰기 
			// .cfg에서 scale 변환 진행. code에서 미진행.
			// byte0~1=LoadPos, 
			// byte2~3=TunePos, 
			// byte4~5=LoadPreset, 
			// byte6~7=TunePreset					
			// ------------------------------------------------------------------
			uTempValue.iData = (int)(SetValue);         
			InOutMap[iWriteIndex].pWriteData[iWritePoint + 0] = uTempValue.ucData[0];
			InOutMap[iWriteIndex].pWriteData[iWritePoint + 1] = uTempValue.ucData[1];
		}


		else if(ID1 >= MFC1_ID && ID1 <= MFC10_ID)
		{
			// for Brooks MFC
			if(ID3 != 0)
			{
				uTempValue.iData = (int)(SetValue * 32767 / ID3);
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]	= uTempValue.ucData[0];
				InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]	= uTempValue.ucData[1];				
			}
		}
		
	else if( ID1 == FRC_ID ) 
	{
		// 상위의 data -> 4byte float Converter 
		uTempfValue4.fValue = (float)SetValue;
		
		// IO 정의 파일: AO_FRC_FLOW_CH01 ... ID2 = 1
		InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]	= uTempfValue4.ucValue[0];
		InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]	= uTempfValue4.ucValue[1];
		InOutMap[iWriteIndex].pWriteData[iWritePoint + 2]	= uTempfValue4.ucValue[2];
		InOutMap[iWriteIndex].pWriteData[iWritePoint + 3]	= uTempfValue4.ucValue[3];
		
     }
		
	else if(ID1 == APC_ID)	// apc
	{
		if(ID2 == 1)		// APC Pressure Setpoint
		{
			uTempValue.iData = (short)SetValue * 5;//* 10;
		}
		else if(ID2 == 3)	// APC Position Setpoint
		{
			uTempValue.iData = (short)SetValue * 10;
		}

		InOutMap[iWriteIndex].pWriteData[iWritePoint + 0]		= uTempValue.ucData[0];
		InOutMap[iWriteIndex].pWriteData[iWritePoint + 1]		= uTempValue.ucData[1];
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


int READ_KYOSAN_RFGEN(int MacId)
{
	char szLog[512]  = {0};
	int  iReadIndex;
	int  iReadPoint;
	int  nIndex;

	union DATA uTempValu2;                    //  (2Byte 조합용)

	iReadIndex = MacId;
	if      (MacId == KYOSAN_SRFG_ID) nIndex = 0;
	else if (MacId == KYOSAN_BRFG_ID) nIndex = 1;

	if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1,
	                      InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
	{
		gnKYOSAN_RFCommSts[MacId - KYOSAN_SRFG_ID] = FALSE;
		return -1;
	}
	else
	{
		gnKYOSAN_RFCommSts[MacId - KYOSAN_SRFG_ID] = TRUE;
	}

	// ------------------------------------------------------------------
	// byte0~1 : Set Point Power (readback)
	// RFPT Gen 사양서 챕터11 Input offset 0~1, uint16 LE, 1W
	// ------------------------------------------------------------------
	iReadPoint = 0;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblDeliveredPwr[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte2~3 : RF Frequency In (readback)
	// RFPT Gen 사양서 챕터11 Input offset 2~3, uint16 LE, 1kHz
	// ------------------------------------------------------------------
	iReadPoint = 2;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblRFFreqIn[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte4~5 : Forward Power In
	// RFPT Gen 사양서 챕터11 Input offset 4~5, uint16 LE, 1W
	// ------------------------------------------------------------------
	iReadPoint = 4;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblFwdPwr[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte6~7 : Reflected Power In
	// RFPT Gen 사양서 챕터11 Input offset 6~7, uint16 LE, 1W
	// ------------------------------------------------------------------
	iReadPoint = 6;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblRefPwr[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte8~9 : Delivery Power In 
	// RFPT Gen 사양서 챕터11 Input offset 8~9, uint16 LE, 1W
	// ------------------------------------------------------------------
	iReadPoint = 8;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblDeliveryPwr[nIndex] = uTempValu2.iData;

	// byte10~17 : 미사용 (Real/Image Gamma, RF Phase, Temperature)

	// ------------------------------------------------------------------
	// byte18 : Mode readback (nibble)
	// RFPT Gen 사양서 챕터11 Input offset 18, nibble
	// 하위 4bit = Regulation Mode, 
	// 상위 4bit = Ramp Mode
	// ------------------------------------------------------------------
	iReadPoint = 18;
	// byte 자체를 OnReadAnalog에서 활용 (nibble 분리는 상위에서 처리)
	// → 필요 시 별도 변수로 저장 가능. 현재 AI ID2=18로 직접 반환 방식 채택

	// ------------------------------------------------------------------
	// byte19~20 : Ramp Up Time (readback)
	// RFPT Gen 사양서 챕터11 Input offset 19~20, uint16 LE
	// ------------------------------------------------------------------
	iReadPoint = 19;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdbRampUpTime[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte21~22 : Ramp Down Time (readback)
	// RFPT Gen 사양서 챕터11 Input offset 21~22, uint16 LE
	// ------------------------------------------------------------------
	iReadPoint = 21;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdbRampDownTime[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte23 : Pulse Control rb 비트필드 
	// RFPT Gen 사양서 챕터11 Input offset 23
	// byte10(Pulse Control 출력)과 동일한 비트 정의
	// bit0=Pulse On/Off rb, 
	// bit1=Master/Slave rb
	// ------------------------------------------------------------------
	iReadPoint = 23;
	gnGenPulseOnOffRb[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenPulseMSRb[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);

	// ------------------------------------------------------------------
	// byte24~25 : Pulse Frequency (readback)
	// RFPT Gen 사양서 챕터11 Input offset 24~25, uint16 LE, 1Hz 
	// ------------------------------------------------------------------
	iReadPoint = 24;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblPulseFreqRb[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte26~27 : Pulse Duty (readback)
	// RFPT Gen 사양서 챕터11 Input offset 26~27, uint16 LE, 0.1% 
	// ------------------------------------------------------------------
	iReadPoint = 26;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblPulseDutyRb[nIndex] = uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte28 : LED Status 비트필드
	// RFPT Gen 사양서 챕터5 byte28 LED Status
	// ------------------------------------------------------------------
	iReadPoint = 28;
	gnGenACOn[nIndex]        = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0); // AC On
	gnGenInterlockSts[nIndex]= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1); // Interlock
	gnGenAlarmSts[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2); // Alarm
	gnGenOverTempSts[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3); // Over Temp
	gnGenPwrLimitSts[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4); // Power Limit
	gnGenRFOnSts[nIndex]     = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5); // RF On/Off
	gnGenRemoteIn[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6); // Remote In off/on
	

	// ------------------------------------------------------------------
	// byte29 : System State 비트필드
	// RFPT Gen 사양서 챕터5 byte29 System State 
	// ------------------------------------------------------------------
	iReadPoint = 29;
	gnGenSysPwrMode[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenSysRampMode[nIndex]   = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenSysPulseMode[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnGenSysCEXMode[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnGenSysCEXLock[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnGenSysFreqTuning[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	gnGenSysTuningOnOff[nIndex]= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	gnGenSysDCBias[nIndex]     = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);

	// ------------------------------------------------------------------
	// byte30 : Alarm 하위 바이트 (bit0~7)
	// RFPT Gen 사양서 챕터5 byte30~31 Alarm 16bit, LSB=byte30
	// ------------------------------------------------------------------
	iReadPoint = 30;
	gnGenAlm_AUX3V3[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnGenAlm_AUX5V[nIndex]   = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnGenAlm_AUX24V[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnGenAlm_ACL12[nIndex]   = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnGenAlm_ACL23[nIndex]   = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnGenAlm_ACL31[nIndex]   = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);
	gnGenAlm_PFC[nIndex]     = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6);
	gnGenAlm_MaxPwr[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);

	// ------------------------------------------------------------------
	// byte31 : Alarm 상위 바이트 (bit8~15)
	// RFPT Gen 사양서 챕터5 byte31, 상위 바이트 Alarm 16bit
	// ------------------------------------------------------------------
	iReadPoint = 31;
	gnGenAlm_GateDrv[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0); // bit8
	gnGenAlm_Fan[nIndex]      = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1); // bit9
	gnGenAlm_OverTemp[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2); // bit10
	gnGenAlm_TopIntlk[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3); // bit11
	gnGenAlm_BotIntlk[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4); // bit12
	gnGenAlm_RFIntlk[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5); // bit13
	gnGenAlm_UserIntlk[nIndex]= Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 6); // bit14
	gnGenAlm_UnderFWD[nIndex] = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7); // bit15

	return 1;
}


//-------------------------------------------------------------------------//
// Function    : READ_KYOSAN_MATCH                                         //
// Description : RFPT XPQ Matcher DeviceNet Input 데이터 읽기              //
//             : RFPT Mat 사양서 챕터5 Slave→Master Poll Response 22B 전체 파싱//
// Arguments   : int MacId (11 or 12)                                      //
// Returns     : 1(success), -1(failure)                                   //
//-------------------------------------------------------------------------//
int READ_KYOSAN_MATCH(int MacId)
{
	char szLog[512] = {0};
	int  iReadIndex;
	int  iReadPoint;
	int  nIndex;

	union DATA  uTempValu2;                   

	iReadIndex = MacId;
	if      (MacId == KYOSAN_SMAT_ID) nIndex = 0;
	else if (MacId == KYOSAN_BMAT_ID) nIndex = 1;

	if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1,
	                      InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
	{
		gnKYOSAN_MatCommSts[MacId - KYOSAN_SMAT_ID] = FALSE;
		return -1;
	}
	else
	{
		gnKYOSAN_MatCommSts[MacId - KYOSAN_SMAT_ID] = TRUE;
	}
	
	
	// ------------------------------------------------------------------
	// byte0~1 : Actual Load Position
	// RFPT Mat 사양서 챕터11 Input offset 0~1, uint16 LE ×10 (0.0~100.0%)
	// ------------------------------------------------------------------
	iReadPoint = 0;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblCLPos[nIndex] = (double)uTempValu2.iData;  // 

	// ------------------------------------------------------------------
	// byte2~3 : Actual Tune Position
	// RFPT Mat 사양서 챕터11 Input offset 2~3, uint16 LE
	// ------------------------------------------------------------------
	iReadPoint = 2;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblCTPos[nIndex] = (double)uTempValu2.iData;  // ÷10 적용

	// ------------------------------------------------------------------
	// byte4~5 : Actual Load Preset 
	// RFPT Mat 사양서 챕터11 Input offset 4~5, uint16 LE ×10
	// ------------------------------------------------------------------
	iReadPoint = 4;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblLoadPreset[nIndex] = (double)uTempValu2.iData;  // ÷10 적용

	// ------------------------------------------------------------------
	// byte6~7 : Actual Tune Preset 
	// RFPT Mat 사양서 챕터11 Input offset 6~7, uint16 LE ×10
	// ------------------------------------------------------------------
	iReadPoint = 6;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblTunePreset[nIndex] = (double)uTempValu2.iData;  // ÷10 적용

	// ------------------------------------------------------------------
	// byte8~9 : VDC In
	// RFPT Mat 사양서 챕터11 Input offset 8~9, int16 LE, 1V (부호있음)
	// ------------------------------------------------------------------
	iReadPoint = 8;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblVdc[nIndex] = uTempValu2.iData;                       // int16 부호있음, 단위 1V

	// ------------------------------------------------------------------
	// byte10 : Status 비트필드
	// RFPT Mat 사양서 챕터5 byte10 상태 비트필드
	// RFPT: bit0~3=MatcherMode, bit4=RemoteIn, bit5=RFState
	// ------------------------------------------------------------------
	iReadPoint = 10;
	gnblMATMatcherMode[nIndex] = InOutMap[iReadIndex].pReadData[iReadPoint] & 0x0F; // bit0~3 마스크
	gnblMATRemoteIn[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 4);
	gnblMATRFState[nIndex]     = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 5);

	// ------------------------------------------------------------------
	// byte11 : Alarm 비트필드
	// RFPT Mat 사양서 챕터5 byte11 알람 비트필드
	//   RFPT: bit7=Alarm(종합), bit3=leak, bit2=conn, bit1=cover, bit0=fan
	// ------------------------------------------------------------------
	iReadPoint = 11;
	gnblMATFan[nIndex]    = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 0);
	gnblMATCover[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 1);
	gnblMATConn[nIndex]   = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 2);
	gnblMATLeak[nIndex]   = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 3);
	gnblMATAlarm[nIndex]  = Get_Bit_Sts(InOutMap[iReadIndex].pReadData[iReadPoint], 7);

	// ------------------------------------------------------------------
	// byte12~13 : VSWR 
	// RFPT Mat 사양서 챕터11 Input offset 12~13, uint16 LE, 
	// 실제값 = 수신값 * 0.01 (예: 150 → 1.50) //.cfg 에서 스케일 변환 진행.
	// ------------------------------------------------------------------
	iReadPoint = 12;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblVSWR[nIndex] = (double)uTempValu2.iData;       

	// ------------------------------------------------------------------
	// byte14~15 : Input R 
	// RFPT Mat 사양서 챕터11 Input offset 14~15, int16 LE, 0.1Ω
	// ------------------------------------------------------------------
	iReadPoint = 14;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblInputR[nIndex] = (double)uTempValu2.iData ;

	// ------------------------------------------------------------------
	// byte16~17 : Input X (부호있음)
	// RFPT Mat 사양서 챕터11 Input offset 16~17, int16 LE, 0.1Ω
	// ------------------------------------------------------------------
	iReadPoint = 16;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblInputX[nIndex] = (double)uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte18~19 : Vrms (부호있음)
	// RFPT Mat 사양서 챕터11 Input offset 18~19, int16 LE, 0.1V
	// ------------------------------------------------------------------
	iReadPoint = 18;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblVrms[nIndex] = (double)uTempValu2.iData;

	// ------------------------------------------------------------------
	// byte20~21 : Irms (부호있음)
	// RFPT Mat 사양서 챕터11 Input offset 20~21, int16 LE, 0.1A
	// ------------------------------------------------------------------
	iReadPoint = 20;
	uTempValu2.ucData[0] = InOutMap[iReadIndex].pReadData[iReadPoint];
	uTempValu2.ucData[1] = InOutMap[iReadIndex].pReadData[iReadPoint + 1];
	gdblIrms[nIndex] = (double)uTempValu2.iData;

	return 1;
}

int GetAPCSlaveSts(int value)
{
	int retVal;
	
	if		(value == 0)	retVal = 1;	//Sync
	else if (value == 1)	retVal = 2;	//Pos
	else if (value == 2)	retVal = 3;	//Cl
	else if (value == 4)	retVal = 4;	//Of
	else if (value == 8)	retVal = 5;	//Cf

	else if (value == 16)	retVal = 6;	//Serv
	else if (value == 17)	retVal = 6;	//Pos.Serv
	else if (value == 18)	retVal = 6;	//Cl.Serv
	else if (value == 20)	retVal = 6;	//Of.Serv
	else if (value == 24)	retVal = 6;//Cf.Serv

	else if (value == 32)	retVal = 7;//Pf
	else if (value == 33)	retVal = 7;//Pos.Pf
	else if (value == 34)	retVal = 7;//Cl.Pf
	else if (value == 36)	retVal = 7;//Of.Pf
	else if (value == 40)	retVal = 7;//Cf.Pf
	else if (value == 48)   retVal = 7;//Serv.Pf	//추가
	else if (value == 49)	retVal = 7;//Pos.Serv.Pf
	else if (value == 50)	retVal = 7;//Cl.Serv.Pf
	else if (value == 52)	retVal = 7;//Of.Serv.Pf
	else if (value == 56)	retVal = 7;//Cf.Serv.Pf

	else if (value == 64)	retVal = 8;//Af
	else if (value == 65)	retVal = 8;//Pos.Af
	else if (value == 66)	retVal = 8;//Cl.Af
	else if (value == 68)	retVal = 8;//Of.Af
	else if (value == 72)	retVal = 8;//Cf.Af
	else if (value == 80)	retVal = 8;//Serv.Af
	else if (value == 81)	retVal = 8;//Pos.Serv.Af
	else if (value == 82)	retVal = 8;//Cl.Serv.Af
	else if (value == 84)	retVal = 8;//Of.Serv.Af
	else if (value == 88)	retVal = 8;//Cf.Serv.Af
	else if (value == 96)	retVal = 8;//Pf.Af
	else if (value == 97)	retVal = 8;//Pos.Pf.Af
	else if (value == 98)	retVal = 8;//Cl.Pf.Af
	else if (value == 100)	retVal = 8;//Of.Pf.Af
	else if (value == 104)	retVal = 8;//Cf.Pf.Af
	else if (value == 112)	retVal = 8;//Serv.Pf.Af
	else if (value == 113)	retVal = 8;//Pos.Serv.Pf.Af
	else if (value == 114)	retVal = 8;//Cl.Serv.Pf.Af
	else if (value == 116)	retVal = 8;//Of.Serv.Pf.Af
	else if (value == 120)	retVal = 8;//Cf.Serv.Pf.Af

	else if (value == 128)	retVal = 9;//Offline
	else					retVal = 0;//Unknown
	
	return retVal;
}


int READ_FRC(int MacId)
{
	int iReadIndex;
	union uValue sData; 
 	
	int nTempValue = 0;
	int idx = 0;
	
	iReadIndex = MacId;
	
	if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1, InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
	{
		gnFRCComSts = FALSE; 
		return -1;           
	}
	else
	{
		gnFRCComSts = TRUE;
	}
     
    // Assembly Instance 6 USE
	// FRC Master Controller Status Check 
	// 데이터 맵의 가장 첫 번째 바이트(0번 위치)는 Status 정보
    gnFRC1_Status = InOutMap[iReadIndex].pReadData[0];


	// Total Flow (SCCM)
	// 데이터 맵의 1번 위치부터 4바이트를 읽어, 총 유량 값을 저장
	sData.ucValue[0] = InOutMap[iReadIndex].pReadData[1];
	sData.ucValue[1] = InOutMap[iReadIndex].pReadData[1+1];
	sData.ucValue[2] = InOutMap[iReadIndex].pReadData[1+2];
	sData.ucValue[3] = InOutMap[iReadIndex].pReadData[1+3];
	gdFRC_Total_Flow_SCCM = sData.fValue;

	gdFRC_Total_Flow_Ratio = gdAI_FRC_FLOW_Ratio[0] + 
							 gdAI_FRC_FLOW_Ratio[1] + 
							 gdAI_FRC_FLOW_Ratio[2] ;

		for(int i=0; i<36; i++)
		{
			sData.ucValue[0] = InOutMap[iReadIndex].pReadData[9+(4*i)];  // 9번 위치
			sData.ucValue[1] = InOutMap[iReadIndex].pReadData[9+(4*i)+1]; 
			sData.ucValue[2] = InOutMap[iReadIndex].pReadData[9+(4*i)+2]; 
			sData.ucValue[3] = InOutMap[iReadIndex].pReadData[9+(4*i)+3]; 

			switch(i)
			{
				// --- 9개 채널의 Ratio (%) 데이터 읽기 ---
				// Ratio 데이터는 Offset 9 시작, 채널은 4바이트 float 

				case 0 :
				case 1 :
				case 2 :
				case 3 :
				case 4 :
				case 5 :
				case 6 :
				case 7 :
				case 8 :
					gdAI_FRC_FLOW_Ratio[i] = sData.fValue;    		break;  
				case 9: 
				case 10: 
				case 11: 
				case 12: 
				case 13: 
				case 14: 
				case 15: 
				case 16: 
				case 17: 
					gdAI_FRC_FLOW_SCCM[i%9] = sData.fValue; 			break;

				case 18: 
				case 19: 
				case 20: 
				case 21: 
				case 22: 
				case 23: 
				case 24: 
				case 25: 
				case 26: 
					gdAI_FRC_FLOW_Ratio_SetChk[i%(9*2)] = sData.fValue; 	break;

				case 27: 
				case 28: 
				case 29: 
				case 30: 
				case 31: 
				case 32: 
				case 33: 
				case 34: 
				case 35: 
					gdAI_FRC_FLOW_SCCM_SetChk[i%(9*3)] = sData.fValue; 	break;

				case 36:
					gdFRC_Total_Flow_SCCM = sData.fValue; 	break;					

			}
		}
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

int READ_APCVLV(int MacId)
{
	int iReadIndex;
	
	//union uValue uTempValue;
	union DATA uTempValue2;
	
	char szLog[512] = {0};
	
	int nTempValue = 0;
	int idx = 0;
	//int	Byte[4];

	iReadIndex = MacId;

	try{
		if( DNS_ReadDeviceIo( CardHandle, DeviceConfig[iReadIndex].MacId, DNS_INPUT1, InOutMap[iReadIndex].pReadData, DeviceConfig[iReadIndex].Input1Size ) == FALSE )
		{
			// Exception
			gnAPCComSts = FALSE;
			
			gnAPCAcessMode = 0; // access - unknown mode
			gnAPCDeviceSts = 0;	// devicestatus - unknown
			gnAPCException = 0; // offline
			gdblAPCPosition = (float)9999.99;

			//sprintf(szLog, "[READ_APCVLV] DNS_ReadDeviceIo return FALSE... Err : %d, MacID[%d]", GetLastError(), iReadIndex);
			//LogFileSave(szLog);
			
			for(idx = 0; idx < 8; idx++)
			{
				gnAPCValveSts[idx] = 999;	// slave status - unknown
			}
			
			//printf("*********************APC Read NG!!!!!!!!!!!!!!!!!!!!!!\n");
			return - 1;
		}
		else
		{
			gnAPCComSts = TRUE;
		}
	}
	catch(...) {
		//sprintf(szLog, "[READ_APCVLV] DNS_ReadDeviceIo Exception... Err : %d, MacID[%d]", GetLastError(), iReadIndex);
		//LogFileSave(szLog);
	}


	try{
		//printf("*********************APC Read OK\n");
		
		//................................
		// Exception
		nTempValue = InOutMap[iReadIndex].pReadData[0];		
		if		(nTempValue == 128)	gnAPCException = 1;	// offline
		else if	(nTempValue == 132)	gnAPCException = 2;	//
		else if	(nTempValue == 192)	gnAPCException = 3;
		else if	(nTempValue == 196)	gnAPCException = 4;
		else						gnAPCException = 0;
		//printf("*********************APC APC Read OK 111111  ( %d )\n",nTempValue);
		//................................
		// Access Mode
		nTempValue = InOutMap[iReadIndex].pReadData[7];	
		if(nTempValue >= 0 && nTempValue <= 2)
		{
			gnAPCAcessMode = nTempValue + 1;
		}
		//	printf("*********************APC APC Read OK 2222222  ( %d )\n",nTempValue);
		//................................
		// Device Status2
		nTempValue = InOutMap[iReadIndex].pReadData[8];	
		if		(nTempValue > 0  && nTempValue < 8)		gnAPCDeviceSts = nTempValue;
		else if	(nTempValue > 11 && nTempValue < 15)	gnAPCDeviceSts = nTempValue - 4;
		else											gnAPCDeviceSts = 0;
		
		//	printf("*********************APC APC Read OK 333333  ( %d )\n",nTempValue);
		//................................
		// Individual valve status
		nTempValue = InOutMap[iReadIndex].pReadData[9];
		gnAPCValveSts[0]	= GetAPCSlaveSts(nTempValue);
		
		nTempValue = InOutMap[iReadIndex].pReadData[10];
		gnAPCValveSts[1]	= GetAPCSlaveSts(nTempValue);
		
		nTempValue = InOutMap[iReadIndex].pReadData[11];
		gnAPCValveSts[2]	= GetAPCSlaveSts(nTempValue);
		
		nTempValue = InOutMap[iReadIndex].pReadData[12];
		gnAPCValveSts[3]	= GetAPCSlaveSts(nTempValue);
		
		nTempValue = InOutMap[iReadIndex].pReadData[13];
		gnAPCValveSts[4]	= GetAPCSlaveSts(nTempValue);
		
		nTempValue = InOutMap[iReadIndex].pReadData[14];
		gnAPCValveSts[5]	= GetAPCSlaveSts(nTempValue);
		
		nTempValue = InOutMap[iReadIndex].pReadData[15];
		gnAPCValveSts[6]	= GetAPCSlaveSts(nTempValue);
		
		nTempValue = InOutMap[iReadIndex].pReadData[16];
		gnAPCValveSts[7]	= GetAPCSlaveSts(nTempValue);
		
		
		
		//................................
		// APC Sensor 1 - 1.0 Torr
		uTempValue2.ucData[0]	= InOutMap[iReadIndex].pReadData[1 + 0];
		uTempValue2.ucData[1]	= InOutMap[iReadIndex].pReadData[1 + 1];
		gdblAPCSensor1  = uTempValue2.iData;
		
		
		//printf("*********************APC APC Read OK Sensor 111111  ( %f )\n",gdblAPCSensor1);
		
		
		//................................
		// APC Sensor 2 - 0.1 Torr
		uTempValue2.ucData[0]	= InOutMap[iReadIndex].pReadData[3 + 0];
		uTempValue2.ucData[1]	= InOutMap[iReadIndex].pReadData[3 + 1];
		gdblAPCSensor2  = uTempValue2.iData;
		
		//printf("*********************APC APC Read OK Sensor 222222  ( %f )\n",gdblAPCSensor2);
		
		//................................
		// APC Position
		uTempValue2.ucData[0]	= InOutMap[iReadIndex].pReadData[5 + 0];
		uTempValue2.ucData[1]	= InOutMap[iReadIndex].pReadData[5 + 1];
		gdblAPCPosition	= uTempValue2.iData;
		
		if(gdblAPCPosition == 32767)	gdblAPCPosition = 9999.9;
		else							gdblAPCPosition = gdblAPCPosition * 0.1;
	}
	catch(...) {
		//sprintf(szLog, "[READ_APCVLV] InOutMap Exception... Err : %d, MacID[%d]", GetLastError(), iReadIndex);
		//LogFileSave(szLog);
	}

	
	

//	printf("*********************APC APC Read OK6666666666666666\n");

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