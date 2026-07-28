#include <windows.h>
#include <stdio.h>

#include <tMain.h>
#include <TUtilStr.h>
#include <TTimer.h>
#include <TString.h>


#include "h/Define.h"
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
enum	{ OFF   , ON };

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL SIMULATION_MODE = FALSE;
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_OBJECT_ENUMERATION

	CDIO SIMULATION_CONTROL("System.SimulMode");

	CDIO		PM_Start_Flag				(_TEXT("PM_Start_Flag"			));
                                        
	CAIO        PR_SRFG_POWER			    (_TEXT("PR_SRFG_POWER"			));
	CAIO        PR_BRFG_POWER			    (_TEXT("PR_BRFG_POWER"			));
	CDIO        PM_PROC_STS		            (_TEXT("PM_PROC_STS"			));
                                        
	  // Virtual Input HRF                                     
	CDIO        SRF_CommSts       	        (_TEXT("SRF_CommSts"			));
	CDIO        SRF_MAT_CommSts             (_TEXT("SRF_MAT_CommSts"		));
                                        
	CDIO        SRF_StandBy_RD		        (_TEXT("SRF_StandBy_RD"			));
	CAIO        SRF_Setpoint_RA	            (_TEXT("SRF_Setpoint_RA"		));
	CAIO        SRF_FwdPwr_RA  	            (_TEXT("SRF_FwdPwr_RA"			));
	CAIO        SRF_RefPwr_RA  	            (_TEXT("SRF_RefPwr_RA"			));
	CAIO        SRF_Vdc_RA     	            (_TEXT("SRF_Vdc_RA"				));
	CAIO        SRF_Vpp_RA     	            (_TEXT("SRF_Vpp_RA"				));
	CAIO        SRFM_Match_Pos	            (_TEXT("SRFM_Match_Pos"			));
	CAIO        SRFM_Tune_Pos	            (_TEXT("SRFM_Tune_Pos"			));
                                        
	 // Virtual Input LRF                                       
	CDIO        BRF_CommSts       	        (_TEXT("BRF_CommSts"			));
	CDIO        BRF_MAT_CommSts             (_TEXT("BRF_MAT_CommSts"		));
                                        
	CDIO        BRF_StandBy_RD		        (_TEXT("BRF_StandBy_RD"			));
	CAIO        BRF_Setpoint_RA	            (_TEXT("BRF_Setpoint_RA"		));
	CAIO        BRF_FwdPwr_RA  	            (_TEXT("BRF_FwdPwr_RA"			));
	CAIO        BRF_RefPwr_RA  	            (_TEXT("BRF_RefPwr_RA"			));
	CAIO        BRF_Vdc_RA     	            (_TEXT("BRF_Vdc_RA"				));
	CAIO        BRF_Vpp_RA     	            (_TEXT("BRF_Vpp_RA"				));
	CAIO        BRFM_Match_Pos	            (_TEXT("BRFM_Match_Pos"			));
	CAIO        BRFM_Tune_Pos	            (_TEXT("BRFM_Tune_Pos"			));
                                        
                                        
	 //Real Input HRF                                   
	CDIO        eDI_SRFG_CommSts	        (_TEXT("eDI_SRFG_CommSts"		));
	CDIO        eDI_SRFM_CommSts			(_TEXT("eDI_SRFM_CommSts"		));
                                        
	CDIO        eDI_SRF_GenPowerSts_ON      (_TEXT("eDI_SRF_GenPowerSts_ON"	));
	CDIO        eDI_SRF_GenPowerSts_OFF     (_TEXT("eDI_SRF_GenPowerSts_OFF"));
	CDIO        eDI_SRF_GenFailSts	        (_TEXT("eDI_SRF_GenFailSts"		));
	CDIO        eDI_SRF_GenRemoteMode	    (_TEXT("eDI_SRF_GenRemoteMode"	));
	CDIO        eDI_SRF_MAT_Fail	        (_TEXT("eDI_SRF_MAT_Fail"		));
	CDIO        eDI_SRF_MAT_Mode	        (_TEXT("eDI_SRF_MAT_Mode"		));
                              
	CAIO        eAI_SRF_FwdPwr		        (_TEXT("eAI_SRF_FwdPwr"			));
	CAIO        eAI_SRF_RefPwr		        (_TEXT("eAI_SRF_RefPwr"			));
	CAIO        eAI_SRF_MAT_Vdc		        (_TEXT("eAI_SRF_MAT_Vdc"		));
	CAIO        eAI_SRF_MAT_Vpp		        (_TEXT("eAI_SRF_MAT_Vpp"		));
	CAIO        eAI_SRF_MAT_MatchPos	    (_TEXT("eAI_SRF_MAT_MatchPos"	));
	CAIO        eAI_SRF_MAT_TunePos	        (_TEXT("eAI_SRF_MAT_TunePos"	));                                      
                                        
	 //Real Input LRF                                    
	CDIO        eDI_BRFG_CommSts	        (_TEXT("eDI_BRFG_CommSts"		));
	CDIO        eDI_BRFM_CommSts		    (_TEXT("eDI_BRFM_CommSts"		));
                                        
	CDIO        eDI_BRF_GenPowerSts_ON      (_TEXT("eDI_BRF_GenPowerSts_ON"	));
	CDIO        eDI_BRF_GenPowerSts_OFF     (_TEXT("eDI_BRF_GenPowerSts_OFF"));
	CDIO        eDI_BRF_GenFailSts	        (_TEXT("eDI_BRF_GenFailSts"		));
	CDIO        eDI_BRF_GenRemoteMode	    (_TEXT("eDI_BRF_GenRemoteMode"	));
	CDIO        eDI_BRF_MAT_Fail	        (_TEXT("eDI_BRF_MAT_Fail"		));
	CDIO        eDI_BRF_MAT_Mode	        (_TEXT("eDI_BRF_MAT_Mode"		));
	
	CAIO        eAI_BRF_FwdPwr		        (_TEXT("eAI_BRF_FwdPwr"			));
	CAIO        eAI_BRF_RefPwr		        (_TEXT("eAI_BRF_RefPwr"			));
	CAIO        eAI_BRF_MAT_Vdc		        (_TEXT("eAI_BRF_MAT_Vdc"		));
	CAIO        eAI_BRF_MAT_Vpp		        (_TEXT("eAI_BRF_MAT_Vpp"		));
	CAIO        eAI_BRF_MAT_MatchPos	    (_TEXT("eAI_BRF_MAT_MatchPos"	));
	CAIO        eAI_BRF_MAT_TunePos	        (_TEXT("eAI_BRF_MAT_TunePos"	));                                      
                                        
	//Real Output RF                                      
	CAIO        eAO_SRF_FwdPwr		        (_TEXT("eAO_SRF_FwdPwr"			));
	CAIO        eAO_SRF_MAT_MatchPos	    (_TEXT("eAO_SRF_MAT_MatchPos"	));
	CAIO        eAO_SRF_MAT_TunePos	        (_TEXT("eAO_SRF_MAT_TunePos"	));
                                        
	CAIO        eAO_BRF_FwdPwr		        (_TEXT("eAO_BRF_FwdPwr"			));
	CAIO        eAO_BRF_MAT_MatchPos	    (_TEXT("eAO_BRF_MAT_MatchPos"	));
	CAIO        eAO_BRF_MAT_TunePos	        (_TEXT("eAO_BRF_MAT_TunePos"	));
                                        
 		// Virtual Output RF                                       
	CAIO        eSRF_Setpoint_SA	        (_TEXT("eSRF_Setpoint_SA"		));
	CAIO        eSRFM_Match_SetPos	        (_TEXT("eSRFM_Match_SetPos"		));
	CAIO        eSRFM_Tune_SetPos	        (_TEXT("eSRFM_Tune_SetPos"		));
	CAIO        eSRF_Setpoint_RA	        (_TEXT("eSRF_Setpoint_RA"		));
                                        
	CAIO        eBRF_Setpoint_SA	        (_TEXT("eBRF_Setpoint_SA"		));
	CAIO        eBRFM_Match_SetPos	        (_TEXT("eBRFM_Match_SetPos"		));
	CAIO        eBRFM_Tune_SetPos	        (_TEXT("eBRFM_Tune_SetPos"		));
	CAIO        eBRF_Setpoint_RA	        (_TEXT("eBRF_Setpoint_RA"		));
                                        
	CDIO        eSRF_PwrSts_RD              (_TEXT("eSRF_PwrSts_RD"			));
	CDIO        eBRF_PwrSts_RD              (_TEXT("eBRF_PwrSts_RD"			));
                                        
