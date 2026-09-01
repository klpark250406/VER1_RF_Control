#include <windows.h>
#include <stdio.h>
#include <math.h>
#include <process.h>
#include <time.h>

#include <tMain.h>
#include <TUtilStr.h>
#include <TTimer.h>
#include <TString.h>

//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL SIMULATION_MODE = FALSE;
//----------------------------------------------------------------------------------------------------------------------------------------------
enum	{OFF   , ON };
enum	{eON   , eOFF };
enum	{ESC_UNKNOWN, ESC_OFF, ESC_ON};
enum	{TIMER_IDLE, TIMER_RUN};
enum	{ePM_PROC_IDLE, ePM_PROC_RUNNING};
enum	{eRF_OFF1, eRF_HIGH, eRF_LOW, eRF_BOTH};
enum	{eFnc_RF_NONE, eFnc_RF_INIT, eFnc_RF_PRESET, eFnc_RF_POWEROFF, eFnc_RF_POWERON, eFnc_RF_RAMPUP};
enum	{eAlarm, eIdle};
enum	{eUNKNOWN1, eATM};
enum	{eUNKNOWN2, eVAC};
enum	{eNC_Close, eNC_Open};
enum	{eNO_Open, eNO_Close};
enum	{GATE_NONE, GATE_OPEN, GATE_CLOSE, GATE_ERROR};
enum	{eSTOP, eRUN};
enum	{eNotFaults, eFaluts};
enum	{eIntlk, eNotIntlk};
enum	{NORMAL_OFF, INTERLOCKED_OFF};
enum	{RAMP_DISABLE,RAMP_ENABLE};
enum	{eNG, eOK};
enum    {eErrorOff, eErrorOn};
enum	{GateNone, GateOpen, GateClose, GateError};
enum	{SOURCE_FIRST, BIAS_FIRST};
enum	{RF_OFF, RF_ON};
enum	{FNC_IDLE, FNC_RUNNING, FNC_ABORTED};
enum  {eInterface, eDeviceNet};

//KLP_EDIT
// RFPT Remote check.
enum	{REMOTE_OFF_0, REMOTE_ON_1};	// 0 : Remote OFF / 1 :Remote ON
// -----------------------------------------------------------------------------
// RFPT Generator Byte5 Mode 전용 enum
// 사양서 근거:
// - 하위니블(bit0~3) : Power Regulation Mode
// - 상위니블(bit4~7) : Ramp Mode
// -----------------------------------------------------------------------------
enum	{eMode_Reg_Forward_0, eMode_Reg_Load_1, eMode_Reg_External_2, eMode_Reg_VA_Limit_3};
enum	{eMode_Ramp_Disable_0, eMode_Ramp_Watt_1, eMode_Ramp_Timed_2};

//----------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_OBJECT_ENUMERATION

	CDIO			SIMULATION_CONTROL				("System.SimulMode"				);

	CSequence		FNC_SRF							(_TEXT("FNC_SRF"				));
	CSequence		FNC_BRF							(_TEXT("FNC_BRF"				));
	CSequence		FNC_VVC_MOTOR					(_TEXT("FNC_VVC_MOTOR"			));
                                          
	CDIO			PM_Start_Flag					(_TEXT("PM_Start_Flag"			));
	CDIO			PM_PROC_STS						(_TEXT("PM_PROC_STS"			));
                                               
	CDIO			PM_PROC_RFCTRL		            (_TEXT("PM_PROC_RFCTRL"			));
    CDIO			Fnc_RF_Mode						(_TEXT("Fnc_RF_Mode"			));
	CDIO			PM_ReleaseESC		            (_TEXT("PM_ReleaseESC"			));
                                               
	CDIO			PM_SRF_Cmd			            (_TEXT("PM_SRF_Cmd"				));
	CDIO			PM_BRF_Cmd			            (_TEXT("PM_BRF_Cmd"				));
	CDIO			PM_StartTimer		            (_TEXT("PM_StartTimer"			));
	CAIO			PM_GlassInTime		            (_TEXT("PM_GlassInTime"			));
                                               
	CAIO			PR_SRFG_POWER					(_TEXT("PR_SRFG_POWER"			));
	CAIO			PR_SRFM_MATCH					(_TEXT("PR_SRFM_MATCH"			));
	CAIO			PR_SRFM_TUNE					(_TEXT("PR_SRFM_TUNE"			));
                                               
	CAIO			PR_BRFG_POWER					(_TEXT("PR_BRFG_POWER"			));
	CAIO			PR_BRFM_MATCH					(_TEXT("PR_BRFM_MATCH"			));
	CAIO			PR_BRFM_TUNE					(_TEXT("PR_BRFM_TUNE"			));
                                               
	CDIO			PM_RF_CTRL			            (_TEXT("PM_RF_CTRL"				));
	CDIO			PM_RF_STS			            (_TEXT("PM_RF_STS"				));
                                               
	CDIO			Fnc_SRF_Mode					(_TEXT("Fnc_SRF_Mode"			));
	CDIO			Fnc_BRF_Mode					(_TEXT("Fnc_BRF_Mode"			));
                                               
	CAIO			Fnc_SRF_SetPower			    (_TEXT("Fnc_SRF_SetPower"		));
	CAIO			Fnc_BRF_SetPower	            (_TEXT("Fnc_BRF_SetPower"		));
                                               
	CAIO			Fnc_SRFM_SetTune			    (_TEXT("Fnc_SRFM_SetTune"		));
	CAIO			Fnc_SRFM_SetMatch				(_TEXT("Fnc_SRFM_SetMatch"		));
                                               
	CAIO			Fnc_BRFM_SetTune				(_TEXT("Fnc_BRFM_SetTune"		));
	CAIO			Fnc_BRFM_SetMatch				(_TEXT("Fnc_BRFM_SetMatch"		));
                                               
	CAIO			Fnc_SRF_SetRampTime             (_TEXT("Fnc_SRF_SetRampTime"	));
	CAIO			Fnc_BRF_SetRampTime             (_TEXT("Fnc_BRF_SetRampTime"	));

	CDIO			DI_RF_SHIELD_COVER1	            (_TEXT("DI_RF_SHIELD_COVER1"		));
	CDIO			DI_RF_SHIELD_COVER2	            (_TEXT("DI_RF_SHIELD_COVER2"		));
	CDIO			DI_RF_SHIELD_COVER3	            (_TEXT("DI_RF_SHIELD_COVER3"		));
	//CDIO			DI_RF_SHIELD_COVER4	            (_TEXT("DI_RF_SHIELD_COVER4"		)); //26.06.01 KJC COVER 1~3
	CDIO			DI_RF_FEEDER_COVER_OPEN	        (_TEXT("DI_RF_FEEDER_COVER_OPEN"	));

	CDIO			DI_SOURCE_MATCHER_WLS			(_TEXT("DI_SOURCE_MATCHER_WLS"		));
	CDIO			DI_BIAS_MATCHER_WLS				(_TEXT("DI_BIAS_MATCHER_WLS"		));
	CDIO			DI_SOURCE_RFM_WFS		        (_TEXT("DI_SOURCE_RFM_WFS"			));
	CDIO			DI_BIAS_RFM_WFS		            (_TEXT("DI_BIAS_RFM_WFS"			));

	//KLP_EDIT WLS IO name Edit
	CDIO			DI_SOURCE_RFG_WATER_LEAK_OK		(_TEXT("DI_SOURCE_RFG_WATER_LEAK_OK"	));
	CDIO			DI_BIAS_RFG_WATER_LEAK_OK		(_TEXT("DI_BIAS_RFG_WATER_LEAK_OK"	));	
	CDIO			DI_SOURCE_RFG_WFS		        (_TEXT("DI_SOURCE_RFG_WFS"			));
	CDIO			DI_BIAS_RFG_WFS		            (_TEXT("DI_BIAS_RFG_WFS"			));

	CDIO			DI_PROCESS_GAS_SUPPLY_VALVE	    (_TEXT("DI_PROCESS_GAS_SUPPLY_VALVE"));
   
//	CDIO			DI_SOURCE_RFG_INTERLOCK			(_TEXT("DI_SOURCE_RFG_INTERLOCK"	));
//	CDIO			DI_SOURCE_RFG_REMOTE_MODE		(_TEXT("DI_SOURCE_RFG_REMOTE_MODE"	));
//	CDIO			DI_SOURCE_RFG_ALARM				(_TEXT("DI_SOURCE_RFG_ALARM"		));
		
