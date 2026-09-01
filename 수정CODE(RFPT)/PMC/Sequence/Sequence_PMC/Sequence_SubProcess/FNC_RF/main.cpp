#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <process.h>
#include <time.h>
#include <tMain.h>
#include <TUtilStr.h>
#include <TTimer.h>
#include <TString.h>

#include "h/Define.h"

//----------------------------------------------------------------------------------------------------------------------------------------------

enum {OFF   ,		ON };
enum {eOffline,		eOnline};
enum {eNO,			eYES};														// Use main etch
enum {eRF_NONE,		eRF_INIT,    eRF_PRESET,  eRF_OFF, eRF_ON, eRF_RAMP, eRF_DUAL, eRF_AUTOPRESET};
enum {eDisable,		eEnable};
enum {eOFF_CMD,		eON_CMD};
enum {eOFF_STATUS,	eON_STATUS};
enum {eNotRemote,  eRemote };
enum {NoPreset, Table01, Table02, Table03, Table04, Table05, Table06, Table07, Table08, Table09, Table10};
enum {eFNC_IDLE, eFNC_RUNNING, eFNC_ABORTED};
enum {RESET_OFF, RESET_ON};
enum {RF_NotReady, RF_Ready};
enum {RAMP_DISABLE, RAMP_ENABLE};
enum {FNC_IDLE, FNC_RUNNING, FNC_ABORTED};
enum {eFlow_Error, eFlow_Ok};
enum {eRF_FaultSts_Idle, eRF_FaultSts_Alarm};
enum {eProcGasVlvSns_Open, eProcGasVlvSns_Close};
enum {NotReady, Ready };
enum {eInterface, eDeviceNet};

// KLP_EDIT
// RFPT Control
enum {eMATCH_MANUAL_0, eMATCH_AUTO_1, eMATCH_PRESET_2};
enum {UserIO_PLC_0,	OnlyRS232_1,	OnlyEthernet_2,	OnlyDnet_3,	RS232_PLC_4,	Ethernet_PLC_5,	Dnet_PLC_6};



//----------------------------------------------------------------------------------------------------------------------------------------------

BOOL SIMULATION_MODE = FALSE;
#define SRF									0
#define BRF									1

int		gnPR_RF_Ctrl;
int     gnAlarmOffset = 0;

double	gdPR_RFG_Power;
double	gdPR_RFM_Match;
double	gdPR_RFM_Tune;

char	OBJECT_NAME[32] = "";			
char    OBJECT_HEADER[8] = "";
char	szMessage[256] = "";			
char	RFType[256] = "";		
char	AutoPresetMessage[1024] = "";		


//----------------------------------------------------------------------------------------------------------------------------------------------

