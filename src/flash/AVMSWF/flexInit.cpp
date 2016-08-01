#include "StdAfxflash.h"
#include "splayer.h"
#include "avm2.h"
#include "ShellCore.h"
#include "flexInit.h"

namespace FLEX
{
		/*bool LoadIfNeed(Stringp className,XAVM2*pAVM2)
		{
			if(pAVM2->m_flexPlayer) return false;
			ShellCore*pCore=pAVM2->GetCore();
			if(!pCore->kApplication->equals(className)) return false;
			pAVM2->m_flexPlayer=InitFlex(pCore->GetPlayer(),pAVM2,pCore->GetShellContext());
			return pAVM2->m_flexPlayer!=NULL;
		}*/

		ScriptPlayer* InitFlex(XSWFPlayer*root,XAVM2*pAVM2,ShellCodeContext*pContext,int nFlex)
		{
			//return NULL;
			XString8 strFile=root->m_strWorkPath.strTxt;
			//XResource::TransResPath(strFile);
			switch(nFlex)
			{
			default:
			case 3:
				strFile+="framework_3.swf";
				break;
			case 4:
				strFile+="framework_4.swf";
				break;
			}
			XFile file;
			if(!file.Open(strFile,XFile::XBINARY|XFile::XREAD))
			{
				AvmAssert(false);
				return XNULL;
			}
			int len=file.GetLength();
			XU8* buf=new XU8[len];
			if(buf==XNULL)
			{
				AvmAssert(false);
				file.Close();
				return XNULL;
			}
			file.Read(buf,len);
			file.Close();

			ScriptPlayer* player=new ScriptPlayer(XFALSE);
			player->m_pAVM2=pAVM2;

			player->display=&root->display;
			player->player=player;
			player->splayer = root;
			player->m_url="flex-lib";
			player->layerDepth = 16001;
			//ShellCodeContext*pContext=NULL;

			player->InitAS3(NULL,NULL,pContext->m_pAppDomain,XNULL);
			player->InitRoot(false);
			player->m_pCodeContext->m_domain->SetOverWrite(true);
			player->m_pCodeContext->m_domainEnv->setOverwrite(true);

			//player->SetDisplay(&root->display,false);


			player->PushData((U8*)buf, len,NULL,false);

			delete buf;

			player->DoTags(0);

			player->rootObject->SetVisible(XFALSE);

			return player;
		}
		void   ReleaseFlex(ScriptPlayer*player)
		{
			if(player)
				delete player;
		}
}
