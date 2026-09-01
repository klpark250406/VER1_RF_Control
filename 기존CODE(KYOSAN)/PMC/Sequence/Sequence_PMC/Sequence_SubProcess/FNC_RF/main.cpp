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
enum {eAUTO_0, eMANUAL_1};
enum {eALL_STOP_0, eMOVE_1};



//----------------------------------------------------------------------------------------------------------------------------------------------

BOOL SIMULATION_MODE = FALSE;
#define SRF									0
#define BRF									50

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
                                    
	CDIO        DO_RFG_ALRAM_RESET		    (_TEXT("DO_$1G_ALRAM_RESET"		));
	CDIO        DO_RFG_DNET_RESET		    (_TEXT("eDO_$0G_RFReset"		));
	CDIO        DO_RFM_DNET_RESET		    (_TEXT("eDO_$0_MAT_Reset"		));

	CDIO        DO_RFG_POWER_ON				(_TEXT("DO_$1G_POWER_ON"		));	
	CDIO        DO_RFG_POWER_DNET_ON		(_TEXT("eDO_$0G_RFPower"		));	

	CDIO        DO_RFG_EXTERNAL_INK_OK		(_TEXT("DO_$1G_EXTERNAL_INK_OK"	));	

	CDIO		DI_RFG_POWER_ON				(_TEXT("DI_$1G_POWER_ON"		));	                                   
	CDIO        DI_RFG_WFS			        (_TEXT("DI_$1G_WFS"				));
	CDIO        DI_RFM_WFS			        (_TEXT("DI_$1M_WFS"				));
	CDIO        DI_RFG_REMOTE_MODE			(_TEXT("DI_$1G_REMOTE_MODE"		));
	CDIO        DI_RFG_ALARM		        (_TEXT("DI_$1G_ALARM"			));
	CDIO        DI_RFG_INTERLOCK			(_TEXT("DI_$1G_INTERLOCK"		));
	CDIO        DI_RFG_LEAK_SENSOR			(_TEXT("DI_$1G_LEAK_SENSOR"		));

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
	
	CDIO        DI_RFGenErrorSts	        (_TEXT("eDI_$0_GenFailSts"		));

	CAIO        AI_RFG_FwdPwr			    (_TEXT("eAI_$0_FwdPwr"			));
	CAIO        AI_RFG_RefPwr			    (_TEXT("eAI_$0_RefPwr"			));
	CAIO        AI_RFM_MatchPos		        (_TEXT("eAI_$0_MAT_MatchPos"	));
	CAIO        AI_RFM_TunePos		        (_TEXT("eAI_$0_MAT_TunePos"		));
                                        
	CAIO        eAO_RFG_FwdPwr			    (_TEXT("eAO_$0_FwdPwr"			));
	CAIO        eAO_RFM_MatchPos		    (_TEXT("eAO_$0_MAT_MatchPos"	));
	CAIO        eAO_RFM_TunePos		        (_TEXT("eAO_$0_MAT_TunePos"		));
                                        
	CDIO        eDI_RFG_GenRemoteMode		(_TEXT("eDI_$0_GenRemoteMode"	));
	CDIO        eDO_RFG_CtrlMode		    (_TEXT("eDO_$0G_CtrlMode"		));

	CDIO        eDO_RFG_MAT_Control			(_TEXT("eDO_$0_MAT_Control"		));
	CDIO        eDO_RFG_MAT_Move		    (_TEXT("eDO_$0_MAT_Move"		));
                                                                                                                                                            	
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

	DO_RFG_EXTERNAL_INK_OK.Write(RF_Ready,			nCS); 
	Make_Log("[RFSUB_RUN]", "%s RF_ExtIntlk Set On(=Ready)  \n ", RFType);
	_sleep(500);

	eDO_RFG_CtrlMode.Write(eRemote,	nCS);  
	Make_Log("[RFSUB_RUN]", "%s RF_CtrlMode Set Remote Mode  \n ", RFType);
	_sleep(500);

	Make_Log("[RFSUB_RUN]", "%s RF_Reset Set On->Off Run  \n ", RFType);


	DO_RFM_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);
	DO_RFM_DNET_RESET.Write(eOFF_CMD,   nCS);

	_sleep(1000);

	DO_RFG_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);
	DO_RFG_DNET_RESET.Write(eOFF_CMD,   nCS);


	eDO_RFG_MAT_Control.Write( 0, nCS);
	eDO_RFG_MAT_Move.Write(0, nCS);


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
	eAO_RFG_FwdPwr.Write(dblSetpoint, nCS);

	Make_Log("[RFSUB_RUN]", "%s Power Set Start = %.0f OK \n ", RFType,dblSetpoint);
	
	if(nCS == TRUE)
	{

		Make_Log("[RFSUB_RUN]", "%s SetPoint Power = %.0f OK \n ", RFType,dblSetpoint);
		Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER() END \n ", RFType);
		
		return SEQ_SUCCESS;
	}
	else
	{
		_sleep(100);
		eAO_RFG_FwdPwr.Write(dblSetpoint, nCS);
		
		if(nCS == TRUE)
		{
			Make_Log("[RFSUB_RUN]", "%s SetPoint Power = %.0f OK[One More Check] \n ", RFType,dblSetpoint);
			Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER() END \n ", RFType);

			return SEQ_SUCCESS;

		}
		else
		{
			AlmAt(&SRF_6426 , gnAlarmOffset)->PostAnyTh();

			Make_Log("[RFSUB_RUN]", "%s Failed to set Power = %.0f \n ", RFType,dblSetpoint);
			Make_Log("[RFSUB_RUN]", "%s SET_RF_POWER() ABORT END \n ", RFType);

			return SEQ_ABORT;

		}	
	}

	return SEQ_SUCCESS;

}


