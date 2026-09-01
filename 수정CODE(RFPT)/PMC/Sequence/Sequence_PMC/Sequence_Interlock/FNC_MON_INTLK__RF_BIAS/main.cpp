#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <time.h>

#include <tMain.h>
#include <TUtilStr.h>
#include <TTimer.h>
#include <TString.h>

#include "h/Define.h"

//----------------------------------------------------------------------------------------------------------------------------------------------

BOOL SIMULATION_MODE = FALSE;

#define MONITORING_SLEEP		100 

//----------------------------------------------------------------------------------------------------------------------------------------------

enum {eOFF,  eON};
enum {eRFOff, eRFOn};
enum {PM_Starting, PM_Started};
enum {GateNone, GateOpen, GateClose, GateError};

//----------------------------------------------------------------------------------------------------------------------------------------------

static int gnItk_RF_Fwd		= 0;
static int gnItk_RF_Ref		= 0;
static int gnItk_RF_Ref_Max	= 0;
static int gnItk_RF_Vpp		= 0;
static int gnItk_RF_Vdc		= 0;
static int gnItk_RF_Arc		= 0;
static int gnItk_RF_Dechuck = 0;
static int gnItk_RF_PLC		= 0;
static int gnItk_RF_Util	= 0;
static int gnItk_RF_ArcCount= 0;


//----------------------------------------------------------------------------------------------------------------------------------------------

