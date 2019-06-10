#include "IVFDRtspPlayer.h"
using namespace std;
#include "ChanelWidget.h"
#include <QString>
#include <QByteArray>

int glb_flag = 0;
static double r2d(AVRational r)
{

	return r.num == 0 || r.den == 0?0. : (double)r.num / (double)r.den;
}


IVFDRtspPlayer::IVFDRtspPlayer(QObject *parent)
	: QObject(parent)//h264Video0
{
	m_timer = NULL;
	m_timeStamp = 0;
	m_rtspThread = new QThread(0);
	m_rtspThread->start();
	//m_url = QString("rtsp://192.168.120.166/h264Video0");

	this->moveToThread(m_rtspThread);

	QObject::connect(m_rtspThread, SIGNAL(finished()), this, SLOT(deleteLater()));
	QObject::connect(this, SIGNAL(SIG_StartToPlay0()), this, SLOT(SLOT_StartToPlay()));
	QObject::connect(this, SIGNAL(SIG_StartTryPlay()), this, SLOT(SLOT_StartTryPlay()));

}

IVFDRtspPlayer::IVFDRtspPlayer(QObject *parent, QString url, qlonglong timeStamp)
	: QObject(parent), m_url(url), m_timeStamp(timeStamp)
{
	m_timer = NULL;
	m_rtspThread = new QThread(0);
	m_rtspThread->start();

	this->moveToThread(m_rtspThread);
	qDebug() << "now get the timestamp = " << timeStamp;
	QObject::connect(m_rtspThread, SIGNAL(finished()), this, SLOT(deleteLater()));
	QObject::connect(this, SIGNAL(SIG_StartToPlay0()), this, SLOT(SLOT_StartToPlay()));
	QObject::connect(this, SIGNAL(SIG_StartTryPlay()), this, SLOT(SLOT_StartTryPlay()));

}

IVFDRtspPlayer::~IVFDRtspPlayer()
{

}


