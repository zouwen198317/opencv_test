#pragma once
#include <string>
#include <list>
#include <iostream>
#include <fstream>
#include <opencv2\opencv.hpp>

#include "Poco/Runnable.h"
#include "Poco/Exception.h"

#include "Poco/AutoPtr.h"  
#include "Poco/Logger.h"
#include "Poco/PatternFormatter.h"
#include "Poco/FormattingChannel.h"
#include "Poco/FileChannel.h"
#include "Poco/Message.h"

#include "Poco/DirectoryIterator.h"
#include "Poco/File.h"
#include "Poco/Path.h"

#include "Poco/JSON/JSON.h"
#include "Poco/JSON/Parser.h"
#include "Poco/Dynamic/Var.h"
#include "Poco/JSON/Object.h"

#include "Poco/AutoPtr.h"  
#include "Poco/Util/IniFileConfiguration.h" 

#include "Poco/Random.h"

#include "Poco/MD5Engine.h"
#include "Poco/DigestStream.h"
#include "Poco/StreamCopier.h"

#include "Poco/LocalDateTime.h"

#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include "Poco/URI.h"
#include "Poco/Net/NetException.h"
#include "Poco/Net/TCPServer.h"

#include "zip.h"
#include "unzip.h"
#include "MyCodeConvert.h"


#include "zbar.h"   



#define PIC_RECTIFY_TEST	//ͼ����ת��������
#define WarpAffine_TEST		//����任����

#define  MSG_ERR_RECOG	(WM_USER + 110)

#define SOFT_VERSION	_T("1.0")
#define MAX_DLG_WIDTH	1024
#define MAX_DLG_HEIGHT	768

#define SAFE_RELEASE(pObj)	if(pObj){delete pObj; pObj = NULL;}
#define SAFE_RELEASE_ARRY(pObj) if(pObj) {delete[] pObj; pObj = NULL;}

// extern CString				g_strCurrentPath;
// extern std::string			g_strPaperSavePath;
// extern std::string			g_strModelSavePath;
// extern Poco::Logger*		g_pLogger;
// extern int					g_nExitFlag;
// extern float				g_fSamePercent;		//�ж�У�������Ƿ���ͼ�ٷֱ�

typedef enum CPType
{
	UNKNOWN = 0,
	Fix_CP,			//���㣬���������
	H_HEAD,			//ˮƽͬ��ͷ
	V_HEAD,			//��ֱͬ��ͷ
	ABMODEL,		//AB����
	COURSE,			//��Ŀ
	QK_CP,			//ȱ��
	GRAY_CP,		//�Ҷ�У��
	WHITE_CP,		//��У��
	SN,				//������
	OMR				//ѡ��������
};

typedef struct _RectInfo_
{
	CPType		eCPType;						//У�������
	int			nThresholdValue;				//�˾���ʶ��ʱҪ��ı�׼ֵ�����ﵽ�ĻҶ�ֵ�ķ�ֵ
	float		fStandardValue;					//�˾���ʶ��ʱ����ֵ
	float		fRealValue;						//
	float		fStandardValuePercent;			//�˾���Ҫ��ı�׼ֵ���������ﵽ�����ĻҶ�ֵ�ı���
	float		fRealValuePercent;				//�˾���ʵ�ʵ�ֵ(�Ҷ�ֵ)
	int			nHItem;							//�ڼ���ˮƽͬ��ͷ
	int			nVItem;							//�ڼ�����ֱͬ��ͷ
	int			nSnVal;							//��ʶ׼��֤������ 0-9
	int			nTH;							//���
	int			nAnswer;						//��ѭ�����ڵڼ����𰸣���1-A,2-B,3-C,4-D,5-E,6-F...
	int			nSingle;						//0-��ѡ��1-��ѡ
	int			nRecogFlag;						//ʶ���ʶ��ʶ��SNʱ--ʶ�𿼺�˳����ѡ���Ŀ��Ŵ��ڱ�ʶֵ��ʶ��OMRʱ--ʶ�����˳����ѡ����OMR���ô��ڵı�ʶֵ
//	cv::Point	ptFix;
	cv::Rect	rt;
//	cv::Rect	rtFix;
	_RectInfo_()
	{
		eCPType = UNKNOWN;
		nSnVal = -1;
		nHItem = -1;
		nVItem = -1;
		nTH = -1;
		nAnswer = -1;
		nRecogFlag = 0;
		nSingle = 0;
		nThresholdValue = 0;
		fRealValuePercent = 0.0;
		fStandardValuePercent = 0.0;
		fStandardValue = 0.0;
		fRealValue = 0.0;
//		ptFix = cv::Point(0, 0);
	}
}RECTINFO,*pRECTINFO;