BEGIN_OBJECT_ENUMERATION

	CDIO		SIMULATION_CONTROL			("System.SimulMode"				);
                                        
	CDIO        PR_RF_CTRL				    (_TEXT("PR_$0_CTRL"				));
	CAIO        PR_RFG_POWER		        (_TEXT("PR_$0G_POWER"			));
	CAIO        PR_RFM_MATCH		        (_TEXT("PR_$0M_MATCH"			));
	CAIO        PR_RFM_TUNE			        (_TEXT("PR_$0M_TUNE"			));

	CDIO        PM_RF_Cmd			        (_TEXT("PM_$0_Cmd"				));
    
	//KLP_EDIT
	// 아래 IO 사용안함.
	//CDIO        DO_RFG_ALRAM_RESET		    (_TEXT("DO_$1G_ALRAM_RESET"		));
	
	CDIO        DO_RFG_DNET_RESET		    (_TEXT("eDO_$0G_RFReset"		));

	//KLP_EDIT
	// RFPT Name Change
	CDIO        DO_RFM_DNET_RESET		    (_TEXT("eDO_$0_MAT_AlarmRST"	));

	CDIO        DO_RFG_POWER_ON				(_TEXT("DO_$1G_POWER_ON"		));	
	CDIO        DO_RFG_POWER_DNET_ON		(_TEXT("eDO_$0G_RFPower"		));	

	CDIO        DO_RFG_EXTERNAL_INK_OK		(_TEXT("DO_$1G_EXTERNAL_INK_OK"	));	

	CDIO		DI_RFG_POWER_ON				(_TEXT("DI_$1G_POWER_ON"		));	                                   
	CDIO        DI_RFG_WFS			        (_TEXT("DI_$1G_WFS"				));
	CDIO        DI_RFM_WFS			        (_TEXT("DI_$1M_WFS"				));
	
	//KLP_EDIT
	// 아래 IO3개는 code에서 사용하는 곳이 없어서 주석처리.
	//CDIO        DI_RFG_ALARM		        (_TEXT("DI_$1G_ALARM"			));
	//CDIO        DI_RFG_INTERLOCK			(_TEXT("DI_$1G_INTERLOCK"		));
	//CDIO        DI_RFG_LEAK_SENSOR			(_TEXT("DI_$1G_LEAK_SENSOR"		));

	CDIO        Fnc_RF_Mode		            (_TEXT("Fnc_$0_Mode"			));
	CDIO        Fnc_RF_CtrlSts		        (_TEXT("Fnc_$0_CtrlSts"			));

    CDIO        SRF_StandBy_RD              (_TEXT("SRF_StandBy_RD"			));
    CDIO        BRF_StandBy_RD              (_TEXT("BRF_StandBy_RD"			));
                                        
	CAIO        Fnc_RFG_SetPower	        (_TEXT("Fnc_$0_SetPower"		));
	CAIO        Fnc_RFM_SetTune		        (_TEXT("Fnc_$0M_SetTune"		));
	CAIO        Fnc_RFM_SetMatch	        (_TEXT("Fnc_$0M_SetMatch"		));
	CAIO        Fnc_RFG_SetRampTime	        (_TEXT("Fnc_$0_SetRampTime"		));
                                        
	CDIO        eDI_RFG_CommSts		        (_TEXT("eDI_$0G_CommSts"		));
	CDIO        eDI_RFM_CommSts		        (_TEXT("eDI_$0M_CommSts"		));
	
	//KLP_EDIT
	// RFPT Name Change
	CDIO        DI_RFGenErrorSts	        (_TEXT("eDI_$0_GenAlarm"		));	// RFPT Generator 종합 Alarm 비트
	CDIO        DI_RFMatErrorSts	        (_TEXT("eDI_$0_MAT_Alarm"		));	// RFPT Matcher 종합 Alarm 비트
	CDIO        DI_RFM_RemoteIn		        (_TEXT("eDI_$0_MAT_RemoteIn"	));	// RFPT Matcher Remote In 수락 여부를 확인

	//KLP_EDIT
	CAIO        AI_RFG_FwdPwr			    (_TEXT("eAI_$0_FwdPwr"			));
	CAIO        AI_RFG_RefPwr			    (_TEXT("eAI_$0_RefPwr"			));
	CAIO        AI_RFM_MatchPos		        (_TEXT("eAI_$0_MAT_MatchPos"	));
	CAIO        AI_RFM_TunePos		        (_TEXT("eAI_$0_MAT_TunePos"		));
	CAIO        AI_RFM_LoadPreset		    (_TEXT("eAI_$0_MAT_LoadPreset"	));	// RFPT Matcher Actual Load Preset readback 채널
	CAIO        AI_RFM_TunePreset		    (_TEXT("eAI_$0_MAT_TunePreset"	));	// RFPT Matcher Actual Tune Preset readback 채널

	CAIO        AI_RFG_FwdPwr_Set		    (_TEXT("eAI_$0_FwdPwr_Set"		));	// RFPT Generator Set Point Power readback 채널
	CAIO        AI_RFG_RFFreqIn			    (_TEXT("eAI_$0_RFFreqIn"		));	// RFPT Generator RF Frequency readback 채널

	CAIO        AI_RFG_ControlModeRb       (_TEXT("eAI_$0_ControlModeRb"   ));	// RFPT Generator byte54 Control Mode readback
	



                                        
	CAIO        eAO_RFG_FwdPwr			    (_TEXT("eAO_$0_FwdPwr"			));
	CAIO        eAO_RFM_MatchPos		    (_TEXT("eAO_$0_MAT_MatchPos"	));
	CAIO        eAO_RFM_TunePos		        (_TEXT("eAO_$0_MAT_TunePos"		));
	CAIO        eAO_RFM_LoadPreset		    (_TEXT("eAO_$0_MAT_LoadPreset"	));	// RFPT Matcher Preset용 Load Preset 채널
	CAIO        eAO_RFM_TunePreset		    (_TEXT("eAO_$0_MAT_TunePreset"	));	// RFPT Matcher Preset용 Tune Preset 채널

	//KLP_EDIT
	CAIO        eAO_RFG_RFFreqOut		    (_TEXT("eAO_$0_RFFreqOut"		));	// RFPT Generator RF Frequency 출력 채널
	CAIO        eAO_RFG_ControlMode        (_TEXT("eAO_$0_ControlMode"     ));	// RFPT Generator byte41 Control Mode write

                                        
	CDIO        eDI_RFG_GenRemoteMode		(_TEXT("eDI_$0_GenRemoteMode"	));
	CDIO        eDO_RFG_CtrlMode		    (_TEXT("eDO_$0G_CtrlMode"		));

	//KLP_EDIT 
	CDIO        eDO_RFG_MAT_Control			(_TEXT("eDO_$0_MAT_MatcherMode"	));	// enum {RFPT_Manual_0, RFPT_Auto_1, RFPT_Preset_2};
	CDIO        eDO_RFG_MAT_Remote		    (_TEXT("eDO_$0_MAT_Remote"		));	// RFPT Matcher Remote Out 제어 채널
	
	                                                                                                                                                            	
	CAlarm		SRF_6425					(_TEXT("SRF_6425"));
	CAlarm		BRF_6475					(_TEXT("BRF_6475"));

	CAlarm		SRF_6426					(_TEXT("SRF_6426"));
	CAlarm		BRF_6476					(_TEXT("BRF_6476"));	

	CAlarm		SRF_6427					(_TEXT("SRF_6427"));
	CAlarm		BRF_6477					(_TEXT("BRF_6477"));

	CAlarm		SRF_6428					(_TEXT("SRF_6428"));	//Source Match Position Preset Failed
	CAlarm		BRF_6478					(_TEXT("BRF_6478"));

	CAlarm		SRF_6429					(_TEXT("SRF_6429"));	//Source Tune Position Preset Failed
	CAlarm		BRF_6479					(_TEXT("BRF_6479"));

	
END_OBJECT_ENUMERATION
	

//----------------------------------------------------------------------------------------------------------------------------------------------
void Make_Log(char *SeparateSting, LPSTR list, ...) 
{
	char		FOLDER[64]	 =  { "Log\\SEQ\\RF_ACTION\\RF_SUB" };

	FILE *fpt;
	char FileName[512]; 
	
	SYSTEMTIME		SysTime;
	va_list			va;
	
	GetLocalTime(&SysTime);
	
	va_start(va, list);
	
	{
		sprintf(FileName, "%s\\%s_%04d%02d%02d.log", FOLDER, RFType, SysTime.wYear, SysTime.wMonth, SysTime.wDay);
		
		fpt = fopen(FileName, "a");
		
		if (fpt != NULL) 
		{
			fprintf(fpt, "%04d/%02d/%02d %02d:%02d:%02d:%02d\t%s\t", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds,SeparateSting);
			vfprintf(fpt, list, (LPSTR)va);
			fclose(fpt);
		}
	}
	
	
	va_end(va);
	
}

void AutoPreset_Log(char *SeparateSting, LPSTR list, ...) 
{
	char		FOLDER[64]	 =  { "Log\\SEQ\\RF_ACTION\\AutoPreset" };
	char		LOG[64]		 =  { "RF_AutoPreset" };

	FILE *fpt;
	char FileName[512]; 
	
	SYSTEMTIME		SysTime;
	va_list			va;
	
	GetLocalTime(&SysTime);
	
	va_start(va, list);
	
	{
		sprintf(FileName, "%s\\%s_%04d%02d%02d.log", FOLDER, LOG, SysTime.wYear, SysTime.wMonth, SysTime.wDay);
		
		fpt = fopen(FileName, "a");
		
		if (fpt != NULL) 
		{
			fprintf(fpt, "%04d/%02d/%02d %02d:%02d:%02d:%02d\t%s\t", SysTime.wYear, SysTime.wMonth, SysTime.wDay, SysTime.wHour, SysTime.wMinute, SysTime.wSecond, SysTime.wMilliseconds,SeparateSting);
			vfprintf(fpt, list, (LPSTR)va);
			fclose(fpt);
		}
	}
	
	
	va_end(va);
	
}