BEGIN_OBJECT_ENUMERATION

	CDIO			SIMULATION_CONTROL			("System.SimulMode");

	CSequence		FNC_SRF						(_TEXT("FNC_SRF"					));
	CSequence       FNC_BRF				        (_TEXT("FNC_BRF"					));
	CSequence       FNC_ESC				        (_TEXT("FNC_ESC"					));
	CSequence       FNC_HE					    (_TEXT("FNC_HE"						));
	CSequence       FNC_MFC				        (_TEXT("FNC_MFC"					));
	CSequence       FNC_APC				        (_TEXT("FNC_APC"					));
	CSequence       FNC_RF_CTRL			        (_TEXT("FNC_RF_CTRL"				));
                            
	CDIO            PM_PROC_RFCTRL			    (_TEXT("PM_PROC_RFCTRL"				));
	CDIO            PM_RF_CTRL				    (_TEXT("PM_RF_CTRL"					));
	CDIO            PM_Start_Flag			    (_TEXT("PM_Start_Flag"				));
                                                                                        
	CDIO            dSoftware_Intlk_Sts1	    (_TEXT("dSoftware_Intlk_Sts1"		));
	CDIO            dSoftware_Intlk_Sts2	    (_TEXT("dSoftware_Intlk_Sts2"		));
	CDIO            dSoftware_Intlk_Sts3	    (_TEXT("dSoftware_Intlk_Sts3"		));
	CDIO            dSoftware_Intlk_Sts4	    (_TEXT("dSoftware_Intlk_Sts4"		));
	CDIO            dSoftware_Intlk_Sts5	    (_TEXT("dSoftware_Intlk_Sts5"		));
                                            
	CAIO            Rcp_CurStepNum			    (_TEXT("Rcp_CurStepNum"				));
	CAIO            Step_CurTime			    (_TEXT("Step_CurTime"				));

	CDIO            DO_SRF_Cmd_Return			(_TEXT("DO_SOURCE_RFG_POWER_ON"		));
	CDIO            DO_BRF_Cmd_Return			(_TEXT("DO_BIAS_RFG_POWER_ON"		));                                           
	CDIO            rDO_SRF_Cmd_Return			(_TEXT("rDO_SOURCE_RFG_POWER_ON"	));
	CDIO            rDO_BRF_Cmd_Return			(_TEXT("rDO_BIAS_RFG_POWER_ON"		));
	CDIO			DI_SRF_PowerSts				(_TEXT("DI_SOURCE_RFG_POWER_ON"		));
	CDIO			DI_BRF_PowerSts				(_TEXT("DI_BIAS_RFG_POWER_ON"		));
                                            
	CDIO            Itk_SRC_Mon			        (_TEXT("Itk_SRC_Mon"				));
	CDIO            Itk_BIAS_Mon			    (_TEXT("Itk_BIAS_Mon"				));

	CAIO            PR_SRFG_POWER			    (_TEXT("PR_SRFG_POWER"				));
	CAIO            PR_BRFG_POWER			    (_TEXT("PR_BRFG_POWER"				));
												
	CAIO            PR_SRF_MAX_VPP			    (_TEXT("PR_SRF_MAX_VPP"				));
                                            
	CAIO            SRF_FwdPwr_RA			    (_TEXT("SRF_FwdPwr_RA"				));
	CAIO            SRF_RefPwr_RA			    (_TEXT("SRF_RefPwr_RA"				));
	//CAIO            SRF_Vpp_RA				    (_TEXT("SRF_Vpp_RA"					));
	//CAIO            SRF_Vdc_RA				    (_TEXT("SRF_Vdc_RA"					));
                                                                                       
	CAIO            cSRfFwdStableTime		    (_TEXT("cSRfFwdStableTime"			));
	CAIO            cSRfFwdAbort			    (_TEXT("cSRfFwdAbort"				));
	CAIO            cSRfFwdAbortCnt		        (_TEXT("cSRfFwdAbortCnt"			));
	CAIO            cSRfFwdWarn			        (_TEXT("cSRfFwdWarn"				));
	CAIO            cSRfFwdWarnCnt			    (_TEXT("cSRfFwdWarnCnt"				));
                                            
	CAIO            cSRfRefStableTime		    (_TEXT("cSRfRefStableTime"			));
	CAIO            cSRfRefAbort			    (_TEXT("cSRfRefAbort"				));
	CAIO            cSRfRefAbortCnt		        (_TEXT("cSRfRefAbortCnt"			));
	CAIO            cSRfRefWarn			        (_TEXT("cSRfRefWarn"				));
	CAIO            cSRfRefWarnCnt			    (_TEXT("cSRfRefWarnCnt"				));
                                            
	CAIO            cSRfVppStableTime		    (_TEXT("cSRfVppStableTime"			));
	CAIO            cSRfVppAbort			    (_TEXT("cSRfVppAbort"				));
	CAIO            cSRfVppAbortCnt		        (_TEXT("cSRfVppAbortCnt"			));
	CAIO            cSRfVppWarn			        (_TEXT("cSRfVppWarn"				));
	CAIO            cSRfVppWarnCnt			    (_TEXT("cSRfVppWarnCnt"				));
                                            
	CAIO            cSRfVdcStableTime		    (_TEXT("cSRfVdcStableTime"			));
	CAIO            cSRfVdcAbort			    (_TEXT("cSRfVdcAbort"				));
	CAIO            cSRfVdcAbortCnt		        (_TEXT("cSRfVdcAbortCnt"			));
	CAIO            cSRfVdcWarn			        (_TEXT("cSRfVdcWarn"				));
	CAIO            cSRfVdcWarnCnt			    (_TEXT("cSRfVdcWarnCnt"				));
                                            
	CAIO            PR_BRF_MAX_VPP			    (_TEXT("PR_BRF_MAX_VPP"				));
	
	CAIO            BRF_FwdPwr_RA			    (_TEXT("BRF_FwdPwr_RA"				));
	CAIO            BRF_RefPwr_RA			    (_TEXT("BRF_RefPwr_RA"				));
	//CAIO            BRF_Vpp_RA				    (_TEXT("BRF_Vpp_RA"					));
	CAIO            BRF_Vdc_RA				    (_TEXT("BRF_Vdc_RA"					));
                                          
	CAIO            cBRfFwdStableTime		    (_TEXT("cBRfFwdStableTime"			));
	CAIO            cBRfFwdAbort			    (_TEXT("cBRfFwdAbort"				));
	CAIO            cBRfFwdAbortCnt		        (_TEXT("cBRfFwdAbortCnt"			));
	CAIO            cBRfFwdWarn			        (_TEXT("cBRfFwdWarn"				));
	CAIO            cBRfFwdWarnCnt			    (_TEXT("cBRfFwdWarnCnt"				));
                                            
	CAIO            cBRfRefStableTime		    (_TEXT("cBRfRefStableTime"			));
	CAIO            cBRfRefAbort			    (_TEXT("cBRfRefAbort"				));
	CAIO            cBRfRefAbortCnt		        (_TEXT("cBRfRefAbortCnt"			));
	CAIO            cBRfRefWarn			        (_TEXT("cBRfRefWarn"				));
	CAIO            cBRfRefWarnCnt			    (_TEXT("cBRfRefWarnCnt"				));
                                            
	CAIO            cBRfVppStableTime		    (_TEXT("cBRfVppStableTime"			));
	CAIO            cBRfVppAbort			    (_TEXT("cBRfVppAbort"				));
	CAIO            cBRfVppAbortCnt		        (_TEXT("cBRfVppAbortCnt"			));
	CAIO            cBRfVppWarn			        (_TEXT("cBRfVppWarn"				));
	CAIO            cBRfVppWarnCnt			    (_TEXT("cBRfVppWarnCnt"				));
                                            
	CAIO            cBRfVdcStableTime		    (_TEXT("cBRfVdcStableTime"			));
	CAIO            cBRfVdcAbort			    (_TEXT("cBRfVdcAbort"				));
	CAIO            cBRfVdcAbortCnt		        (_TEXT("cBRfVdcAbortCnt"			));
	CAIO            cBRfVdcWarn			        (_TEXT("cBRfVdcWarn"				));
	CAIO            cBRfVdcWarnCnt			    (_TEXT("cBRfVdcWarnCnt"				));

	CAIO            cMaxRefAbort			    (_TEXT("cMaxRefAbort"				));
                                                                                        
	CSIO            Rcp_Name				    (_TEXT("Rcp_Name"					));
	CSIO            Step_Name				    (_TEXT("Step_Name"					));
                                                                                        
	CDIO            Dechuck_InterlockOn	        (_TEXT("Dechuck_InterlockOn"		));

	CAIO            cDechuckStableTime		    (_TEXT("cDechuckStableTime"			));
	CAIO            cDechuckAbort				(_TEXT("cDechuckAbort"				));
	CAIO            cDechuckAbortCnt		    (_TEXT("cDechuckAbortCnt"			));

	CDIO            PR_ETCHSTEP			        (_TEXT("PR_ETCHSTEP"				));	



	CDIO			XI_TM_GateVlv_Sns			(_TEXT("XI_TM_GateVlv_Sns"			));
	CAIO		    PM_Pressure_mtorr		    (_TEXT("PM_Pressure_mtorr"			));
	CAIO		    eAPC_Position_RA			(_TEXT("APC_Position_RA"			));	
	CDIO			DI_PROCESS_GAS_SUPPLY_VALVE	(_TEXT("DI_PROCESS_GAS_SUPPLY_VALVE"));
	CDIO			DI_DP_RUN					(_TEXT("DI_DP_RUN"					));

	CAlarm          BRF_6451			        (_TEXT("BRF_6451"					));		// RF Forward power exceeded warning_level
	CAlarm          BRF_6452			        (_TEXT("BRF_6452"					));		// RF Reflect power exceeded warning_level
	CAlarm          BRF_6453			        (_TEXT("BRF_6453"					));		// RF Vdc exceeded warning_level
	CAlarm          BRF_6454			        (_TEXT("BRF_6454"					));		// RF Vpp exceeded warning_level
	CAlarm          BRF_6455			        (_TEXT("BRF_6455"					));		// Bias RF initial Reflect Level is high. Check Preset

	CAlarm          BRF_6461			        (_TEXT("BRF_6461"					));		// RF Forward power exceeded Alarm_level
	CAlarm          BRF_6462			        (_TEXT("BRF_6462"					));	    // RF Reflect power exceeded Alarm_level
	CAlarm          BRF_6463			        (_TEXT("BRF_6463"					));	    // RF Vdc exceeded Alarm_level  
	CAlarm          BRF_6464			        (_TEXT("BRF_6464"					));	    // RF Vpp exceeded Alarm_level
	CAlarm          BRF_6465			        (_TEXT("BRF_6465"					));		// RF Reflect power exceeded Max_Over_level

	CAlarm          RF_OFF_6510					(_TEXT("RF_OFF_6510"				));		// PM Pressure is lower than 5.0 mtorr. RF Power Off Action Run
	CAlarm          RF_OFF_6511					(_TEXT("RF_OFF_6511"				));		// DRP PUMP is Not Running. RF Power Off Action Run	
	CAlarm          RF_OFF_6512					(_TEXT("RF_OFF_6512"				));		// APC Position is higher than 800. RF Power Off Action Run	
	CAlarm          RF_OFF_6513					(_TEXT("RF_OFF_6513"				));		// Process gas Valve Sensor was not open. RF Power Off Action Run
	CAlarm          RF_OFF_6514					(_TEXT("RF_OFF_6514"				));		// PC Gate is Not Open. RF Power Off Action Run
	

