/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is [Open Source Virtual Machine.].
 *
 * The Initial Developer of the Original Code is
 * Adobe System Incorporated.
 * Portions created by the Initial Developer are Copyright (C) 1993-2006
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *   Adobe AS3 Team
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */


#include "avmplus.h"

namespace avmplus
{
	DomainEnv::DomainEnv(AvmCore *core, Domain* domain, DomainEnv* base)
		: m_domain(domain),
		  m_base(base),
		  m_namedScripts(new (core->GetGC()) MultinameHashtable())
		  ,m_bOverwrite(false)
		  //m_bTopSeek(false)
	{
	}

	MethodEnv* DomainEnv::getScriptInit(Namespacep ns, Stringp name) const
	{
		//if(!m_bTopSeek)//[Note X]
		{
			MethodEnv *env = NULL;
			if (m_base) {
				env = m_base->getScriptInit(ns, name);
				//[Note X]
				//if(env&&env->isOverwrite())
				//	env=NULL;
			}
			if (!env) {
				env = (MethodEnv*) m_namedScripts->get(name, ns);
			}
			return env;
		}
		/*else
		{
			Multiname multiname(ns,name);
			MethodEnv *env = (MethodEnv*) m_namedScripts->getMulti(multiname);
			
			if (!env&&m_base) {
				env = m_base->getScriptEntry(multiname);
			}		
			return env;
		}*/
	}	

	MethodEnv* DomainEnv::getScriptInit(const Multiname& multiname) const
	{
		//StUTF8String s(multiname.getName());
		//if(strcmp(s.c_str(),"CrossDomainRSLItem")==0)
		//	int v=0;
		//if(!m_bTopSeek)//[Note X]
		{
			MethodEnv *env = NULL;
			if (m_base) {
				env = m_base->getScriptInit(multiname);				
				//[Note X]
				//if(env&&env->isOverwrite())
				//	env=NULL;
			}
			if (!env) {
				env = (MethodEnv*) m_namedScripts->getMulti(multiname);
			}
			return env;
		}
		/*else
		{
			MethodEnv *env = (MethodEnv*) m_namedScripts->getMulti(multiname);
			
			if (!env&&m_base) {
				env = m_base->getScriptEntry(multiname);
			}		
			return env;
		}*/
	}

	MethodEnv* DomainEnv::getScriptEntry(const Multiname& multiname) const
	{
		MethodEnv *env = (MethodEnv*) m_namedScripts->getMulti(multiname);
		
		if (!env&&m_base) {
			env = m_base->getScriptEntry(multiname);
		}		
		return env;
	}

	int DomainEnv::scriptNext(int index) const
	{
		int v = m_namedScripts->next(index);
		return v;
	}

	Stringp DomainEnv::scriptNameAt(int index) const
	{
		Stringp s = m_namedScripts->keyAt(index);
		return s;
	}

	Namespacep DomainEnv::scriptNsAt(int index) const
	{
		Namespacep ns = m_namedScripts->nsAt(index);
		return ns;
	}
	
	Toplevel* DomainEnv::toplevel() const
	{
		if(m_toplevel) return m_toplevel;
		if(m_base) return m_base->toplevel();
		AvmAssert(0);
		return NULL;
	}
}


