//------------------------------------------------------
//
// FILENAME : dns32exp.h Rev. 1.05
//
// (C) 1998-1999 SST/Woodhead Canada Limited
//
// This sample code is provide as-is. It may be copied freely
// in whole or in part, providing that it is used in conjunction
// with an SST/Woodhead Canada Limited interface card.
//------------------------------------------------------
// 1.03     02/24/99		IO connection flags for SS_ACK, SS_COS, and SS_CYC fixed
// 1.05     Oct. 5, 1999	Added ProductionInhibitTime to DEVICE_CFG structure

//--------------------------
// Module Id constants
//--------------------------
#define DNSCAN_ID 0x14

//--------------------------
// share flags
//--------------------------
#define SS_WRITE	0x01
#define SS_XWRITE   0x02
#define SS_HCONTROL 0x04
#define SS_CARDIRQ  0x08
#define SS_APPIRQ   0x10
#define SS_OVERLAP  0x4000

//--------------------------
// load flags ( 5-7 reserved, 8-11 user access, 12-13 reserved )
//--------------------------
#define SS_REPLACE	0x01
#define SS_RELOAD	0x02
#define SS_EMBEDDED	0x04

//--------------------------
// signal id flags
//--------------------------
#define SS_LED	    0x00
#define SS_WINSIZE	0x01
#define SS_BANK 	0x02
#define SS_ACCESSCTL 0x03

//--------------------------
// signal id values
//--------------------------
#define LED_RED	    0x01
#define LED_GREEN	0x02

//--------------------------
// status flags
//--------------------------
#define DNS_EXP_MSG_RECEIVED   0x01
#define DNS_EXP_MSG_TRUNCATED  0x02
#define DNS_INPUT_DATA_UPDATE  0x01
#define DNS_RECEIVE_IDLE 0x02
#define DNS_SERVER_EXPLICIT_RECEIVED 0x01

//--------------------------
// DeviceNet
//--------------------------
#define MIN_MAC_ID 0
#define MAX_MAC_ID 63
#define NUM_MAC_ID 64
#define DN_125K 0
#define DN_250K 1
#define DN_500K 2

//--------------------------
// I/O data areas
//--------------------------
#define DNS_INPUT1  0
#define DNS_OUTPUT1 1
#define DNS_INPUT2  2
#define DNS_OUTPUT2 3

#define DNS_SCAN_EVENT 1
#define DNS_STATUS_EVENT 0
#define DNS_IO1_EVENT 1
#define DNS_IO2_EVENT 2
#define DNS_EXP_EVENT 3
#define DNS_EXP_REQ_EVENT 3
#define DNS_EXP_RES_EVENT 4

//--------------------------
// connection flags
//--------------------------
#define SS_EX  0x01
#define SS_P   0x02
#define SS_ST  0x04
#define SS_COS 0x10
#define SS_CYC 0x20
#define SS_AKS 0x40

//------------------------------------------------------
// Type definitions
//------------------------------------------------------

//--------------------------
// Module header
//--------------------------
typedef struct
{
	WORD ModuleType;
	WORD WinSize;
	WORD CardId;
	WORD KernelId;
	WORD KernelRev;
	WORD ModuleId;
	WORD ModuleRev;
	DWORD NetSerial;
	BYTE CardType[16];
	BYTE CardSerial[8];
	WORD IrqControl;
	WORD IrqStatus;
	WORD MainCode;
	WORD CanStatus;
	WORD CanTx;
	WORD CanAck;
	WORD CanRx;
	WORD CanErr;
	WORD CanLost;
	WORD CanOverrun;
	WORD AddCode;
	BYTE ModuleString[64];
} DNS_MODULE_HEADER;

//--------------------------
// Scaner config data
//--------------------------
typedef struct
{
	WORD MacId;
	WORD BaudRate;
	WORD ScanInterval;
	WORD ReconnectTime;
	WORD Flags;
	WORD ExplicitRequestSize;
	WORD ExplicitRequestOffset;
	WORD ExplicitResponseSize;
	WORD ExplicitResponseOffset;
	WORD Io1Interval;
	WORD Output1Size;
	WORD Output1Offset;
	WORD Output1PathOffset;
	WORD Input1Size;
	WORD Input1Offset;
	WORD Input1PathOffset;
	WORD Io2Interval;
	WORD Output2Size;
	WORD Output2Offset;
	WORD Output2PathOffset;
	WORD Input2Size;
	WORD Input2Offset;
	WORD Input2PathOffset;
} DNS_SCANNER_CFG;

