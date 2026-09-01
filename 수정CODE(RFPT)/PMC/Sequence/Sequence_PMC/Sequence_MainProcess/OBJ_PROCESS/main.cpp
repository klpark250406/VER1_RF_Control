#include <windows.h>
#include <stdio.h>
#include <process.h>
#include <direct.h>
#include <time.h>
#include <tMain.h>
#include <TUtilStr.h>
#include <TTimer.h>
#include <TRecipeAnal.h>
#include <TSchDataIntf.h>
#include "iodefine.h"
#include "VersionInfo.h"

#pragma once
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------

#define		MAX_STEPNO_COUNT		32
#define		MAX_GAS_COUNT			10
#define		MAX_ANALOG_PIN_POS		100
#define		FILE_SIZE				5000000	

#define		__RETRY					1
#define		__NEXT					2
#define		__STEPJUMP				4
#define		__RUN					8
#define		__ABORT				   -1

#define		__RESULT_IS_ABORTED		-1
#define		__RESULT_IS_NORMAL		1

#define		__CHECK_RANGE		   -1
#define		__GO_STEP				1

#define		__PROC_ABORT		   -1
#define		__PROC_PAUSE			0
#define		__PROC_GOING			1
#define		__PROC_DONE				2
#define		__PROC_SKIP				4

#define		__STEP_ALARM_ABORTED   -2
#define		__STEP_ABORT		   -1
#define		__STEP_NONE				0
#define		__STEP_RETRY			1
#define		__STEP_DONE				2
#define		__STEP_SKIP				4
#define		__STEP_PAUSE			8
#define		__STEP_JUMP             16

#define		ALARM_STEP_APC_FAIL		1100
#define		ALARM_STEP_MFC_FAIL		1101
#define		ALARM_STEP_ESC_FAIL		1102
#define		ALARM_STEP_HE_FAIL		1103
#define		ALARM_STEP_SRF_FAIL		1104
#define		ALARM_STEP_BRF_FAIL		1105
#define		ALARM_STEP_PIN_FAIL		1106
#define		ALARM_STEP_TIMEOUT		1000
#define		ALARM_STEP_INTERLOCKED	1001

//----------------------------------------------------------------------------------------------------------------------------------------------

enum {FAULT = -1, EVENT = 1};
enum {OFF     , ON    };
enum {STEP_IDLE , STEP_IGNORE   , STEP_STOP  , STEP_DISABLE };
enum {eOffline = 0, eOnline = 1};
enum {eTYPE_NONE, eTYPE_COND, eTYPE_TIME, eTYPE_DISCH, eTYPE_FINISH, eTYPE_EPD};
enum {eSTEPTYPE_NONE, eSTEPTYPE_CHUCK_ST, eSTEPTYPE_CHUCK_ED, eSTEPTYPE_PROCESS, eSTEPTYPE_DECHUCK_ST, eSTEPTYPE_DECHUCK_ED};
enum {eTYPE_PINPOS_2, eTYPE_PINPOS_3, eTYPE_PINPOS_4};
enum {eTYPE_VVCSET_1, eTYPE_VVCSET_2, eTYPE_VVCSET_3, eTYPE_VVC_NOUSE};
enum {ePROC_IDLE, ePROC_RUN, ePROC_ABORT};
enum {eEPD_UNKNOWN, eEPD_STOPPED, eEPD_STARTED, eEPD_DETECTED, eEPD_NOT_DETECTED};
enum {eEPD_CMD_NONE, eEPD_CMD_RST,eEPD_CMD_START, eEPD_CMD_STOP, eEPD_CMD_RCPINFO, eEPD_CMD_RCPNO };
enum {LOG_DISABLE, LOG_ENABLE};
enum {ePROCESS_NORMAL, ePROCESS_AGING, ePROCESS_ONLY_TRANSFER};
enum {RAMP_DISABLE,RAMP_ENABLE};
enum {PinCond_DIO, PinCond_AIO, PinCond_BOTH};					
enum {DischargeStep_CheckPinPos, DischargeStep_CheckTime};			
enum {EPDUnderAlarm_Use, EPDUnderAlarm_NoUse};						
enum {eCon_IDLE,  eCon_Start,   eCon_End,  eCon_Fail};	
enum {eNoCheck, eYesCheck};
enum {HeNoAlarm,	HeYesAlarm};
enum {NoAction,		AutoDecuck};
enum {eNoReady,    eReady};
enum {EmP_Idle,	 EmP_Run, EmP_Error, EmP_Wait, EmP_Stop};
enum {CountPro_Idle, CountPro_Run, CountPro_Error, CountPro_Wait, CountPro_Stop};
enum {IDLE_Process, Normal_Process1};
enum {Count_Process, Normal_Process2};
enum {MDL_OFF,	MDL_ENABLE,	MDL_DISABLESW, 	MDL_ENABLESW };
enum {Mon_Dis, Mon_En};
enum {ePIN_DIO_CHECK, ePIN_AIO_CHECK, ePIN_BOTH_CHECK};
enum {eXI_VLV_UNKNOWN, eXI_VLV_OPEN, eXI_VLV_CLOSE, eXI_VLV_ERROR};
enum {eINTERLOCK_CHECK_OFF, eINTERLOCK_CHECK_ON};
enum {ePOPUP_NONE, ePOPUP_STEPJUMP, ePOPUP_RUN, ePOPUP_ABORT, ePOPUP_RETRY, ePOPUP_NEXT}; 
enum {eFNC_IDLE, eFNC_RUNNING, eFNC_ABORTED};
enum {eVLV_CLOSE, eVLV_OPEN};
enum {GLS_NONE, GLS_UNKNOWN1, GLS_PRESENT, GLS_SUCCESS, GLS_FAILURE, GLS_PROCESSING, GLS_UNKNOWN2};
enum {IDLE, NOMAL_RUN, IDLE_RUN, COUNT_RUN};
enum {MccEnd,MccStart};
enum {MccNormalProcess, MccChangeProcess, MccCleaningProcess};
enum {EQ_Normal,EQ_Fault, EQ_PM};
enum {Option_NoUse,	Option_Use};
enum {C_Heater_ON,C_Heater_OFF};
enum {eNG,eOK};

enum { ePORT1, ePORT2, ePORT3, ePORT4, ePORT5	};	//20171010 crkim
enum { ePM1  , ePM2  , ePM3  , ePM4  , ePM5		};
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_OBJECT_ENUMERATION

	CDIO SIMULATION_CONTROL					 ( _TEXT( "System.SimulMode"	));

	CAIO 	PR_GAS1 						 ( _TEXT( "PR_GAS1"				));
	CAIO 	PR_GAS2							 ( _TEXT( "PR_GAS2"				));
	CAIO 	PR_GAS3							 ( _TEXT( "PR_GAS3"				));
	CAIO 	PR_GAS4							 ( _TEXT( "PR_GAS4"				));
	CAIO 	PR_GAS5							 ( _TEXT( "PR_GAS5"				));
	CAIO 	PR_GAS6							 ( _TEXT( "PR_GAS6"				));
	CAIO 	PR_GAS7							 ( _TEXT( "PR_GAS7"				));
	CAIO 	PR_GAS8							 ( _TEXT( "PR_GAS8"				));
	CAIO 	PR_GAS9							 ( _TEXT( "PR_GAS9"				));
	CAIO 	PR_GAS10						 ( _TEXT( "PR_GAS10"			));

	CAIO	PR_FRC_MIDDLE					 ( _TEXT( "PR_FRC_MIDDLE"		));		
	CAIO	PR_FRC_CENTER					 ( _TEXT( "PR_FRC_CENTER"		));		

	CDIO 	PR_APC_CTRL						 ( _TEXT( "PR_APC_CTRL"			));
	CAIO 	PR_APC_PRESS					 ( _TEXT( "PR_APC_PRESS"		));

	CAIO 	PR_SRFG_POWER					 ( _TEXT( "PR_SRFG_POWER"		));
	CAIO 	PR_SRFM_MATCH					 ( _TEXT( "PR_SRFM_MATCH"		));
	CAIO 	PR_SRFM_TUNE					 ( _TEXT( "PR_SRFM_TUNE"		));

	CAIO 	PR_BRFG_POWER					 ( _TEXT( "PR_BRFG_POWER"		));
	CAIO 	PR_BRFM_MATCH					 ( _TEXT( "PR_BRFM_MATCH"		));
	CAIO 	PR_BRFM_TUNE					 ( _TEXT( "PR_BRFM_TUNE"		));
	CDIO 	PR_VVC_MOTOR					 ( _TEXT( "PR_VVC_MOTOR"		));

	CAIO 	PR_HE_PRESS						 ( _TEXT( "PR_HE_PRESS"			));
	CAIO 	PR_ESC_VOLT						 ( _TEXT( "PR_ESC_VOLT"			));

	CDIO 	PR_ETCHSTEP						 ( _TEXT( "PR_ETCHSTEP"			));
	CDIO 	PR_STEPENDTYPE					 ( _TEXT( "PR_STEPENDTYPE"		));

	CAIO 	PR_EPD_RECIPENUM				 ( _TEXT( "PR_EPD_RECIPENUM"	));
	CAIO 	PR_EPD_MIN_TIME					 ( _TEXT( "PR_EPD_MIN_TIME"		));

	CAIO 	PR_EPD_STEPNUM					 ( _TEXT( "PR_EPD_STEPNUM"		));
	CDIO 	PR_PIN_POS						 ( _TEXT( "PR_PIN_POS"			));

	CDIO 	PR_EPD_ALARM					 ( _TEXT( "PR_EPD_ALARM"		));
	CDIO 	PR_OVERETCH_TYPE				 ( _TEXT( "PR_OVERETCH_TYPE"	));
	CAIO 	PR_OVERETCH_VALUE				 ( _TEXT( "PR_OVERETCH_VALUE"	));
	
	CAIO 	PR_HE_FLOW_ALARM				 ( _TEXT( "PR_HE_FLOW_ALARM"	));
	CAIO 	PR_VVC01_POS					 ( _TEXT( "PR_VVC01_POS"		));
	CAIO 	PR_VVC02_POS					 ( _TEXT( "PR_VVC02_POS"		));
	CAIO 	PR_VVC03_POS					 ( _TEXT( "PR_VVC03_POS"		));
	CAIO 	PR_VVC04_POS					 ( _TEXT( "PR_VVC04_POS"		));
	CAIO 	PR_VVC05_POS					 ( _TEXT( "PR_VVC05_POS"		));
	CAIO 	PR_VVC06_POS					 ( _TEXT( "PR_VVC06_POS"		));

	CAIO 	PR_CHILLER_CH01_TEMP			 ( _TEXT( "PR_CHILLER_CH01_TEMP"));
	CAIO 	PR_CHILLER_CH02_TEMP			 ( _TEXT( "PR_CHILLER_CH02_TEMP"));
	CAIO 	PR_CHILLER_CH03_TEMP			 ( _TEXT( "PR_CHILLER_CH03_TEMP"));

	CSIO 	Recipe_Name						 ( _TEXT( "Recipe_Name"			));
	CSIO 	Rcp_Name						 ( _TEXT( "Rcp_Name"			));
	CAIO 	Rcp_TotalStep					 ( _TEXT( "Rcp_TotalStep"		));
	CAIO 	Rcp_TotalTime					 ( _TEXT( "Rcp_TotalTime"		));

	CSIO 	Step_Name						 ( _TEXT( "Step_Name"			));
	CAIO 	Step_Time						 ( _TEXT( "Step_Time"			));
	CDIO 	Step_EndType					 ( _TEXT( "Step_EndType"		));

	CAIO 	Rcp_CurStepNum					 ( _TEXT( "Rcp_CurStepNum"		));
	CAIO 	Rcp_CurTime						 ( _TEXT( "Rcp_CurTime"			));
	CAIO 	Step_CurTime					 ( _TEXT( "Step_CurTime"		));

	CAIO 	FNC_PROC_AddTime				 ( _TEXT( "FNC_PROC_AddTime"	));
	CAIO 	FNC_PROC_JumpStep				 ( _TEXT( "FNC_PROC_JumpStep"	));
	CAIO 	FNC_PROC_RemainTime				 ( _TEXT( "FNC_PROC_RemainTime"	));
	CAIO 	FNC_PROC_AbortStep				 ( _TEXT( "FNC_PROC_AbortStep"	));
	
	CDIO 	cHE_Monitor_ME					 ( _TEXT( "cHE_Monitor_ME"		));
	CDIO 	cESC_Monitor_ME					 ( _TEXT( "cESC_Monitor_ME"		));

	CAIO 	PM_Pressure						 ( _TEXT( "PM_Pressure"			));
	CDIO 	APC_RUN_NGOK					 ( _TEXT( "APC_RUN_NGOK"		));

	CDIO 	PM_PROC_CTRL					 ( _TEXT( "PM_PROC_CTRL"		));
	CDIO 	PM_PROC_STS						 ( _TEXT( "PM_PROC_STS"			));

	CDIO 	PM_PROC_RFCTRL					 ( _TEXT( "PM_PROC_RFCTRL"		));

	CDIO 	PM_PROCRUN_MODE					 ( _TEXT( "PM_PROCRUN_MODE"		));
	CAIO 	PM_AGING_COUNTS				     ( _TEXT( "PM_AGING_COUNTS"		));
	CAIO 	PM_AGING_STABLETIME			     ( _TEXT( "PM_AGING_STABLETIME"	));

	CDIO 	PM_GLASS_STS					 ( _TEXT( "PM_GLASS_STS"		));

	CDIO 	dSoftware_Intlk_Sts1			 ( _TEXT( "dSoftware_Intlk_Sts1"));
	CDIO 	dSoftware_Intlk_Sts2			 ( _TEXT( "dSoftware_Intlk_Sts2"));
	CDIO 	dSoftware_Intlk_Sts3			 ( _TEXT( "dSoftware_Intlk_Sts3"));
	CDIO 	dSoftware_Intlk_Sts4			 ( _TEXT( "dSoftware_Intlk_Sts4"));
	CDIO 	dSoftware_Intlk_Sts5			 ( _TEXT( "dSoftware_Intlk_Sts5"));
	CDIO 	dSoftware_Intlk_Sts6			 ( _TEXT( "dSoftware_Intlk_Sts6"));
	CDIO 	dSystem_Intlk_Sts				 ( _TEXT( "dSystem_Intlk_Sts"	));
	CDIO 	dIntlk_Chk_Disable				 ( _TEXT( "dIntlk_Chk_Disable"	));
	CDIO 	dSystem_Intlk_Monitoring		 ( _TEXT( "dSystem_Intlk_Monitoring"));

	CSIO 	sGUI_Step_msg1					 ( _TEXT( "sGUI_Step_msg1"		));
	CSIO 	sGUI_Step_msg2					 ( _TEXT( "sGUI_Step_msg2"		));

	CSIO 	sGUI_Proc_msg					 ( _TEXT( "sGUI_Proc_msg"		));
	
	CDIO 	DO_PROCESS_GAS_SUPPLY_VALVE		 ( _TEXT("DO_PROCESS_GAS_SUPPLY_VALVE"	));
	CDIO 	DO_IG_SHUT_OFF_VALVE			 ( _TEXT("DO_IG_SHUT_OFF_VALVE"			));
	CDIO 	DO_IG_ONOFF						 ( _TEXT("DO_IG_ONOFF"					));
	CDIO 	DO_CDG_SHUT_OFF_VALVE			 ( _TEXT("DO_CDG_SHUT_OFF_VALVE"		));
	CDIO    DO_ESCHE_OUTLET_VALVE			 ( _TEXT("DO_ESCHE_OUTLET_VALVE"		));  

	CAIO 	eAI_APC_Position				 ( _TEXT( "eAPC_Position_RA"		));
	CAIO 	eAI_APC_Sensor1					 ( _TEXT( "eAPC_Sensor1_RA"			));
	CAIO 	eAI_APC_Sensor2				     ( _TEXT( "eAPC_Sensor2_RA"			));

	//KLP_EDIT
	// RF ON Check
	CDIO 	eDI_SRF_GenRFOn			 ( _TEXT( "eDI_SRF_GenRFOn"	));
	CDIO 	eDI_BRF_GenRFOn			 ( _TEXT( "eDI_BRF_GenRFOn"	));


	CDIO 	eDO_SRFG_RFPower				 ( _TEXT( "eDO_SRFG_RFPower"		));
	CDIO 	eDO_BRFG_RFPower				 ( _TEXT( "eDO_BRFG_RFPower"		));
	CDIO 	DI_SOURCE_RFG_POWER_ON			 ( _TEXT( "DI_SOURCE_RFG_POWER_ON"	));
	CDIO 	DI_BIAS_RFG_POWER_ON			 ( _TEXT( "DI_BIAS_RFG_POWER_ON"	));
	CDIO 	DO_SOURCE_RFG_POWER_ON			 ( _TEXT( "DO_SOURCE_RFG_POWER_ON"	));
	CDIO 	DO_BIAS_RFG_POWER_ON			 ( _TEXT( "DO_BIAS_RFG_POWER_ON"	));
	CDIO 	rDO_SOURCE_RFG_POWER_ON			 ( _TEXT( "rDO_SOURCE_RFG_POWER_ON"	));
	CDIO 	rDO_BIAS_RFG_POWER_ON			 ( _TEXT( "rDO_BIAS_RFG_POWER_ON"	));

	CAIO 	eAI_SRF_MAT_MatchPos			 ( _TEXT( "eAI_SRF_MAT_MatchPos"	));
	CAIO 	eAI_SRF_MAT_TunePos				 ( _TEXT( "eAI_SRF_MAT_TunePos"		));
	CAIO 	eAI_BRF_MAT_MatchPos			 ( _TEXT( "eAI_BRF_MAT_MatchPos"	));
	CAIO 	eAI_BRF_MAT_TunePos				 ( _TEXT( "eAI_BRF_MAT_TunePos"		));

	CDIO 	XI_TM_GateVlv_Sns				 ( _TEXT( "XI_TM_GateVlv_Sns"		));
	CDIO 	DI_Pin_Busy						 ( _TEXT( "DI_Pin_Busy"				));
	CDIO 	DO_Pin_EMGStop					 ( _TEXT( "DO_Pin_EMGStop"			));

	CDIO 	Itk_SRC_Mon						 ( _TEXT( "Itk_SRC_Mon"				));
	CDIO 	Itk_BIAS_Mon					 ( _TEXT( "Itk_BIAS_Mon"			));

	CAIO 	eAI_SRFG_FwdPwr					 ( _TEXT( "eAI_SRF_FwdPwr"			));
	CAIO 	eAI_BRFG_FwdPwr					 ( _TEXT( "eAI_BRF_FwdPwr"			));

	CAIO 	eAO_TotalEtchTime				 ( _TEXT( "eAO_TotalEtchTime"		));
	CAIO 	eAO_JustEtchTime				 ( _TEXT( "eAO_JustEtchTime"		));

	CDIO 	eDI_EPD_Error					 ( _TEXT( "eDI_EPD_Error"		));
	CDIO 	eDI_EPD_EndPoint				 ( _TEXT( "eDI_EPD_EndPoint"	));
	CDIO 	eDO_EPD_Cmd					 	 ( _TEXT( "eDO_EPD_Cmd"			));

	CAIO 	eAI_EPD_JustEtchTime			 ( _TEXT( "eAI_EPD_JustEtchTime"	));
	CAIO 	eAI_EPD_TotalEtchTime			 ( _TEXT( "eAI_EPD_TotalEtchTime"	));
	CAIO 	eAI_EPD_OverEtchTime			 ( _TEXT( "eAI_EPD_OverEtchTime"	));
	CAIO 	eAI_EPD_EndPointStep			 ( _TEXT( "eAI_EPD_EndPointStep"	));

	CAIO 	eAO_EPD_RecipeNum				 ( _TEXT( "eAO_EPD_RecipeNum"		));
	CAIO 	eAO_EPD_EQPStepNum				 ( _TEXT( "eAO_EPD_EQPStepNum"		));
	CAIO 	eAO_EPD_EPDStepNum				 ( _TEXT( "eAO_EPD_EPDStepNum"		));

	CSIO 	eSO_EPD_DevId					 ( _TEXT( "eSO_EPD_DevId"			));
	CSIO 	eSO_EPD_StepName				 ( _TEXT( "eSO_EPD_StepName"		));
	CSIO 	eSO_EPD_BatchId				 	 ( _TEXT( "eSO_EPD_BatchId"			));

	CSIO 	eSI_EPD_DevId					 ( _TEXT( "eSI_EPD_DevId"			));
	CSIO 	eSI_EPD_StepName				 ( _TEXT( "eSI_EPD_StepName"		));
	CSIO 	eSI_EPD_BatchId				 	 ( _TEXT( "eSI_EPD_BatchId"			));
	CSIO 	eSI_EPD_GlassName				 ( _TEXT( "eSI_EPD_GlassName"		));
	CSIO 	eSI_EPD_RcpName				 	 ( _TEXT( "eSI_EPD_RcpName"			));
	CSIO 	eSI_EPD_ErrorMsg				 ( _TEXT( "eSI_EPD_ErrorMsg"		));

	CSIO 	PM_ModuleName					 ( _TEXT( "PM_ModuleName"			));
	CSIO 	GLASS_ID						 ( _TEXT( "GLASS_ID"				));
	CSIO 	FRONT_GLASS_ID					 ( _TEXT( "FRONT_GLASS_ID"			));
	CSIO 	REAR_GLASS_ID					 ( _TEXT( "REAR_GLASS_ID"			));

	CSIO 	EQ_BATCH_ID						 ( _TEXT( "EQ_BATCH_ID"				));
	CSIO 	eSO_EPD_LotID					 ( _TEXT( "eSO_EPD_LotID"			));
	CSIO 	eSO_EPD_PortID					 ( _TEXT( "eSO_EPD_PortID"			));
	CSIO 	eSO_EPD_SlotID					 ( _TEXT( "eSO_EPD_SlotID"			));
	CSIO 	eSO_EPD_GlassName				 ( _TEXT( "eSO_EPD_GlassName"		));
	CSIO 	eSO_EPD_RcpName					 ( _TEXT( "eSO_EPD_RcpName"			));
	CSIO 	eSO_EPD_GlassName2				 ( _TEXT( "eSO_EPD_GlassName2"		));

	CSIO 	LOT_ID							 ( _TEXT( "LOT_ID"					));
	CSIO 	PORT_ID							 ( _TEXT( "PORT_ID"					));
	CSIO 	SLOT_ID							 ( _TEXT( "SLOT_ID"					));

	CDIO 	cDI_ProcEnd_PinCond				 ( _TEXT( "cDI_ProcEnd_PinCond"		));
	CDIO 	cDI_ProcEnd_dPinPos				 ( _TEXT( "cDI_ProcEnd_dPinPos"		));
	CAIO 	cAI_ProcEnd_aPinPos				 ( _TEXT( "cAI_ProcEnd_aPinPos"		));

	CDIO 	XI_Pin_CurPosition				 ( _TEXT( "XI_Pin_CurPosition"		));
	CAIO 	AI_CPin1Pos					     ( _TEXT( "AI_CPin1Pos"				));

	CDIO 	cDI_DischargeStep_Cond			 ( _TEXT( "cDI_DischargeStep_Cond"	));

	CDIO 	exStepNum_01					 ( _TEXT( "exStepNum_01"			));
	CDIO 	exStepNum_02					 ( _TEXT( "exStepNum_02"			));
	CDIO 	exStepNum_03					 ( _TEXT( "exStepNum_03"			));
	CDIO 	exStepNum_04					 ( _TEXT( "exStepNum_04"			));
	CDIO 	exStepNum_05					 ( _TEXT( "exStepNum_05"			));
	CDIO 	exStepNum_06					 ( _TEXT( "exStepNum_06"			));
	CDIO 	exStepNum_07					 ( _TEXT( "exStepNum_07"			));
	CDIO 	exStepNum_08					 ( _TEXT( "exStepNum_08"			));
	CDIO 	exStepNum_09					 ( _TEXT( "exStepNum_09"			));
	CDIO 	exStepNum_10					 ( _TEXT( "exStepNum_10"			));
	CDIO 	exStepNum_11					 ( _TEXT( "exStepNum_11"			));
	CDIO 	exStepNum_12					 ( _TEXT( "exStepNum_12"			));
	CDIO 	exStepNum_13					 ( _TEXT( "exStepNum_13"			));
	CDIO 	exStepNum_14					 ( _TEXT( "exStepNum_14"			));
	CDIO 	exStepNum_15					 ( _TEXT( "exStepNum_15"			));
	CDIO 	exStepNum_16					 ( _TEXT( "exStepNum_16"			));
	CDIO 	exStepNum_17					 ( _TEXT( "exStepNum_17"			));
	CDIO 	exStepNum_18					 ( _TEXT( "exStepNum_18"			));
	CDIO 	exStepNum_19					 ( _TEXT( "exStepNum_19"			));
	CDIO 	exStepNum_20					 ( _TEXT( "exStepNum_20"			));
	CDIO 	exStepNum_21					 ( _TEXT( "exStepNum_21"			));
	CDIO 	exStepNum_22					 ( _TEXT( "exStepNum_22"			));
	CDIO 	exStepNum_23					 ( _TEXT( "exStepNum_23"			));
	CDIO 	exStepNum_24					 ( _TEXT( "exStepNum_24"			));
	CDIO 	exStepNum_25					 ( _TEXT( "exStepNum_25"			));
	CDIO 	exStepNum_26					 ( _TEXT( "exStepNum_26"			));
	CDIO 	exStepNum_27					 ( _TEXT( "exStepNum_27"			));
	CDIO 	exStepNum_28					 ( _TEXT( "exStepNum_28"			));
	CDIO 	exStepNum_29					 ( _TEXT( "exStepNum_29"			));
	CDIO 	exStepNum_30					 ( _TEXT( "exStepNum_30"			));
	CDIO 	exStepNum_31					 ( _TEXT( "exStepNum_31"			));
	CDIO 	exStepNum_32					 ( _TEXT( "exStepNum_32"			));

	CSIO 	exStepName_01			 		 ( _TEXT( "exStepName_01"			));
	CSIO 	exStepName_02			 		 ( _TEXT( "exStepName_02"			));
	CSIO 	exStepName_03			 		 ( _TEXT( "exStepName_03"			));
	CSIO 	exStepName_04			 		 ( _TEXT( "exStepName_04"			));
	CSIO 	exStepName_05			 		 ( _TEXT( "exStepName_05"			));
	CSIO 	exStepName_06			 		 ( _TEXT( "exStepName_06"			));
	CSIO 	exStepName_07			 		 ( _TEXT( "exStepName_07"			));
	CSIO 	exStepName_08			 		 ( _TEXT( "exStepName_08"			));
	CSIO 	exStepName_09			 		 ( _TEXT( "exStepName_09"			));
	CSIO 	exStepName_10			 		 ( _TEXT( "exStepName_10"			));
	CSIO 	exStepName_11			 		 ( _TEXT( "exStepName_11"			));
	CSIO 	exStepName_12			 		 ( _TEXT( "exStepName_12"			));
	CSIO 	exStepName_13			 		 ( _TEXT( "exStepName_13"			));
	CSIO 	exStepName_14			 		 ( _TEXT( "exStepName_14"			));
	CSIO 	exStepName_15			 		 ( _TEXT( "exStepName_15"			));
	CSIO 	exStepName_16			 		 ( _TEXT( "exStepName_16"			));
	CSIO 	exStepName_17			 		 ( _TEXT( "exStepName_17"			));
	CSIO 	exStepName_18			 		 ( _TEXT( "exStepName_18"			));
	CSIO 	exStepName_19			 		 ( _TEXT( "exStepName_19"			));
	CSIO 	exStepName_20			 		 ( _TEXT( "exStepName_20"			));
	CSIO 	exStepName_21			 		 ( _TEXT( "exStepName_21"			));
	CSIO 	exStepName_22			 		 ( _TEXT( "exStepName_22"			));
	CSIO 	exStepName_23			 		 ( _TEXT( "exStepName_23"			));
	CSIO 	exStepName_24			 		 ( _TEXT( "exStepName_24"			));
	CSIO 	exStepName_25			 		 ( _TEXT( "exStepName_25"			));
	CSIO 	exStepName_26			 		 ( _TEXT( "exStepName_26"			));
	CSIO 	exStepName_27			 		 ( _TEXT( "exStepName_27"			));
	CSIO 	exStepName_28			 		 ( _TEXT( "exStepName_28"			));
	CSIO 	exStepName_29			 		 ( _TEXT( "exStepName_29"			));
	CSIO 	exStepName_30			 		 ( _TEXT( "exStepName_30"			));
	CSIO 	exStepName_31			 		 ( _TEXT( "exStepName_31"			));
	CSIO 	exStepName_32			 		 ( _TEXT( "exStepName_32"			));

	CDIO 	exStepType_01			 		 ( _TEXT( "exStepType_01"			));
	CDIO 	exStepType_02			 		 ( _TEXT( "exStepType_02"			));
	CDIO 	exStepType_03			 		 ( _TEXT( "exStepType_03"			));
	CDIO 	exStepType_04			 		 ( _TEXT( "exStepType_04"			));
	CDIO 	exStepType_05			 		 ( _TEXT( "exStepType_05"			));
	CDIO 	exStepType_06			 		 ( _TEXT( "exStepType_06"			));
	CDIO 	exStepType_07			 		 ( _TEXT( "exStepType_07"			));
	CDIO 	exStepType_08			 		 ( _TEXT( "exStepType_08"			));
	CDIO 	exStepType_09			 		 ( _TEXT( "exStepType_09"			));
	CDIO 	exStepType_10			 		 ( _TEXT( "exStepType_10"			));
	CDIO 	exStepType_11			 		 ( _TEXT( "exStepType_11"			));
	CDIO 	exStepType_12			 		 ( _TEXT( "exStepType_12"			));
	CDIO 	exStepType_13			 		 ( _TEXT( "exStepType_13"			));
	CDIO 	exStepType_14			 		 ( _TEXT( "exStepType_14"			));
	CDIO 	exStepType_15			 		 ( _TEXT( "exStepType_15"			));
	CDIO 	exStepType_16			 		 ( _TEXT( "exStepType_16"			));
	CDIO 	exStepType_17			 		 ( _TEXT( "exStepType_17"			));
	CDIO 	exStepType_18			 		 ( _TEXT( "exStepType_18"			));
	CDIO 	exStepType_19			 		 ( _TEXT( "exStepType_19"			));
	CDIO 	exStepType_20			 		 ( _TEXT( "exStepType_20"			));
	CDIO 	exStepType_21			 		 ( _TEXT( "exStepType_21"			));
	CDIO 	exStepType_22			 		 ( _TEXT( "exStepType_22"			));
	CDIO 	exStepType_23			 		 ( _TEXT( "exStepType_23"			));
	CDIO 	exStepType_24			 		 ( _TEXT( "exStepType_24"			));
	CDIO 	exStepType_25			 		 ( _TEXT( "exStepType_25"			));
	CDIO 	exStepType_26			 		 ( _TEXT( "exStepType_26"			));
	CDIO 	exStepType_27			 		 ( _TEXT( "exStepType_27"			));
	CDIO 	exStepType_28			 		 ( _TEXT( "exStepType_28"			));
	CDIO 	exStepType_29			 		 ( _TEXT( "exStepType_29"			));
	CDIO 	exStepType_30			 		 ( _TEXT( "exStepType_30"			));
	CDIO 	exStepType_31			 		 ( _TEXT( "exStepType_31"			));
	CDIO 	exStepType_32			 		 ( _TEXT( "exStepType_32"			));

	CAIO 	exStepTime_01			 		 ( _TEXT( "exStepTime_01"			));
	CAIO 	exStepTime_02			 		 ( _TEXT( "exStepTime_02"			));
	CAIO 	exStepTime_03			 		 ( _TEXT( "exStepTime_03"			));
	CAIO 	exStepTime_04			 		 ( _TEXT( "exStepTime_04"			));
	CAIO 	exStepTime_05			 		 ( _TEXT( "exStepTime_05"			));
	CAIO 	exStepTime_06			 		 ( _TEXT( "exStepTime_06"			));
	CAIO 	exStepTime_07			 		 ( _TEXT( "exStepTime_07"			));
	CAIO 	exStepTime_08			 		 ( _TEXT( "exStepTime_08"			));
	CAIO 	exStepTime_09			 		 ( _TEXT( "exStepTime_09"			));
	CAIO 	exStepTime_10			 		 ( _TEXT( "exStepTime_10"			));
	CAIO 	exStepTime_11			 		 ( _TEXT( "exStepTime_11"			));
	CAIO 	exStepTime_12			 		 ( _TEXT( "exStepTime_12"			));
	CAIO 	exStepTime_13			 		 ( _TEXT( "exStepTime_13"			));
	CAIO 	exStepTime_14			 		 ( _TEXT( "exStepTime_14"			));
	CAIO 	exStepTime_15			 		 ( _TEXT( "exStepTime_15"			));
	CAIO 	exStepTime_16			 		 ( _TEXT( "exStepTime_16"			));
	CAIO 	exStepTime_17			 		 ( _TEXT( "exStepTime_17"			));
	CAIO 	exStepTime_18			 		 ( _TEXT( "exStepTime_18"			));
	CAIO 	exStepTime_19			 		 ( _TEXT( "exStepTime_19"			));
	CAIO 	exStepTime_20			 		 ( _TEXT( "exStepTime_20"			));
	CAIO 	exStepTime_21			 		 ( _TEXT( "exStepTime_21"			));
	CAIO 	exStepTime_22			 		 ( _TEXT( "exStepTime_22"			));
	CAIO 	exStepTime_23			 		 ( _TEXT( "exStepTime_23"			));
	CAIO 	exStepTime_24			 		 ( _TEXT( "exStepTime_24"			));
	CAIO 	exStepTime_25			 		 ( _TEXT( "exStepTime_25"			));
	CAIO 	exStepTime_26			 		 ( _TEXT( "exStepTime_26"			));
	CAIO 	exStepTime_27			 		 ( _TEXT( "exStepTime_27"			));
	CAIO 	exStepTime_28			 		 ( _TEXT( "exStepTime_28"			));
	CAIO 	exStepTime_29			 		 ( _TEXT( "exStepTime_29"			));
	CAIO 	exStepTime_30			 		 ( _TEXT( "exStepTime_30"			));
	CAIO 	exStepTime_31			 		 ( _TEXT( "exStepTime_31"			));
	CAIO 	exStepTime_32			 		 ( _TEXT( "exStepTime_32"			));

	CDIO 	exStepRun_01			 		 ( _TEXT( "exStepRun_01"			));
	CDIO 	exStepRun_02			 		 ( _TEXT( "exStepRun_02"			));
	CDIO 	exStepRun_03			 		 ( _TEXT( "exStepRun_03"			));
	CDIO 	exStepRun_04			 		 ( _TEXT( "exStepRun_04"			));
	CDIO 	exStepRun_05			 		 ( _TEXT( "exStepRun_05"			));
	CDIO 	exStepRun_06			 		 ( _TEXT( "exStepRun_06"			));
	CDIO 	exStepRun_07			 		 ( _TEXT( "exStepRun_07"			));
	CDIO 	exStepRun_08			 		 ( _TEXT( "exStepRun_08"			));
	CDIO 	exStepRun_09			 		 ( _TEXT( "exStepRun_09"			));
	CDIO 	exStepRun_10			 		 ( _TEXT( "exStepRun_10"			));
	CDIO 	exStepRun_11			 		 ( _TEXT( "exStepRun_11"			));
	CDIO 	exStepRun_12			 		 ( _TEXT( "exStepRun_12"			));
	CDIO 	exStepRun_13			 		 ( _TEXT( "exStepRun_13"			));
	CDIO 	exStepRun_14			 		 ( _TEXT( "exStepRun_14"			));
	CDIO 	exStepRun_15			 		 ( _TEXT( "exStepRun_15"			));
	CDIO 	exStepRun_16			 		 ( _TEXT( "exStepRun_16"			));
	CDIO 	exStepRun_17			 		 ( _TEXT( "exStepRun_17"			));
	CDIO 	exStepRun_18			 		 ( _TEXT( "exStepRun_18"			));
	CDIO 	exStepRun_19			 		 ( _TEXT( "exStepRun_19"			));
	CDIO 	exStepRun_20			 		 ( _TEXT( "exStepRun_20"			));
	CDIO 	exStepRun_21			 		 ( _TEXT( "exStepRun_21"			));
	CDIO 	exStepRun_22			 		 ( _TEXT( "exStepRun_22"			));
	CDIO 	exStepRun_23			 		 ( _TEXT( "exStepRun_23"			));
	CDIO 	exStepRun_24			 		 ( _TEXT( "exStepRun_24"			));
	CDIO 	exStepRun_25			 		 ( _TEXT( "exStepRun_25"			));
	CDIO 	exStepRun_26			 		 ( _TEXT( "exStepRun_26"			));
	CDIO 	exStepRun_27			 		 ( _TEXT( "exStepRun_27"			));
	CDIO 	exStepRun_28			 		 ( _TEXT( "exStepRun_28"			));
	CDIO 	exStepRun_29			 		 ( _TEXT( "exStepRun_29"			));
	CDIO 	exStepRun_30			 		 ( _TEXT( "exStepRun_30"			));
	CDIO 	exStepRun_31			 		 ( _TEXT( "exStepRun_31"			));
	CDIO 	exStepRun_32			 		 ( _TEXT( "exStepRun_32"			));

	CAIO 	exStepAddTime_01		 		 ( _TEXT( "exStepAddTime_01"			));
	CAIO 	exStepAddTime_02		 		 ( _TEXT( "exStepAddTime_02"			));
	CAIO 	exStepAddTime_03		 		 ( _TEXT( "exStepAddTime_03"			));
	CAIO 	exStepAddTime_04		 		 ( _TEXT( "exStepAddTime_04"			));
	CAIO 	exStepAddTime_05		 		 ( _TEXT( "exStepAddTime_05"			));
	CAIO 	exStepAddTime_06		 		 ( _TEXT( "exStepAddTime_06"			));
	CAIO 	exStepAddTime_07		 		 ( _TEXT( "exStepAddTime_07"			));
	CAIO 	exStepAddTime_08		 		 ( _TEXT( "exStepAddTime_08"			));
	CAIO 	exStepAddTime_09		 		 ( _TEXT( "exStepAddTime_09"			));
	CAIO 	exStepAddTime_10		 		 ( _TEXT( "exStepAddTime_10"			));
	CAIO 	exStepAddTime_11		 		 ( _TEXT( "exStepAddTime_11"			));
	CAIO 	exStepAddTime_12		 		 ( _TEXT( "exStepAddTime_12"			));
	CAIO 	exStepAddTime_13		 		 ( _TEXT( "exStepAddTime_13"			));
	CAIO 	exStepAddTime_14		 		 ( _TEXT( "exStepAddTime_14"			));
	CAIO 	exStepAddTime_15		 		 ( _TEXT( "exStepAddTime_15"			));
	CAIO 	exStepAddTime_16		 		 ( _TEXT( "exStepAddTime_16"			));
	CAIO 	exStepAddTime_17		 		 ( _TEXT( "exStepAddTime_17"			));
	CAIO 	exStepAddTime_18		 		 ( _TEXT( "exStepAddTime_18"			));
	CAIO 	exStepAddTime_19		 		 ( _TEXT( "exStepAddTime_19"			));
	CAIO 	exStepAddTime_20		 		 ( _TEXT( "exStepAddTime_20"			));
	CAIO 	exStepAddTime_21		 		 ( _TEXT( "exStepAddTime_21"			));
	CAIO 	exStepAddTime_22		 		 ( _TEXT( "exStepAddTime_22"			));
	CAIO 	exStepAddTime_23		 		 ( _TEXT( "exStepAddTime_23"			));
	CAIO 	exStepAddTime_24		 		 ( _TEXT( "exStepAddTime_24"			));
	CAIO 	exStepAddTime_25		 		 ( _TEXT( "exStepAddTime_25"			));
	CAIO 	exStepAddTime_26		 		 ( _TEXT( "exStepAddTime_26"			));
	CAIO 	exStepAddTime_27		 		 ( _TEXT( "exStepAddTime_27"			));
	CAIO 	exStepAddTime_28		 		 ( _TEXT( "exStepAddTime_28"			));
	CAIO 	exStepAddTime_29		 		 ( _TEXT( "exStepAddTime_29"			));
	CAIO 	exStepAddTime_30		 		 ( _TEXT( "exStepAddTime_30"			));
	CAIO 	exStepAddTime_31		 		 ( _TEXT( "exStepAddTime_31"			));
	CAIO 	exStepAddTime_32		 		 ( _TEXT( "exStepAddTime_32"			));

	CSIO 	RECIPE_FILE			 			 ( _TEXT( "RECIPE_FILE"					));
	CDIO 	LockItem				 		 ( _TEXT( "LockItem"					));
	CSIO 	sGUI_StepProc_msg		 		 ( _TEXT( "sGUI_StepProc_msg"			));
	CAIO 	View_CurStepNum		 			 ( _TEXT( "View_CurStepNum"				));
	CSIO 	OnMessage						 ( _TEXT( "OnMessage"					));
	CAIO	PROC_TimeOffset				  	 ( _TEXT( "PROC_TimeOffset"				));
	CDIO 	StepProcEnable			 		 ( _TEXT( "StepProcEnable"				));

	CDIO 	Process_Condition		 		 ( _TEXT( "Process_Condition"			));
	CDIO 	Glass_Existence_CHK	 		     ( _TEXT( "Glass_Existence_CHK"			));

	CAIO 	Dechuck_SRF_Power				 ( _TEXT( "Dechuck_SRF_Power"			));
	CAIO 	Dechuck_BRF_Power				 ( _TEXT( "Dechuck_BRF_Power"			));

	CDIO 	PM_IsRunEMP						 ( _TEXT( "PM_IsRunEMP"					));
	CDIO 	EmpORNormal_Process			     ( _TEXT( "EmpORNormal_Process"			));
	CAIO 	EmptyProcess_Count				 ( _TEXT( "EmptyProcess_Count"			));
	CSIO 	sGUI_EmptyProcessEnd_msg 		 ( _TEXT( "sGUI_EmptyProcessEnd_msg"	));

	CDIO 	CountProcess_IsRun				 ( _TEXT( "CountProcess_IsRun"			));
	CDIO 	CountORNormal_Process			 ( _TEXT( "CountORNormal_Process"		));
	CAIO 	CountProcess_Count				 ( _TEXT( "CountProcess_Count"			));
	CSIO 	sGUI_CountProcessEnd_msg 		 ( _TEXT( "sGUI_CountProcessEnd_msg"	));

	CDIO 	Dechuck_InterlockOn				 ( _TEXT( "Dechuck_InterlockOn"			));
	CAIO 	cCDGUseLevel					( _TEXT( "cCDGUseLevel"				));

	CAIO 	cPreset_EndRange				 ( _TEXT( "cPreset_EndRange"			));
	CDIO 	cVVC_PreSetUse					 ( _TEXT( "cVVC_PreSetUse"				));
	CAIO 	cCDGConditionStepLevel			( _TEXT( "cCDGConditionStepLevel"	));
	CAIO 	cPinPosition3_mmSET				( _TEXT( "cPinPosition3_mmSET"		));

	CAIO 	cPinPosition2_mmSET				( _TEXT( "cPinPosition2_mmSET"		));
	CDIO 	ManualAbortAutoDechuck			( _TEXT( "ManualAbortAutoDechuck"	));
	CDIO 	dHW_Interlock_On_Status			( _TEXT( "dHW_Interlock_On_Status"	));

	CAIO 	cCHL_CH1_Temp					( _TEXT( "cCHL_CH1_Temp"		));
	CAIO 	cCHL_CH2_Temp					( _TEXT( "cCHL_CH2_Temp"		));
	CAIO 	cCHL_CH3_Temp					( _TEXT( "cCHL_CH3_Temp"		));

	CDIO	Recipe_Max_Step					( _TEXT( "Recipe_Max_Step"		));
	CDIO	Recipe_Step_Num					( _TEXT( "Recipe_Step_Num"		));
	CDIO	Process_Run						( _TEXT( "Process_Run"			));
	CSIO	Recipe_Step_Desc				( _TEXT( "Recipe_Step_Desc"	    ));
	
	CDIO	STEP01_STEPNO					( _TEXT( "STEP01_STEPNO"	    ));
	CDIO	STEP02_STEPNO					( _TEXT( "STEP02_STEPNO"	    ));
	CDIO	STEP03_STEPNO					( _TEXT( "STEP03_STEPNO"	    ));
	CDIO	STEP04_STEPNO					( _TEXT( "STEP04_STEPNO"	    ));
	CDIO	STEP05_STEPNO					( _TEXT( "STEP05_STEPNO"	    ));
	CDIO	STEP06_STEPNO					( _TEXT( "STEP06_STEPNO"	    ));
	CDIO	STEP07_STEPNO					( _TEXT( "STEP07_STEPNO"	    ));
	CDIO	STEP08_STEPNO					( _TEXT( "STEP08_STEPNO"	    ));
	CDIO	STEP09_STEPNO					( _TEXT( "STEP09_STEPNO"	    ));
	CDIO	STEP10_STEPNO					( _TEXT( "STEP10_STEPNO"	    ));
	CDIO	STEP11_STEPNO					( _TEXT( "STEP11_STEPNO"	    ));
	CDIO	STEP12_STEPNO					( _TEXT( "STEP12_STEPNO"	    ));
	CDIO	STEP13_STEPNO					( _TEXT( "STEP13_STEPNO"	    ));
	CDIO	STEP14_STEPNO					( _TEXT( "STEP14_STEPNO"	    ));
	CDIO	STEP15_STEPNO					( _TEXT( "STEP15_STEPNO"	    ));
	CDIO	STEP16_STEPNO					( _TEXT( "STEP16_STEPNO"	    ));
	CDIO	STEP17_STEPNO					( _TEXT( "STEP17_STEPNO"	    ));
	CDIO	STEP18_STEPNO					( _TEXT( "STEP18_STEPNO"	    ));
	CDIO	STEP19_STEPNO					( _TEXT( "STEP19_STEPNO"	    ));
	CDIO	STEP20_STEPNO					( _TEXT( "STEP20_STEPNO"	    ));
	CDIO	STEP21_STEPNO					( _TEXT( "STEP21_STEPNO"	    ));
	CDIO	STEP22_STEPNO					( _TEXT( "STEP22_STEPNO"	    ));
	CDIO	STEP23_STEPNO					( _TEXT( "STEP23_STEPNO"	    ));
	CDIO	STEP24_STEPNO					( _TEXT( "STEP24_STEPNO"	    ));
	CDIO	STEP25_STEPNO					( _TEXT( "STEP25_STEPNO"	    ));
	CDIO	STEP26_STEPNO					( _TEXT( "STEP26_STEPNO"	    ));
	CDIO	STEP27_STEPNO					( _TEXT( "STEP27_STEPNO"	    ));
	CDIO	STEP28_STEPNO					( _TEXT( "STEP28_STEPNO"	    ));
	CDIO	STEP29_STEPNO					( _TEXT( "STEP29_STEPNO"	    ));
	CDIO	STEP30_STEPNO					( _TEXT( "STEP30_STEPNO"	    ));
	CDIO	STEP31_STEPNO					( _TEXT( "STEP31_STEPNO"	    ));
	CDIO	STEP32_STEPNO					( _TEXT( "STEP32_STEPNO"	    ));
	
	CSIO	STEP01_DESC						( _TEXT( "STEP01_DESC"			));
	CSIO	STEP02_DESC						( _TEXT( "STEP02_DESC"			));
	CSIO	STEP03_DESC						( _TEXT( "STEP03_DESC"			));
	CSIO	STEP04_DESC						( _TEXT( "STEP04_DESC"			));
	CSIO	STEP05_DESC						( _TEXT( "STEP05_DESC"			));
	CSIO	STEP06_DESC						( _TEXT( "STEP06_DESC"			));
	CSIO	STEP07_DESC						( _TEXT( "STEP07_DESC"			));
	CSIO	STEP08_DESC						( _TEXT( "STEP08_DESC"			));
	CSIO	STEP09_DESC						( _TEXT( "STEP09_DESC"			));
	CSIO	STEP10_DESC						( _TEXT( "STEP10_DESC"			));
	CSIO	STEP11_DESC						( _TEXT( "STEP11_DESC"			));
	CSIO	STEP12_DESC						( _TEXT( "STEP12_DESC"			));
	CSIO	STEP13_DESC						( _TEXT( "STEP13_DESC"			));
	CSIO	STEP14_DESC						( _TEXT( "STEP14_DESC"			));
	CSIO	STEP15_DESC						( _TEXT( "STEP15_DESC"			));
	CSIO	STEP16_DESC						( _TEXT( "STEP16_DESC"			));
	CSIO	STEP17_DESC						( _TEXT( "STEP17_DESC"			));
	CSIO	STEP18_DESC						( _TEXT( "STEP18_DESC"			));
	CSIO	STEP19_DESC						( _TEXT( "STEP19_DESC"			));
	CSIO	STEP20_DESC						( _TEXT( "STEP20_DESC"			));
	CSIO	STEP21_DESC						( _TEXT( "STEP21_DESC"			));
	CSIO	STEP22_DESC						( _TEXT( "STEP22_DESC"			));
	CSIO	STEP23_DESC						( _TEXT( "STEP23_DESC"			));
	CSIO	STEP24_DESC						( _TEXT( "STEP24_DESC"			));
	CSIO	STEP25_DESC						( _TEXT( "STEP25_DESC"			));
	CSIO	STEP26_DESC						( _TEXT( "STEP26_DESC"			));
	CSIO	STEP27_DESC						( _TEXT( "STEP27_DESC"			));
	CSIO	STEP28_DESC						( _TEXT( "STEP28_DESC"			));
	CSIO	STEP29_DESC						( _TEXT( "STEP29_DESC"			));
	CSIO	STEP30_DESC						( _TEXT( "STEP30_DESC"			));
	CSIO	STEP31_DESC						( _TEXT( "STEP31_DESC"			));
	CSIO	STEP32_DESC						( _TEXT( "STEP32_DESC"			));
	
	CDIO	Datalog_Control					( _TEXT( "Datalog_Control"		));	
	CDIO	Empty_Idle_Process				( _TEXT( "Empty_Idle_Process"	));
	CDIO	DO_PLC_ResetRun					( _TEXT("DO_PLC_ResetRun"		));

	CDIO    CURRENT_PM_MODULE				(_TEXT("CURRENT_PM_MODULE"			));	//PM1/2/3/4/5/6

	CSequence 	FNC_SRF						( _TEXT( "FNC_SRF"				));
	CSequence 	FNC_BRF						( _TEXT( "FNC_BRF"				));
	CSequence 	FNC_MFC						( _TEXT( "FNC_MFC"				));
	CSequence 	FNC_ESC						( _TEXT( "FNC_ESC"				));
	CSequence 	FNC_APC						( _TEXT( "FNC_APC"				));
	CSequence 	FNC_HE						( _TEXT( "FNC_HE"				));
	CSequence 	FNC_PIN						( _TEXT( "FNC_PIN"				));
	CSequence 	FNC_RF_CTRL					( _TEXT( "FNC_RF_CTRL"			));
	CSequence 	FNC_VVC_MOTOR				( _TEXT( "FNC_VVC_MOTOR"		));
	CSequence	utilIOLogPC					( _TEXT( "utilIOLogPC"			));	
	CSequence	PROCESSLOG					( _TEXT( "PROCESSLOG"			));

	//Alarm
	CAlarm 		ProcessAborted 				( _TEXT( "ProcessAborted"			)); // Process Aborted	
	CAlarm 		ProcessTimeout 				( _TEXT( "ProcessTimeout"			)); // Process aborted by timeout
	CAlarm 		GateNotClose 				( _TEXT( "GateNotClose"				)); // PC Gate valve was not closed
	CAlarm 		ManualProcessAborted		( _TEXT( "ManualProcessAborted"		)); // Process aborted by Manual Button
	
	CAlarm 		AgingOverProcessAborted 	( _TEXT( "AgingOverProcessAborted"	)); // Can not Run process. Aging Run Process Count is Over 9999
	CAlarm 		AgingZeroProcessAborted 	( _TEXT( "AgingZeroProcessAborted"	)); // Can not Run process. Aging Run Process Count is Zero
	CAlarm 		EmptyProcessAborted 		( _TEXT( "EmptyProcessAborted"		)); // Can not Run Empty Process
	CAlarm 		NoGlassProcessAborted 		( _TEXT( "NoGlassProcessAborted"	)); // Can not Run process. Glass is absent
	CAlarm 		InGlassProcessAborted 		( _TEXT( "InGlassProcessAborted"	)); // Can not Run Empty process. Glass is Present
	CAlarm 		InGlassRFNoSetAborted 		( _TEXT( "InGlassRFNoSetAborted"	)); // Can not Run process. Recipe Discharge Step is NoSetting
	CAlarm 		InGlassChillerAborted 		( _TEXT( "InGlassChillerAborted"	)); // Can not Run process. Chiller UseSetTemp is wrong

	CAlarm 		PROCESS_1100 				( _TEXT( "PROCESS_1100"			));		// Fail to run APC function. Check APC valve status
	CAlarm 		PROCESS_1101 				( _TEXT( "PROCESS_1101"			));		// Fail to run MFC function. Check MFC valve status
	CAlarm 		PROCESS_1102 				( _TEXT( "PROCESS_1102"			));		// Fail to run ESC function. Check ESC status
	CAlarm 		PROCESS_1103 				( _TEXT( "PROCESS_1103"			));		// Fail to run ESC_He function. Check Helium line
	CAlarm 		PROCESS_1104 				( _TEXT( "PROCESS_1104"			));		// Fail to run Source RF function. Check Source RF Gen status	
	CAlarm 		PROCESS_1105 				( _TEXT( "PROCESS_1105"			));		// Fail to run Bias RF function. Check Bias RF Gen status
	CAlarm 		PROCESS_1106 				( _TEXT( "PROCESS_1106"			));		// Fail to run PIN function. Check pin status
	CAlarm 		PROCESS_1107 				( _TEXT( "PROCESS_1107"			));		// Fail to run Strip function. Check Strip Device status
	CAlarm 		PROCESS_1108 				( _TEXT( "PROCESS_1108"			));		// Fail to run RF Control function. Check RF status
	
	CAlarm 		PROCESS_1140 				( _TEXT( "PROCESS_1140"			));		// Can not run Process__Failed to move pin down__PROS_POS1
	CAlarm 		PROCESS_1141 				( _TEXT( "PROCESS_1141"			));		// Can not run Process__Failed to preset VVC Moto
	
	CAlarm 		PROCESS_1151 				( _TEXT( "PROCESS_1151"			));		// Can not run process__Total step is less than 1
	CAlarm 		PROCESS_1160 				( _TEXT( "PROCESS_1160"			));		// Can not run process__Sub function is already running
	CAlarm 		PROCESS_1180 				( _TEXT( "PROCESS_1180"			));		// Can not run STEP_CONTROL__PM_PROC_CTRL is ABORT
	
	CAlarm 		RECIPEFAIL_1200 			( _TEXT( "RECIPEFAIL_1200"		));		// Process Recipe is difference
	CAlarm 		RECIPEFAIL_1201 			( _TEXT( "RECIPEFAIL_1201"		));		// Can not run process__Failed to read recipe file
	CAlarm 		RECIPEFAIL_1202 			( _TEXT( "RECIPEFAIL_1202"		));		// Recipe RF Power is higher than Limit Power
	CAlarm 		RECIPEFAIL_1203 			( _TEXT( "RECIPEFAIL_1203"		));		// Dechuck Enable Step is NoSetting. Check Recipe
	CAlarm 		RECIPEFAIL_1204 			( _TEXT( "RECIPEFAIL_1204"		));		// Dechuck Enable StepNum is lower than Last EtchStepNum
	CAlarm 		RECIPEFAIL_1205 			( _TEXT( "RECIPEFAIL_1205"		));		// Dechuck Enable Step is higher than PinMovePos UseStep
	CAlarm 		RECIPEFAIL_1206 			( _TEXT( "RECIPEFAIL_1206"		));		// Dechuck Enable StepCount is too many
	CAlarm 		RECIPEFAIL_1207 			( _TEXT( "RECIPEFAIL_1207"		));		// Can not run process__Failed to read recipe file
	CAlarm 		RECIPEFAIL_1208 			( _TEXT( "RECIPEFAIL_1208"		));		// ESC GRIP Or He Flow Use. Recipe Check

	CAlarm 		EPD_5504 					( _TEXT( "EPD_5504"				));
	CAlarm 		EPD_5510 					( _TEXT( "EPD_5510"				));

			
