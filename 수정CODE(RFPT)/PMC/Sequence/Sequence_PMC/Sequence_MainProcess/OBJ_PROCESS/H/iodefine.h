//#ifndef IODEFINE_H
//#define IODEFINE_H
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#define		MAX_STEP_COUNT			100
#define		MAX_RECIPENAME_LEN		255
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#define		DEFAULT_LENGTH					257
#define		DEFAULT_LTH0008					9
#define		DEFAULT_LTH0016					17
#define		DEFAULT_LTH0032					33
#define		DEFAULT_LTH0064					65
#define		DEFAULT_LTH0128					129
#define		DEFAULT_LTH0256					257
#define		DEFAULT_LTH0512					513
#define		DEFAULT_LTH1024					1025
#define		DEFAULT_LTH2048					2049
#define		DEFAULT_LTH4096					4097
#define		DEFAULT_LTH8192					8193
#define		DEFAULT_SUMMARY					50000
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
#define		MAX_PORTMDL_COUNT				4
#define		MAX_PORTSLOT_COUNT				30
#define		MAX_STEP_COUNT2					200
//  [2013/6/12 HOONI] UOP-0500-00a-A3
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
//----------------------------------------------------------------------------------------
// typedef struct {
// 	BOOL	PORTUSE;
// 	char	CARRIER;
// 	int		WAFERID[MAX_PORTSLOT_COUNT];
// 	char	PJNAME[MAX_PORID[DEFAULT_LTH0128];
// 	char	JOBNAME[DEFAULT_LTH0128]; // LOTID in 200mm, CJNAME in 300mm
// 	int		WAFERCOUNTTSLOT_COUNT][DEFAULT_LTH0128];
// 	char	LOGDIRNAME[DEFAULT_LTH0256]; // CTC Job Directory Name
// } PortInfoStruct;
 //----------------------------------------------------------------------------------------
// typedef struct {
// 	BOOL	DUMMY;
// 	PortInfoStruct	PORTINFO[MAX_PORTMDL_COUNT];
// 	int		RECIPETYPE; // RECIPETYPE : 0=Pre,1=Main,2=Post,9=Manual,11=LotPre,12=LotPost,21=Etc => Current report the event for only Main(0) Recipe, but logging
// 	char	RECIPENAME[DEFAULT_LTH0128];
// 	int		TOTALSTEP;
// 	char	STEPNAME[MAX_STEP_COUNT2][DEFAULT_LTH0064];
// 	BOOL	STEPLOGGING[MAX_STEP_COUNT2]; // if STEPLOGGING is FALSE, STEPSUMMARY/STEPREPORT don't mean
// 	BOOL	STEPSUMMARY[MAX_STEP_COUNT2]; // for Wafer, Lot Min/Max/Avr
// 	BOOL	STEPREPORT[MAX_STEP_COUNT2]; // STEPREPORT : STEP START/END Event Disable(0)/Enable(1), No Step Calculation for only Main(0) Recipe
// } RecipeInfoStruct;
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------------
typedef struct 
{
	int	DELAY_TIME;
	int	WAIT_TIME;
	int	TEMPERATURE;
} RecipeStepTemplate;

typedef struct 
{
	char	STEP_NAME[255];
	int		STEP_ENDTYPE;
	int		STEP_TYPE;
	double	STEP_TIME;

	int		Is_RF_Step;
	
	int		GAS1;
	int		GAS2;
	int		GAS3;
	int		GAS4;
	int		GAS5;
	int		GAS6;
	int		GAS7;
	int		GAS8;
	int		GAS9;
	int		GAS10;

	//ADD
	int     FRC_CENTER;
	int     FRC_MIDDLE;
	
	char	SRF_CTRL[32];
	char	BRF_CTRL[32];
	char	APC_CTRL[32];
	char	ESC_CTRL[32];
	char	HE_CTRL[32];
	
	int		SRFG_POWER;
	int		SRFM_MATCH;
	int		SRFM_TUNE;
	
	int		BRFG_POWER;
	int		BRFM_MATCH;
	int		BRFM_TUNE;
	
	int     PIN_POS;
	int		PRESSURE;
	int		EPD_RCPNUM;
	int     EPD_MINTIME;
	int		EPD_STEPNUM;
	int		EPD_ALARM;
	int		EPD_OVRETCHTYPE;
	int		EPD_OVRETCHTIME;

		
	int		ESC_VOLT;
	double	HE_PRESSURE;

	double	VVC01_POS;
	double	VVC02_POS;
	double	VVC03_POS;
	double	VVC04_POS;
	double	VVC05_POS;
	double	VVC06_POS;
	double	CHILLER_CH01_TEMP;
	double	CHILLER_CH02_TEMP;
	double	CHILLER_CH03_TEMP;
	double	HE_FLOW_ALARM;

	int		VVC_MOTER_TABLE;

		
} RecipeStepTemplateM;

//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------

