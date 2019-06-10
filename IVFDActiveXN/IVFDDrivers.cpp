#include "IVFDDrivers.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

/*

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
unsigned char 	enableflag;			//使能标识； 1为使用，0为不使用；
short 			real_x;				//真实x坐标；
short 			real_y;				//真实y坐标；
short 			image_x;			//图像x坐标；
short 			image_y;			//图像y坐标；
}VFD_SENCE_CLBR;

typedef struct tagSMTK_CHANNEL_VIDEO_CALIB
{
	VFD_PARA_SENCE 		video_result;
	VFD_REGULATE		video_regulate;
	VFD_SENCE_CLBR		video_clbr[6];
	unsigned int 		calib_flag;

}SMTK_CHANNEL_VIDEO_CALIB;

*/



VFD_SENCE_CLBR	video_clbr_para[6]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};
VFD_SENCE_CLBR	video_clbr_para_tmp[6]={{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0},{0,0,0,0}};

static	SMTK_CHANNEL_VIDEO_CALIB 	video_calib_default = {{518067.4,3229.8,118,866.7,176.5,848.9},//k,c,b,w,v,lamt
{2,-2,10,2}, //fire_bd_regulate,fire_bd_regu_high,fire_bd_regu_level,fire_bd_regumode
{{1,0,8000,220,430},{1,0,16000,294,392},{1,0,20000,310,382},{1,0,24000,320,376},{1,800,24000,350,376},{1,1200,24000,368,376}},//{enableflag,real_x,real_y,image_x,image_y}
0};



float vfd_vetor_multiply(float * a, float * b, int length)
{
	int i;
	float sum =0;
	for (i = 0; i < length; i++)
	{
		sum+= a[i] * b[i];
	}
	return sum;
}
float vfd_vetor_sum(float * a, int length)
{
	int i;
	float sum = 0;
	for (i = 0; i < length; i++)
	{
		sum+= a[i];
	}
	return sum;
}

/*
通过标定的realxy 和 imgexy，求出k,v,b,w,m,lamt.
*/
void vfd_sence_count(VFD_SENCE_CLBR * clbr_para, VFD_PARA_SENCE * sencerult)
{
	int ponum = 0;
	int i,j;
	int temp;
	VFD_SENCE_CLBR Iclbr_para[6];
	float imagey[6],realy[6];
	float imagex[6],realx[6];
	float dt[6];
	int ynum, xnum;
	int flag;
	float b0 = 704;//704
	float k,b,c;
	float temp1,temp2;
	float lamt,w;
	float t;
	float v = 352.5;//352.5
	int  maxy = 0;

	int increasconver = 26;

	for (i = 0; i < 6; i++)
	{
		if (clbr_para[i].enableflag)
		{
			memcpy(Iclbr_para+ponum, clbr_para+i, sizeof(VFD_SENCE_CLBR));
			ponum++;
		}
	}
	if (ponum < 3)
	{
		return;
	}

	imagey[0] = Iclbr_para[0].image_y;
	realy[0] = Iclbr_para[0].real_y;
	maxy = imagey[0];
	ynum = 1;
	for (i = 1; i < ponum;i++)
	{
		temp = Iclbr_para[i].image_y;
		flag =0;
		for (j=0;j<i;j++)
		{
			if (temp == imagey[j])
			{
				flag = 1;//在
			}
		}
		if (Iclbr_para[i].real_y == 0)
		{
			flag = 0;
		}
		if (!flag)
		{
			imagey[ynum] = temp;
			realy[ynum] = Iclbr_para[i].real_y;
			maxy = max(maxy, temp);
			ynum++;
		}
	}

	if (ynum > 3)//确认可用点个数
	{
		b = b0+1;
		j = 0;
		t = (realy[0] -realy[1]) * (imagey[1] - imagey[2])/( (realy[1] -realy[2]) * (imagey[0] - imagey[1]));
		b = (t*imagey[0] - imagey[2])/(t - 1);
		temp1 =1;

		while ((b - b0 >0.0000001 || b-b0 <-0.0000001) && j<10000)
		{
			b0 =b; 
			j++;
			for (i =0; i< ynum; i++)
			{
				dt[i] = 100.0/(b - imagey[i]);
			}
			k =100 * (vfd_vetor_sum(realy,ynum)*vfd_vetor_sum(dt,ynum)/ynum - vfd_vetor_multiply(realy,dt,ynum)) /
				(vfd_vetor_sum(dt,ynum) * vfd_vetor_sum(dt,ynum)/ynum - vfd_vetor_multiply(dt,dt,ynum));
			c =(k * vfd_vetor_sum(dt,ynum) - 100*vfd_vetor_sum(realy,ynum))/(100*ynum);
			if (j < 0)
			{
				temp1 = 0;
				temp2 = 0;
				for (i = 0; i < ynum;i++)
				{
					temp1 += (realy[i]*100 - k *dt[i] - c *100 )*(realy[i]*100 - k *dt[i] - 100*c);
					temp2 += k*(realy[i]*100 - k*dt[i] - c*100 )*dt[i] * dt[i];
				}
				b = b + 50 * temp1/temp2;
				temp2 =temp1;
			}else
			{
				for (i = 0; i < ynum; i++)
				{
					dt[i] = k/(realy[i] +c);
				}
				temp2 =temp1;
				temp1 = (vfd_vetor_sum(imagey,ynum) + vfd_vetor_sum(dt,ynum))/ynum  - b;

				if (temp1 *temp2 < 0 && increasconver>4)
				{
					increasconver-=4;
				}	
				b = b + increasconver*temp1;
			}
		}
	}

	if (ponum < 3)
	{
		return;
	}

	xnum = 0;
	for (i = 0; i < ponum; i++)
	{
		temp = Iclbr_para[i].real_x;
		if (temp != 0)
		{
			imagex[xnum] =Iclbr_para[i].image_x;
			realx[xnum] = Iclbr_para[i].real_x;
			imagey[xnum] = Iclbr_para[i].image_y;
			xnum++;
		}
	}

	if (xnum >= 2)
	{
		for (i =0; i< xnum; i++)
		{
			dt[i] = (imagex[i]- v)/( b -imagey[i] );			
		}

		lamt = ( vfd_vetor_sum(dt,xnum) * vfd_vetor_sum(realx,xnum)/xnum -  vfd_vetor_multiply(realx,dt,xnum) )/
			(vfd_vetor_sum(dt,xnum) * vfd_vetor_sum(dt,xnum)/xnum - vfd_vetor_multiply(dt,dt,xnum));

		w = (vfd_vetor_sum(realx,xnum) - lamt*vfd_vetor_sum(dt,xnum))/xnum;

	}
	else
	{
		lamt = 0;
		w = 0;
	}

	sencerult->b = b;
	sencerult->k = k;
	sencerult->c = c;
	sencerult->lamt =lamt;
	sencerult->v =v;
	sencerult->w =w;
}	




