#include "UnitTest++.h"

#include "K15_File.h"
#include "K15_Directory.h"
#include "K15_EventManager.h"
#include "K15_FileWatcher.h"

using namespace K15_EngineV2;

SUITE(FileManagerTest)
{
	TEST(FileFunctionality)
	{
		//Erase a file.
		File::Erase("C:\\Test.txt");

		//Check if has been deleted
		CHECK(!File::Exists("C:\\Test.txt"));

		//Create a new file.
		File fFile("C:\\Test.txt",File::OM_WRITE | File::OM_READ);

		//Write something in the file.
		fFile.Write(1,12,(byte*)"Was geht ab");
	
		//Read something out of the file.
		byte *pBuffer = K15_NEW byte[12];
		fFile.Read(1,12,pBuffer);

		int iEqual = strcmp((const char*)pBuffer,"Was geht ab");
		fFile.Close();

		K15_DELETE pBuffer;

		CHECK_EQUAL(iEqual,0);
		
		//Erase file and check if the deletion was successfull.
		File::Erase("C:\\Test.txt");
		CHECK(!File::Exists("C:\\Test.txt"));
	}

	TEST(FileWatcher)
	{
		class FileWatchListener : public EventListener
		{
			virtual void HandleEvent(const Event &evGameEvent)
			{
				String sFileName = (const char*)evGameEvent.GetArgument();
				if(evGameEvent.GetType() == EventTypes::FileCreated){
					printf("Filewatcher - %s has been created.\n",sFileName.C_Str());
				}else if(evGameEvent.GetType() == EventTypes::FileChanged){
					printf("Filewatcher - %s has been changed.\n",sFileName.C_Str());
				}else if(evGameEvent.GetType() == EventTypes::FileDeleted){
					printf("Filewatcher - %s has been deleted.\n",sFileName.C_Str());
				}
			}
		};

		g_pEventManager->AddListener(EventTypes::FileCreated,K15_NEW FileWatchListener());
		g_pEventManager->AddListener(EventTypes::FileChanged,K15_NEW FileWatchListener());
		g_pEventManager->AddListener(EventTypes::FileDeleted,K15_NEW FileWatchListener());

		//g_pFileWatcher->WatchDirectory("C:\\Test");

		//g_pFileWatcher->UnwatchDirectory("C:\\Test");

		g_pEventManager->RemoveListener(EventTypes::FileCreated,K15_NEW FileWatchListener());
		g_pEventManager->RemoveListener(EventTypes::FileChanged,K15_NEW FileWatchListener());
		g_pEventManager->RemoveListener(EventTypes::FileDeleted,K15_NEW FileWatchListener());
	}
}