void IVFDRtspPlayer::SLOT_StartToPlay()
{
	qDebug() << "do SLOT_StartToPlay!";
	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVFrame *pFrame, *pFrameRGB;
	AVPacket *packet;
	uint8_t *out_buffer;
    static struct SwsContext *img_convert_ctx;

    int videoStream, i, numBytes;
    int ret, got_picture;

    avformat_network_init();   //��ʼ��FFmpeg����ģ�飬2017.8.5---wanggy
    av_register_all();         //��ʼ��FFMPEG  ��������������������ñ������ͽ�����
	qDebug() << "end av_register_all";

    //Allocate an AVFormatContext.
    pFormatCtx = avformat_alloc_context();
	qDebug() << "end avformat_alloc_context";
    //2017.8.5---
    AVDictionary *avdic=NULL;
    char option_key[]="rtsp_transport";
    char option_value[]="tcp";
    av_dict_set(&avdic,option_key,option_value,0);
    char option_key2[]="max_delay";
    char option_value2[]="100";//100
    av_dict_set(&avdic,option_key2,option_value2,0);
	qDebug() << "end av_dict_set";
    //char url[]="rtsp://192.168.120.166/h264Video0";

	QByteArray ba = m_url.toLatin1();
	const char *url = ba.data();
    //char url[]="rtsp://192.168.120.166/h264ESVideoTest";
	int rettmp = avformat_open_input(&pFormatCtx, url, NULL, &avdic);
	qDebug() << "IVFDRtspPlayer thread:" << QThread::currentThread();

	qDebug() << "rettmp = " << rettmp << "IVFDRtspPlayer thread:" << QThread::currentThread();
    if (rettmp != 0) {
		//SLOT_StartTryPlay();
		qDebug() << "couldnt open the file";
        //printf("can't open the file. \n");
		m_rtspThread->quit();
        return;
    }
	rettmp = avformat_find_stream_info(pFormatCtx, NULL) ;
    if (rettmp < 0) {
		//SLOT_StartTryPlay();
		qDebug() << "couldnt find streadm information";
        //printf("Could't find stream infomation.\n");
        return;
    }

    videoStream = -1;

    ///ѭ��������Ƶ�а���������Ϣ��ֱ���ҵ���Ƶ���͵���
    ///�㽫���¼���� ���浽videoStream������
    ///������������ֻ������Ƶ��  ��Ƶ���Ȳ�����
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }
    ///���videoStreamΪ-1 ˵��û���ҵ���Ƶ��
    if (videoStream == -1) {
        printf("Didn't find a video stream.\n");
        return;
    }

    ///���ҽ�����
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
	qDebug() << "pFormatCtx->streams[videoStream] = " << pFormatCtx->streams[videoStream]->first_dts;

    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    //2017.8.9---wanggy+
    pCodecCtx->bit_rate =0;   //��ʼ��Ϊ0
    pCodecCtx->time_base.num=1;  //�������У�һ����25֡
    pCodecCtx->time_base.den= 25; //10
    pCodecCtx->frame_number=1;  //ÿ��һ����Ƶ֡

    if (pCodec == NULL) {
        printf("Codec not found.\n");
        return;
    }

    ///�򿪽�����
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec.\n");
        return;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    //2017.8.7---wanggy
    //cout<<pCodecCtx->width<<endl;

    ///�������Ǹĳ��� ��������YUV����ת����RGB32
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
            pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
            PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    numBytes = avpicture_get_size(PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer, PIX_FMT_RGB32,
            pCodecCtx->width, pCodecCtx->height);

    int y_size = pCodecCtx->width * pCodecCtx->height;

    packet = (AVPacket *) malloc(sizeof(AVPacket)); //����һ��packet
    av_new_packet(packet, y_size); //����packet������

    //2017.8.1---wanggy
    av_dump_format(pFormatCtx, 0, url, 0); //�����Ƶ��Ϣ
	qDebug("read ---first dts is %lld", pFormatCtx->streams[packet->stream_index]->first_dts);

    while (1)
    {
		glb_flag = 1;
		//qDebug() << "In IVFDRtspPlayer do while()!" ;

        if (av_read_frame(pFormatCtx, packet) < 0)
        {
			qDebug() << "video end";
			QImage tmpImg("test.png");
			QImage image = tmpImg.copy(); //��ͼ����һ�� ���ݸ�������ʾ
			emit SIG_GetOneFrame(image);  //�����ź�
			glb_flag = 0;
			//emit SIG_StartTryPlay();
			qDebug() << "ready to quit rtspthread";
			if(m_rtspThread != NULL)
			{
				m_rtspThread->quit();
			}

            break; //������Ϊ��Ƶ��ȡ����
        }

        if (packet->stream_index == videoStream) {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

            if (ret < 0) {
                printf("decode error.\n");
                return;
            }

            if (got_picture) {
                sws_scale(img_convert_ctx,
                        (uint8_t const * const *) pFrame->data,
                        pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                        pFrameRGB->linesize);

                //�����RGB���� ��QImage����
                QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
                QImage image = tmpImg.copy(); //��ͼ����һ�� ���ݸ�������ʾ
                emit SIG_GetOneFrame(image);  //�����ź�

				int pts = packet->pts*r2d(pFormatCtx->streams[packet->stream_index]->time_base)*1000;
				for(int i = 0; i < pFormatCtx->streams[packet->stream_index]->codec->extradata_size; i++)
				{
					//qDebug("%d ---%x  ", i, pFormatCtx->streams[packet->stream_index]->codec->extradata[i]);
				}
				//qDebug() << "pcket.dts = " << packet->dts << "packet.pts = " << packet->pts << " pts = " << pts;
				//qDebug("read dts is %lld", packet->dts);

				//qDebug("read ---current dts is %lld", pFormatCtx->streams[packet->stream_index]->cur_dts);
            }
        }
        av_free_packet(packet); //�ͷ���Դ,�����ڴ��һֱ����

        //2017.12.17---wanggy
        QThread::msleep(1); //ͣһͣ  ��Ȼ�ŵ�̫����

        //2017.12.9---wanggy
        /*int64_t start_time=av_gettime();
        AVRational time_base=pFormatCtx->streams[videoStream]->time_base;
        AVRational time_base_q={1,AV_TIME_BASE};
        int64_t pts_time = av_rescale_q(packet->dts, time_base, time_base_q);
        int64_t now_time = av_gettime() - start_time;
        if (pts_time > now_time)
             av_usleep(pts_time - now_time);*/
    }
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
	qDebug() << "---------ffmpeg release finished!---------" << QThread::currentThread();
	//m_rtspThread->quit();

	QThread::msleep(0.05);

	//emit SIG_StartTryPlay();
}

