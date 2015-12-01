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
    Rect  m_FaceRect;//������
    float m_afKeyPoint[81*2];//81�ؼ���
    KeyPos m_KeyPos;//14�ؼ���
}HWFaceInfo;
#endif

#ifdef __cplusplus
extern "C"
{
#endif


//��ʼ���ֵ�, strName ����HWDetect.dll����·������"D:\\Prog"
//������غ�����һ�Σ�����HWInitial����
HWRESULT HWInitialD( char *strName);
//�����˳�ǰ����һ��
HWRESULT HWReleaseD( );

//��ʼ��һ��HANDLE, strName ����NULL
//�������ж��ʵ��
HWRESULT HWInitial( HW_HANDLE *pHandle, char *strName);


HWRESULT HWRelease( HW_HANDLE *pHandle);

/**************************************************

    ������λ
    
*************************************************/


                 
HWRESULT HWDetectFaceKeyPoints( HW_HANDLE Handle,
                            const unsigned char*pImg, 
                            int nImgWidth, int nImgHeight,
                            int* pnMaxFace, 
                            HWFaceInfo *pFaceInfo);


//���ÿ��Զ�λ�����Ĵ�С��Χ
//iMinScale: 0~3�� 0 ʱ�����Զ�λ����С�������Դﵽ�������ľ���25�����ء�
//                 1 ʱ�����Զ�λ����С�������Դﵽ�������ľ���35������
//                 2 ʱ, ���Զ�λ����С�������Դﵽ�������ľ���45������
//                 3 ʱ, ���Զ�λ����С�������Դﵽ�������ľ���64������
//iScaleCover:5~9�� ���ÿ��Զ�λ�������Ĵ�С����仯��Χ��iMinScale + iScaleCover <=9
//                  5 ʱ���ɶ�λ����������۾�����С�����۾��3.3����
//                  6 ʱ���ɶ�λ����������۾�����С�����۾��4.5����
//                  7 ʱ���ɶ�λ����������۾�����С�����۾��6����
//                  8 ʱ���ɶ�λ����������۾�����С�����۾��8.2����
//                  9 ʱ���ɶ�λ����������۾�����С�����۾��11����
//����ע��iMinScale + iScaleCover <=9
//Ĭ������: iMinScale = 1, iScaleCover = 7.
//iMinScaleԽС����Ҫ����ʱ��Խ����iScaleCoverԽ�󣬻���ʱ��Խ����
//����������ֵ������Χ���ڲ��ָ�Ĭ�����á�
//return: S_OK, S_FAIL
HWRESULT HWSetLocationScale( HW_HANDLE Handle, int iMinScale, int iScaleCover );

//ȡ��Scale�趨ֵ��
//return : S_OK, S_FAIL
HWRESULT HWGetLocationScale( HW_HANDLE Handle, int *piMinScale, int *piScaleCover );

//���ö�λ��ֵ��
//���ú��ķ��ص÷ִ�����ֵ�������۾������Ժ˰汾��Ĭ���۾���ֵ0.3,����ֵ0.3
//��ֵ���÷�Χ��fFace : 0.0f ~ 1.0f, fEye: 0.0f ~ 1.0f
//��ֵԽ������Խ�ϸ�
//return : S_OK, S_FAIL
HWRESULT HWSetLocationThreshold( HW_HANDLE Handle,float fFace, float fEye );

//return : S_OK, S_FAIL
HWRESULT HWGetLocationThreshold( HW_HANDLE Handle,float *pfFace, float *pfEye );
                               

/**************************************************

    �����ͱȶ�

/*************************************************/
//return : ���������ֽڸ�����
HWRESULT HWGetFeatureSize( HW_HANDLE Handle, int *piFtrSize );


//��ȡ������
//pImg[in] ����ͼƬ
//nImgWidth, nImgHeight[in] ��ȸ߶�
//pFaceInfo [in]һ��������Ϣ
//pOutFeature [out]��������������������ȼ�HWGetFeatureSize�� ��Ҫ�ⲿ����á�
//return : S_OK. other failed
HWRESULT HWExtractFeature( HW_HANDLE Handle,
                          const unsigned char* pImg, int nImgWidth, int nImgHeight,
                          HWFaceInfo *pFaceInfo,
						  unsigned char *pOutFeature);


//���ڵ����Ƚ�����ͼƬ�������������ԡ�ע����÷־�ʶ���ܽ�����
//pFeaA[in] ������
//pFeaB[in] ������
//fScore[out] �����Զ���ֵ��0~1.0 ��Խ��Խ���ơ�
//return : S_OK. other failed
HWRESULT  HWCompareFeature( HW_HANDLE Handle,
                            const unsigned char *pFeaA,
                            const unsigned char *pFeaB,
                            float *fScore);
                               

#ifdef __cplusplus
}
#endif

#endif //HWFACERECSDK_H