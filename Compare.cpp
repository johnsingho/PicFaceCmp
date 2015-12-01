/*******************************************************

  Copyright (c) 2015 Hanwang Tech, All rights reserved.

  例子程序。
  核心初始化，Handle初始化。
  人脸的定位，特征提取，特征比对。  1比1 比对， 1比N 比对。
*******************************************************/
#include "stdafx.h"
#include "Compare.h"
#include "comm/misc.h"

//例子样本
// #define EXAMPLE_FILE1 "\\Img-1-1.bin"//"1.bin";
// #define EXAMPLE_FILE2 "\\Img-1-2.bin"//"1.bin";
// #define EXAMPLE_FILE3 "\\Img-2-1.bin"//"1.bin";
// #define EXAMPLE_FILE4 "\\1-0.bin"//"1.bin";
// #define EXAMPLE_FILE5 "\\1-0-1.bin"//"1.bin";
//#define STR_DLL "D:\\Proc\\HwCompare.dll"
//#define STR_PATH "D:\\Proc"

//dll库所在位置和路径
#define STR_DLL "HWcompare.dll"
char   CurrentPath[MAX_PATH];   

//PICPIXEL picPixel[2];	
HW_HANDLE MyHandle = NULL;

typedef HWRESULT (*tagHWInitialD)( char *strName);
typedef HWRESULT (*tagHWReleaseD)();
typedef HWRESULT (*tagHWInitial)( HW_HANDLE *pHandle, char *strName);
typedef HWRESULT (*tagHWRelease)( HW_HANDLE *pHandle);
typedef HWRESULT (*tagHWDetectFaceKeyPoints)( HW_HANDLE Handle,
                            const unsigned char*pImg, 
                            int nImgWidth, int nImgHeight,
                            int* pnMaxFace, 
                            HWFaceInfo *pFaceInfo);

typedef HWRESULT (*tagHWSetLocationScale)( HW_HANDLE Handle, int iMinScale, int iScaleCover );
typedef HWRESULT (*tagHWGetLocationScale)( HW_HANDLE Handle, int *piMinScale, int *piScaleCover );
typedef HWRESULT (*tagHWSetLocationThreshold)( HW_HANDLE Handle,float fFace, float fEye );
typedef HWRESULT (*tagHWGetLocationThreshold)( HW_HANDLE Handle,float *pfFace, float *pfEye );
typedef HWRESULT (*tagHWGetFeatureSize)( HW_HANDLE Handle, int *piFtrSize );
typedef HWRESULT (*tagHWExtractFeature)( HW_HANDLE Handle,
                          const unsigned char* pImg, int nImgWidth, int nImgHeight,
                          HWFaceInfo *pFaceInfo,
						  unsigned char *pOutFeature);
typedef HWRESULT  (*tagHWCompareFeature)( HW_HANDLE Handle,
                            const unsigned char *pFeaA,
                            const unsigned char *pFeaB,
                            float *fScore);

tagHWInitialD MyInitD = NULL;
tagHWReleaseD MyReleaseD = NULL;
tagHWInitial MyInitH = NULL;
tagHWRelease MyReleaseH = NULL;
tagHWDetectFaceKeyPoints MyDetect = NULL;
tagHWSetLocationScale MySetScale = NULL;
tagHWGetLocationScale MyGetScale = NULL;
tagHWSetLocationThreshold MySetTh = NULL;
tagHWGetLocationThreshold MyGetTh = NULL;
tagHWGetFeatureSize   MyGetFtrSize = NULL;
tagHWExtractFeature   MyGetFtr = NULL;
tagHWCompareFeature   MyCompare = NULL;

HMODULE MyLib = NULL;

//得分的阈值，判断是否同一个人。需要根据实际情况调整。
//0.6 较宽松。常用于证件照与抓拍人脸这样的 时间跨度及光线差别较大的比对情况
//可设置0.6f 到0.9f 之间的数值。
//如果比对图片之间的光线环境等条件变化不大，则可把阈值调高。
const float TH_RESHOLD = 0.6f;
                          

int    initialCompare()
{
	int ret =-1;
    //识别核心初始化
    CString strCurDir = MakeModuleFilePath("");
    strncpy(CurrentPath, strCurDir, strCurDir.GetLength()-1);
	//GetCurrentDirectory(MAX_PATH,CurrentPath); 
    if( S_OK != InitEngine())
    {
//        printf("Init Failed\n");
        return ret;
    }
    //初始化一个Handle. 不同的线程可以初始化各自的Handle使用。
    if( S_OK != MyInitH( &MyHandle, CurrentPath))
    {
//        printf("Init Handle Failed\n");
        return ret;
    }
	else 
	{
//		printf("Init Handle Ok\n");
		ret=0;
	}
	return ret;
}

