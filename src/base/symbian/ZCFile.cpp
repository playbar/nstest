#include <f32file.h>
#include "ZCFile.h"

ZCFile::ZCFile()
{

}

ZCFile::~ZCFile()
{

}
//////////////////////////////////////////////////////////////
//删除文件
int ZCFile::ZCDeleteFile(const TDesC& fileName)
{
	//连接文件服务器，建立文件会话
	fsession.Connect();
	int err;
	
	err=fsession.Delete(fileName);
	//关闭文件会话
	fsession.Close();
	return err;
}

/////////////////////////////////////////////////////////////
//删除目录,目录下必须为空
int ZCFile::ZCDeleteDirectory(const TDesC& strPath)
{
	//关闭文件会话
	fsession.Connect();
	int err;

		err=fsession.RmDir(strPath);
	//关闭文件会话
	fsession.Close();
	return err;
}

////////////////////////////////////////////////////////////
//创建目录
int ZCFile::ZCCreateDirectory(const TDesC& strPath)
{
	fsession.Connect();
	int err;

		err=fsession.MkDir(strPath);
	fsession.Close();
	return err;
}

///////////////////////////////////////////////////////////
//得到驱动器列表
void ZCFile::ZCGetDriverList(TDriveList &drivelist)
	{
	fsession.Connect();
	fsession.DriveList(drivelist);        //得到当前驱动器列表
	fsession.Close();
	}

///////////////////////////////////////////////////////////
//得到盘符
void ZCFile::ZCGetDrverChar(int aDriver,TUint &aChar)
	{
	fsession.Connect();
	TChar buf;
	fsession.DriveToChar(aDriver,buf);
	fsession.Close();
	aChar=(TUint)buf;
	}

///////////////////////////////////////////////////////////
//得到驱动器名
void ZCFile::ZCGetDriverName(int aDriver,TDes &aDriverName)
	{
	fsession.Connect();
	fsession.GetDriveName(aDriver,aDriverName);
	fsession.Close();
	}

////////////////////////////////////////////////////////////
//设置驱动器名
void ZCFile::ZCSetDriverName(int aDriver,const TDes &aDriverName)
	{
	fsession.Connect();
	fsession.SetDriveName(aDriver,aDriverName);
	fsession.Close();
	}
//void ZCFile::ZCGetDriverList(char** drvers,char** name)
//{
//
//	char dick[]="C:\\";
//	char str[]="(C:)";
//	TDriveList drivelist;  //定义驱动器列表数组
//	TChar driveLetter;     //驱动器盘符
//	TInt driveNumber=EDriveA;    //定义驱动器列表索引号
//
//	User::LeaveIfError(fsession.DriveList(drivelist));  //得到当前驱动器列表
//		for(;driveNumber<=EDriveZ;driveNumber++)
//		{    
//		if (drivelist[driveNumber])
//			{
//				char strName[64];
//				TPtr buf(strName);     
//			User::LeaveIfError(fsession.DriveToChar(driveNumber,driveLetter));
//			User::LeaveIfError(fsession.GetDriveName(driveNumber,buf));
////			console->Printf(KDrive, TUint(driveLetter), drivelist[driveNumber]);
//			dick[0]=driveLetter;
//			*drvers=new char[strlen(dick)];
//			strcpy(*drvers++,dick);
//			str[1]=dick[0];
//			strcat(strName,str);
//			*name=new char[strlen(strName)];
//			strcpy(*name++,strName);
//			}
//		}
//}


////////////////////////////////////////////////////////////
//查看是否是文件夹 成功返回大于0，失败返回小于0
int ZCFile::ZCIsDir(const TDesC& fileName)
	{
	fsession.Connect();
		TEntry theEntry;
		int err;
		err=fsession.Entry(fileName,theEntry);
	fsession.Close();
		if(err == KErrNone)
			{
				return theEntry.IsDir();
			}
		else
			return err;
	}
	
////////////////////////////////////////////////////////////
//取得文件或文件夹的属性
int ZCFile::ZCGetEntry(const TDesC& fileName,TEntry&entry)
{
	//TEntry theEntry;
	fsession.Connect();
	int err;
	err = fsession.Entry(fileName,entry);
	fsession.Close();
	if(err!=KErrNone) return 0;
	return 1;
}


////////////////////////////////////////////////////////////
//取得指定目录下的所有项,以CDir指针接受返回值，使用结束后delete
CDir* ZCFile::ZCGetDir(const TDesC& strPath)
{
	CDir *dir;
	fsession.Connect();
	int err = fsession.GetDir(strPath,KEntryAttNormal|KEntryAttMatchMask,ESortByDate,dir);
	fsession.Close();
	return dir;
}

////////////////////////////////////////////////////////////
//检查文件夹是否存在,并返回完整路径名
int ZCFile::ZCDirFinder(const TDesC& strPath,const TDesC& fileName,TFileName& FullFileName)
{
	CDir *dir=NULL;
	int len;
	dir=ZCGetDir(strPath);
	len=dir->Count();
	for(int i=0;i<len;i++)
		{
		if(ZCIsDir((*dir)[i].iName))
			{
			if(!fileName.CompareC((*dir)[i].iName))
				{
				FullFileName=strPath;
				FullFileName.Append(_L("\\"));
				FullFileName.Append((*dir)[i].iName);
				delete dir;
				return TRUE;
				}
			else
				{
				TBuf<KMaxFileName> tmp;
				tmp=strPath;
				tmp.Append(_L("\\"));
				tmp.Append((*dir)[i].iName);
				if(ZCDirFinder(tmp,fileName,FullFileName))
					{
					delete dir;
					return TRUE;
					}
				}
			}
		}
	delete dir;
	return false;
}


////////////////////////////////////////////////////////////
//检查文件是否存在，并且返回文件完整路径
int ZCFile::ZCFileFinder(const TDesC& strPath,const TDesC& fileName,TFileName& FullFileName)
	{
	CDir* dir=NULL;
	int len;
	dir=ZCGetDir(strPath);
	len=dir->Count();
	for(int i=0;i<len;i++)
		{
		if(!ZCIsDir((*dir)[i].iName)) 
			{//如果不是文件夹
			if(!(*dir)[i].iName.CompareC(fileName))
				{
				FullFileName=strPath;
				FullFileName.Append(_L("\\"));
				FullFileName.Append((*dir)[i].iName);
				delete dir;
				return TRUE;
				}
			}
		else
			{//如果是文件夹
			TBuf<KMaxFileName> tmp;
			tmp=strPath;
			tmp.Append(_L("\\"));
			tmp.Append((*dir)[i].iName);
			if(ZCFileFinder(tmp,fileName,FullFileName))
				{
				delete dir;
				return TRUE;
				}
			}
		}
	delete dir;
	return FALSE;
	}