END_OBJECT_ENUMERATION
//----------------------------------------------------------------------------------------------------------------------------------------------
void Make_Log(char *SeparateSting, LPSTR list, ...) 
{
	char		FOLDER[64]	 =  { "Log\\SEQ\\INTERLOCK\\RF" };
	char		LogName[64]	 =  { "RF_ON" };
	
	FILE *fpt;
	char FileName[512]; 
	
	SYSTEMTIME		SysTime;
	va_list			va;
	
	GetLocalTime(&SysTime);
	
	va_start(va, list);
	
	{
		sprintf(FileName, "%s\\%s%04d%02d%02d.log", FOLDER, LogName, SysTime.wYear, SysTime.wMonth, SysTime.wDay);
		
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


//KLP_EDIT
// 해당 Function은 기존에서도 사용하지 않았음. 
// 기존 2G 및 A3에서도 아래 함수는 사용하지 않고 선언만 되어있음.
// 그러므로 주석처리 진행.
/*void Fnc_Vpp_Check(void *pDummy)
{
	int		nCs = 0;

	double	dSetpoint = 0.0;
	double	dActual   = 0.0;
	
	int     alarm_result = 0;

	char    szTemp[256] = {0};

	double	dcfg_warning_level	= 0.0;
	double	dcfg_alarm_level	= 0.0;

	int		cfg_warning_cnt		= 0;
	int		cfg_alarm_cnt		= 0;
	int		cfg_stable_time		= 0;

	int		cur_stable_time		= 0;
	int		cur_warning_cnt		= 0;
	int		cur_alarm_cnt		= 0;

	CAlarm  alarm_id	= BRF_6464;
	CAlarm	warning_id	= BRF_6454;

	char	szTest[128] = {0};
	int		str_length   = 0;
	char	strMsg[256] = {0};


	cur_stable_time		= 0;
	cur_warning_cnt		= 0;
	cur_alarm_cnt		= 0;
	
	gnItk_RF_Vpp		= 0;
	
	
	while(1)
	{
	RF_VPP_IDLE:

		while(1)
		{

			if(Itk_BIAS_Mon.Read(nCs) == 1)
			{
				if(strcmp(strMsg, "BIAS_Vpp Mon Run") != 0)
				{
					sprintf(strMsg, "BIAS_Vpp Mon Run");
				}
				break;
			}

			if(WaitKill(100))	break;
		}



		if(PR_BRF_MAX_VPP.Read(nCs) > 1.0) 
		{
			dcfg_alarm_level=		PR_BRF_MAX_VPP.Read(nCs);
		}
		else
		{
			dcfg_alarm_level=		cBRfVppAbort.Read(nCs);
		}

		dcfg_warning_level	=		cBRfVppWarn.Read(nCs);
		cfg_alarm_cnt		= (int)(cBRfVppAbortCnt.Read(nCs));
		cfg_warning_cnt		= (int)(cBRfVppWarnCnt.Read(nCs));
		cfg_stable_time		= (int)(cBRfVppStableTime.Read(nCs)) * 10;

		if(WaitKill(100))	break;

		if(Itk_BIAS_Mon.Read(nCs) == 0)
		{
			cur_stable_time		= 0;
			cur_warning_cnt		= 0;
			cur_alarm_cnt		= 0;
			gnItk_RF_Vpp		= 0;

			_sleep(100);

			if(strcmp(strMsg, "BIAS_Vpp Mon Stopped") != 0)
			{
				sprintf(strMsg, "BIAS_Vpp Mon Stopped");
			}

			goto RF_VPP_IDLE;
		}


		dActual	= BRF_Vpp_RA.Read(nCs);


		if(cur_stable_time <= cfg_stable_time)
		{
			cur_stable_time++;

			goto RF_VPP_IDLE;
		}
		else
		{
			if(PR_BRF_MAX_VPP.Read(nCs) > 1.0) 
			{
				if(dcfg_alarm_level != PR_BRF_MAX_VPP.Read(nCs))
				{
					cur_stable_time		= 0;
					cur_warning_cnt		= 0;
					cur_alarm_cnt		= 0;
					dcfg_alarm_level	= PR_BRF_MAX_VPP.Read(nCs);
				}
			}

					
			if(dActual > dcfg_alarm_level)
			{
				cur_alarm_cnt++;
			}
			else
			{
				cur_alarm_cnt = 0;
				gnItk_RF_Vpp = 0;


				if(dActual > dcfg_warning_level)	cur_warning_cnt++;
				else								cur_warning_cnt = 0;
			}


			if(cur_warning_cnt >= cfg_warning_cnt)
			{
				if(warning_id.Result() != ALM_POSTED)
				{
					warning_id.PostAnyTh();
				}
			}
			else if(cur_warning_cnt == 0)
			{
				if(warning_id.Result() == ALM_POSTED)
					warning_id.Clear();
			}


			if(cur_alarm_cnt	>= cfg_alarm_cnt)
			{
	
				FNC_RF_CTRL.Run("PROC_RF_OFF_INTLK");

				alarm_id.PostAnyTh();
				
				gnItk_RF_Vpp = 1;

				FNC_SRF.Abort();
				FNC_BRF.Abort();
	
				_sleep(100);


				do
				{
					_sleep(250);

					alarm_result = alarm_id.Result();
					
					if(alarm_result == ALM_ABORT)
					{
						break;
					}
				}while(alarm_result != ALM_CLEAR);

				cur_stable_time		= 0;
				cur_alarm_cnt		= 0;
				cur_warning_cnt		= 0;

				warning_id.Clear();

				gnItk_RF_Vpp = 0;

				goto RF_VPP_IDLE;
			}
		}
	}
	_endthread();
}
*/

void Fnc_Vdc_Check(void *pDummy)
{

	int		nCs = 0;
	
	double	dSetpoint = 0.0;
	double	dActual   = 0.0;
	
	int     alarm_result = 0;
	
	char    szTemp[256] = {0};
	
	double	dcfg_warning_level	= 0.0;
	double	dcfg_alarm_level	= 0.0;
	
	double  dStep_CurTime;
	char    RecipeName[256]		= {0};
	char    CurStepName[256]	= {0};

	int		cfg_warning_cnt		= 0;
	int		cfg_alarm_cnt		= 0;
	int		cfg_stable_time		= 0;
	
	int		cur_stable_time		= 0;
	int		cur_warning_cnt		= 0;
	int		cur_alarm_cnt		= 0;
	
	CAlarm  alarm_id	= BRF_6463;
	CAlarm	warning_id	= BRF_6453;
	
	char	szTest[128] = {0};
	int		str_length   = 0;
		
	char	strMsg[256] = {0};
	
	cur_stable_time		= 0;
	cur_warning_cnt		= 0;
	cur_alarm_cnt		= 0;
	gnItk_RF_Vdc		= 0;

	
	while(1)
	{
		if(WaitKill(10))	break;
		
		while(1)
		{
			if(Itk_BIAS_Mon.Read(nCs) == 1)
			{
				break;
			}

			if(WaitKill(100))	break;
		}


		do 
		{
			
			if( WaitKill(MONITORING_SLEEP) ) {	Itk_BIAS_Mon.Write(eOFF, nCs);	break; }

			dcfg_alarm_level	=		cBRfVdcAbort.Read(nCs);
			dcfg_warning_level	=		cBRfVdcWarn.Read(nCs);
			dStep_CurTime		=		Step_CurTime.Read(nCs);
		
			cfg_alarm_cnt		= (int)(cBRfVdcAbortCnt.Read(nCs));
			cfg_warning_cnt		= (int)(cBRfVdcWarnCnt.Read(nCs));
			cfg_stable_time		= (int)(cBRfVdcStableTime.Read(nCs)) * 10;

			if(CheckKill()) break;
			Rcp_Name.Read(RecipeName , nCs);
			Step_Name.Read(CurStepName , nCs);

			if(Itk_BIAS_Mon.Read(nCs) == 0)
			{
				cur_stable_time		= 0;
				cur_warning_cnt		= 0;
				cur_alarm_cnt		= 0;
				
				gnItk_RF_Vdc		= 0;
				
				if(WaitKill(100)) { break; }
				
				break;
			}

			if(cur_stable_time <= cfg_stable_time)
			{
				if(cur_stable_time == cfg_stable_time)
				{
					sprintf(strMsg, "BIAS_VDC MONITORING START.[CONFIG DATA:: STABLE_TIME: %d sec, WARNING: %d Count, ALARM : %d Count]", cfg_stable_time/10, cfg_warning_cnt, cfg_alarm_cnt);
					Make_Log("[RF_PWRON]","%s \n",strMsg );	
				}	
				
				cur_stable_time++;
				break;
			}

			if(dStep_CurTime < cBRfVdcStableTime.Read(nCs)) break;

			if(CheckKill()) break;

			dActual	= BRF_Vdc_RA.Read(nCs);

			if(dActual < 0) dActual = abs(dActual);


			if(dActual > dcfg_alarm_level)
			{
				cur_alarm_cnt++;
				sprintf(strMsg, "BIAS_VDC Alarm Counting[%d Count][StepTime: %3.0f sec, AlarmLevel: %3.0f]",cfg_alarm_cnt, dStep_CurTime,dcfg_alarm_level);
											
				Make_Log("[RF_PWRON]","%s \n",strMsg );
				
			}
			else
			{
				cur_alarm_cnt = 0;
								
				gnItk_RF_Vdc = 0;
				
				if(dActual > dcfg_warning_level)	
				{
					cur_warning_cnt++;
				}
				
				else
				{
					cur_warning_cnt = 0;
				}
			}


			if(cur_warning_cnt >= cfg_warning_cnt)
			{
				if(warning_id.Result() != ALM_POSTED)
				{
					warning_id.PostAnyTh();

					sprintf(strMsg, "BIAS_VDC Warning Post [StepTime: %3.0f sec, Real_Data: %3.0f  WarningLevel: %3.0f]" ,dStep_CurTime,BRF_Vdc_RA.Read(nCs),dcfg_warning_level);
													
															
					Make_Log("[RF_PWRON]","%s \n",strMsg );
				}
			}

			if(CheckKill()) break;
			if(cur_alarm_cnt >= cfg_alarm_cnt)
			{			
				
				FNC_RF_CTRL.Run("PROC_RF_OFF_INTLK");

				
				sprintf(szTemp, "Bias Vdc exceeded Abort(%s step, %3.0f sec, VDC: %3.0f )",
															  CurStepName,dStep_CurTime,dActual);
															 
				alarm_id.PostAnyTh(szTemp);
		

				gnItk_RF_Vdc = 1;


				sprintf(strMsg, "BIAS_VDC Alarm Post[VDC: %3.0f ]", BRF_Vdc_RA.Read(nCs));
														
				Make_Log("[RF_PWRON]","%s \n",strMsg );			
				Make_Log("[RF_PWRON]","Process Run Abort. FNC_RF|POWEROFF \n");

				FNC_SRF.Abort();
				FNC_BRF.Abort();
				
				
				do
				{
					if(WaitKill(250)) break;
					
					alarm_result = alarm_id.Result();
					
					if(alarm_result == ALM_ABORT)
					{
						break;
					}
				}while(alarm_result != ALM_CLEAR);
				
				cur_stable_time		= 0;
				cur_alarm_cnt		= 0;
				cur_warning_cnt		= 0;
				
				warning_id.Clear();
				
				gnItk_RF_Vdc = 0;
				
				break;

			}

		} while(1);

	} 

}


void Fnc_Fwd_Check(void *pDummy)
{
	int		nCs = 0;
	
	double	dSetpoint = 0.0;
	double	dActual   = 0.0;
	
	int     alarm_result = 0;
	char    szTemp[256]  = {0};	
	
	double	dcfg_warning_level	= 0.0;
	double	dcfg_alarm_level	= 0.0;

	double  dStep_CurTime;
	
	int		cfg_warning_cnt		= 0;
	int		cfg_alarm_cnt		= 0;
	int		cfg_stable_time		= 0;
	
	int		cur_stable_time		= 0;
	int		cur_warning_cnt		= 0;
	int		cur_alarm_cnt		= 0;
	
	CAlarm  alarm_id	= BRF_6461;
	CAlarm	warning_id	= BRF_6451;

	char	szTest[128] = {0};
	int		str_length   = 0;
	
	char	strMsg[256] = {0};
	
	cur_stable_time		= 0;
	cur_warning_cnt		= 0;
	cur_alarm_cnt		= 0;
	
	gnItk_RF_Fwd		= 0;

	while(1)
	{
		if(WaitKill(10))	break;
	
		while(1)
		{
			if(Itk_BIAS_Mon.Read(nCs) == 1)
			{
				if(strcmp(strMsg, "BIAS_Fwd Mon Run") != 0)
				{
					sprintf(strMsg, "BIAS_Fwd Mon Run");
				}
				
				break;
			}
		
			if(WaitKill(100))	break;
		}


		do 
		{
			if( WaitKill(MONITORING_SLEEP) ) {	Itk_BIAS_Mon.Write(eOFF, nCs);	}

			dcfg_alarm_level	=		cBRfFwdAbort.Read(nCs);
			dcfg_warning_level	=		cBRfFwdWarn.Read(nCs);
			dStep_CurTime		=		Step_CurTime.Read(nCs);

			cfg_alarm_cnt		= (int)(cBRfFwdAbortCnt.Read(nCs));
			cfg_warning_cnt		= (int)(cBRfFwdWarnCnt.Read(nCs));
			cfg_stable_time		= (int)(cBRfFwdStableTime.Read(nCs)) * 10;

			if(CheckKill()) break;
			if(Itk_BIAS_Mon.Read(nCs) == 0)
			{
				cur_stable_time		= 0;
				cur_warning_cnt		= 0;
				cur_alarm_cnt		= 0;
				
				gnItk_RF_Fwd		= 0;
				
				if(WaitKill(100)) break;
				
				if(strcmp(strMsg, "BIAS_Fwd Mon Stopped") != 0)
				{
					sprintf(strMsg, "BIAS_Fwd Mon Stopped");
				}
				break;
			}


			if(CheckKill()) break;
			if(cur_stable_time <= cfg_stable_time)
			{
				if(cur_stable_time == cfg_stable_time)
				{
					sprintf(strMsg, "BIAS_FORWARD MONITORING START.[CONFIG DATA:: STABLE_TIME: %d sec, WARNING: %d Count, ALARM : %d Count]", cfg_stable_time/10, cfg_warning_cnt, cfg_alarm_cnt);
					Make_Log("[RF_PWRON]","%s \n",strMsg );	
				}
			
				cur_stable_time++;
				break;
			}

			if(dStep_CurTime < cBRfFwdStableTime.Read(nCs)) break;


			dActual		= BRF_FwdPwr_RA.Read(nCs);
			dSetpoint	= PR_BRFG_POWER.Read(nCs);

			if( dActual< 100 ) 
			{
				if(cur_alarm_cnt ==0)
				{
					sprintf(strMsg, "BIAS_FORWARD Power is under 100 V. AbortCount Change 5 Counts[StepTime: %.0f sec, Set: %.0f, RealData: %.0f] ",dStep_CurTime,dSetpoint, dActual);
					Make_Log("[RF_PWRON]","%s \n",strMsg );
				}

				cfg_alarm_cnt =5;
			}

			if(CheckKill()) break;
			if(dActual > (dSetpoint + dcfg_alarm_level) || dActual < (dSetpoint - dcfg_alarm_level))
			{
				cur_warning_cnt = 0;
				cur_alarm_cnt++;
				
				sprintf(strMsg, "BIAS_FORWARD Alarm Counting[%d Count][StepTime: %.0f sec, Set: %.0f, RealData: %.0f, AlarmLevel: %.0f] ",cur_alarm_cnt,dStep_CurTime,dSetpoint, dActual,dcfg_alarm_level);
				Make_Log("[RF_PWRON]","%s \n",strMsg );
				
			}
			else
			{
				cur_alarm_cnt = 0;
				gnItk_RF_Fwd  = 0;
				
				if(dActual > (dSetpoint + dcfg_warning_level) || dActual < (dSetpoint - dcfg_warning_level))
				{
					cur_warning_cnt++;	
				}
				else
				{
					cur_warning_cnt = 0;
				}
			}


			if(CheckKill()) break;
			if(cur_warning_cnt >= cfg_warning_cnt)
			{
				if(warning_id.Result() != ALM_POSTED)
				{
					warning_id.PostAnyTh();
					
					sprintf(szTemp, "BIAS_FORWARD Warning Post[StepTime: %.0f sec, Set: %.0f, RealData: %.0f, WarningLevel: %.0f] ",dStep_CurTime,dSetpoint, dActual, dcfg_warning_level);
					Make_Log("[RF_PWRON]","%s \n",strMsg );

				}
			}
			else if(cur_warning_cnt == 0)
			{
				if(warning_id.Result() == ALM_POSTED)
					warning_id.Clear();
			}


			if(cur_alarm_cnt	>= cfg_alarm_cnt)
			{
				if(CheckKill()) break;
				
				FNC_RF_CTRL.Run("PROC_RF_OFF_INTLK");				
				
				alarm_id.PostAnyTh();
				
				gnItk_RF_Fwd = 1;

				sprintf(strMsg, "BIAS_FORWARD Alarm Post[StepTime: %.0f sec, Set: %.0f, RealData: %.0f] ",dStep_CurTime,dSetpoint, dActual);
				Make_Log("[RF_PWRON]","%s \n",strMsg );				
				Make_Log("[RF_PWRON]","Process Run Abort. FNC_RF|POWEROFF \n");
							
				FNC_SRF.Abort();
				FNC_BRF.Abort();
	
							
				do
				{
					if(WaitKill(250)) break;
					
					alarm_result = alarm_id.Result();
					
					if(alarm_result == ALM_ABORT)
					{
						break;
					}
				}while(alarm_result != ALM_CLEAR);
				
			
				cur_stable_time		= 0;
				cur_alarm_cnt		= 0;
				cur_warning_cnt		= 0;
				
				warning_id.Clear();
				
				gnItk_RF_Fwd  = 0;
								
				break;

			} 
		} while(1);

	}

	_endthread();

}

void Fnc_Ref_Check(void *pDummy)
{
	int		nCs = 0;
	
	double	dSetpoint = 0.0;
	double	dActual   = 0.0;
	
	int     alarm_result = 0;
	
	char    szTemp[256] = {0};
	
	double	dcfg_warning_level	= 0.0;
	double	dcfg_alarm_level	= 0.0;
	
	double  dStep_CurTime;
	double  dHRF_RefValue, dHRF_FwdValue, dLRF_RefValue, dLRF_FwdValue;
	char    RecipeName[256]		= {0};
	char    CurStepName[256]	= {0};

	
	int		cfg_warning_cnt		= 0;
	int		cfg_alarm_cnt		= 0;
	int		cfg_stable_time		= 0;
	
	int		cur_stable_time		= 0;
	int		cur_warning_cnt		= 0;
	int		cur_alarm_cnt		= 0;
	
	CAlarm  alarm_id	= BRF_6462;
	CAlarm	warning_id	= BRF_6452;
	
	char	szTest[128] = {0};
	int		str_length   = 0;
		
	char	strMsg[256] = {0};
	
	cur_stable_time		= 0;
	cur_warning_cnt		= 0;
	cur_alarm_cnt		= 0;
	gnItk_RF_Ref		= 0;

	
	while(1)
	{
		if(WaitKill(10))	break;
		
		while(1)
		{
			if(Itk_BIAS_Mon.Read(nCs) == 1)
			{
				if(strcmp(strMsg, "BIAS_Ref Mon Run") != 0)
				{
					sprintf(strMsg, "BIAS_Ref Mon Run");
				}
				break;
			}
			if(WaitKill(100))	break;
		}


		do 
		{
			
			if( WaitKill(MONITORING_SLEEP) ) {	Itk_BIAS_Mon.Write(eOFF, nCs);	}

			dcfg_alarm_level	=		cBRfRefAbort.Read(nCs);
			dcfg_warning_level	=		cBRfRefWarn.Read(nCs);
			dStep_CurTime		=		Step_CurTime.Read(nCs);
		
			cfg_alarm_cnt		= (int)(cBRfRefAbortCnt.Read(nCs));
			cfg_warning_cnt		= (int)(cBRfRefWarnCnt.Read(nCs));
			cfg_stable_time		= (int)(cBRfRefStableTime.Read(nCs)) * 10;

			Rcp_Name.Read(RecipeName , nCs);
			Step_Name.Read(CurStepName , nCs);

			if(CheckKill()) break;
			if(Itk_BIAS_Mon.Read(nCs) == 0)
			{
				cur_stable_time		= 0;
				cur_warning_cnt		= 0;
				cur_alarm_cnt		= 0;
				
				gnItk_RF_Ref		= 0;
				
				if(WaitKill(100)) break;
				
				if(strcmp(strMsg, "BIAS_Ref Mon Stopped") != 0)
				{
					sprintf(strMsg, "BIAS_Ref Mon Stopped");
				}
				break;
			}

			if(cur_stable_time <= cfg_stable_time)
			{
				if(cur_stable_time == cfg_stable_time)
				{
					sprintf(strMsg, "BIAS_REFLECT MONITORING START.[CONFIG DATA:: STABLE_TIME: %d sec, WARNING: %d Count, ALARM : %d Count]", cfg_stable_time/10, cfg_warning_cnt, cfg_alarm_cnt);
					Make_Log("[RF_PWRON]","%s \n",strMsg );	
				}	
				
				cur_stable_time++;
				break;
			}

			if(dStep_CurTime < cBRfRefStableTime.Read(nCs)) break;


			if(CheckKill()) break;
			dActual	= BRF_RefPwr_RA.Read(nCs);

			dHRF_FwdValue	= SRF_FwdPwr_RA.Read(nCs);
			dHRF_RefValue	= SRF_RefPwr_RA.Read(nCs);
			dLRF_FwdValue	= BRF_FwdPwr_RA.Read(nCs);
			dLRF_RefValue	= BRF_RefPwr_RA.Read(nCs);

			
			if(dActual > dcfg_alarm_level)
			{
				cur_alarm_cnt++;

				sprintf(strMsg, "BIAS_REFLECT Alarm Counting[%d Count][B_FWD: %.0f][B_REF: %.0f][S_FWD: %.0f][S_REF: %.0f][StepTime: %.0f sec, AlarmLevel: %.0f]"
															,cur_alarm_cnt, dLRF_FwdValue,dLRF_RefValue,dHRF_FwdValue,dHRF_RefValue
															,dStep_CurTime,dcfg_alarm_level);
				Make_Log("[RF_PWRON]","%s \n",strMsg );
				
			}
			else
			{
				cur_alarm_cnt = 0;
								
				gnItk_RF_Ref = 0;
				
				if(dActual > dcfg_warning_level)	
				{
					cur_warning_cnt++;
				}
				
				else
				{
					cur_warning_cnt = 0;
				}
			}


			if(CheckKill()) break;
			if(cur_warning_cnt >= cfg_warning_cnt)
			{
				if(warning_id.Result() != ALM_POSTED)
				{
					warning_id.PostAnyTh();

					sprintf(strMsg, "BIAS_REFLECT Warning Post[B_FWD: %.0f][B_REF: %.0f][S_FWD: %.0f][S_REF: %.0f][StepTime: %.0f sec, WarningLevel: %.0f]"
																,dLRF_FwdValue,dLRF_RefValue,dHRF_FwdValue,dHRF_RefValue
																,dStep_CurTime,dcfg_warning_level);
					Make_Log("[RF_PWRON]","%s \n",strMsg );
				}
			}

			if(cur_alarm_cnt >= cfg_alarm_cnt)
			{			
				
				if(CheckKill()) break;
				FNC_RF_CTRL.Run("PROC_RF_OFF_INTLK");

				
				sprintf(szTemp, "Bias Reflect exceeded Abort(%s step, %.0fsec, Ref: %.0f/%.0f Fwd: %.0f/%.0f)",
															  CurStepName,dStep_CurTime,
															  dHRF_RefValue,dLRF_RefValue,dHRF_FwdValue,dLRF_FwdValue);
				alarm_id.PostAnyTh(szTemp);
		

				gnItk_RF_Ref = 1;


				sprintf(strMsg, "BIAS_REFLECT Alarm Post[B_FWD: %.0f][B_REF: %.0f][S_FWD: %.0f][S_REF: %.0f]"
															, dLRF_FwdValue,dLRF_RefValue,dHRF_FwdValue,dHRF_RefValue);
				Make_Log("[RF_PWRON]","%s \n",strMsg );			
				Make_Log("[RF_PWRON]","Process Run Abort. FNC_H/LRF|POWEROFF \n");

				FNC_SRF.Abort();
				FNC_BRF.Abort();
				
				
				do
				{
					if(WaitKill(250)) break;

					
					alarm_result = alarm_id.Result();
					
					if(alarm_result == ALM_ABORT)
					{
						break;
					}
				}while(alarm_result != ALM_CLEAR);
				
				cur_stable_time		= 0;
				cur_alarm_cnt		= 0;
				cur_warning_cnt		= 0;
				
				warning_id.Clear();
				
				gnItk_RF_Ref = 0;
				
				break;

			}

		} while(1);

	} 

	_endthread();

}

void Fnc_MaxRef_Check(void *pDummy)
{
	int		nCs = 0;
	
	double	dActual   = 0.0;
	
	int     alarm_result = 0;
	
	char    szTemp[256] = {0};
	
	double	dcfg_alarm_level	= 0.0;
	
	double  dStep_CurTime;
	double  dHRF_RefValue, dLRF_RefValue, dBiasVdcData;

	char    RecipeName[256]		= {0};
	char    CurStepName[256]	= {0};
	
	int		cfg_stable_time		= 0;	
	int		cur_stable_time		= 0;
	int		cur_alarm_cnt	 	= 0;
	
	CAlarm  alarm_id	= BRF_6465;
				
	cur_stable_time		= 0;
	gnItk_RF_Ref_Max	= 0;

	
	while(1)
	{
		if(WaitKill(10))	break;
		
		while(1)
		{
			if(Itk_BIAS_Mon.Read(nCs) == 1){	break;}
			if(WaitKill(100))	break;
		}


		do 
		{
			
			if( WaitKill(MONITORING_SLEEP) ) {	Itk_BIAS_Mon.Write(eOFF, nCs);	break; }

			dcfg_alarm_level	=		cMaxRefAbort.Read(nCs);
			dStep_CurTime		=		Step_CurTime.Read(nCs);
			cfg_stable_time		= (int)(cBRfRefStableTime.Read(nCs)) * 10;

			Rcp_Name.Read(RecipeName , nCs);
			Step_Name.Read(CurStepName , nCs);

			if(CheckKill()) break;
			if(Itk_BIAS_Mon.Read(nCs) == 0)
			{
				cur_stable_time		= 0;
				gnItk_RF_Ref_Max	= 0;
				_sleep(100);
				
				break;
			}

			if(cur_stable_time <= cfg_stable_time)
			{				
				cur_stable_time++;
				break;
			}

			if(dStep_CurTime < cBRfRefStableTime.Read(nCs)) break;


			if(CheckKill()) break;
		
			//KLP_EDIT
			// 아래 1개 항목 dActual 추가하는거 횡전개 해야할듯.dd
			dActual			= BRF_RefPwr_RA.Read(nCs);
			dHRF_RefValue	= SRF_RefPwr_RA.Read(nCs);
			dLRF_RefValue	= BRF_RefPwr_RA.Read(nCs);
			dBiasVdcData	= BRF_Vdc_RA.Read(nCs);
			if(dBiasVdcData < 0) dBiasVdcData = abs(dBiasVdcData);

			if(dActual > dcfg_alarm_level)
			{
				cur_alarm_cnt++;
				
				sprintf(szTemp, "BIAS_REFLECT_MAX Alarm Counting[%d Count][S_REF: %.0f][B_REF: %.0f][B_VDC: %.0f][StepTime: %.0f sec, AlarmLevel: %.0f]"
															,cur_alarm_cnt,dHRF_RefValue,dLRF_RefValue,BRF_Vdc_RA.Read(nCs),dStep_CurTime,dcfg_alarm_level);
															
				Make_Log("[RF_PWRON]","%s \n",szTemp );
				
			}
			else
			{
				cur_alarm_cnt	 = 0;				
				gnItk_RF_Ref_Max = 0;
			}


			if(cur_alarm_cnt >= 5)
			{			
				if(CheckKill()) break;
				FNC_RF_CTRL.Run("PROC_RF_OFF_INTLK");

				sprintf(szTemp, "Bias Reflect exceeded Max_Over_level(%s step, %.0fsec, Reflect: %.0f)",
															  CurStepName,dStep_CurTime,dActual);
															  
				alarm_id.PostAnyTh(szTemp);
		

				gnItk_RF_Ref_Max = 1;

				sprintf(szTemp, "BIAS_REFLECT_MAX Alarm Post[S_REF: %.0f][B_REF: %.0f]",dHRF_RefValue,dLRF_RefValue);
															
				Make_Log("[RF_PWRON]","%s \n",szTemp );			
				Make_Log("[RF_PWRON]","Process Run Abort. FNC_RF|POWEROFF \n");

				FNC_SRF.Abort();
				FNC_BRF.Abort();
				
				
				do
				{
					if(WaitKill(250)) break;
					
					alarm_result = alarm_id.Result();
					
					if(alarm_result == ALM_ABORT)
					{
						break;
					}

				}while(alarm_result != ALM_CLEAR);
				
				cur_stable_time		= 0;				
				gnItk_RF_Ref_Max	= 0;
				
				break;

			}

		} while(1);

	} 

	_endthread();

}

void Fnc_Ref_Check_Preset(void *pDummy)
{
	int		nCs = 0;
	
	char    szTemp[256]		= {0};
	char	strMsg[256]		= {0};
	char    RecipeName[256]		= {0};
	char    CurStepName[256]	= {0};


	double	dActual				= 0.0;
	double	dcfg_warning_level	= 0.0;
	double  dStep_CurTime		= 0.0;
	
	double	cfg_warning_cnt		= 0.0;
	double	cur_warning_cnt		= 0.0;
	CAlarm	warning_id	= BRF_6455;
	

	while(1)
	{
		if(WaitKill(10))	break;
		
		while(1)
		{
			if(Itk_BIAS_Mon.Read(nCs) == 1)
			{
				if(strcmp(strMsg, "BIAS_Ref_Preset Mon Run") != 0)
				{
					sprintf(strMsg, "BIAS_Ref_Preset Mon Run");
				}
				break;
			}
			if(WaitKill(100))	break;
		}

		do 
		{
			if(WaitKill(MONITORING_SLEEP)) { break; }
			
			dcfg_warning_level	=		cBRfRefAbort.Read(nCs);
			cfg_warning_cnt		=		cBRfRefAbortCnt.Read(nCs);
			dStep_CurTime		=		Step_CurTime.Read(nCs);

			Rcp_Name.Read(RecipeName , nCs);
			Step_Name.Read(CurStepName , nCs);

		
			if(CheckKill()) break;
			if(Itk_BIAS_Mon.Read(nCs) == 0)
			{
				cur_warning_cnt		= 0;
				
				if(WaitKill(100)) break;
				
				if(strcmp(strMsg, "BIAS_Ref Mon Stopped") != 0)
				{
					sprintf(strMsg, "BIAS_Ref Mon Stopped");
				}
				break;
			}


			if(dStep_CurTime >4)
			{

				if(CheckKill()) break;
				dActual	= BRF_RefPwr_RA.Read(nCs);
				
				
				if(dActual > dcfg_warning_level) 	
				{
					cur_warning_cnt++;
				}
				else
				{
					cur_warning_cnt = 0;
				}
				
				
				if(cur_warning_cnt >= cfg_warning_cnt)
				{
					if(warning_id.Result() != ALM_POSTED)
					{
						sprintf(szTemp, "Bias initial Reflect Level is high. Check Preset(Step:%s)",CurStepName);
						warning_id.PostAnyTh(szTemp);

						sprintf(strMsg, "BIAS INITIAL REFLECT LEVEL CHECK Warning Post[StepTime: %.0f sec, Reflect: %.0f]",dStep_CurTime,dActual);
						Make_Log("[RF_PWRON]","%s \n",strMsg );

					}
						
				}	
				else if(cur_warning_cnt == 0)
				{
					if(warning_id.Status() == ALM_POSTED)
						warning_id.Clear();
				}		

			}
			
			if(cur_warning_cnt == 0)
			{
				if(warning_id.Result() == ALM_POSTED)	warning_id.Clear();
			}


		} while(1);

	} 


	_endthread();

}

int	Software_Interlock_Monitoring()
{
	int nItk_RF;
	int ncs = 0;
	
	int Logging =0;
	
	while(1)
	{
		nItk_RF		= gnItk_RF_Fwd + gnItk_RF_Ref + gnItk_RF_Ref_Max + gnItk_RF_Vpp + gnItk_RF_Vdc + gnItk_RF_PLC + gnItk_RF_Dechuck + gnItk_RF_Util + gnItk_RF_ArcCount;

		if((nItk_RF > 0))	
		{
			Logging++;

			if(Logging ==1 )
			{
				Make_Log("[RF_OFFRUN]", "Software_Intlk_Sts3_SRF[FWD: %d][REF: %d][REF_MAX: %d][VPP: %d][VDC: %d][PLC: %d][LOW: %d][UTIL: %d][ARC: %d] is On \n",
							gnItk_RF_Fwd,gnItk_RF_Ref,gnItk_RF_Ref_Max,gnItk_RF_Vpp,gnItk_RF_Vdc,gnItk_RF_PLC,gnItk_RF_Dechuck,gnItk_RF_Util,gnItk_RF_ArcCount);
			}
			dSoftware_Intlk_Sts4.Write(1 , ncs);
		}
		else				 
		{
			Logging=0;
			dSoftware_Intlk_Sts4.Write(0 , ncs);
		}

		if(WaitKill(50))	break;
	 }
	return 1;
}

void MON_SW_INTERLOCK(void *pDummy)
{
	Software_Interlock_Monitoring();  
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
SEQ_STATUS SeqMain(const char* szCommand, const void* pParam, int nNumOfBytesRecved)
{
	printf("-----------> [ %s ] START... [%s]\n", GetSeqName(), szCommand);

	int Red = 0;
	int AlmSts = 0;
	int ncs = 0;
	if(SIMULATION_MODE)	PM_Start_Flag.Write(PM_Started);
	
	while(1)
	{
		if(PM_Starting == PM_Start_Flag.Read(ncs))
		{
			if(WaitKill(1000))	break;
		}
		else
		{
			if(WaitKill(5000))	break;
			break;
		}
	}

	_beginthread(Fnc_Fwd_Check,				0, NULL);	_sleep(100);
	_beginthread(Fnc_Ref_Check,				0, NULL);	_sleep(100);
	_beginthread(Fnc_MaxRef_Check,			0, NULL);	_sleep(100);
	_beginthread(Fnc_Ref_Check_Preset,		0, NULL);	_sleep(100);
	_beginthread(Fnc_Vdc_Check,				0, NULL);	_sleep(100);

	_beginthread(MON_SW_INTERLOCK,			0, NULL);	_sleep(100);


	while(TRUE)
	{
		if(WaitKill(2000))	break;
	}
	
	_sleep(2000);

	return SEQ_SUCCESS;
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
	if ( SIMULATION_CONTROL.Read(CommStatus) == eON ) SIMULATION_MODE = TRUE;
	else											  SIMULATION_MODE = FALSE;

	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnUnLoad( int nCode ) 
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnEventRecv( const char* szCommand, const void* pParam , int nNumOfBytesRecved ) {
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