SEQ_STATUS INIT()
{
	int	nCS;
	int nCount = 0;
	char szAlarm[256] = {0};
	char szTemp[256] = {0};						// 일반 로그 메시지를 저장 버퍼

	double	dblCurSetPwr = 0.0;						// 현재 HW에 들어있는 Generator Set Point Power readback 값을 저장
	double	dblCurFreq = 0.0;						// 현재 HW에 들어있는 Generator Frequency readback 값을 저장
	
	Make_Log("[RFSUB_RUN]", "%s INIT() START \n ", RFType);

	if(eDI_RFG_CommSts.Read(nCS) != eOnline)
	{
		Make_Log("[RFSUB_RUN]", "%s INIT() ABORT. Generator Connection is Offline  \n ", RFType);
		Make_Log("[RFSUB_RUN]", "%s INIT() ABORT END \n ", RFType);
		return SEQ_ABORT;
	}

	if(eDI_RFM_CommSts.Read(nCS) != eOnline)
	{
		Make_Log("[RFSUB_RUN]", "%s INIT() ABORT. Matcher Connection is Offline  \n ", RFType);
		Make_Log("[RFSUB_RUN]", "%s INIT() ABORT END \n ", RFType);
		return SEQ_ABORT;
	}

	dblCurFreq = AI_RFG_RFFreqIn.Read(nCS);
	if(dblCurFreq <= 0 || dblCurFreq > 65535)
	{
		sprintf(szAlarm, "%s INIT() ABORT. Current RF Frequency readback is invalid(= %.0f).", RFType, dblCurFreq);
		Make_Log("[RFSUB_RUN]", "%s \n ", szAlarm);
		return SEQ_ABORT;
	}
	dblCurSetPwr = AI_RFG_FwdPwr_Set.Read(nCS);						// 현재 HW에 저장된 Generator Set Point Power readback 값을 읽는다.
	
	// -------------------------------------------------------------------------
	// 아래 2개 Write는 "새 값을 설정"하려는 목적이 아니라,
	// 현재 HW에 들어있는 값을 그대로 Driver output buffer에 미러링하기 위한 목적
	// Remote Out이 아직 OFF 상태이므로 RFPT 사양서 기준 장치가 명령을 무시하더라도,
	// Driver 내부 pWriteData 버퍼에는 값이 채워지므로 이후 Remote Out write 시
	// 기존 HW 주파수/SetPoint를 보존한 채 제어 비트만 보낼 수 있다.
	// -------------------------------------------------------------------------
	eAO_RFG_FwdPwr.Write(dblCurSetPwr, nCS);						// 현재 HW Set Point Power 값을 output buffer에 그대로 반영한다.
	eAO_RFG_RFFreqOut.Write(dblCurFreq, nCS);						// 현재 HW Frequency 값을 output buffer에 그대로 반영한다.
	
	sprintf(szTemp, "Mirror current HW values. SetPoint=%.0f, Frequency=%.0f kHz", dblCurSetPwr, dblCurFreq);	// 미러링된 값 로그 문자열을 만든다.
	Make_Log("[RFSUB_RUN]", "%s INIT(). %s \n ", RFType, szTemp);	// SetPoint/Frequency 미러링 로그를 남긴다.
	
	DO_RFG_EXTERNAL_INK_OK.Write(RF_Ready, nCS);					// 외부 Interlock OK 신호를 Ready 상태로 출력한다.
	Make_Log("[RFSUB_RUN]", "%s RF_ExtIntlk Set On(=Ready)  \n ", RFType);	// 외부 Interlock Ready 로그를 남긴다.
	_sleep(500);													// 장치가 상태를 안정적으로 반영할 시간을 준다.
	

	// -------------------------------------------------------------------------
	// RFPT Generator Control Mode(byte41) = 3(DeviceNet) 설정
	// 주의:
	// 1. byte41 Control Mode 와 byte4 bit3 Remote Out 은 서로 다른 항목이다.
	// 2. Control Mode 는 RF OFF 상태에서만 변경 가능하다.
	// 3. 현재 code의 eDO_RFG_CtrlMode 는 이름은 CtrlMode 이지만 실제로는 Remote Out(bit3) 채널이다.
	// -------------------------------------------------------------------------
	if(DI_RFG_POWER_ON.Read(nCS) == eON_STATUS)
	{
		Make_Log("[RFSUB_RUN]", "%s INIT() ABORT. RF is ON. Control Mode(byte41) can be changed only when RF OFF \n ", RFType);
		return SEQ_ABORT;
	}
	
	eAO_RFG_ControlMode.Write(OnlyDnet_3, nCS);		// RFPT Gen byte41 = 3(DeviceNet)
	Make_Log("[RFSUB_RUN]", "%s Control Mode Set DeviceNet(3) \n ", RFType);
	_sleep(500);
	
	if((int)AI_RFG_ControlModeRb.Read(nCS) != 3)
	{
		sprintf(szAlarm, "%s INIT() ABORT. Control Mode readback is not 3. ControlModeRb=%.0f",
			RFType,
			AI_RFG_ControlModeRb.Read(nCS));
		
		Make_Log("[RFSUB_RUN]", "%s \n ", szAlarm);
		return SEQ_ABORT;
	}	


	
	eDO_RFG_CtrlMode.Write(eRemote, nCS);							// RFPT Generator Remote Out을 ON으로 설정한다.
	Make_Log("[RFSUB_RUN]", "%s RF Generator Remote Out Set ON \n ", RFType);	// Generator Remote Out ON 로그를 남긴다.
	_sleep(500);													// Generator Remote 요청 반영 시간을 짧게 준다.
	
	eDO_RFG_MAT_Remote.Write(eRemote, nCS);							// RFPT Matcher Remote Out을 ON으로 설정한다.
	Make_Log("[RFSUB_RUN]", "%s RF Matcher Remote Out Set ON \n ", RFType);	// Matcher Remote Out ON 로그를 남긴다.
	_sleep(500);													// Matcher Remote 요청 반영 시간을 짧게 준다.

	
	if( (eDI_RFG_GenRemoteMode.Read(nCS) != eRemote) || (DI_RFM_RemoteIn.Read(nCS) != eRemote) )	// Generator Remote In과 Matcher Remote In 둘중 하나라도 OFF 면 에러
	{
		sprintf(szAlarm, "%s INIT() ABORT. Remote acknowledge Error. GenRemoteIn=%d, MatRemoteIn=%d",
			RFType,
			eDI_RFG_GenRemoteMode.Read(nCS),
			DI_RFM_RemoteIn.Read(nCS));													// Remote 수락 error 메시지를 만든다.

		Make_Log("[RFSUB_RUN]", "%s \n ", szAlarm);										// Remote 수락 error 로그를 남긴다.
		Make_Log("[RFSUB_RUN]", "%s INIT() ABORT END Remote Error ! \n ", RFType);						// INIT Abort 종료 로그를 남긴다.
		return SEQ_ABORT;	

	}


	Make_Log("[RFSUB_RUN]", "%s RF_Reset Set On->Off Run  \n ", RFType);


	DO_RFM_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);	//Matcher
	DO_RFM_DNET_RESET.Write(eOFF_CMD,   nCS);

	_sleep(1000);

	DO_RFG_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);	//generator
	DO_RFG_DNET_RESET.Write(eOFF_CMD,   nCS);

	// -------------------------------------------------------------------------
	// RFPT Matcher 기본 모드 설정
	// KYOSAN과 달리 RFPT는 Move bit가 아니라 MatcherMode 값 자체로 동작한다.
	// INIT 완료 후 기본 상태는 Auto(1)로 두는 것이 기존 KYOSAN 흐름과 가장 유사하다.
	// -------------------------------------------------------------------------
	eDO_RFG_MAT_Control.Write(eMATCH_AUTO_1, nCS);	// enum {RFPT_Manual_0, RFPT_Auto_1, RFPT_Preset_2};
	Make_Log("[RFSUB_RUN]", "%s RF Matcher Mode Set AUTO(1) \n ", RFType);				// Matcher Auto Mode 설정 로그를 남긴다.
	
	Make_Log("[RFSUB_RUN]", "%s INIT() END \n ", RFType);									// INIT 종료 로그를 남긴다.
	return SEQ_SUCCESS;
}