END_OBJECT_ENUMERATION
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
	// System IO
	CSIO System_Date("System.Date");
	CSIO System_Time("System.Time");

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------

void UPDATE_DIGITAL(CDIO &src_ch, CDIO &dst_ch, BOOL bOption)
{
	int ncs		= 0;
	int nData	= 0;
	
	nData = src_ch.Read(ncs);
	dst_ch.Write(nData , ncs);
}

void UPDATE_ANALOG(CAIO &src_ch, CAIO &dst_ch, BOOL bOption)
{
	int ncs				= 0;
	double dData		= 0;
	
	dData = src_ch.Read(ncs);
	dst_ch.Write(dData , ncs);
	
}

void UPDATE_ANALOG2(double src_ch, CAIO &dst_ch, BOOL bOption)
{

	int ncs				= 0;
	double dData		= 0;
	
	if(src_ch == 99)		dData = 0;

	dst_ch.Write(dData , ncs);
}



void RF_Value_Monitoring(void *pDummy)
{
	
	int ncs;	
	BOOL bLOG_STS;
	double SRF,BRF;
	int PROC_STS;
	
	if(WaitKill(20*1000)) { _endthread(); return; }

	while(1)
	{
		if(CheckKill()) { break; }
		bLOG_STS = FALSE;
		
		SRF		 =	PR_SRFG_POWER.Read(ncs);
		BRF		 =	PR_BRFG_POWER.Read(ncs);
		PROC_STS =  PM_PROC_STS.Read(ncs);
		
  		if(SRF <100   && PROC_STS == 1)
 		{
  			UPDATE_ANALOG2 (99,						SRF_FwdPwr_RA,       	bLOG_STS);
  			UPDATE_ANALOG2 (99,						SRF_RefPwr_RA,       	bLOG_STS);
  			UPDATE_ANALOG2 (99,          			SRF_Vpp_RA,          	bLOG_STS);
  			UPDATE_ANALOG2 (99,          			SRF_Vdc_RA,          	bLOG_STS);
  		}
 		else
 		{
			UPDATE_ANALOG (eAI_SRF_FwdPwr,       	SRF_FwdPwr_RA,       	bLOG_STS);
			UPDATE_ANALOG (eAI_SRF_RefPwr,       	SRF_RefPwr_RA,       	bLOG_STS);
			UPDATE_ANALOG (eAI_SRF_MAT_Vpp,       	SRF_Vpp_RA,          	bLOG_STS);
			UPDATE_ANALOG (eAI_SRF_MAT_Vdc,        	SRF_Vdc_RA,          	bLOG_STS);
		}
		
		if(CheckKill()) { break; }
		if(BRF <100  && PROC_STS == 1)
		{
			UPDATE_ANALOG2 (99,						BRF_FwdPwr_RA,       	bLOG_STS);
			UPDATE_ANALOG2 (99,						BRF_RefPwr_RA,       	bLOG_STS);
			UPDATE_ANALOG2 (99,          			BRF_Vpp_RA,          	bLOG_STS);
			UPDATE_ANALOG2 (99,          			BRF_Vdc_RA,          	bLOG_STS);
		}
		else
		{
			UPDATE_ANALOG (eAI_BRF_FwdPwr,       	BRF_FwdPwr_RA,       	bLOG_STS);
			UPDATE_ANALOG (eAI_BRF_RefPwr,       	BRF_RefPwr_RA,       	bLOG_STS);
			UPDATE_ANALOG (eAI_BRF_MAT_Vpp,       	BRF_Vpp_RA,          	bLOG_STS);
			UPDATE_ANALOG (eAI_BRF_MAT_Vdc,        	BRF_Vdc_RA,          	bLOG_STS);
		}

		UPDATE_DIGITAL(eDI_SRF_GenPowerSts_ON,		eSRF_PwrSts_RD,			bLOG_STS);
		UPDATE_DIGITAL(eDI_BRF_GenPowerSts_ON,		eBRF_PwrSts_RD,			bLOG_STS);
			
		if( WaitKill(100) ) break;
	}
	
	_endthread();
}
void RF_Channel_Monitoring(void *pDummy)
{
	int Red = 0;
	int AlmSts = 0;
	int ncs = 0;
	
	int nData = 0;
	double dData = 0.0;
	int i = 0;
	char szBuf[128] = {0};
	int nCount = 0;
	
	char szData[256] = {0};
	
	BOOL bLOG_STS = FALSE;
	
	
	int  nRFGErrorSts = 0;
	int  nRFMErrorSts = 0;

	int  SRFG_Comm=0, SRFM_Comm=0, BRFG_Comm=0, BRFM_Comm=0; 
	
	if(WaitKill(20*1000)) { _endthread(); return; }
	
	while(1)
	{

		if(CheckKill()) { break; }

		nRFGErrorSts    =	eDI_SRF_GenFailSts.Read(ncs);
		nRFMErrorSts	 =	eDI_SRF_MAT_Fail.Read(ncs);
		SRFG_Comm	    =	eDI_SRFG_CommSts.Read(ncs);
		SRFM_Comm	    =	eDI_SRFM_CommSts.Read(ncs);
				
		if(nRFGErrorSts == 0 && nRFMErrorSts == 0  && SRFG_Comm == 1 && SRFM_Comm == 1)		SRF_StandBy_RD.Write(ON , ncs);
		else																				SRF_StandBy_RD.Write(OFF , ncs);
		
		if(CheckKill()) { break; }

		nRFGErrorSts    =	eDI_BRF_GenFailSts.Read(ncs);
		nRFMErrorSts	 =	eDI_BRF_MAT_Fail.Read(ncs);
		BRFG_Comm	    =	eDI_BRFG_CommSts.Read(ncs);
		BRFM_Comm	    =	eDI_BRFM_CommSts.Read(ncs);
		
		if(nRFGErrorSts == 0 && nRFMErrorSts == 0  && BRFG_Comm == 1 && BRFM_Comm == 1)		BRF_StandBy_RD.Write(ON , ncs);
		else																				BRF_StandBy_RD.Write(OFF , ncs);	

		if(CheckKill()) { break; }
		UPDATE_DIGITAL(eDI_SRFG_CommSts,		SRF_CommSts,          	bLOG_STS);
		UPDATE_DIGITAL(eDI_BRFG_CommSts,		BRF_CommSts,          	bLOG_STS);
		UPDATE_DIGITAL(eDI_SRFM_CommSts,		SRF_MAT_CommSts,       	bLOG_STS);
		UPDATE_DIGITAL(eDI_BRFM_CommSts,		BRF_MAT_CommSts,       	bLOG_STS);

		if(CheckKill()) { break; }
		UPDATE_ANALOG(eAI_SRF_FwdPwr,			eSRF_Setpoint_RA,		bLOG_STS);
		UPDATE_ANALOG(eAO_SRF_FwdPwr,			eSRF_Setpoint_SA,		bLOG_STS);
		UPDATE_ANALOG(eAO_SRF_MAT_MatchPos,		eSRFM_Match_SetPos,		bLOG_STS);
		UPDATE_ANALOG(eAO_SRF_MAT_TunePos,		eSRFM_Tune_SetPos,		bLOG_STS);
		UPDATE_ANALOG(eAI_SRF_MAT_MatchPos,    	SRFM_Match_Pos,     	bLOG_STS);
		UPDATE_ANALOG(eAI_SRF_MAT_TunePos,     	SRFM_Tune_Pos,			bLOG_STS);		
	
		if(CheckKill()) { break; }
		UPDATE_ANALOG(eAI_BRF_FwdPwr,			eBRF_Setpoint_RA,		bLOG_STS);
		UPDATE_ANALOG(eAO_BRF_FwdPwr,			eBRF_Setpoint_SA,		bLOG_STS);
		UPDATE_ANALOG(eAO_BRF_MAT_MatchPos,		eBRFM_Match_SetPos,		bLOG_STS);
		UPDATE_ANALOG(eAO_BRF_MAT_TunePos,		eBRFM_Tune_SetPos,		bLOG_STS);
		UPDATE_ANALOG(eAI_BRF_MAT_MatchPos,    	BRFM_Match_Pos,     	bLOG_STS);
		UPDATE_ANALOG(eAI_BRF_MAT_TunePos,     	BRFM_Tune_Pos,			bLOG_STS);		
	
		if( WaitKill(100) ) break;
	}
	
	_endthread();
}


//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
SEQ_STATUS SeqMain(const char* szCommand, const void* pParam, int nNumOfBytesRecved) 
{
	printf("-----------> [ %s ] START... [%s]\n", GetSeqName(), szCommand);
	
	
	int ncs = 0;
	
	BOOL bLOG_STS = FALSE;

	_beginthread(RF_Value_Monitoring,		0, NULL); 	_sleep(100);
	_beginthread(RF_Channel_Monitoring,		0, NULL);	_sleep(100);	
    
	while(1) 
	{
		if( WaitKill(1000) ) break;
	}
	_sleep(2000);	
	
	return SEQ_SUCCESS;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------
void OnLoadComplete( const TCHAR* szArgument )
{


}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL OnInitComplete( const TCHAR* szArgument )
{
	int CommStatus;
	if ( SIMULATION_CONTROL.Read(CommStatus) == ON ) SIMULATION_MODE = TRUE;
	else											 SIMULATION_MODE = FALSE;
	
	return TRUE;
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnUnLoad( int nCode ) 
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnEventRecv( const char* szCommand, const void* pParam , int nNumOfBytesRecved ) {
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