void IVFDRtspPlayer::SLOT_StartTryPlay()
{
	qDebug() << "do SLOT_StartTryPlay()!";
	m_timer = new QTimer(0);
	m_timer->setInterval(1000);
	QObject::connect(m_timer, &QTimer::timeout, [&](){

		if(glb_flag == 0)
		{
			SLOT_StartToPlay();
		}else
		{
			m_timer->deleteLater();
		}

		qDebug() << "try connect again!";
	});
	m_timer->start();


}

void IVFDRtspPlayer::SLOT_StartToPlay1()
{
	qDebug() << "do SLOT_StartToPlay1 !";
	AVFormatContext *pFormatCtx;
	AVCodecContext *pCodecCtx;
	AVCodec *pCodec;
	AVFrame *pFrame, *pFrameRGB;
	AVPacket *packet;
	uint8_t *out_buffer;
    static struct SwsContext *img_convert_ctx;

    int videoStream, i, numBytes;
    int ret, got_picture;

    avformat_network_init();   //��ʼ��FFmpeg����ģ�飬2017.8.5---wanggy
    av_register_all();         //��ʼ��FFMPEG  ��������������������ñ������ͽ�����


    //Allocate an AVFormatContext.
    pFormatCtx = avformat_alloc_context();

    //2017.8.5---
    AVDictionary *avdic=NULL;
    char option_key[]="rtsp_transport";
    char option_value[]="tcp";
    av_dict_set(&avdic,option_key,option_value,0);
    char option_key2[]="max_delay";
    char option_value2[]="100";//100
    av_dict_set(&avdic,option_key2,option_value2,0);

	QByteArray url = m_url.toLatin1();

	//char url[]="rtsp://192.168.120.168/h264ESVideoTest";//h264Video0
    //char url[]="rtsp://192.168.120.168/h264ESVideoTest";//h264Video0
    //char url[]="rtsp://192.168.120.166/h264ESVideoTest";
	int rettmp = avformat_open_input(&pFormatCtx, url.data(), NULL, &avdic);
    if (rettmp != 0) {
		//SLOT_StartTryPlay();
		qDebug() << "couldnt open the file";
        //printf("can't open the file. \n");
        return;
    }
	rettmp = avformat_find_stream_info(pFormatCtx, NULL) ;
    if (rettmp < 0) {
		//SLOT_StartTryPlay();
		qDebug() << "couldnt find streadm information";
        //printf("Could't find stream infomation.\n");
        return;
    }

    videoStream = -1;

    ///ѭ��������Ƶ�а���������Ϣ��ֱ���ҵ���Ƶ���͵���
    ///�㽫���¼���� ���浽videoStream������
    ///������������ֻ������Ƶ��  ��Ƶ���Ȳ�����
    for (i = 0; i < pFormatCtx->nb_streams; i++) {
        if (pFormatCtx->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStream = i;
        }
    }
    ///���videoStreamΪ-1 ˵��û���ҵ���Ƶ��
    if (videoStream == -1) {
        printf("Didn't find a video stream.\n");
        return;
    }

    ///���ҽ�����
    pCodecCtx = pFormatCtx->streams[videoStream]->codec;
    pCodec = avcodec_find_decoder(pCodecCtx->codec_id);
    //2017.8.9---wanggy
    pCodecCtx->bit_rate =0;   //��ʼ��Ϊ0
    pCodecCtx->time_base.num=1;  //�������У�һ����25֡
    pCodecCtx->time_base.den=10;
    pCodecCtx->frame_number=1;  //ÿ��һ����Ƶ֡

    if (pCodec == NULL) {
        printf("Codec not found.\n");
        return;
    }

    ///�򿪽�����
    if (avcodec_open2(pCodecCtx, pCodec, NULL) < 0) {
        printf("Could not open codec.\n");
        return;
    }

    pFrame = av_frame_alloc();
    pFrameRGB = av_frame_alloc();

    //2017.8.7---wanggy
    //cout<<pCodecCtx->width<<endl;

    ///�������Ǹĳ��� ��������YUV����ת����RGB32
    img_convert_ctx = sws_getContext(pCodecCtx->width, pCodecCtx->height,
            pCodecCtx->pix_fmt, pCodecCtx->width, pCodecCtx->height,
            PIX_FMT_RGB32, SWS_BICUBIC, NULL, NULL, NULL);

    numBytes = avpicture_get_size(PIX_FMT_RGB32, pCodecCtx->width,pCodecCtx->height);

    out_buffer = (uint8_t *) av_malloc(numBytes * sizeof(uint8_t));
    avpicture_fill((AVPicture *) pFrameRGB, out_buffer, PIX_FMT_RGB32,
            pCodecCtx->width, pCodecCtx->height);

    int y_size = pCodecCtx->width * pCodecCtx->height;

    packet = (AVPacket *) malloc(sizeof(AVPacket)); //����һ��packet
    av_new_packet(packet, y_size); //����packet������

    //2017.8.1---wanggy
    av_dump_format(pFormatCtx, 0, url, 0); //�����Ƶ��Ϣ

    while (1)
    {
		glb_flag = 1;
		qDebug() << "In IVFDRtspPlayer do while()!" ;

        if (av_read_frame(pFormatCtx, packet) < 0)
        {
			qDebug() << "video end";
			QImage tmpImg("test.png");
			QImage image = tmpImg.copy(); //��ͼ����һ�� ���ݸ�������ʾ
			emit SIG_GetOneFrame(image);  //�����ź�
			glb_flag = 0;
			//emit SIG_StartTryPlay();
            break; //������Ϊ��Ƶ��ȡ����
        }

        if (packet->stream_index == videoStream) {
            ret = avcodec_decode_video2(pCodecCtx, pFrame, &got_picture,packet);

            if (ret < 0) {
                printf("decode error.\n");
                return;
            }

            if (got_picture) {
                sws_scale(img_convert_ctx,
                        (uint8_t const * const *) pFrame->data,
                        pFrame->linesize, 0, pCodecCtx->height, pFrameRGB->data,
                        pFrameRGB->linesize);

                //�����RGB���� ��QImage����
                QImage tmpImg((uchar *)out_buffer,pCodecCtx->width,pCodecCtx->height,QImage::Format_RGB32);
                QImage image = tmpImg.copy(); //��ͼ����һ�� ���ݸ�������ʾ
                emit SIG_GetOneFrame(image);  //�����ź�
            }
        }
        av_free_packet(packet); //�ͷ���Դ,�����ڴ��һֱ����

        //2017.12.17---wanggy
        QThread::msleep(1); //ͣһͣ  ��Ȼ�ŵ�̫����

        //2017.12.9---wanggy
        /*int64_t start_time=av_gettime();
        AVRational time_base=pFormatCtx->streams[videoStream]->time_base;
        AVRational time_base_q={1,AV_TIME_BASE};
        int64_t pts_time = av_rescale_q(packet->dts, time_base, time_base_q);
        int64_t now_time = av_gettime() - start_time;
        if (pts_time > now_time)
             av_usleep(pts_time - now_time);*/
    }
    av_free(out_buffer);
    av_free(pFrameRGB);
    avcodec_close(pCodecCtx);
    avformat_close_input(&pFormatCtx);
	qDebug() << "---------ffmpeg release finished!---------" << QThread::currentThread();
	m_rtspThread->quit();

	QThread::msleep(500);

	emit SIG_StartTryPlay();
}