SEQ_STATUS READY()
{

	int	nCS;
	char szTemp[256] = {0};
	char szAlarm[512] = {0};

	Make_Log("[RFSUB_RUN]", "%s READY() START \n ", RFType);

	gnPR_RF_Ctrl		= PR_RF_CTRL.Read(nCS);
	gdPR_RFG_Power		= PR_RFG_POWER.Read(nCS);
	gdPR_RFM_Match		= PR_RFM_MATCH.Read(nCS);
	gdPR_RFM_Tune		= PR_RFM_TUNE.Read(nCS);

	sprintf(szTemp, "PR_RF_CTRL = %d, PR_RFG_POWER = %.1f, PR_RFM_MATCH = %.1f, PR_RFM_TUNE = %.1f", 
		gnPR_RF_Ctrl, gdPR_RFG_Power, gdPR_RFM_Match, gdPR_RFM_Tune);
	
	Make_Log("[RFSUB_RUN]", "%s READY() DATA SET. %s \n", RFType,szTemp);
	Make_Log("[RFSUB_RUN]", "%s READY() END \n ", RFType);

	return SEQ_SUCCESS;
}

SEQ_STATUS IsOn()
{
	int	   nCS;
	char   szTemp[256] ={0};
	double dRF_Power_RA;
	int    nRF_Power_DI;
	int    nCount=0;
	char   szAlarm[256] = {0};

	Make_Log("[RFSUB_RUN]", "%s IsOn() START \n ", RFType);
	
	while(1)
	{
		if(WaitAbort(100))
		{
			Make_Log("[RFSUB_RUN]", "%s IsOn() RUNNING. MANUAL ABORT \n ", RFType);
			Make_Log("[RFSUB_RUN]", "%s IsOn() ABORT END \n ", RFType);
			
			return SEQ_ABORT;
		}
	
		dRF_Power_RA = AI_RFG_FwdPwr.Read(nCS);
		nRF_Power_DI = DI_RFG_POWER_ON.Read(nCS);

		if((dRF_Power_RA > 100) && (nRF_Power_DI == eON_STATUS))			
		{
			sprintf(szTemp, "OnCheckCount = %d, FwdPwr = %.1f, DI_RF_PowerSts = %d[On:1]", nCount, dRF_Power_RA,nRF_Power_DI);	
			Make_Log("[RFSUB_RUN]", "%s IsOn() Check End. %s \n ", RFType,szTemp);
			
			break;
		}

		if(nCount > 20)
		{

			sprintf(szTemp, "OnCheckCount = %d, FwdPwr = %.1f, DI_RF_PowerSts = %d[On:1]", nCount, dRF_Power_RA,nRF_Power_DI);	
			Make_Log("[RFSUB_RUN]", "%s IsOn() Check Fail. %s \n ", RFType,szTemp);

			sprintf(szAlarm, "%s turn on failed. %s", OBJECT_NAME, szTemp);
			AlmAt(&SRF_6426 , gnAlarmOffset)->PostAnyTh(szAlarm);

			Make_Log("[RFSUB_RUN]", "%s IsOn() ABORT END \n ", RFType);


			return SEQ_ABORT;
		}

		nCount++;
		_sleep(100);
	}

	Make_Log("[RFSUB_RUN]", "%s IsOn() END \n ", RFType);

	return SEQ_SUCCESS;
}


