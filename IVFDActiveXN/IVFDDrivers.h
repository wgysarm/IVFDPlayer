#pragma once
#ifndef IVFDDRIVERS_H_
#define IVFDDRIVERS_H_


//#include <cmath>
#include <algorithm>

#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define SMTK_ERROR_OTHER 5
#define SMTK_OK 1
#define IVFDIMAGE_HEIGHT (1080-1)
#define IVFDIMAGE_WIDTH 1920

typedef void * SMTK_ChannelVideoHandle;



typedef struct tagVFD_REGULATE
{
	int				fire_bd_regulate;
	int				fire_bd_regu_high;
	unsigned short 	fire_bd_regu_level;
	unsigned short 	fire_bd_regumode;
}VFD_REGULATE;

typedef struct tagVFD_PARA_SENCE
{
	float	k;
	float 	c;	
	float 	b;	
	float 	w;
	float 	v;
	float	lamt;
}VFD_PARA_SENCE;
typedef struct tagVFD_SENCE_CLBR
{
	unsigned char 	enableflag;			//ʹ�ܱ�ʶ�� 1Ϊʹ�ã�0Ϊ��ʹ�ã�
	short 			real_x;				//��ʵx���ꣻ
	short 			real_y;				//��ʵy���ꣻ
	short 			image_x;			//ͼ��x���ꣻ
	short 			image_y;			//ͼ��y���ꣻ
}VFD_SENCE_CLBR;

typedef struct tagSMTK_CHANNEL_VIDEO_CALIB
{
	VFD_PARA_SENCE 		video_result;
	VFD_REGULATE		video_regulate;
	VFD_SENCE_CLBR		video_clbr[6];
	unsigned int 		calib_flag;

}SMTK_CHANNEL_VIDEO_CALIB;

void vfd_sence_count(VFD_SENCE_CLBR * clbr_para, VFD_PARA_SENCE * sencerult);

void _VideoRealXYcount(VFD_PARA_SENCE * secpara, short image_x, short image_y,short * real_x,short *real_y);

#endif