//===================================================
//识别核心初始化
 int     InitEngine()
{
    MyLib  = LoadLibrary( STR_DLL );
    if( MyLib == NULL )
    {
//        printf("Load Lib Failed\n");
        return S_FAIL;
    }
    MyInitD = (tagHWInitialD)GetProcAddress( MyLib, "HWInitialD");
    MyReleaseD = (tagHWReleaseD )GetProcAddress( MyLib, "HWReleaseD");
    MyInitH = (tagHWInitial)GetProcAddress( MyLib, "HWInitial");
    MyReleaseH = (tagHWRelease)GetProcAddress( MyLib, "HWRelease");
    MyDetect = (tagHWDetectFaceKeyPoints)GetProcAddress( MyLib, "HWDetectFaceKeyPoints");
    MySetScale = (tagHWSetLocationScale)GetProcAddress( MyLib, "HWSetLocationScale");
    MyGetScale = (tagHWGetLocationScale)GetProcAddress( MyLib, "HWGetLocationScale");
    MySetTh = (tagHWSetLocationThreshold)GetProcAddress( MyLib, "HWSetLocationThreshold");
    MyGetTh = (tagHWGetLocationThreshold)GetProcAddress( MyLib, "HWGetLocationThreshold");
    MyGetFtrSize = (tagHWGetFeatureSize)GetProcAddress( MyLib, "HWGetFeatureSize");
    MyGetFtr = (tagHWExtractFeature)GetProcAddress( MyLib, "HWExtractFeature");
    MyCompare = (tagHWCompareFeature)GetProcAddress( MyLib, "HWCompareFeature");

    if( MyInitD == NULL || MyReleaseD == NULL ||
        MyInitH == NULL || MyReleaseH == NULL ||
        MyDetect == NULL || MySetScale == NULL ||
        MyGetScale == NULL || MySetTh == NULL ||
        MyGetTh == NULL || MyGetFtrSize == NULL ||
        MyGetFtr == NULL || MyCompare == NULL )
    {
//        printf("Find Proc Address Failed\n");
        return S_FAIL;
    }
    if( S_OK != MyInitD(CurrentPath))
    {
//        printf("Init Failed\n");
        return S_FAIL;
    }
    return S_OK;
}

//释放识别核心。
void ReleaseEngine()
{
    if(MyHandle)
    {
        MyReleaseH( &MyHandle);//释放Handle
        MyHandle=NULL;
    }
    if( MyReleaseD != NULL )
    {
        MyReleaseD();
    }
    if( MyLib != NULL )
    {
        FreeLibrary( MyLib);
    }
}

int GetFeature( HW_HANDLE Handle, const unsigned char *pb, 
                      int iW, int iH, 
                      unsigned char *pbFtr,int faceSize)
{
    HWRESULT iRst;
    HWFaceInfo *pFaceInfos = new HWFaceInfo[faceSize];
	int i;
	int iFtrSize;
    MyGetFtrSize( MyHandle, &iFtrSize);

    iRst = MyDetect( Handle, pb, iW, iH, &faceSize, &pFaceInfos[0] );
    //MyDetect进行人脸定位，返回的MyInfo包含人脸定位的信息，
    //MyInfo.m_FaceRect 给出的人脸框矩形坐标，应用程序可依照这个信息在屏上画出相应小框。
    //显示人脸定位的结果。
    if( iRst == S_OK )
    {
        //根据定位信息求出人脸特征。
		for(i=0;i<faceSize;i++)
		{
			MyGetFtr( Handle, pb, iW, iH, &pFaceInfos[i], &pbFtr[iFtrSize*i] );		
		}
        delete[] pFaceInfos;
		return S_OK;
    }
    delete[] pFaceInfos;
    return S_FAIL;
}

// unsigned char * ReadAFile( char *str, int *piLen)
// {
//     FILE *pF;
//     int iLen;
//     unsigned char *pb;
// 
//     pF =fopen( str, "rb");
//     if( pF == NULL )
//     {
// //        printf("Open %s Failed\n", str );
//         *piLen = 0;
//         return NULL;
//     }
//     fseek( pF, 0, SEEK_END);
//     iLen = ftell( pF );
//     fseek( pF, 0, SEEK_SET);
//     pb = new BYTE[ iLen];
//     fread( pb, 1, iLen, pF );
//     fclose( pF );
//     *piLen = iLen;
//     return pb;
// }