//	CDIO			DI_BIAS_RFG_INTERLOCK			(_TEXT("DI_BIAS_RFG_INTERLOCK"		));
//	CDIO			DI_BIAS_RFG_REMOTE_MODE			(_TEXT("DI_BIAS_RFG_REMOTE_MODE"	));
//	CDIO			DI_BIAS_RFG_ALARM				(_TEXT("DI_BIAS_RFG_ALARM"			));

	// DIO
	CDIO			DO_SOURCE_RFG_POWER_ON			(_TEXT("DO_SOURCE_RFG_POWER_ON"		));
	CDIO			rDO_SOURCE_RFG_POWER_ON			(_TEXT("rDO_SOURCE_RFG_POWER_ON"	));	

	CDIO			DO_BIAS_RFG_POWER_ON			(_TEXT("DO_BIAS_RFG_POWER_ON"		));
	CDIO			rDO_BIAS_RFG_POWER_ON			(_TEXT("rDO_BIAS_RFG_POWER_ON"		));

	CDIO			DI_SOURCE_RFG_POWER_ON			(_TEXT("DI_SOURCE_RFG_POWER_ON"		));
	CDIO			DI_BIAS_RFG_POWER_ON			(_TEXT("DI_BIAS_RFG_POWER_ON"		));
	
	//ALARM
	//KLP_EDIT
	// RFPT 기준 Alarm 종합비트 변경. 
	CDIO			eDI_SRF_GenFailSts				(_TEXT("eDI_SRF_GenAlarm"			));	// RFPT Gen 챕터5 byte28 bit2: Alarm 1=알람발생(종합)
	CDIO			eDI_SRF_MAT_Fail				(_TEXT("eDI_SRF_MAT_Alarm"			));	// RFPT Mat 챕터9 bit7: Alarm 종합알람
	CDIO			eDI_BRF_GenFailSts				(_TEXT("eDI_BRF_GenAlarm"			));	// RFPT Mat 챕터9 bit7: Alarm 종합알람(bit0~3중하나라도발생시1)
	CDIO			eDI_BRF_MAT_Fail				(_TEXT("eDI_BRF_MAT_Alarm"			)); // RFPT Gen 챕터5 byte28 bit2: Alarm 1=알람발생(종합)
	
	// KLP_EDIT
	CDIO			eDI_SRF_GenRemoteMode				(_TEXT("eDI_SRF_GenRemoteMode"		));
	CDIO			eDI_BRF_GenRemoteMode				(_TEXT("eDI_BRF_GenRemoteMode"		));


	CDIO			eDO_SRFG_CtrlMode				(_TEXT("eDO_SRFG_CtrlMode"			));
	CDIO			eDO_BRFG_CtrlMode				(_TEXT("eDO_BRFG_CtrlMode"			));

	CDIO			eDO_SRF_MAT_Remote				(_TEXT("eDO_SRF_MAT_Remote"			));
	CDIO			eDO_BRF_MAT_Remote				(_TEXT("eDO_BRF_MAT_Remote"			));

	CDIO			eDO_SRFG_ArcDetect				(_TEXT("eDO_SRFG_ArcDetect"			));
	CDIO			eDO_BRFG_ArcDetect				(_TEXT("eDO_BRFG_ArcDetect"			));
	
	// -----------------------------------------------------------------------------
	// RFPT Generator Byte5 Mode nibble 분리 제어 채널
	// - RegMode  : byte5 하위니블(bit0~3)
	// - RampMode : byte5 상위니블(bit4~7)
	// -----------------------------------------------------------------------------
	CDIO			eDO_SRFG_RegMode				(_TEXT("eDO_SRFG_RegMode"			));
	CDIO			eDO_SRFG_RampMode				(_TEXT("eDO_SRFG_RampMode"			));
	CDIO			eDO_BRFG_RegMode				(_TEXT("eDO_BRFG_RegMode"			));
	CDIO			eDO_BRFG_RampMode				(_TEXT("eDO_BRFG_RampMode"			));

	CDIO			cRF_eDO_SRFG_RegMode				(_TEXT("cRF_eDO_SRFG_RegMode"			));
	CDIO			cRF_eDO_SRFG_RampMode				(_TEXT("cRF_eDO_SRFG_RampMode"			));
	CDIO			cRF_eDO_BRFG_RegMode				(_TEXT("cRF_eDO_BRFG_RegMode"			));
	CDIO			cRF_eDO_BRFG_RampMode				(_TEXT("cRF_eDO_BRFG_RampMode"			));


	// -----------------------------------------------------------------------------
	// 기존 raw byte5 직접 write 채널
	// 현재 FNC_RF_CTRL에서는 더 이상 직접 쓰지 않고,
	// nibble 분리 제어 채널(eDO_*_RegMode / eDO_*_RampMode)을 사용한다.
	// 다른 legacy code와 호환을 위해 object 선언은 유지한다.
	// -----------------------------------------------------------------------------
	CAIO			eAO_SRF_Mode					(_TEXT("eAO_SRF_Mode"				));
	CAIO			eAO_BRF_Mode					(_TEXT("eAO_BRF_Mode"				));



	//D-NET
	CDIO			eDO_SRFG_RFPower				(_TEXT("eDO_SRFG_RFPower"			));
	CDIO			eDO_BRFG_RFPower				(_TEXT("eDO_BRFG_RFPower"			));
	CAIO			Rcp_CurStepNum		            (_TEXT("Rcp_CurStepNum"				));
	CDIO			cESC_Monitor_ME					(_TEXT("cESC_Monitor_ME"			));
                                                                                              
	CDIO			eDI_SRFG_CommSts				(_TEXT("eDI_SRFG_CommSts"			));
	CDIO			eDI_BRFG_CommSts				(_TEXT("eDI_BRFG_CommSts"			));
	CDIO			eDI_SRFM_CommSts				(_TEXT("eDI_SRFM_CommSts"			));
	CDIO			eDI_BRFM_CommSts				(_TEXT("eDI_BRFM_CommSts"			));

	CAIO			eAO_SRF_RampUpTime			    (_TEXT("eAO_SRF_RampUpTime"			));
	CAIO			eAO_BRF_RampUpTime				(_TEXT("eAO_BRF_RampUpTime"			));

	CAIO			cRF_SRF_RampUp_Time			    (_TEXT("cRF_SRF_RampUp_Time"		));
	CAIO			cRF_BRF_RampUp_Time				(_TEXT("cRF_BRF_RampUp_Time"		));		 

	CAIO			eAO_SRF_FwdPwr				    (_TEXT("eAO_SRF_FwdPwr"				));
	CAIO			eAO_BRF_FwdPwr					(_TEXT("eAO_BRF_FwdPwr"				));
	
	CAIO            cRF_BothDelay_TO				(_TEXT("cRF_BothDelay_TO"			));	
	CDIO			cRF_BothFirstRF				    (_TEXT("cRF_BothFirstRF"			));
	CDIO			cRF_PowerOnOptoin			    (_TEXT("cRF_PowerOnOptoin"			));

	CDIO            eDI_FRC_CommSts					(_TEXT("eDI_FRC_CommSts"			));
	CDIO            APC_CommSts						(_TEXT("APC_CommSts"				));
	CDIO            VVC_MOTOR_CommSts				(_TEXT("VVC_MOTOR_CommSts"			));

	CAIO		    PM_Pressure_mtorr		        (_TEXT("PM_Pressure_mtorr"			));
	CAIO		    eAPC_Position_RA				(_TEXT("APC_Position_RA"			));	
	CDIO            DO_PLC_ResetRun					(_TEXT("DO_PLC_ResetRun"			));

	CAIO 			Dechuck_SRF_Power				(_TEXT("Dechuck_SRF_Power"			));
	CAIO 			Dechuck_BRF_Power				(_TEXT("Dechuck_BRF_Power"			));

	//SCREEN LOCK
	CDIO            SRF_ONLINE_CHK					(_TEXT("SRF_ONLINE_CHK"				));
	CDIO            SRF_ERROR_CHK					(_TEXT("SRF_ERROR_CHK"				));
	CDIO            SRF_REMOTE_CHK					(_TEXT("SRF_REMOTE_CHK"				));
	CDIO            SRM_ONLINE_CHK					(_TEXT("SRM_ONLINE_CHK"				));
	CDIO            SRM_ERROR_CHK					(_TEXT("SRM_ERROR_CHK"				));
	
	CDIO            BRF_ONLINE_CHK					(_TEXT("BRF_ONLINE_CHK"				));
	CDIO            BRF_ERROR_CHK					(_TEXT("BRF_ERROR_CHK"				));
	CDIO            BRF_REMOTE_CHK					(_TEXT("BRF_REMOTE_CHK"				));
	CDIO            BRM_ONLINE_CHK					(_TEXT("BRM_ONLINE_CHK"				));
	CDIO            BRM_ERROR_CHK					(_TEXT("BRM_ERROR_CHK"				));

	CDIO            RF_ALL_WFS_CHK					(_TEXT("RF_ALL_WFS_CHK"				));
	CDIO            RF_ALL_WLS_CHK					(_TEXT("RF_ALL_WLS_CHK"				));
	CDIO            RF_PRC_GAS_VLV_CHK				(_TEXT("RF_PRC_GAS_VLV_CHK"			));
	CDIO            RF_SRF_EXTINTLK_CHK				(_TEXT("RF_SRF_EXTINTLK_CHK"		));
	CDIO            RF_BRF_EXTINTLK_CHK				(_TEXT("RF_BRF_EXTINTLK_CHK"		));
	CDIO            RF_ALL_COVER_CHK				(_TEXT("RF_ALL_COVER_CHK"			));

	CDIO            RF_ERROR_CHK					(_TEXT("RF_ERROR_CHK"				));
	CDIO            RF_PLCERR_CHK					(_TEXT("RF_PLCERR_CHK"				));
	CDIO            RF_RUN_GATECLOE_CHK				(_TEXT("RF_RUN_GATECLOE_CHK"		));
	CDIO            RF_APC_ONLINE_CHK				(_TEXT("RF_APC_ONLINE_CHK"			));
	CDIO            RF_FRC_ONLINE_CHK				(_TEXT("RF_FRC_ONLINE_CHK"			));

	CDIO            RF_VVC_MOTOR_ONLINE_CHK			(_TEXT("RF_VVC_MOTOR_ONLINE_CHK"	));
	CDIO            RF_PRESSURE_OVER_CHK			(_TEXT("RF_PRESSURE_OVER_CHK"		));
	CDIO            RF_PRESSURE_UNDER_CHK			(_TEXT("RF_PRESSURE_UNDER_CHK"		));
	CDIO            RF_APC_CONRTROL_CHK				(_TEXT("RF_APC_CONRTROL_CHK"		));

	CDIO            RF_INIT_RUN_NGOK				(_TEXT("RF_INIT_RUN_NGOK"			));
	CDIO            RF_POWERON_RUN_NGOK				(_TEXT("RF_POWERON_RUN_NGOK"		));

	CDIO			DO_PROCESS_GAS_SUPPLY_VALVE		(_TEXT("DO_PROCESS_GAS_SUPPLY_VALVE"));
	CDIO			rDO_PROCESS_GAS_SUPPLY_VALVE	(_TEXT("rDO_PROCESS_GAS_SUPPLY_VALVE"));
	CDIO			XI_TM_GateVlv_Sns				(_TEXT("XI_TM_GateVlv_Sns"			));
	CDIO			APC_RUN_NGOK					(_TEXT("APC_RUN_NGOK"				));

	//RF_POWER STEP SET

	CDIO			cRF_STEP_POWERON_OPTION			(_TEXT("cRF_STEP_POWERON_OPTION"	));
	CAIO		    cRF_STEP_POWERON_SETTIME		(_TEXT("cRF_STEP_POWERON_SETTIME"	));	
	CAIO		    cRF_STEP_POWERON_SETCOUNT		(_TEXT("cRF_STEP_POWERON_SETCOUNT"	));	

	CAIO		    cRF_STEP_POWERON_SRF_OVER		(_TEXT("cRF_STEP_POWERON_SRF_OVER"	));	
	CAIO		    cRF_STEP_POWERON_BRF_OVER		(_TEXT("cRF_STEP_POWERON_BRF_OVER"	));	
	CAlarm          SRF_6421						(_TEXT("SRF_6421"					));	//	Source RF Generator is offline
	CAlarm          SRF_6422						(_TEXT("SRF_6422"					));	//	Source RF Matcher is offline
	CAlarm          BRF_6471						(_TEXT("BRF_6471"					));	//	Bias RF Generator is offline
	CAlarm          BRF_6472						(_TEXT("BRF_6472"					));	//	Bias RF Matcher is offline
	
	CAlarm          VVC_MOTOR_6540				 	(_TEXT("VVC_MOTOR_6540"				));	//	VVC MOTOR Controller is Offline

	CAlarm          FRC_6700						(_TEXT("FRC_6700"					));	
	CAlarm          APC_7405						(_TEXT("APC_7405"					));	
	
	CAlarm          RF_OFF_6510						(_TEXT("RF_OFF_6510"				));	// 	PM Pressure is lower than 5.0 mtorr. RF Power Off Action Run
	CAlarm          RF_OFF_6511						(_TEXT("RF_OFF_6511"				));	//  DRP PUMP is Not Running. RF Power Off Action Run	
	CAlarm          RF_OFF_6512						(_TEXT("RF_OFF_6512"				));	//  APC Position is higher than 800. RF Power Off Action Run	
	CAlarm          RF_OFF_6513						(_TEXT("RF_OFF_6513"				));	//	Process gas Valve Sensor was not open. RF Power Off Action Run
	CAlarm          RF_OFF_6514						(_TEXT("RF_OFF_6514"				));	//  PC Gate is Not Open. RF Power Off Action Run	

								
