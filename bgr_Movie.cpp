#include<string>
#include<iostream>
#include<highgui.h>
#include<fstream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
//#define SHOW_WINDOW   
using namespace std;
using namespace cv;

int load_bgr_file(Mat mat,int sn,char* path)
{
    static int i;
    char file_name[100];
    i++;
    sprintf(file_name, "%s/%06d.raw",path,sn);  
    
    ifstream  file_in; 
    file_in.open(file_name,ios::in|ios::binary);
    if (file_in.is_open()) 
    {
        file_in.seekg (0, ios::beg);
        file_in.read((char *)mat.data,mat.rows*mat.step1());
        file_in.close();
        return true;
    }
    else
        return false;
}

/**
 * @brief 视频文件转为BGR或者YUV文件
 * @param argv[1] bgr文件路径
 * @param argv[2] 图像缩小倍数
 * 输出文件在是new.avi
 */
int main(int argc, char ** argv)
{
    const char* videoOut = "new.avi";

    int scale=-1;
    int i;
    if(argc<3)
    {
        printf("please input path of bgr images and scale!\n ");
        return 0;
    }
    printf("The bgr file path  is ");
    printf(argv[1]);
    printf("\n");
    scale=atoi(argv[2]);
    if(scale<0||scale>100)
    {
        printf("Error:input a invalid scale argv!");
        return 0;
    }
   // cv::VideoWriter vout(videoOut, CV_FOURCC('X','2','6','4'), 25, 
    //    cv::Size(1920, 1080));
        cv::VideoWriter vout(videoOut, CV_FOURCC('D', 'I', 'V', '3'), 25, 
        cv::Size(1920/scale, 1080/scale));
    Mat srcImage(1080, 1920, CV_8UC3, Scalar(0, 0, 255)); 

    for(int i=1;i<100000;i++)
    {
        if(load_bgr_file(srcImage,i,argv[1]))
        {
#ifdef SHOW_WINDOW            
            namedWindow("Movie Player");
            imshow("Movie Player",srcImage);
#endif 
            Mat dstImg(1080/scale, 1920/scale, CV_8UC3, Scalar(0, 0, 255)); 
            resize(srcImage, dstImg, dstImg.size(), 0, 0, INTER_LINEAR);  
            //imshow("Movie Player",dstImg);
            vout.write(dstImg);
            printf("frame %d encode!\n",i);
        }
#ifdef SHOW_WINDOW   
        char c=waitKey(1);
        if(c==27)
            break;
#endif 
    }
#ifdef SHOW_WINDOW
    destroyWindow("Movie Player");
#endif
    vout.release();

}