typedef std::list<RECTINFO> RECTLIST;			//����λ���б�

typedef struct _OmrQuestion_			//��Ŀ
{
	int nTH;
	int nSingle;						//0-��ѡ��1-��ѡ
	std::string strRecogVal;			//ʶ������A��B��C...
	RECTLIST	lSelAnswer;				//ѡ���б�
	_OmrQuestion_()
	{
		nTH = -1;
		nSingle = 0;
	}
}OMR_QUESTION, *pOMR_QUESTION;
typedef std::list<OMR_QUESTION> OMRLIST;

typedef struct _OmrResult_
{
	int		nTH;				//���
	int		nSingle;			//0-��ѡ��1-��ѡ
	int		nDoubt;				//0-�޻���, 1-�л���
	std::string strRecogVal;	//ʶ������A��B��C...
	std::string strRecogVal2;
	RECTLIST	lSelAnswer;				//ѡ���б�
	_OmrResult_()
	{
		nDoubt = 0;
		nTH = -1;
		nSingle = 0;
	}
}OMR_RESULT, *pOMR_RESULT;
typedef std::list<OMR_RESULT> OMRRESULTLIST;

typedef struct _SN_
{
	int nItem;			//�ڼ�λ��
	int nRecogVal;		//ʶ��Ľ��
	RECTLIST	lSN;
	_SN_()
	{
		nItem = -1;
	}
// 	~_SN_()
// 	{
// 		lSNDETAIL::iterator itSN = lSN.begin();
// 		for (; itSN != lSN.end();)
// 		{
// 			pSN_DETAIL pSN = *itSN;
// 			itSN = lSN.erase(itSN);
// 			SAFE_RELEASE(pSN);
// 		}
// 	}
}SN_ITEM, *pSN_ITEM;
typedef std::list<pSN_ITEM> SNLIST;

typedef struct _PaperModel_
{
	int			nPaper;					//��ʶ��ģ�����ڵڼ����Ծ�
	CString		strModelPicName;		//ģ��ͼƬ����
	cv::Rect	rtHTracker;
	cv::Rect	rtVTracker;
	cv::Rect	rtSNTracker;
	SNLIST		lSNInfo;				//SN��Ϣ
	RECTLIST	lSelHTracker;			//ѡ���ˮƽͬ��ͷ����
	RECTLIST	lSelVTracker;			//ѡ��Ĵ�ֱͬ��ͷ����
	RECTLIST	lSelFixRoi;				//ѡ�񶨵��ROI�ľ����б�����ѡ����Ĵ���ο�
	OMRLIST		lOMR2;
	RECTLIST	lFix;					//�����б�
	RECTLIST	lH_Head;				//ˮƽУ����б�
	RECTLIST	lV_Head;				//��ֱͬ��ͷ�б�
	RECTLIST	lABModel;				//����У���
	RECTLIST	lCourse;				//��ĿУ���
	RECTLIST	lQK_CP;					//ȱ��У���
	RECTLIST	lGray;					//�Ҷ�У���
	RECTLIST	lWhite;					//�հ�У���
	~_PaperModel_()
	{
		SNLIST::iterator itSn = lSNInfo.begin();
		for (; itSn != lSNInfo.end();)
		{
			pSN_ITEM pSNItem = *itSn;
			itSn = lSNInfo.erase(itSn);
			SAFE_RELEASE(pSNItem);
		}
	}
}PAPERMODEL,*pPAPERMODEL;

typedef struct _Model_
{
	int			nEnableModify;			//ģ���Ƿ�����޸�
	int			nPicNum;				//ͼƬ����
	int			nABModel;				//�Ƿ���AB��ģʽ
	int			nHasHead;				//�Ƿ���ˮƽ�ʹ�ֱͬ��ͷ
	CString		strModelName;			//ģ������

	std::vector<pPAPERMODEL> vecPaperModel;	//�洢ÿһҳ�Ծ���ģ����Ϣ
	_Model_()
	{
		nPicNum = 0;
		nABModel = 0;
		nHasHead = 1;
	}
	~_Model_()
	{
		std::vector<pPAPERMODEL>::iterator itModel = vecPaperModel.begin();
		for (; itModel != vecPaperModel.end();)
		{
			pPAPERMODEL pModel = *itModel;
			itModel = vecPaperModel.erase(itModel);
			SAFE_RELEASE(pModel);
		}
	}
}MODEL, *pMODEL;
typedef std::list<pMODEL> MODELLIST;	//ģ���б�


typedef struct _PicInfo_				//ͼƬ��Ϣ
{
	bool			bRecoged;		//�Ƿ��Ѿ�ʶ���
	bool			bFindIssue;		//�Ƿ��ҵ������
//	bool			bImgOpen;		//�Ծ�ͼƬ�Ƿ��Ѿ��򿪣����˾Ͳ���Ҫ�ٴδ�
//	cv::Point		ptModelFix;		//ģ��Ķ���
//	cv::Point		ptFix;			//����λ��
	void*			pPaper;			//�����Ծ�����Ϣ
	cv::Rect		rtFix;			//�������
	std::string		strPicName;		//ͼƬ����
	std::string		strPicPath;		//ͼƬ·��
	RECTLIST		lFix;			//�����б�
	RECTLIST		lNormalRect;	//ʶ�������������λ��
	RECTLIST		lIssueRect;		//ʶ������������Ծ��������λ�ã�ֻҪ���������Ͳ�������һҳ��ʶ��
// 	cv::Mat			matSrc;
// 	cv::Mat			matDest;
	_PicInfo_()
	{
		bRecoged = false;
		bFindIssue = false;
		pPaper = NULL;
//		ptFix = cv::Point(0, 0);
//		ptModelFix = cv::Point(0, 0);
//		bImgOpen = false;
	}
}ST_PicInfo, *pST_PicInfo;
typedef std::list<pST_PicInfo> PIC_LIST;	//ͼƬ�б�����

typedef struct _PaperInfo_
{
	bool		bIssuePaper;		//�Ƿ��������Ծ�
	int			bQKFlag;			//ȱ����ʶ
	pMODEL		pModel;				//ʶ���ѧ���Ծ����õ�ģ��
	void*		pPapers;			//�������Ծ�����Ϣ
	void*		pSrcDlg;			//��Դ�������ĸ����ڣ�ɨ��or�����Ծ�����
	std::string strStudentInfo;		//ѧ����Ϣ	
	std::string strSN;
	
	OMRRESULTLIST		lOmrResult;			//OMRRESULTLIST
	PIC_LIST	lPic;
	_PaperInfo_()
	{
		bIssuePaper = false;
		bQKFlag = false;
		pModel = NULL;
		pPapers = NULL;
		pSrcDlg = NULL;
	}
	~_PaperInfo_()
	{
		PIC_LIST::iterator itPic = lPic.begin();
		for (; itPic != lPic.end();)
		{
			pST_PicInfo pPic = *itPic;
			SAFE_RELEASE(pPic);
			itPic = lPic.erase(itPic);
		}		
	}
}ST_PaperInfo, *pST_PaperInfo;		//�Ծ���Ϣ��һ��ѧ����Ӧһ���Ծ���һ���Ծ������ж��ͼƬ
typedef std::list<pST_PaperInfo> PAPER_LIST;	//�Ծ��б�

typedef struct _PapersInfo_				//�Ծ�����Ϣ�ṹ��
{
	int		nPaperCount;				//�Ծ������Ծ�������(ѧ����)
	int		nRecogErrCount;				//ʶ������Ծ�����
	Poco::FastMutex fmlPaper;			//���Ծ��б���д��
	Poco::FastMutex fmlIssue;			//�������Ծ��б���д��
	std::string  strPapersName;			//�Ծ�������
	std::string	 strPapersDesc;			//�Ծ�����ϸ����

	PAPER_LIST	lPaper;					//���Ծ������Ծ��б�
	PAPER_LIST	lIssue;					//���Ծ�����ʶ����������Ծ��б�
	_PapersInfo_()
	{
		nPaperCount = 0;
		nRecogErrCount = 0;
	}
	~_PapersInfo_()
	{
		fmlPaper.lock();
		PAPER_LIST::iterator itPaper = lPaper.begin();
		for (; itPaper != lPaper.end();)
		{
			pST_PaperInfo pPaper = *itPaper;
			SAFE_RELEASE(pPaper);
			itPaper = lPaper.erase(itPaper);
		}
		fmlPaper.unlock();
		fmlIssue.lock();
		PAPER_LIST::iterator itIssuePaper = lIssue.begin();
		for (; itIssuePaper != lIssue.end();)
		{
			pST_PaperInfo pPaper = *itIssuePaper;
			SAFE_RELEASE(pPaper);
			itIssuePaper = lIssue.erase(itIssuePaper);
		}
		fmlIssue.unlock();
	}
}PAPERSINFO, *pPAPERSINFO;
typedef std::list<pPAPERSINFO> PAPERS_LIST;		//�Ծ����б�

typedef struct _RecogTask_
{
	int		nPic;						//���Ծ�����ģ��ĵڼ���
	pMODEL pModel;						//ʶ���õ�ģ��
	std::string strPath;	
	pST_PaperInfo	pPaper;				//��Ҫʶ����Ծ�
}RECOGTASK, *pRECOGTASK;
typedef std::list<pRECOGTASK> RECOGTASKLIST;	//ʶ�������б�

extern Poco::FastMutex		g_fmRecog;		//ʶ���̻߳�ȡ������
extern RECOGTASKLIST		g_lRecogTask;	//ʶ�������б�

extern Poco::FastMutex		g_fmPapers;		//�����Ծ����б���������
extern PAPERS_LIST			g_lPapers;		//���е��Ծ�����Ϣ


//TCP��������
typedef struct _TcpTask_
{
	unsigned short usCmd;
	int		nPkgLen;
	char	szSendBuf[1024];
}TCP_TASK, *pTCP_TASK;
typedef std::list<pTCP_TASK> TCP_TASKLIST;

extern Poco::FastMutex		g_fmTcpTaskLock;
extern TCP_TASKLIST			g_lTcpTask;

//�ļ��ϴ�����
typedef struct _SendTask_
{
	std::string strFileName;
	std::string strPath;
}SENDTASK, *pSENDTASK;
typedef std::list<pSENDTASK> SENDTASKLIST;	//ʶ�������б�

extern Poco::FastMutex		g_fmSendLock;
extern SENDTASKLIST			g_lSendTask;

typedef struct _ExamSubjects_
{
	int			nSubjID;		//���Կ�ĿID
	int			nSubjCode;		//���Կ�Ŀ����
	std::string strSubjName;	//���Կ�Ŀ����
	std::string strModelName;	//ɨ������ģ������
}EXAM_SUBJECT, *pEXAM_SUBJECT;
typedef std::list<EXAM_SUBJECT> SUBJECT_LIST;

typedef struct _examInfo_
{
	int			nExamID;			//����ID
	int			nExamGrade;			//�꼶
	int			nExamState;			//����״̬
	std::string strExamName;		//��������
	std::string strExamTypeName;	//������������
	std::string strGradeName;		//�꼶����
	SUBJECT_LIST lSubjects;			//��Ŀ�б�
}EXAMINFO, *pEXAMINFO;
typedef std::list<EXAMINFO> EXAM_LIST;

extern EXAM_LIST	g_lExamList;


extern Poco::Event			g_eTcpThreadExit;
extern Poco::Event			g_eSendFileThreadExit;
extern Poco::Event			g_eFileUpLoadThreadExit;

int		GetRectInfoByPoint(cv::Point pt, CPType eType, pPAPERMODEL pPaperModel, RECTINFO*& pRc);
bool	ZipFile(CString strSrcPath, CString strDstPath, CString strExtName = _T(".zip"));
bool	UnZipFile(CString strZipPath);
pMODEL	LoadModelFile(CString strModelPath);		//����ģ���ļ�
bool	SortByArea(cv::Rect& rt1, cv::Rect& rt2);		//���������
bool	SortByPositionX(RECTINFO& rc1, RECTINFO& rc2);
bool	SortByPositionY(RECTINFO& rc1, RECTINFO& rc2);
bool	SortByPositionX2(cv::Rect& rt1, cv::Rect& rt2);
bool	SortByPositionY2(cv::Rect& rt1, cv::Rect& rt2);
bool	SortByPositionXYInterval(cv::Rect& rt1, cv::Rect& rt2);
bool	GetPosition(RECTLIST& lFix, RECTLIST& lModelFix, cv::Rect& rt, int nPicW = 0, int nPicH = 0);
std::string calcFileMd5(std::string strPath);
void	CopyData(char *dest, const char *src, int dataByteSize, bool isConvert, int height);
bool	PicRectify(cv::Mat& src, cv::Mat& dst, cv::Mat& rotMat);
int		FixWarpAffine(int nPic, cv::Mat& matCompPic, RECTLIST& lFix, RECTLIST& lModelFix);		//������з���任
int		FixwarpPerspective(int nPic, cv::Mat& matCompPic, RECTLIST& lFix, RECTLIST& lModelFix);	//����͸�ӱ任
int		PicTransfer(int nPic, cv::Mat& matCompPic, RECTLIST& lFix, RECTLIST& lModelFix);




//---------	����	--------------
using namespace std;
using namespace zbar;  //����zbar���ƿռ�    
using namespace cv;
//zbar�ӿ�
string ZbarDecoder(Mat img, string& strTypeName);

//�Զ�ֵͼ�����ʶ�����ʧ����������ж���ʶ��
string GetQRInBinImg(Mat binImg, string& strTypeName);

//main function
string GetQR(Mat img, string& strTypeName);

//--------------------------------