END_OBJECT_ENUMERATION

//----------------------------------------------------------------------------------------------------------------------------------------------
void Make_Log(char *SeparateSting, LPSTR list, ...) 
{
	char		FOLDER[64]	 =  { "Log\\SEQ\\RF_ACTION\\RF_MAIN" };
	char		LOG[64]		 =  { "RF_ACTION" };

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
void RFOFFRUN_Log(char *SeparateSting, LPSTR list, ...) 
{
	char		FOLDER[64]	 =  { "Log\\SEQ\\RF_ACTION\\RF_MAIN" };
	char		LOG[64]		 =  { "RF_OFFRUN" };
	
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

// [SET_RF_MODE__SOURCE]: Source RF Generator의 Byte5 Mode를 nibble 단위로 설정한다.
// 매개변수:
// nRegMode  = Power Regulation Mode (0=Forward, 1=Load, 2=External, 3=VA Limit)
// nRampMode = Ramp Mode (0=Disable, 1=Watts/s, 2=Timed(ms))
// 반환값: 없음
void SET_RF_MODE__SOURCE(int nRegMode, int nRampMode)
{
	int nCS = 0;
	
	// -----------------------------------------------------------------
	// Source Generator Byte5 하위니블(RegMode) 먼저 기록한다.
	// Driver(DeviceNet.cpp)에서 기존 상위니블은 유지하면서 하위니블만 바꾼다.
	// -----------------------------------------------------------------
	eDO_SRFG_RegMode.Write(nRegMode, nCS);
	
	// -----------------------------------------------------------------
	// Source Generator Byte5 상위니블(RampMode) 기록한다.
	// Driver(DeviceNet.cpp)에서 기존 하위니블은 유지하면서 상위니블만 바꾼다.
	// -----------------------------------------------------------------
	eDO_SRFG_RampMode.Write(nRampMode, nCS);
	
	Make_Log("[RF_ACTION]", "SET_RF_MODE__SOURCE() Reg=%d Ramp=%d \n", nRegMode, nRampMode);
}


// [SET_RF_MODE__BIAS]: Bias RF Generator의 Byte5 Mode를 nibble 단위로 설정한다.
// 매개변수:
// nRegMode  = Power Regulation Mode (0=Forward, 1=Load, 2=External, 3=VA Limit)
// nRampMode = Ramp Mode (0=Disable, 1=Watts/s, 2=Timed(ms))
// 반환값: 없음
void SET_RF_MODE__BIAS(int nRegMode, int nRampMode)
{
	int nCS = 0;

	// -----------------------------------------------------------------
	// Bias Generator Byte5 하위니블(RegMode) 먼저 기록한다.
	// Driver(DeviceNet.cpp)에서 기존 상위니블은 유지하면서 하위니블만 바꾼다.
	// -----------------------------------------------------------------
	eDO_BRFG_RegMode.Write(nRegMode, nCS);
	
	// -----------------------------------------------------------------
	// Bias Generator Byte5 상위니블(RampMode) 기록한다.
	// Driver(DeviceNet.cpp)에서 기존 하위니블은 유지하면서 상위니블만 바꾼다.
	// -----------------------------------------------------------------
	eDO_BRFG_RampMode.Write(nRampMode, nCS);
	
	Make_Log("[RF_ACTION]", "SET_RF_MODE__BIAS() Reg=%d Ramp=%d \n", nRegMode, nRampMode);
}


// [SET_RF_MODE__BOTH]: Source/Bias Generator의 Byte5 Mode를 동일 조합으로 한 번에 설정한다.
// 매개변수:
// nRegMode  = Power Regulation Mode
// nRampMode = Ramp Mode
// 반환값: 없음
void SET_RF_MODE__BOTH(int nRegMode, int nRampMode)
{
	// -----------------------------------------------------------------
	// Source 쪽 Byte5 Mode를 먼저 설정한다.
	// -----------------------------------------------------------------
	SET_RF_MODE__SOURCE(nRegMode, nRampMode);
	
	// -----------------------------------------------------------------
	// Bias 쪽 Byte5 Mode를 동일하게 설정한다.
	// -----------------------------------------------------------------
	SET_RF_MODE__BIAS(nRegMode, nRampMode);
	
	Make_Log("[RF_ACTION]", "SET_RF_MODE__BOTH() Reg=%d Ramp=%d \n", nRegMode, nRampMode);
}





SEQ_STATUS RampUpSetTime()
{
	int nCS = 0;
	double SRF_RampUpTime, BRF_RampUpTime;

	Make_Log("[RF_ACTION]", " RampUpSetTime() START \n ");

	// -------------------------------------------------------------------
    // RFPT Gen 사양서 챕터5 byte5 Mode nibble 설정
    // 하위4bit = Regulation Mode 0 (Forward 레귤레이션 사용)
    // 상위4bit = Ramp Mode     2 (Timed ms 방식 사용)
    // → byte5 = (2 << 4) | 0 = 0x20
    // OnWriteAnalog ID2=5 → 1byte 직접 set → InOutMap.pWriteData[5]
    // -------------------------------------------------------------------
    eAO_SRF_Mode.Write(0x20, nCS);
    eAO_BRF_Mode.Write(0x20, nCS);


	if(PM_PROC_STS.Read(nCS) == ePM_PROC_RUNNING)
	{
		if(PR_BRFG_POWER.Read(nCS) ==0 )
		{
			if(   PR_SRFG_POWER.Read(nCS)  <  Dechuck_SRF_Power.Read(nCS) )
			{
				SRF_RampUpTime =0;
			}
			else
			{
				// -------------------------------------------------------------------
                // RFPT Gen 사양서 챕터5 byte6~7 / 사용자 추가1
                // Ramp Mode = Timed(ms) → cRF_SRF_RampUp_Time(ms) 그대로 전달
                // 기존 KYOSAN 수식 (Time/Power)*20000/1000 제거
                // -------------------------------------------------------------------
                SRF_RampUpTime = cRF_SRF_RampUp_Time.Read(nCS);				
				Make_Log("[RF_ACTION]", " RampUpSetTime(ONLY_SRF) SRF: %3.1f  \n", SRF_RampUpTime);
			}

			BRF_RampUpTime=0;			
		}
		else
		{
			// -------------------------------------------------------------------
            // RFPT Gen 사양서 챕터5 byte6~7, byte8~9 / 사용자 추가1
            // Ramp Up / Down Time ms 단위 직접 전달
            // 기존 KYOSAN 수식 (Time/Power)*20000/1000 제거
            // -------------------------------------------------------------------
            SRF_RampUpTime = cRF_SRF_RampUp_Time.Read(nCS);
            BRF_RampUpTime = cRF_BRF_RampUp_Time.Read(nCS);			
			Make_Log("[RF_ACTION]", " RampUpSetTime(PROCESSING) SRF: %3.1f  BRF: %3.1f \n", SRF_RampUpTime,BRF_RampUpTime);
			
		}


	}
	else
	{
		// -------------------------------------------------------------------
        // [변경] RFPT Gen 사양서 챕터5 byte6~7 / 사용자 추가1
        // MANUAL 모드 : Fnc_SRF_SetRampTime(ms) 그대로 전달
        // Power 0 체크 구조는 KYOSAN 흐름 유지 (분모 0 방어)
        // 기존 KYOSAN 수식 (Time/Power)*20000/1000 제거
        // -------------------------------------------------------------------
        if(Fnc_SRF_SetPower.Read(nCS) != 0)    SRF_RampUpTime = Fnc_SRF_SetRampTime.Read(nCS);
        else                                    SRF_RampUpTime = 0;
		
        if(Fnc_BRF_SetPower.Read(nCS) != 0)    BRF_RampUpTime = Fnc_BRF_SetRampTime.Read(nCS);
        else                                    BRF_RampUpTime = 0;

		Make_Log("[RF_ACTION]", " RampUpSetTime(MANUAL) SRF: %3.1f  BRF: %3.1f \n", SRF_RampUpTime,BRF_RampUpTime);

	}

	eAO_SRF_RampUpTime.Write(SRF_RampUpTime, nCS);
	eAO_BRF_RampUpTime.Write(BRF_RampUpTime, nCS);

	Make_Log("[RF_ACTION]", " RampUpSetTime() END \n ");


	return SEQ_SUCCESS;
}


int CHK_RF_INTLK()
{
	int		nCS					= 0;
	int		nRF_INTLK_STS		= 0;
	int     nShildCover1, nShildCover2,nShildCover3, nFeedCover;
	int     nRFGWFS, nRFMWFS, nRFGWLS, nRFMWLS;  

	int     nProcGas;
	int		nRFCover;
	//int     nExterIntlk;
	char    szLog[256]		    = {0};

	Make_Log("[RF_ACTION]", "CHK_SRF_INTLK() START \n ");


	if(DI_SOURCE_RFG_WATER_LEAK_OK.Read(nCS) == eOK && DI_BIAS_RFG_WATER_LEAK_OK.Read(nCS) == eOK)	nRFGWLS = eOK;
	else																						nRFGWLS = eNG;	

	sprintf(szLog, "1st Check[Generator WLS 1:OK] : %d -> SRFG Water Leak : %d  BRFG Leak Flow : %d ", nRFGWLS, DI_SOURCE_RFG_WATER_LEAK_OK.Read(nCS), DI_BIAS_RFG_WATER_LEAK_OK.Read(nCS));
	Make_Log("[RF_ACTION]", "%s \n",szLog);


	if(DI_SOURCE_RFG_WFS.Read(nCS) == eOK && DI_BIAS_RFG_WFS.Read(nCS) == eOK)					nRFGWFS = eOK;
	else																						nRFGWFS = eNG;	
	
	sprintf(szLog, "2nd Check[Generator WFS 1:OK] : %d -> SRFG Water Flow : %d  BRFG Water Flow : %d ", nRFGWFS, DI_SOURCE_RFG_WFS.Read(nCS), DI_BIAS_RFG_WFS.Read(nCS));
	Make_Log("[RF_ACTION]", "%s \n",szLog);


	if( DI_SOURCE_MATCHER_WLS.Read(nCS) == eOK && DI_BIAS_MATCHER_WLS.Read(nCS) == eOK)			nRFMWLS = eOK;
	else																						nRFMWLS = eNG;
	sprintf(szLog, "3rd Check[Matcher WLS 1:OK] : %d -> SRFM Water Leak : %d  BRFM Water Leak : %d ", nRFMWLS, DI_SOURCE_MATCHER_WLS.Read(nCS), DI_BIAS_MATCHER_WLS.Read(nCS));
	Make_Log("[RF_ACTION]", "%s \n",szLog);


	if(DI_SOURCE_RFM_WFS.Read(nCS) == eOK && DI_BIAS_RFM_WFS.Read(nCS) == eOK)					nRFMWFS = eOK;
	else																						nRFMWFS = eNG;
	sprintf(szLog, "4th Check[Matcher WFS 1:OK] : %d -> SRFM Water Flow : %d  BRFM Water Flow : %d ", nRFMWFS, DI_SOURCE_RFM_WFS.Read(nCS), DI_BIAS_RFM_WFS.Read(nCS));
	Make_Log("[RF_ACTION]", "%s \n",szLog);


	nFeedCover		= DI_RF_FEEDER_COVER_OPEN.Read(nCS);

	nShildCover1	= DI_RF_SHIELD_COVER1.Read(nCS);
	nShildCover2	= DI_RF_SHIELD_COVER2.Read(nCS);
	nShildCover3	= DI_RF_SHIELD_COVER3.Read(nCS);
	//nShildCover4	= DI_RF_SHIELD_COVER4.Read(nCS);
	

	if(	nFeedCover	 == eNO_Close && 
		nShildCover1 == eNO_Close && nShildCover2 == eNO_Close && 
		nShildCover3 == eNO_Close /*&& nShildCover4 == eNO_Close*/	)	nRFCover = eOK;
	else															nRFCover = eNG;


	sprintf(szLog, "5th Check[Cover 1:OK]  : %d -> FeedCover:%d ShldBox1:%d ShldBox2:%d ShldBox3:%d", nRFCover,nFeedCover, nShildCover1,nShildCover2,nShildCover3);
	Make_Log("[RF_ACTION]", "%s \n",szLog);

	nProcGas = DI_PROCESS_GAS_SUPPLY_VALVE.Read(nCS);
	sprintf(szLog, "6th Check[1:OK] ProcGas Valve Sensor : %d ", nProcGas);
	Make_Log("[RF_ACTION]", "%s \n",szLog);
	

	/*if(DI_SOURCE_RFG_INTERLOCK.Read(nCS) == eNotIntlk && DI_BIAS_RFG_INTERLOCK.Read(nCS) == eNotIntlk)  nExterIntlk = eOK;
	else																								nExterIntlk = eNG;

	sprintf(szLog, "7th Check : RF External Interlock : %d",nExterIntlk);
	Make_Log("[RF_ACTION]", "%s \n",szLog);*/	

	//if(nRFGWFS == eOK && nRFMWFS == eOK && nRFGWLS == eOK && nRFCover == eOK && nProcGas == eOK && nExterIntlk ==eOK)	
	
	//KLP_EDIT
	// nRFMWLS 도 interlock Check에 포함되도록 추가.
	// 이 항목도 횡전개 진행해야 할듯. 
	if(nRFGWFS == eOK && nRFMWFS == eOK && nRFGWLS == eOK && nRFCover == eOK && nProcGas == eOK && nRFMWLS )	
	{
		nRF_INTLK_STS = 1;
		sprintf(szLog, "All Check_OK : %d",nRF_INTLK_STS);
		Make_Log("[RF_ACTION]", "%s \n",szLog);
		
	}
	else 
	{	nRF_INTLK_STS = 0;
		sprintf(szLog, "All Check_NG: %d",nRF_INTLK_STS);
		Make_Log("[RF_ACTION]", "%s \n",szLog);
	
	}
	
	Make_Log("[RF_ACTION]", "CHK_HRF_INTLK() END \n ");

	return nRF_INTLK_STS;
}


SEQ_STATUS PROC_RF_OFF(int mode)
{
	int	nCS;
	int nProcStatus, nStepNo;
	int  nRF_PROC_TYPE	= 0;
	char szTemp[80]			= {0};

	Make_Log("[RF_ACTION]", "PROC_RF_OFF() START \n ");

	if(mode == INTERLOCKED_OFF)
	{
		Make_Log("[RF_ACTION]", "Mode is INTERLOCKED_OFF. PM_PROC_RFCTRL and RF_Cmd Set OFF. \n ");
		PM_PROC_RFCTRL.Write(eRF_OFF1 , nCS);
	
		DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
		DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);
		eDO_SRFG_RFPower.Write(OFF , nCS);
		eDO_BRFG_RFPower.Write(OFF , nCS);
		
		eAO_SRF_FwdPwr.Write(0 , nCS);
		eAO_BRF_FwdPwr.Write(0 , nCS);
		
		PM_StartTimer.Write(TIMER_IDLE , nCS);
		
		Make_Log("[RF_ACTION]", "PROC_RF_OFF() END \n ");

		return SEQ_SUCCESS;

	} 
	else 
	{
		nProcStatus = PM_PROC_STS.Read(nCS);
		nStepNo     = (int)(Rcp_CurStepNum.Read(nCS));

		sprintf(szTemp, "Step [ %d ], PM_PROC_STS [ %d ], PM_PROC_RFCTRL [ %d ], Source RF/Bias RF is off", nStepNo, nProcStatus, eRF_OFF1);
		Make_Log("[RF_ACTION]", "%s  \n",szTemp);

		PM_PROC_RFCTRL.Write(eRF_OFF1 , nCS);
		PM_ReleaseESC.Write(ESC_OFF , nCS);

		if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
		{
			if(cRF_PowerOnOptoin.Equal(eInterface)) DO_BIAS_RFG_POWER_ON.Write(OFF	 , nCS);
			else									eDO_BRFG_RFPower.Write(OFF		 , nCS);
		
			_sleep(200);
		}
		

		eDO_SRFG_RFPower.Write(OFF , nCS);
		eDO_BRFG_RFPower.Write(OFF , nCS);	
		DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
		DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);

		eAO_SRF_FwdPwr.Write(0 , nCS);
		eAO_BRF_FwdPwr.Write(0 , nCS);


		PM_StartTimer.Write(TIMER_IDLE , nCS);
	}


	Make_Log("[RF_ACTION]", "PROC_RF_OFF() END \n ");

	return SEQ_SUCCESS;
}