SEQ_STATUS IsOff()
{
	int	   nCS;
	char   szTemp[256] = {0};
	double dRF_Power_RA =0.0;
	int    nRF_Power_DI = 0;
	int    nCount=0;
	char   szAlarm[256] = {0};

	Make_Log("[RFSUB_RUN]", "%s IsOff() START \n ", RFType);
	Make_Log("[RFSUB_RUN]", "%s IsOff() RUN. PM_RF_Cmd Set OFF \n ", RFType);

	while(1)
	{
		if(WaitAbort(100))
		{
			PM_RF_Cmd.Write(eOFF_CMD, nCS);

			Make_Log("[RFSUB_RUN]", "%s IsOff() RUNNING. MANUAL ABORT \n ", RFType);
			Make_Log("[RFSUB_RUN]", "%s IsOff() ABORT END \n ", RFType);

			return SEQ_ABORT;
		}

		dRF_Power_RA = AI_RFG_FwdPwr.Read(nCS);
		nRF_Power_DI = DI_RFG_POWER_ON.Read(nCS);

		if((dRF_Power_RA < 100) && (nRF_Power_DI == eOFF_STATUS))		
		{

			sprintf(szTemp, "OnCheckCount = %d, FwdPwr = %.1f, DI_RF_PowerSts = %d[On:1]", nCount, dRF_Power_RA,nRF_Power_DI);	
			Make_Log("[RFSUB_RUN]", "%s IsOff() Check End. %s \n ", RFType,szTemp);

			break;
		}
		
		if(nCount > 20)	
		{
			PM_RF_Cmd.Write(eOFF_CMD, nCS);

			sprintf(szTemp, "OnCheckCount = %d, FwdPwr = %.1f, DI_RF_PowerSts = %d[On:1]", nCount, dRF_Power_RA,nRF_Power_DI);	
			Make_Log("[RFSUB_RUN]", "%s IsOff() Check Fail. %s \n ", RFType,szTemp);

			sprintf(szAlarm, "%s turn off failed. %s", OBJECT_NAME, szTemp);
			AlmAt(&SRF_6427 , gnAlarmOffset)->PostAnyTh(szAlarm);
			
			return SEQ_ABORT;
		}

		nCount++;
		_sleep(100);	
	}

	Make_Log("[RFSUB_RUN]", "%s IsOff() END \n ", RFType);

	return SEQ_SUCCESS;
}

SEQ_STATUS ControlOff()
{
	char   szTemp[256] = {0};

	int	   nCS;
	int    nRF_Power_DI;
	int    nResult = 0;

	Make_Log("[RFSUB_RUN]", "%s ControlOff() START \n ", RFType);
	Make_Log("[RFSUB_RUN]", "%s ControlOff() RUN. PM_RF_Cmd and DO_RF_Cmd Set OFF \n ", RFType);
	
	PM_RF_Cmd.Write(eOFF_CMD, nCS);

	DO_RFG_POWER_ON.Write(eOFF_CMD, nCS);
	DO_RFG_POWER_DNET_ON.Write(eOFF_CMD, nCS);
	
	nRF_Power_DI = DI_RFG_POWER_ON.Read(nCS);

	Make_Log("[RFSUB_RUN]", "%s ControlOff() Check. DI_RF_PowerSts(= %d) \n ", RFType,nRF_Power_DI);

	if(nRF_Power_DI == eOFF_STATUS)
	{

		Make_Log("[RFSUB_RUN]", "%s Is already Off. Set PM_RF_Cmd(= eOFF_CMD) \n ", RFType);
		Make_Log("[RFSUB_RUN]", "%s ControlOff() RUN. PM_RF_Cmd and DO_RF_Cmd Set OFF one more \n ", RFType);

		PM_RF_Cmd.Write(eOFF_CMD, nCS);
		DO_RFG_POWER_ON.Write(eOFF_CMD, nCS);
		DO_RFG_POWER_DNET_ON.Write(eOFF_CMD, nCS);

		Make_Log("[RFSUB_RUN]", "%s ControlOff() END \n ", RFType);

		
		return SEQ_SUCCESS;
	}
	else
	{
		Make_Log("[RFSUB_RUN]", "%s Is Not off. Wait 0.5 sec \n ", RFType);

		_sleep(500);	
		Make_Log("[RFSUB_RUN]", "%s ControlOff() RUN. PM_RF_Cmd and DO_RF_Cmd Set OFF \n ", RFType);

	}

	Make_Log("[RFSUB_RUN]", "%s ControlOff() END \n ", RFType);

	return SEQ_SUCCESS;
}


SEQ_STATUS SET_RF_POWER()
{
	int	   nCS = 0;
	int    nCount=0;

	int    nWrSetpoint = 0;							// 실제 Driver에 기록될 정수형 Set Point 값을 저장하는 변수 

	int    nResult = 0;
	int    nTimeout = 30;

	char   szTemp[256]    = {0};
	char   szGetData1[64] = {0};
	char   szGetData2[64] = {0};
	double dblSetpoint = 0.0;
	double dblRdSetpoint = 0.0;
	double dblCT = 0.0;
	double dblCL = 0.0;
	char   szAlarm[256] = {0};

	int    nCheck = -1;

	Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER() START \n ", RFType);

	dblSetpoint = PR_RFG_POWER.Read(nCS);
	nWrSetpoint = (int)dblSetpoint;					// Driver.cpp가 int 형식으로 write하므로 실제 전송될 값을 정수형으로 변환 
	
	if(nWrSetpoint < 0)								// 비정상적으로 음수값이 들어온 경우 0
	{
		nWrSetpoint = 0;							// RF Power는 음수가 될 수 없으므로 0
	}

	eAO_RFG_FwdPwr.Write((double)nWrSetpoint, nCS);	// RFPT Generator의 Set Point Power 출력 타입 변환 
	_sleep(1000);

	sprintf(szTemp, "SetPoint Write Start. Req=%.1f, Write=%d", dblSetpoint, nWrSetpoint);	
	Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER(). %s \n ", RFType, szTemp);					
	

	dblRdSetpoint = AI_RFG_FwdPwr_Set.Read(nCS);	// RFPT Generator의 Set Point Power readback 값을 read


	if((int)dblRdSetpoint == nWrSetpoint)		// readback 값이 실제 write된 정수형 Set Point와 일치하면 정상 반영으로 판단한다.
	{
		sprintf(szTemp, "SetPoint Write OK. Write=%d, ReadBack=%.0f ", nWrSetpoint, dblRdSetpoint);	// 성공 로그 문자열을 만든다.
		Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER(). %s \n ", RFType, szTemp);											// 성공 로그를 남긴다.
		Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER() END \n ", RFType);													// 함수 종료 로그를 남긴다.
		return SEQ_SUCCESS;						// 정상 완료이므로 Success를 반환한다.
	}
	else
	{
		sprintf(szAlarm, "Set point power set failed. Write=%d, ReadBack=%.0f", nWrSetpoint, dblRdSetpoint);	// 최종 실패 알람 메시지를 만든다.

		AlmAt(&SRF_6426 , gnAlarmOffset)->PostAnyTh();
		Make_Log("[RFSUB_RUN]", "%s Failed to set Power = %.0f \n ", RFType,(double)nWrSetpoint);
		Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER() ABORT END \n ", RFType);

		return SEQ_ABORT;
	}	
}