//--------------------------
// Device config data
//--------------------------
typedef struct
{
	WORD MacId;
	WORD VendorId;
	WORD DeviceType;
	WORD ProductCode;
	WORD ProductionInhibitTime;
	WORD Reserved2;
	WORD Reserved3;
	WORD Flags;
	WORD ExplicitSize;
	WORD ExplicitOffset;
	WORD Io1Interval;
	WORD Output1Size;
	WORD Output1Offset;
	WORD Output1LocalPathOffset;
	WORD Output1RemotePathOffset;
	WORD Input1Size;
	WORD Input1Offset;
	WORD Input1LocalPathOffset;
	WORD Input1RemotePathOffset;
	WORD Io2Interval;
	WORD Output2Size;
	WORD Output2Offset;
	WORD Output2LocalPathOffset;
	WORD Output2RemotePathOffset;
	WORD Input2Size;
	WORD Input2Offset;
	WORD Input2LocalPathOffset;
	WORD Input2RemotePathOffset;
} DNS_DEVICE_CFG;


//--------------------------
// Device id
//--------------------------

typedef struct
{
	WORD MacId;
} DNS_DEVICE_ID;


//--------------
// CAN Messages
//--------------
typedef struct
{
    WORD CANID;
    BYTE Length;
    BYTE Data[8];
}DNS_CAN_MESSAGE;

//--------------------------
// Command error
//--------------------------
typedef struct
{
	WORD ErrCode;
	WORD ErrCodeExt;
} DNS_CMD_ERROR;

//--------------------------
// CAN Counters
//--------------------------
typedef struct
{
	WORD CanTx;
	WORD CanAck;
	WORD CanRx;
	WORD CanErr;
	WORD CanLost;
	WORD CanOverrun;
} DNS_CAN_COUNTERS;  

//--------------------------
// status structure
//--------------------------
typedef struct
{
	BYTE StatusCode;
	BYTE StatusFlags;
} DNS_STATUS;

//--------------------------
// structure
//--------------------------
typedef struct
{
	PBYTE pReadData;
	PBYTE pWriteData;
} IO_Map;

typedef struct
{
	int Analog_Max;
	int Digital_Max;
} IO_Info;

//--------------------------
// dnscan32.dll error codes
//--------------------------
// general error codes
#define DRIVER_LOADED           0x20000000
#define DRIVER_NOT_FOUND        0x20000001
#define INVALID_DRIVER          0x20000002
#define DRIVER_NOT_LOADED       0x20000003
#define COMMAND_TIMEOUT         0x20000004
// dnscan32.dll specific error codes
#define GENERAL_FAILURE         0x20000200
#define DNS_ERR_CMD             0x20000201
#define DNS_ERR_MAC             0x20000202
#define DNS_ERR_BAUD            0x20000203
#define DNS_ERR_DUPMAC          0x20000204
#define DNS_ERR_DUPDEV          0x20000205
#define DNS_ERR_NODEV           0x20000206
#define DNS_ERR_OFF             0x20000207
#define DNS_ERR_ACTIVE          0x20000208
#define DNS_ERR_NOTOFF          0x20000209
#define DNS_ERR_SCAN            0x2000020a
#define DNS_ERR_NOTSCAN         0x2000020b
#define DNS_ERR_SCANOFF         0x2000020c
#define DNS_ERR_IO_OFFSET       0x2000020d
#define DNS_ERR_BUS_FAULT       0x2000020e
#define DNS_ERR_UNKNOWN         0x2000020f

//--------------------------
// DNS_CMD_ID commands
//--------------------------
#define ONLINE 1
#define OFFLINE 2
#define ADD_DEVICE 3
#define GET_DEVICE 4
#define DELETE_DEVICE 5
#define START_SCAN 6
#define STOP_SCAN 7
#define IO_ACTIVE 8
#define IO_IDLE 9

//--------------------------
// Command ErrCode codes
//--------------------------
#define ERR_NULL 0
#define ERR_CMD 1
#define ERR_MAC 2
#define ERR_BAUD 3
#define ERR_DUPMAC 4
#define ERR_DUPDEV 5
#define ERR_NODEV 6
#define ERR_OFF 7
#define ERR_ACTIVE 8
#define ERR_NOTOFF 9
#define ERR_SCAN 10
#define ERR_NOTSCAN 11
#define ERR_SCANOFF 12
#define ERR_OFFSET 13
#define ERR_BUS_FAULT 14
#define ERR_CONNECTION_FLAGS 15
#define ERR_EXPLICIT_BUFFER_SIZE 0x10
#define ERR_STROBE_BUFFER_SIZE 0x11
#define ERR_PATH_BUFFER 0x12

