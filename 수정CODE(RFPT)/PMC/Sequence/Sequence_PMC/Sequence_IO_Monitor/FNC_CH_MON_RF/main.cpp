#include <windows.h>						
#include <stdio.h>							

#include <tMain.h>							
#include <TUtilStr.h>						
#include <TTimer.h>							
#include <TString.h>						

#include "h/Define.h"						
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
enum { OFF, ON };							

//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
//------------------------------------------------------------------------------
BOOL SIMULATION_MODE = FALSE;				
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
BEGIN_OBJECT_ENUMERATION

	CDIO SIMULATION_CONTROL("System.SimulMode");								

	CDIO        PM_Start_Flag               (_TEXT("PM_Start_Flag"          ));	

	CAIO        PR_SRFG_POWER               (_TEXT("PR_SRFG_POWER"          ));	
	CAIO        PR_BRFG_POWER               (_TEXT("PR_BRFG_POWER"          ));	
	CDIO        PM_PROC_STS                 (_TEXT("PM_PROC_STS"            ));	

	// Virtual Input SRF
	CDIO        SRF_CommSts                 (_TEXT("SRF_CommSts"            ));	// Source RF Generator	통신 상태를 표시하는 가상 채널
	CDIO        SRF_MAT_CommSts             (_TEXT("SRF_MAT_CommSts"        ));	// Source RF Matcher	통신 상태를 표시하는 가상 채널

	CDIO        SRF_StandBy_RD              (_TEXT("SRF_StandBy_RD"         ));	// Source RF StandBy	가능 여부를 표시하는 가상 채널
	CAIO        SRF_Setpoint_RA             (_TEXT("SRF_Setpoint_RA"        ));	// Source RF Setpoint readback 값을 표시하는 기존 가상 채널
	CAIO        SRF_FwdPwr_RA               (_TEXT("SRF_FwdPwr_RA"          ));	// Source RF Forward Power readback		가상 채널
	CAIO        SRF_RefPwr_RA               (_TEXT("SRF_RefPwr_RA"          ));	// Source RF Reflected Power readback	가상 채널
	CAIO        SRF_DeliveryPwr_RA          (_TEXT("SRF_DeliveryPwr_RA"     ));	// RFPT Generator Delivery Power readback 가상 채널
	CAIO        SRF_Vdc_RA                  (_TEXT("SRF_Vdc_RA"             ));	// Source Matcher VDC readback 가상 채널
	//CAIO        SRF_Vpp_RA                  (_TEXT("SRF_Vpp_RA"             ));	// 교산 화면 호환용 Vpp 가상 채널. RFPT에는 실제 값이 없으므로 0으로 유지
	CAIO        SRFM_Match_Pos              (_TEXT("SRFM_Match_Pos"         ));	// Source Matcher 실제 Match Position	가상 채널
	CAIO        SRFM_Tune_Pos               (_TEXT("SRFM_Tune_Pos"          ));	// Source Matcher 실제 Tune Position	가상 채널

	// Virtual Input BRF
	CDIO        BRF_CommSts                 (_TEXT("BRF_CommSts"            ));	// Bias RF Generator	통신 상태를 표시하는 가상 채널
	CDIO        BRF_MAT_CommSts             (_TEXT("BRF_MAT_CommSts"        ));	// Bias RF Matcher		통신 상태를 표시하는 가상 채널

	CDIO        BRF_StandBy_RD              (_TEXT("BRF_StandBy_RD"         ));	// Bias RF StandBy 가능 여부를 표시하는 가상 채널
	CAIO        BRF_Setpoint_RA             (_TEXT("BRF_Setpoint_RA"        ));	// Bias RF Setpoint readback 값을 표시하는 기존 가상 채널
	CAIO        BRF_FwdPwr_RA               (_TEXT("BRF_FwdPwr_RA"          ));	// Bias RF Forward Power readback 가상 채널
	CAIO        BRF_RefPwr_RA               (_TEXT("BRF_RefPwr_RA"          ));	// Bias RF Reflected Power readback 가상 채널
	CAIO        BRF_DeliveryPwr_RA          (_TEXT("BRF_DeliveryPwr_RA"     ));	// RFPT Generator Delivery Power readback 가상 채널
	CAIO        BRF_Vdc_RA                  (_TEXT("BRF_Vdc_RA"             ));	// Bias Matcher VDC readback 가상 채널
	//CAIO        BRF_Vpp_RA                  (_TEXT("BRF_Vpp_RA"             ));	// 교산 화면 호환용 Vpp 가상 채널 RFPT에는 실제 값이 없으므로 0으로 유지
	CAIO        BRFM_Match_Pos              (_TEXT("BRFM_Match_Pos"         ));	// Bias Matcher 실제 Match Position 가상 채널
	CAIO        BRFM_Tune_Pos               (_TEXT("BRFM_Tune_Pos"          ));	// Bias Matcher 실제 Tune Position 가상 채널


	// Real Input SRF - RFPT Generator/Matcher
	CDIO        eDI_SRFG_CommSts            (_TEXT("eDI_SRFG_CommSts"       ));	// RFPT Source Generator 실제 통신 상태 채널
	CDIO        eDI_SRFM_CommSts            (_TEXT("eDI_SRFM_CommSts"       ));	// RFPT Source Matcher 실제 통신 상태 채널

	CDIO        eDI_SRF_GenRFOn             (_TEXT("eDI_SRF_GenRFOn"        ));	// RFPT Generator Input byte28 bit5: 실제 RF ON 상태 채널
	CDIO        eDI_SRF_GenAlarm            (_TEXT("eDI_SRF_GenAlarm"       ));	// RFPT Generator Input byte28 bit2: 종합 Alarm 채널
	CDIO        eDI_SRF_GenRemoteMode       (_TEXT("eDI_SRF_GenRemoteMode"  ));	// RFPT Generator Input byte28 bit6: Remote In 채널
	CDIO        eDI_SRF_MAT_Alarm           (_TEXT("eDI_SRF_MAT_Alarm"      ));	// RFPT Matcher Input byte11 bit7: 종합 Alarm 채널
	CDIO        eDI_SRF_MAT_MatcherMode     (_TEXT("eDI_SRF_MAT_MatcherMode"));	// RFPT Matcher Input byte10 bit0~3: Matcher Mode 채널

	CAIO        eAI_SRF_FwdPwr_Set          (_TEXT("eAI_SRF_FwdPwr_Set"     ));	// RFPT Generator Input byte0~1: Set Point Power readback 채널
	CAIO        eAI_SRF_FwdPwr              (_TEXT("eAI_SRF_FwdPwr"         ));	// RFPT Generator Input byte4~5: Forward Power 채널
	CAIO        eAI_SRF_RefPwr              (_TEXT("eAI_SRF_RefPwr"         ));	// RFPT Generator Input byte6~7: Reflected Power 채널
	CAIO        eAI_SRF_DeliveryPwr         (_TEXT("eAI_SRF_DeliveryPwr"    ));	// RFPT Generator Input byte8~9: Delivery Power 채널
	CAIO        eAI_SRF_MAT_Vdc             (_TEXT("eAI_SRF_MAT_Vdc"        ));	// RFPT Matcher Input byte8~9: VDC 채널
	CAIO        eAI_SRF_MAT_MatchPos        (_TEXT("eAI_SRF_MAT_MatchPos"   ));	// RFPT Matcher Input byte0~1: Actual Load Position 채널
	CAIO        eAI_SRF_MAT_TunePos         (_TEXT("eAI_SRF_MAT_TunePos"    ));	// RFPT Matcher Input byte2~3: Actual Tune Position 채널


	// Real Input BRF - RFPT Generator/Matcher
	CDIO        eDI_BRFG_CommSts            (_TEXT("eDI_BRFG_CommSts"       ));	// RFPT Bias Generator 실제 통신 상태 채널
	CDIO        eDI_BRFM_CommSts            (_TEXT("eDI_BRFM_CommSts"       ));	// RFPT Bias Matcher 실제 통신 상태 채널

	CDIO        eDI_BRF_GenRFOn             (_TEXT("eDI_BRF_GenRFOn"        ));	// RFPT Generator Input byte28 bit5: 실제 RF ON 상태 채널
	CDIO        eDI_BRF_GenAlarm            (_TEXT("eDI_BRF_GenAlarm"       ));	// RFPT Generator Input byte28 bit2: 종합 Alarm 채널
	CDIO        eDI_BRF_GenRemoteMode       (_TEXT("eDI_BRF_GenRemoteMode"  ));	// RFPT Generator Input byte28 bit6: Remote In 채널
	CDIO        eDI_BRF_MAT_Alarm           (_TEXT("eDI_BRF_MAT_Alarm"      ));	// RFPT Matcher Input byte11 bit7: 종합 Alarm 채널
	CDIO        eDI_BRF_MAT_MatcherMode     (_TEXT("eDI_BRF_MAT_MatcherMode"));	// RFPT Matcher Input byte10 bit0~3: Matcher Mode 채널

	CAIO        eAI_BRF_FwdPwr_Set          (_TEXT("eAI_BRF_FwdPwr_Set"     ));	// RFPT Generator Input byte0~1: Set Point Power readback 채널
	CAIO        eAI_BRF_FwdPwr              (_TEXT("eAI_BRF_FwdPwr"         ));	// RFPT Generator Input byte4~5: Forward Power 채널
	CAIO        eAI_BRF_RefPwr              (_TEXT("eAI_BRF_RefPwr"         ));	// RFPT Generator Input byte6~7: Reflected Power 채널
	CAIO        eAI_BRF_DeliveryPwr         (_TEXT("eAI_BRF_DeliveryPwr"    ));	// RFPT Generator Input byte8~9: Delivery Power 채널
	CAIO        eAI_BRF_MAT_Vdc             (_TEXT("eAI_BRF_MAT_Vdc"        ));	// RFPT Matcher Input byte8~9: VDC 채널
	CAIO        eAI_BRF_MAT_MatchPos        (_TEXT("eAI_BRF_MAT_MatchPos"   ));	// RFPT Matcher Input byte0~1: Actual Load Position 채널
	CAIO        eAI_BRF_MAT_TunePos         (_TEXT("eAI_BRF_MAT_TunePos"    ));	// RFPT Matcher Input byte2~3: Actual Tune Position 채널


	// Real Output RF
	CAIO        eAO_SRF_FwdPwr              (_TEXT("eAO_SRF_FwdPwr"         ));	// Source RF 실제 출력 Setpoint 채널
	CAIO        eAO_SRF_MAT_MatchPos        (_TEXT("eAO_SRF_MAT_MatchPos"   ));	// Source Matcher Match Position command 채널
	CAIO        eAO_SRF_MAT_TunePos         (_TEXT("eAO_SRF_MAT_TunePos"    ));	// Source Matcher Tune Position command 채널

	CAIO        eAO_BRF_FwdPwr              (_TEXT("eAO_BRF_FwdPwr"         ));	// Bias RF 실제 출력 Setpoint 채널
	CAIO        eAO_BRF_MAT_MatchPos        (_TEXT("eAO_BRF_MAT_MatchPos"   ));	// Bias Matcher Match Position command 채널
	CAIO        eAO_BRF_MAT_TunePos         (_TEXT("eAO_BRF_MAT_TunePos"    ));	// Bias Matcher Tune Position command 채널

	// Virtual Output RF
	CAIO        eSRF_Setpoint_SA            (_TEXT("eSRF_Setpoint_SA"       ));	// Source RF Setpoint command 표시용 가상 채널
	CAIO        eSRFM_Match_SetPos          (_TEXT("eSRFM_Match_SetPos"     ));	// Source Matcher Match command 표시용 가상 채널
	CAIO        eSRFM_Tune_SetPos           (_TEXT("eSRFM_Tune_SetPos"      ));	// Source Matcher Tune command 표시용 가상 채널
	CAIO        eSRF_Setpoint_RA            (_TEXT("eSRF_Setpoint_RA"       ));	// Source RF Setpoint readback 표시용 e-prefix 가상 채널

	CAIO        eBRF_Setpoint_SA            (_TEXT("eBRF_Setpoint_SA"       ));	// Bias RF Setpoint command 표시용 가상 채널
	CAIO        eBRFM_Match_SetPos          (_TEXT("eBRFM_Match_SetPos"     ));	// Bias Matcher Match command 표시용 가상 채널
	CAIO        eBRFM_Tune_SetPos           (_TEXT("eBRFM_Tune_SetPos"      ));	// Bias Matcher Tune command 표시용 가상 채널
	CAIO        eBRF_Setpoint_RA            (_TEXT("eBRF_Setpoint_RA"       ));	// Bias RF Setpoint readback 표시용 e-prefix 가상 채널

	CDIO        eSRF_PwrSts_RD              (_TEXT("eSRF_PwrSts_RD"         ));	// Source RF 실제 ON 상태를 표시하는 가상 디지털 채널
	CDIO        eBRF_PwrSts_RD              (_TEXT("eBRF_PwrSts_RD"         ));	// Bias RF 실제 ON 상태를 표시하는 가상 디지털 채널