//比较两张图片。1 比 1 比对。
//返回相似度
//
float  TestCompare1V1( HW_HANDLE MyHandle, PICPIXEL* pIDPic, PICPIXEL* pLivePic)
{
    unsigned char *pbImg;
    int iFtrSize;
    unsigned char *pbFtr1 = NULL, *pbFtr2 = NULL;
    float fScore=0,fScoreMax=0;
    const int MAX_TRY_FACES = 2;
    int i=0;
//     pF =fopen( "Ftr.bin","wb");

    MyGetFtrSize( MyHandle, &iFtrSize);
    pbFtr1 = new unsigned char[ iFtrSize];
    pbFtr2 = new unsigned char[ iFtrSize*MAX_TRY_FACES];

    pbImg = pIDPic->pixel;
    if( S_OK != GetFeature( MyHandle, pbImg, pIDPic->width, pIDPic->height, pbFtr1, 1))
    {
        goto CleanUp;
    }
    pbImg = NULL;

    pbImg = pLivePic->pixel;
    if( S_OK != GetFeature( MyHandle, pbImg, pLivePic->width, pLivePic->height, pbFtr2, MAX_TRY_FACES))
    {
        goto CleanUp;
    }
    pbImg = NULL;

	for(i=0;i<MAX_TRY_FACES;i++)
	{
		MyCompare( MyHandle, pbFtr1, &pbFtr2[iFtrSize*i], &fScore);
//		printf("Compare Score = %f\n", fScore);
		if(fScore>fScoreMax)   fScoreMax=fScore;
	}
//    if( fScoreMax > TH_RESHOLD )
//    {
//        printf("Maybe Same Person\n");
///    }
//     fwrite( pbFtr1, 1, iFtrSize, pF );
//     fwrite( pbFtr2, 1, iFtrSize, pF );

CleanUp:
//     if( pF!= NULL )
//     {
//         fclose( pF );
//     }
    if( pbFtr1 != NULL )
    {
        delete []pbFtr1;
    }
    if( pbFtr2 != NULL )
    {
        delete []pbFtr2;
    }
    if( pbImg != NULL )
    {
        pbImg =NULL;
    }
    return fScoreMax;
}

//读入前面两个图片特征，
//一个新图片与已有的图片特征比对。 1比N 比对。
// void    TestCompare1VN( HW_HANDLE MyHandle)
// {
//     int iLen;
//     int i;
//     int iFtrSize;
//     int iNum;
//     int iW, iH;
//     int iIdx;
//     float fMaxScore;
//     unsigned char *pbImg = NULL;
//     unsigned char *pbFtrLib = NULL, *pbFtr3 = NULL;
//     char *strPic3 = EXAMPLE_FILE2;
//     
//     pbFtrLib = ReadAFile( "Ftr.bin", &iLen);
//     MyGetFtrSize( MyHandle, &iFtrSize);
//     iNum = iLen / iFtrSize;//求出文件中保存了几个人脸的特征
// 
//     pbFtr3 = new unsigned char[ iFtrSize ];
//     pbImg = ReadAFile( strPic3, &i);
//     iW = 480;
//     iH = 640;
// 
//     if( S_OK != GetFeature( MyHandle, pbImg, iW, iH, pbFtr3,1))
//     {
// //        printf("Get Ftr From %s Failed\n", strPic3 );
//         goto CleanUp;
//     }
// 
//     //与保存的特征逐一比对，求出最好的特征
//     iIdx = -1;
//     fMaxScore = 0;
//     for( i=0; i<iNum; i++)
//     {
//         float fScore;
//         MyCompare( MyHandle, &pbFtrLib[i*iFtrSize], pbFtr3, &fScore);
//         if( fScore > fMaxScore)
//         {
//             fMaxScore = fScore;
//             iIdx = i;
//         }
//     }
// //    printf("Most Similar : %d  Score = %f\n", iIdx, fMaxScore);
//     if( fMaxScore > TH_RESHOLD )
//     {
// //        printf("Maybe Same Person\n");
//     }
// 
// CleanUp:
//     if( pbImg != NULL )
//     {
//         delete []pbImg;
//     }
//     if( pbFtr3 != NULL)
//     {
//         delete []pbFtr3;
//     }
//     if( pbFtrLib != NULL )
//     {
//         delete []pbFtrLib;
//     }
// 
// }



PICPIXEL* CreatePicPixel( int nW, int nH )
{
    PICPIXEL* pPic = new PICPIXEL;
    pPic->pixel = new unsigned char[nW*nH];
    if(!pPic->pixel){ delete pPic; return NULL;}
    pPic->width=nW;
    pPic->height=nH;
    return pPic;
}

void DeletePicPixel( PICPIXEL* pPicPixel)
{
    if(pPicPixel)
    {
        delete[] pPicPixel->pixel;
        delete pPicPixel;
    }
}