SEQ_STATUS ControlPreset()
{
	int	nCS = 0;
	int nCount=0;

	int    nMatchOk = FALSE;							// Match Preset 완료 여부를 저장하는 변수
	int    nTuneOk = FALSE;								// Tune Preset 완료 여부를 저장하는 변수

	char	szTemp[256] = {0};
	char	szAlarm[256] = {0};

	double dblTune = 0.0;
	double dblMatch = 0.0;
 
    double dblPreTune = 0.0;
    double dblPreMatch = 0.0;

	double dblCurTune = 0.0;
	double dblCurMatch = 0.0;

	Make_Log("[RFSUB_RUN]", "%s ControlPreset() START \n ", RFType);

    if( SRF_StandBy_RD.Read(nCS) != Ready || BRF_StandBy_RD.Read(nCS) != Ready )
    {
		Make_Log("[RFSUB_RUN]", "%s StandBy_Mode Error  END \n ", RFType);

        if( SRF_StandBy_RD.Read(nCS) != Ready )	SRF_6425.PostAnyTh();
        if( BRF_StandBy_RD.Read(nCS) != Ready )	BRF_6475.PostAnyTh();
   
		Make_Log("[RFSUB_RUN]", "%s ControlPreset() ABORT END \n ", RFType);

        return SEQ_ABORT;
    }
	
	if(DI_RFMatErrorSts.Read(nCS) == eRF_FaultSts_Alarm)					// Matcher 종합 Alarm이 이미 ON이면 Preset을 진행하지 않는다.
	{
		sprintf(szAlarm, "%s matcher alarm is already ON before preset.", OBJECT_NAME);	// Preset 전 Alarm 상태 메시지를 만든다.
		Make_Log("[RFSUB_RUN]", "%s ControlPreset() ABORT. %s \n ", RFType, szAlarm);	// Preset 전 Alarm 상태 로그를 남긴다.
		Make_Log("[RFSUB_RUN]", "%s ControlPreset() ABORT END \n ", RFType);			// 함수 Abort 종료 로그를 남긴다.
		return SEQ_ABORT;																// Alarm 상태이므로 Abort를 반환한다.
	}

	Make_Log("[RFSUB_RUN]", "%s ControlPreset() RF ALARM Reset Set On->Off Run  \n ", RFType);
	
	DO_RFM_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);
	DO_RFM_DNET_RESET.Write(eOFF_CMD,   nCS);
	_sleep(1000);
	
	DO_RFG_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);
	DO_RFG_DNET_RESET.Write(eOFF_CMD,   nCS);


    // until here.
	//step 1
	dblTune		= PR_RFM_TUNE.Read(nCS);
	dblMatch	= PR_RFM_MATCH.Read(nCS);

	eAO_RFM_LoadPreset.Write(dblMatch, nCS);								// RFPT Matcher의 Load Preset 출력 채널에 목표 Preset 값을 기록한다.
	eAO_RFM_TunePreset.Write(dblTune, nCS);									// RFPT Matcher의 Tune Preset 출력 채널에 목표 Preset 값을 기록한다.
	sprintf(szTemp, "Preset Write. LoadPreset=%.1f, TunePreset=%.1f", dblMatch, dblTune);	// Preset write 로그 문자열을 만든다.
	Make_Log("[RFSUB_RUN]", "%s ControlPreset() %s \n ", RFType, szTemp);					// Preset write 로그를 남긴다.

	eDO_RFG_MAT_Control.Write(eMATCH_PRESET_2, nCS);						// RFPT Matcher Mode를 Preset(2)로 설정 // 이 단계에서 Move 일어난다. 
	Make_Log("[RFSUB_RUN]", "%s ControlPreset() Matcher Mode Set PRESET(2) \n ", RFType);	// Matcher Mode Preset 설정 로그를 남긴다.


	// STEP 2
	for(nCount = 0; nCount < 30; nCount++)									// 최대 30회(약 3초) 동안 Preset readback과 실제 위치 도달 여부를 확인한다.
	{
		if(WaitAbort(100))													// 100ms 단위로 수동 Abort를 감시한다.
		{
			Make_Log("[RFSUB_RUN]", "%s ControlPreset() MANUAL ABORT while waiting preset complete \n ", RFType);	// 수동 Abort 로그를 남긴다.
			Make_Log("[RFSUB_RUN]", "%s ControlPreset() ABORT END \n ", RFType);									// 함수 Abort 종료 로그를 남긴다.
			return SEQ_ABORT;												// 수동 Abort 이므로 Abort를 반환한다.
		}
		
		dblPreMatch = AI_RFM_LoadPreset.Read(nCS);							// Actual Load Preset readback 값을 읽는다.
		dblPreTune  = AI_RFM_TunePreset.Read(nCS);							// Actual Tune Preset readback 값을 읽는다.
		dblCurMatch = AI_RFM_MatchPos.Read(nCS);							// Actual Load Position 값을 읽는다.
		dblCurTune  = AI_RFM_TunePos.Read(nCS);								// Actual Tune Position 값을 읽는다.
		
		if( (dblPreMatch == dblMatch) && (dblCurMatch == dblMatch) )	// Load Preset readback과 Actual Position이 모두 목표값이면 Match 완료로 판단한다.
		{
			nMatchOk = TRUE;												// Match Preset 완료 플래그를 ON으로 설정한다.
		}
		else
		{
			nMatchOk = FALSE;												// 조건을 만족하지 않으면 Match Preset 미완료로 유지한다.
		}
		
		if( (dblPreTune == dblTune) && (dblCurTune == dblTune) )		// Tune Preset readback과 Actual Position이 모두 목표값이면 Tune 완료로 판단한다.
		{
			nTuneOk = TRUE;													// Tune Preset 완료 플래그를 ON으로 설정한다.
		}
		else
		{
			nTuneOk = FALSE;												// 조건을 만족하지 않으면 Tune Preset 미완료로 유지한다.
		}
		
		if(nMatchOk == TRUE && nTuneOk == TRUE)								// Match와 Tune이 모두 완료되었으면 Preset 성공으로 종료한다.
		{
			sprintf(szTemp, "Preset OK. LoadSet=%.1f, LoadRB=%.1f, LoadPos=%.1f, TuneSet=%.1f, TuneRB=%.1f, TunePos=%.1f",
				dblMatch, dblPreMatch, dblCurMatch, dblTune, dblPreTune, dblCurTune);	// 성공 로그 작성
			Make_Log("[RFSUB_RUN]", "%s ControlPreset() %s \n ", RFType, szTemp);		// 성공 로그
			Make_Log("[RFSUB_RUN]", "%s ControlPreset() END \n ", RFType);				// 종료 로그
			return SEQ_SUCCESS;			
		}
	}

	if(nMatchOk == FALSE)												// Match 축 실패면
	{
		sprintf(szAlarm, "%s match preset failed. Set=%.1f, PresetRB=%.1f, PosRB=%.1f", OBJECT_NAME, dblMatch, dblPreMatch, dblCurMatch);	// 알람 메시지
		AlmAt(&SRF_6428 , gnAlarmOffset)->PostAnyTh(szAlarm);			// 기존 Match Preset 실패 알람 사용
		Make_Log("[RFSUB_RUN]", "%s ControlPreset() %s \n ", RFType, szAlarm);	// 실패 로그
	}
	
	if(nTuneOk == FALSE)												// Tune 축 실패면
	{
		sprintf(szAlarm, "%s tune preset failed. Set=%.1f, PresetRB=%.1f, PosRB=%.1f", OBJECT_NAME, dblTune, dblPreTune, dblCurTune);	// 알람 메시지
		AlmAt(&SRF_6429 , gnAlarmOffset)->PostAnyTh(szAlarm);			// 기존 Tune Preset 실패 알람 사용
		Make_Log("[RFSUB_RUN]", "%s ControlPreset() %s \n ", RFType, szAlarm);	// 실패 로그
	}
	
	Make_Log("[RFSUB_RUN]", "%s ControlPreset() ABORT END \n ", RFType);	// 종료 로그
	return SEQ_ABORT;											// Preset 완료로 Success를 반환한다.
}