END_OBJECT_ENUMERATION
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
CSIO System_Date("System.Date");				// 시스템 날짜 채널
CSIO System_Time("System.Time");				// 시스템 시간 채널

//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------

// [UPDATE_DIGITAL]: 실제 디지털 채널 값을 가상 디지털 채널로 그대로 복사하는 함수다.
// 매개변수: src_ch = 원본 실제 채널, dst_ch = 복사 대상 가상 채널, bOption = 기존 형식 유지용 옵션
// 반환값: 없음
void UPDATE_DIGITAL(CDIO &src_ch, CDIO &dst_ch, BOOL bOption)
{
	int ncs = 0;								
	int nData = 0;								

	nData = src_ch.Read(ncs);					
	dst_ch.Write(nData, ncs);					
}

// [UPDATE_ANALOG]: 실제 아날로그 채널 값을 가상 아날로그 채널로 그대로 복사하는 함수다.
// 매개변수: src_ch = 원본 실제 채널, dst_ch = 복사 대상 가상 채널, bOption = 기존 형식 유지용 옵션
// 반환값: 없음
void UPDATE_ANALOG(CAIO &src_ch, CAIO &dst_ch, BOOL bOption)
{
	int    ncs = 0;								
	double dData = 0.0;							

	dData = src_ch.Read(ncs);					
	dst_ch.Write(dData, ncs);					
}