//标定验证
/******************************************************************************************************  
Function Name:                          
Description:                            通过模型参数求实际realX,realY
Input:                                  

Output:                                 

Return:                                 
********************************************************************************************************/

void _VideoRealXYcount(VFD_PARA_SENCE * secpara, short image_x, short image_y,short * real_x,short *real_y)
{
	int calibratepiexl = 0 ;
	int calibratehigh = 0;
	//计算y = k(b-x)-c;
	if (  abs(secpara->b - image_y) <1)
	{
		*real_x = 0;
		*real_y = 0;
		printf("abs(secpara->b - image_y) <1\n");
		return;
	}
	if (abs(secpara->b <0.1)  || abs(secpara->k)  < 0.1)
	{
		*real_x = 0;
		*real_y = 0;
		printf("abs(secpara->b <0.1)  || abs(secpara->k)  < 0.1\n");
		return;
	}

	image_y-= calibratepiexl;
	*real_y = (short)(secpara->k /(secpara->b - image_y ) - secpara->c);


	if (secpara->lamt != 0)
	{	
		*real_x = (short) ((image_x -secpara->v) * secpara->lamt/(secpara->b - image_y) +secpara->w);
	}else
		*real_x = 0;


	*real_y -=  calibratehigh * secpara->k/(secpara->lamt * ( secpara->b - image_y));
	printf("realx = %d, realy = %d \n", *real_x, *real_y);

}

/******************************************************************************************************  
    Function Name:                          
    Description:       设置参考坐标系                     
    Input:                                  
                                            
    Output:                                 
                                            
    Return:                                 
********************************************************************************************************/
void SMTK_calibCounterSetPoint(int num,int x,int y)
{
	video_clbr_para[num].enableflag = 1;
	video_clbr_para[num].image_x = x;
	video_clbr_para[num].image_y = y;	
}

void SMTK_calibCounterGetPoint(int num,int *x,int *y)
{
	if (video_clbr_para[num].enableflag == 1)
	{
		*x = video_clbr_para[num].image_x;
		*y = video_clbr_para[num].image_y;	
	}
}