END_OBJECT_ENUMERATION


//----------------------------------------------------------------------------------------------------------------------------------------------

RecipeStepTemplate  RECIPE0;
RecipeStepTemplateM RECIPE[ MAX_STEP_COUNT ];
RecipeInfoStruct	RECIPEINFO;

HANDLE	pFileHandle;

BOOL	SIMULATION_MODE			= FALSE;
BOOL	g_bStepControlRunning	= FALSE;
BOOL	g_bDechuck_powerState	= FALSE;
BOOL	g_nIdleESCPowerOnState	= FALSE;
BOOL	EPD_ALARM_ABORT_NOW		= TRUE;
BOOL	EPD_ALARM_HAPPEN		= FALSE;

static int g_nFunctionResult[8] = {0};

int		STEP_RESULT				= 0;
int		g_nPROC_SKIP_FLAG		= -1;
int		g_nBackup_Step_Number	= 0;
int		g_nTotalStep			= 0;
int		g_nCurrent_Step_Number  = 0;
int		g_nDechuckStep_ST		= 0;
int		g_nDechuckStep_ED		= 0;
int		g_nDechuckStep			= 0;
int		g_nEPDStepCount			= 0;
int		g_nLastEtchStep			= 0;
int		g_nDechuckStepCount     = 0;
int		g_nPin_3Position_USESTEP= 0;
int		g_nPin_4Position_USESTEP= 0;
int		g_iCurrentStepType		= 0;
int		print					= 0;
int		nLock;
int		STEP_COUNT;
int		WAFER_INFORMATION;
int     nMFC_FunctionCount		= 0;
int		nDechuckStepNumber		= 0;
int		nFunctionAbortCheck		= 0;
			
double	g_dblCurrentProcessTime = 0.0;
double	g_dblCurrentStepTime	= 0.0;
double	g_dblStepInProcessTime  = 0.0;
double	g_dblTotalProcessTime	= 0.0;
double	g_dblJustEtchTime		= 0.0;
double	g_dblAtortedStepNumber	= 0;
double	g_dblAtortedStepTime	= 0;
double	g_dblRecipeStepTime		= 0;
double	g_dblRecipeStepNo		= 0;
double  mTargetValue			= 0;
double	g_dblTotalEtchTime		= 0.0;
double	g_dblEtchingTime		= 0.0;
double	g_dblReStartStepTime	= 0.0;
double  TOTAL_TIME;

char	g_szRecipeName[128]		= "";
char	gszPath[512]			= {0};
char	gszPath_origin[512]		= {0};
char	gszFileName[128]		= {0};
char	mLog[2048]				= "";
char	mPcName[32]				= "";
char	mGlassID[256]			= "";
char	mRecipeID[256]			= "";
char	Condition_Step_Alarm[512] ="";
char	RECIPE_NAME[ MAX_RECIPENAME_LEN + 1 ];
char    RECIPE_STEPNAME[255];
char	RECIPE_FILE_NAME[512];
char	EVENT_MODULE_DISABLE[64];

TString strFile_Old; //181121 KYS Abort Add 
TString strFile_Err; //181121 KYS Abort Add

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL FuncAbortChk() 
{
	BOOL nResult=TRUE;

	if( FNC_SRF.Status()		!= SEQ_SUCCESS )		nResult=FALSE;
	if( FNC_BRF.Status()		!= SEQ_SUCCESS )		nResult=FALSE;
	if( FNC_MFC.Status()		!= SEQ_SUCCESS )		nResult=FALSE;
	if( FNC_ESC.Status()		!= SEQ_SUCCESS )		nResult=FALSE;
	if( FNC_APC.Status()		!= SEQ_SUCCESS )		nResult=FALSE;
	if( FNC_HE.Status()			!= SEQ_SUCCESS )		nResult=FALSE;
	if( FNC_PIN.Status()		!= SEQ_SUCCESS )		nResult=FALSE;
	if( FNC_RF_CTRL.Status()	!= SEQ_SUCCESS )		nResult=FALSE;

	return nResult;
}

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
	nLock = 0;


	if(strlen(gszPath_origin) < 1)
	{
		strcpy(gszPath_origin, csPath);
		strcpy(gszPath, csPath);
	}
	else
	{
		strcpy(gszPath, gszPath_origin);
	}

	_splitpath(gszPath, sz_drive, sz_dir, gszFileName, sz_ext);


	sprintf(gszPath, "%s%s%s%s", sz_drive, sz_dir, gszFileName, sz_ext);
	sprintf(sz_FullPath, "%s%s", sz_drive, sz_dir);

	CreateDirectory("Log", NULL);
	CreateDirectory("Log//MAIN_PROCESS", NULL);
	CreateDirectory(sz_FullPath, NULL);	

	return 1;
}


int MakeFile()
{
	FILE *in;
	int flag  = 1;
	int count = 1;

	while(nLock > 0) 
	{
	    if(FuncAbortChk() == TRUE)			break;
		if(WaitAbort(10))	return -1;
	}
	nLock = 1;


	in = fopen(gszPath,"a+");
	if(in == NULL)	
	{
		flag = -1;

		pFileHandle = CreateFile(gszPath,
						   GENERIC_WRITE|GENERIC_READ,
						   FILE_SHARE_READ|FILE_SHARE_WRITE,
						   NULL,
						   CREATE_ALWAYS,
						   FILE_ATTRIBUTE_NORMAL,
						   NULL);

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
	}
	
	nLock = 0;


	return 1;
}