SEQ_STATUS PROC_RF_DUAL()
{

	int	nCS;
	int nCount				= 0;
	SEQ_STATUS nResult;

	int nProcStatus, nStepNo;
	int nRF_PROC_TYPE		= 0;

	int nConditionRF		= 0;
	int nRFPowerOnStepCount = 0;
	
	double nRFPowerOnCurrentRange;
	double nRFPowerOnStepdelayTime=0;
	char szTemp[80]			= {0};
	double nSourceRFPower,nBiasRFPower;

	Make_Log("[RF_ACTION]", "PROC_RF_DUAL() START \n ");

	nSourceRFPower	= PR_SRFG_POWER.Read(nCS);
	nBiasRFPower	= PR_BRFG_POWER.Read(nCS);	
	nProcStatus		= PM_PROC_STS.Read(nCS);
	nStepNo     = (int)(Rcp_CurStepNum.Read(nCS));

	nRFPowerOnStepdelayTime = cRF_STEP_POWERON_SETTIME.Read(nCS);

	if(nProcStatus == ePM_PROC_RUNNING)
	{

		Make_Log("[RF_ACTION]", "RF HW INTERLOCK CHECK \n ");

		nConditionRF = CHK_RF_INTLK();
		nRF_PROC_TYPE = PM_PROC_RFCTRL.Read(nCS);

		if(eRF_BOTH == nRF_PROC_TYPE)
		{
			if(nConditionRF ==eOK)
			{	
				//KLP_EDIT
				// Remote ON
				Make_Log("[RF_ACTION]", "RF Generator Source/Bias Remote ON Start \n ");
				eDO_SRFG_CtrlMode.Write(REMOTE_ON_1, nCS);
				eDO_BRFG_CtrlMode.Write(REMOTE_ON_1, nCS);
				Make_Log("[RF_ACTION]", "RF Generator Source/Bias Remote ON END \n ");

				Make_Log("[RF_ACTION]", "RF Matcher Source/Bias Remote ON Start \n ");
				eDO_SRF_MAT_Remote.Write(REMOTE_ON_1, nCS);
				eDO_BRF_MAT_Remote.Write(REMOTE_ON_1, nCS);
				Make_Log("[RF_ACTION]", "RF Matcher Source/Bias Remote ON END \n ");




				Make_Log("[RF_ACTION]", "RF HW INTERLOCK CHECK OK \n ");

				eAO_SRF_RampUpTime.Write(0, nCS);
				eAO_BRF_RampUpTime.Write(0, nCS);

				if(	cRF_STEP_POWERON_OPTION.Equal(ON) && 
					cRF_STEP_POWERON_SRF_OVER.Read(nCS) < nSourceRFPower &&  
					cRF_STEP_POWERON_BRF_OVER.Read(nCS) < nBiasRFPower && 
					cRF_STEP_POWERON_SETCOUNT.Read(nCS) >= 2)
				{
					// -------------------------------------------------------------------
					// RFPT Gen 사양서 챕터5 byte5 Mode nibble 설정
					// Step Power ON 분기는 RampUpSetTime()을 호출하지 않으므로 여기서 별도 설정
					// Regulation Mode 0 (Forward) + Ramp Mode 2 (Timed ms) = 0x20
					// RampUp Time은 0ms (즉시) → Step Power ON 특성상 Ramp 미사용
					// -------------------------------------------------------------------

					eAO_SRF_Mode.Write(0x20, nCS);
					eAO_BRF_Mode.Write(0x20, nCS);

					Make_Log("[RF_ACTION]", "RF STEP_UP POWER ON START \n");
					
					eAO_SRF_FwdPwr.Write(0, nCS);
					eAO_BRF_FwdPwr.Write(0,nCS);

					if(cRF_PowerOnOptoin.Equal(eInterface)) 
					{
						eDO_SRFG_ArcDetect.Write(ON, nCS);		// Source Generator Arc Detect Enable
						eDO_BRFG_ArcDetect.Write(ON, nCS);		// Bias Generator Arc Detect Enable

						DO_SOURCE_RFG_POWER_ON.Write(ON	 , nCS);
						DO_BIAS_RFG_POWER_ON.Write(ON	 , nCS);
					}
					else
					{
						eDO_SRFG_ArcDetect.Write(ON, nCS);		// Source Generator Arc Detect Enable
						eDO_BRFG_ArcDetect.Write(ON, nCS);		// Bias Generator Arc Detect Enable

						eDO_SRFG_RFPower.Write(ON		 , nCS);
						eDO_BRFG_RFPower.Write(ON		 , nCS);
					}
												
					_sleep(nRFPowerOnStepdelayTime);

					for( nRFPowerOnStepCount=1; nRFPowerOnStepCount <=cRF_STEP_POWERON_SETCOUNT.Read(nCS); nRFPowerOnStepCount++)
					{

						nRFPowerOnCurrentRange = nRFPowerOnStepCount/cRF_STEP_POWERON_SETCOUNT.Read(nCS);

						eAO_SRF_FwdPwr.Write(nSourceRFPower*nRFPowerOnCurrentRange, nCS);
						eAO_BRF_FwdPwr.Write(nBiasRFPower*nRFPowerOnCurrentRange, nCS);

						Make_Log("[RF_ACTION]", "RF STEP POWER ON RUNNING [%d Count][%0.1f] -> SRF: %.f BRF: %.f  \n",
													nRFPowerOnStepCount, nRFPowerOnCurrentRange,
													eAO_SRF_FwdPwr.Read(nCS),eAO_BRF_FwdPwr.Read(nCS));

						_sleep(nRFPowerOnStepdelayTime);
					}

					Make_Log("[RF_ACTION]", "RF STEP_UP POWER ON END \n");

					
					eAO_SRF_FwdPwr.Write(nSourceRFPower);
					eAO_BRF_FwdPwr.Write(nBiasRFPower);

					Make_Log("[RF_ACTION]", "RF FULL POWER ON END \n");


				}
				else
				{

					RampUpSetTime();

					if(cRF_BothFirstRF.Read(nCS) == BIAS_FIRST)
					{
						Make_Log("[RF_ACTION]", "BIAS POWER ON START [cRF_BothFirstRF =BIAS_FIRST] \n");
					
		
						_sleep(cRF_BothDelay_TO.Read(nCS));	
					
						Make_Log("[RF_ACTION]", "SOURCE POWER ON START [cRF_BothFirstRF =BIAS_FIRST] \n");

						if(cRF_PowerOnOptoin.Equal(eInterface)) DO_SOURCE_RFG_POWER_ON.Write(ON	 , nCS);
						else									eDO_SRFG_RFPower.Write(ON		 , nCS);
					}
					else
					{	
						Make_Log("[RF_ACTION]", "SOURCE POWER ON START [cRF_BothFirstRF =SOURCE_FIRST] \n");
					
						if(cRF_PowerOnOptoin.Equal(eInterface)) DO_SOURCE_RFG_POWER_ON.Write(ON	 , nCS);
						else									eDO_SRFG_RFPower.Write(ON		 , nCS);
					
						_sleep(cRF_BothDelay_TO.Read(nCS));	
					
						Make_Log("[RF_ACTION]", "BIAS POWER ON START [cRF_BothFirstRF =SOURCE_FIRST] \n");
						if(cRF_PowerOnOptoin.Equal(eInterface)) DO_BIAS_RFG_POWER_ON.Write(ON	 , nCS);
						else									eDO_BRFG_RFPower.Write(ON		 , nCS);
					}
					
				}

				PM_StartTimer.Write(TIMER_RUN , nCS);

				if(cESC_Monitor_ME.Read(nCS) == 0)	cESC_Monitor_ME.Write(1 , nCS);
				
				sprintf(szTemp, "Step [ %d ], Source RF/Bias RF is on", nStepNo);
				Make_Log("[RF_ACTION]", "%s \n ",szTemp);

				nResult = SEQ_SUCCESS;
			}
			else
 			{
				Make_Log("[RF_ACTION]", "RF HW INTERLOCK CHECK NG \n ");
 				
				sprintf(szTemp, "Step [ %d ], Dual RF can not turn on. nConditionHRF = 0 or nConditionLRF = 0", nStepNo);
				Make_Log("[RF_ACTION]", "%s \n ",szTemp);
 				Make_Log("[RF_ACTION]", "Call PROC_RF_OFF (INTERLOCKED_OFF) \n");
 				PROC_RF_OFF(INTERLOCKED_OFF);
 
 				nResult = SEQ_ABORT;
 			}
		}
		else
		{
			Make_Log("[RF_ACTION]", "Invalid mode : PM_PROC_RFCTRL is not eRF_BOTH \n");
			Make_Log("[RF_ACTION]", "Call PROC_RF_OFF (INTERLOCKED_OFF) \n");
	
			PROC_RF_OFF(INTERLOCKED_OFF);

			nResult = SEQ_ABORT;
		}
	}
	else
	{
		Make_Log("[RF_ACTION]", "Invalid mode : PM_PROC_STS is not ePM_PROC_RUNNING \n");
		Make_Log("[RF_ACTION]", "Call PROC_RF_OFF (INTERLOCKED_OFF) \n");

		PROC_RF_OFF(INTERLOCKED_OFF);
		nResult = SEQ_ABORT;
	}

	if (nResult == SEQ_SUCCESS ) Make_Log("[RF_ACTION]", "PROC_RF_DUAL() END \n");
	else						 Make_Log("[RF_ACTION]", "PROC_RF_DUAL() ABORT \n");

	return nResult;
}