// [UPDATE_ANALOG2]: 실제 채널이 아닌 직접 지정한 값을 가상 아날로그 채널에 쓰는 보조 함수다.
// 매개변수: src_ch = 직접 기록할 값(기존 스타일에서 99는 0으로 강제 처리), dst_ch = 목적지 채널, bOption = 기존 형식 유지용 옵션
// 반환값: 없음
void UPDATE_ANALOG2(double src_ch, CAIO &dst_ch, BOOL bOption)
{
	int    ncs = 0;								
	double dData = 0.0;							

	if(src_ch == 99)							
		dData = 0.0;							
	else
		dData = src_ch;							

	dst_ch.Write(dData, ncs);					
}


//----------------------------------------------------------------------------------------------------------------------------------------------
//  Thread : RF_Value_Monitoring
//  Description : RF 전력 관련 실측값을 가상 모니터 채널로 복사하는 thread다.
//              : RFPT 기준으로 실제 RF OFF 상태일 때만 0으로 표시한다.
//              : 기존 교산의 "<100W 이면 0" 규칙은 RFPT 사양과 맞지 않으므로 제거했다.
//              : RFPT 변경사항:
//                1. Delivery Power 표시 추가
//                2. Matcher Vpp는 사양서에 없으므로 항상 0
//                3. RF ON/OFF 판정은 eDI_*_GenRFOn 사용
//----------------------------------------------------------------------------------------------------------------------------------------------
void RF_Value_Monitoring(void *pDummy)
{
	int    ncs = 0;								// 채널 접근용 CS 값을 반드시 0으로 초기화한다.
	BOOL   bLOG_STS = FALSE;					// 기존 함수 형식 유지를 위한 로그 옵션 변수다.
	double SRF = 0.0;							// Source RF 목표 Power를 읽어 둘 변수다.
	double BRF = 0.0;							// Bias RF 목표 Power를 읽어 둘 변수다.
	int    PROC_STS = 0;						// 현재 공정 상태를 저장할 변수다.
	int    nSRF_RFOn = OFF;						// Source RF 실제 ON 상태를 저장할 변수다.
	int    nBRF_RFOn = OFF;						// Bias RF 실제 ON 상태를 저장할 변수다.
	
	if(WaitKill(20*1000)) { _endthread(); return; }	// 시작 직후 Kill 신호가 오면 바로 thread를 종료한다.

	while(1)									// 모니터링 thread를 계속 반복 실행한다.
	{
		if(CheckKill()) { break; }				// 외부 종료 신호가 오면 loop를 빠져나간다.
		bLOG_STS = FALSE;						// 현재 코드에서는 별도 로그 옵션을 사용하지 않으므로 FALSE로 유지한다.
		
		SRF = PR_SRFG_POWER.Read(ncs);			// Source RF 목표 전력 값을 읽는다.
		BRF = PR_BRFG_POWER.Read(ncs);			// Bias RF 목표 전력 값을 읽는다.
		PROC_STS = PM_PROC_STS.Read(ncs);		// 현재 공정 상태를 읽는다.

		nSRF_RFOn = eDI_SRF_GenRFOn.Read(ncs);	// RFPT Source Generator 실제 RF ON 상태(bit28 bit5)를 읽는다.
		nBRF_RFOn = eDI_BRF_GenRFOn.Read(ncs);	// RFPT Bias Generator 실제 RF ON 상태(bit28 bit5)를 읽는다.

		// RFPT에서는 100W 미만도 정상 출력 범위이므로, 교산의 "<100W 이면 0" 규칙을 쓰면 안 된다.
		// 따라서 공정 중이고 실제 RF ON bit가 OFF일 때만 0으로 정리한다.
		if(nSRF_RFOn == OFF && PROC_STS == 1)
		{
			UPDATE_ANALOG2(99, SRF_FwdPwr_RA,      bLOG_STS);		// Source Forward Power 표시를 0으로 만든다.
			UPDATE_ANALOG2(99, SRF_RefPwr_RA,      bLOG_STS);		// Source Reflected Power 표시를 0으로 만든다.
			UPDATE_ANALOG2(99, SRF_DeliveryPwr_RA, bLOG_STS);		// Source Delivery Power 표시를 0으로 만든다.
			UPDATE_ANALOG2(99, SRF_Vdc_RA,         bLOG_STS);		// Source VDC 표시를 0으로 만든다.
			//UPDATE_ANALOG2(99, SRF_Vpp_RA,         bLOG_STS);		// RFPT에는 Vpp가 없으므로 0으로 유지한다.
		}
		else
		{
			UPDATE_ANALOG(eAI_SRF_FwdPwr,      SRF_FwdPwr_RA,      bLOG_STS);	// Source 실제 Forward Power를 가상 채널로 복사한다.
			UPDATE_ANALOG(eAI_SRF_RefPwr,      SRF_RefPwr_RA,      bLOG_STS);	// Source 실제 Reflected Power를 가상 채널로 복사한다.
			UPDATE_ANALOG(eAI_SRF_DeliveryPwr, SRF_DeliveryPwr_RA, bLOG_STS);	// Source 실제 Delivery Power를 가상 채널로 복사한다.
			UPDATE_ANALOG(eAI_SRF_MAT_Vdc,     SRF_Vdc_RA,         bLOG_STS);	// Source Matcher VDC를 가상 채널로 복사한다.
			//UPDATE_ANALOG2(99,                 SRF_Vpp_RA,         bLOG_STS);	// RFPT에는 Vpp가 없으므로 0으로 유지한다.
		}
		
		if(CheckKill()) { break; }												// 중간에도 종료 신호를 한 번 더 확인한다.

		// Bias도 Source와 같은 기준으로 처리한다.
		if(nBRF_RFOn == OFF && PROC_STS == 1)
		{
			UPDATE_ANALOG2(99, BRF_FwdPwr_RA,      bLOG_STS);		// Bias Forward Power 표시를 0으로 만든다.
			UPDATE_ANALOG2(99, BRF_RefPwr_RA,      bLOG_STS);		// Bias Reflected Power 표시를 0으로 만든다.
			UPDATE_ANALOG2(99, BRF_DeliveryPwr_RA, bLOG_STS);		// Bias Delivery Power 표시를 0으로 만든다.
			UPDATE_ANALOG2(99, BRF_Vdc_RA,         bLOG_STS);		// Bias VDC 표시를 0으로 만든다.
			//UPDATE_ANALOG2(99, BRF_Vpp_RA,         bLOG_STS);		// RFPT에는 Vpp가 없으므로 0으로 유지한다.
		}
		else
		{
			UPDATE_ANALOG(eAI_BRF_FwdPwr,      BRF_FwdPwr_RA,      bLOG_STS);	// Bias 실제 Forward Power를 가상 채널로 복사한다.
			UPDATE_ANALOG(eAI_BRF_RefPwr,      BRF_RefPwr_RA,      bLOG_STS);	// Bias 실제 Reflected Power를 가상 채널로 복사한다.
			UPDATE_ANALOG(eAI_BRF_DeliveryPwr, BRF_DeliveryPwr_RA, bLOG_STS);	// Bias 실제 Delivery Power를 가상 채널로 복사한다.
			UPDATE_ANALOG(eAI_BRF_MAT_Vdc,     BRF_Vdc_RA,         bLOG_STS);	// Bias Matcher VDC를 가상 채널로 복사한다.
			//UPDATE_ANALOG2(99,                 BRF_Vpp_RA,         bLOG_STS);	// RFPT에는 Vpp가 없으므로 0으로 유지한다.
		}

		UPDATE_DIGITAL(eDI_SRF_GenRFOn, eSRF_PwrSts_RD, bLOG_STS);	// Source 실제 RF ON bit를 가상 Power Status 채널에 복사한다.
		UPDATE_DIGITAL(eDI_BRF_GenRFOn, eBRF_PwrSts_RD, bLOG_STS);	// Bias 실제 RF ON bit를 가상 Power Status 채널에 복사한다.
			
		if(WaitKill(100)) break;										// 100ms 주기로 반복하면서 종료 신호를 감시한다.
	}
	
	_endthread();														// thread를 정상 종료한다.
}