SEQ_STATUS MinMax_AutoPreset()
{
	Make_Log("[RFSUB_RUN]", "%s MinMax_AutoPreset() START \n ", RFType);		
	Make_Log("[RFSUB_RUN]", "%s MinMax_AutoPreset() ABORT. AutoPreset function is not supported in current RFPT build \n ", RFType);	
	Make_Log("[RFSUB_RUN]", "%s MinMax_AutoPreset() ABORT END \n ", RFType);	
	
	return SEQ_ABORT;		
}


SEQ_STATUS PROCESS()
{
	int	   nCS;
	char   szTemp[256] = {0};
	int    nFncMode = 0;
	char   szStr[64] = {0};
	int    nFnResult = 0;

	Make_Log("[RFSUB_RUN]", "%s PROCESS() START \n ", RFType);

	nFncMode = PR_RF_CTRL.Read(nCS);
			   PR_RF_CTRL.ReadAndGetStr(szStr, nCS);


	sprintf(szTemp, "PR_RF = %s", szStr);
	Make_Log("[RFSUB_RUN]", "%s %s \n ", RFType,szTemp);

	switch (nFncMode)
	{
		case eRF_OFF : 
				Make_Log("[RFSUB_RUN]", "%s PROCESS(). ControlOff() called by FNC_PROCESS \n ", RFType);
				nFnResult = ControlOff();
				break;

		case eRF_ON :
				//KLP_EDIT
				Make_Log("[RFSUB_RUN]", "%s PROCESS(). eRF_ON mode is not handled in PROCESS(). Use POWERON command path instead \n ", RFType);	
				nFnResult = SEQ_ABORT;																											
				break;
		
		case eRF_DUAL :
				//KLP_EDIT
				Make_Log("[RFSUB_RUN]", "%s PROCESS(). eRF_DUAL mode is controlled by FNC_RF_CTRL, not by FNC_RF PROCESS() \n ", RFType);		
				nFnResult = SEQ_ABORT;																											
				break;

		case eRF_PRESET :
				Make_Log("[RFSUB_RUN]", "%s PROCESS(). ControlPreset() called by FNC_PROCESS \n ", RFType);
				nFnResult = ControlPreset();
				break;

		case eRF_RAMP :
				nFnResult = SEQ_ABORT;
				break;

		default :
				//KLP_EDIT
				Make_Log("[RFSUB_RUN]", "%s PROCESS(). Unknown PR_RF_CTRL mode = %d \n ", RFType, nFncMode);										// 정의되지 않은 RF 제어 모드가 들어왔음을 로그로 남긴다.
				nFnResult = SEQ_ABORT;																												// 알 수 없는 모드이므로 Abort를 반환한다.
				break;
	}

	if(nFnResult == SEQ_ABORT)
	{

		Make_Log("[RFSUB_RUN]", "%s PROCESS(). Set PM_RF_Cmd(= eOFF_CMD) \n ", RFType);
		PM_RF_Cmd.Write(eOFF_CMD, nCS);

		Make_Log("[RFSUB_RUN]", "%s PROCESS() FUCTION ABORT END \n ", RFType);

		return SEQ_ABORT;
	}
	

	if(WaitAbort(100))
	{
		Make_Log("[RFSUB_RUN]", "%s PROCESS() MANUAL ABORT END \n ", RFType);
		return SEQ_ABORT;
	}

	Make_Log("[RFSUB_RUN]", "%s PROCESS() END \n ", RFType);

	return SEQ_SUCCESS;
}



SEQ_STATUS ABORT()
{
	
	int nCS;
	char  szTemp[256]	= {0};

	Make_Log("[RFSUB_RUN]", "%s ABORT() START \n ", RFType);

	Make_Log("[RFSUB_RUN]", "%s ABORT() RUN. PM_RF_Cmd and DO_RF_Cmd Set OFF \n ", RFType);

	PM_RF_Cmd.Write(eOFF_CMD, nCS);
	DO_RFG_POWER_ON.Write(eOFF_CMD, nCS);
	DO_RFG_POWER_DNET_ON.Write(eOFF_CMD, nCS);

	_sleep(100);

	Make_Log("[RFSUB_RUN]", "%s ABORT() RUN. PM_RF_Cmd and DO_RF_Cmd Set OFF[ONE MORE] \n ", RFType);

	PM_RF_Cmd.Write(eOFF_CMD, nCS);
	DO_RFG_POWER_ON.Write(eOFF_CMD, nCS);
	DO_RFG_POWER_DNET_ON.Write(eOFF_CMD, nCS);

	Make_Log("[RFSUB_RUN]", "%s ABORT() RUN. FORWARD POWER  Set ZERO \n ", RFType);
	eAO_RFG_FwdPwr.Write(0);	

	Make_Log("[RFSUB_RUN]", "%s ABORT() ENDED \n ", RFType);

	return SEQ_SUCCESS;
}