SEQ_STATUS PROC_RF_HIGH()
{
	
	int	nCS;
	int nCount				= 0;
	int nProcStatus, nStepNo;
	int nRF_PROC_TYPE		= 0;
	int nConditionRF		= 0;

	SEQ_STATUS nResult;	

	char szTemp[80]			= {0};

	Make_Log("[RF_ACTION]", "PROC_RF_HIGH() START \n ");
	RampUpSetTime();

	nProcStatus = PM_PROC_STS.Read(nCS);
	nStepNo     = (int)(Rcp_CurStepNum.Read(nCS));

	if(nProcStatus == ePM_PROC_RUNNING)
	{
		Make_Log("[RF_ACTION]", "SOURCE RF HW INTERLOCK CHECK \n ");

		nConditionRF = CHK_RF_INTLK();
		nRF_PROC_TYPE = PM_PROC_RFCTRL.Read(nCS);
	
		if(eRF_HIGH == nRF_PROC_TYPE)
		{
			if(nConditionRF == 1)
			{
				//KLP_EDIT
				// Remote ON
				Make_Log("[RF_ACTION]", "RF Generator Source/Bias Remote ON Start \n ");
				eDO_SRFG_CtrlMode.Write(REMOTE_ON_1, nCS);
				eDO_BRFG_CtrlMode.Write(REMOTE_ON_1, nCS);
				Make_Log("[RF_ACTION]", "RF Generator Source/Bias Remote ON END \n ");

				Make_Log("[RF_ACTION]", "RF Matcher Source/Bias Remote ON Start \n ");
				eDO_SRF_MAT_Remote.Write(REMOTE_ON_1, nCS);
				eDO_BRF_MAT_Remote.Write(REMOTE_ON_1, nCS);
				Make_Log("[RF_ACTION]", "RF Matcher Source/Bias Remote ON END \n ");


				if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
				{
					eDO_BRFG_RFPower.Write(OFF , nCS);
					DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);
				
					_sleep(200);
				}
			
				if(cRF_PowerOnOptoin.Equal(eInterface)) 
				{
					eDO_SRFG_ArcDetect.Write(ON, nCS);		// Source Generator Arc Detect Enable
					DO_SOURCE_RFG_POWER_ON.Write(ON	 , nCS);
				}
				else									
				{
					eDO_SRFG_ArcDetect.Write(ON, nCS);		// Source Generator Arc Detect Enable
					eDO_SRFG_RFPower.Write(ON		 , nCS);
				}
	
				PM_StartTimer.Write(TIMER_RUN , nCS);

				sprintf(szTemp, "Step [ %d ], Source RF is on", nStepNo);
				Make_Log("[RF_ACTION]", "%s \n ",szTemp);

				nResult = SEQ_SUCCESS;
			}
			else
			{
				Make_Log("[RF_ACTION]", "SOURCE RF HW INTERLOCK CHECK NG \n ");

				sprintf(szTemp, "Step [ %d ], Source RF can not turn on. nConditionHRF = 0", nStepNo);
				Make_Log("[RF_ACTION]", "%s \n ",szTemp);
				Make_Log("[RF_ACTION]", "Call PROC_RF_OFF (INTERLOCKED_OFF) \n");
				PROC_RF_OFF(INTERLOCKED_OFF);
 
				nResult = SEQ_ABORT;
			}
		}
		else
		{
			Make_Log("[RF_ACTION]", "Invalid mode : PM_PROC_RFCTRL is not eRF_BOTH \n");
			Make_Log("[RF_ACTION]", "Call PROC_RF_OFF (INTERLOCKED_OFF) \n");
			
			PROC_RF_OFF(INTERLOCKED_OFF);
			nResult = SEQ_ABORT;
		}
	}
	else
	{
		Make_Log("[RF_ACTION]", "Invalid mode : PM_PROC_STS is not ePM_PROC_RUNNING \n");
		Make_Log("[RF_ACTION]", "Call PROC_RF_OFF (INTERLOCKED_OFF) \n");
		
		PROC_RF_OFF(INTERLOCKED_OFF);
		nResult = SEQ_ABORT;
	}

	if (nResult == SEQ_SUCCESS ) Make_Log("[RF_ACTION]", "PROC_RF_HIGH() END \n");
	else						 Make_Log("[RF_ACTION]", "PROC_RF_HIGH() ABORT \n");

	return nResult;
}


