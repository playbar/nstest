



#if !defined(ZCFILE_H_)
#define ZCFILE_H_

#include <f32file.h>

/*
*有待考察*/

class ZCFile{
	protected:
		RFs fsession;
	public:
		ZCFile();
		~ZCFile();
		//////////////////////////////////////////////////////////////
		//删除文件
		int ZCDeleteFile(const TDesC& fileName);

		/////////////////////////////////////////////////////////////
		//删除目录,目录下必须为空
		int ZCDeleteDirectory(const TDesC& strPath);

		////////////////////////////////////////////////////////////
		//创建目录
		int ZCCreateDirectory(const TDesC& strPath);

		///////////////////////////////////////////////////////////
		//得到驱动器列表
		void ZCGetDriverList(TDriveList &drvelist);
		
		///////////////////////////////////////////////////////////
		//得到盘符
		void ZCGetDrverChar(int aDriver,TUint &aChar);
		
		///////////////////////////////////////////////////////////
		//得到驱动器名
		void ZCGetDriverName(int aDriver,TDes &aDriverName);
		
		////////////////////////////////////////////////////////////
		//设置驱动器名
		void ZCSetDriverName(int aDriver,const TDes &aDriverName);

		////////////////////////////////////////////////////////////
		//查看是否是文件夹 成功返回大于0，失败返回小于0
		int ZCIsDir(const TDesC& fileName);
		
		////////////////////////////////////////////////////////////
		//取得文件或文件夹的属性
		int ZCGetEntry(const TDesC& fileName,TEntry&entry);
		
		////////////////////////////////////////////////////////////
		//取得指定目录下的所有项,以CDir指针接受返回值，使用结束后delete
		CDir* ZCGetDir(const TDesC& strPath);
		////////////////////////////////////////////////////////////
		//检查文件夹是否存在,并返回完整路径名
		int ZCDirFinder(const TDesC& strPath,const TDesC& fileName,TFileName& FullFileName);
		
		////////////////////////////////////////////////////////////
		//检查文件是否存在，并且返回文件完整路径
		int ZCFileFinder(const TDesC& strPath,const TDesC& fileName,TFileName& FullFileName);
		
};
#endif