int OutputLog(char *csMsg)
{
	DWORD dwWritten;
	DWORD dwSize;
	char  szFileName[512] = {0};
	int   nCounts = 0;

	while(nLock > 0) 
	{
		if(FuncAbortChk() == TRUE)			break;
		if(WaitAbort(10))	return -1;
	}
	nLock = 1;

	if(strlen(csMsg) < 1 || strlen(gszPath) < 1)
	{
		nLock = 0;
		return -1;
	}

	pFileHandle = CreateFile(gszPath,
							   GENERIC_WRITE|GENERIC_READ,
							   FILE_SHARE_READ|FILE_SHARE_WRITE,
							   NULL,
							   OPEN_ALWAYS,
							   FILE_ATTRIBUTE_NORMAL,
							   NULL);
	dwSize = GetFileSize(pFileHandle, NULL);
	if(dwSize > 10 * FILE_SIZE)
	{
		CloseHandle(pFileHandle);
		
		pFileHandle = INVALID_HANDLE_VALUE;
		_sleep(10);
		
		sprintf(szFileName, "%s.%d", gszPath, nCounts);
	
		if(DeleteFile(szFileName) != 0)
		{
			printf("$DeleteFile [ %s ] -- OK \r\n", szFileName);
		}
		
		MoveFile(gszPath, szFileName);
		
		InitLog("");
		MakeFile();
		
		pFileHandle = CreateFile(gszPath,
			GENERIC_WRITE|GENERIC_READ,
			FILE_SHARE_READ|FILE_SHARE_WRITE,
			NULL,
			OPEN_ALWAYS,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
	}


	if(pFileHandle != INVALID_HANDLE_VALUE)
	{
		SetFilePointer(pFileHandle, 0, 0, FILE_END);

		WriteFile(pFileHandle, csMsg,  strlen(csMsg),  &dwWritten, NULL);
		CloseHandle(pFileHandle);
		pFileHandle = INVALID_HANDLE_VALUE;
	}

	nLock = 0;
	return 1;
}


void print_log(char	*szLog, int nLevel)
{
	char csTime[1024] = {0};
	SYSTEMTIME st;

	GetLocalTime(&st);

	if(nLevel == -1)
	{
		sprintf(csTime,	"%00004d/%02d/%02d %002d:%002d:%002d:%003d  [ERROR]  %s\r\n",
					st.wYear,st.wMonth,	st.wDay,
					st.wHour,st.wMinute,st.wSecond,st.wMilliseconds, szLog);
	}
	else
	{
		sprintf(csTime,	"%00004d/%02d/%02d %002d:%002d:%002d:%003d  [EVENT]  %s\r\n",
					st.wYear,st.wMonth,	st.wDay,
					st.wHour,st.wMinute,st.wSecond,st.wMilliseconds, szLog);
	}
	
	OutputLog(csTime);
}

int Make_Log(char* msg1, char* msg2, int lvl)
{
	char msg[1024] = {0};
	int CS = 0;

	sprintf(msg, "[%-*s]  %s", 20, msg1, msg2);
	print_log(msg, lvl);

	return 1;
}

int Run_Func_Check_Result(CSequence FunctionId, char *szParam, int nSec)
{
	int nCfgTime = 0;
	int nFnResult = 0;
	char szBuf[256] = {0};
	int nCS = 0;
	nCfgTime = nSec * 20;

	FunctionId.RunTh(szParam);

	Make_Log("Check_Fnc_Sts", "Started", EVENT);

	do 
	{
		nFnResult = FunctionId.Status();

		if(nFnResult == SEQ_SUCCESS || nFnResult == SEQ_ABORT || nFnResult == SEQ_ERROR)
		{
			break;
		}

		if(WaitAbort(10))
		{
			Make_Log("", "FuncAbortChk is TRUE", EVENT);
			break;
		}

		if(PM_PROC_CTRL.Read(nCS) == ePROC_ABORT)
		{
			Make_Log("", "PM_PROC_CTRL(= ePROC_ABORT)", EVENT);
			break;
		}
		nCfgTime--;
		if(WaitAbort(10))	return SEQ_ABORT;
	}while(nCfgTime > 0);


	nFnResult = FunctionId.Status();
	
	if( nFnResult == SEQ_RUNNING )
	{
		FunctionId.Abort();
		nFnResult = SEQ_ABORT;
		
		sprintf(szBuf, "Function Id = %d, Check Time = %d, Call RUN_FUNCTION_ABORT()", FunctionId, nCfgTime);
		Make_Log("", szBuf, FAULT);
	}

	Make_Log("Check_Fnc_Sts", "Ended", EVENT);

	return nFnResult;
}

int BothRF_TurnOFF()
{
	int CS;
	
	if(DI_BIAS_RFG_POWER_ON.Read(CS) == 1)
	{
		DO_BIAS_RFG_POWER_ON.Write(0 , CS);
		WaitAbort(200);
	}
	
	DO_BIAS_RFG_POWER_ON.Write(0,CS);
	DO_SOURCE_RFG_POWER_ON.Write(0,CS);
	
	if(FNC_RF_CTRL.Status() == SEQ_RUNNING)
	{
		FNC_RF_CTRL.Abort();
	}
	FNC_RF_CTRL.Run("PROC_RF_OFF");
	
	return 1;
}


int Abort()
{
	int nResult = 0;
	int CS = 0;

	Make_Log("ABORT", "STARTED", EVENT);

	sGUI_Proc_msg.Write(_TEXT("ABORT Function is running."));

	if(DI_BIAS_RFG_POWER_ON.Read(CS) == 1)
	{
		DO_BIAS_RFG_POWER_ON.Write(OFF);		WaitAbort(200);
	}

	DO_BIAS_RFG_POWER_ON.Write(0,CS);
	DO_SOURCE_RFG_POWER_ON.Write(0,CS);

	cHE_Monitor_ME.Write(Mon_Dis,	CS);
	cESC_Monitor_ME.Write(Mon_Dis , CS);
	Itk_SRC_Mon.Write(Mon_Dis,		CS);
	Itk_BIAS_Mon.Write(Mon_Dis,		CS);
	
	if(FNC_RF_CTRL.Status() == SEQ_RUNNING)
	{
		FNC_RF_CTRL.Abort();
	}
	FNC_RF_CTRL.RunAnyTh(_TEXT("PROC_RF_OFF"));
	FNC_SRF.Abort();
	
	Make_Log("", "ABORT FNC_SRF", FAULT);
		
	FNC_BRF.Abort();
	Make_Log("", "ABORT FNC_BRF", FAULT);
	
	FNC_APC.Abort();

	if(APC_RUN_NGOK.Equal(eNG))
	{
		Make_Log("", "APC Run Condition is NG, FNC_APC|ABORT", EVENT);
		FNC_APC.RunAnyTh(_TEXT("ABORT"));
	}
	else
	{
		Make_Log("", "APC Run Condition is OK, FNC_APC|FULLOPEN", EVENT);
		FNC_APC.RunAnyTh(_TEXT("FULLOPEN"));
	}

	Make_Log("", "ABORT FNC_APC", FAULT);
	
	FNC_MFC.Abort();
	FNC_MFC.RunAnyTh(_TEXT("FULLCLOSE"));
	Make_Log("", "ABORT FNC_MFC", FAULT);
	
	FNC_HE.Abort();
	FNC_HE.RunAnyTh(_TEXT("ABORT"));
	Make_Log("", "ABORT FNC_HE", FAULT);

	FNC_ESC.Abort();
	FNC_ESC.RunAnyTh(_TEXT("ABORT"));
	Make_Log("", "ABORT FNC_ESC", FAULT);
	
	sGUI_Proc_msg.Write(_TEXT("ABORT Function is completed"));
	Make_Log("ABORT", "ENDED", EVENT);
	
	return 1;
}

int SetRcpParamToFunParam(int nStepNo)
{
	int index = 0;
	int CS = 0;
	char szTemp1[256] = {0};
	char szTemp2[256] = {0};
	
	sprintf(szTemp1, "STEP NUM   = %d",		nStepNo);							Make_Log(szTemp1, "START : RCP PARAMETER TO FUNCTION", EVENT);

	sprintf(szTemp2, "STEP NAME  = %s",		RECIPE[nStepNo].STEP_NAME);			Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "STEP END TYPE  = %d", RECIPE[nStepNo].STEP_ENDTYPE);		Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "STEP TYPE  = %d",		RECIPE[nStepNo].STEP_TYPE);			Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "STEP TIME  = %.0f",	RECIPE[nStepNo].STEP_TIME);			Make_Log("", szTemp2, EVENT);

	PR_STEPENDTYPE.Write( RECIPE[nStepNo].STEP_ENDTYPE , CS);

	if(RECIPE[nStepNo].SRFG_POWER >= Dechuck_SRF_Power.Read(CS) && strcmp(RECIPE[nStepNo].SRF_CTRL, "POWERON")   == 0) RECIPE[nStepNo].Is_RF_Step = 1;

	sprintf(szTemp2, "Is_RF_Step = %d", RECIPE[nStepNo].Is_RF_Step);	Make_Log("", szTemp2, EVENT);
	
	PR_GAS1.Write( RECIPE[nStepNo].GAS1 ,	CS);
	PR_GAS2.Write( RECIPE[nStepNo].GAS2 ,	CS);
	PR_GAS3.Write( RECIPE[nStepNo].GAS3 ,	CS);
	PR_GAS4.Write( RECIPE[nStepNo].GAS4 ,	CS);
	PR_GAS5.Write( RECIPE[nStepNo].GAS5 ,	CS);
	PR_GAS6.Write( RECIPE[nStepNo].GAS6 ,	CS);
	PR_GAS7.Write( RECIPE[nStepNo].GAS7 ,	CS);
	PR_GAS8.Write( RECIPE[nStepNo].GAS8 ,	CS);
	PR_GAS9.Write( RECIPE[nStepNo].GAS9 ,	CS);
	PR_GAS10.Write( RECIPE[nStepNo].GAS10 , CS);

	for(index = 0; index < MAX_GAS_COUNT; index++)
	{
		if(AioAt(&PR_GAS1 , index )->Read(CS) > 0.0)
		{
			sprintf(szTemp2, "PR_GAS%d = %.0f", index + 1, AioAt(&PR_GAS1 , index )->Read(CS));
			Make_Log("", szTemp2, EVENT);
		}
	}

	PR_FRC_CENTER.Write( RECIPE[nStepNo].FRC_CENTER , CS);

	sprintf(szTemp2, "PR_FRC Center=%.0f" , PR_FRC_CENTER.Read(CS)); 
	Make_Log("", szTemp2, EVENT);

	PR_APC_PRESS.Write( RECIPE[nStepNo].PRESSURE , CS );
	sprintf(szTemp2, "APC_CTRL = %s", RECIPE[nStepNo].APC_CTRL);				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_APC_PRESS = %.0f", PR_APC_PRESS.Read(CS));				Make_Log("", szTemp2, EVENT);

	PR_SRFG_POWER.Write(RECIPE[nStepNo].SRFG_POWER,CS);
	PR_SRFM_MATCH.Write(RECIPE[nStepNo].SRFM_MATCH,CS);
	PR_SRFM_TUNE.Write(RECIPE[nStepNo].SRFM_TUNE,CS);
	
	sprintf(szTemp2, "SRF_CTRL = %s", RECIPE[nStepNo].SRF_CTRL);					Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_SRFG_POWER = %.0f", PR_SRFG_POWER.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_SRFM_TUNE = %.0f",	 PR_SRFM_TUNE.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_SRFM_MATCH = %.0f", PR_SRFM_MATCH.Read(CS));				Make_Log("", szTemp2, EVENT);

	PR_BRFG_POWER.Write(RECIPE[nStepNo].BRFG_POWER,CS);
	PR_BRFM_MATCH.Write(RECIPE[nStepNo].BRFM_MATCH,CS);
	PR_BRFM_TUNE.Write(RECIPE[nStepNo].BRFM_TUNE,CS);

	sprintf(szTemp2, "BRF_CTRL = %s", RECIPE[nStepNo].BRF_CTRL);					Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_BRFG_POWER = %.0f", PR_BRFG_POWER.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_BRFM_TUNE = %.0f",	 PR_BRFM_TUNE.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_BRFM_MATCH = %.0f", PR_BRFM_MATCH.Read(CS));				Make_Log("", szTemp2, EVENT);
	
	PR_VVC_MOTOR.Write(RECIPE[nStepNo].VVC_MOTER_TABLE,CS);
	sprintf(szTemp2, "PR_VVC_MOTOR = %d", PR_VVC_MOTOR.Read(CS));				Make_Log("", szTemp2, EVENT);

	PR_VVC01_POS.Write(RECIPE[nStepNo].VVC01_POS,CS);
	PR_VVC02_POS.Write(RECIPE[nStepNo].VVC02_POS,CS);
	PR_VVC03_POS.Write(RECIPE[nStepNo].VVC03_POS,CS);
	PR_VVC04_POS.Write(RECIPE[nStepNo].VVC04_POS,CS);
	PR_VVC05_POS.Write(RECIPE[nStepNo].VVC05_POS,CS);
	PR_VVC06_POS.Write(RECIPE[nStepNo].VVC06_POS,CS);

	sprintf(szTemp2, "PR_VVC01_POS = %d", PR_VVC01_POS.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_VVC02_POS = %d", PR_VVC02_POS.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_VVC03_POS = %d", PR_VVC03_POS.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_VVC04_POS = %d", PR_VVC04_POS.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_VVC05_POS = %d", PR_VVC05_POS.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_VVC06_POS = %d", PR_VVC06_POS.Read(CS));				Make_Log("", szTemp2, EVENT);

	PR_HE_PRESS.Write(RECIPE[nStepNo].HE_PRESSURE,CS);
	PR_ESC_VOLT.Write(RECIPE[nStepNo].ESC_VOLT,CS);
	PR_HE_FLOW_ALARM.Write(RECIPE[nStepNo].HE_FLOW_ALARM,CS);

	sprintf(szTemp2, "HE_CTRL = %s", RECIPE[nStepNo].HE_CTRL);					Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_HE_PRESS = %.1f", PR_HE_PRESS.Read(CS));				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_HE_FLOW_ALARM = %.1f", PR_HE_FLOW_ALARM.Read(CS));		Make_Log("", szTemp2, EVENT);

	sprintf(szTemp2, "ESC_CTRL = %s", RECIPE[nStepNo].ESC_CTRL);				Make_Log("", szTemp2, EVENT);
	sprintf(szTemp2, "PR_ESC_VOLT = %.0f", PR_ESC_VOLT.Read(CS));				Make_Log("", szTemp2, EVENT);


	PR_EPD_RECIPENUM.Write(RECIPE[nStepNo].EPD_RCPNUM		, CS);
	PR_EPD_MIN_TIME.Write(RECIPE[nStepNo].EPD_MINTIME		, CS);	
	PR_EPD_STEPNUM.Write(RECIPE[nStepNo].EPD_STEPNUM		, CS);
	PR_EPD_ALARM.Write(RECIPE[nStepNo].EPD_ALARM			, CS);
	PR_OVERETCH_TYPE.Write(RECIPE[nStepNo].EPD_OVRETCHTYPE	, CS);
	PR_OVERETCH_VALUE.Write(RECIPE[nStepNo].EPD_OVRETCHTIME	, CS);
	sprintf(szTemp1, "STEP NUM   = %d", nStepNo);								Make_Log(szTemp1, "ENDED : RCP PARAMETER TO FUNCTION", EVENT);

	if		(strcmp(RECIPE[nStepNo].APC_CTRL, "NONE") == 0)		PR_APC_CTRL.Write(0 , CS);
	else if	(strcmp(RECIPE[nStepNo].APC_CTRL, "FULLOPEN") == 0)	PR_APC_CTRL.Write(1 , CS);
	else if	(strcmp(RECIPE[nStepNo].APC_CTRL, "PRESS") == 0)	PR_APC_CTRL.Write(2 , CS);
	else if	(strcmp(RECIPE[nStepNo].APC_CTRL, "POS") == 0)		PR_APC_CTRL.Write(3 , CS);
	else														PR_APC_CTRL.Write(0 , CS);


	return 1;
}


int SetRcpParamFristStep(int nStepNo)
{
	int CS = 0;
	PR_PIN_POS.Write( RECIPE[nStepNo].PIN_POS ,	CS);

	PR_CHILLER_CH01_TEMP.Write( RECIPE[nStepNo].CHILLER_CH01_TEMP ,	CS);
	PR_CHILLER_CH02_TEMP.Write( RECIPE[nStepNo].CHILLER_CH02_TEMP ,	CS);
	PR_CHILLER_CH03_TEMP.Write( RECIPE[nStepNo].CHILLER_CH03_TEMP ,	CS);

	return 1;
}


//----------------------------------------------------------------------------------------------------------------------------------------------
void SetRecipeInfo2StepControlChannel(int nStepNum)
{
	int  cs;
	int  i;
	char strTemp[512]="";
	View_CurStepNum.Write(0 , cs);
	
	RECIPEINFO.TOTALSTEP = g_nTotalStep;
	
	for(i=0; i<nStepNum; i++)
	{
		strcpy(strTemp, RECIPE[i+1].STEP_NAME);	
		
		SioAt(&exStepName_01 ,		i)->Write(strTemp , cs);
		DioAt(&exStepType_01 ,		i)->Write(RECIPE[i+1].STEP_ENDTYPE , cs);
		AioAt(&exStepTime_01 ,		i)->Write(RECIPE[i+1].STEP_TIME , cs);
		DioAt(&exStepRun_01 ,		i)->Write(1 , cs);
		AioAt(&exStepAddTime_01 ,	i)->Write(0 , cs);
		//
		DioAt(&STEP01_STEPNO ,		i)->Write(i+1 , cs);
		SioAt(&STEP01_DESC ,		i)->Write(strTemp , cs);
		//
		RECIPEINFO.STEPLOGGING[i] = TRUE;
		RECIPEINFO.STEPSUMMARY[i] = TRUE;
		RECIPEINFO.STEPREPORT[i] = TRUE;
		strcpy(RECIPEINFO.STEPNAME[i],strTemp);
	}

	Rcp_TotalStep.Write(g_nTotalStep, cs);
	Recipe_Max_Step.Write(g_nTotalStep, cs);
	
	return;
}


//----------------------------------------------------------------------------------------------------------------------------------------------
SEQ_STATUS RECIPE_READING(const void* pParam, int nNumOfBytesRecved) 
{
	int	iii , Total_Step_Count;
	int ii_return;
	BOOL	FindName = FALSE , DataStart = FALSE;
	int		nCS;
	int     GlassCheckOption;
	double  nDechcuk_Power;
	int     nIdleORNormal_Proc=0;
	int     nCountORNoraml_Proc;
	
	char    szTemp[512] = {0};
	TString strStepName;
	
	TRecipeAnal RCP( (void*)pParam, nNumOfBytesRecved );

	RCP.Print();
	
	STEP_COUNT = RCP.GetStepCount();
	RECIPEINFO.TOTALSTEP = STEP_COUNT;
	
	Total_Step_Count			=	0;
	TOTAL_TIME					=	0.0;
	g_bDechuck_powerState		=	TRUE;
	g_nDechuckStepCount			=	0;
	g_nDechuckStep				=	0;
	g_nIdleESCPowerOnState		=	TRUE;
	g_nPin_3Position_USESTEP	=	0;
	g_nPin_4Position_USESTEP	=	0;
	nDechuckStepNumber      	=	0;
	g_nEPDStepCount				=	0;

	//////////////////////////////////////////////////////////////////////////


	Total_Step_Count = STEP_COUNT;
	for( iii=0 ; iii<STEP_COUNT ; iii++) 
	{

			RECIPE[iii+1].Is_RF_Step  = -1; 	
			//------------------------------------------------------PROCESS STEP INFO. START
			strStepName.Format(_TEXT("%s") , RCP.GetStepItemString(iii , "PR_STEPNAME") );
			sprintf( RECIPE[iii+1].STEP_NAME    , "%s" , strStepName.CStr()  );		
			sprintf( RECIPEINFO.STEPNAME[iii+1] , "%s" , RECIPE[iii+1].STEP_NAME );
			//
			RECIPE[iii+1].STEP_ENDTYPE= RCP.GetStepItemInt(iii, "PR_STEPENDTYPE"	);	
			sprintf(szTemp, "RECIPE[%d].STEP_ENDTYPE = %d", iii, RECIPE[iii+1].STEP_ENDTYPE);	Make_Log("", szTemp, EVENT);

			if(RECIPE[iii+1].STEP_ENDTYPE == eTYPE_DISCH)
			{
				nDechuckStepNumber =iii;
				sprintf(szTemp, "DISCHAGE STEP[%d] IN ADD CEHCK", iii);	Make_Log("", szTemp, EVENT);
			}
			if(RECIPE[iii+1].STEP_ENDTYPE == eTYPE_EPD)
			{
				g_nEPDStepCount++;
			}
			

			RECIPE[iii+1].STEP_TIME= RCP.GetStepItemReal(iii, "PR_STEPTIME"	);	
			sprintf(szTemp, "RECIPE[%d].STEP_TIME = %.0f", iii, RECIPE[iii+1].STEP_TIME);	Make_Log("", szTemp, EVENT);
			
			TOTAL_TIME = TOTAL_TIME + RECIPE[iii+1].STEP_TIME;
			//

			//
			RECIPE[iii+1].GAS1= (int)RCP.GetStepItemReal(iii, "PR_GAS1"	);		
			sprintf(szTemp, "RECIPE[%d].GAS1 = %d", iii, RECIPE[iii+1].GAS1);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS2= (int)RCP.GetStepItemReal(iii, "PR_GAS2"	);			
			sprintf(szTemp, "RECIPE[%d].GAS2 = %d", iii, RECIPE[iii+1].GAS2);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS3= (int)RCP.GetStepItemReal(iii, "PR_GAS3");			
			sprintf(szTemp, "RECIPE[%d].GAS3 = %d", iii, RECIPE[iii+1].GAS3);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS4= (int)RCP.GetStepItemReal(iii, "PR_GAS4");		
			sprintf(szTemp, "RECIPE[%d].GAS4 = %d", iii, RECIPE[iii+1].GAS4);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS5= (int)RCP.GetStepItemReal(iii, "PR_GAS5");				
			sprintf(szTemp, "RECIPE[%d].GAS5 = %d", iii, RECIPE[iii+1].GAS5);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS6= (int)RCP.GetStepItemReal(iii, "PR_GAS6");		
			sprintf(szTemp, "RECIPE[%d].GAS6 = %d", iii, RECIPE[iii+1].GAS6);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS7= (int)RCP.GetStepItemReal(iii, "PR_GAS7");			
			sprintf(szTemp, "RECIPE[%d].GAS7 = %d", iii, RECIPE[iii+1].GAS7);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS8= (int)RCP.GetStepItemReal(iii, "PR_GAS8");		
			sprintf(szTemp, "RECIPE[%d].GAS8 = %d", iii, RECIPE[iii+1].GAS8);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS9= (int)RCP.GetStepItemReal(iii, "PR_GAS9");		
			sprintf(szTemp, "RECIPE[%d].GAS9 = %d", iii, RECIPE[iii+1].GAS9);		Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].GAS10= (int)RCP.GetStepItemReal(iii, "PR_GAS10");			
			sprintf(szTemp, "RECIPE[%d].GAS10 = %d", iii, RECIPE[iii+1].GAS10);		Make_Log("", szTemp, EVENT);

		
			//FRC
			RECIPE[iii+1].FRC_CENTER= (int)RCP.GetStepItemReal(iii, "PR_FRC_CENTER"	);		
			sprintf(szTemp, "RECIPE[%d].FRC_CENTER = %d", iii, RECIPE[iii+1].FRC_CENTER);	Make_Log("", szTemp, EVENT);
			//
			RECIPE[iii+1].FRC_MIDDLE= (int)RCP.GetStepItemReal(iii, "PR_FRC_MIDDLE"	);			
			sprintf(szTemp, "RECIPE[%d].FRC_MIDDLE = %d", iii, RECIPE[iii+1].FRC_MIDDLE);	Make_Log("", szTemp, EVENT);
	

			ii_return = RCP.GetStepItemInt(iii, "PR_SRF_CTRL"	);
			if	   (ii_return == 0)	sprintf(RECIPE[iii+1].SRF_CTRL, "NONE");
			else if(ii_return == 1)	sprintf(RECIPE[iii+1].SRF_CTRL, "POWEROFF");
			else if(ii_return == 2)	sprintf(RECIPE[iii+1].SRF_CTRL, "POWERON");
		
			sprintf(szTemp, "RECIPE[%d].SRF_CTRL = %s", iii, RECIPE[iii+1].SRF_CTRL);	Make_Log("", szTemp, EVENT);
			
			RECIPE[iii+1].SRFG_POWER= RCP.GetStepItemReal(iii, "PR_SRFG_POWER"	);

			GlassCheckOption	=	Glass_Existence_CHK.Read(nCS);
			nDechcuk_Power		=	Dechuck_SRF_Power.Read(nCS);
		
			if(RECIPE[iii+1].SRFG_POWER > 0)
			{

				if(RECIPE[iii+1].SRFG_POWER > nDechcuk_Power)	RECIPE[iii+1].Is_RF_Step = 1;
				else											RECIPE[iii+1].Is_RF_Step = 0;
	
				sprintf(szTemp, "RECIPE[%d].SRFG_POWER = %d, RECIPE[%d].Is_RF_Step = %d", iii, RECIPE[iii+1].SRFG_POWER, iii, RECIPE[iii+1].Is_RF_Step);
				Make_Log("", szTemp, EVENT);
				
			}
		
			if( (nIdleORNormal_Proc == IDLE_Process) || (nCountORNoraml_Proc == Count_Process))
			{
				
				if(nDechcuk_Power < RECIPE[iii+1].SRFG_POWER)
				{
					sprintf(szTemp, "EmpProcess Abort.  RECIPE[%d].SRFG_POWER(= %d) > %4.1f", iii, RECIPE[iii+1].SRFG_POWER,nDechcuk_Power);
					Make_Log("", szTemp, EVENT);
					g_bDechuck_powerState= FALSE;
					return SEQ_ABORT;
				}
			}
			else
			{
				if((GlassCheckOption !=eYesCheck) &&(nDechcuk_Power < RECIPE[iii+1].SRFG_POWER))
				{
					sprintf(szTemp, "Normal Process Abort.  RECIPE[%d].SRFG_POWER(= %d) > %4.1f", iii, RECIPE[iii+1].SRFG_POWER,nDechcuk_Power);
					Make_Log("", szTemp, EVENT);		
					g_bDechuck_powerState= FALSE;
					return SEQ_ABORT;
				}
			}
			//
			RECIPE[iii+1].SRFM_MATCH= RCP.GetStepItemReal(iii, "PR_SRFM_MATCH"	);			
			sprintf(szTemp, "RECIPE[%d].SRFM_MATCH = %d", iii, RECIPE[iii+1].SRFM_MATCH);	Make_Log("", szTemp, EVENT);
		
			RECIPE[iii+1].SRFM_TUNE= RCP.GetStepItemReal(iii, "PR_SRFM_TUNE"	);		
			sprintf(szTemp, "RECIPE[%d].SRFM_TUNE = %d", iii, RECIPE[iii+1].SRFM_TUNE);	Make_Log("", szTemp, EVENT);
		

			ii_return= RCP.GetStepItemInt(iii, "PR_BRF_CTRL"	);							
		
			if	   (ii_return == 0)	sprintf(RECIPE[iii+1].BRF_CTRL, "NONE");
			else if(ii_return == 1)	sprintf(RECIPE[iii+1].BRF_CTRL, "POWEROFF");
			else if(ii_return == 2)	sprintf(RECIPE[iii+1].BRF_CTRL, "POWERON");
			
			sprintf(szTemp, "RECIPE[%d].BRF_CTRL = %s", iii, RECIPE[iii+1].BRF_CTRL);	Make_Log("", szTemp, EVENT);


			RECIPE[iii+1].BRFG_POWER= RCP.GetStepItemReal(iii, "PR_BRFG_POWER"	);	
			if(RECIPE[iii+1].BRFG_POWER > 0)
			{
				sprintf(szTemp, "RECIPE[%d].BRFG_POWER = %d, RECIPE[%d].Is_RF_Step = %d", iii, RECIPE[iii+1].BRFG_POWER, iii, RECIPE[iii+1].Is_RF_Step);
				Make_Log("", szTemp, EVENT);
			}
			
			GlassCheckOption	=	Glass_Existence_CHK.Read(nCS);
			nDechcuk_Power		=	Dechuck_BRF_Power.Read(nCS);
			nIdleORNormal_Proc	=	EmpORNormal_Process.Read(nCS);
			nCountORNoraml_Proc =	CountORNormal_Process.Read(nCS);
			
			if( (nIdleORNormal_Proc == IDLE_Process) || (nCountORNoraml_Proc == Count_Process))
			{
				if(nDechcuk_Power < RECIPE[iii+1].BRFG_POWER)
				{
					sprintf(szTemp, "EmpProcess Abort.  RECIPE[%d].BRFG_POWER(= %d) > %4.1f", iii, RECIPE[iii+1].BRFG_POWER,nDechcuk_Power);
					Make_Log("", szTemp, EVENT);
					g_bDechuck_powerState= FALSE;
					return SEQ_ABORT;
				}
			}
			else
			{	
				if((GlassCheckOption !=eYesCheck) &&(nDechcuk_Power < RECIPE[iii+1].BRFG_POWER))
				{
					sprintf(szTemp, "Normal Process Abort.  RECIPE[%d].BRFG_POWER(= %d) > %4.1f", iii, RECIPE[iii+1].BRFG_POWER,nDechcuk_Power);
					Make_Log("", szTemp, EVENT);		
					g_bDechuck_powerState= FALSE;
					return SEQ_ABORT;
				}
			}
			

			RECIPE[iii+1].BRFM_MATCH= RCP.GetStepItemReal(iii, "PR_BRFM_MATCH"	);		
			sprintf(szTemp, "RECIPE[%d].BRFM_MATCH = %d", iii, RECIPE[iii+1].BRFM_MATCH);		Make_Log("", szTemp, EVENT);
		
			RECIPE[iii+1].BRFM_TUNE= RCP.GetStepItemReal(iii, "PR_BRFM_TUNE"	);		
			sprintf(szTemp, "RECIPE[%d].BRFM_TUNE = %d", iii, RECIPE[iii+1].BRFM_TUNE);			Make_Log("", szTemp, EVENT);
							
			RECIPE[iii+1].VVC_MOTER_TABLE= (int)RCP.GetStepItemInt(iii, "PR_VVC_MOTOR");		
			sprintf(szTemp, "RECIPE[%d].VVC_MOTER_TABLE = %d", iii, RECIPE[iii+1].VVC_MOTER_TABLE);		Make_Log("", szTemp, EVENT);

	
					
			RECIPE[iii+1].PIN_POS= RCP.GetStepItemReal(iii, "PR_PIN_POS"	);		
			sprintf(szTemp, "RECIPE[%d].PIN_POS = %d", iii, RECIPE[iii+1].PIN_POS);				Make_Log("", szTemp, EVENT);	
			

			RECIPE[iii+1].HE_FLOW_ALARM= RCP.GetStepItemReal(iii, "PR_HE_FLOW_ALARM"	);	
			sprintf(szTemp, "RECIPE[%d].HE_FLOW_ALARM = %.1f", iii, RECIPE[iii+1].HE_FLOW_ALARM);	Make_Log("", szTemp, EVENT);

			RECIPE[iii+1].VVC01_POS= RCP.GetStepItemReal(iii, "PR_VVC01_POS"	);	
			sprintf(szTemp, "RECIPE[%d].VVC01_POS = %.1f", iii, RECIPE[iii+1].VVC01_POS);	Make_Log("", szTemp, EVENT);	
			
			RECIPE[iii+1].VVC02_POS= RCP.GetStepItemReal(iii, "PR_VVC02_POS"	);	
			sprintf(szTemp, "RECIPE[%d].VVC02_POS = %.1f", iii, RECIPE[iii+1].VVC02_POS);	Make_Log("", szTemp, EVENT);	
		
			RECIPE[iii+1].VVC03_POS= RCP.GetStepItemReal(iii, "PR_VVC03_POS"	);	
			sprintf(szTemp, "RECIPE[%d].VVC03_POS = %.1f", iii, RECIPE[iii+1].VVC03_POS);	Make_Log("", szTemp, EVENT);	
		
			RECIPE[iii+1].VVC04_POS= RCP.GetStepItemReal(iii, "PR_VVC04_POS"	);	
			sprintf(szTemp, "RECIPE[%d].VVC04_POS = %.1f", iii, RECIPE[iii+1].VVC04_POS);	Make_Log("", szTemp, EVENT);	
		
			RECIPE[iii+1].VVC05_POS= RCP.GetStepItemReal(iii, "PR_VVC05_POS"	);	
			sprintf(szTemp, "RECIPE[%d].VVC05_POS = %.1f", iii, RECIPE[iii+1].VVC05_POS);	Make_Log("", szTemp, EVENT);	
	
			RECIPE[iii+1].VVC06_POS= RCP.GetStepItemReal(iii, "PR_VVC06_POS"	);	
			sprintf(szTemp, "RECIPE[%d].VVC06_POS = %.1f", iii, RECIPE[iii+1].VVC06_POS);	Make_Log("", szTemp, EVENT);	
	
			
			RECIPE[iii+1].CHILLER_CH01_TEMP= RCP.GetStepItemReal(iii, "PR_CHILLER_CH01_TEMP"	);	
			sprintf(szTemp, "RECIPE[%d].CHILLER_CH01_TEMP = %.1f", iii, RECIPE[iii+1].CHILLER_CH01_TEMP);	Make_Log("", szTemp, EVENT);	
			
			RECIPE[iii+1].CHILLER_CH02_TEMP= RCP.GetStepItemReal(iii, "PR_CHILLER_CH02_TEMP"	);	
			sprintf(szTemp, "RECIPE[%d].CHILLER_CH02_TEMP = %.1f", iii, RECIPE[iii+1].CHILLER_CH02_TEMP);	Make_Log("", szTemp, EVENT);	

			RECIPE[iii+1].CHILLER_CH03_TEMP= RCP.GetStepItemReal(iii, "PR_CHILLER_CH03_TEMP"	);	
			sprintf(szTemp, "RECIPE[%d].CHILLER_CH03_TEMP = %.1f", iii, RECIPE[iii+1].CHILLER_CH03_TEMP);	Make_Log("", szTemp, EVENT);	

		
			//
			ii_return= RCP.GetStepItemInt(iii, "PR_APC_CTRL"	);
			if	   (ii_return == 0)	sprintf(RECIPE[iii+1].APC_CTRL, "NONE");
			else if(ii_return == 1)	sprintf(RECIPE[iii+1].APC_CTRL, "FULLOPEN");
			else if(ii_return == 2)	sprintf(RECIPE[iii+1].APC_CTRL, "PRESS");
			else if(ii_return == 3)	sprintf(RECIPE[iii+1].APC_CTRL, "POS");

			sprintf(szTemp, "RECIPE[%d].APC_CTRL = %s", iii, RECIPE[iii+1].APC_CTRL);				Make_Log("", szTemp, EVENT);	
		
			//
			RECIPE[iii+1].PRESSURE= RCP.GetStepItemReal(iii, "PR_APC_PRESS"	);	
			sprintf(szTemp, "RECIPE[%d].PRESSURE = %f", iii, RECIPE[iii+1].PRESSURE);				Make_Log("", szTemp, EVENT);
			

				//
			RECIPE[iii+1].EPD_RCPNUM= RCP.GetStepItemReal(iii, "PR_EPD_RECIPENUM"	);
			sprintf(szTemp, "RECIPE[%d].EPD_RCPNUM = %d", iii, RECIPE[iii+1].EPD_RCPNUM);			Make_Log("", szTemp, EVENT);
			

			RECIPE[iii+1].EPD_MINTIME= RCP.GetStepItemReal(iii, "PR_EPD_MIN_TIME"	);
			sprintf(szTemp, "RECIPE[%d].EPD_MINTIME = %d", iii, RECIPE[iii+1].EPD_MINTIME);			Make_Log("", szTemp, EVENT);
		
			RECIPE[iii+1].EPD_STEPNUM= RCP.GetStepItemReal(iii, "PR_EPD_STEPNUM"	);	
			sprintf(szTemp, "RECIPE[%d].EPD_STEPNUM = %d", iii, RECIPE[iii+1].EPD_STEPNUM);			Make_Log("", szTemp, EVENT);
			
			RECIPE[iii+1].EPD_ALARM= RCP.GetStepItemReal(iii, "PR_EPD_ALARM"	);					Make_Log("", szTemp, EVENT);
			sprintf(szTemp, "RECIPE[%d].EPD_ALARM = %d", iii, RECIPE[iii+1].EPD_ALARM);
	

			RECIPE[iii+1].EPD_OVRETCHTYPE= RCP.GetStepItemInt(iii, "PR_OVERETCH_TYPE"	);			Make_Log("", szTemp, EVENT);
			sprintf(szTemp, "RECIPE[%d].EPD_OVRETCHTYPE = %d", iii, RECIPE[iii+1].EPD_OVRETCHTYPE);
	
			RECIPE[iii+1].EPD_OVRETCHTIME= RCP.GetStepItemReal(iii, "PR_OVERETCH_VALUE"	);			Make_Log("", szTemp, EVENT);
			sprintf(szTemp, "RECIPE[%d].EPD_OVRETCHTIME = %d", iii, RECIPE[iii+1].EPD_OVRETCHTIME);
				
			ii_return= RCP.GetStepItemInt(iii, "PR_ESC_CTRL"	);	
			nIdleORNormal_Proc	=	EmpORNormal_Process.Read(nCS);
			nCountORNoraml_Proc =	CountORNormal_Process.Read(nCS);
			
			if	   (ii_return == 0)		sprintf(RECIPE[iii+1].ESC_CTRL, "NONE");
			else if(ii_return == 1)		sprintf(RECIPE[iii+1].ESC_CTRL, "READY");
			else if(ii_return == 2)		
			{
				if( (nIdleORNormal_Proc == IDLE_Process) || (nCountORNoraml_Proc == Count_Process))
				{		
					sprintf(szTemp, "EmpProcess Abort. Recipe ESC Grip Use");
					Make_Log("", szTemp, EVENT);
					g_nIdleESCPowerOnState = FALSE;
					return SEQ_ABORT;
					
				}
				else
				{
					sprintf(RECIPE[iii+1].ESC_CTRL, "ESC_ON");
					
				}
				
			}
			else if(ii_return == 3)		sprintf(RECIPE[iii+1].ESC_CTRL, "ESC_OFF");
			sprintf(szTemp, "RECIPE[%d].ESC_CTRL = %s", iii, RECIPE[iii+1].ESC_CTRL);
			//
			RECIPE[iii+1].ESC_VOLT= RCP.GetStepItemReal(iii, "PR_ESC_VOLT"	);	;
			sprintf(szTemp, "RECIPE[%d].ESC_VOLT = %d", iii, RECIPE[iii+1].ESC_VOLT);
			//
			ii_return= RCP.GetStepItemInt(iii, "PR_HE_CTRL"	);
			{
				
				nIdleORNormal_Proc	=	EmpORNormal_Process.Read(nCS);
				nCountORNoraml_Proc =	CountORNormal_Process.Read(nCS);
				
				if	   (ii_return == 0)		sprintf(RECIPE[iii+1].HE_CTRL, "NONE");
				else if(ii_return == 1)		sprintf(RECIPE[iii+1].HE_CTRL, "CLOSE");
				else if(ii_return == 2)		sprintf(RECIPE[iii+1].HE_CTRL, "READY");
				else if(ii_return == 3)		sprintf(RECIPE[iii+1].HE_CTRL, "FLOW");
				else if(ii_return == 4)		sprintf(RECIPE[iii+1].HE_CTRL, "DUMP");
				
				if( ii_return == 2 ||  ii_return == 3 )
				{
					if( (nIdleORNormal_Proc == IDLE_Process) || (nCountORNoraml_Proc == Count_Process))
					{		
						sprintf(szTemp, "EmpProcess Abort. Recipe He Flow Use");
						Make_Log("", szTemp, EVENT);
						g_nIdleESCPowerOnState = FALSE;
						return SEQ_ABORT;										
					}
					
				}
				
			}

			sprintf(szTemp, "RECIPE[%d].HE_CTRL = %s", iii, RECIPE[iii+1].HE_CTRL);				Make_Log("", szTemp, EVENT);
		
			RECIPE[iii+1].HE_PRESSURE= RCP.GetStepItemReal(iii, "PR_HE_PRESS"	);	
			sprintf(szTemp, "RECIPE[%d].HE_PRESSURE = %.1f", iii, RECIPE[iii+1].HE_PRESSURE);	Make_Log("", szTemp, EVENT);		

		}
	
		if ( Total_Step_Count != STEP_COUNT ) 
		{
			sprintf(szTemp, "Recipe Data Error. Total_Step_Count(= %d), Step_Count(= %d)", Total_Step_Count, STEP_COUNT);
			Make_Log("", szTemp, FAULT);
			STEP_COUNT = 0;
		}
		
		g_nTotalStep = Total_Step_Count;
		
		SetRecipeInfo2StepControlChannel(g_nTotalStep);
		
		
		return SEQ_SUCCESS;
}