SEQ_STATUS MNT_RF_ON()
{
	int nCS			= 0;
	int SResult		= 0;
	int BResult		= 0;
	int SRF_Mode	= 0;
	int BRF_Mode	= 0;
	int nCfgTime	= 50;

	double SetPower = 0.0;
	double SetMatch	= 0.0;
	double SetTune	= 0.0;

	SEQ_STATUS nResult = SEQ_SUCCESS;
	char szLog[256] = {0};

	Make_Log("[RF_ACTION]", "MNT_RF_ON() START \n ");
	RampUpSetTime();

	SetPower	= Fnc_SRF_SetPower.Read(nCS);

	if(	SetPower > 100.0)
	{
		SRF_Mode = RF_ON;
		PR_SRFG_POWER.Write(SetPower , nCS);
		sprintf(szLog, "Source RF is on [ %.0f ]", SetPower);
		Make_Log("[RF_ACTION]", "%s \n ",szLog);
	}
	else
	{
		SRF_Mode = RF_OFF;
		PR_SRFG_POWER.Write(0.0 , nCS);
		sprintf(szLog, "Source RF is off [ %.0f ]", SetPower);
		Make_Log("[RF_ACTION]", "%s \n ",szLog);
	}


	SetPower	= Fnc_BRF_SetPower.Read(nCS);

	if(	SetPower > 100.0)
	{
		BRF_Mode = RF_ON;
		PR_BRFG_POWER.Write(SetPower , nCS);
		sprintf(szLog, "Bias RF is on [ %.0f ]", SetPower);
		Make_Log("[RF_ACTION]", "%s \n ",szLog);
	}
	else
	{
		BRF_Mode = RF_OFF;	
		PR_BRFG_POWER.Write(0.0 , nCS);
		sprintf(szLog, "Bias RF is off [ %.0f ]", SetPower);
		Make_Log("[RF_ACTION]", "%s \n ",szLog);
	}


	if(SRF_Mode == RF_OFF && BRF_Mode == RF_OFF)
	{
		if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
		{
			eDO_BRFG_RFPower.Write(OFF , nCS);	
			DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);	_sleep(200);		
		}

		Make_Log("[RF_ACTION]", "Source, Bias RF is Off \n");

		eDO_BRFG_RFPower.Write(OFF , nCS);
		eDO_SRFG_RFPower.Write(OFF , nCS);	
		DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);
		DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);	

		eAO_SRF_FwdPwr.Write(0 , nCS);
		eAO_BRF_FwdPwr.Write(0 , nCS);

		
		while(TRUE) 
		{
			if( FNC_SRF.Status() != SEQ_RUNNING && FNC_BRF.Status() != SEQ_RUNNING )	break;
			if(WaitAbort(10))	return SEQ_ABORT;
		}

		FNC_SRF.RunTh("POWEROFF");
		FNC_BRF.RunTh("POWEROFF");

		PM_StartTimer.Write(TIMER_IDLE , nCS);

		Make_Log("[RF_ACTION]", "Invalid mode : Function is RF-on, but Set RF Power is all less than 100.0 \n");
		Make_Log("[RF_ACTION]", "MNT_RF_ON() ABORT \n ");
		
		return SEQ_ABORT;
	}
	else if(SRF_Mode == RF_OFF && BRF_Mode == RF_ON)
	{
		if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
		{
			eDO_BRFG_RFPower.Write(OFF , nCS);		
			DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);	_sleep(200);
		}
		
		Make_Log("[RF_ACTION]", "Source, Bias RF is Off \n");
	
		eDO_BRFG_RFPower.Write(OFF , nCS);
		eDO_SRFG_RFPower.Write(OFF , nCS);	
		DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);
		DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
		
		
		while(TRUE) 
		{
			if( FNC_SRF.Status() != SEQ_RUNNING && FNC_BRF.Status() != SEQ_RUNNING )	break;
			if(WaitAbort(10))	return SEQ_ABORT;
		}
		
		FNC_SRF.RunTh("POWEROFF");
		FNC_BRF.RunTh("POWEROFF");

		PM_StartTimer.Write(TIMER_IDLE , nCS);

		Make_Log("[RF_ACTION]", "Invalid mode : Source RF is off, Bias RF is off \n");
		Make_Log("[RF_ACTION]", "MNT_RF_ON() ABORT \n ");
	
		return SEQ_ABORT;
	}


	if(SRF_Mode == RF_ON)		
	{
		Make_Log("[RF_ACTION]", "Run FNC_SRF | POWERON \n ");
		
		while(TRUE) 
		{
			if( FNC_SRF.Status() != SEQ_RUNNING )	break;
			if(WaitAbort(10))						return SEQ_ABORT;
		}

		FNC_SRF.RunTh("POWERON");
	}


	if(BRF_Mode == RF_ON)
	{
		Make_Log("[RF_ACTION]", "Run FNC_BRF | POWERON \n ");

		while(TRUE) 
		{
			if( FNC_BRF.Status() != SEQ_RUNNING )	break;
			if(WaitAbort(10))						return SEQ_ABORT;
		}

		FNC_BRF.RunTh("POWERON");
	}


	nCfgTime = 50;
	while(nCfgTime > 0)
	{
		if(WaitAbort(100))
		{
			Make_Log("[RF_ACTION]", "MNT_RF_ON() MANUAL ABORT. END \n ");
			return SEQ_ABORT;
		}

		SResult = FNC_SRF.Status();
		BResult = FNC_BRF.Status();

		if(SResult == SEQ_SUCCESS && BResult == SEQ_SUCCESS)
		{
			if(SRF_Mode == RF_ON && BRF_Mode == RF_ON)
			{
				if(cRF_PowerOnOptoin.Equal(eInterface)) 
				{
					eDO_SRFG_ArcDetect.Write(ON, nCS);		// Source Generator Arc Detect Enable
					eDO_BRFG_ArcDetect.Write(ON, nCS);		// Bias Generator Arc Detect Enable

					DO_SOURCE_RFG_POWER_ON.Write(ON	 , nCS); 	WaitAbort(500);	
					DO_BIAS_RFG_POWER_ON.Write(ON	 , nCS);
				}
				else			
				{
					eDO_SRFG_ArcDetect.Write(ON, nCS);		// Source Generator Arc Detect Enable
					eDO_BRFG_ArcDetect.Write(ON, nCS);		// Bias Generator Arc Detect Enable

					eDO_SRFG_RFPower.Write(ON , nCS);	WaitAbort(500);	
					eDO_BRFG_RFPower.Write(ON , nCS);
				}


				PM_StartTimer.Write(TIMER_RUN , nCS);
				Make_Log("[RF_ACTION]", "Source RF/Bias RF is Power-On \n ");
			}
			else if(SRF_Mode == RF_ON && BRF_Mode == RF_OFF)
			{
				if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
				{
					eDO_BRFG_RFPower.Write( OFF , nCS);	
					DO_BIAS_RFG_POWER_ON.Write(OFF	, nCS); WaitAbort(200);
				}

				eDO_BRFG_RFPower.Write(OFF , nCS); DO_BIAS_RFG_POWER_ON.Write(OFF , nCS); eAO_BRF_FwdPwr.Write(OFF , nCS);

				if	(cRF_PowerOnOptoin.Equal(eInterface))  DO_SOURCE_RFG_POWER_ON.Write(ON, nCS);
				else									   eDO_SRFG_RFPower.Write(ON , nCS);

				PM_StartTimer.Write(TIMER_RUN , nCS);
				Make_Log("[RF_ACTION]", "Source RF is Power-On, Bias RF is Power-Off \n ");
			}
			else
			{
				if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
				{
					eDO_BRFG_RFPower.Write( OFF , nCS);	
					DO_BIAS_RFG_POWER_ON.Write(OFF	, nCS); WaitAbort(200);
				}

				eDO_SRFG_RFPower.Write(OFF , nCS);
				eDO_BRFG_RFPower.Write(OFF , nCS);
				DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
				DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);

				eAO_SRF_FwdPwr.Write(0 , nCS);
				eAO_BRF_FwdPwr.Write(0 , nCS);

				PM_StartTimer.Write(TIMER_IDLE , nCS);
				Make_Log("[RF_ACTION]", "Source RF/Bias RF is Power-off \n ");
			}

			Make_Log("[RF_ACTION]", "MNT_RF_ON() END \n ");
			return SEQ_SUCCESS;
		}
		else if(SResult == SEQ_ERROR || BResult == SEQ_ABORT)
		{
			if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
			{
				eDO_BRFG_RFPower.Write( OFF , nCS);	
				DO_BIAS_RFG_POWER_ON.Write(OFF	, nCS); WaitAbort(200);
			}
			
			eDO_SRFG_RFPower.Write(OFF , nCS);
			eDO_BRFG_RFPower.Write(OFF , nCS);
			DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
			DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);
		
			eAO_SRF_FwdPwr.Write(0 , nCS);
			eAO_BRF_FwdPwr.Write(0 , nCS);
			
			PM_StartTimer.Write(TIMER_IDLE , nCS);

			Make_Log("[RF_ACTION]", "FNC_SRF is sys_error or sys_aborted\n ");
			Make_Log("[RF_ACTION]", "MNT_RF_ON() ABORT_END \n ");
			
			return SEQ_ABORT;
		}
		//KLP_EDIT
		// 위에꺼랑 동일한 조건이여서 의미없음. 횡전개 해야하는 항목일듯. 
		else if(SResult == SEQ_ABORT || BResult == SEQ_ERROR)
		{
			if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
			{
				eDO_BRFG_RFPower.Write( OFF , nCS);	
				DO_BIAS_RFG_POWER_ON.Write(OFF	, nCS); WaitAbort(200);
			}
			
			eDO_SRFG_RFPower.Write(OFF , nCS);
			eDO_BRFG_RFPower.Write(OFF , nCS);
			DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
			DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);

			PM_StartTimer.Write(TIMER_IDLE , nCS);

			Make_Log("[RF_ACTION]", "FNC_BRF is sys_error or sys_aborted\n ");
			Make_Log("[RF_ACTION]", "MNT_RF_ON() ABORT_END \n ");

			return SEQ_ABORT;
		}

		nCfgTime--;
		_sleep(100);
	}


	Make_Log("[RF_ACTION]", "RF_ON timeout error\n ");
	Make_Log("[RF_ACTION]", "FNC_SRF and FNC_BRF Set Abort() \n ");
	
	FNC_SRF.Abort();
	FNC_BRF.Abort();

	eDO_SRFG_RFPower.Write(OFF , nCS);
	eDO_BRFG_RFPower.Write(OFF , nCS);
	DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
	DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);

	eAO_SRF_FwdPwr.Write(0 , nCS);
	eAO_BRF_FwdPwr.Write(0 , nCS);

	
	PM_StartTimer.Write(TIMER_IDLE , nCS);


	return nResult;
}



SEQ_STATUS MNT_RF_OFF()
{
	int nCS = 0;

	Make_Log("[RF_ACTION]", "MNT_RF_OFF() START \n ");

	if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
	{
		eDO_BRFG_RFPower.Write( OFF , nCS);	
		DO_BIAS_RFG_POWER_ON.Write(OFF	, nCS); WaitAbort(200);
	}

	Make_Log("[RF_ACTION]", "Source, Bias RF is Off \n");

	eDO_SRFG_RFPower.Write(OFF , nCS);
	eDO_BRFG_RFPower.Write(OFF , nCS);
	DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
	DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);

	eAO_SRF_FwdPwr.Write(0 , nCS);
	eAO_BRF_FwdPwr.Write(0 , nCS);

	PM_StartTimer.Write(TIMER_IDLE , nCS);

	while(TRUE) 
	{
		if(FNC_SRF.Status() != SEQ_RUNNING && FNC_BRF.Status() != SEQ_RUNNING  )	break;
		if(WaitAbort(10))	return SEQ_ABORT;
	}

	Make_Log("[RF_ACTION]", "FNC_SRF and FNC_BRF POWEROFF() is Run \n ");
	FNC_SRF.RunTh("POWEROFF");
	FNC_BRF.RunTh("POWEROFF");
	
	Make_Log("[RF_ACTION]", "MNT_RF_OFF() END \n ");

	return SEQ_SUCCESS;
}



SEQ_STATUS MNT_RF_ABORT()
{
	int nCS = 0;

	Make_Log("[RF_ACTION]", "MNT_RF_ABORT() START \n ");

	if(DI_BIAS_RFG_POWER_ON.Read(nCS) == ON)
	{
		eDO_BRFG_RFPower.Write( OFF , nCS);	
		DO_BIAS_RFG_POWER_ON.Write(OFF	, nCS); WaitAbort(200);
	}

	Make_Log("[RF_ACTION]", "Source, Bias RF is Off \n");

	eDO_SRFG_RFPower.Write(OFF , nCS);
	eDO_BRFG_RFPower.Write(OFF , nCS);
	DO_SOURCE_RFG_POWER_ON.Write(OFF , nCS);
	DO_BIAS_RFG_POWER_ON.Write(OFF , nCS);
	
	eAO_SRF_FwdPwr.Write(0 , nCS);
	eAO_BRF_FwdPwr.Write(0 , nCS);

	Make_Log("[RF_ACTION]", "FNC_SRF and FNC_BRF Set Abort() \n ");

	FNC_SRF.Abort();
	FNC_BRF.Abort();

	Make_Log("[RF_ACTION]", "MNT_RF_ABORT() END \n ");

	return SEQ_SUCCESS;
}