//--------------------------
// command timeouts (in ms)
//--------------------------
#define INIT_TIMEOUT 5000
#define CMD_TIMEOUT 2000

//--------------------------
// Device Status 
//--------------------------

// device status codes
#define DNS_NON_EXIST 0x00
#define DNS_IDLE 0x01
#define DNS_ACTIVE 0x02
#define DNS_TIMEOUT 0x03
#define DNS_FAULT_UCMM 0x04
#define DNS_MS_BUSY 0x05
#define DNS_FAULT_MS 0x06
#define DNS_BAD_VENDOR 0x07
#define DNS_FAULT_VENDOR 0x08
#define DNS_BAD_TYPE 0x09
#define DNS_FAULT_TYPE 0x0a
#define DNS_BAD_CODE 0x0b
#define DNS_FAULT_CODE 0x0c
#define DNS_BAD_IO1_INPUT_SIZE 0x0d
#define DNS_FAULT_IO1_INPUT_SIZE 0x0e
#define DNS_BAD_IO1_OUTPUT_SIZE 0x0f
#define DNS_FAULT_IO1_OUTPUT_SIZE 0x10
#define DNS_BAD_IO2_INPUT_SIZE 0x11
#define DNS_FAULT_IO2_INPUT_SIZE 0x12
#define DNS_BAD_IO2_OUTPUT_SIZE 0x13
#define DNS_FAULT_IO2_OUTPUT_SIZE 0x14
#define DNS_FAULT_SETTING_IO1_RATE 0x15
#define DNS_FAULT_SETTING_IO2_RATE 0x16
#define DNS_MS_SYNC 0x17

