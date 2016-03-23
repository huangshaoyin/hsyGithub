#include "mainwindow.h"
#include "ui_mainwindow.h"
#include<math.h>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    QObject::connect(ui->actionN,SIGNAL(triggered(bool)),this,SLOT(newFileSlot()));
    QObject::connect(ui->actionO,SIGNAL(triggered(bool)),this,SLOT(openFileSlot()));
}


MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::newFileSlot()
{
    if(ui->textEdit->document()->isModified())
    {
       // QDebug()<<"current file";
    }
                  else{
                  ui->textEdit->clear();
                  this->setWindowTitle("Untilse");
                  }

}
void MainWindow::openFileSlot()
{
   // QString fileName=QFileDialog::getOpenFileName(this,"open File",QDir::currentPath());
 QString fileName = QFileDialog::getOpenFileName(this, tr("File Dialog"), "E:", tr("pic file(* png * jpg * bmp)"));
  // IplImage *img = cvLoadImage("C:/1.png",1);
    //image = cv::imread(fileName.toAscii().data());
//fileName.toLocal8Bit();
    IplImage *pImg; //声明IplImage指针
   // pImg= cv::imread(fileName.toAscii().data());
  qDebug() << fileName;
    pImg=cvLoadImage(fileName.toLatin1().data(),1); //载入图像fileName.toLocal8Bit()
//qDebug() <<pImg->height;
   // deal(pImg);
   IplImage* histsrc = cvCreateImage(cvGetSize(pImg),pImg->depth,1);
   IplImage* src = cvCreateImage(cvGetSize(pImg),pImg->depth,1);
   IplImage* linesrc = cvCreateImage(cvGetSize(pImg),pImg->depth,1);
  // IplImage* dst = cvCreateImage(cvGetSize(pImg),pImg->depth,1);
    cvCvtColor(pImg,src,CV_BGR2GRAY);
    //dst=strengthImage_Histogram(src);
    histsrc=myhistf(src,1.5,600);
    linesrc=myline(histsrc);
    cvNamedWindow( "Image", 1 );//创建窗口
     cvNamedWindow( "Image1",1);//创建窗口
      //cvNamedWindow( "Image2",1);//创建窗口
      // cvNamedWindow( "Image1",1);//创建窗口
        //cvNamedWindow( "Image1",1);//创建窗口
    cvShowImage( "Image", pImg );//显示图像
    cvShowImage( "Image1", histsrc);//
    //cvShowImage( "Image2",linesrc);
     // cvShowImage( "Image1",dst);
    // cvShowImage( "Image1",dst);
    cvWaitKey(0); //等待按键
    cvDestroyWindow( "Image" );//
     cvDestroyWindow( "Image1" );
        // cvDestroyWindow( "Image2" );
             //cvDestroyWindow( "Image1" );
                 //cvDestroyWindow( "Image1" );
    cvReleaseImage( &pImg ); //释放图像
    cvReleaseImage( &src );
     cvReleaseImage( &histsrc);
      //cvReleaseImage( &linesrc);
    //return 0;
}
void MainWindow::deal(IplImage* pImg1)
{
    for (int i = 0; i < pImg1->height; i++)
        {
            for (int j = 0; j <pImg1->width; j++)
            {
                //方法一：使用cvGet2D()函数间接访CvScalar s = cvGet2D(img, i, j);
                //((uchar *)(img->imageData + i*img->widthStep))[j]=111;
                //cvSet2D(pImg1, i, j,0);
                //方法二：使用直接访问
               // ((uchar *)(pImg1->imageData + i*pImg1->widthStep))[j]=0;	//i和j的意义同上
               // ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 0]=111;
             // B ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 1]=112;
             // G ((uchar *)(img->imageData + i*img->widthStep))[j*img->nChannels + 2]=113; // R

            }
        }
}
/*
    1、图像增强
*/
//直方图均衡化图像增强，这里是针对单通道灰度图像的增强
IplImage* MainWindow::strengthImage_Histogram(IplImage* img)
{
    IplImage* dst=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
    int width = img->width;
    int height = img->height;
    int step = img->widthStep;
    double his[256] = {0};//灰度
    double p_hist[256]={0};//灰度比例
    double s_hist[256]={0};//累计灰度比例
    double total = img->width*img->height;//总像素数
    for(int i = 0; i < height; i++)
    {
        for(int j = 0; j < width; j++)
        his[((uchar*)img->imageData+i*step)[j]]++;
    }

    //p(r) = n(k)/n; k = 0、1、2。。。；
    //0<= r <= 1;n为总像素数，n(k)表示灰度k的像素数
    for(int i = 0; i < 256; i++)
    {
        p_hist[i] = (double)his[i]/total;
        if(i == 0)s_hist[i] = p_hist[i];
        else s_hist[i] = s_hist[i-1]+p_hist[i];
    }

    //图像增强
    for(int i = 0; i < img->height; i++)
    {
        for(int j = 0; j < img->width; j++)
        {
            ((uchar*)(i * dst->widthStep +dst->imageData))[j] = s_hist[((uchar*)(i * img->widthStep +img->imageData))[j]]*255+0.5;
        }
    }
    return dst;
}
 IplImage* MainWindow::myhistf(IplImage* img,double khist,double lowgray)
 {
     IplImage* dst=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
     int width = img->width;
     int height = img->height;
     int step = img->widthStep;
     int tempmax=0;
     int pos=0;
     //int mymin=257;
     int mymax=0;
     int locmp,mp,locnp,np,b,c,i,j;
     double l,rd,k1,const1,const2,const3;
     double imhist[256] = {0};//灰度
     double p_hist[256]={0};//灰度比例
     double s_hist[256]={0};//累计灰度比例
     //double total = img->width*img->height;//总像素数
     for(int i = 0; i < height; i++)
     {
         for(int j = 0; j < width; j++)
         {

               ((uchar*)dst->imageData+i*step)[j]=((uchar*)img->imageData+i*step)[j];
         }
     }


     for(int i = 0; i < height; i++)
     {
         for(int j = 0; j < width; j++)
         imhist[((uchar*)img->imageData+i*step)[j]]++;
     }
     for(int i=1;i<=256;i++)
     {
         if(imhist[i-1]>lowgray)
         {
             c=i;
             break;
         }
     }
     for(int i=256;i>=1;i--)
     {
         if(imhist[i-1]>lowgray)
         {
             b=i;
             break;
         }
     }
     for(i=c+1;i<=256;i++)
     {
         p_hist[i-1]=(imhist[i-1]-imhist[c-1])/(i-c);
     }
     for(i=1;i<=256;i++)
     {
         if(p_hist[i-1]>mymax)
         {
             locmp=i;
             mymax=p_hist[i-1];
         }
     }
     mymax=0;
     mp=imhist[locmp-1];
     for(i=1;i<b;i++)
     {
         s_hist[i-1]=-(imhist[i-1]-imhist[b-1])/(i-b);

     }
     for(i=1;i<=256;i++)
     {
         if(s_hist[i-1]>mymax)
         {
             locnp=i;
             mymax=s_hist[i-1];
         }
     }
     mymax=0;
     np=imhist[locnp-1];
     k1=(np-mp)/(locnp-locmp);
     for(i=locmp;i<=locnp;i++)
     {
         l=mp+k1*(i-locmp);
         rd=l-imhist[i-1];
         if(rd>tempmax)
         {
             tempmax=rd;
             pos=i;
         }
     }
     const1=log((pos/khist+1))/pos;
     const2=9/(255-pos);
     const3=1-pos*const2;
     for(int i = 0; i < height; i++)
     {
         for(int j = 0; j < width; j++)
         {
             if(((uchar*)dst->imageData+i*step)[j]<=pos)
             {
                 ((uchar*)dst->imageData+i*step)[j]=(int)(pow(2.718,((uchar*)img->imageData+i*step)[j]*const1)-1);

             }
             else
             {
                   ((uchar*)dst->imageData+i*step)[j]=(int)(pos/khist+(255-pos/khist)*log(const2*((uchar*)img->imageData+i*step)[j]+const3));
             }
         }
     }
     return dst;

 }
  IplImage* MainWindow::myline(IplImage* img)
 {   IplImage* dst=cvCreateImage(cvSize(img->width,img->height),img->depth,img->nChannels);
     int width = img->width;
     int height = img->height;
     int step = img->widthStep;
     int num=4,i,j,k=0,t=0,tem=0,num1=0,ind=0,line=0;
    // double gy1[height][width];
     double **gy1 ,**gy;
     gy1 = new double*[height] ;
      gy = new double*[height] ;
     // gradienty=new double*[height];

      double *gradienty=new double[height];
     for( i = 0 ; i <height; i ++ )
     {
       gy1[i] = new double[width] ;
       gy[i] = new double[width] ;
     }
    // gy1=(int (*)[width])malloc(height*width*sizeof(double));
     //double gy[height][width];
    // double gradienty[height];
     double max1=0,gdata[4][6]={0};
     for(i=0;i<height;i++)
     {
         for(j=0;j<width;j++)
         {
             gy[i][j]=0;
             gy1[i][j]=0;
         }
         gradienty[i]=0;
     }
     for(int i = 0; i < height; i++)
     {
         for(int j = 0; j < width; j++)
         {

               ((uchar*)dst->imageData+i*step)[j]=((uchar*)img->imageData+i*step)[j];
         }
     }
      for(int i = 0; i < height; i++)
      {
          for(int j = 0; j < width; j++)
          gy1[i][j]=(double)((uchar*)img->imageData+i*step)[j];
      }
     for(i=ceil(width/30);i<=(width-ceil(width/30));i++)
     {
         for(j=1;j<=height;j++)
         {
         gy[i-1][j-1]=gy1[i-1][j-1]-gy1[i][j-1];
         }
     }
      for(i=0;i<height;i++)
          {
              for(j=0;j<width;j++)
              {
                gradienty[i]=gradienty[i]+gy[i][j];

              }
          }
          for(i=1;i<height-1;i++)
          {
             if(gradienty[i]>gradienty[i-1]&&gradienty[i]>gradienty[i+1])
             {

                 gdata[k][1]=i;
                 gdata[k][2]=gradienty[i];
                 k++;
                   if(k>=num)
                       break;
             }
          }
          for(i=0;i<num;i++)
          {
              for(j=num-2;j>=0;j--)
              {
                  if(gdata[j+1][2]>gdata[j][2])
                  {
                      tem=gdata[j+1][2];
                      gdata[j+1][2]=gdata[j][2];
                      gdata[j][2]=tem;
                      tem=gdata[j+1][1];
                      gdata[j+1][1]=gdata[j][1];
                      gdata[j][1]=tem;
                  }

              }
          }

          for(i=1;i<=num;i++)
           {
              for(j=1;j<=gdata[i-1][1];j++)
              {
                  if(gradienty[j-1]<0)
                      num1++;
              }
              gdata[i-1][3]=num1/gdata[i-1][1];
              num1=0;
              for(j=1;j<=gdata[i-1][1];j++)
              {
                  gdata[i-1][4]=gdata[i-1][4]+j;
              }
          }
          for(i=0;i<num;i++)
          {
              if(gdata[i][4]>max1)
              {
                 max1=gdata[i][4];
              }

          }
          for(i=0;i<num;i++)
          {
              gdata[i][2]=gdata[i][2]/gdata[0][2];
              gdata[i][4]=1-gdata[i][4]/max1;
              gdata[i][5]=gdata[i][2]*0.8+gdata[i][3]+0.5*gdata[i][4];
          }
          max1=0;
          for(i=0;i<num;i++)
          {
              if(gdata[i][5]>max1)
              {
                  max1=gdata[i][5];
                  ind=i;

              }
          }
          line=gdata[ind][1];
          t=(int)line-floor(height/10);
          for(int i = 0; i < t; i++)
          {
              for(int j = 0; j < width; j++)
              ((uchar*)dst->imageData+i*step)[j]=0;
          }
          return dst;
 }
 int MainWindow::max(int* s,int len)
 {
     int i,temp=0,k;
     for(i=0;i<len;i++)
     {
         if(s[i]>temp)
         {
             temp=s[i];
             k=i;
         }
     }
     return k;
 }