SEQ_STATUS MNT_RF_INIT()
{
	int nCfgTime = 600;
	int SResult;
	int BResult;

	SEQ_STATUS nResult = SEQ_SUCCESS;
	char szBuf[256] = {0};

	Make_Log("[RF_ACTION]", "MNT_RF_INIT() START \n ");


	while(TRUE) 
	{
		if( FNC_SRF.Status() != SEQ_RUNNING && FNC_BRF.Status() != SEQ_RUNNING  )	break;
		if(WaitAbort(10))	return SEQ_ABORT;
	}

	Make_Log("[RF_ACTION]", "Run FNC_SRF|INIT \n ");	FNC_SRF.RunTh("INIT");
	Make_Log("[RF_ACTION]", "Run FNC_BRF|INIT \n ");	FNC_BRF.RunTh("INIT");

	nCfgTime = 600;
	while(nCfgTime)
	{
		if(WaitAbort(100))
		{
			Make_Log("[RF_ACTION]", "MNT_RF_INIT() MANUAL ABORT. END \n ");
			nResult = SEQ_ABORT;
			break;
		}

		SResult = FNC_SRF.Status();
		BResult = FNC_BRF.Status();
		
		if(SResult == SEQ_SUCCESS && BResult == SEQ_SUCCESS)
		{
			Make_Log("[RF_ACTION]", "FNC_SRF and FNC_BRF is SEQ_SUCCESS \n ");
			Make_Log("[RF_ACTION]", "MNT_RF_INIT() END \n ");
			return SEQ_SUCCESS;
		}

		if(SResult == SEQ_ERROR || BResult == SEQ_ABORT)
		{
			Make_Log("[RF_ACTION]", "FNC_SRF is SEQ_ERROR or SEQ_ABORT \n ");
			nResult = SEQ_ABORT;
			break;
		}
	
		if(SResult == SEQ_ERROR || BResult == SEQ_ABORT)
		{
			Make_Log("[RF_ACTION]", "FNC_BRF is SEQ_ERROR or SEQ_ABORT \n ");
			nResult = SEQ_ABORT;
			break;
		}

		nCfgTime--;
		_sleep(100);
	}


	Make_Log("[RF_ACTION]", "MNT_RF_INIT timeout error\n ");
	Make_Log("[RF_ACTION]", "FNC_SRF and FNC_BRF Set Abort() \n ");

	FNC_SRF.Abort();
	FNC_BRF.Abort();

	Make_Log("[RF_ACTION]", "MNT_RF_INIT() ABORT END \n ");

	return nResult;
}

SEQ_STATUS MNT_RF_PRESET()
{
	int nCS = 0;
	int SResult = 0;
	int BResult = 0;
	int VVCMoterResult=0;
	
	double SetMatch = 0.0;
	double SetTune  = 0.0;

	int nCfgTime = 100;
	SEQ_STATUS nResult = SEQ_SUCCESS;

	int sRF_Mode = 0;
	int bRF_Mode = 0;
	char szBuf[256] = {0};



	Make_Log("[RF_ACTION]", "MNT_RF_PRESET() START \n ");
	
	SetMatch	= Fnc_SRFM_SetMatch.Read(nCS);
	SetTune		= Fnc_SRFM_SetTune.Read(nCS);

	RampUpSetTime();
	Make_Log("[RF_ACTION]", "SOURCE RF PRESET START \n ");


	if(SetMatch > 0.0 || SetTune > 0.0)
	{
		PR_SRFM_MATCH.Write(SetMatch , nCS);
		PR_SRFM_TUNE.Write(SetTune , nCS);
		
		while(TRUE) 
		{
			if( FNC_SRF.Status() != SEQ_RUNNING  )	break;
			if(WaitAbort(10))	return SEQ_ABORT;
		}

		FNC_SRF.RunTh("PRESET");
		sRF_Mode = 1;

		sprintf(szBuf, "Source Matcher Match/Tune = %.0f/%.0f, RUN FNC_SRF|PRESET", SetMatch, SetTune);
		Make_Log("[RF_ACTION]", "%s \n",szBuf);
		
	}

	
	SetMatch	= Fnc_BRFM_SetMatch.Read(nCS);
	SetTune		= Fnc_BRFM_SetTune.Read(nCS);
	Make_Log("[RF_ACTION]", "BIAS RF PRESET START \n ");
	
	if(SetMatch > 0.0 || SetTune > 0.0)
	{
		PR_BRFM_MATCH.Write(SetMatch , nCS);
		PR_BRFM_TUNE.Write(SetTune , nCS);
		
		while(TRUE) 
		{
			if( FNC_BRF.Status() != SEQ_RUNNING  )	break;
			if(WaitAbort(10))	return SEQ_ABORT;
		}
		
		FNC_BRF.RunTh("PRESET");
		bRF_Mode = 1;
		
		sprintf(szBuf, "Bias Matcher Match/Tune = %.0f/%.0f, RUN FNC_BRF|PRESET", SetMatch, SetTune);
		Make_Log("[RF_ACTION]", "%s \n",szBuf);
		
	}

	//VVC MOTOR SET
	Make_Log("[RF_ACTION]", "VVC MOTOR SET START \n ");	FNC_VVC_MOTOR.RunTh("MANUALSET");

	nCfgTime = 600;
	while(nCfgTime)
	{
		if(WaitAbort(100))
		{
			Make_Log("[RF_ACTION]", "MNT_RF_PRESET() MANUAL ABORT. END \n ");
			nResult = SEQ_ABORT;
			break;
		}
		
		SResult			= FNC_SRF.Status();
		BResult			= FNC_BRF.Status();
		VVCMoterResult  = FNC_VVC_MOTOR.Status();
		
		
		if(SResult == SEQ_SUCCESS && BResult == SEQ_SUCCESS && VVCMoterResult == SEQ_SUCCESS)
		{
			Make_Log("[RF_ACTION]", "ALL FUNCTION is SEQ_SUCCESS \n ");
			Make_Log("[RF_ACTION]", "MNT_RF_PRESET() END \n ");
			return SEQ_SUCCESS;
		}
		
		if(SResult == SEQ_ERROR || SResult == SEQ_ABORT )
		{
			Make_Log("[RF_ACTION]", "FNC_SRF is SEQ_ERROR or SEQ_ABORT \n ");
			nResult = SEQ_ABORT;
			FNC_SRF.Abort();
			break;
		}
		
		if(BResult == SEQ_ERROR || BResult == SEQ_ABORT)
		{
			Make_Log("[RF_ACTION]", "FNC_BRF is SEQ_ERROR or SEQ_ABORT \n ");
			nResult = SEQ_ABORT;
			FNC_BRF.Abort();
			break;
		}

		if(VVCMoterResult == SEQ_ERROR || VVCMoterResult == SEQ_ABORT)
		{
			Make_Log("[RF_ACTION]", "FNC_VVC_MOTOR is SEQ_ERROR or SEQ_ABORT \n ");
			nResult = SEQ_ABORT;
			FNC_VVC_MOTOR.Abort();
			break;
		}

		
		nCfgTime--;
		_sleep(100);
	}
	

	Make_Log("[RF_ACTION]", "MNT_RF_PRESET timeout error\n ");
	Make_Log("[RF_ACTION]", "FNC_SRF and FNC_BRF Set Abort() \n ");
	
	FNC_SRF.Abort();
	FNC_BRF.Abort();
		
	Make_Log("[RF_ACTION]", "MNT_RF_PRESET() ABORT END \n ");

	return nResult;
}