//-------------------------------------------------------
// Function Pointer typedefs
//-------------------------------------------------------
typedef BOOL (WINAPI *dnsAddDevice) (DWORD CardHandle, DNS_DEVICE_CFG *DeviceCfg );
typedef BOOL (WINAPI *dnsCANGetRxQStatus) ( DWORD CardHandle, BYTE *QueueStatus );
typedef BOOL (WINAPI *dnsCANSetRXQ)   ( DWORD CardHandle, WORD QueueOffset, WORD QueueSize );
typedef BOOL (WINAPI *dnsCANReceive)   ( DWORD CardHandle, DNS_CAN_MESSAGE *CANMsg );
typedef BOOL (WINAPI *dnsCANSetFilter) ( DWORD CardHandle, WORD CANID, BOOL Enable );
typedef BOOL (WINAPI *dnsCANTransmit)  ( DWORD CardHandle, DNS_CAN_MESSAGE *CANMsg );
typedef BOOL (WINAPI *dnsCloseCard) ( DWORD  CardHandle );
typedef BOOL (WINAPI *dnsDeleteDevice) (DWORD CardHandle, WORD DeviceId );
typedef BOOL (WINAPI *dnsDriver) ( TCHAR *Buffer, WORD *Version, DWORD Size );
typedef BOOL (WINAPI *dnsFreeDriver) ( void );
typedef BOOL (WINAPI *dnsGetBusStatus) ( DWORD CardHandle, WORD *BusStatus );
typedef BOOL (WINAPI *dnsGetClientEvent) ( DWORD CardHandle, BYTE EventId, BYTE *ClientStatus );
typedef BOOL (WINAPI *dnsGetClientStatus) ( DWORD CardHandle, DNS_STATUS *ClientStatus );
typedef BOOL (WINAPI *dnsGetDevice) ( DWORD CardHandle, WORD DeviceId, DNS_DEVICE_CFG *DeviceCfg );
typedef BOOL (WINAPI *dnsGetDeviceEvent) ( DWORD CardHandle, WORD DeviceId, BYTE EventId, BYTE *DeviceEvent );
typedef BOOL (WINAPI *dnsGetDevicePath) ( DWORD CardHandle, WORD DeviceId, BYTE PathId, WORD *MaxPathLength, WORD *PathLength, BYTE *Path );
typedef BOOL (WINAPI *dnsGetDeviceStatus) ( DWORD CardHandle, WORD DeviceId, DNS_STATUS *DeviceStatus );
typedef BOOL (WINAPI *dnsGetModuleHeader) ( DWORD CardHandle, DNS_MODULE_HEADER *ModuleHeader );
typedef BOOL (WINAPI *dnsGetServerEvent) ( DWORD CardHandle, BYTE EventId, BYTE *ServerEvent );
typedef BOOL (WINAPI *dnsGetServerPath)( DWORD CardHandle, BYTE PathId, WORD *MaxPathLength, WORD *PathLength, BYTE *Path );
typedef BOOL (WINAPI *dnsGetServerStatus) ( DWORD CardHandle, DNS_STATUS *ServerStatus );
typedef BOOL (WINAPI *dnsInitializePathBuffer) ( DWORD CardHandle, WORD Offset, WORD MaxPathSize, WORD PathLength, BYTE *Path );
typedef BOOL (WINAPI *dnsIoActive) (DWORD CardHandle, WORD Timeout);
typedef BOOL (WINAPI *dnsIoIdle) (DWORD CardHandle);	
typedef BOOL (WINAPI *dnsLoadDriver) ( TCHAR *DriverName );
typedef BOOL (WINAPI *dnsLog) ( DWORD LogFlags, DWORD LogFileHandle );
typedef BOOL (WINAPI *dnsOffline) ( DWORD CardHandle );        
typedef BOOL (WINAPI *dnsOnline) ( DWORD CardHandle, DNS_SCANNER_CFG *ScannerCfg );
typedef BOOL (WINAPI *dnsOpenCard) ( DWORD *CardHandle, TCHAR *CardName, void *Module, DWORD Flags );
typedef BOOL (WINAPI *dnsReadDeviceIo) ( DWORD CardHandle, WORD DeviceId, BYTE IoArea, void *Buffer, WORD Size );
typedef BOOL (WINAPI *dnsReadServerIo) ( DWORD CardHandle, BYTE IoArea, void *Buffer, WORD Size );
typedef BOOL (WINAPI *dnsReceiveDeviceExplicit) ( DWORD CardHandle, WORD DeviceId, BYTE *Service, void *ServiceData, WORD *Size );
typedef BOOL (WINAPI *dnsReceiveServerExplicit) ( DWORD CardHandle, WORD *ConnectionId, BYTE *Service, WORD *ClassId, WORD *InstanceId, void *ServiceData, WORD *Size );
typedef BOOL (WINAPI *dnsRegisterCANRxEvent) ( DWORD CardHandle, DWORD ThreadId, DWORD MsgId, LPARAM lParam );
typedef BOOL (WINAPI *dnsRegisterBusStatusEvent) ( DWORD CardHandle, DWORD ThreadId, DWORD MsgId, LPARAM lParam );
typedef BOOL (WINAPI *dnsRegisterClientEvent) ( DWORD CardHandle, BYTE EventId, DWORD ThreadId, DWORD MsgId, LPARAM lParam );
typedef BOOL (WINAPI *dnsRegisterDeviceEvent) ( DWORD CardHandle, WORD DeviceId, BYTE EventId, DWORD ThreadId, DWORD MsgId, LPARAM lParam );
typedef BOOL (WINAPI *dnsRegisterServerEvent) ( DWORD CardHandle, BYTE EventId, DWORD ThreadId, DWORD MsgId, LPARAM lParam );
typedef BOOL (WINAPI *dnsSendDeviceExplicit) ( DWORD CardHandle, WORD DeviceId, BYTE Service, WORD ClassId, WORD InstanceId, void *ServiceData, WORD Size );
typedef BOOL (WINAPI *dnsSendServerExplicit) ( DWORD CardHandle, WORD ConnectionId, BYTE Service, void *ServiceData, WORD Size );
typedef BOOL (WINAPI *dnsSetAccessTimeout) ( DWORD CardHandle, DWORD Timeout );
typedef BOOL (WINAPI *dnsSetEventNotificationInterval) ( DWORD CardHandle, DWORD Interval );
typedef BOOL (WINAPI *dnsStartScan) (DWORD CardHandle);
typedef BOOL (WINAPI *dnsStopScan) (DWORD CardHandle);
typedef BOOL (WINAPI *dnsUnRegisterBusStatusEvent) ( DWORD CardHandle );
typedef BOOL (WINAPI *dnsUnRegisterCANRxEvent) ( DWORD CardHandle );
typedef BOOL (WINAPI *dnsUnRegisterClientEvent) ( DWORD CardHandle, BYTE EventId );
typedef BOOL (WINAPI *dnsUnRegisterDeviceEvent) ( DWORD CardHandle, WORD DeviceId, BYTE EventId );
typedef BOOL (WINAPI *dnsUnRegisterServerEvent) ( DWORD CardHandle, BYTE EventId );
typedef BOOL (WINAPI *dnsVersion) ( TCHAR *Buffer, WORD *Version, DWORD Size );
typedef BOOL (WINAPI *dnsWriteDeviceIo) ( DWORD CardHandle, WORD DeviceId, BYTE IoArea, void *Data, WORD Size );
typedef BOOL (WINAPI *dnsWriteServerIo) ( DWORD CardHandle, BYTE IoArea, void *Data, WORD Size );
typedef BOOL (WINAPI *dnsEnumDrivers) ( DWORD Index, TCHAR *lpName, DWORD *Size );


