#ifndef _AS3File_
#define _AS3File_
namespace avmplus{namespace NativeID{
class FileClassSlots{
friend class SlotOffsetsAndAsserts;
public://Declare your STATIC AS3 slots here!!!

private:};
class FileObjectSlots{
friend class SlotOffsetsAndAsserts;
public:
//Declare your MEMBER AS3 slots here!!!
private:};
}}
namespace avmshell{
	class FileObject;
	class FileClass : public ClassClosure
	{
	public:
		FileClass(VTable *vtable);
		ScriptObject *createInstance(VTable *ivtable, ScriptObject *delegate);

		//new adds 11
		FileObject* AS3_cacheDirectory_get();
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileClassSlots m_slots_FileClass;
};
class FileObject : public ScriptObject
{
	public:
		FileObject(VTable* _vtable, ScriptObject* _delegate, int capacity);
	public:
		void* pData;//Set your data!!
		AvmBox AS3_applicationDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_applicationStorageDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_desktopDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_documentsDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_lineEnding_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_separator_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_systemCharset_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_userDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_createTempDirectory(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_createTempFile(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getRootDirectories(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_downloaded_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_downloaded_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_exists_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isDirectory_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isHidden_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isPackage_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_isSymbolicLink_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_nativePath_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_nativePath_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_parent_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_spaceAvailable_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_url_get(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_url_set(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_constructor(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_browseForDirectory(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_browseForOpen(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_browseForOpenMultiple(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_browseForSave(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_cancel(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_canonicalize(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_clone(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_copyTo(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_copyToAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_createDirectory(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_deleteDirectory(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_deleteDirectoryAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_deleteFile(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_deleteFileAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getDirectoryListing(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getDirectoryListingAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_getRelativePath(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_moveTo(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_moveToAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_moveToTrash(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_moveToTrashAsync(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_openWithDefaultApplication(AvmMethodEnv env, uint32_t argc, AvmBox* argv);
		AvmBox AS3_resolvePath(AvmMethodEnv env, uint32_t argc, AvmBox* argv);

		//new adds 11
		
		bool AS3_preventBackup_get();
		void AS3_preventBackup_set(bool b);
		//new adds 11 end
	private:
		friend class avmplus::NativeID::SlotOffsetsAndAsserts;
		avmplus::NativeID::FileObjectSlots m_slots_FileObject;
};}
#endif