//----------------------------------------------------------------------------------------------------------------------------------------------
//  Thread : RF_Channel_Monitoring
//  Description : RF 통신상태, StandBy 상태, Setpoint, Matcher 위치를 가상 채널로 중계하는 thread다.
//              : RFPT 변경사항:
//                1. StandBy 조건의 Fail 채널을 RFPT Alarm 채널로 교체
//                2. Setpoint readback은 eAI_*_FwdPwr_Set 사용
//                3. plain 채널과 e-prefix 채널을 모두 갱신
//----------------------------------------------------------------------------------------------------------------------------------------------
void RF_Channel_Monitoring(void *pDummy)
{
	int   ncs = 0;							
	BOOL  bLOG_STS = FALSE;					

	int   nRFGErrorSts = 0;					
	int   nRFMErrorSts = 0;					

	int   SRFG_Comm = 0;					
	int   SRFM_Comm = 0;					
	int   BRFG_Comm = 0;					
	int   BRFM_Comm = 0;					

	if(WaitKill(20*1000)) { _endthread(); return; }	// 시작 직후 Kill 신호가 오면 바로 thread를 종료한다.

	while(1)									
	{
		if(CheckKill()) { break; }				

		// Source RF StandBy 판정
		// RFPT 기준으로 Generator Alarm 없음 + Matcher Alarm 없음 + Generator Comm OK + Matcher Comm OK 일 때만 ON
		nRFGErrorSts = eDI_SRF_GenAlarm.Read(ncs);		
		nRFMErrorSts = eDI_SRF_MAT_Alarm.Read(ncs);		
		SRFG_Comm = eDI_SRFG_CommSts.Read(ncs);			
		SRFM_Comm = eDI_SRFM_CommSts.Read(ncs);			

		if(nRFGErrorSts == 0 && nRFMErrorSts == 0 && SRFG_Comm == 1 && SRFM_Comm == 1)
			SRF_StandBy_RD.Write(ON, ncs);				// 네 조건이 모두 만족되면 Source StandBy Ready를 ON으로 만든다.
		else
			SRF_StandBy_RD.Write(OFF, ncs);				// 하나라도 틀리면 Source StandBy Ready를 OFF로 만든다.

		if(CheckKill()) { break; }						// 중간에도 종료 신호를 한 번 더 확인한다.

		// Bias RF StandBy 판정
		nRFGErrorSts = eDI_BRF_GenAlarm.Read(ncs);		// Bias Generator 종합 Alarm 상태를 읽는다.
		nRFMErrorSts = eDI_BRF_MAT_Alarm.Read(ncs);		// Bias Matcher 종합 Alarm 상태를 읽는다.
		BRFG_Comm = eDI_BRFG_CommSts.Read(ncs);			// Bias Generator 통신 상태를 읽는다.
		BRFM_Comm = eDI_BRFM_CommSts.Read(ncs);			// Bias Matcher 통신 상태를 읽는다.

		if(nRFGErrorSts == 0 && nRFMErrorSts == 0 && BRFG_Comm == 1 && BRFM_Comm == 1)
			BRF_StandBy_RD.Write(ON, ncs);				// 네 조건이 모두 만족되면 Bias StandBy Ready를 ON으로 만든다.
		else
			BRF_StandBy_RD.Write(OFF, ncs);				// 하나라도 틀리면 Bias StandBy Ready를 OFF로 만든다.

		if(CheckKill()) { break; }						// 중간에도 종료 신호를 한 번 더 확인한다.

		UPDATE_DIGITAL(eDI_SRFG_CommSts, SRF_CommSts,     bLOG_STS);		
		UPDATE_DIGITAL(eDI_BRFG_CommSts, BRF_CommSts,     bLOG_STS);		
		UPDATE_DIGITAL(eDI_SRFM_CommSts, SRF_MAT_CommSts, bLOG_STS);		
		UPDATE_DIGITAL(eDI_BRFM_CommSts, BRF_MAT_CommSts, bLOG_STS);		

		if(CheckKill()) { break; }											

		// Source Setpoint 및 Matcher 위치 업데이트
		UPDATE_ANALOG(eAI_SRF_FwdPwr_Set,   SRF_Setpoint_RA,    bLOG_STS);	
		UPDATE_ANALOG(eAI_SRF_FwdPwr_Set,   eSRF_Setpoint_RA,   bLOG_STS);	
		UPDATE_ANALOG(eAO_SRF_FwdPwr,       eSRF_Setpoint_SA,   bLOG_STS);	
		UPDATE_ANALOG(eAO_SRF_MAT_MatchPos, eSRFM_Match_SetPos, bLOG_STS);	
		UPDATE_ANALOG(eAO_SRF_MAT_TunePos,  eSRFM_Tune_SetPos,  bLOG_STS);	
		UPDATE_ANALOG(eAI_SRF_MAT_MatchPos, SRFM_Match_Pos,     bLOG_STS);	
		UPDATE_ANALOG(eAI_SRF_MAT_TunePos,  SRFM_Tune_Pos,      bLOG_STS);	

		if(CheckKill()) { break; }											

		// Bias Setpoint 및 Matcher 위치 업데이트
		UPDATE_ANALOG(eAI_BRF_FwdPwr_Set,   BRF_Setpoint_RA,    bLOG_STS);	
		UPDATE_ANALOG(eAI_BRF_FwdPwr_Set,   eBRF_Setpoint_RA,   bLOG_STS);	
		UPDATE_ANALOG(eAO_BRF_FwdPwr,       eBRF_Setpoint_SA,   bLOG_STS);	
		UPDATE_ANALOG(eAO_BRF_MAT_MatchPos, eBRFM_Match_SetPos, bLOG_STS);	
		UPDATE_ANALOG(eAO_BRF_MAT_TunePos,  eBRFM_Tune_SetPos,  bLOG_STS);	
		UPDATE_ANALOG(eAI_BRF_MAT_MatchPos, BRFM_Match_Pos,     bLOG_STS);	
		UPDATE_ANALOG(eAI_BRF_MAT_TunePos,  BRFM_Tune_Pos,      bLOG_STS);	

		if(WaitKill(100)) break;											
	}

	_endthread();															
}


