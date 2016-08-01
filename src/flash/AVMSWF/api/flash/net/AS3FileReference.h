#ifndef _AS3FileReference_
#define _AS3FileReference_

#include "AS3EventDispatcher.h"
#include "../core/DateObject.h"
#include "../core/ArrayObject.h"

#include "AS3URLRequest.h"
#include "AS3ByteArray.h"

namespace avmplus{namespace NativeID{
class FileReferenceClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FileReferenceObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FileReferenceObject;

	class FileReferenceClass : public ClassClosure
	{
	public:
		FileReferenceObject* CreateFileReference();

	public:
		FileReferenceClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);
	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileReferenceClassSlots m_slots_FileReferenceClass;
};
class FileReferenceObject : public EventDispatcherObject
{
	public:
		FileReferenceObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		DateObject	*m_pCreationDate;
		Stringp		m_creator;
		DateObject	*m_pModificationDate;
		Stringp		m_name;
		uint32_t	m_size;
		Stringp		m_type;
		
	public:
		DateObject* AS3_creationDate_get();
		Stringp		AS3_creator_get();
		DateObject* AS3_modificationDate_get();
		Stringp		AS3_name_get();
		uint32_t	AS3_size_get();
		Stringp		AS3_type_get();
		void		AS3_constructor();
		bool		AS3_browse(ArrayObject *pTypeFilter);
		void		AS3_cancel();
		void		AS3_download(URLRequestObject *pRequest, Stringp defaultFileName);
		void		AS3_upload(URLRequestObject *pRequest, Stringp uploadDataFieldName, bool testUpload);

		//new adds
		ByteArrayObject* AS3_data_get();
		Stringp AS3_extension_get();

		void AS3_load();
		void AS3_save(Atom data, Stringp pDefaultFileName);
		void AS3_uploadUnencoded(URLRequestObject *pRequest);
		//new adds end

	private:
#ifdef _SYMBIAN
	public:
#endif
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileReferenceObjectSlots m_slots_FileReferenceObject;
};}
#endif