void Mon_ACTION_SCREENLOCK(void *pDummy )
{
	int nCs =0;
	double dcurPress=0;
	
	if(WaitKill(30*1000)) { _endthread(); return; }

	while(1)
	{
		if(WaitKill(1000))	break;

		//1. RF INIT CHECK
 
		//KLP_EDIT
		//RFPT Remote Check
		if (eDI_SRF_GenRemoteMode.Equal(REMOTE_ON_1))		SRF_REMOTE_CHK.Write(eOK);
		else												SRF_REMOTE_CHK.Write(eNG);
		
			
		if (eDI_SRFG_CommSts.Equal(ON))				SRF_ONLINE_CHK.Write(eOK);
		else										SRF_ONLINE_CHK.Write(eNG);
		
	
		if (eDI_SRFM_CommSts.Equal(ON))				SRM_ONLINE_CHK.Write(eOK);
		else										SRM_ONLINE_CHK.Write(eNG);
	
		if(CheckKill()) break;

		//KLP_EDIT
		//RFPT Remote Check
		if (eDI_BRF_GenRemoteMode.Equal(REMOTE_ON_1))		BRF_REMOTE_CHK.Write(eOK);
		else												BRF_REMOTE_CHK.Write(eNG);
		
		
		if (eDI_BRFG_CommSts.Equal(ON))				BRF_ONLINE_CHK.Write(eOK);
		else										BRF_ONLINE_CHK.Write(eNG);
		
		
		if (eDI_BRFM_CommSts.Equal(ON))				BRM_ONLINE_CHK.Write(eOK);
		else										BRM_ONLINE_CHK.Write(eNG);

				
		if(CheckKill()) break;
		if( SRF_REMOTE_CHK.Equal(eOK)		&&		BRF_REMOTE_CHK.Equal(eOK)		&&
			SRF_ONLINE_CHK.Equal(eOK)		&&		BRF_ONLINE_CHK.Equal(eOK)		&&
			SRM_ONLINE_CHK.Equal(eOK)		&&		BRM_ONLINE_CHK.Equal(eOK)		)
		{
			RF_INIT_RUN_NGOK.Write(eOK);
		}
		else
		{
			RF_INIT_RUN_NGOK.Write(eNG);

		}

		if(CheckKill()) break;

		//ALARM
		if(	eDI_SRF_GenFailSts.Equal(ON))			SRF_ERROR_CHK.Write(eNG);
		else										SRF_ERROR_CHK.Write(eOK);
			
		if(	eDI_SRF_MAT_Fail.Equal(ON))				SRM_ERROR_CHK.Write(eNG);
		else										SRM_ERROR_CHK.Write(eOK);

		if(	eDI_BRF_GenFailSts.Equal(ON))			BRF_ERROR_CHK.Write(eNG);
		else										BRF_ERROR_CHK.Write(eOK);
		
		if(	eDI_BRF_MAT_Fail.Equal(ON))				BRM_ERROR_CHK.Write(eNG);
		else										BRM_ERROR_CHK.Write(eOK);


		if(CheckKill()) break;
		if( SRF_ERROR_CHK.Equal(eOK)	 && 
			SRM_ERROR_CHK.Equal(eOK)	&& 
			BRF_ERROR_CHK.Equal(eOK)	&& 
			BRM_ERROR_CHK.Equal(eOK)	&& 
			RF_INIT_RUN_NGOK.Equal(eOK) )
		{
			RF_ERROR_CHK.Write(eOK);
		}
		else
		{
			RF_ERROR_CHK.Write(eNG);
		}

	
		if(CheckKill()) break;
		//2. RF POWER ON

		if( DI_PROCESS_GAS_SUPPLY_VALVE.Equal(ON) && rDO_PROCESS_GAS_SUPPLY_VALVE.Equal(ON) )	 RF_PRC_GAS_VLV_CHK.Write(eOK);
		else																					 RF_PRC_GAS_VLV_CHK.Write(eNG);

		/*if( DI_SOURCE_RFG_INTERLOCK.Equal(ON)  )					   RF_SRF_EXTINTLK_CHK.Write(eOK);
		else													       RF_SRF_EXTINTLK_CHK.Write(eNG);
	
		if( DI_BIAS_RFG_INTERLOCK.Equal(ON)  )						   RF_BRF_EXTINTLK_CHK.Write(eOK);
		else													       RF_BRF_EXTINTLK_CHK.Write(eNG);*/

		if(CheckKill()) break;
		if( XI_TM_GateVlv_Sns.Read(nCs) == GateClose )				   RF_RUN_GATECLOE_CHK.Write(eOK,nCs);
		else														   RF_RUN_GATECLOE_CHK.Write(eNG,nCs);
	
		if( eDI_FRC_CommSts.Equal(ON)  )							   RF_FRC_ONLINE_CHK.Write(eOK);
		else													       RF_FRC_ONLINE_CHK.Write(eNG);

		if( APC_CommSts.Equal(ON)  )								   RF_APC_ONLINE_CHK.Write(eOK);
		else													       RF_APC_ONLINE_CHK.Write(eNG);

		if( VVC_MOTOR_CommSts.Equal(ON)  )							   RF_VVC_MOTOR_ONLINE_CHK.Write(eOK);
		else													       RF_VVC_MOTOR_ONLINE_CHK.Write(eNG);


		if(CheckKill()) break;
		dcurPress=PM_Pressure_mtorr.Read(nCs);
		if(dcurPress > 7.0 )										   RF_PRESSURE_OVER_CHK.Write(eOK);
		else													       RF_PRESSURE_OVER_CHK.Write(eNG);
		
		if(dcurPress < 500.0 )									       RF_PRESSURE_UNDER_CHK.Write(eOK);
		else													       RF_PRESSURE_UNDER_CHK.Write(eNG);

		if( APC_RUN_NGOK.Equal(ON)  )								   RF_APC_CONRTROL_CHK.Write(eOK);
		else													       RF_APC_CONRTROL_CHK.Write(eNG);


		RF_PLCERR_CHK.Write(eOK,nCs);

		if(CheckKill()) break;
		if( RF_ERROR_CHK.Equal(eOK)				&&	
			RF_ALL_WFS_CHK.Equal(eOK)			&&		
			RF_ALL_WLS_CHK.Equal(eOK)			&&	
			RF_ALL_COVER_CHK.Equal(eOK)			&&	
			RF_PRC_GAS_VLV_CHK.Equal(eOK)		&&		
			//KLP_EDIT
			// RFPT는 EXTINTLK 신호를 PLC DI로 받고있지 않음. 
			//RF_SRF_EXTINTLK_CHK.Equal(eOK)		&&		
			//RF_BRF_EXTINTLK_CHK.Equal(eOK)		&&	
			RF_PLCERR_CHK.Equal(eOK)			&&
		
			RF_RUN_GATECLOE_CHK.Equal(eOK)		&&	
			RF_APC_ONLINE_CHK.Equal(eOK)		&&	
			RF_FRC_ONLINE_CHK.Equal(eOK)		&&		
			RF_VVC_MOTOR_ONLINE_CHK.Equal(eOK)	&&		
			RF_PRESSURE_OVER_CHK.Equal(eOK)		&&	
			RF_PRESSURE_UNDER_CHK.Equal(eOK)	&&	
			RF_APC_CONRTROL_CHK.Equal(eOK)		)
		{
			RF_POWERON_RUN_NGOK.Write(eOK);
		}
		else
		{
			RF_POWERON_RUN_NGOK.Write(eNG);		
		}
		
	}
	
	_endthread();
}


//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
SEQ_STATUS SeqMain(const char* szCommand, const void* pParam, int nNumOfBytesRecved)
{
	printf("-----------> [ %s ] START... [%s]\n", GetSeqName(), szCommand);

	SEQ_STATUS nResult		= SEQ_SUCCESS;

	TString strCommand = szCommand;
	TStringArray strTokens = strCommand.GetToken( _TEXT(" \t") );

	int		ncs			= 0;
	int		nFncMode	= 0;
	double	dblCT		= 0.0;
	double	dblCL		= 0.0;
	double	dblSetPwr	= 0.0;

	if( strTokens.count() <= 0 ) 
	{
		Make_Log("[SEQ_MAIN]", "Not Support strTokens.count()[%s]....return Abort \n", szCommand);		
		return SEQ_ABORT;
	}

	if(eDI_SRFG_CommSts.Read(ncs) != ON)
	{
		Make_Log("[SEQ_MAIN]", "Source RF Generator is Offline. RF Function Action is Abort \n");		
		SRF_6421.PostAnyTh();
		return SEQ_ABORT;
	}

	if(eDI_BRFG_CommSts.Read(ncs) != ON)
	{
		Make_Log("[SEQ_MAIN]", "Bias RF Generator is Offline. RF Function Action is Abort \n");		
		BRF_6471.PostAnyTh();
		return SEQ_ABORT;
	}

	if(eDI_SRFM_CommSts.Read(ncs) != ON)
	{	
		Make_Log("[SEQ_MAIN]", "Source RF Matcher is Offline. RF Function Action is Abort \n");		
		SRF_6422.PostAnyTh();
		return SEQ_ABORT;
	}

	if(eDI_BRFM_CommSts.Read(ncs) != ON)
	{
		Make_Log("[SEQ_MAIN]", "Bias RF Matcher is Offline. RF Function Action is Abort \n");		
		BRF_6472.PostAnyTh();
		return SEQ_ABORT;
	}
	

	if( strTokens[0] == _TEXT("MNT_RF_ON")  || strTokens[0] == _TEXT("PROC_RF_DUAL") || strTokens[0] == _TEXT("PROC_RF_HIGH")  )
	{
		DO_PLC_ResetRun.Write(ON, ncs); _sleep(100); 		
		DO_PLC_ResetRun.Write(OFF, ncs); 

		if(RF_VVC_MOTOR_ONLINE_CHK.Read(ncs) != ON)
		{		
			Make_Log("[SEQ_MAIN]", "VVC MOTOR Controller is Offline. RF Function Action is Abort \n");		
			VVC_MOTOR_6540.PostAnyTh();
			return SEQ_ABORT;
		}
	
		if(eDI_FRC_CommSts.Read(ncs) != ON)
		{
			Make_Log("[SEQ_MAIN]", "FRC is Offline. RF Function Action is Abort \n");		
			FRC_6700.PostAnyTh();
			return SEQ_ABORT;
		}

		if(APC_CommSts.Read(ncs) != ON)
		{
			Make_Log("[SEQ_MAIN]", "APC is Offline. RF Function Action is Abort \n");		
			APC_7405.PostAnyTh();
			return SEQ_ABORT;
		}

		if(DI_PROCESS_GAS_SUPPLY_VALVE.Equal(OFF) )		 
		{
			Make_Log("[SEQ_MAIN]", "Process Gas Valve Sensor[DI_ProcGasVlv_Sns] is Not Open. RF On Action is Abort \n");		
			RF_OFF_6513.PostTh();
			return SEQ_ABORT;
		}		
		
		if(eAPC_Position_RA.Read(ncs)  > 800 )	  
		{
			Make_Log("[SEQ_MAIN]", "APC Position is 800 Over. RF On Action is Abort \n");		
			RF_OFF_6512.PostTh();
			return SEQ_ABORT;
		}		
		
		if(PM_Pressure_mtorr.Read(ncs) < 5.0 )	  
		{
			Make_Log("[SEQ_MAIN]", "PC Pressure is under 5.0 mtorr under. RF On Action is Abort \n");		
			RF_OFF_6510.PostTh();
			return SEQ_ABORT;
		}

		Make_Log("[SEQ_MAIN]", "BEFORE POWER ON, RAMP UP PARAMETER RE CHECK START \n");

	

		Make_Log("[SEQ_MAIN]", "BEFORE POWER ON, RAMP UP PARAMETER RE CHECK END \n");

	}

	if     (strTokens[0] == _TEXT("NONE"				))									nResult = SEQ_SUCCESS;
	else if(strTokens[0] == _TEXT("PROC_RF_OFF"			))									nResult = PROC_RF_OFF(NORMAL_OFF);	
	else if(strTokens[0] == _TEXT("PROC_RF_OFF_INTLK"	))									nResult = PROC_RF_OFF(INTERLOCKED_OFF);	
	else if(strTokens[0] == _TEXT("PROC_RF_HIGH"		))									nResult = PROC_RF_HIGH();
	else if(strTokens[0] == _TEXT("PROC_RF_DUAL"		))									nResult = PROC_RF_DUAL();
	else if(strTokens[0] == _TEXT("MNT_RF_INIT"			))	{	PM_RF_STS.Write(1 , ncs);	nResult = MNT_RF_INIT();	PM_RF_STS.Write(0 , ncs);	}
	else if(strTokens[0] == _TEXT("MNT_RF_PRESET"		))	{	PM_RF_STS.Write(1 , ncs);	nResult = MNT_RF_PRESET();	PM_RF_STS.Write(0 , ncs);	}
	else if(strTokens[0] == _TEXT("MNT_RF_OFF"			))	{	PM_RF_STS.Write(1 , ncs);	nResult = MNT_RF_OFF();		PM_RF_STS.Write(0 , ncs);	}
	else if(strTokens[0] == _TEXT("MNT_RF_ABORT"		))	{	PM_RF_STS.Write(1 , ncs);	nResult = MNT_RF_ABORT();	PM_RF_STS.Write(0 , ncs);	}
	else if(strTokens[0] == _TEXT("MNT_RF_ON"			))	{	PM_RF_STS.Write(1 , ncs);	nResult = MNT_RF_ON();		PM_RF_STS.Write(0 , ncs);	}
	else																					nResult = SEQ_ABORT;
	
	Make_Log("[SEQ_MAIN]", "---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------\n");

	return nResult;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnLoadComplete( const TCHAR* szArgument ) 
{
	
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL OnInitComplete( const TCHAR* szArgument ) 
{
	int CommStatus;
	if ( SIMULATION_CONTROL.Read(CommStatus) == ON ) SIMULATION_MODE = TRUE;
	else											 SIMULATION_MODE = FALSE;

	//RF OFF TREAD
	_beginthread(Mon_ACTION_SCREENLOCK,			0, NULL);	_sleep(100);
	

				
	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnUnLoad( int nCode ) 
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnEventRecv( const char* szCommand, const void* pParam , int nNumOfBytesRecved ) 
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