typedef BOOL (WINAPI *dneCommandError) ( DWORD ErrorCode, TCHAR *Buffer, DWORD Size );

void CleanUp( DWORD CardHandle, HMODULE LibInst, HMODULE ErrInst );


BOOL ClearConsole(HANDLE hConsole);
BOOL SetConsolexy(HANDLE hConsole, int x ,int y );
BOOL CursorOff(HANDLE hConsole);
BOOL CursorOn(HANDLE hConsole);

// dnscan32.dll function pointers
dnsAddDevice DNS_AddDevice;
dnsCANGetRxQStatus DNS_CANGetRxQStatus;
dnsCANSetRXQ DNS_CANSetRXQ;
dnsCANReceive DNS_CANReceive;
dnsCANSetFilter	DNS_CANSetFilter;
dnsCANTransmit DNS_CANTransmit;
dnsDeleteDevice DNS_DeleteDevice;
dnsCloseCard DNS_CloseCard;
dnsDriver DNS_Driver;
dnsFreeDriver DNS_FreeDriver;
dnsGetBusStatus DNS_GetBusStatus;
dnsGetClientEvent DNS_GetClientEvent;
dnsGetClientStatus DNS_GetClientStatus;
dnsGetDevice DNS_GetDevice;
dnsGetDeviceEvent DNS_GetDeviceEvent;
dnsGetDevicePath DNS_GetDevicePath;
dnsGetDeviceStatus DNS_GetDeviceStatus;
dnsGetModuleHeader DNS_GetModuleHeader;
dnsGetServerEvent DNS_GetServerEvent;
dnsGetServerPath DNS_GetServerPath;
dnsGetServerStatus DNS_GetServerStatus;
dnsInitializePathBuffer DNS_InitializePathBuffer;
dnsIoActive DNS_IoActive;
dnsIoIdle DNS_IoIdle;
dnsLoadDriver DNS_LoadDriver;
dnsLog DNS_Log;
dnsOffline DNS_Offline;
dnsOnline DNS_Online;
dnsOpenCard DNS_OpenCard;
dnsReadDeviceIo DNS_ReadDeviceIo;
dnsReadServerIo DNS_ReadServerIo;
dnsReceiveDeviceExplicit DNS_ReceiveDeviceExplicit;
dnsReceiveServerExplicit DNS_ReceiveServerExplicit;
dnsRegisterBusStatusEvent DNS_RegisterBusStatusEvent;
dnsRegisterCANRxEvent	DNS_RegisterCANRxEvent;
dnsRegisterClientEvent DNS_RegisterClientEvent;
dnsRegisterDeviceEvent DNS_RegisterDeviceEvent;
dnsRegisterServerEvent DNS_RegisterServerEvent;
dnsSendDeviceExplicit DNS_SendDeviceExplicit;
dnsSendServerExplicit DNS_SendServerExplicit;
dnsSetAccessTimeout DNS_SetAccessTimeout;
dnsSetEventNotificationInterval DNS_SetEventNotificationInterval;
dnsStartScan DNS_StartScan;
dnsStopScan DNS_StopScan;
dnsUnRegisterBusStatusEvent DNS_UnRegisterBusStatusEvent;
dnsUnRegisterCANRxEvent DNS_UnRegisterCANRxEvent;
dnsUnRegisterClientEvent DNS_UnRegisterClientEvent;
dnsUnRegisterDeviceEvent DNS_UnRegisterDeviceEvent;
dnsUnRegisterServerEvent DNS_UnRegisterServerEvent;
dnsVersion DNS_Version;
dnsWriteDeviceIo DNS_WriteDeviceIo;
dnsWriteServerIo DNS_WriteServerIo;
dnsEnumDrivers DNS_EnumDrivers;

// dnerr32.dll function pointers
dneCommandError DNE_CommandError;