int _Is_Intlk_Sts()
{
	int ncs		= 0;

	char szTitle[256]	= {0};
	char szMsg1[256]	= {0};
	char szMsg[256]		= {0};

	int  nSWIntlk_Arr[6] = {0};
	int  nSWIntlk		= 0;
	int  idx			= 0;

	nSWIntlk_Arr[0]	= dSoftware_Intlk_Sts1.Read(ncs);
	nSWIntlk_Arr[1]	= dSoftware_Intlk_Sts2.Read(ncs);
	nSWIntlk_Arr[2]	= dSoftware_Intlk_Sts3.Read(ncs);
	nSWIntlk_Arr[3]	= dSoftware_Intlk_Sts4.Read(ncs);
	nSWIntlk_Arr[4]	= dSoftware_Intlk_Sts5.Read(ncs);
	nSWIntlk_Arr[5]	= dSoftware_Intlk_Sts6.Read(ncs);
	
	for(idx = 0; idx < 6; idx++)
	{
		nSWIntlk = nSWIntlk + nSWIntlk_Arr[idx];	
	}

	if(nSWIntlk > 0 || dSystem_Intlk_Sts.Read(ncs) == 1 || dHW_Interlock_On_Status.Read(ncs) == 1)
	{
		sprintf(szTitle, "(PROC) ABORT PROCESS( STEP = %d)", g_nCurrent_Step_Number);

		Make_Log("INTLK", szTitle, FAULT);

		sprintf(szMsg1, "STEP-%d, Interlock is happened.", g_nCurrent_Step_Number);
		sGUI_Proc_msg.Write(szMsg1 , ncs);

		if(dSystem_Intlk_Sts.Read(ncs) == 1)	
		{
			sprintf(szMsg1, "SYS INTLK Happened");
			Make_Log("INTLK", szMsg, FAULT);
		}

		if(dHW_Interlock_On_Status.Read(ncs) == 1)	
		{
			sprintf(szMsg1, "HW INTLK Happened");
			Make_Log("INTLK", szMsg, FAULT);
		}
	
		if( nSWIntlk > 0)
		{
			sprintf(szMsg1, "S/W INTLK %d, %d, %d, %d, %d %d Happened", nSWIntlk_Arr[0], nSWIntlk_Arr[1], nSWIntlk_Arr[2], nSWIntlk_Arr[3], nSWIntlk_Arr[4], nSWIntlk_Arr[5]);
			Make_Log("INTLK", szMsg, FAULT);
		}

		return 1;
	}

	return 0;
}

int _Timeout_Step(char* callFunction)
{
	char szTemp1[256] = {0};
	int  ncs = 0;

	sprintf(szTemp1, "STEP(%d), SET_TIME(%.0f), CURR_TIME(%.0f), (%s)TIMEOUT", 
					g_nCurrent_Step_Number, RECIPE[g_nCurrent_Step_Number].STEP_TIME, g_dblCurrentStepTime, callFunction);

	Make_Log("", szTemp1, FAULT);

	sGUI_Step_msg1.Write(_TEXT("TIMEOUT ALARM") , ncs);
	sGUI_Step_msg2.Write(szTemp1,ncs);


	return 1;
}

int _Timeout_Step_Condition(char* callFunction)
{
	char szTemp1[256] = {0};
	int  ncs = 0;
	
	sprintf(szTemp1, "STEP(%d), SET_TIME(%.0f), CURR_TIME(%.0f), (%s)TIMEOUT", 
		g_nCurrent_Step_Number, RECIPE[g_nCurrent_Step_Number].STEP_TIME, g_dblCurrentStepTime, callFunction);
	
	Make_Log("", szTemp1, FAULT);
	
	sGUI_Step_msg1.Write(_TEXT("TIMEOUT ALARM"),ncs);
	sGUI_Step_msg2.Write(szTemp1,ncs);	

	return 1;
}

int _Check_Intlk_Sts_Step(char* callFunction)
{
	int ncs = 0;	
	char szTitle[256]	= {0};
	char szMsg1[256]	= {0};
	char szMsg[256]		= {0};

	int  nSWIntlk_Arr[6] = {0};
	int  nSWIntlk		= 0;
	int  idx			= 0;

	nSWIntlk_Arr[0]	= dSoftware_Intlk_Sts1.Read(ncs);
	nSWIntlk_Arr[1]	= dSoftware_Intlk_Sts2.Read(ncs);
	nSWIntlk_Arr[2]	= dSoftware_Intlk_Sts3.Read(ncs);
	nSWIntlk_Arr[3]	= dSoftware_Intlk_Sts4.Read(ncs);
	nSWIntlk_Arr[4]	= dSoftware_Intlk_Sts5.Read(ncs);
	nSWIntlk_Arr[5]	= dSoftware_Intlk_Sts6.Read(ncs);

	for(idx = 0; idx < 6; idx++)
	{
		nSWIntlk = nSWIntlk + nSWIntlk_Arr[idx];
	}

	if(nSWIntlk > 0 || dSystem_Intlk_Sts.Read(ncs) == 1 || dHW_Interlock_On_Status.Read(ncs) == 1)
	{
		Make_Log("INTLK", "STARTED", EVENT);

		sprintf(szTitle, "(STEP = %d) %s : INTERLOCK ON", g_nCurrent_Step_Number, callFunction);

		Make_Log("", szTitle, FAULT);


		if(dSystem_Intlk_Sts.Read(ncs) == 1)	
		{
			sprintf(szMsg1, "SYS INTLK Happened");
			Make_Log("INTLK", szMsg, FAULT);
		}

		if(dHW_Interlock_On_Status.Read(ncs) == 1)	
		{
			sprintf(szMsg1, "HW INTLK Happened");
			Make_Log("INTLK", szMsg, FAULT);
		}

		if( nSWIntlk > 0)
		{
			sprintf(szMsg1, "S/W INTLK %d, %d, %d, %d, %d, %d Happened", nSWIntlk_Arr[0], nSWIntlk_Arr[1], nSWIntlk_Arr[2], nSWIntlk_Arr[3], nSWIntlk_Arr[4], nSWIntlk_Arr[5]);
			Make_Log("INTLK", szMsg, FAULT);
		}

		return 1;
	}

	return -1;
}