void 	SMTK_calibCounterMovePoint(int num,int x,int y)
{
	if (video_clbr_para[num].enableflag == 1)
	{
		video_clbr_para[num].image_x = video_clbr_para[num].image_x + x;
		video_clbr_para[num].image_y = video_clbr_para[num].image_y + y;	
	}
}


/******************************************************************************************************  
    Function Name:                          
    Description:                    设置真实坐标系        
    Input:                                  
                                            
    Output:                                 
                                            
    Return:                                 
********************************************************************************************************/

void	SMTK_calibCounterSetLight(int num,float x,float y)
{
	if (video_clbr_para[num].enableflag == 1)
	{
		video_clbr_para[num].real_x = x;
		video_clbr_para[num].real_y = y;	
	}
}

void	SMTK_calibCounterMoveLight(int num,float x,float y)
{
	if (video_clbr_para[num].enableflag == 1)
	{
		video_clbr_para[num].real_x = video_clbr_para[num].real_x + x;
		video_clbr_para[num].real_y = video_clbr_para[num].real_y + y;	
	}
}

void	SMTK_calibCounterGetLight(int num,short *x,short *y)
{
	if (video_clbr_para[num].enableflag == 1)
	{
		*x = video_clbr_para[num].real_x;
		*y = video_clbr_para[num].real_y;	
	}
}


//验证标定数据
double SMTK_channelVideoCalibIdentify(SMTK_CHANNEL_VIDEO_CALIB *video_calib)
{
	int i, sum_aval = 0;

	double diff[6], everage_diff = 0, max_range = 0;

	short rx, ry;
	if (video_calib == NULL)
	{
		return SMTK_ERROR_OTHER;
	}
	SMTK_CHANNEL_VIDEO_CALIB *m_video_calib = (SMTK_CHANNEL_VIDEO_CALIB *)video_calib;

	for (i=0; i<6; i++)
	{
		diff[i] = 0;
		if (m_video_calib->video_clbr[i].enableflag == 1)
		{
			_VideoRealXYcount(&m_video_calib->video_result, 
				m_video_calib->video_clbr[0].image_x, IVFDIMAGE_HEIGHT- m_video_calib->video_clbr[0].image_y,
				&rx, &ry);
			diff[i] += fabs((float)(rx-m_video_calib->video_clbr[0].real_x));
			diff[i] += fabs((float)(ry-m_video_calib->video_clbr[0].real_y));
			sum_aval++;
			if (fabs((float)(m_video_calib->video_clbr[0].real_x)) > max_range)
			{
				max_range = fabs((float)(m_video_calib->video_clbr[0].real_x));
			}
			if (fabs((float)(m_video_calib->video_clbr[0].real_y)) > max_range)
			{
				max_range = fabs((float)(m_video_calib->video_clbr[0].real_y));
			}
		}
	}

	for (i=0; i<6; i++)
	{
		everage_diff += diff[i];
	}

	everage_diff = everage_diff/(sum_aval*max_range);

	return everage_diff;
}

void IVFDCalibrate(){

	SMTK_CHANNEL_VIDEO_CALIB *m_video_calib = (SMTK_CHANNEL_VIDEO_CALIB *)malloc(sizeof(SMTK_CHANNEL_VIDEO_CALIB));
	memcpy(m_video_calib, &video_calib_default, sizeof(SMTK_CHANNEL_VIDEO_CALIB ));
	memcpy(video_clbr_para_tmp,video_clbr_para,6*sizeof(VFD_SENCE_CLBR));
/*	video_clbr_para_tmp[0].image_y = IVFDIMAGE_HEIGHT - video_clbr_para[0].image_y;
	video_clbr_para_tmp[1].image_y = IVFDIMAGE_HEIGHT - video_clbr_para[1].image_y;
	video_clbr_para_tmp[2].image_y = IVFDIMAGE_HEIGHT - video_clbr_para[2].image_y;
	video_clbr_para_tmp[3].image_y = IVFDIMAGE_HEIGHT - video_clbr_para[3].image_y;
	video_clbr_para_tmp[4].image_y = IVFDIMAGE_HEIGHT - video_clbr_para[4].image_y;
	video_clbr_para_tmp[5].image_y = IVFDIMAGE_HEIGHT - video_clbr_para[5].image_y;	*/									
	vfd_sence_count(video_clbr_para_tmp,&(m_video_calib->video_result));//此处是在channel初始化时赋值的。//此处求得k,c,v,b,m,lamt，通过标定的imgxy,和realxy，来求。

	memcpy(m_video_calib->video_clbr, video_clbr_para, 6*sizeof(VFD_SENCE_CLBR));

	//SMTK_channelVideoSaveCalib(video_handle);	
}