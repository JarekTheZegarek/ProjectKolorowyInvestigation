//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
#include <tchar.h>
//---------------------------------------------------------------------------













































USEFORM("UnitMainBooth.cpp", FormMain);
USEFORM("DetailFormBooth.cpp", FormAdditionalDetails);
USEFORM("DataBooth.cpp", DataModule2); /* TDataModule: File Type */
USEFORM("UnitAdditionalDefectDetailsFormBooth.cpp", FormAdditionalDefectDetails);
//---------------------------------------------------------------------------
WINAPI _tWinMain(HINSTANCE, HINSTANCE, LPTSTR, int)
{
	try
	{
		Application->Initialize();
		Application->MainFormOnTaskBar = true;
		Application->CreateForm(__classid(TDataModule2), &DataModule2);
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	catch (...)
	{
		try
		{
			throw Exception("");
		}
		catch (Exception &exception)
		{
			Application->ShowException(&exception);
		}
	}
	return 0;
}
//---------------------------------------------------------------------------