//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
SEQ_STATUS SeqMain(const char* szCommand, const void* pParam, int nNumOfBytesRecved) 
{
	printf("-----------> [ %s ] START... [%s]\n", GetSeqName(), szCommand);
	
	SEQ_STATUS nResult = SEQ_SUCCESS;
	
	TString strCommand = szCommand;
	TStringArray strTokens = strCommand.GetToken( _TEXT(" \t") );

	int		CS			= 0;
	int		nFncMode	= 0;
	double	dblTune		= 0.0;
	double	dblMatch	= 0.0;
	double	dblSetPwr	= 0.0;
	
	if(gnAlarmOffset == BRF)	
	{
		sprintf(OBJECT_NAME, "BIAS RF");
		sprintf(OBJECT_HEADER, "B");
	}
	else						
	{
		sprintf(OBJECT_NAME, "SOURCE RF");
		sprintf(OBJECT_HEADER, "S");
	}


	if( strTokens.count() < 1 ) 
	{
		Make_Log("[SEQ_MAIN]", "Not Support strTokens.count()[%s]....return Abort \n", szCommand);		
		return SEQ_ABORT;
	}
	
	if     (strTokens[0] == _TEXT("NONE"		))		nResult = SEQ_SUCCESS;
	else if(strTokens[0] == _TEXT("INIT"		))		nResult = INIT();	
	else if(strTokens[0] == _TEXT("READY"		))		nResult = READY();
	else if(strTokens[0] == _TEXT("PROCESS"		))		nResult = PROCESS();
	else if(strTokens[0] == _TEXT("ABORT"		))		nResult = ABORT();	
	else if(strTokens[0] == _TEXT("AUTOPRESET"	))		nResult = MinMax_AutoPreset();
	else if(strTokens[0] == _TEXT("POWERON"		))		nResult = SET_RF_POWER();
	else if(strTokens[0] == _TEXT("POWEROFF"	))		nResult = ControlOff();
	else if(strTokens[0] == _TEXT("PULSEON"		))
	{
		Make_Log("[SEQ_MAIN]", "%s PULSEON is not supported in current RFPT build \n ", RFType);	
		nResult = SEQ_ABORT;																		
	}
	else if(strTokens[0] == _TEXT("PRESET"		))		nResult = ControlPreset();
	else if(strTokens[0] == _TEXT("FUNCTION"))
	{
		Fnc_RF_CtrlSts.Write(FNC_RUNNING, CS);

		nFncMode = Fnc_RF_Mode.Read(CS);
	
		if		(nFncMode == eRF_NONE	)	nResult = SEQ_SUCCESS;
		else if	(nFncMode == eRF_INIT	)	nResult = INIT();
		else if	(nFncMode == eRF_OFF	)	nResult = ControlOff();
		else if	(nFncMode == eRF_PRESET	)
		{
			dblTune		= Fnc_RFM_SetTune.Read(CS);
			dblMatch	= Fnc_RFM_SetMatch.Read(CS);	
			PR_RFM_TUNE.Write(dblTune, CS);
			PR_RFM_MATCH.Write(dblMatch, CS);
			
			nResult = ControlPreset();
		}
		else if(nFncMode == eRF_ON)
		{
			dblSetPwr = Fnc_RFG_SetPower.Read(CS);
			PR_RFG_POWER.Write(dblSetPwr, CS);

			dblTune		= Fnc_RFM_SetTune.Read(CS);
			dblMatch	= Fnc_RFM_SetMatch.Read(CS);	
			PR_RFM_TUNE.Write(dblTune, CS);
			PR_RFM_MATCH.Write(dblMatch, CS);

			nResult = SEQ_ABORT;
		}
		else
		{
			nResult = SEQ_ABORT;
		}

		if(nResult == SEQ_SUCCESS)	Fnc_RF_CtrlSts.Write(FNC_IDLE, CS);
		else						Fnc_RF_CtrlSts.Write(FNC_ABORTED, CS);
	
	}
	else 
	{
		Make_Log("[SeqMain]", "SYS_ABORTED : Unknown command(= %s) \n", strTokens[0]);
		nResult = SEQ_ABORT;
	}
	
	Make_Log("[SEQ_MAIN]", "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");


	return nResult;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------

//----------------------------------------------------------------------------------------------------------------------------------------------
void OnLoadComplete( const TCHAR* szArgument ) 
{	
	char szLog[128] = {0};
	char szLog2[128] = {0};
	
	TString strCommand = szArgument;
	TStringArray strTokens = strCommand.GetToken( _TEXT(" |") );
		
	if(strTokens[0] == _TEXT("SRF") == TRUE)			
	{
		sprintf(OBJECT_NAME, "SOURCE RF");
		sprintf(OBJECT_HEADER, "S");
	
		gnAlarmOffset = SRF;
		ReplaceObjectName( "$0" , "SRF"	);
		ReplaceObjectName( "$1" , "SOURCE_RF"	);		
		sprintf(RFType,"SOURCE_RF");
	}
	else if(strTokens[0] == _TEXT("BRF") == TRUE)	
	{
		sprintf(OBJECT_NAME, "BIAS RF");
		sprintf(OBJECT_HEADER, "B");

		gnAlarmOffset = BRF;
		ReplaceObjectName( "$0" , "BRF"	);
		ReplaceObjectName( "$1" , "BIAS_RF"	);
		sprintf(RFType,"BIAS_RF");

	}
	

}
//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL OnInitComplete( const TCHAR* szArgument )
{
	int CommStatus;
	if ( SIMULATION_CONTROL.Read(CommStatus) == ON ) SIMULATION_MODE = TRUE;
	else											 SIMULATION_MODE = FALSE;

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnUnLoad( int nCode ) 
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnEventRecv( const char* szCommand, const void* pParam , int nNumOfBytesRecved ) 
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
