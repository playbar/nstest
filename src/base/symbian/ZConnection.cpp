/*
 ============================================================================
 Name		: ZConnection.cpp
 Author	  : zhangchen
 Version	 : 1.0
 Copyright   : Your copyright notice
 Description : CZConnection implementation
 ============================================================================
 */

#include "ZConnection.h"
#include <cdbcols.h>
#include <CommDbConnPref.h>
#include <commdb.h>
#include "zcconvert.h"
#include "symbiandebug.h"

CZConnection* CZConnection::GetInstance()
	{
	static CZConnection* instance = NULL;
	if(!instance)
		instance = new CZConnection();
	return instance;
	}

void CZConnection::start()
	{
	int err=m_server.Connect();
	err = m_server.ShareAuto();
	
	//����RConnection������������������
	TCommDbConnPref pref;
	err = m_globalConnection.Open(m_server);							//�����ӹ������

	pref.SetDialogPreference(ECommDbDialogPrefPrompt);		//���õ���ѡ�񴰿�
#ifdef _SYMBIAN3
	pref.SetBearerSet(ECommDbBearerWLAN | ECommDbBearerGPRS | ECommDbBearerLAN |ECommDbBearerWcdma);
#else
	pref.SetBearerSet(ECommDbBearerWLAN | ECommDbBearerGPRS | ECommDbBearerCdma2000 | ECommDbBearerLAN |ECommDbBearerWcdma);						//�����������ӣ����ڳ��ؽ����
#endif
	err = m_globalConnection.Start(pref);
	if(err != KErrNone)
		{
		SYMBIANDEB1(DefaultLog,"connection error is %d\r\n",err);
		return;
		}

	//ȡ�������Ϣ
	m_globalConnection.GetIntSetting(_L("IAP\\Id"),m_iapId);
	SYMBIANDEB1(DefaultLog,"iap id is %d\r\n",m_iapId);
	TUint32 serviceId(0);
	HBufC* serviceType = HBufC::NewL(50);
	TPtr serviceTypePtr = serviceType->Des();
	m_globalConnection.GetIntSetting(_L("IAP\\IAPService"),serviceId);
	m_globalConnection.GetDesSetting(_L("IAP\\IAPServiceType"),serviceTypePtr);
//	SYMBIANDEB1(DefaultLog,"iap serviceid is %d\r\n",serviceId);
//	SYMBIANDEBS(DefaultLog,(char*)serviceTypePtr.PtrZ(),100);
	CCommsDatabase* db = CCommsDatabase::NewL(EDatabaseTypeIAP);
	TUint32 iProxyPort = 80;
	CCommsDbTableView* db_view = db->OpenViewOnProxyRecordLC(serviceId, serviceType->Des());
	delete serviceType;


	err = db_view->GotoFirstRecord();
	
	if(err == KErrNone)
		{
		db_view->ReadBoolL(TPtrC(PROXY_USE_PROXY_SERVER), m_bProxy);
		if(m_bProxy)
			{
			HBufC* proxyName = db_view->ReadLongTextLC(TPtrC(PROXY_SERVER_NAME));
			db_view->ReadUintL(TPtrC(PROXY_PORT_NUMBER), iProxyPort);
						if (proxyName)
						{
							m_address.Input(*proxyName);
							m_address.SetPort(iProxyPort);
							char *tempname = (char*)ZCConvert::ConvertToGBK(proxyName->Des().PtrZ());
							SYMBIANDEB2(DefaultLog, "proxy name:%s,port=%d\r\n",tempname, iProxyPort);
							delete tempname;
						}
						CleanupStack::PopAndDestroy(proxyName);
			}
		}
	CleanupStack::PopAndDestroy(db_view);
	delete db;
	}

void CZConnection::close()
	{
#ifdef _SYMBIAN3
#else
	m_globalConnection.Stop();
#endif
	m_globalConnection.Close();
	m_server.Close();
	}

void CZConnection::GetConnectionName(char*& name)
	{
	CCommsDatabase *db = CCommsDatabase::NewL(EDatabaseTypeIAP); //��ݿ�
//	CCommsDbTableView *db_view = db->OpenTableLC(TPtrC(OUTGOING_WCDMA));		//�򿪱?��ͼ
#ifdef _SYMBIAN3
	CCommsDbTableView *db_view = db->OpenIAPTableViewMatchingBearerSetLC(ECommDbBearerWLAN | ECommDbBearerGPRS | ECommDbBearerLAN |ECommDbBearerWcdma,ECommDbConnectionDirectionUnknown);
#else
	CCommsDbTableView *db_view = db->OpenIAPTableViewMatchingBearerSetLC(ECommDbBearerWLAN | ECommDbBearerGPRS | ECommDbBearerCdma2000 | ECommDbBearerLAN |ECommDbBearerWcdma,ECommDbConnectionDirectionUnknown);
#endif
	CleanupStack::Pop(db_view);
	int err = db_view->GotoFirstRecord();
	TUint32 iapId(0);
	do //(err == KErrNone)
		{
		db_view->ReadUintL(TPtrC(COMMDB_ID),iapId);		//ȡ�õ�ǰ��¼��iap��
		if(iapId == m_iapId)
			{
			TBuf<32> apn;
			db_view->ReadTextL(TPtrC(GPRS_APN),apn);	//ȡ�õ�ǰ��¼��apn�����Ϊ��ʵ�Ľ�����ʾ��
			name = (char*)ZCConvert::ConvertToGBK(apn.PtrZ());
			break;
			}
		}
	while(!db_view->GotoNextRecord());
	delete db_view;
	delete db;
	}