SEQ_STATUS ControlPreset()
{
	int	   nCS,nCount=0;

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

	//ADD RF RESET 1012 
	Make_Log("[RFSUB_RUN]", "%s ControlPreset() RF_Reset Set On->Off Run  \n ", RFType);
	
	DO_RFM_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);
	DO_RFM_DNET_RESET.Write(eOFF_CMD,   nCS);
	_sleep(1000);
	
	DO_RFG_DNET_RESET.Write(eON_CMD,	nCS);		_sleep(500);
	DO_RFG_DNET_RESET.Write(eOFF_CMD,   nCS);


    // until here.
	dblTune		= PR_RFM_TUNE.Read(nCS);
	dblMatch	= PR_RFM_MATCH.Read(nCS);

    dblPreTune	= AI_RFM_TunePos.Read(nCS);
    dblPreMatch = AI_RFM_MatchPos.Read(nCS);

	eAO_RFM_TunePos.Write(dblTune, nCS);
	eAO_RFM_MatchPos.Write(dblMatch, nCS);

	//Step 1
	eDO_RFG_MAT_Control.Write(eMANUAL_1, nCS);	//  0:AUTO,      1:MANUAL
	eDO_RFG_MAT_Move.Write(eMOVE_1, nCS);		//  0: All STOP, 1: MOVE

	// STEP 2
	if		( PR_RFG_POWER.Read(nCS) > 0) 
	{
		Make_Log("[RFSUB_RUN]", "RF_POWER_ON_STEP %s ControlPreset() RUN 1 SEC \n ", RFType);
		WaitAbort(1000);
	}
	else if	(	dblCurTune == dblTune && dblCurMatch == dblMatch )
	{
		Make_Log("[RFSUB_RUN]", "PRESET DATA EQUAL. %s ControlPreset() RUN 1 SEC \n ", RFType);
		WaitAbort(1000);
	}
	else
	{
		Make_Log("[RFSUB_RUN]", "%s ControlPreset() MOVING 3 SEC \n ", RFType);
		WaitAbort(3000);	
	}

	//Step 3
	eDO_RFG_MAT_Move.Write(eALL_STOP_0, nCS);		//  0: All STOP, 1: MOVE
	eDO_RFG_MAT_Control.Write(eAUTO_0, nCS);		//  0:AUTO,      1:MANUAL
	
	Make_Log("[RFSUB_RUN]", "%s ControlPreset() END \n ", RFType);

	return SEQ_SUCCESS;
}


SEQ_STATUS MinMax_AutoPreset()
{
	return SEQ_SUCCESS;
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
				nFnResult = SEQ_ABORT;
				break;
		
		case eRF_DUAL :
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
				nFnResult = SEQ_ABORT;
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
	else if(strTokens[0] == _TEXT("PULSEON"		))		nResult = SET_RF_POWER();
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
