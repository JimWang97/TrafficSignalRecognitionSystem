#include<stdio.h>
#include<highgui.h>
#include<opencv.hpp>
#include"List.h"
#include<math.h>
int main()
{
	//处理方向
	int x,y;
	int h=0,U;
	char *color[3];
	char *fangxiang[100];
	double SUM1[3],SUM2[3],SUM3[3],SUM4[3];
	IplImage *Q2=cvLoadImage("自左而右右转.bmp");
	IplImage *Q3=cvLoadImage("自左至右圆形.bmp");
	IplImage *Q4=cvLoadImage("自左至右左转.bmp");
	IplImage *Q1=cvLoadImage("自上而下直行.bmp");
	unsigned char* q1=(unsigned char *)Q1->imageData;
	unsigned char* q2=(unsigned char *)Q2->imageData;
	unsigned char* q3=(unsigned char *)Q3->imageData;
	unsigned char* q4=(unsigned char *)Q4->imageData;
	int P1[999];
	int P2[999];
	int P3[999];
	int P4[999];
	for(x=0;x<=Q1->width;x++)
	{
		h=0;
		for(y=0;y<=Q1->height;y++)
		{
			if(*(q1+y*Q1->widthStep+x*3+2)==255)
				h++;
		}
		P1[x]=h;
	}
	for(x=0;x<=Q2->width;x++)
	{
		h=0;
		for(y=0;y<=Q2->height;y++)
		{
			if(*(q2+y*Q2->widthStep+x*3+2)==255)
				h++;
		}
		P2[x]=h;
	}
	for(x=0;x<=Q3->width;x++)
	{
		h=0;
		for(y=0;y<=Q3->height;y++)
		{
			if(*(q3+y*Q3->widthStep+x*3+2)==255)
				h++;
		}
		P3[x]=h;
	}
	for(x=0;x<=Q4->width;x++)
	{
		h=0;
		for(y=0;y<=Q4->height;y++)
		{
			if(*(q4+y*Q4->widthStep+x*3+2)==255)
				h++;
		}
		P4[x]=h;
	}


	IplImage *img=cvLoadImage("2.jpg");
	unsigned char* o=(unsigned char *)img->imageData;

	{

		IplImage *hsv=cvCreateImage(cvGetSize(img),8,3);
		IplImage *erzhi=cvCreateImage(cvGetSize(img),8,1);

		cvCvtColor(img,hsv,CV_BGR2HSV);
		cvNamedWindow("HSV");
		cvShowImage("HSV",hsv);
		unsigned char* q=(unsigned char *)hsv->imageData;
		unsigned char* p=(unsigned char *)erzhi->imageData;
		for(x=0;x<hsv->width;x++)
		{
			for(y=0;y<hsv->height;y++)
			{
				if(*(q+y*hsv->widthStep+x*3+2)<180)
				{
					*(q+y*hsv->widthStep+x*3+2)=0;
					*(p+y*erzhi->widthStep+x)=0;
				}
				else
					*(p+y*erzhi->widthStep+x)=255;
			}
		}
		cvNamedWindow("亮度分割");
		cvShowImage("亮度分割",hsv);
		cvNamedWindow("亮度分割黑白");
		cvShowImage("亮度分割黑白",erzhi);

		//灰度化
		IplImage *rgb=cvCreateImage(cvGetSize(img),8,3);
		cvCvtColor(hsv,rgb,CV_HSV2BGR);
		CvSize size = cvGetSize(img);
		IplImage* gray = cvCreateImage(size, 8, 1);
		cvCvtColor(rgb, gray, CV_BGR2GRAY);
		cvNamedWindow("Gray");
		cvShowImage("Gray", gray);

		//轮廓检测 
		IplImage *edge=cvCreateImage(cvGetSize(img),8,1);
		cvSmooth(gray,edge,CV_GAUSSIAN,9,9,0,0);
		cvDilate( edge, edge, NULL,1);
		cvCanny(edge,edge,80,120,3);
		cvNamedWindow("edge");
		cvShowImage("edge",edge);


		//外接矩形 长宽比滤除 相对高度滤除 面积滤除 位置提取 颜色提取 方向判断
		CvMemStorage* storage = cvCreateMemStorage( 0 );
		CvSeq* contours = NULL;
		IplImage *imgTemp=cvCloneImage( edge );
		IplImage *edge1=cvCloneImage( edge );
		cvFindContours( imgTemp, storage, &contours, sizeof( CvContour ), CV_RETR_LIST, CV_CHAIN_APPROX_NONE );

		h=0;
		int l=0,L=0,P=0;
		struct SeqList red;
		struct SeqList green;
		initList (&red,100);
		initList (&green,100);
		int j,k,sum;
		double avr1=0,avr2=0;
		int H1=0,H2=0;
		int R[100];
		int i=0;
		//聚类
		cvFindContours( imgTemp, storage, &contours, sizeof( CvContour ), CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
		for( ; contours != NULL; contours = contours -> h_next )
		{
			CvRect rect = cvBoundingRect( contours, 1 );  
			P=0;
			if((double)((double)rect.width/(double)rect.height)>0.8&&(double)((double)rect.width/(double)rect.height)<1.2&&rect.y+ rect.height<= edge1->height/3&&rect.width*rect.height>=50&&(rect.x+rect.width)<2*(double)edge1->width/3)
			{
				for(L=0;L<=100;L++)//去重合
				{
					if(h==1&&abs(R[L]-rect.x)<=10)
					{
						R[L]=rect.x;
						P=1;
						break;
					}
				}
				if(P==1)
					continue;
				h=1;
				cvRectangle( edge1, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalar(255), 1 );
				cvRectangle( img, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalar(0,0,255), 1 );
				int j,k,sum;
				sum=0;
				for(j=rect.x;j<=rect.width+rect.x;j++)//计算平均分量
				{
					for(k=rect.y;k<=rect.height+rect.y;k++)
						sum=*(o+k*img->widthStep+j*3)+sum;
				}
				sum=sum/(rect.width*rect.height);
				if(l%2==0)
					insertLastList(&red,sum);
				else
					insertLastList(&green,sum);
				R[l]=rect.x;
				l++;
			}
		}
		for(h=0;h<=L;h++)
		{
			avr1=0;
			avr2=0;
			for(j=0;j<red.size;j++)
				avr1=avr1+red.list[j];
			avr1=avr1/red.size;
			for(j=0;j<green.size;j++)
				avr2=avr2+green.list[j];
			avr2=avr2/green.size;
			for(j=0;j<red.size;j++)
			{
				if(abs(red.list[j]-avr1)>abs(red.list[j]-avr2))
				{
					insertLastList(&green,red.list[j]);
					deletePosList(&red,j+1);
				}
			}
			for(j=0;j<green.size;j++)
			{
				if(abs(green.list[j]-avr1)<abs(green.list[j]-avr2))
				{
					insertLastList(&red,green.list[j]);
					deletePosList(&green,j+1);
				}
			}
			if(H1==avr1&&H2==avr2)
				break;
			else
			{
				H1=avr1;
				H2=avr2;
			}
		}
		double Red,Green;
		if(avr1>avr2)
		{
			Red=avr2;
			Green=avr1;
		}
		else
		{
			Red=avr1;
			Green=avr2;
		}

		//提取颜色
		h=0;
		for(h=0;h<=l;h++)
			R[h]=0;
		h=0;
		L=0;
		int G=0;
		int b=0;
		cvFindContours( imgTemp, storage, &contours, sizeof( CvContour ), CV_RETR_LIST, CV_CHAIN_APPROX_NONE );
		for( ; contours != NULL; contours = contours -> h_next )
		{
			CvRect rect = cvBoundingRect( contours, 1 );  
			P=0;
			if((double)((double)rect.width/(double)rect.height)>0.8&&(double)((double)rect.width/(double)rect.height)<1.2&&rect.y+ rect.height<= edge1->height/3&&rect.width*rect.height>=50&&(rect.x+rect.width)<2*(double)edge1->width/3)
			{
				for(L=0;L<=l;L++)//去重合
				{
					if(h==1&&abs(R[L]-rect.x)<=10)
					{
						R[L]=rect.x;
						P=1;
						break;
					}
				}
				if(P==1)
					continue;
				h=1;
				cvRectangle( edge1, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalar(255), 1 );
				cvRectangle( img, cvPoint( rect.x, rect.y ),cvPoint( rect.x + rect.width, rect.y + rect.height ), cvScalar(0,0,255), 1 );
				IplImage *temp=cvCreateImage(cvSize(rect.width,rect.height),8,1);
				cvSetImageROI(gray,rect);
				cvCopy(gray, temp); 

				cvResetImageROI(gray); 
				int j,k,sum;
				sum=0;
				for(j=rect.x;j<=rect.width+rect.x;j++)//计算平均分量
				{
					for(k=rect.y;k<=rect.height+rect.y;k++)
						sum=*(o+k*img->widthStep+j*3)+sum;
				}
				sum=sum/(rect.width*rect.height);
				if(abs(sum-Red)<abs(sum-Green))
				{
					color[G]="Red";
				}
				else
				{
					color[G]="Green";
				}

				
				
				
				//对比
				double c[999];
				int g;
				//判断方向 自上而下
				for(g=0;g<999;g++)
					c[g]=0;
				g=0;
				for(y=rect.y;y<=rect.height+rect.y;y++)
				{
					for(x=rect.x;x<=rect.x+rect.width;x++)
					{
						if(*(p+y*erzhi->widthStep+x)==255)
							c[g]++;	
					}
					g++;
				}
				IplImage *zhifang=cvCreateImage(cvSize(rect.width,rect.height),8,3);
				cvZero(zhifang);
				for(y=0;y<=rect.width;y++)
					cvLine(zhifang,cvPoint(y,(rect.height-c[y])),cvPoint(y,rect.height),cvScalar(255,255,255),3);
				unsigned char* I=(unsigned char *)zhifang->imageData;
				double biwidth=rect.width/22.0;
				double biheight=rect.height/22.0;


				SUM1[G]=0;
				SUM2[G]=0;
				SUM3[G]=0;
				SUM4[G]=0;
				for(y=0;y<=22;y++)
				{

					U=(int)(y*biheight);
					SUM1[G]=SUM1[G]+fabs(c[U]/biwidth-P1[y]);
				}
				//自左而右
				for(g=0;g<999;g++)
					c[g]=0;
				g=0;

				for(x=rect.x;x<=rect.x+rect.width;x++)
				{
					for(y=rect.y;y<=rect.height+rect.y;y++)
					{
						if(*(p+y*erzhi->widthStep+x)==255)
							c[g]++;	
					}
					g++;
				}
				for(x=0;x<=22;x++)
				{

					U=(int)(x*biwidth);
					SUM2[G]=SUM2[G]+fabs(c[U]/biheight-P2[x]);

				}
				for(x=0;x<=22;x++)
				{

					U=(int)(x*biwidth);
					SUM3[G]=SUM3[G]+fabs(c[U]/biheight-P3[x]);

				}
				for(x=0;x<=22;x++)
				{

					U=(int)(x*biwidth);
					SUM4[G]=SUM4[G]+fabs(c[U]/biheight-P4[x]);

				}
				
				cvNamedWindow("panduan");
				cvShowImage("panduan",zhifang);
				cvSaveImage("h.bmp",zhifang);
				cvNamedWindow("temp");
				cvShowImage("temp",temp);
				cvReleaseImage(&temp);


				G++;
				R[b]=rect.x;
				b++;
				cvReleaseImage(&temp);
			}

		}

		//对比
		double MIN=9999,M;
		for(G=0;G<=2;G++)
		{
			MIN=9999;
			if(MIN>=SUM1[G])
			{
				M=1;
				MIN=SUM1[G];
			}
			if(MIN>=SUM2[G])
			{
				M=2;
				MIN=SUM2[G];
			}
			if(MIN>=SUM3[G])
			{
				M=3;
				MIN=SUM3[G];
			}
			if(MIN>=SUM4[G])
			{
				M=4;
				MIN=SUM4[G];
			}
			if(M==1)
				fangxiang[G]="Straight";
			if(M==2)
				fangxiang[G]="Right";
			if(M==3)
				fangxiang[G]="Circle";
			if(M==4)
				fangxiang[G]="Left";
		}

		for(i=0;i<=2;i++)
			printf("%s %s\n",fangxiang[i],color[i]);
		cvNamedWindow("edge1");
		cvShowImage("edge1",edge1);
		cvNamedWindow("结果");
		cvShowImage("结果",img);



		cvWaitKey();
		cvReleaseImage(&img);
		cvReleaseImage(&gray);
		cvReleaseImage(&hsv);
		cvReleaseImage(&edge);
		cvReleaseImage(&edge1);
		cvReleaseImage(&imgTemp);
		cvDestroyAllWindows;

	}
}