//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------
SEQ_STATUS SeqMain(const char* szCommand, const void* pParam, int nNumOfBytesRecved)
{
	printf("-----------> [ %s ] START... [%s]\n", GetSeqName(), szCommand);	// 시퀀스 시작 로그를 콘솔에 출력한다.

	int  ncs = 0;															// 기존 형식 유지를 위한 CS 변수다.
	BOOL bLOG_STS = FALSE;													// 기존 형식 유지를 위한 로그 옵션 변수다.

	_beginthread(RF_Value_Monitoring,   0, NULL);	_sleep(100);			// 전력값 모니터링 thread를 시작하고 100ms 대기한다.
	_beginthread(RF_Channel_Monitoring, 0, NULL);	_sleep(100);			// 채널 상태 모니터링 thread를 시작하고 100ms 대기한다.

	while(1)																// 메인 시퀀스는 Kill 신호가 올 때까지 대기한다.
	{
		if(WaitKill(1000)) break;											// 1초마다 Kill 신호를 확인하고 오면 loop를 종료한다.
	}
	_sleep(2000);															// 종료 전에 2초 대기하여 하위 thread가 정리될 시간을 준다.

	return SEQ_SUCCESS;													// 정상 종료를 반환한다.
}
//----------------------------------------------------------------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------------------------------------------------------------


//----------------------------------------------------------------------------------------------------------------------------------------------
void OnLoadComplete(const TCHAR* szArgument)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
BOOL OnInitComplete(const TCHAR* szArgument)
{
	int CommStatus = 0;													// 시뮬레이션 모드 상태를 읽기 위한 임시 변수다.

	if(SIMULATION_CONTROL.Read(CommStatus) == ON)						// 시스템 시뮬레이션 모드가 ON이면
		SIMULATION_MODE = TRUE;											// 내부 시뮬레이션 플래그를 TRUE로 설정한다.
	else																// 시스템 시뮬레이션 모드가 OFF이면
		SIMULATION_MODE = FALSE;										// 내부 시뮬레이션 플래그를 FALSE로 설정한다.

	return TRUE;														// 초기화 완료를 TRUE로 반환한다.
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnUnLoad(int nCode)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
void OnEventRecv(const char* szCommand, const void* pParam, int nNumOfBytesRecved)
{
}
//----------------------------------------------------------------------------------------------------------------------------------------------