int _Is_Interlocked()
{
	int nCS				= 0;

	int  nSWIntlk_Arr[6] = {0};
	int  nSWIntlk		= 0;
	int  idx			= 0;

	nSWIntlk_Arr[0]	= dSoftware_Intlk_Sts1.Read(nCS);
	nSWIntlk_Arr[1]	= dSoftware_Intlk_Sts2.Read(nCS);
	nSWIntlk_Arr[2]	= dSoftware_Intlk_Sts3.Read(nCS);
	nSWIntlk_Arr[3]	= dSoftware_Intlk_Sts4.Read(nCS);
	nSWIntlk_Arr[4]	= dSoftware_Intlk_Sts5.Read(nCS);
	nSWIntlk_Arr[5]	= dSoftware_Intlk_Sts6.Read(nCS);
	
	for(idx = 0; idx < 6; idx++)
	{
		nSWIntlk = nSWIntlk + nSWIntlk_Arr[idx];
	}

	if(nSWIntlk > 0 || dSystem_Intlk_Sts.Read(nCS) > 0 || dHW_Interlock_On_Status.Read(nCS) == 1)
	{
		if(dSystem_Intlk_Sts.Read(nCS)	  == 1)		Make_Log("INTLK", "_Is_Interlocked() dSystem_Intlk_Sts is On", FAULT);	
		if(dSoftware_Intlk_Sts1.Read(nCS) == 1)		Make_Log("INTLK", "_Is_Interlocked() dSoftware_Intlk_Sts1 is On", FAULT);	
		if(dSoftware_Intlk_Sts2.Read(nCS) == 1)		Make_Log("INTLK", "_Is_Interlocked() dSoftware_Intlk_Sts2 is On", FAULT);	
		if(dSoftware_Intlk_Sts3.Read(nCS) == 1)		Make_Log("INTLK", "_Is_Interlocked() dSoftware_Intlk_Sts3 is On", FAULT);	
		if(dSoftware_Intlk_Sts4.Read(nCS) == 1)		Make_Log("INTLK", "_Is_Interlocked() dSoftware_Intlk_Sts4 is On", FAULT);	
		if(dSoftware_Intlk_Sts5.Read(nCS) == 1)		Make_Log("INTLK", "_Is_Interlocked() dSoftware_Intlk_Sts5 is On", FAULT);
		if(dSoftware_Intlk_Sts6.Read(nCS) == 1)		Make_Log("INTLK", "_Is_Interlocked() dSoftware_Intlk_Sts6 is On", FAULT);
		if(dHW_Interlock_On_Status.Read(nCS)== 1)	Make_Log("INTLK", "_Is_Interlocked() dHW_Interlock_On_Status is On", FAULT);	

		return 1;
	}


	return -1;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
int GetResultOfFunction(int nGetMode)
{
	int index   = 0;
	int nResult = 0;

	for(index = 0; index < 8; index++)	
	{
		g_nFunctionResult[index] = 0;
	}

	
	if(FNC_MFC.Status() == SEQ_SUCCESS)	g_nFunctionResult[0] =  1;
	else										
	{
		if(nGetMode == 1) 
		{
			PROCESS_1101.PostAnyTh();
			Make_Log("", "FNC_MFC is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[0] = -1;
	}

	if(FNC_APC.Status() == SEQ_SUCCESS)	g_nFunctionResult[1] =  1;
	else
	{
		if(nGetMode == 1)
		{
			PROCESS_1100.PostAnyTh();
			Make_Log("", "FNC_APC is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[1] = -1;
	}

	if(FNC_SRF.Status() == SEQ_SUCCESS)	g_nFunctionResult[2] =  1;
	else										
	{
		if(nGetMode == 1)
		{
			PROCESS_1104.PostAnyTh();
			Make_Log("", "FNC_SRF is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[2] = -1;
	}

	if(FNC_BRF.Status() == SEQ_SUCCESS)	g_nFunctionResult[3] =  1;
	else
	{
		if(nGetMode == 1)
		{
			PROCESS_1105.PostAnyTh();
			Make_Log("", "FNC_BRF is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[3] = -1;
	}

	if(FNC_ESC.Status() == SEQ_SUCCESS)	g_nFunctionResult[4] =  1;
	else										
	{
		if(nGetMode == 1) 
		{
			PROCESS_1102.PostAnyTh();
			Make_Log("", "FNC_ESC is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[4] = -1;
	}

	if(FNC_HE.Status()  == SEQ_SUCCESS)	g_nFunctionResult[5] =  1;
	else										
	{
		if(nGetMode == 1) 
		{
			PROCESS_1103.PostAnyTh();
			Make_Log("", "FNC_HE is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[5] = -1;
	}

	
	if(FNC_PIN.Status() == SEQ_SUCCESS)	g_nFunctionResult[6] =  1;
	else                                        
	{

		if(nGetMode == 1) 
		{
			PROCESS_1106.PostAnyTh();
			Make_Log("", "FNC_PIN is not SEQ_SUCCESS", FAULT);
		}

		g_nFunctionResult[6] = -1;
	}

	if(FNC_RF_CTRL.Status() == SEQ_SUCCESS)	g_nFunctionResult[7] =  1;
	else                                        
	{
		if(nGetMode == 1) 
		{
			PROCESS_1108.PostAnyTh();
			Make_Log("", "FNC_RF_CTRL is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[7] = -1;
	}


	for(index = 0; index < 8; index ++)
	{
		nResult = nResult + g_nFunctionResult[index];
	}

	return nResult;
}


int GetResultOfFunction_FINISH_STEP(int nGetMode)
{
	int index   = 0;
	int nResult = 0;
	
	for(index = 0; index < 7; index++)	
	{
		g_nFunctionResult[index] = 0;
	}
	
	if(FNC_MFC.Status() == SEQ_SUCCESS)	g_nFunctionResult[0] =  1;
	else										
	{
		if(nGetMode == 1) 
		{
			PROCESS_1101.PostAnyTh();
			Make_Log("", "FNC_MFC is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[0] = -1;
	}
	
	if(FNC_APC.Status() == SEQ_SUCCESS)	g_nFunctionResult[1] =  1;
	else
	{
		if(nGetMode == 1)
		{
			PROCESS_1100.PostAnyTh();
			Make_Log("", "FNC_APC is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[1] = -1;
	}
	
	if(FNC_SRF.Status() == SEQ_SUCCESS)	g_nFunctionResult[2] =  1;
	else										
	{
		if(nGetMode == 1)
		{
			PROCESS_1104.PostAnyTh();
			Make_Log("", "FNC_SRF is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[2] = -1;
	}
	
	if(FNC_BRF.Status() == SEQ_SUCCESS)	g_nFunctionResult[3] =  1;
	else
	{
		if(nGetMode == 1)
		{
			PROCESS_1105.PostAnyTh();
			Make_Log("", "FNC_BRF is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[3] = -1;
	}
	
	if(FNC_ESC.Status() == SEQ_SUCCESS)	g_nFunctionResult[4] =  1;
	else										
	{
		if(nGetMode == 1) 
		{
			PROCESS_1102.PostAnyTh();
			Make_Log("", "FNC_ESC is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[4] = -1;
	}
	
	if(FNC_HE.Status()  == SEQ_SUCCESS)	g_nFunctionResult[5] =  1;
	else										
	{
		if(nGetMode == 1) 
		{
			PROCESS_1103.PostAnyTh();
			Make_Log("", "FNC_HE is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[5] = -1;
	}
	
	
	if(FNC_RF_CTRL.Status() == SEQ_SUCCESS)	g_nFunctionResult[6] =  1;
	else                                        
	{
		if(nGetMode == 1) 
		{
			PROCESS_1108.PostAnyTh();
			Make_Log("", "FNC_RF_CTRL is not SEQ_SUCCESS", FAULT);
		}
		g_nFunctionResult[6] = -1;
	}


	for(index = 0; index < 7; index ++)
	{
		nResult = nResult + g_nFunctionResult[index];
	}
	
	return nResult;
}


int CheckAbortSts()
{
	int nResult = 0;
	int ncs = 0;
	int alarm_id = 0;
	char szTemp[256] = {0};
	int FnResult = 0;

	FnResult = FNC_MFC.Status();
	if(FnResult == SEQ_ABORT)
	{
		alarm_id = ALARM_STEP_MFC_FAIL;
		Make_Log("CheckAbortSts", "FNC_MFC == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("MFC Function is aborted"));
		nResult = 1;
	}

	FnResult = FNC_APC.Status();
	if(FnResult == SEQ_ABORT)	
	{
		alarm_id = ALARM_STEP_APC_FAIL;
		Make_Log("CheckAbortSts", "FNC_APC == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("APC Function is aborted"));
		nResult = 2;
	}

	FnResult = FNC_SRF.Status();
	if(FnResult == SEQ_ABORT)
	{
		alarm_id = ALARM_STEP_SRF_FAIL;
		Make_Log("CheckAbortSts", "FNC_SRF == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("SOURCE RF Function is aborted"));
		nResult = 3;
	}

	FnResult =FNC_BRF.Status();
	if(FnResult == SEQ_ABORT)	
	{
		alarm_id = ALARM_STEP_BRF_FAIL;
		Make_Log("CheckAbortSts", "FNC_BRF == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("BIAS RF Function is aborted"));
		nResult = 4;
	}

	FnResult = FNC_ESC.Status();
	if(FnResult == SEQ_ABORT)
	{
		alarm_id = ALARM_STEP_ESC_FAIL;
		Make_Log("CheckAbortSts", "FNC_ESC == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("ESC Function is aborted"));
		nResult = 5;
	}
	
	FnResult= FNC_HE.Status();
	if(FnResult == SEQ_ABORT)
	{
		alarm_id = ALARM_STEP_HE_FAIL;
		Make_Log("CheckAbortSts", "FNC_HE == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("He Function is aborted"));
		nResult = 6;
	}

	FnResult = FNC_PIN.Status();
	if(FnResult == SEQ_ABORT)	
	{
		alarm_id = ALARM_STEP_PIN_FAIL;
		Make_Log("CheckAbortSts", "FNC_PIN == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("Pin Function is aborted"));
		nResult = 7;
	}

	FnResult = FNC_RF_CTRL.Status();
	if(FnResult == SEQ_ABORT)	
	{
		Make_Log("CheckAbortSts", "FNC_RF_CTRL == SYS_ABORTED", FAULT);
		sGUI_Proc_msg.Write(_TEXT("RF_CTRL Function is aborted"));
		nResult = 8;
	}

	if(nResult> 0)
	{
		sprintf(szTemp, "(STEP = %d) RESULT = %d Fail to Run Function", g_nCurrent_Step_Number, nResult);
		Make_Log("CheckAbortSts", szTemp, FAULT);
	}

	return nResult;
}

int End_CondStable()
{
	TTimer CondStableTimer;

	int		nCS				= 0;
	int		nAlarm_Action	= 0;
	int		nCheckSts		= 0;
	int		nAPCSetPressure = 0;
	int		nCheckAPC		= 0;
	int		nCheckMFC		= 0;
	int		nCheckHe		= 0;
	int		nProcResult		= 0;
	int		nInterlocked	= 0;

	double	dblGetData		= 0;
	double	dblAPCSensor1	= 0;
	double	dblAPCSensor2	= 0;
	double	dblAPCPostion	= 0;
	double	nAPCSetPostion	= 0;	
	double	dElapsedTime	= 0.0;
	double	dStepTime		= 0.0;
	double	dRcpStepTime	= 0.0;
	double  APCCheckLevel   = 0;

	char szTemp[256]		= {0};


	Make_Log("END_CONDSTABLE", "STARTED", EVENT);
	
	CondStableTimer.Start();

	dElapsedTime = Step_CurTime.Read(nCS);

	nAPCSetPressure	= RECIPE[g_nCurrent_Step_Number].PRESSURE;
	nCheckHe		 = 99;
	
	dRcpStepTime = RECIPE[g_nCurrent_Step_Number].STEP_TIME;
	dStepTime	 = dRcpStepTime;

	do 
	{

		if(PM_PROC_CTRL.Read(nCS) == ePROC_ABORT)
		{
			Abort();
			Make_Log("", "PM_PROC_CTRL(ePROC_ABORT [1])", EVENT);
			return -1;
		}

		if(strcmp(RECIPE[g_nCurrent_Step_Number].APC_CTRL, "PRESS") == 0)
		{
			if(nAPCSetPressure != 0 && nAPCSetPostion ==0)
			{
		
				if(nAPCSetPressure < (int)cCDGUseLevel.Read(nCS) )		dblGetData = eAI_APC_Sensor2.Read(nCS);
				else													dblGetData = eAI_APC_Sensor1.Read(nCS);
				
				APCCheckLevel = cCDGConditionStepLevel.Read(nCS);
	
				if((dblGetData < nAPCSetPressure * (1.0 + APCCheckLevel/100)) && (dblGetData > nAPCSetPressure * (1.0 - APCCheckLevel/100))) nCheckAPC++;
				else																														 nCheckAPC = 0;
			
			}
		}
		else if(strcmp(RECIPE[g_nCurrent_Step_Number].APC_CTRL, "POS") == 0)
		{

			if(nAPCSetPostion !=0 && nAPCSetPressure == 0)
			{
				dblGetData=eAI_APC_Position.Read(nCS);

				if((dblGetData < nAPCSetPostion * 1.2) && (dblGetData > nAPCSetPostion * 0.8))
				{
					nCheckAPC++;
				}
				else
				{
					nCheckAPC = 0;
				}

			}		
			
		}
		else if(strcmp(RECIPE[g_nCurrent_Step_Number].APC_CTRL, "FULLOPEN") == 0)
		{
			dblGetData = eAI_APC_Position.Read(nCS);
		
			if(dblGetData > 999)
			{
				nCheckAPC++;
			}
			else
			{
				nCheckAPC = 0;
			}
		}
		else
		{
			nCheckAPC = 99;
		}

		if(GetResultOfFunction(0) == 8)
		{
			if(nCheckAPC > 1  && nCheckHe > 1 )
			{
				sprintf(szTemp, "nCheckAPC = %d, nCheckHe = %d", nCheckAPC,  nCheckHe);
				Make_Log("", szTemp, EVENT);
				
				Make_Log("END_CONDSTABLE", "ENDED - 1", EVENT);
				return 1;
			}
		}
		

		nFunctionAbortCheck = CheckAbortSts();
		if(nFunctionAbortCheck > 0)			
		{
			Abort();
			Make_Log("END_CONDSTABLE", "ENDED - 3", FAULT);
			return -1;
		}

		if(_Is_Interlocked() > 0)					
		{
			_Check_Intlk_Sts_Step("END_CONDSTABLE");	
			Abort();

			Make_Log("END_CONDSTABLE", "ENDED - 4", EVENT);
			return -1;
		}


		g_dblCurrentStepTime    = dElapsedTime + CondStableTimer.ElapsedTime()/1000;
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

		Step_CurTime.Write(g_dblCurrentStepTime,nCS);
		
		if(g_nCurrent_Step_Number > 0)	
		{
			Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);
		}

		if(WaitAbort(100))	return SEQ_ABORT;

	}while(RECIPE[g_nCurrent_Step_Number].STEP_TIME > g_dblCurrentStepTime);
	
	if(RECIPE[g_nCurrent_Step_Number].Is_RF_Step >  0)
	{
		g_dblTotalProcessTime += g_dblCurrentStepTime;
	}

	eAO_TotalEtchTime.Write(g_dblTotalProcessTime,nCS);


	//////////////////////////////////////////////////////////////////////////
	if( FNC_MFC.Status() == SEQ_RUNNING)
	{

		for(int nMFC_FunctionCount=1; nMFC_FunctionCount<=100 ; nMFC_FunctionCount++)
		{
			_sleep(100* nMFC_FunctionCount );
			sprintf(szTemp, "MFC FUNCTION RUNNING.. %d Counting,", nMFC_FunctionCount);
			Make_Log("", szTemp, EVENT);

			if (FNC_MFC.Status() !=SEQ_RUNNING) 
			{
				Make_Log("","MFC FUNCTION RUNNING Check End.", EVENT);
				break;
			}

		}
	}


	//////////////////////////////////////////////////////////////////////////
	if( FNC_HE.Status() == SEQ_RUNNING)
	{
		for(int nESCHe_FunctionCount=1; nESCHe_FunctionCount<=50 ; nESCHe_FunctionCount++)
		{
			_sleep(100* nESCHe_FunctionCount );
			sprintf(szTemp, "ESC_HE FUNCTION RUNNING.. %d Counting,", nESCHe_FunctionCount);
			Make_Log("", szTemp, EVENT);
			
			if (FNC_HE.Status() !=SEQ_RUNNING) 
			{
				Make_Log("","HE FUNCTION RUNNING Check End.", EVENT);
				
				_sleep(200);
				break;
			}	
		}
	}


	if(GetResultOfFunction(1) == 8)	
	{
		if(nCheckAPC > 1  && nCheckHe > 1 )
		{
			sprintf(szTemp, "nCheckAPC = %d, nCheckHe = %d", nCheckAPC, nCheckHe);
			Make_Log("", szTemp, EVENT);
			
			Make_Log("END_CONDSTABLE", "ENDED - 2", EVENT);
			return 1;
		}
		else
		{
			if(nCheckAPC < 1)
			{
				dblAPCSensor1 = eAI_APC_Sensor1.Read(nCS);
				dblAPCSensor2 = eAI_APC_Sensor2.Read(nCS);
				dblAPCPostion= eAI_APC_Position.Read(nCS);

				if(nAPCSetPressure != 0 && nAPCSetPostion ==0)	
				{
					sprintf(szTemp, "APC SetPressure =%d, dblAPCSensor1 = %.2f, dblAPCSensor2 = %.2f ", nAPCSetPressure, dblAPCSensor1, dblAPCSensor2);
				}
				else if(nAPCSetPressure == 0 && nAPCSetPostion !=0)
				{
					sprintf(szTemp, "APC SetPostion =%.0f, dblAPCPostion=%.2f ", nAPCSetPostion, dblAPCPostion);
				}

				Make_Log("", szTemp, FAULT);
			}

			
			if(nCheckAPC < 1  )
			{
				 sprintf(Condition_Step_Alarm, "Process aborted by timeout. Check APC Pressure");
			}		
			else
			{
				sprintf(Condition_Step_Alarm,"Process aborted by timeout");
				
			}
			
			sprintf(szTemp, "Check !! nCheckAPC = %d, nCheckHe = %d, nCheckCMN = %d", nCheckAPC,nCheckHe);
			Make_Log("", szTemp, FAULT);

		}
	}
	else
	{
		sprintf(szTemp, "GetResultOfFunction() = %d is failed", GetResultOfFunction(1));
		Make_Log("", szTemp, FAULT);
	}


	if(_Is_Interlocked() > 0)									
	{
	
		nAlarm_Action = _Check_Intlk_Sts_Step("END_CONDSTABLE");
		Abort();
		
		Make_Log("", "Condition Step Interlock is on", FAULT);
	}
	else
	{
		nAlarm_Action = _Timeout_Step_Condition("END_CONDSTABLE");	
		Abort();

		Make_Log("", "Condition Step Timeout Error", FAULT);
	}


	g_dblCurrentStepTime    = dElapsedTime + CondStableTimer.ElapsedTime()/1000;
	g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

	Step_CurTime.Write(g_dblCurrentStepTime,nCS);
	
	if(g_nCurrent_Step_Number > 0)	
	{
		Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);
	}

	Make_Log("END_CONDSTABLE", "ENDED - 6", EVENT);

	return -1;
}


int End_Discharge()	
{
	TTimer DischargeTimer;

	int		nCS				=	0;
	int		nAlarm_Action	=	0;
	int		nResult			=	0;
	int		nCheckSts		=	0;
	int		nInterlocked	=	0;
	int		nTimeout		=	50;	

	int		nRFOnSts1, nRFOnSts2;

	double	dElapsedTime	=	0.0;
	double	dStepTime		=	0.0;
	double	dRcpStepTime	=	0.0;

	char	szTemp[256]		=	{0};

	Make_Log("END_DISCHARGE", "STARTED", EVENT);
	
	DischargeTimer.Start();
	dElapsedTime = Step_CurTime.Read(nCS);
	
	dRcpStepTime = RECIPE[g_nCurrent_Step_Number].STEP_TIME;
	dStepTime	 = dRcpStepTime;

	while(nTimeout > 0)
	{

		if(PM_PROC_CTRL.Read(nCS) == ePROC_ABORT)
		{
			Abort();
			Make_Log("", "PM_PROC_CTRL(ePROC_ABORT)[1]", EVENT);
			return -1;
		}
		
		nFunctionAbortCheck = CheckAbortSts();
		if(nFunctionAbortCheck > 0)			
		{
			Abort();
			
			Make_Log("END_DISCHARGE", "ENDED", EVENT);
			return -1;
		}


		if(_Is_Interlocked() > 0)								
		{
			nAlarm_Action = _Check_Intlk_Sts_Step("END_DISCHARGE");	
			
			Abort();	
			Make_Log("END_DISCHARGE", "ENDED", EVENT);
			return -1;
		}

		if(rDO_SOURCE_RFG_POWER_ON.Read(nCS) == 1 && eAI_SRFG_FwdPwr.Read(nCS) > 100)
		{
			nRFOnSts1 = DI_SOURCE_RFG_POWER_ON.Read(nCS);
			nRFOnSts2 = rDO_SOURCE_RFG_POWER_ON.Read(nCS);
			
			sprintf(szTemp, "DI_SOURCE_RFG_POWER_ON[ %d ], rDO_SOURCE_RFG_POWER_ON[ %d ]", nRFOnSts1, nRFOnSts2);
			Make_Log("", szTemp, EVENT);

			nResult = 1;
			break;
		}

		g_dblCurrentStepTime    = dElapsedTime + DischargeTimer.ElapsedTime()/1000 ;
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

		Step_CurTime.Write(g_dblCurrentStepTime,nCS);
		
		if(g_nCurrent_Step_Number > 0)	
		{
			Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);
		}

		nTimeout--;
	
		if( WaitAbort(100) )	return SEQ_ABORT;
	}
	
	if(nResult == 1)
	{
		Make_Log("", "RF Power is on. Now Pin is moving", EVENT);
		_sleep(500);
		FNC_PIN.RunTh(_TEXT("DISCHARGE"));

	}
	else
	{
		nRFOnSts1 = DI_SOURCE_RFG_POWER_ON.Read(nCS);
		nRFOnSts2 = rDO_SOURCE_RFG_POWER_ON.Read(nCS);
		
		sprintf(szTemp, "DI_SOURCE_RFG_POWER_ON[ %d ], rDO_SOURCE_RFG_POWER_ON[ %d ]", nRFOnSts1, nRFOnSts2);
		Make_Log("", szTemp, FAULT);

		Make_Log("", "RF Power is not on. Pin moving is canceled", FAULT);
	}

	do 
	{
		if(DI_Pin_Busy.Read(nCS) == 1 && rDO_SOURCE_RFG_POWER_ON.Read(nCS) == 0)
		{
			FNC_PIN.Abort();
			Make_Log("", "SRC RF is off. pin moving is aborted. Set DO_Pin_EMGStop(= 1)", FAULT);
			DO_Pin_EMGStop.Write(1,nCS);
			
			_sleep(250);
		
			DO_Pin_EMGStop.Write(0,nCS);
			Abort();
			Make_Log("END_DISCHARGE", "ENDED", EVENT);

			return -1;
		}

		
		if(cDI_DischargeStep_Cond.Read(nCS) == DischargeStep_CheckPinPos)
		{
			if(GetResultOfFunction(0) == 8)
			{
				Make_Log("END_DISCHARGE", "ENDED", EVENT);
				return 1;
			}
		}

		if(PM_PROC_CTRL.Read(nCS) == ePROC_ABORT)
		{
			Abort();
			Make_Log("", "PM_PROC_CTRL(ePROC_ABORT)[2]", EVENT);
			return -1;
		}

		nFunctionAbortCheck = CheckAbortSts();
		if(nFunctionAbortCheck > 0)		
		{
			Abort();
			Make_Log("END_DISCHARGE", "ENDED", EVENT);
			return -1;
		}

		if(_Is_Interlocked() > 0)								
		{
			nAlarm_Action = _Check_Intlk_Sts_Step("END_DISCHARGE");

			Abort();
			Make_Log("END_DISCHARGE", "ENDED", EVENT);
			return -1;
		}

		g_dblCurrentStepTime    = dElapsedTime + DischargeTimer.ElapsedTime()/1000;
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

		Step_CurTime.Write(g_dblCurrentStepTime,nCS);

		if(g_nCurrent_Step_Number > 0)	
		{
			Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);
		}

		if(WaitAbort(100))	return SEQ_ABORT;

	}while(RECIPE[g_nCurrent_Step_Number].STEP_TIME > g_dblCurrentStepTime);


	if(GetResultOfFunction(1) == 8)
	{
		Make_Log("END_DISCHARGE", "ENDED", EVENT);
		return 1;
	}
	else
	{
		Abort();
		sprintf(szTemp, "GetResultOfFunction() %d is failed", GetResultOfFunction(1));
		Make_Log("", szTemp, FAULT);
	}

	if(_Is_Interlocked() > 0)	
	{
		nAlarm_Action = _Check_Intlk_Sts_Step("END_DISCHARGE");	
		Abort();
		Make_Log("", "Discharge Step Interlock is on", FAULT);
	}
	else
	{
		nAlarm_Action = _Timeout_Step("END_DISCHARGE");			
		Abort();
		Make_Log("", "Discharge Step Timeout Error", FAULT);
	}


	g_dblCurrentStepTime    = dElapsedTime + DischargeTimer.ElapsedTime()/1000;
	g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

	Step_CurTime.Write(g_dblCurrentStepTime,nCS);
	
	if(g_nCurrent_Step_Number > 0)		
	{
		Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);
	}

	Make_Log("END_DISCHARGE", "ENDED", FAULT);
	return -1;
}



int End_Finish()
{
	char szTemp[256] = {0};
	int nCS = 0;
	int nAlarm_Action = 0;
	int nCheckSts = 0;

	double dElapsedTime = 0.0;

	int nCfgPos;
	int nPinCheckMode;

	double dCfgPos;
	
	double dStepTime = 0.0;
	double dRcpStepTime = 0.0;
	int nInterlocked = 0;
	TTimer FinishTimer;
	//-------------------------------------

	Make_Log("END_FINISH", "STARTED", EVENT);
	
	FinishTimer.Start();
	dElapsedTime = Step_CurTime.Read(nCS);
	
	dRcpStepTime = RECIPE[g_nCurrent_Step_Number].STEP_TIME;
	dStepTime	 = dRcpStepTime;
	//------------------------------------------------------------

	do 
	{		
		if(GetResultOfFunction_FINISH_STEP(0) == 7 )
		{
			nCfgPos			= cDI_ProcEnd_dPinPos.Read(nCS);
			nPinCheckMode	= cDI_ProcEnd_PinCond.Read(nCS);
			dCfgPos			= cAI_ProcEnd_aPinPos.Read(nCS);

			if(nPinCheckMode == ePIN_BOTH_CHECK)		
			{
				if(XI_Pin_CurPosition.Read(nCS) >= nCfgPos)
				{
					if(AI_CPin1Pos.Read(nCS) > dCfgPos - 1 && AI_CPin1Pos.Read(nCS) < dCfgPos + 1)
					{
						Make_Log("Pin Position Ok - Digital, Analog", "ENDED", EVENT);
						Make_Log("END_FINISH", "ENDED", EVENT);
						return 1;
					}
				}
			}
			else if(nPinCheckMode  == ePIN_AIO_CHECK)	// Analog I/O AI_PIN0_CurPosition 
			{
				if(AI_CPin1Pos.Read(nCS) > dCfgPos - 1 && AI_CPin1Pos.Read(nCS) < MAX_ANALOG_PIN_POS)
				{
					Make_Log("Pin Position Ok - Analog", "ENDED", EVENT);
					Make_Log("END_FINISH", "ENDED", EVENT);
					return 1;
				}
			}
			else										// Digital I/O XI_PIN_CurPosition 
			{
				if(XI_Pin_CurPosition.Read(nCS) >= nCfgPos)	//
				{
					Make_Log("Pin Position Ok - Digital", "ENDED", EVENT);
					Make_Log("END_FINISH", "ENDED", EVENT);
					return 1;
				}
			}
		}
		else
		{

			sprintf(szTemp, "GetResultOfFunction() = %d is Failed", GetResultOfFunction_FINISH_STEP(0));
			Make_Log("", szTemp, FAULT);

		}

		if(PM_PROC_CTRL.Read(nCS) == ePROC_ABORT)
		{
			Abort();

			Make_Log("", "PM_PROC_CTRL(ePROC_ABORT)", EVENT);
			return -1;
		}

		nFunctionAbortCheck = CheckAbortSts();
		if(nFunctionAbortCheck > 0)			
		{
			Abort();

			Make_Log("END_FINISH", "ENDED", EVENT);
			return -1;
		}

		if(_Is_Interlocked() > 0)
		{
			nAlarm_Action = _Check_Intlk_Sts_Step("END_FINISH");

			Abort();

			Make_Log("END_FINISH", "ENDED", EVENT);
			return -1;
		}

		
		g_dblCurrentStepTime    = dElapsedTime + FinishTimer.ElapsedTime()/1000 ;
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

		Step_CurTime.Write(g_dblCurrentStepTime,nCS);
		
		if(g_nCurrent_Step_Number > 0)	Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);

		if(WaitAbort(100))	
		{
			return SEQ_ABORT;
		}

	}while(RECIPE[g_nCurrent_Step_Number].STEP_TIME > g_dblCurrentStepTime);
	

	if(GetResultOfFunction_FINISH_STEP(1) == 7)	
	{
		if(GetResultOfFunction_FINISH_STEP(0) == 7 )
		{
			nCfgPos			= cDI_ProcEnd_dPinPos.Read(nCS);
			nPinCheckMode	= cDI_ProcEnd_PinCond.Read(nCS);;
			dCfgPos			= cAI_ProcEnd_aPinPos.Read(nCS);
			
			if(nPinCheckMode == ePIN_BOTH_CHECK)		// Digital I/O XI_PIN_CurPosition 
			{
				if(XI_Pin_CurPosition.Read(nCS) == nCfgPos)
				{
					if(AI_CPin1Pos.Read(nCS) > dCfgPos - 1 && AI_CPin1Pos.Read(nCS) < dCfgPos + 1)
					{
						Make_Log("Pin Position Ok - Digital, Analog", "ENDED", EVENT);
						Make_Log("END_FINISH", "ENDED", EVENT);
						return 1;
					}
				}
			}
			else if(nPinCheckMode  == ePIN_AIO_CHECK)	// Analog I/O AI_PIN0_CurPosition 
			{
				if(AI_CPin1Pos.Read(nCS) > dCfgPos - 1 && AI_CPin1Pos.Read(nCS) < MAX_ANALOG_PIN_POS)
				{
					Make_Log("Pin Position Ok - Analog", "ENDED", EVENT);
					Make_Log("END_FINISH", "ENDED", EVENT);
					return 1;
				}
			}
			else										// Digital I/O XI_PIN_CurPosition 
			{
				if(XI_Pin_CurPosition.Read(nCS) == nCfgPos)
				{
					Make_Log("Pin Position Ok - Analog", "ENDED", EVENT);
					Make_Log("END_FINISH", "ENDED", EVENT);
					return 1;
				}
			}
		}

		return 1;
	}
	else
	{
		sprintf(szTemp, "GetResultOfFunction() = %d is Failed", GetResultOfFunction_FINISH_STEP(1));
		Make_Log("", szTemp, FAULT);
	}
	
	if(_Is_Interlocked() > 0)
	{
		nAlarm_Action = _Check_Intlk_Sts_Step("END_FINISH");
		
		Abort();
		
		Make_Log("", "Finish Step Interlock is on", FAULT);
	}
	else
	{
		nAlarm_Action = _Timeout_Step("END_FINISH");	

		Abort();
		
		Make_Log("", "Finish Step Timeout Error", FAULT);
	}

	g_dblCurrentStepTime    = dElapsedTime + FinishTimer.ElapsedTime()/1000;
	g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

	Step_CurTime.Write(g_dblCurrentStepTime,nCS);
	
	if(g_nCurrent_Step_Number > 0)	Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);

	Make_Log("END_FINISH", "ENDED", EVENT);
	return -1;
}



int End_Pumping()
{
	return 0;
}

int End_TimeStable()
{

	TTimer	TimeStableTimer;

	int		nCS					= 0;
	int		nAlarm_Action		= 0;
	int		nCheckSts			= 0;
	int		nCheckStableTime	= 0;
	int		nResultCnt			= 0;
	int		nInterlocked		= 0;
	int     nMFC_FunctionCount	= 0;
	int     nESCHe_FunctionCount= 0;
	
	double	dElapsedTime		= 0.0;
	double	dStepTime			= 0.0;
	double	dRcpStepTime		= 0.0;

	char	szTemp[256]			= {0};

	Make_Log("END_TIMESTABLE", "STARTED", EVENT);

	TimeStableTimer.Start();
	dElapsedTime = Step_CurTime.Read(nCS);
	
	dRcpStepTime = RECIPE[g_nCurrent_Step_Number].STEP_TIME;
	dStepTime	 = dRcpStepTime;

	do 
	{

		if(PM_PROC_CTRL.Read(nCS) == ePROC_ABORT)
		{
			Make_Log("", "PM_PROC_CTRL(ePROC_ABORT)", EVENT);
			
			eDO_EPD_Cmd.Write(eEPD_CMD_STOP,nCS);
			Abort();
			
			Make_Log("END_TIMESTABLE", "PM_PROC_CTRL == ePROC_ABORT", FAULT);
			Make_Log("END_TIMESTABLE", "ENDED", EVENT);
			return -1;
		}

		nFunctionAbortCheck = CheckAbortSts();
		if(nFunctionAbortCheck > 0)			
		{
			Abort();
			eDO_EPD_Cmd.Write(eEPD_CMD_STOP,nCS);
			Make_Log("END_TIMESTABLE", "CheckAbortSts > 0", FAULT);
			Make_Log("END_TIMESTABLE", "ENDED", EVENT);
			return -1;
		}

		if(_Is_Interlocked() > 0)	
		{
			nAlarm_Action = _Check_Intlk_Sts_Step("END_TIMESTABLE");

			Abort();
			
			eDO_EPD_Cmd.Write(eEPD_CMD_STOP,nCS);
			Make_Log("END_TIMESTABLE", "_Is_Interlocked", FAULT);
			Make_Log("END_TIMESTABLE", "ENDED", EVENT);
			return -1;
		}


		g_dblCurrentStepTime    = dElapsedTime + TimeStableTimer.ElapsedTime()/1000;
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;
	
		Step_CurTime.Write(g_dblCurrentStepTime,nCS);
		Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);

		if(WaitAbort(100))	return SEQ_ABORT;

	}while(RECIPE[g_nCurrent_Step_Number].STEP_TIME > g_dblCurrentStepTime);


	if(RECIPE[g_nCurrent_Step_Number].Is_RF_Step > 0)
	{
		g_dblTotalProcessTime += g_dblCurrentStepTime;
	}
	
	eAO_TotalEtchTime.Write(g_dblTotalProcessTime,nCS);
	
	
	//MFC RUNNING//	
	//////////////////////////////////////////////////////////////////////////
	//20170710  
	if( FNC_MFC.Status() == SEQ_RUNNING)
	{
		
		for(nMFC_FunctionCount=1; nMFC_FunctionCount<=100 ; nMFC_FunctionCount++)
		{
			_sleep(100* nMFC_FunctionCount );
			sprintf(szTemp, "MFC FUNCTION RUNNING.. %d Counting,", nMFC_FunctionCount);
			Make_Log("", szTemp, EVENT);
			
			if (FNC_MFC.Status() !=SEQ_RUNNING) 
			{
				Make_Log("","MFC FUNCTION RUNNING Check End.", EVENT);

				_sleep(200);
				break;
			}
			
		}
	}

	//////////////////////////////////////////////////////////////////////////
	if( FNC_HE.Status() == SEQ_RUNNING)
	{
		for(nESCHe_FunctionCount=1; nESCHe_FunctionCount<=50 ; nESCHe_FunctionCount++)
		{
			_sleep(100* nESCHe_FunctionCount );
			sprintf(szTemp, "ESC_HE FUNCTION RUNNING.. %d Counting,", nESCHe_FunctionCount);
			Make_Log("", szTemp, EVENT);
			
			if (FNC_HE.Status() !=SEQ_RUNNING) 
			{
				Make_Log("","HE FUNCTION RUNNING Check End.", EVENT);
				
				_sleep(200);
				break;
			}	
		}
	}

	nResultCnt = GetResultOfFunction(1);

	if( nResultCnt == 8)	
	{
		eDO_EPD_Cmd.Write(eEPD_CMD_STOP,nCS);
		Make_Log("END_TIMESTABLE", "ENDED", EVENT);	
		return  1;
	}
	else
	{
		sprintf(szTemp, "GetResultOfFunction() = %d is failed", nResultCnt);
		Make_Log("", szTemp, FAULT);
	}
	
	Make_Log("", "Time Step Timeout Error", FAULT);
	nAlarm_Action = _Timeout_Step("END_TIMESTABLE");

	Abort();
	g_dblCurrentStepTime    = dElapsedTime + TimeStableTimer.ElapsedTime()/1000;
	g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

	Step_CurTime.Write(g_dblCurrentStepTime,nCS);
	Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);

	Make_Log("END_TIMESTABLE", "ENDED", EVENT);

	return -1;
}


int Control_EPD()
{
	int		CS;
	int		EpdError		=	0;

	char	szGlassName[80]	=	{0};
	char szGlassName2[80]	= {0};
	char	szLotID[80]		=	{0};
	char	szPortID[80]	=	{0};
	char	szSlotID[80]	=	{0};
	char	szTemp[256]		=	{0};
	char	AlmMsg[256];				// 2020.05.21

	EPD_ALARM_HAPPEN =FALSE;

	eDO_EPD_Cmd.Write(eEPD_CMD_STOP,CS);		_sleep(100);
	eDO_EPD_Cmd.Write(eEPD_CMD_RST,CS);
	
	eAO_EPD_RecipeNum.Write(RECIPE[g_nCurrent_Step_Number].EPD_RCPNUM,CS);
	eAO_EPD_EPDStepNum.Write(RECIPE[g_nCurrent_Step_Number].EPD_STEPNUM,CS);
	eAO_EPD_EQPStepNum.Write(g_nCurrent_Step_Number,CS);

	GLASS_ID.Read(szGlassName,CS);
	LOT_ID.Read(szLotID,CS);
	PORT_ID.Read(szPortID,CS);
	SLOT_ID.Read(szSlotID,CS);
	REAR_GLASS_ID.Read(szGlassName2,CS);		// A3 OCTA
		
	eSO_EPD_LotID.Write(szLotID,CS);
	eSO_EPD_PortID.Write(szPortID,CS);
	eSO_EPD_SlotID.Write(szSlotID,CS);
	eSO_EPD_RcpName.Write(RECIPE_NAME , CS);
	eSO_EPD_GlassName.Write(szGlassName,CS);
	eSO_EPD_GlassName2.Write(szGlassName2,CS);		// A3 OCTA

	if(PM_GLASS_STS.Read(CS) == GLS_NONE) eSO_EPD_GlassName.Write("NoGlass",CS);
	_sleep(100);

	eDO_EPD_Cmd.Write(eEPD_CMD_RCPINFO,CS); _sleep(100);
	eDO_EPD_Cmd.Write(eEPD_CMD_RCPNO,CS);	_sleep(100);
	
	sprintf(szTemp, "StepName=%s, EQPStepNum=%d, EPDRcpNum=%d, EPDRcpStepNum=%d", 
		RECIPE[g_nCurrent_Step_Number].STEP_NAME, g_nCurrent_Step_Number, RECIPE[g_nCurrent_Step_Number].EPD_RCPNUM, RECIPE[g_nCurrent_Step_Number].EPD_STEPNUM);

	Make_Log("", szTemp, EVENT);

	_sleep(300);

	EpdError = eDI_EPD_Error.Read(CS);

	if(EpdError > 1) 
	{
		// 2020.05.21
		memset( AlmMsg , 0x00 , sizeof(AlmMsg) );
		sprintf( AlmMsg , "EPD ERROR STATUS IS ON. ERROR CHECK. ERROR CODE [%d]" , EpdError );

		//EPD_5504.PostAnyTh(AlmMsg); // KJC 
		EPD_ALARM_HAPPEN =TRUE;
		Make_Log("", "EPD_ALARM_HAPPEN Change True[1]", EVENT);

		Abort();

		return -1; // 2020.03.30, jihyun
	}

	return 1;
}


int End_EPD()
{
	TTimer EPDTimer;

	int		CS						= 0;
	int		nAlarm_Action			= 0;
	int		nResult					= 0;
	int		nCheckSts				= 0;
	int		nEPD_Result				= 0;
	int		nOverEtch_Use			= 0;
	int		nUnderEtch_Use			= 0;
	int		nRampSts				= -1;
	int		nHRF_result				= 0;
	int		nLRF_result				= 0;
	int		nHRF_RampStatus			= -1;
	int		nLRF_RampStatus			= -1;	
	int		nCheckStableTime		= 0;
	int		nInterlocked			= 0;

	double	dElapsedTime			= 0.0;
	double	dEndPointTime			= 0.0;
	double	dOverEtchTime			= 0.0;
	double	dJustEtchTime			= 0.0;
	double	dRcp_OverEtchTime		= 0.0;
	double	dRcp_OverEtchPercent	= 0.0;
	double	dGetTime1				= 0.0;
	double	dGetTime2				= 0.0;
	double	stepTime;
	double	warningLimit			= 0;
	double	alarmLimit				= 0;
	double	dSRC_FwdPwr				= 0.0;
	double	dBIAS_FwdPwr			= 0.0;		
	double	dStepTime				= 0.0;
	double	dRcpStepTime			= 0.0;

	char	szGetError[128]			= {0};
	char	szTmep[128]				= "";
	char	szTemp[256]				= {0};

	//-------------------------------------

	Make_Log("END_EPD", "STARTED", EVENT);

	EPDTimer.Start();
	dElapsedTime = Step_CurTime.Read(CS);
	dRcpStepTime = RECIPE[g_nCurrent_Step_Number].STEP_TIME;
	dStepTime	 = dRcpStepTime;

	do 
	{

		if(nRampSts < 0 && RECIPE[g_nCurrent_Step_Number].SRFG_POWER > 0 && RECIPE[g_nCurrent_Step_Number].BRFG_POWER > 0)
		{
			dSRC_FwdPwr  = eAI_SRFG_FwdPwr.Read(CS);
			dBIAS_FwdPwr = eAI_BRFG_FwdPwr.Read(CS);
			
			if(dSRC_FwdPwr > 0.98 * RECIPE[g_nCurrent_Step_Number].SRFG_POWER && dSRC_FwdPwr < 1.02 * RECIPE[g_nCurrent_Step_Number].SRFG_POWER)
			{
				nHRF_RampStatus = 1;
			}
			
			if(dBIAS_FwdPwr > 0.98 * RECIPE[g_nCurrent_Step_Number].BRFG_POWER && dBIAS_FwdPwr < 1.02 * RECIPE[g_nCurrent_Step_Number].BRFG_POWER)
			{
				nLRF_RampStatus = 1;
			}
			
			if(nHRF_RampStatus == 1 && nLRF_RampStatus == 1)
			{
				nRampSts = 0;
			}
			
		}
		else if(nRampSts == 0)
		{
			nHRF_result = FNC_SRF.Status();
			nLRF_result = FNC_BRF.Status();

			if(nHRF_result == SEQ_SUCCESS && nLRF_result == SEQ_SUCCESS)
			{
				nRampSts = 1;
			}
		}
		
		if(nRampSts == 1 && nCheckStableTime < 999)
		{
			nCheckStableTime++;	
		}
		
		if(nCheckStableTime == 5)	
		{
			
			if(RECIPE[g_nCurrent_Step_Number].SRFG_POWER > 3000 && RECIPE[g_nCurrent_Step_Number].BRFG_POWER > 3000)
			{				
				nCheckStableTime = 999;
				
				sprintf(szTmep, "Ramp up Completed");
				Make_Log("", szTmep, EVENT);
			}
		}
		if(PM_PROC_CTRL.Read(CS) == ePROC_ABORT)
		{
			Make_Log("", "PM_PROC_CTRL(ePROC_ABORT)[1]", EVENT);

			nEPD_Result = -1;
			break;
		}


		if(eDI_EPD_Error.Read(CS) != 0)
		{ 
			CEnum en;
			en = eDI_EPD_Error.GetEnum();

			sprintf(szTemp, "EPD Error was detected. : %s", en.GetEnumStr(eDI_EPD_Error.Read(CS) ) );
			Make_Log("", szTemp, FAULT);
			
			nEPD_Result = -2;
			
			break;
		}

		nCheckSts = CheckAbortSts();
	
		if(nCheckSts > 0)		
		{
			Make_Log("", "Sub Function Status is Failure", FAULT);
			
			nEPD_Result = -3;
			break;
		}

		if(_Is_Interlocked() > 0)
		{
			Make_Log("", "_Is_Interlocked() > 0", FAULT);
			nAlarm_Action = _Check_Intlk_Sts_Step("END_EPD");	
			
			nEPD_Result = -4;
			break;
		}


		nEPD_Result = eDI_EPD_EndPoint.Read(CS);
		if(nEPD_Result == eEPD_DETECTED)
		{
			Make_Log("", "EPD Endpoint is detected", EVENT);
			break;
		}
		else if(nEPD_Result == eEPD_STOPPED)
		{
			Make_Log("", "EPD is stopped", EVENT);
			break;
		}
		
		g_dblCurrentStepTime    = dElapsedTime + EPDTimer.ElapsedTime()/1000;
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

		Step_CurTime.Write(g_dblCurrentStepTime,CS);
		Rcp_CurTime.Write(g_dblCurrentProcessTime,CS);

		if(WaitAbort(100))	return SEQ_ABORT;

	}while(RECIPE[g_nCurrent_Step_Number].STEP_TIME > g_dblCurrentStepTime);


	if(RECIPE[g_nCurrent_Step_Number].Is_RF_Step >  0)
	{
		g_dblTotalProcessTime += g_dblCurrentStepTime;
	}
	
	eAO_TotalEtchTime.Write(g_dblTotalProcessTime,CS);	
	
	
	if(nEPD_Result == eEPD_DETECTED)
	{

		stepTime	 = RECIPE[g_nCurrent_Step_Number].STEP_TIME;
		alarmLimit   = RECIPE[g_nCurrent_Step_Number].EPD_MINTIME;

		if(g_dblCurrentStepTime < alarmLimit)
		{
			EPD_5504.PostAnyTh(_TEXT("EPD_End point signal is too fast detected"));			//5503
			eDO_EPD_Cmd.Write(eEPD_CMD_STOP,CS);
			
			if(EPD_ALARM_ABORT_NOW)
			{
				Abort();	_sleep(1000);

				dGetTime1 = eAI_EPD_TotalEtchTime.Read(CS);
				Step_CurTime.Write(dGetTime1,CS);
			
				dGetTime2 = eAI_EPD_JustEtchTime.Read(CS);
			
				sprintf(szTemp, "eAI_EPD_TotalEtchTime = %.2f, eAI_EPD_JustEtchTime = %.2f", dGetTime1, dGetTime2);
				Make_Log("", szTemp, EVENT);
			
				g_dblCurrentStepTime    = dGetTime1;	
				g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

				Step_CurTime.Write(g_dblCurrentStepTime,CS);
				Rcp_CurTime.Write(g_dblCurrentProcessTime,CS);
			
				Make_Log("END_EPD", "ENDED", EVENT);

				return -1;
			}
		}

	}

	
	if(nEPD_Result < 0 || nEPD_Result == 2) 
	{
		EPD_ALARM_HAPPEN =TRUE;
		Make_Log("", "EPD_ALARM_HAPPEN Change True[2]", EVENT);


		if		(nEPD_Result ==  2)		EPD_5504.PostAnyTh(_TEXT("EPD_End point signal is not detected"));
		else if (nEPD_Result == -2)		EPD_5504.PostAnyTh(_TEXT("EPD Result is Error. EPD Check"));

		eDO_EPD_Cmd.Write(eEPD_CMD_STOP,CS);

		if(EPD_ALARM_ABORT_NOW)
		{
			Abort();
			
			_sleep(1000);
			
			dGetTime1 = g_dblCurrentStepTime;
			dGetTime2 = eAI_EPD_JustEtchTime.Read(CS);
			
			sprintf(szTemp, "eAI_EPD_TotalEtchTime = %.2f, eAI_EPD_JustEtchTime = %.2f", dGetTime1, dGetTime2);
			Make_Log("", szTemp, EVENT);
			
			g_dblCurrentStepTime    = dGetTime1;	
			g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;
			
			Step_CurTime.Write(g_dblCurrentStepTime,CS);
			Rcp_CurTime.Write(g_dblCurrentProcessTime,CS);
			
			
			Make_Log("END_EPD", "ENDED", EVENT);
			return -1;

		}
	}


	if(GetResultOfFunction(1) != 8)
	{	
		eDO_EPD_Cmd.Write(eEPD_CMD_STOP,CS);

		Abort();

		_sleep(1000);
		
		dGetTime1 = g_dblCurrentStepTime;	
		dGetTime2 = eAI_EPD_JustEtchTime.Read(CS);
		
		sprintf(szTemp, "eAI_EPD_TotalEtchTime = %.2f, eAI_EPD_JustEtchTime = %.2f", dGetTime1, dGetTime2);
		Make_Log("", szTemp, EVENT);
		
		g_dblCurrentStepTime    = dGetTime1;
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;
		
		Step_CurTime.Write(g_dblCurrentStepTime,CS);
		Rcp_CurTime.Write(g_dblCurrentProcessTime,CS);


		Make_Log("END_EPD", "ENDED", EVENT);
		return -1;
	}


	if(nEPD_Result != eEPD_DETECTED)
	{	
		eDO_EPD_Cmd.Write(eEPD_CMD_STOP,CS);

		//20190603
		//Kim CS B Confirm
		Abort();

		Make_Log("", "EPD_ALARM_HAPPEN Change True[3]", EVENT);
		EPD_ALARM_HAPPEN =TRUE;

		dGetTime1 = g_dblCurrentStepTime;
		dGetTime2 = eAI_EPD_JustEtchTime.Read(CS);
		
		sprintf(szTemp, "eAI_EPD_TotalEtchTime = %.2f, eAI_EPD_JustEtchTime = %.2f", dGetTime1, dGetTime2);
		Make_Log("", szTemp, EVENT);

		g_dblCurrentStepTime    = dGetTime1;	
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;
		
		Step_CurTime.Write(g_dblCurrentStepTime,CS);
		Rcp_CurTime.Write(g_dblCurrentProcessTime,CS);


		if(PR_EPD_ALARM.Read(CS) == 1)
		{
			Make_Log("", "EPD Endpoint not detected. But, PR_EPD_ALARM(= 1)", FAULT);
			Make_Log("END_EPD", "ENDED", EVENT);
			return 1;
		}
		else
		{
			Make_Log("", "EPD Endpoint not detected", FAULT);
			Make_Log("END_EPD", "ENDED", EVENT);
			return -1;
		}
	}
	
	nOverEtch_Use = PR_OVERETCH_TYPE.Read(CS);

	if		(nOverEtch_Use == 1)	{	nResult = 2;	}
	else if	(nOverEtch_Use == 2)	{	nResult = 3;	}
	else							{	nResult = 99;	}	

	if		(nResult == 2)			
	{	
		dRcp_OverEtchTime	= PR_OVERETCH_VALUE.Read(CS);
		if(dRcp_OverEtchTime == 0.0)	nResult = 99;
	}	
	else if	(nResult == 3)			
	{
		dRcp_OverEtchPercent= PR_OVERETCH_VALUE.Read(CS);
		if(dRcp_OverEtchPercent == 0.0)	nResult = 99;
	}

	if(nResult == 99)
	{
		dEndPointTime =eAI_EPD_JustEtchTime.Read(CS);
		sprintf(szTemp, "EPD Just Etch O.K - CurrentStepTime = %.1f, EndPointTime = %.1f", g_dblCurrentStepTime, dEndPointTime);
		Make_Log("", szTemp, EVENT);
		Make_Log("END_EPD", "ENDED", EVENT);
		
		g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;
		
		if(RECIPE[g_nCurrent_Step_Number].Is_RF_Step > 0)
		{
			g_dblTotalProcessTime += g_dblCurrentStepTime;
		}
	
		eAO_JustEtchTime.Write(dEndPointTime,CS);
		eAO_TotalEtchTime.Write(g_dblTotalProcessTime,CS);
		Step_CurTime.Write(g_dblCurrentStepTime,CS);
		Rcp_CurTime.Write(g_dblCurrentProcessTime,CS);

		return 1;		
	}

	dEndPointTime = eAI_EPD_TotalEtchTime.Read(CS);
	dJustEtchTime = eAI_EPD_JustEtchTime.Read(CS);
	dOverEtchTime = dEndPointTime - dJustEtchTime;
				
	g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;
	

	Step_CurTime.Write(g_dblCurrentStepTime,CS);
	Rcp_CurTime.Write(g_dblCurrentProcessTime,CS);

	if(RECIPE[g_nCurrent_Step_Number].Is_RF_Step > 0)
	{
		g_dblTotalProcessTime += g_dblCurrentStepTime;
	}
	
	eAO_JustEtchTime.Write(dJustEtchTime,CS);
	eAO_TotalEtchTime.Write(g_dblTotalProcessTime,CS);


	if		(nResult == 2)	
	{
		if(dEndPointTime >= dJustEtchTime + dRcp_OverEtchTime)
		{
			sprintf(szTemp, "OverEtch O.K - CurrentStepTime = %.1f, Total=%.1f, JusEtch=%.1f, rOverEtchTime=%.1f", g_dblCurrentStepTime, dEndPointTime, dJustEtchTime, dOverEtchTime);
			Make_Log("", szTemp, EVENT);

			Make_Log("END_EPD", "ENDED", EVENT);
			return 1;	
		}
		else
		{
			sprintf(szTemp, "OverEtch NotOk - CurrentStepTime = %.1f, Total=%.1f, JusEtch=%.1f, rOverEtchTime=%.1f", g_dblCurrentStepTime, dEndPointTime, dJustEtchTime, dOverEtchTime);
			Make_Log("", szTemp, EVENT);
		}
	}
	else if	(nResult == 3)	
	{	
		if(dEndPointTime >= (dJustEtchTime + ( dJustEtchTime * dRcp_OverEtchPercent ) * 0.01))
		{
			sprintf(szTemp, "OverEtch O.K - CurrentStepTime=%.1f, Total=%.1f, JustEtch=%.1f, rOverEtchRatio=%.1f", g_dblCurrentStepTime, dEndPointTime, dJustEtchTime, dRcp_OverEtchPercent);
			Make_Log("", szTemp, EVENT);
			
			Make_Log("END_EPD", "ENDED", EVENT);
			return 1;
		}
		else
		{
			sprintf(szTemp, "OverEtch NotOk - CurrentStepTime = %.1f, Total=%.1f, JustEtch=%.1f, rOverEtchRatio=%.1f", g_dblCurrentStepTime, dEndPointTime, dJustEtchTime, dRcp_OverEtchPercent);
			Make_Log("", szTemp, EVENT);
		}
	}

	if(PR_EPD_ALARM.Read(CS) == 1)
	{
		Make_Log("", "OverEtch is not O.K. But, PR_EPD_ALARM(= 1)", EVENT);
		Make_Log("END_EPD", "ENDED", EVENT);
		return 1;
	}

	Make_Log("", "EPD Process Time is under the OverEtch Condition", FAULT);
	
	Abort();
	Make_Log("END_EPD", "ENDED", EVENT);

	return -1;
}

void Update_StepTime()
{
	int nCS = 0;

	g_dblCurrentProcessTime = g_dblStepInProcessTime + g_dblCurrentStepTime;

	Step_CurTime.Write(g_dblCurrentStepTime,nCS);
	
	if(g_nCurrent_Step_Number > 0) 
	{
		Rcp_CurTime.Write(g_dblCurrentProcessTime,nCS);
	}
}


SEQ_STATUS STEP_CONTROL(int nStepNum)
{
	TTimer	StepTimer;
	BOOL	isEpdMode			= FALSE;

	int		nControlState		= 0;
	int		CS					= 0;
	int		nRFCheck			= 0;

	char	szLog[128]			= {0};
	char	szTemp[128]			= {0};
	char	szTemp2[128]		= {0};

	int		nEPDControlState		= 0; // 2020.05.21

	Make_Log("STEP_CONTROL", "STARTED", EVENT);

	StepTimer.Start();

	//2019.09.04
	// EPD Mode 구별 이유 없음
/* 
	if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_EPD)
 	{
 		isEpdMode = TRUE;
 		sprintf(szTemp2, "%s", "[EPD]");
 	}	
*/	
	g_dblCurrentStepTime	= 0;
	g_dblStepInProcessTime	= Rcp_CurTime.Read(CS);

	if(nStepNum != 0)
	{
		sprintf(szTemp, "STEP %d Is Started%s....", nStepNum, szTemp2);
		sGUI_Proc_msg.Write(szTemp,CS);
		Make_Log("", szTemp, EVENT);
	}

	if(PM_PROC_CTRL.Read(CS) == ePROC_ABORT)
	{
		Abort();

		PROCESS_1180.PostAnyTh();

		Make_Log("", "PM_PROC_CTRL(ePROC_ABORT)", EVENT);
		Make_Log("STEP_CONTROL", "ENDED", EVENT);
	
		return SEQ_ABORT;
	}


 	if  (XI_TM_GateVlv_Sns.Read(CS) == eXI_VLV_CLOSE)
	{
		Make_Log("", "Check XI_TM_GateVlv_Sns(= eXI_VLV_CLOSE) - OK", EVENT);
	}
 	else 
 	{
 		GateNotClose.Clear();
 		GateNotClose.PostAnyTh();
 			
 		Make_Log("", "TM Gate Valve(= Open). Process aborted. Alarm 1005", FAULT);
 			
 			return SEQ_ABORT; 
 	}
 	

	// 2019.09.04 A4 양예슬 PRO 요청
	// EPD Step 시작 시 Step Time 갱신이 되지않음
	// 아래에서 갱신하므로 해당 구문 삭제
/*	
	if( TRUE != isEpdMode )
	{
		g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;		
		Update_StepTime();
	}
*/
	g_dblReStartStepTime	= 0.0;
	g_dblTotalEtchTime		= 0.0;	
	g_dblEtchingTime		= 0.0;

	Make_Log("", "Call SetRcpParamToFunParam", EVENT);
	SetRcpParamToFunParam(nStepNum);

	g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;		
	Update_StepTime();

	// 2019.09.04
	// 해당 구문에서 EPD 포함 전 Step 시작 시 Timer Reset
/*
	if( TRUE != isEpdMode )
	{
		g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;		
		Update_StepTime();
	}
*/
	nRFCheck = 0;

	if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_COND) 
	{
		dIntlk_Chk_Disable.Write(eINTERLOCK_CHECK_ON,CS);
	}
	else
	{
		dIntlk_Chk_Disable.Write(eINTERLOCK_CHECK_OFF,CS);
	}


	if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_FINISH)
	{
		Make_Log("FINISH", "STARTED", EVENT);


		sprintf(szLog, "STEP [%d] : RUN_FUNCTION  FNC_RF_CTRL|PROC_RF_OFF", nStepNum);
		Make_Log("", szLog, EVENT);

		FNC_RF_CTRL.Run(_TEXT("PROC_RF_OFF"));

		Itk_SRC_Mon.Write(Mon_Dis,CS);
		Itk_BIAS_Mon.Write(Mon_Dis,CS);

		g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	Update_StepTime();
		
		FNC_MFC.RunTh(_TEXT("FULLCLOSE"));			
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_MFC|FULLCLOSE", nStepNum);
		Make_Log("", szLog, EVENT);

		FNC_APC.RunTh(_TEXT("FULLOPEN"));		
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_APC|%s", nStepNum, RECIPE[nStepNum].APC_CTRL);
		Make_Log("", szLog, EVENT);
		
		FNC_PIN.RunTh(_TEXT("TRANS_UP"));		
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_PIN|TRANS_UP", nStepNum);
		Make_Log("", szLog, EVENT);
		
		FNC_HE.RunTh(_TEXT("NONE"));
		FNC_ESC.RunTh(_TEXT("NONE"));
		
		g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;
		Update_StepTime();
		
	}
	else if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_COND)
	{
		Make_Log("CONDITION", "STARTED", EVENT);

		sprintf(szLog, "STEP [%d] : RUN_FUNCTION  FNC_RF_CTRL|PROC_RF_OFF", nStepNum);
		Make_Log("", szLog, EVENT);
		
		FNC_RF_CTRL.Run(_TEXT("PROC_RF_OFF"));

		Itk_SRC_Mon.Write(Mon_Dis,CS);
		Itk_BIAS_Mon.Write(Mon_Dis,CS);
		
		FNC_ESC.RunTh(RECIPE[nStepNum].ESC_CTRL);
		
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_ESC|%s", nStepNum, RECIPE[nStepNum].ESC_CTRL);
		Make_Log("", szLog, EVENT);
		
		FNC_MFC.RunTh(_TEXT("PROCESS"));
		
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_MFC|PROCESS", nStepNum);
		Make_Log("", szLog, EVENT);

		FNC_APC.RunTh(RECIPE[nStepNum].APC_CTRL);
		
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_APC|%s", nStepNum, RECIPE[nStepNum].APC_CTRL);
		Make_Log("", szLog, EVENT);

		FNC_HE.RunTh(RECIPE[nStepNum].HE_CTRL);	

		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_HE |%s", nStepNum, RECIPE[nStepNum].HE_CTRL);
		Make_Log("", szLog, EVENT);

		if(RECIPE[nStepNum].SRFM_MATCH > 0 && RECIPE[nStepNum].SRFM_TUNE > 0)	
		{
			FNC_SRF.RunTh(_TEXT("PRESET"));

			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_SRF|PRESET", nStepNum);
			Make_Log("", szLog, EVENT);
		}
		if(RECIPE[nStepNum].BRFM_MATCH > 0 && RECIPE[nStepNum].BRFM_TUNE > 0)
		{
			FNC_BRF.RunTh(_TEXT("PRESET"));	

			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_BRF|PRESET", nStepNum);
			Make_Log("", szLog, EVENT);
		}

		if(RECIPE[nStepNum].VVC_MOTER_TABLE> 0)	
		{
			FNC_VVC_MOTOR.RunTh(_TEXT("PROCESS"));

			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_VVC_MOTOR|PROCESS", nStepNum);
			Make_Log("", szLog, EVENT);

		}

		g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;
		Update_StepTime();
	}
	else if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_TIME || RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_DISCH  || RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_EPD)
	{
		if		(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_TIME)	Make_Log("TIME",		"STARTED", EVENT);	
		else if	(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_EPD)	Make_Log("EPD",			"STARTED", EVENT);
		else													Make_Log("DISCHARGE",	"STARTED", EVENT);

		if(RECIPE[nStepNum].SRFG_POWER ==0 && RECIPE[nStepNum].BRFG_POWER ==0 )
		{
			sprintf(szLog, "ALL POWER IS ZERO. STEP [%d] : RUN_SET_FUNCTION  FNC_RF_CTRL|PROC_RF_OFF", nStepNum);

			FNC_RF_CTRL.RunTh(_TEXT("PROC_RF_OFF"));  //check
			Itk_SRC_Mon.Write(Mon_Dis,CS);
			Itk_BIAS_Mon.Write(Mon_Dis,CS);
		}

		if(RECIPE[nStepNum].SRFM_MATCH > 0 && RECIPE[nStepNum].SRFM_TUNE > 0)
		{
			FNC_SRF.RunTh(_TEXT("PRESET"));
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_SRF|PRESET", nStepNum);
			Make_Log("", szLog, EVENT);
		}
	
		if(RECIPE[nStepNum].BRFM_MATCH > 0 && RECIPE[nStepNum].BRFM_TUNE > 0)
		{
			FNC_BRF.RunTh(_TEXT("PRESET"));
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_BRF|PRESET", nStepNum);
			Make_Log("", szLog, EVENT);
		}
	
		if(RECIPE[nStepNum].SRFM_MATCH > 0 || RECIPE[nStepNum].SRFM_TUNE > 0)
		{
			FNC_SRF.Status();
			
			g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	
			Update_StepTime();
			
			sprintf(szLog, "STEP [%d] : FNC_SRF|PRESET is Completed", nStepNum);
			Make_Log("", szLog, EVENT);
		}
		if(RECIPE[nStepNum].BRFM_MATCH > 0 || RECIPE[nStepNum].BRFM_TUNE > 0)
		{
			FNC_BRF.Status();
			
			g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	
			Update_StepTime();
			
			sprintf(szLog, "STEP [%d] : FNC_BRF|PRESET is Completed", nStepNum);
			Make_Log("", szLog, EVENT);
		}
	
		if(RECIPE[nStepNum].VVC_MOTER_TABLE> 0)	
		{
			FNC_VVC_MOTOR.RunTh(_TEXT("PROCESS"));
			
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_VVC_MOTOR|PROCESS", nStepNum);
			Make_Log("", szLog, EVENT);		
		}

		g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	
		Update_StepTime();


		if(RECIPE[nStepNum].SRFG_POWER >0 )
		{
			Itk_SRC_Mon.Write(Mon_En,CS);
			Itk_BIAS_Mon.Write(Mon_En,CS);

			if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_TIME && RECIPE[nStepNum].EPD_RCPNUM !=0 && RECIPE[nStepNum].EPD_STEPNUM !=0 )
			{
				Make_Log("TIME", "CONTROL_EPD SET", EVENT);	
				
				// 2020.05.21
				// Control_EPD();
				nEPDControlState = Control_EPD(); // 2020.03.30, jihyun
				if( nEPDControlState < 0 ) { return SEQ_ABORT; }
			}

			
					
			sprintf(szLog, "SRF_Setpoint = %.0f, BRF_Setpoint = %.0f", (double)(RECIPE[nStepNum].SRFG_POWER), (double)(RECIPE[nStepNum].BRFG_POWER));
			Make_Log("", szLog, EVENT);

 			FNC_SRF.RunTh(RECIPE[nStepNum].SRF_CTRL);
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_SRF|%s", nStepNum, RECIPE[nStepNum].SRF_CTRL);
 			Make_Log("", szLog, EVENT);
 
 			FNC_BRF.RunTh(RECIPE[nStepNum].BRF_CTRL);
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_BRF|%s", nStepNum, RECIPE[nStepNum].BRF_CTRL);
 			Make_Log("", szLog, EVENT);
		
			FNC_ESC.RunTh(RECIPE[nStepNum].ESC_CTRL);
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_ESC|%s", nStepNum, RECIPE[nStepNum].ESC_CTRL);
			Make_Log("", szLog, EVENT);
		
			FNC_MFC.RunTh(_TEXT("PROCESS"));
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_MFC|PROCESS", nStepNum);
			Make_Log("", szLog, EVENT);

			FNC_APC.RunTh(RECIPE[nStepNum].APC_CTRL);
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_APC|%s", nStepNum, RECIPE[nStepNum].APC_CTRL);
			Make_Log("", szLog, EVENT);
	
			FNC_HE.RunTh(RECIPE[nStepNum].HE_CTRL);		
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_HE |%s", nStepNum, RECIPE[nStepNum].HE_CTRL);
			Make_Log("", szLog, EVENT);

			sprintf(szLog, "STEP [%d] : Wait For Running FNC_SRF, FNC_BRF", nStepNum);
			Make_Log("", szLog, EVENT);
			
			g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	
			Update_StepTime();
			
			nRFCheck = PM_PROC_RFCTRL.Read(CS);				
			sprintf(szLog, "STEP [%d] : PM_PROC_RFCTRL = %d", nStepNum - 1, nRFCheck);
			Make_Log("", szLog, EVENT);


			if((strcmp(RECIPE[nStepNum].SRF_CTRL, "POWERON") == 0) && RECIPE[nStepNum].SRFG_POWER > 1.0)	{ nRFCheck = nRFCheck | 0x0001;	}	
			else																							{ nRFCheck = nRFCheck & 0xfffe; Itk_SRC_Mon.Write(Mon_Dis,CS);}
					
			if((strcmp(RECIPE[nStepNum].BRF_CTRL, "POWERON") == 0) && RECIPE[nStepNum].BRFG_POWER > 1.0)	{ nRFCheck = nRFCheck | 0x0002;	}
			else																							{ nRFCheck = nRFCheck & 0xfffd;	Itk_BIAS_Mon.Write(Mon_Dis,CS);}
				
		
			if( (nRFCheck == 0x0001) && ( RECIPE[nStepNum].SRFG_POWER <= Dechuck_SRF_Power.Read(CS) ))
			{
				Itk_SRC_Mon.Write(Mon_Dis,CS);
				Itk_BIAS_Mon.Write(Mon_Dis,CS);

				if		( RECIPE[nStepNum].SRFG_POWER == Dechuck_SRF_Power.Read(CS))		Dechuck_InterlockOn.Write(Mon_En,CS);
				else if ( RECIPE[nStepNum].SRFG_POWER == 1500)								Dechuck_InterlockOn.Write(Mon_Dis,CS);
			}
			else
			{
				Dechuck_InterlockOn.Write(Mon_Dis,CS);
			}

			PM_PROC_RFCTRL.Write(nRFCheck,CS);
			
			CEnum en;
			en = PM_PROC_RFCTRL.GetEnum();
			
			sprintf(szLog, "STEP [%d] : Time or Discharge, PM_PROC_RFCTRL = %s", nStepNum, en.GetEnumStr(PM_PROC_RFCTRL.Read(CS)));
			Make_Log("", szLog, EVENT);

			if(nRFCheck == 0x01)
			{
				FNC_RF_CTRL.RunTh(_TEXT("PROC_RF_HIGH"));
				Make_Log("", "RF : PROC_RF_HIGH", EVENT);

				if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_TIME && RECIPE[nStepNum].EPD_RCPNUM !=0 && RECIPE[nStepNum].EPD_STEPNUM !=0 )
				{
					Make_Log("", "RF : PROC_RF_HIGH. EPD START RUN", EVENT);
					eDO_EPD_Cmd.Write(eEPD_CMD_START,CS);
				}
				
			}
			else if(nRFCheck == 0x02)
			{
				FNC_RF_CTRL.RunTh(_TEXT("PROC_RF_OFF"));

				Make_Log("", "RF : INVALID_MODE", EVENT);
			}
			else if(nRFCheck == 0x03)
			{
				FNC_RF_CTRL.RunTh(_TEXT("PROC_RF_DUAL"));
				Make_Log("", "RF : PROC_RF_DUAL", EVENT);

				if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_TIME && RECIPE[nStepNum].EPD_RCPNUM !=0 && RECIPE[nStepNum].EPD_STEPNUM !=0 )
				{
					Make_Log("", "RF : PROC_RF_DUAL. EPD START RUN", EVENT);
					eDO_EPD_Cmd.Write(eEPD_CMD_START,CS);
				}

			}
			else
			{
				FNC_RF_CTRL.RunTh(_TEXT("PROC_RF_OFF"));
				Make_Log("", "RF : PROC_RF_OFF", EVENT);
			}

			g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	
			Update_StepTime();
		}
		else
		{
			FNC_ESC.RunTh(RECIPE[nStepNum].ESC_CTRL);
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_ESC|%s", nStepNum, RECIPE[nStepNum].ESC_CTRL);
			Make_Log("", szLog, EVENT);
			
			FNC_MFC.RunTh(_TEXT("PROCESS"));
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_MFC|PROCESS", nStepNum);
			Make_Log("", szLog, EVENT);

			FNC_APC.RunTh(RECIPE[nStepNum].APC_CTRL);
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_APC|%s", nStepNum, RECIPE[nStepNum].APC_CTRL);
			Make_Log("", szLog, EVENT);

			FNC_HE.RunTh(RECIPE[nStepNum].HE_CTRL);
			sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_HE |%s", nStepNum, RECIPE[nStepNum].HE_CTRL);
			Make_Log("", szLog, EVENT);

		}
	
		g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	
		Update_StepTime();

	}
	else
	{
		Make_Log("UNKNOWN STEP", "STARTED", EVENT);

		nRFCheck = 0;

		PM_PROC_RFCTRL.Write(nRFCheck,CS);
		sprintf(szLog, "STEP [%d] : Condition, PM_PROC_RFCTRL = %d", nStepNum, nRFCheck);
		FNC_RF_CTRL.Run(_TEXT("PROC_RF_OFF"));
		Make_Log("", szLog, EVENT);

		sprintf(szLog, "STEP [%d] : %s", nStepNum, PM_PROC_RFCTRL.GetEnum() );
		Make_Log("", szLog, EVENT);

		FNC_MFC.RunTh(_TEXT("FULLCLOSE"));
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_MFC|FULLCLOSE", nStepNum);
		Make_Log("", szLog, EVENT);
		
		FNC_APC.RunTh(_TEXT("FULLOPEN"));
		sprintf(szLog, "STEP [%d] : RUN_SET_FUNCTION  FNC_APC|FULLOPEN", nStepNum);
		Make_Log("", szLog, EVENT);

		FNC_HE.RunTh(_TEXT("NONE"));
		FNC_ESC.RunTh(_TEXT("NONE"));
	}

	g_dblCurrentStepTime = StepTimer.ElapsedTime()/1000;	Update_StepTime();

	if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_TIME)
	{
		Make_Log("", "CALL : END_TIMESTABLE", EVENT);

		nControlState = End_TimeStable();

		if(RECIPE[nStepNum].SRFG_POWER > 0)
		{
			g_dblTotalEtchTime = g_dblCurrentStepTime;

			if(nControlState > 0)
			{
				FNC_PROC_AbortStep.Write(0.0,CS);
				FNC_PROC_AddTime.Write(0.0,CS);
				FNC_PROC_RemainTime.Write(0.0,CS);
			}
			else
			{
				g_dblAtortedStepNumber = nStepNum;
				g_dblAtortedStepTime   = RECIPE[nStepNum].STEP_TIME;
				FNC_PROC_AddTime.Write(0.0,CS);
				FNC_PROC_AbortStep.Write(g_dblAtortedStepNumber,CS);
				FNC_PROC_RemainTime.Write((g_dblAtortedStepTime-g_dblCurrentStepTime),CS);
			}
		}
	}
	else if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_EPD)
	{
		Make_Log("", "CALL : END_EPD", EVENT);
		
		nControlState = End_EPD();
				
		if(RECIPE[nStepNum].SRFG_POWER > 0)
		{
			g_dblTotalEtchTime = g_dblCurrentStepTime;
			
			if(nControlState > 0)
			{
				FNC_PROC_AbortStep.Write(0.0,CS);
				FNC_PROC_AddTime.Write(0.0,CS);
				FNC_PROC_RemainTime.Write(0.0,CS);
			}
			else
			{
				g_dblAtortedStepNumber = nStepNum;
				g_dblAtortedStepTime   = RECIPE[nStepNum].STEP_TIME;
				FNC_PROC_AddTime.Write(0.0,CS);
				FNC_PROC_AbortStep.Write(g_dblAtortedStepNumber,CS);
				FNC_PROC_RemainTime.Write((g_dblAtortedStepTime-g_dblCurrentStepTime),CS);
			}
		}
	}
	else if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_COND) 
	{
		dIntlk_Chk_Disable.Write(eINTERLOCK_CHECK_ON,CS);

		Make_Log("", "CALL : END_CONDSTABLE", EVENT);
		nControlState = End_CondStable();

		dIntlk_Chk_Disable.Write(eINTERLOCK_CHECK_OFF,CS);
	}
	else if(RECIPE[nStepNum].STEP_ENDTYPE == eTYPE_DISCH)
	{
		Make_Log("", "CALL : END_DISCHARGE", EVENT);
		nControlState = End_Discharge();

		if(RECIPE[nStepNum].SRFG_POWER > 0.0)
		{
			g_dblTotalEtchTime = g_dblCurrentStepTime;
		}
	}
	else if(RECIPE[g_nCurrent_Step_Number].STEP_ENDTYPE == eTYPE_FINISH)
	{
		Make_Log("", "CALL : END_FINISH", EVENT);
		nControlState = End_Finish();
	}
	else
	{
		Make_Log("", "END TYPE IS UNKNOWN", FAULT);
		nControlState = -1;
	}

	Itk_SRC_Mon.Write(Mon_Dis,CS);
	Itk_BIAS_Mon.Write(Mon_Dis,CS);

	if(nControlState < 0 || nControlState == SEQ_ABORT)
	{
		sprintf(szTemp, "CURRENT STEP TIME = %.0f", g_dblCurrentStepTime);
		Make_Log("", szTemp, FAULT);
		Make_Log("STEP_CONTROL", "ENDED", EVENT);

		return SEQ_ABORT;
	}
	else
	{
		sprintf(szTemp, "CURRENT STEP TIME = %.0f", g_dblCurrentStepTime);
		Make_Log("", szTemp, EVENT);
		Make_Log("STEP_CONTROL", "ENDED", EVENT);

		return SEQ_SUCCESS;
	}
}


int _Is_End(int Current_Step_No)
{
	int nRet = 0;
	int CS  = 0;

	if(PM_PROC_CTRL.Read(CS) == ePROC_ABORT)
	{
		nRet = __PROC_ABORT;
	}
	else if(Current_Step_No > g_nTotalStep)
	{
		nRet = __PROC_DONE;
	}
	else
	{
		nRet = __PROC_GOING;
	}
	return nRet;
}
int _Set_Current_Step_Info(int nStepNo)
{
	int CS;
	
	Step_Name.Write(RECIPE[nStepNo].STEP_NAME,CS);
	Recipe_Step_Desc.Write(RECIPE[nStepNo].STEP_NAME,CS);
	Step_Time.Write(RECIPE[nStepNo].STEP_TIME,CS);
	Rcp_CurStepNum.Write(nStepNo,CS);
	Recipe_Step_Num.Write(nStepNo,CS);
	Step_EndType.Write(RECIPE[nStepNo].STEP_ENDTYPE,CS);
	View_CurStepNum.Write(nStepNo,CS);

	return 1;
}
int _Set_Current_Step(int nStepNo, int Mode)
{
	int nResult = 0;

	if(nStepNo > g_nTotalStep)
	{
		nResult = -2;
	}
	else
	{
		nResult =  1;
		if(Mode == __GO_STEP)	g_nCurrent_Step_Number = nStepNo;
	}

	return nResult;
}

int _Init_Step(int nStepNo)
{
	sprintf(RECIPE[nStepNo].STEP_NAME, "");
	RECIPE[nStepNo].STEP_ENDTYPE = 0;
	RECIPE[nStepNo].STEP_TIME = 0.0;
	RECIPE[nStepNo].Is_RF_Step = -1;
	
	RECIPE[nStepNo].GAS1 = 0;
	RECIPE[nStepNo].GAS2 = 0;
	RECIPE[nStepNo].GAS3 = 0;
	RECIPE[nStepNo].GAS4 = 0;
	RECIPE[nStepNo].GAS5 = 0;
	RECIPE[nStepNo].GAS6 = 0;
	RECIPE[nStepNo].GAS7 = 0;
	RECIPE[nStepNo].GAS8 = 0;
	RECIPE[nStepNo].GAS9 = 0;
	RECIPE[nStepNo].GAS10 = 0;

	RECIPE[nStepNo].FRC_CENTER= 0;
	RECIPE[nStepNo].FRC_MIDDLE= 0;

	sprintf(RECIPE[nStepNo].SRF_CTRL, "");
	sprintf(RECIPE[nStepNo].BRF_CTRL, "");
	sprintf(RECIPE[nStepNo].HE_CTRL,  "");
	sprintf(RECIPE[nStepNo].APC_CTRL, "");
	sprintf(RECIPE[nStepNo].ESC_CTRL, "");

	RECIPE[nStepNo].SRFG_POWER		= 0;
	RECIPE[nStepNo].SRFM_MATCH		= 0;
	RECIPE[nStepNo].SRFM_TUNE		= 0;

	RECIPE[nStepNo].BRFG_POWER		= 0;
	RECIPE[nStepNo].BRFM_MATCH		= 0;
	RECIPE[nStepNo].BRFM_TUNE		= 0;

	RECIPE[nStepNo].PRESSURE		= 0;
	RECIPE[nStepNo].EPD_RCPNUM		= 0;
	RECIPE[nStepNo].EPD_STEPNUM		= 0;
	RECIPE[nStepNo].EPD_ALARM		= 0;
	RECIPE[nStepNo].EPD_OVRETCHTIME = 0;
	RECIPE[nStepNo].EPD_OVRETCHTYPE = 0;

	RECIPE[nStepNo].ESC_VOLT		= 0;
	RECIPE[nStepNo].HE_PRESSURE		= 0;

	RECIPE[nStepNo].VVC_MOTER_TABLE	= 0;


	return 1;

}

SEQ_STATUS PROCESS_CONTROL(int nStepSt)
{

	int		nProcessEnd					=	-1;	
	int		nCurrent_Step				=	0;	
	int		nRecovery_Action			=	0;
	int		nProcInfo					=	0;	
	int		nResult						=	0;	
	int		CS							=	0;
	int		nControlResult				=	0;
	int		nIdx						=	0;
	double  nTotalTime_for_Stepjump		=	0;

	char	szLogMsg[256]				=	{0};
	char	szTemp[256]					=	"";

	
	g_dblEtchingTime					=	0.0;
	g_dblTotalEtchTime					=	0.0;
	nProcessEnd							=	-1;
	g_nPROC_SKIP_FLAG					=	-1;	
	g_nCurrent_Step_Number				=	nStepSt;	
	nCurrent_Step						=	nStepSt;
	nControlResult						=	0;
	nRecovery_Action					=	0;
	nProcInfo							=	0;
	nResult								=	0;
	g_nBackup_Step_Number				=	0;
	g_dblCurrentProcessTime				=	0.0;
	g_dblCurrentStepTime				=	0.0;
	g_dblStepInProcessTime				=	0.0;

	LockItem.Write(1,CS);
	
	sprintf(szLogMsg, "RECIPE NAME = %s, TOTAL STEP = %d, TOTAL TIME = %.0f", RECIPE_NAME, STEP_COUNT, TOTAL_TIME);

	Make_Log("===============", "=================================================================", EVENT);
	Make_Log("", szLogMsg, EVENT);
	Make_Log("===============", "=================================================================", EVENT);

	Make_Log("PROCESS_CONTROL", "STARTED", EVENT);
	_Init_Step(0);	
	
	FNC_PROC_AbortStep.Write(0.0,CS);
	FNC_PROC_AddTime.Write(0.0,CS);
	FNC_PROC_RemainTime.Write(0.0,CS);
	
	Rcp_Name.Write(RECIPE_NAME,CS);
	Rcp_TotalStep.Write(STEP_COUNT,CS);
	Rcp_TotalTime.Write(TOTAL_TIME,CS);
	Rcp_CurTime.Write(0,CS);

	Step_Name.Write(RECIPE[nStepSt+1].STEP_NAME,CS);
	Recipe_Step_Desc.Write(RECIPE[nStepSt+1].STEP_NAME,CS);
	Step_Time.Write(RECIPE[nStepSt+1].STEP_TIME,CS);
	Rcp_CurStepNum.Write(nStepSt+1,CS);
	Recipe_Step_Num.Write(nStepSt+1,CS);
	View_CurStepNum.Write(nStepSt+1,CS);
	View_CurStepNum.Write(1,CS);

	if(nStepSt > 0)
	{
		for(nIdx = nStepSt + 1; nIdx < STEP_COUNT + 1; nIdx++)
			nTotalTime_for_Stepjump = nTotalTime_for_Stepjump + RECIPE[nIdx].STEP_TIME;

		Rcp_TotalTime.Write(nTotalTime_for_Stepjump,CS);
	}

	do
	{
		if( WaitAbort(10) ) 
		{
			sprintf(szTemp, "PROCESS_CONTROL Error (WaitAbort(10)), Curr_Step = %d", nCurrent_Step);
			Make_Log("PROCESS_CONTROL", szTemp, FAULT);
			return SEQ_ABORT;
		}
		nCurrent_Step++;

		nProcInfo = _Is_End(nCurrent_Step);

		if		( nProcInfo == __PROC_ABORT  )
		{ 
			Make_Log("", "PROC_ABORT", EVENT);
			nControlResult = -1; 
			break; 
		}
		else if	( nProcInfo == __PROC_DONE   )
		{ 
			Make_Log("", "PROC_DONE", EVENT);
			nControlResult =  1;
			break; 
		}
		else if	( nProcInfo == __PROC_GOING  )
		{
			_Set_Current_Step_Info(nCurrent_Step);
			_Set_Current_Step(nCurrent_Step, __GO_STEP);
			Make_Log("", "PROC_GOING", EVENT);
		}

		sprintf(szLogMsg, "CURR_STEP = %d, STARTED", g_nCurrent_Step_Number);
		Make_Log(">>>>>>>>>>>>>>",szLogMsg, EVENT);

		if(RECIPE[nCurrent_Step].Is_RF_Step == 1)		PR_ETCHSTEP.Write(1,CS);
		else											PR_ETCHSTEP.Write(0,CS);
	
		nControlResult	= STEP_CONTROL(nCurrent_Step);

		PR_ETCHSTEP.Write(0,CS);
		
		sprintf(szLogMsg, "CURR_STEP = %d, RESULT = %d", g_nCurrent_Step_Number, nControlResult);
		Make_Log(">>>>>>>>>>>>>>",szLogMsg, EVENT);
				
		if(nControlResult == SEQ_ABORT)
		{
			nControlResult = -1; 

			sprintf(szLogMsg, "Change RESULT = %d", nControlResult);
			Make_Log(">>>>>>>>>>>>>>",szLogMsg, EVENT);
				
			Abort(); 
			Make_Log("", "Abort() Call", EVENT);

			if(_Is_Intlk_Sts() > 0 )
			{
				Make_Log("", "FNC_PROCESS : ABORT(1). Case: _Is_Intlk_Sts() > 0 ", EVENT);
				break;
			}
			else if(nFunctionAbortCheck >0 )
			{
				Make_Log("", "FNC_PROCESS : ABORT(2). Case: nFunctionAbortCheck  > 0 ", EVENT);
				break;
			}
			else if(EPD_ALARM_HAPPEN)
			{
				Make_Log("", "FNC_PROCESS : ABORT(3). Case: EPD Alarm ", EVENT);
				break;
			}
			else if(dHW_Interlock_On_Status.Equal(ON))
			{
				Make_Log("", "FNC_PROCESS : ABORT(4). Case: dHW_Interlock_On_Status is On ", EVENT);
				break;
			}
			else
			{
				if ( dSystem_Intlk_Monitoring.Equal(OFF))
				{
					Make_Log("", "FNC_PROCESS : ABORT(6). Case: System Manual Abort", EVENT);
					ManualProcessAborted.PostAnyTh();
				}

				break;
			}

			PM_PROC_CTRL.Write(ePROC_ABORT,CS);
			return SEQ_ABORT;
			
		}
		else if(nControlResult == SEQ_SUCCESS)
		{
			
			mTargetValue = nCurrent_Step;		
			continue;
		}

		if(WaitAbort(10))	return SEQ_ABORT;
	}while(nProcessEnd < 0); 


	LockItem.Write(0,CS);

	if(nFunctionAbortCheck>0)
	{	
		nFunctionAbortCheck=0;
		Make_Log("", "nFunctionAbortCheck Change Zero", EVENT);
	}
	
	Make_Log("PROCESS_CONTROL", "ENDED", EVENT);	

	if(nControlResult > 0)	return SEQ_SUCCESS;
	else					return SEQ_ABORT;
}


int GetStatusAllFunction()
{
	int nFnResult = 0;

	nFnResult = FNC_SRF.Status();

	if	( nFnResult == SEQ_RUNNING)	 return -1;	 
	else				            nFnResult = FNC_SRF.Status();							

	if	( nFnResult == SEQ_RUNNING) return -1;	
	else							nFnResult = FNC_APC.Status();
	
	if	( nFnResult == SEQ_RUNNING) return -1;	
	else							nFnResult = FNC_PIN.Status();
	
	if	( nFnResult == SEQ_RUNNING) return -1;	
	else							nFnResult = FNC_HE.Status();

	if	( nFnResult == SEQ_RUNNING) return -1;	
	else							nFnResult = FNC_MFC.Status();

	if	( nFnResult == SEQ_RUNNING) return -1;	
	else							nFnResult = FNC_ESC.Status();

	if	( nFnResult == SEQ_RUNNING) return -1;	
	else							return  1;

		
}

//-----------------------------------------------------------------------------------------


SEQ_STATUS AGING_PROCESS( int nSetCounts , const void* pParam, int nNumOfBytesRecved )
{
	SEQ_STATUS nResult			= SEQ_ABORT;

	int		nCS					= 0;
	int		nAgingCounts		= 1;
	int		nCheckTime			= 600;
	double	dAgingStableTime	= 0.0;
	
	char	szTemp[256]			= {0};

	Make_Log("AGING_PROCESS", "STARTED", EVENT);

	dAgingStableTime = PM_AGING_STABLETIME.Read(nCS);

	if(dAgingStableTime < 1.0)	
	{
		dAgingStableTime = 10.0;
	}

	sGUI_Proc_msg.Write( _TEXT("Prepare Condition.") ,nCS );
	
	if(WaitAbort(200))	return SEQ_ABORT;
	
	while(1)
	{
		if(nSetCounts < 1)	
		{
			Make_Log("", "Aging_Process counts is less than 1", FAULT);
			Make_Log("AGING_PROCESS", "ENDED", EVENT);

			PM_AGING_COUNTS.Write(0,nCS);
			
			PM_PROC_STS.Write(eFNC_ABORTED,nCS);
			PM_PROC_CTRL.Write(0,nCS);

			PM_PROCRUN_MODE.Write(ePROCESS_NORMAL,nCS);
										
			if(WaitAbort(200))	return SEQ_ABORT;


			return SEQ_ABORT;
		}


		if(nAgingCounts > nSetCounts)	break;

	
		sprintf(szTemp, "PM_AGING_COUNTS = %d, CURR_AGING_COUNTS = %d", nSetCounts, nAgingCounts);
		Make_Log("", szTemp, EVENT);

		sGUI_Proc_msg.Write(_TEXT("Move Pin PROS_POS1..."),nCS);

		nResult = FNC_PIN.Run(_TEXT("PROS_POS1"));
		if(nResult == SEQ_ABORT)
		{
			PROCESS_1140.PostAnyTh();
			Make_Log("", "PIN MOVING FAIL. FNC_PIN|PROS_POS1", FAULT);
			nResult = SEQ_ABORT;

			sGUI_Proc_msg.Write(_TEXT("Fail To Move Pin PROS_POS1!!!  Abort Process."),nCS);
			goto AGING_PROCESS_END;
		}

		sGUI_Proc_msg.Write("Reading RECIPE File...",nCS);
		Make_Log("", "Reading RECIPE File...", EVENT);

		nResult = RECIPE_READING(pParam, nNumOfBytesRecved);
	
		if(nResult != SEQ_SUCCESS)	
		{
			RECIPEFAIL_1201.Clear();

			if(g_bDechuck_powerState==FALSE)
			{
				RECIPEFAIL_1202.Clear();
				RECIPEFAIL_1202.PostAnyTh();
			}

			RECIPEFAIL_1201.PostAnyTh();

			Make_Log("", "RECIPE_READING() = SEQ_ABORT", FAULT);
			PM_PROC_STS.Write(eFNC_ABORTED,nCS);
			nResult = SEQ_ABORT;

			sGUI_Proc_msg.Write("Fail to Reading RECIPE File!!!  Abort Process",nCS);
			goto AGING_PROCESS_END;
		}
		else
		{
			if( STEP_COUNT <= 0 ) 
			{
				PROCESS_1151.Clear();
				PROCESS_1151.PostAnyTh();

				Make_Log("", "STEP COUNT <= 0", FAULT);
				PM_PROC_STS.Write(eFNC_ABORTED,nCS);
				nResult = SEQ_ABORT;

				sGUI_Proc_msg.Write( "Total Step Count <= 0.  Abort Process...",nCS);
				goto AGING_PROCESS_END;
			}
		}


		nCheckTime = 0;
		while(1)
		{
			nCheckTime++;

			if(GetStatusAllFunction() > 0)
			{
				break;
			}
			if(FuncAbortChk() == TRUE)
			{
				PROCESS_1160.Clear();
				PROCESS_1160.PostAnyTh();
				Make_Log("", "SUB FUNCTION IS NOW RUNNING. Cancel to wait for function idle status", FAULT);
				nResult = SEQ_ABORT;
				
				sGUI_Proc_msg.Write(_TEXT("Sub Function is now running!!!  Abort Process..."),nCS);
				goto AGING_PROCESS_END;
			}
			
			if(nCheckTime > 600)
			{
				PROCESS_1160.Clear();
				PROCESS_1160.PostAnyTh();
				Make_Log("", "SUB FUNCTION IS NOW RUNNING. Cancel to wait for function idle status[60sec]", FAULT);
				nResult = SEQ_ABORT;
				
				sGUI_Proc_msg.Write(_TEXT("Sub Function is now running!!!  Abort Process..."),nCS);
				goto AGING_PROCESS_END;
			}
			
			if(WaitAbort(100))	return SEQ_ABORT;
		}

		Make_Log("", "CALL PROCESS_CONTROL()", EVENT);
		sGUI_Proc_msg.Write(_TEXT("Run AGING_PROCESS CONTROL..."),nCS);
	
		nResult = PROCESS_CONTROL(0);

		if(nResult == SEQ_SUCCESS)
		{
			Make_Log("", "PM_GLASS_STS = PROCESSED", EVENT);

			sprintf(szTemp, "Completed(%d/%d Count)!!  Wait %3.0fSec.. ",nAgingCounts, nSetCounts, dAgingStableTime);
			sGUI_Proc_msg.Write(szTemp,nCS);

		}
		else						
		{
			Make_Log("", "PM_GLASS_STS = ABORTED", EVENT);
			sGUI_Proc_msg.Write(_TEXT("AGING_PROCESS CONTROL is Aborted."),nCS);
		}
	

	AGING_PROCESS_END:
	
		if(nResult == SEQ_SUCCESS)		
		{
			nAgingCounts++;
			WaitAbort(dAgingStableTime);

			continue;

		}
		else							
		{
			nAgingCounts = 9999;
			break;
		}
		
		Make_Log("::::::::::::::::::::", "::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::::", EVENT);

		if(WaitAbort(10))	return SEQ_ABORT;
	}
	

	if(nAgingCounts == 9999)
	{
		AgingOverProcessAborted.Clear();
		AgingOverProcessAborted.PostAnyTh();

		nResult = SEQ_ABORT;

		PM_PROC_STS.Write(eFNC_ABORTED,nCS);
		Process_Condition.Write(eCon_Fail,nCS);
		sGUI_Proc_msg.Write(_TEXT("Aging process is aborted..!!!"),nCS);

		Make_Log("", "Aging process is aborted", EVENT);
	}
	else
	{
		nResult = SEQ_SUCCESS;

		PM_PROC_STS.Write(eFNC_IDLE,nCS);
		Process_Condition.Write(eCon_End,nCS);
		sGUI_Proc_msg.Write(_TEXT("Aging process is completed..!!!"),nCS);

		Make_Log("", "Aging process is completed", EVENT);
	}
	
	PM_AGING_COUNTS.Write(0,nCS);
	PM_PROC_CTRL.Write(0,nCS);
	PM_PROCRUN_MODE.Write(ePROCESS_NORMAL,nCS);

	DO_PROCESS_GAS_SUPPLY_VALVE.Write(eVLV_CLOSE,nCS);
	WaitAbort(500);
	
	DO_CDG_SHUT_OFF_VALVE.Write(eVLV_OPEN,nCS);

	if(WaitAbort(200))	return SEQ_ABORT;


	Make_Log("AGING_PROCESS", "ENDED", EVENT);
	return nResult;
}




//----------------------------------------------------------------------------------------------------------------------------------------------
void Process_End_Control() 
{
	int CS , i;
	Datalog_Control.Write(OFF,CS);
	Process_Run.Write(OFF,CS);
	PROCESSLOG.RunAny( "LOG_END" );

	utilIOLogPC.SendEventMsg(_TEXT("STOP"));			//CIM PC Remote Seq
	Recipe_Max_Step.Write( 0 , CS );
	Recipe_Step_Num.Write( 0 , CS );
	Recipe_Step_Desc.Write( "" , CS );
	
	for ( i = 0 ; i < MAX_STEPNO_COUNT ; i++ ) 
	{
		SioAt(&STEP01_DESC , i )->Write( "" , CS );
		DioAt(&STEP01_STEPNO , i )->Write( 0 , CS );
	}

}

void Process_LOG_Rename() //181121 KYS Process Log Abort Name Add 
{
	int result;
	char szTemp[1024]= {0};
	TString strDirTimeCurrent, strLoggingPathNew;
	SYSTEMTIME st; GetLocalTime(&st);

	strDirTimeCurrent.Format( "%04d%02d%02d" , st.wYear , st.wMonth , st.wDay );
	strLoggingPathNew.Format( "%s\\", strDirTimeCurrent.CStr()  );
	
 	TString strFileOrg = _TEXT("DATALOG\\ProcessLog\\")+strLoggingPathNew + strFile_Old;
 	TString strFileRenew = _TEXT("DATALOG\\ProcessLog\\")+strLoggingPathNew + strFile_Err +_TEXT("_ABORT.CSV");
	

	_sleep(500);

	result = rename(strFileOrg.CStr(), strFileRenew.CStr());

	
	if (result == 0) {sprintf(szTemp, "Process Abort.. Log Data Rename Success : %s_ABORT.csv"  , strFile_Err.CStr()); Make_Log("", szTemp, EVENT);
					   printf("===== > Process Abort.. Log Data Rename Success : %s_ABORT.csv\n", strFile_Err.CStr());}
	else			 {sprintf(szTemp, "Process Abort.. Log Data Rename Fail : %s"  , strFile_Err.CStr()); Make_Log("", szTemp, EVENT);
					   printf("===== > Process Abort.. Log Data Rename Fail : %s\n", strFile_Err.CStr());}

}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL GetRRecipeInfoFromSchedulerData(const char* szCommand) {
	TString strCommand = szCommand;
	TStringArray strTokens = strCommand.GetToken( _TEXT(" \t") );

	memset( &RECIPEINFO , 0 , sizeof(RecipeInfoStruct) );

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	//                                         PORT1                              PORT2                              PORT3                              PORT4							   PORT5                              PORT1 Log Directory							    PORT2 Log Directory						          PORT3 Log Directory PORT4 Log Directory PORT4 Log Directory
	//szCommand= Recipe/Process/Test|1|sp1|sp2|MID:JobID:PPID:cmslot1:cmslot2:...|MID:JobID:PPID:cmslot1:cmslot2:...|MID:JobID:PPID:cmslot1:cmslot2:...|MID:JobID:PPID:cmslot1:cmslot2:...|MID:JobID:PPID:cmslot1:cmslot2:...|D:/NPS_RHP620/Scheduler/JobLog/20100518_224248203|D:/NPS_RHP620/Scheduler/JobLog/20100518_224248203|PORT3 Log Directory|PORT4 Log Directory|PORT5 Log Directory|
	//           0					 1  2   3  4								  5									 6									7								   8								  9												    10				                                  11                  12	              13
	//           Recipe Full Path  
	//                               0:Pre  Process
	//								 1:Main Process
	//								 2:Post Process
	//								 9:Manual Process
	//								 999:
	//								 CM1:Cassette, 5:slot, A:Place Arm
	int source=ePORT1, cnt, i;
	TString strCmd;
	TStringArray strArg, strArg_0, strArg_1, strArg_2, strArg_3, strArg_4, strArg_5, strArg_6, strArg_7, strArg_8, strArg_9, strArg_10, strArg_11;
	//
	strCmd  = szCommand;
	strArg  = strCmd.GetToken( _TEXT("|") );
	cnt     = strArg.count();
	strArg_0= strArg[0].GetToken( _TEXT("/") );
	cnt     = strArg_0.count();
	sprintf( RECIPEINFO.RECIPENAME , "%s" , (char *)strArg_0[cnt-1].CStr() );

	RECIPEINFO.RECIPETYPE = strArg[1].ToInt();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	if ( RECIPEINFO.RECIPETYPE == 1 ) {	//AUTO-RUN : MAIN PROCESS 
//		// DUMMY TYPE
//		if ( strArg[3].Compare( _TEXT("D") ) ) RECIPEINFO.DUMMY=TRUE ;
//		else								   RECIPEINFO.DUMMY=FALSE;
		RECIPEINFO.DUMMY=FALSE;

		//--------------------------//
		// PORT1 PROCESS LOG INFO	//
		//--------------------------//
		source=ePORT1;
		strArg_4= strArg[4].GetToken( _TEXT(":") );
		cnt     = strArg_4.count()-1;
		if ( cnt >= 4 ) {
			_tprintf(_TEXT("=========> PORT1 [CARRIERID = %s]\n"), (char *)strArg_4[0].CStr() );
			_tprintf(_TEXT("=========> PORT1 [JOBNAME   = %s]\n"), (char *)strArg_4[1].CStr() );
			_tprintf(_TEXT("=========> PORT1 [RECIPE    = %s]\n"), (char *)strArg_4[2].CStr() );
			_tprintf(_TEXT("=========> PORT1 [SLOT      = %s]\n"), (char *)strArg_4[3].CStr() );
			_tprintf(_TEXT("=========> PORT1 [LOGDIRNAME= %s]\n"), (char *)strArg[9].CStr()   );

			sprintf( RECIPEINFO.PORTINFO[source].CARRIERID , "%s", (char *)strArg_4[0].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].JOBNAME   , "%s", (char *)strArg_4[1].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].LOGDIRNAME, "%s", (char *)strArg[9].CStr()  ); 

			RECIPEINFO.PORTINFO[source].PORTUSE    = TRUE;
			RECIPEINFO.PORTINFO[source].WAFERCOUNT = cnt - 3;

			_tprintf(_TEXT("====> PORT%d, CARRIERID    = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].CARRIERID );
			_tprintf(_TEXT("====> PORT%d, JOBNAME      = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].JOBNAME   );
			_tprintf(_TEXT("====> PORT%d, LOGDIRNAME   = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].LOGDIRNAME);
			_tprintf(_TEXT("====> PORT%d, WAFERCOUNT   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERCOUNT);

			for(i=3; i<cnt; i++) {
				RECIPEINFO.PORTINFO[source].WAFERID[i-3] = strArg_4[i].ToInt();
				_tprintf(_TEXT("=============> PORT%d, WAFERID   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERID[i-3] );
			}
		}

		//--------------------------//
		// PORT2 PROCESS LOG INFO	//
		//--------------------------//
		source=ePORT2;
		strArg_5= strArg[5].GetToken( _TEXT(":") );
		cnt     = strArg_5.count()-1;
		if ( cnt >= 4 ) {
			_tprintf(_TEXT("=========> PORT2 [CARRIERID = %s]\n"), (char *)strArg_5[0].CStr() );
			_tprintf(_TEXT("=========> PORT2 [JOBNAME   = %s]\n"), (char *)strArg_5[1].CStr() );
			_tprintf(_TEXT("=========> PORT2 [RECIPE    = %s]\n"), (char *)strArg_5[2].CStr() );
			_tprintf(_TEXT("=========> PORT2 [SLOT      = %s]\n"), (char *)strArg_5[3].CStr() );
			_tprintf(_TEXT("=========> PORT2 [LOGDIRNAME= %s]\n"), (char *)strArg[10].CStr()   );

			sprintf( RECIPEINFO.PORTINFO[source].CARRIERID , "%s", (char *)strArg_5[0].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].JOBNAME   , "%s", (char *)strArg_5[1].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].LOGDIRNAME, "%s", (char *)strArg[10].CStr()  ); 
			
			RECIPEINFO.PORTINFO[source].PORTUSE    = TRUE;
			RECIPEINFO.PORTINFO[source].WAFERCOUNT = cnt - 3;

			_tprintf(_TEXT("====> PORT%d, CARRIERID    = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].CARRIERID );
			_tprintf(_TEXT("====> PORT%d, JOBNAME      = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].JOBNAME   );
			_tprintf(_TEXT("====> PORT%d, LOGDIRNAME   = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].LOGDIRNAME);
			_tprintf(_TEXT("====> PORT%d, WAFERCOUNT   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERCOUNT);

			for(i=3; i<cnt; i++) {
				RECIPEINFO.PORTINFO[source].WAFERID[i-3] = strArg_5[i].ToInt();
				_tprintf(_TEXT("=============> PORT%d, WAFERID   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERID[i-3] );
			}
		}

		//--------------------------//
		// PORT3 PROCESS LOG INFO	//
		//--------------------------//
		source=ePORT3;
		strArg_6= strArg[6].GetToken( _TEXT(":") );
		cnt     = strArg_6.count()-1;
		if ( cnt >= 4 ) {
			_tprintf(_TEXT("=========> PORT3 [CARRIERID = %s]\n"), (char *)strArg_6[0].CStr() );
			_tprintf(_TEXT("=========> PORT3 [JOBNAME   = %s]\n"), (char *)strArg_6[1].CStr() );
			_tprintf(_TEXT("=========> PORT3 [RECIPE    = %s]\n"), (char *)strArg_6[2].CStr() );
			_tprintf(_TEXT("=========> PORT3 [SLOT      = %s]\n"), (char *)strArg_6[3].CStr() );
			_tprintf(_TEXT("=========> PORT3 [LOGDIRNAME= %s]\n"), (char *)strArg[11].CStr()  );

			sprintf( RECIPEINFO.PORTINFO[source].CARRIERID , "%s", (char *)strArg_6[0].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].JOBNAME   , "%s", (char *)strArg_6[1].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].LOGDIRNAME, "%s", (char *)strArg[11].CStr()  ); 
			
			RECIPEINFO.PORTINFO[source].PORTUSE    = TRUE;
			RECIPEINFO.PORTINFO[source].WAFERCOUNT = cnt - 3;

			_tprintf(_TEXT("====> PORT%d, CARRIERID    = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].CARRIERID );
			_tprintf(_TEXT("====> PORT%d, JOBNAME      = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].JOBNAME   );
			_tprintf(_TEXT("====> PORT%d, LOGDIRNAME   = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].LOGDIRNAME);
			_tprintf(_TEXT("====> PORT%d, WAFERCOUNT   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERCOUNT);

			for(i=3; i<cnt; i++) {
				RECIPEINFO.PORTINFO[source].WAFERID[i-3] = strArg_6[i].ToInt();
				_tprintf(_TEXT("=============> PORT%d, WAFERID   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERID[i-3] );
			}
		}

		//--------------------------//
		// PORT4 PROCESS LOG INFO	//
		//--------------------------//
		source=ePORT4;
		strArg_7= strArg[7].GetToken( _TEXT(":") );
		cnt     = strArg_7.count()-1;
		if ( cnt >= 4 ) {
			_tprintf(_TEXT("=========> PORT4 [CARRIERID = %s]\n"), (char *)strArg_7[0].CStr() );
			_tprintf(_TEXT("=========> PORT4 [JOBNAME   = %s]\n"), (char *)strArg_7[1].CStr() );
			_tprintf(_TEXT("=========> PORT4 [RECIPE    = %s]\n"), (char *)strArg_7[2].CStr() );
			_tprintf(_TEXT("=========> PORT4 [SLOT      = %s]\n"), (char *)strArg_7[3].CStr() );
			_tprintf(_TEXT("=========> PORT4 [LOGDIRNAME= %s]\n"), (char *)strArg[12].CStr()  );

			sprintf( RECIPEINFO.PORTINFO[source].CARRIERID , "%s", (char *)strArg_7[0].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].JOBNAME   , "%s", (char *)strArg_7[1].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].LOGDIRNAME, "%s", (char *)strArg[12].CStr()  ); 
			
			RECIPEINFO.PORTINFO[source].PORTUSE    = TRUE;
			RECIPEINFO.PORTINFO[source].WAFERCOUNT = cnt - 3;

			_tprintf(_TEXT("====> PORT%d, CARRIERID    = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].CARRIERID );
			_tprintf(_TEXT("====> PORT%d, JOBNAME      = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].JOBNAME   );
			_tprintf(_TEXT("====> PORT%d, LOGDIRNAME   = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].LOGDIRNAME);
			_tprintf(_TEXT("====> PORT%d, WAFERCOUNT   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERCOUNT);

			for(i=3; i<cnt; i++) {
				RECIPEINFO.PORTINFO[source].WAFERID[i-3] = strArg_7[i].ToInt();
				_tprintf(_TEXT("=============> PORT%d, WAFERID   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERID[i-3] );
			}
		}

		//--------------------------//
		// PORT5 PROCESS LOG INFO	//
		//--------------------------//
		source=ePORT5;
		strArg_8= strArg[8].GetToken( _TEXT(":") );
		cnt     = strArg_8.count()-1;
		if ( cnt >= 4 ) {
			_tprintf(_TEXT("=========> PORT4 [CARRIERID = %s]\n"), (char *)strArg_8[0].CStr() );
			_tprintf(_TEXT("=========> PORT4 [JOBNAME   = %s]\n"), (char *)strArg_8[1].CStr() );
			_tprintf(_TEXT("=========> PORT4 [RECIPE    = %s]\n"), (char *)strArg_8[2].CStr() );
			_tprintf(_TEXT("=========> PORT4 [SLOT      = %s]\n"), (char *)strArg_8[3].CStr() );
			_tprintf(_TEXT("=========> PORT4 [LOGDIRNAME= %s]\n"), (char *)strArg[13].CStr()  );

			sprintf( RECIPEINFO.PORTINFO[source].CARRIERID , "%s", (char *)strArg_8[0].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].JOBNAME   , "%s", (char *)strArg_8[1].CStr()); 
			sprintf( RECIPEINFO.PORTINFO[source].LOGDIRNAME, "%s", (char *)strArg[13].CStr()  ); 
			
			RECIPEINFO.PORTINFO[source].PORTUSE    = TRUE;
			RECIPEINFO.PORTINFO[source].WAFERCOUNT = cnt - 3;

			_tprintf(_TEXT("====> PORT%d, CARRIERID    = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].CARRIERID );
			_tprintf(_TEXT("====> PORT%d, JOBNAME      = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].JOBNAME   );
			_tprintf(_TEXT("====> PORT%d, LOGDIRNAME   = %s\n"), source+1, (char *)RECIPEINFO.PORTINFO[source].LOGDIRNAME);
			_tprintf(_TEXT("====> PORT%d, WAFERCOUNT   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERCOUNT);

			for(i=3; i<cnt; i++) {
				RECIPEINFO.PORTINFO[source].WAFERID[i-3] = strArg_8[i].ToInt();
				_tprintf(_TEXT("=============> PORT%d, WAFERID   = %d\n"), source+1, RECIPEINFO.PORTINFO[source].WAFERID[i-3] );
			}
		}
	}
	else {		// Manual Process
		for( int i=0; i<MAX_PORTMDL_COUNT; i++ ) {
			sprintf( RECIPEINFO.PORTINFO[i].LOGDIRNAME, "DataLog\\ProcessLog\\PC%02d", CURRENT_PM_MODULE.ReadFromMem()+1 ); 
			RECIPEINFO.PORTINFO[i].PORTUSE    = TRUE;
			RECIPEINFO.PORTINFO[i].WAFERCOUNT = 1;
		}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	return TRUE;
} 
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------

SEQ_STATUS SeqMain(const char* szCommand, const void* pParam, int nNumOfBytesRecved) 
{
	int		 cnt, i;
	TString strCmd;
	TStringArray strArg, strArg_0, strArg_1, strArg_2, strArg_3, strArg_4, strArg_5, strArg_6, strArg_7, strArg_8, strArg_9, strArg_10, strArg_11;
	SEQ_STATUS nResult				= SEQ_ABORT;
	BOOL	 bRet					= FALSE;

	char szTemp[1024]		= {0};
	char ProcRecipeName[50]	= {0,};

	printf("============> [%s] START...[%s]\n", GetSeqName(), szCommand);
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	//           0					1 2 3 4:PORT1                      5:PORT2                      6:PORT3   7:PORT4   8:PORT5	  9:PORT1 CTC Log Directory			   10:   11:	12	   13     14:PORT1 PJ Name List
	//szCommand=>[RECIPE/PC04/TEST4|1|1|P|:CJ20171101160900287:TEST:25:|:CJ20171101154510591::(null)|:::(null)|:::(null)|:::(null)|Scheduler/JobLog/20171101_160900287|(null)|(null)|(null)|(null)|PJ2017110116090028701;(null);(null);(null);(null)]
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	//	[0]=Recipe Full Path  
	//	[1]=Process Type(1:Main/2:IdleEmpty/3:CountEmpty), User Use=9:Manual
	//	[2]=Process Depth
	//	[3]=SubstType(P:PP/D:DUMMY)
	//
	//	[4]=PORT1 Run Info ==> [0]:MID, [1]:JobID, [2]:PPID, [3]:Slot
	//	[5]=PORT2 Run Info ==> [0]:MID, [1]:JobID, [2]:PPID, [3]:Slot
	//	[6]=PORT3 Run Info ==> [0]:MID, [1]:JobID, [2]:PPID, [3]:Slot
	//	[7]=PORT4 Run Info ==> [0]:MID, [1]:JobID, [2]:PPID, [3]:Slot
	//	[8]=PORT5 Run Info ==> [0]:MID, [1]:JobID, [2]:PPID, [3]:Slot
	//
	//	[9] =PORT1 JobLog Directory
	//	[10]=PORT2 JobLog Directory
	//	[11]=PORT3 JobLog Directory
	//	[12]=PORT4 JobLog Directory
	//	[13]=PORT5 JobLog Directory
	//
	//	[14]=Port PJ Name List ==> [0]:PORT1, [1]:PORT2, [2]:PORT3, [3]:PORT4, [4]:PORT5
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	strCmd  = szCommand;
	strArg  = strCmd.GetToken( "|" );
	cnt     = strArg.count();

	sprintf(szTemp, "Process Run all Command[%s] :%s",szCommand); 
	Make_Log("", szTemp, EVENT);

	for(i=0; i<cnt; i++) 
	{
		sprintf(szTemp, "Process Run Cmd strArg[%d] :%s",i, strArg[i]);
		Make_Log("", szTemp, EVENT);

	}

	strArg_0= strArg[0].GetToken( "/" );
	cnt     = strArg_0.count();

	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	// Recipe Info Read from Scheduler Command
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//
	if(strArg[1].ToInt()!=9) 
	{	
		
		Make_Log("", "GET RECIPEINFO_FROM_SCHEDULER_DATA START  ", EVENT);
		bRet  = GET_RECIPEINFO_FROM_SCHEDULER_DATA( szCommand , &RECIPEINFO , TRUE , FALSE , TRUE , TRUE );

		if( !bRet )	
		{
			Make_Log("", "GET RECIPEINFO_FROM_SCHEDULER_DATA ERROR  ", FAULT);
		}
	}

	if ( strArg.count() <= 8 )
	{		//MANUAL-RUN
		for( i=0; i<MAX_PORTMDL_COUNT; i++ ) 
		{
			sprintf( RECIPEINFO.PORTINFO[i].LOGDIRNAME, "DataLog\\ProcessLog\\PC%02d", CURRENT_PM_MODULE.ReadFromMem()+1 ); 
			RECIPEINFO.PORTINFO[i].PORTUSE    = TRUE;
			RECIPEINFO.PORTINFO[i].WAFERCOUNT = 1;
		}
	}
	//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++//

	sprintf(RECIPE_NAME, "%s", strArg_0[cnt-1].CStr()); 
	Rcp_Name.WriteTh( RECIPE_NAME );

	//
	RECIPE_FILE.Write( RECIPE_NAME );
	//

	sprintf(szTemp, "Process Run Cmd Recipe :%s",RECIPE_NAME);
	Make_Log("", szTemp, EVENT);

	int		CS						= 0;
	int		nProcCondition			= 0;
	int		nStartStep				= 0;
	int		nAgingCounts			= 0;
	int		nCheckTime				= 600;
	int		nAutoPresetCount		= 0;
	int		nPMCount				= 0;
	int		nAutoPresetRun			= 0;
	int		CfgProcEnd_PinCond		= 0;
	int		CfgProcEnd_dPinPos		= 0;
	int		CfgDischargeStep_Cond	= 0;
	int		n						= 0;
	int		j						= 0;
	int		nIdleOrNormal_Process;
	int		nCountOrNormal_Process;

	double	nIdleProcessCount		= 0;
	double	nCountProcessCount		= 0;
	double	StartTime				= 0;
	double	CfgProcEnd_aPinPos		= 0.0;

	char GlassID[128]					= {0};
	char szCountProcessEndMessage[256]	= {0};
	char szIdleProcessEndMessage[256]	= {0};
	char Glass_ID[50]					= {0,};
	char Slot_ID[50]					= {0,};
	char Port_ID[50]					= {0,};

	TString strFile;

	SYSTEMTIME st;
	GetLocalTime(&st);

	g_dblTotalProcessTime = 0.0;
	g_dblJustEtchTime     = 0.0;

	StartTime = clock();

	GLASS_ID.Read(mGlassID,CS);
	Rcp_Name.Read(mRecipeID,CS);
	PM_ModuleName.Read(mPcName,CS);
	eAO_JustEtchTime.Write(0,CS);
	eAO_TotalEtchTime.Write(0,CS);
	OnMessage.Write(RECIPE_NAME,CS);

	sprintf(szTemp, "Process Recipe Name :[%s]",RECIPE_NAME);
	Make_Log("", szTemp, EVENT);

	StepProcEnable.Write(0,CS);
	
	nIdleOrNormal_Process	=	EmpORNormal_Process.Read(CS);
	nIdleProcessCount		=	EmptyProcess_Count.Read(CS);
	nCountOrNormal_Process	=	CountORNormal_Process.Read(CS);
	nCountProcessCount		=	CountProcess_Count.Read(CS);
	
	Rcp_CurStepNum.Write(0);
	Step_CurTime.Write(0);
	dHW_Interlock_On_Status.Write(OFF,CS);

	DO_IG_ONOFF.Write(OFF, CS);
	DO_IG_SHUT_OFF_VALVE.Write(eVLV_CLOSE, CS);

	if(strArg[1] == NOMAL_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process ==Normal_Process2))
	{
		Process_Condition.Write(eCon_Start,CS);
		PM_IsRunEMP.Write(eCon_Start,CS);
		CountProcess_IsRun.Write(CountPro_Idle,CS);

		Make_Log("", "Normal Process Mode(Not Empty Process)", EVENT);

	}
	else //Idle OR Count Process 
	{
		Process_Condition.Write(eCon_IDLE,CS);
		
		if(strArg[1] == IDLE_RUN || ( strArg[1] == 9 && nIdleOrNormal_Process == IDLE_Process && nCountOrNormal_Process == Normal_Process2 ))
		{
			PM_IsRunEMP.Write(EmP_Run,CS);
			CountProcess_IsRun.Write(CountPro_Idle,CS);
			Make_Log("", "Empty Idle Process Mode", EVENT);
			Empty_Idle_Process.Write(1);//Idle Process
			
		}
		else if(strArg[1] == COUNT_RUN|| (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process == Count_Process))
		{
		
			PM_IsRunEMP.Write(EmP_Idle,CS);
			CountProcess_IsRun.Write(CountPro_Run,CS);
			Make_Log("", "Empty Count Process Mode", EVENT);
			Empty_Idle_Process.Write(2);//Count Process
		}
		else
		{
			EmptyProcessAborted.Clear();
			EmptyProcessAborted.PostAnyTh();
			Make_Log("", "Can not Run Idle & Count Empty Process", FAULT);
			return SEQ_ABORT;
		}
	}

		
	nIdleOrNormal_Process	=	EmpORNormal_Process.Read(CS);
	nCountOrNormal_Process	=	CountORNormal_Process.Read(CS);
	
	if(strArg[1] == NOMAL_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process == Normal_Process2))
	{
		
		if(Glass_Existence_CHK.Read(CS) ==eYesCheck)
		{
			n=0;
			while(1)
			{
				if(PM_GLASS_STS.Read(CS) > GLS_NONE) break;
				
				n++; 
				if(WaitAbort(200))
				{
					return SEQ_ABORT;
				}
				
				if(n>50)
				{
					NoGlassProcessAborted.Clear();
					NoGlassProcessAborted.PostAnyTh();
					
					Make_Log("", "Can't Run to process. PM_GLASS_STS(= GLS_NONE)", FAULT);
					
					return SEQ_ABORT;
				}
			}
		}
		else
		{
			Make_Log("", "Glass Existence Check Option: NoCheck", EVENT);
		}
		
	}
	else 
	{
		Make_Log("", "Empty Process is Running : NoCheck", EVENT);
		
		if(PM_GLASS_STS.Read(CS) > GLS_NONE)
		{
			InGlassProcessAborted.Clear();
			InGlassProcessAborted.PostAnyTh();
			
			Make_Log("", "Can't Run to process. PM_GLASS_STS(= GLS_NONE)", FAULT);
	
			return SEQ_ABORT;
			
		}
		
 		if(nIdleOrNormal_Process == IDLE_Process && nCountOrNormal_Process == Count_Process)
 		{
 			EmptyProcessAborted.Clear();
 			EmptyProcessAborted.PostAnyTh();
 			
 			Make_Log("", "Can not Run Idle & Count Empty Process", FAULT);
 			
			return SEQ_ABORT;
 			
 		}
		
	}
	
	PM_PROC_STS.Write(eFNC_RUNNING,CS);

	OnMessage.Write(RECIPE_NAME,CS);		
	if(PM_PROCRUN_MODE.Read(CS) == ePROCESS_AGING )
	{	
		nAgingCounts = (int)PM_AGING_COUNTS.Read(CS);
		
		if(nAgingCounts > 0)
		{
			sprintf(szTemp, "Call AGING_PROCESS, PM_AGING_COUNTS = %d", nAgingCounts);
			Make_Log("", szTemp, EVENT);
		}
		else
		{
			PM_AGING_COUNTS.Write(0,CS);
			PM_PROC_CTRL.Write(0,CS);
			PM_PROCRUN_MODE.Write(ePROCESS_NORMAL,CS);
			
			FNC_PROC_JumpStep.Write(0,CS);
			
			Make_Log("", "SYS_ABORTED : AGING_PROCESS is canceled", EVENT);
			
			AgingZeroProcessAborted.Clear();
			AgingZeroProcessAborted.PostAnyTh();
			
			nResult = SEQ_ABORT;
			goto PROCESS_END;
		}
		

		nResult = AGING_PROCESS(nAgingCounts,pParam, nNumOfBytesRecved);
			
		Make_Log("", "Completed AGING_PROCESS", EVENT);
		
		return nResult;
	}
	
	sGUI_Proc_msg.Write(_TEXT("Prepare Condition."));	
	DO_IG_SHUT_OFF_VALVE.Write(eVLV_CLOSE,CS);
	
	
	//RECIPE READ.. 
	sGUI_Proc_msg.Write(_TEXT("Reading RECIPE File..."));
	_Init_Step(0);

	sprintf(szTemp, "Recipe Reading Check1 [pParam:: %p, nNumOfBytesRecved:: %d]",pParam, nNumOfBytesRecved);
	Make_Log("", szTemp, EVENT);

	if((void*)pParam == NULL || nNumOfBytesRecved == 0)	
	{
		sprintf(szTemp, " Recipe Reading Fail. Don't have a Parameter pParam:: %p, nNumOfBytesRecved:: %d" ,pParam, nNumOfBytesRecved);
		Make_Log("", szTemp, EVENT);

		RECIPEFAIL_1201.PostAnyTh();
		
		_sleep(200);
		goto PROCESS_END;
		
	}

	nResult = RECIPE_READING(pParam, nNumOfBytesRecved);

	sprintf(szTemp, "Recipe Reading Check2 [pParam:: %p, nNumOfBytesRecved:: %d] ",pParam, nNumOfBytesRecved);
	Make_Log("", szTemp, EVENT);

	PROCESSLOG.Run( "RECIPE_INFO" , &RECIPEINFO , sizeof(RecipeInfoStruct) );
	PROCESSLOG.Run( "LOG_START" );
	Process_Run.Write(ON,CS);
	Datalog_Control.Write(ON,CS);
	GLASS_ID.Read(Glass_ID, CS);
	PORT_ID.Read(Port_ID,CS);
	SLOT_ID.Read(Slot_ID,CS);

	j=0;

	for(i=0; i<strlen(RECIPE_NAME); i++)
	{
		if(RECIPE_NAME[i] == 'P' && RECIPE_NAME[i+1] == 'C' )	j = i + 5;
	}
	strncpy(ProcRecipeName, RECIPE_NAME + j, 50);

	if(nResult != SEQ_SUCCESS)	
	{
		
		if(g_bDechuck_powerState == FALSE)	RECIPEFAIL_1202.PostAnyTh();
		if(g_nIdleESCPowerOnState== FALSE)	RECIPEFAIL_1208.PostAnyTh();
		
		Make_Log("", "RECIPE_READING() = SYS_ABORTED", FAULT);
	
		PM_PROC_STS.Write(eFNC_ABORTED,CS);
		nResult = SEQ_ABORT;
		
		sGUI_Proc_msg.Write(_TEXT("Fail to Reading RECIPE File!!!  Abort Process"));
		goto PROCESS_END;
	}
	else
	{
		if( STEP_COUNT <= 0 ) 
		{

			RECIPEFAIL_1201.PostAnyTh();
			
			if		(strArg[1] ==IDLE_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == IDLE_Process)) 
			{
				Make_Log("", "Empty Process(IDLE) Recipe is Error. Check Process(IDLE) Recipe", FAULT);
			}
			else if (strArg[1] ==COUNT_RUN || (strArg[1] == 9 && nCountOrNormal_Process == Count_Process)) 
			{
				Make_Log("", "Empty Process(COUNT) Recipe is Error. Check Process(COUNT) Recipe", FAULT);
			}
			else 
			{
				Make_Log("", "Recipe Reading Error. Check Recipe &Restart", FAULT);
			}
			
			Make_Log("", "STEP COUNT <= 0", FAULT);
			PM_PROC_STS.Write(eFNC_ABORTED,CS);
			
			sGUI_Proc_msg.Write(_TEXT("Total Step Count <= 0.  Abort Process..."));
			
			nResult = SEQ_ABORT;
			goto PROCESS_END;
		}
	}

	//HE FLOW CHECK 
	if(PM_GLASS_STS.Read(CS) == GLS_FAILURE) FNC_HE.Run(_TEXT("INIT"));

	Make_Log("", "Call SetRcpParamFristStep", EVENT);
	SetRcpParamFristStep(1);

	if(PM_PROCRUN_MODE.Read(CS) == ePROCESS_NORMAL)
	{
		DO_PROCESS_GAS_SUPPLY_VALVE.Write(eVLV_OPEN, CS);

		if( STEP_COUNT==1 )
		{
			if(AI_CPin1Pos.Read(CS) >= cPinPosition2_mmSET.Read(CS)-2)
			{
				sGUI_Proc_msg.Write(_TEXT("Max 1 Step Process Start"));
				Make_Log("", "STEP MAX COUNT is 1. Process Start", EVENT);		
				FNC_PIN.Run(_TEXT("STEP1_MOVE")); 
			}
			else
			{
				PROCESS_1140.PostAnyTh();
				Make_Log("", "PIN MOVING FAIL.", FAULT);
				nResult = SEQ_ABORT;
				sGUI_Proc_msg.Write(_TEXT("Fail To Move Pin!!!  Abort Process."));
				goto PROCESS_END;

			}
		}
		else
		{
			sGUI_Proc_msg.Write(_TEXT("Move Pin PROS_POS1..."));				
			FNC_PIN.Run(_TEXT("PROS_POS1"));

		}
		
		nResult = FNC_PIN.Status();
			
		if(nResult == SEQ_ABORT)
		{
			PROCESS_1140.Clear();
			PROCESS_1140.PostAnyTh();
			Make_Log("", "PIN MOVING FAIL.", FAULT);
			nResult = SEQ_ABORT;
				
			sGUI_Proc_msg.Write(_TEXT("Fail To Move Pin!!!  Abort Process."));
			goto PROCESS_END;
		}

		if (nDechuckStepNumber == 0 && PM_GLASS_STS.Read(CS) > GLS_NONE && STEP_COUNT!=1) 
		{
			InGlassRFNoSetAborted.PostAnyTh();
			PM_PROC_STS.Write(eFNC_ABORTED,CS);
			sGUI_Proc_msg.Write(_TEXT("Dechuck Step No Setting.  Abort Process..."));
			Make_Log("", "Dechuck Step No Setting.", FAULT);
				
			nResult = SEQ_ABORT;
			goto PROCESS_END;			
		}
		
		if (cVVC_PreSetUse.Equal(Option_Use))
		{
			sGUI_Proc_msg.Write(_TEXT("VVC MOTOR PRESET RUN"));
			FNC_VVC_MOTOR.Run(_TEXT("PRESET"));

			nResult = FNC_VVC_MOTOR.Status();
			
			if(nResult == SEQ_ABORT)
			{
				PROCESS_1141.Clear();
				PROCESS_1141.PostAnyTh();
				Make_Log("", "VVC MOTOR PRESET FAIL. FNC_VVC_MOTOR|PRESET", FAULT);
				nResult = SEQ_ABORT;
				
				sGUI_Proc_msg.Write(_TEXT("Fail To VVC MOTOR PRESET!!!  Abort Process."));
				goto PROCESS_END;
			}

		}

		if( (PR_CHILLER_CH01_TEMP.Read(CS) > 0 && cCHL_CH1_Temp.Read(CS) != PR_CHILLER_CH01_TEMP.Read(CS)) ||
			(PR_CHILLER_CH02_TEMP.Read(CS) > 0 && cCHL_CH2_Temp.Read(CS) != PR_CHILLER_CH02_TEMP.Read(CS)) ||	
			(PR_CHILLER_CH03_TEMP.Read(CS) > 0 && cCHL_CH3_Temp.Read(CS) != PR_CHILLER_CH03_TEMP.Read(CS)) )
		{
			InGlassChillerAborted.PostAnyTh();
			PM_PROC_STS.Write(eFNC_ABORTED,CS);
			sGUI_Proc_msg.Write(_TEXT("Chiller Use Temp Set Wrong"));
			Make_Log("", "Chiller Use Temp Set Wrong.", FAULT);
			
			nResult = SEQ_ABORT;
			goto PROCESS_END;			
		}
		

	}
	else
	{
		sGUI_Proc_msg.Write(_TEXT("PM_PROCRUN_MODE is invalid. Abort..."));
		Make_Log("", "SYS_ABORTED : PM_PROCRUN_MODE is invalid", FAULT);
		goto PROCESS_END;
	}
	Rcp_TotalTime.Write(TOTAL_TIME,CS);
	Rcp_CurTime.Write(0,CS);
	


	
	nCheckTime = 0;	
	while( TRUE )
	{		
		nCheckTime++;
		
		if( GetStatusAllFunction() > 0 )
		{
			Make_Log("", "All Sub Function is idle", EVENT);
			break;
		}
		
		if(FuncAbortChk() == TRUE)
		{
			PROCESS_1160.Clear();
			ProcessAborted.PostAnyTh();
			Make_Log("", "SUB FUNCTION IS NOW RUNNING. Cancel to wait for function idle status", FAULT);
						
			sGUI_Proc_msg.Write(_TEXT("Sub Function is now running!!!  Abort Process..."));
			nResult = SEQ_ABORT;

			goto PROCESS_END;
		}
		
		if(nCheckTime > 600)
		{
			PROCESS_1160.Clear();
			ProcessAborted.PostAnyTh();
			Make_Log("", "SUB FUNCTION IS NOW RUNNING. Cancel to wait for function idle status[60sec]", FAULT);
			
			Process_Condition.Write(eCon_Fail,CS);
			
			sGUI_Proc_msg.Write(_TEXT("Sub Function is now running!!!  Abort Process..."));
			nResult = SEQ_ABORT;

			goto PROCESS_END;
		}
		
		if(WaitAbort(100))	{ Process_End_Control(); Process_LOG_Rename(); return SEQ_ABORT; } //181121 KYS Abort Add
	}
	
	Make_Log("", "CALL PROCESS_CONTROL()", EVENT);
	sGUI_Proc_msg.Write(_TEXT("Run PROCESS CONTROL..."));

	_Init_Step(0);

	strFile.Format( _TEXT("%02d%02d_%02d%02d_%s_%s_%s_%s.csv") , st.wMonth , st.wDay , st.wHour , st.wMinute , Port_ID , Slot_ID , Glass_ID , ProcRecipeName  );

	strFile_Old = strFile; //181121 KYS Abort Add

	strFile_Err.Format( _TEXT("%02d%02d_%02d%02d_%s_%s_%s_%s") , st.wMonth , st.wDay , st.wHour , st.wMinute , Port_ID , Slot_ID , Glass_ID , ProcRecipeName  ); //181121 KYS Abort Add


	if(SEQ_RUNNING == utilIOLogPC.Status())
	{
		utilIOLogPC.SendEventMsg(_TEXT("STOP"));
		Sleep(100);
	}

	utilIOLogPC.RunTh( strFile );			

	//----------------------------------------------------------------------------------------
	// PROCESS START
	nResult = PROCESS_CONTROL( nStartStep );

	if(nResult == SEQ_SUCCESS)
	{
		if(strArg[1] == NOMAL_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process ==Normal_Process2))
		{
			Process_Condition.Write(eCon_End,CS);
			PM_IsRunEMP.Write(EmP_Idle,CS);
			CountProcess_IsRun.Write(CountPro_Idle,CS);

			if(	AI_CPin1Pos.Read(CS) < cPinPosition3_mmSET.Read(CS)+5 )
			{
				Make_Log("", "PROCESS END. PIN_POS4 MOVE ACTION RUN", EVENT);
				FNC_PIN.Run(_TEXT("PROS_POS4"));
			}
			
			Make_Log("", "PM_GLASS_STS = PROCESSED", EVENT);
			sGUI_Proc_msg.Write(_TEXT("PROCESS CONTROL is Completed."));
			//
		}
		else 
		{
			Process_Condition.Write(eCon_IDLE,CS);
			
			if(strArg[1] == IDLE_RUN|| (strArg[1] == 9 && nIdleOrNormal_Process == IDLE_Process && nCountOrNormal_Process == Normal_Process2 ))
			{
				PM_IsRunEMP.Write(EmP_Stop,CS);
				CountProcess_IsRun.Write(CountPro_Idle,CS);
				
				Make_Log("", "Empty IDLE Process is Completed.", EVENT);
				sGUI_Proc_msg.Write(_TEXT("Empty IDLE Process is Completed."));
			}
			else if(strArg[1] == COUNT_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process == Count_Process))
			{
				PM_IsRunEMP.Write(EmP_Idle,CS);
				CountProcess_IsRun.Write(CountPro_Stop,CS);

				Make_Log("", "Empty COUNT Process is Completed.", EVENT);
				sGUI_Proc_msg.Write(_TEXT("Empty COUNT Process is Completed."));
			}
			else
			{
				EmptyProcessAborted.Clear();
				EmptyProcessAborted.PostAnyTh();
		
				Make_Log("", "Both Idle And Count Empty Process is Enable. Abort!!", FAULT);
				Process_End_Control();
				Process_LOG_Rename(); //181121 KYS Abort Add
				return SEQ_ABORT;
			}
		}

	
	}
	else						
	{
	
		if(strArg[1] == NOMAL_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process ==Normal_Process2))
		{
			Process_Condition.Write(eCon_Fail,CS);
			PM_IsRunEMP.Write(EmP_Idle,CS);
			CountProcess_IsRun.Write(CountPro_Idle,CS);
			
			Make_Log("", "PM_GLASS_STS = ABORTED", EVENT);
			sGUI_Proc_msg.Write(_TEXT("PROCESS CONTROL is Aborted."));
			
		}
		else 
		{
			Process_Condition.Write(eCon_IDLE,CS);
			if(strArg[1] == IDLE_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == IDLE_Process && nCountOrNormal_Process == Normal_Process2 ))
			{
				PM_IsRunEMP.Write(EmP_Error,CS);
				CountProcess_IsRun.Write(CountPro_Idle,CS);
			
				Make_Log("", "Empty IDLE Process is Abort.", EVENT);
				sGUI_Proc_msg.Write(_TEXT("Empty IDLE Process is Completed."));
			}
			else if(strArg[1] == COUNT_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process == Count_Process))
			{
				PM_IsRunEMP.Write(EmP_Idle,CS);
				CountProcess_IsRun.Write(CountPro_Error,CS);
				
				Make_Log("", "Empty COUNT Process is Completed.", EVENT);
				sGUI_Proc_msg.Write(_TEXT("Empty COUNT Process is Completed."));
			}
			else
			{
				EmptyProcessAborted.Clear();
				EmptyProcessAborted.PostAnyTh();
				
				Make_Log("", "Both Idle And Count Empty Process is Enable. Abort!!", FAULT);
				Process_End_Control();
				Process_LOG_Rename(); //181121 KYS Abort Add
				return SEQ_ABORT;
			}
		}


	}

	sprintf(szTemp, "cDI_ProcEnd_PinCond[ %d ], cDI_ProcEnd_dPinPos[ %d ], cAI_ProcEnd_aPinPos[ %.1f ]", 
					cDI_ProcEnd_PinCond.Read(CS), cDI_ProcEnd_dPinPos.Read(CS), cAI_ProcEnd_aPinPos.Read(CS));

	Make_Log("", szTemp, EVENT);
	
	sprintf(szTemp, "XI_Pin_CurPosition[ %d ], AI_CPin1Pos[ %.1f ]", XI_Pin_CurPosition.Read(CS), AI_CPin1Pos.Read(CS));
	Make_Log("", szTemp, EVENT);


PROCESS_END:

	Process_End_Control();

	if(nResult == SEQ_SUCCESS)		
	{
		PM_PROC_STS.Write(eFNC_IDLE,CS);
		if(strArg[1] == NOMAL_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process ==Normal_Process2))
		{
			Process_Condition.Write(eCon_End,CS);
			PM_IsRunEMP.Write(EmP_Idle,CS);
			CountProcess_IsRun.Write(CountPro_Idle,CS);
		}
		else
		{
			Process_Condition.Write(eCon_IDLE,CS);
			if(strArg[1] == IDLE_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == IDLE_Process && nCountOrNormal_Process == Normal_Process2 ))
			{
				PM_IsRunEMP.Write(EmP_Stop,CS);
				CountProcess_IsRun.Write(CountPro_Idle,CS);

				
				nIdleProcessCount++;
				EmptyProcess_Count.Write(nIdleProcessCount,CS);
				
				GetLocalTime(&st);
				sprintf(szIdleProcessEndMessage,"%02d/%02d %002d:%002d Idle_Proc End(%4.0f Count)",
												st.wMonth,st.wDay, st.wHour,st.wMinute,nIdleProcessCount);
				
				Make_Log("",szIdleProcessEndMessage, EVENT);
				sGUI_EmptyProcessEnd_msg.Write(szIdleProcessEndMessage,CS);
			}
			else if(strArg[1] == COUNT_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process == Count_Process))
			{
				PM_IsRunEMP.Write(EmP_Idle,CS);
				CountProcess_IsRun.Write(CountPro_Stop,CS);
				
				nCountProcessCount++;
				CountProcess_Count.Write(nCountProcessCount,CS);
				
				GetLocalTime(&st);
				sprintf(szCountProcessEndMessage,"%02d/%02d %002d:%002d Count_Proc End(%4.0f Count)",
												st.wMonth,st.wDay, st.wHour,st.wMinute,nCountProcessCount);
				
				Make_Log("",szCountProcessEndMessage, EVENT);
				sGUI_CountProcessEnd_msg.Write(szCountProcessEndMessage,CS);
			}	
			else
			{
				EmptyProcessAborted.Clear();
				EmptyProcessAborted.PostAnyTh();

				Make_Log("", "Both Idle And Count Empty Process is Enable. Abort!!", FAULT);

				Process_LOG_Rename(); //181121 KYS Abort Add
				return SEQ_ABORT;
			}
		}
	}
	else
	{
		PM_PROC_STS.Write(eFNC_ABORTED,CS);

		Process_LOG_Rename(); //181121 KYS Abort Add
		
		if(strArg[1] == NOMAL_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process ==Normal_Process2))
		{
			Process_Condition.Write(eCon_Fail,CS);
			PM_IsRunEMP.Write(EmP_Idle,CS);
			CountProcess_IsRun.Write(CountPro_Idle,CS);

			
		}
		else 
		{
			Process_Condition.Write(eCon_IDLE,CS);

			if(strArg[1] == IDLE_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == IDLE_Process && nCountOrNormal_Process == Normal_Process2 ))
			{
				PM_IsRunEMP.Write(EmP_Error,CS);
				CountProcess_IsRun.Write(CountPro_Idle,CS);
				
			}
			else if(strArg[1] == COUNT_RUN || (strArg[1] == 9 && nIdleOrNormal_Process == Normal_Process1 && nCountOrNormal_Process == Count_Process))
			{
				PM_IsRunEMP.Write(EmP_Idle,CS);
				CountProcess_IsRun.Write(CountPro_Error,CS);
				
			}
			else
			{
				EmptyProcessAborted.Clear();
				EmptyProcessAborted.PostAnyTh();
			}
		}
		
	}
	
	if(Glass_Existence_CHK.Read(CS) ==eNoCheck)		Process_Condition.Write(eCon_End,CS);
				
	PM_PROC_CTRL.Write(0,CS);
	
	WaitAbort(10);
	
	DO_CDG_SHUT_OFF_VALVE.Write(eVLV_OPEN,CS);	
	DO_IG_SHUT_OFF_VALVE.Write(eVLV_CLOSE,CS);	
	DO_ESCHE_OUTLET_VALVE.Write(eVLV_CLOSE,CS);
	DO_PROCESS_GAS_SUPPLY_VALVE.Write(eVLV_CLOSE,CS);
	
	EmpORNormal_Process.Write(Normal_Process1,CS);	
	CountORNormal_Process.Write(Normal_Process2,CS);	

	if(cHE_Monitor_ME.Read(CS) == 1) cHE_Monitor_ME.Write(0,CS);	
	
	FNC_PROC_JumpStep.Write(0,CS);	
	
	Make_Log("", "------------------------------------------------------------- process completed. ", EVENT);
	return nResult;
}

//----------------------------------------------------------------------------------------------------------------------------------------------
void OnLoadComplete( const TCHAR* szArgument ) 
{
	SetVersionInfo( _TEXT(VERSION_INFO) , _TEXT(BUILD_DATE) , _TEXT(BUILD_TIME) );
}

//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL OnInitComplete( const TCHAR* szArgument ) 
{
	int CommStatus = 0;
	if ( SIMULATION_CONTROL.Read(CommStatus) == ON ) SIMULATION_MODE = TRUE;
	else											 SIMULATION_MODE = FALSE;

	InitLog("Log//MAIN_PROCESS//FNC_PROC.log");
	MakeFile();
	
	print_log("--------------------------------------------------------------------", EVENT);
	print_log("------------------------- FUNCTION PROC LOG ------------------------", EVENT);
	print_log("--------------------------------------------------------------------", EVENT);

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
