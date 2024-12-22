//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop
//---------------------------------------------------------------------------
























































USEFORM("UnitFormCoCincompatibility.cpp", FormCoCincompatibility);
USEFORM("UnitMain.cpp", FormMain);
USEFORM("UnitExportToCoC.cpp", FormCoCExport);
USEFORM("UnitOption.cpp", FormOption);
USEFORM("UnitOEEForm.cpp", FormOEE);
USEFORM("UnitPacks.cpp", FormPacks);
USEFORM("DataCoater.cpp", DataModule1);
USEFORM("FromProgress.cpp", FormProgress);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		SetApplicationMainFormOnTaskBar(Application, true);
		Application->Title = "Coater E-Logbook (Kolorowy Œwiat Coater'a)";
		Application->CreateForm(__classid(TDataModule1), &DataModule1);
		Application->CreateForm(__classid(TFormMain), &FormMain);
		Application->CreateForm(__classid(TFormCoCExport), &FormCoCExport);
		Application->CreateForm(__classid(TFormCoCincompatibility), &FormCoCincompatibility);
		Application->CreateForm(__classid(TFormOption), &FormOption);
		Application->CreateForm(__classid(TFormProgress), &FormProgress);
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
