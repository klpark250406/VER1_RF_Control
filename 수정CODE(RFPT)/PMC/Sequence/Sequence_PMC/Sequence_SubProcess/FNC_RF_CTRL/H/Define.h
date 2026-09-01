#ifndef DEFINE_H
#define DEFINE_H

typedef struct 
{
	int	   itk_enable;
	int	   itk_chktime;
	int	   itk_nFlag;
	int	   itk_mon_ch;
	int    itk_alarm_id;

	int    data;

	char   al_message[256];

} Intlk_Channel;

#endif