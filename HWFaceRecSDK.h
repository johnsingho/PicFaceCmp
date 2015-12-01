#ifndef HWFACERECSDK_H
#define HWFACERECSDK_H

typedef long HWRESULT;
typedef void*  HW_HANDLE;
#ifndef  S_OK
#define S_OK   0
#endif
#define S_FAIL 1

#define LEFT_EYE   6
#define RIGHT_EYE  14

#ifndef  __RECT
#define  __RECT
typedef struct tagARect
{
	int top;
	int bottom;
	int left;
	int right;
}Rect;
#endif

#ifndef __POS
#define __POS
typedef struct  tagPos
{
	int col;
	int row;
	int width;
	int confi;
}Pos;
#endif

#ifndef __KEYPOS
#define __KEYPOS
typedef struct tagKeyPos
{
	Pos face;
	Pos leftEye;
	Pos rightEye;
	Pos leftUpperEye;
	Pos rightUpperEye;
	Pos leftleftEye;
	Pos leftrightEye;
	Pos rightleftEye;
	Pos rightrightEye;
	Pos leftNostril;
	Pos rightNostril;
	Pos nosePoint;
	Pos leftMouth;
	Pos rightMouth;
}KeyPos;
#endif
#ifndef HW_FACE_INFO
#define HW_FACE_INFO
typedef struct tagHWFaceInfo{
    Rect  m_FaceRect;//人脸框
    float m_afKeyPoint[81*2];//81关键点
    KeyPos m_KeyPos;//14关键点
}HWFaceInfo;
#endif

#ifdef __cplusplus
extern "C"
{
#endif


//初始化字典, strName 输入HWDetect.dll所在路径，如"D:\\Prog"
//程序加载后运行一次，先于HWInitial运行
HWRESULT HWInitialD( char *strName);
//程序退出前运行一次
HWRESULT HWReleaseD( );

//初始化一个HANDLE, strName 输入NULL
//可以运行多个实例
HWRESULT HWInitial( HW_HANDLE *pHandle, char *strName);


HWRESULT HWRelease( HW_HANDLE *pHandle);

/**************************************************

    人脸定位
    
*************************************************/


                 
HWRESULT HWDetectFaceKeyPoints( HW_HANDLE Handle,
                            const unsigned char*pImg, 
                            int nImgWidth, int nImgHeight,
                            int* pnMaxFace, 
                            HWFaceInfo *pFaceInfo);


//设置可以定位的脸的大小范围
//iMinScale: 0~3。 0 时，可以定位的最小人脸可以达到两眼中心距离25个像素。
//                 1 时，可以定位的最小人脸可以达到两眼中心距离35个像素
//                 2 时, 可以定位的最小人脸可以达到两眼中心距离45个像素
//                 3 时, 可以定位的最小人脸可以达到两眼中心距离64个像素
//iScaleCover:5~9。 设置可以定位的人脸的大小级别变化范围。iMinScale + iScaleCover <=9
//                  5 时，可定位的最大人脸眼距是最小人脸眼距的3.3倍。
//                  6 时，可定位的最大人脸眼距是最小人脸眼距的4.5倍。
//                  7 时，可定位的最大人脸眼距是最小人脸眼距的6倍。
//                  8 时，可定位的最大人脸眼距是最小人脸眼距的8.2倍。
//                  9 时，可定位的最大人脸眼距是最小人脸眼距的11倍。
//设置注意iMinScale + iScaleCover <=9
//默认设置: iMinScale = 1, iScaleCover = 7.
//iMinScale越小，需要花的时间越长；iScaleCover越大，花得时间越长。
//如果输入的数值超出范围，内部恢复默认设置。
//return: S_OK, S_FAIL
HWRESULT HWSetLocationScale( HW_HANDLE Handle, int iMinScale, int iScaleCover );

//取得Scale设定值。
//return : S_OK, S_FAIL
HWRESULT HWGetLocationScale( HW_HANDLE Handle, int *piMinScale, int *piScaleCover );

//设置定位阈值。
//设置核心返回得分大于阈值的脸和眼睛，线性核版本　默认眼睛阈值0.3,脸阈值0.3
//阈值设置范围：fFace : 0.0f ~ 1.0f, fEye: 0.0f ~ 1.0f
//阈值越大，限制越严格。
//return : S_OK, S_FAIL
HWRESULT HWSetLocationThreshold( HW_HANDLE Handle,float fFace, float fEye );

//return : S_OK, S_FAIL
HWRESULT HWGetLocationThreshold( HW_HANDLE Handle,float *pfFace, float *pfEye );
                               

/**************************************************

    特征和比对

/*************************************************/
//return : 特征长度字节个数。
HWRESULT HWGetFeatureSize( HW_HANDLE Handle, int *piFtrSize );


//提取特征。
//pImg[in] 输入图片
//nImgWidth, nImgHeight[in] 宽度高度
//pFaceInfo [in]一个人脸信息
//pOutFeature [out]输出特征串。特征串长度见HWGetFeatureSize， 需要外部申请好。
//return : S_OK. other failed
HWRESULT HWExtractFeature( HW_HANDLE Handle,
                          const unsigned char* pImg, int nImgWidth, int nImgHeight,
                          HWFaceInfo *pFaceInfo,
						  unsigned char *pOutFeature);


//用于单独比较两张图片的特征串相似性。注意其得分拒识功能较弱。
//pFeaA[in] 特征串
//pFeaB[in] 特征串
//fScore[out] 相似性度量值，0~1.0 ，越大越相似。
//return : S_OK. other failed
HWRESULT  HWCompareFeature( HW_HANDLE Handle,
                            const unsigned char *pFeaA,
                            const unsigned char *pFeaB,
                            float *fScore);
                               

#ifdef __cplusplus
}
#endif

#endif //HWFACERECSDK_H