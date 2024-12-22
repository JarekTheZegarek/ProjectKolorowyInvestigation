//---------------------------------------------------------------------------


#pragma hdrstop
#include <comobj.hpp>
#include <vcl.h>
#include "AccessCOM.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitExportToCoC.h"
#include "UnitFormCoCincompatibility.h"
#include "UnitMain.h"
#include "FromProgress.h"
#include <System.DateUtils.hpp>
//---------------------------------------------------------------------------

#pragma package(smart_init)


Variant accessApp;
Variant accessDocuments;
Variant excelWorkbook;
static AnsiString Database = "Line_Status";
static AnsiString Database2 = "LineStatusRemarks";
static AnsiString DataQuery = "QueryLineStatusRemarks";
std::map<int,int>  CSummaryLineStatus::LineIDToCoCCell;
std::map<int,UnicodeString> CSummaryLineStatus::CoCCellToCategory;
std::map<int,UnicodeString> CSummaryProduction::IDtoProductName;
std::map<int,UnicodeString> CSummaryProduction::IDtoCoCCell;
std::map<int,UnicodeString> CSummaryProduction::IDtoShortProductName;
bool CoCDataPrepared = false;

CSummaryLineStatus CurrentSumka;
CSummaryProduction CurrentSummaryProduction;


void AccessConnect()
{
        if (accessApp.IsEmpty())
        {
            accessApp=Variant::CreateObject("Access.Application");
        }
        else
        {
            accessApp=GetActiveOleObject("Access.Application");
        }
        if (accessApp.IsEmpty())
        {
            ShowMessage("Unable to find Access application.");
            return;
        }
        if (accessApp.IsEmpty()) {
 			return;
 		}
 WideString Buf;
 accessDocuments = accessApp.OlePropertyGet("DoCmd");
 accessApp.OlePropertySet("visible",false);
// accessApp.OleProcedure("echo",false);
 accessDocuments.OleProcedure("echo",false);
 //accessApp.OleProcedure("SendKeys","+");
 //"c:\\Documents and Settings\\jsolecki\\My Documents\\Nowy Raport Coater\'a\\Creport.mdb"
 Buf =  CGlobalVariables::CreportPath.c_str();
 accessApp.OleProcedure("OpenCurrentDatabase",Buf);

 accessDocuments.OleProcedure("RunMacro","Print") ;
 //accessApp.OleProcedure("SetOption","Show Startup Dialog Box",true);
 accessApp.OleProcedure("CloseCurrentDatabase");
 accessApp.OleProcedure("Quit");
 accessApp = Unassigned;
 accessApp= Variant().Empty();


//    if(OpenDialog1->Execute())
//    {
//       // TAutoArgs<1> Argument;
//       // Argument[1]=  (Variant) OpenDialog1->FileName.c_str();
//       //	Argument[2]= (Variant) "false";
//       // TAutoArgsBase* Arg = (TAutoArgsBase*) (&Argument);
//       	accessDocuments.OleProcedure("Open",  OpenDialog1->FileName.c_str(),0,false); // False ReadOnly
//        excelWorkbook = accessApp.OlePropertyGet("ActiveWorkbook");
//       // accessDocuments.OleProcedure("Open", Arg); // False ReadOnly
//       // accessDocuments.OlePropertySet(
//    }
//    accessApp.OlePropertySet("Visible", (Variant) true);


}

void WriteToDBCurrentID(int ID)
{
try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString;
       int Quantity,i=1,j=1;


       		Query->Close();
            Query->SQL->Clear();
            QueryString=	" UPDATE CurrentData 		"
                        	" SET CurrentID = :CurrentID "
                            " WHERE ID = 1 "


                    		;
          //  ExchangeAnsStr(QueryString,":Defect","["+ReturnField(Defect)+"]");
        	Query->SQL->Add(QueryString);
            Query->Parameters->ParamByName("CurrentID")->Value=ID;
            Query->ExecSQL();
            Query->Close();
            Query->SQL->Clear();
     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

void AccessPrintCurrent()
{
		WriteToDBCurrentID(Current.GetID());

        if (accessApp.IsEmpty())
        {
            accessApp=Variant::CreateObject("Access.Application");
        }
        else
        {
            accessApp=GetActiveOleObject("Access.Application");
        }
        if (accessApp.IsEmpty())
		{
			ShowMessage("Unable to find Access application.");
			return;
		}
		if (accessApp.IsEmpty()) {
			return;
		}



 Variant qdf;
 Variant Database;
 Variant Param;
 WideString Buf;
 accessDocuments = accessApp.OlePropertyGet("DoCmd");
 accessApp.OlePropertySet("visible",false);
 accessDocuments.OleProcedure("echo",false);
 Buf =  CGlobalVariables::CreportPath.c_str();
 accessApp.OleProcedure("OpenCurrentDatabase",Buf);
  Buf =  "PrintCurrent"  ;
 //	WriteToDBCurrentID(Current.GetID());
	accessDocuments.OleProcedure("RunMacro",Buf) ;
	WriteToDBCurrentID(Current.GetID());
 accessApp.OleProcedure("CloseCurrentDatabase");
 accessApp.OleProcedure("Quit");
 accessApp = Unassigned;
 accessApp= Variant().Empty();


 WriteToDBCurrentID(0);
}

//Converting column and row number to Excel Cell address e.g. "A5" for col 1 and row 5
AnsiString ExcelChar(int Col, int Row)
{
	char L;
	AnsiString Kolumna="";
	AnsiString Wiersz=Row;
	if ((Col/26)> 0 && Col!=26 ) {          // For 26 it is last letter in the alphabet
		if( Col%26==0)
		   L = char(64+Col/26-1);
		else
		   L = char(64+Col/26);
		Kolumna=L;
	}

	L= Col % 26;
	if(L == 0) L=26;
	L=64+ L;
	Kolumna = Kolumna + L;
	return Kolumna = Kolumna + Wiersz;
}

//Convert excel string cell address e.g. "A5" into pair Col Row in this example 1, 5
// so first element of pair is column and the second  is row
std::pair<int,int> ExcelColRow(UnicodeString ExcelAddress)
{
	int i=0;
	UnicodeString RowString;
	char ColChar;
	int row=0,col=0;
	ExcelAddress=ExcelAddress.UpperCase();
	for(int i=0;i<ExcelAddress.Length();i++)
	{
		ColChar = ExcelAddress[i+1];   //base index is 1
		if (!IsDigit(ColChar)) {
		   col=col*26+(ColChar-'A' + 1);
		}
		else
		{
			RowString = ExcelAddress.SubString(i+1,ExcelAddress.Length()-i);
			break;
		}
	}
	row = RowString.ToInt();
	std::pair<int,int> res(col,row);
	return res;
}

void ExportExcel(TStringGrid* Sheet)
{
 Variant Temp,Temp1,Temp2;
 Variant ExcelApp;
 Variant ExcelDocuments;
 Variant ExcelSheet;
 if (ExcelApp.IsEmpty())
		{
			ExcelApp=Variant::CreateObject("Excel.Application");
		}
		else
		{
			ExcelApp=GetActiveOleObject("Excel.Application");
		}
 if (ExcelApp.IsEmpty())
		{
			ShowMessage("Nie mo¿na otworzyæ programu Excel!\n\nUnable to find Excel application.");
			return;
		}
 if (ExcelApp.IsEmpty()) {
			return;
		}
 ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");

 ExcelApp.OlePropertySet("visible",true);

 ExcelDocuments = ExcelDocuments.OleFunction("Add");
 ExcelSheet = ExcelDocuments.OlePropertyGet("ActiveSheet");
 WideString Buf;
 Buf = "Eksport z Coater Report" ;
 ExcelSheet.OlePropertySet("Name",Buf);

 AnsiString a = ExcelSheet.OlePropertyGet("Name");
 for(int i=0;i<Sheet->ColCount;i++)
 {
  for(int j=0;j<Sheet->RowCount;j++)
  {
   Buf = Sheet->Cells[i][j].c_str();
   ExcelSheet.OlePropertySet(L"Cells",j+1 ,i+1,Buf/*FormMain->StringGridReport->Cells[i][j].w_str()*/);
   Temp=ExcelSheet.OlePropertyGet(L"Cells",j+1 ,i+1);///*FormMain->StringGridReport->Cells[i][j].w_str()*/);
  }
 }

  Temp1 = ExcelSheet.OlePropertyGet(L"Cells",1,1);
  Temp2 = ExcelSheet.OlePropertyGet(L"Cells",Sheet->RowCount,Sheet->ColCount);
  Temp=ExcelSheet.OlePropertyGet(L"Range",Temp1,Temp2);///*Sheet->Cells[i][j].w_str()*/);
  Temp=Temp.OlePropertyGet(L"Borders");
  Temp.OlePropertySet(L"LineStyle",L"xlContinuous");

  for(int i=0;i<Sheet->ColCount;i++)
	 {
	   Temp=ExcelSheet.OlePropertyGet(L"Cells",1 ,i+1);///*Sheet->Cells[i][j].w_str()*/);
	   Temp=Temp.OlePropertyGet(L"Font");
	   Temp.OlePropertySet(L"Bold",L"True");
	 }

  Temp=ExcelSheet.OlePropertyGet(L"Columns");//,Sheet->ColCount);
  Temp.OleProcedure("AutoFit");
  Temp=ExcelSheet.OlePropertyGet(L"Rows");//,Sheet->ColCount);
  Temp.OleProcedure("AutoFit");

  ExcelSheet.OleProcedure("Activate");

  ExcelApp = Unassigned;
  ExcelApp= Variant().Empty();
return;

}


void ExportExcel()
{
	ExportExcel(FormMain->StringGridReport);
	return;

//  Variant Temp;
//  Variant ExcelApp;
//  Variant ExcelDocuments;
//  Variant ExcelSheet;
//  if (ExcelApp.IsEmpty())
//		  {
//			  ExcelApp=Variant::CreateObject("Excel.Application");
//		  }
//		  else
//		  {
//			  ExcelApp=GetActiveOleObject("Excel.Application");
//		  }
//		  if (ExcelApp.IsEmpty())
//		  {
//			  ShowMessage("Nie mo¿na otworzyæ programu Excel!\n\nUnable to find Excel application.");
//			  return;
//		  }
//		  if (ExcelApp.IsEmpty()) {
//			  return;
//		  }
//		  ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");
//
//		  ExcelApp.OlePropertySet("visible",true);
//		  // ExcelApp.OleProcedure("echo",false);
//   //		ExcelDocuments.OleProcedure("echo",false);
//   //ExcelApp.OleProcedure("SendKeys","+");
//   //"c:\\Documents and Settings\\jsolecki\\My Documents\\Nowy Raport Coater\'a\\Creport.mdb"
//   ExcelDocuments = ExcelDocuments.OleFunction("Add");
//   ExcelSheet = ExcelDocuments.OlePropertyGet("ActiveSheet");
//   WideString Buf;
//   Buf = "Eksport z Coater Report" ;
//   ExcelSheet.OlePropertySet("Name",Buf);
//
//   AnsiString a = ExcelSheet.OlePropertyGet("Name");
//   for(int i=1;i<FormMain->StringGridReport->ColCount;i++)
//   {
//	for(int j=0;j<FormMain->StringGridReport->RowCount;j++)
//	{
//	 Buf = FormMain->StringGridReport->Cells[i][j].c_str();
//	 ExcelSheet.OlePropertySet(L"Cells",j+1 ,i+1,Buf/*FormMain->StringGridReport->Cells[i][j].w_str()*/);
//	 Temp=ExcelSheet.OlePropertyGet(L"Cells",j+1 ,i+1);///*FormMain->StringGridReport->Cells[i][j].w_str()*/);
//	 Temp=Temp.OlePropertyGet(L"Borders");
//	 Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
//	}
//	Temp=ExcelSheet.OlePropertyGet("Columns",i+1);//,FormMain->StringGridReport->ColCount);
//	Temp.OleProcedure("AutoFit");
//   }
//
//	for(int j=0;j<FormMain->StringGridReport->RowCount;j++)
//	{
//	  if(FormMain->StringGridReport->Cells[0][j].IsEmpty()) continue;
//	  Buf =  AnsiString(" "+FormMain->StringGridReport->Cells[0][j]). c_str();
//	  ExcelSheet.OlePropertySet("Cells",j+1 , 1,Buf );
//	  Temp=ExcelSheet.OlePropertyGet(L"Cells",j+1 ,1);///*FormMain->StringGridReport->Cells[i][j].w_str()*/);
//	  Temp=Temp.OlePropertyGet(L"Borders");
//	  Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
//	}
//	for(int i=0;i<FormMain->StringGridReport->ColCount;i++)
//	{
//
//	  //Buf = FormMain->StringGridReport->Cells[i][j].c_str();
//	  Temp=ExcelSheet.OlePropertyGet(L"Cells",1 ,i+1);///*FormMain->StringGridReport->Cells[i][j].w_str()*/);
//	  Temp=Temp.OlePropertyGet(L"Font");
//	  Temp.OlePropertySet(L"Bold",L"True");
//	}
//	Temp=ExcelSheet.OlePropertyGet("Columns",1);//,FormMain->StringGridReport->ColCount);
//	Temp.OleProcedure("AutoFit");
//
//	ExcelApp = Unassigned;
//	ExcelApp= Variant().Empty();
//  return;
//
//   ExcelApp.OleProcedure("Quit");
//
//
//
//
//
//  //        _WorkbookPtr wb;
//  //        AnsiString SheetName;
//  //        _WorksheetPtr ws;
//  //        Excel_xp::ExcelWorksheetPtr Wksh;
//  //        Excel_xp::SheetsPtr sheets;
//  //        Excel_xp::ExcelRangePtr R;
//  //        Excel_xp::WorkbooksPtr Wbp;
//  ////        Excel_xp::ExcelRange ;
//  //
//  //        FormMain->ExcelApplication1->Connect();
//  //
//  //        //wb = ExcelApplication1->Workbooks->Add(EmptyParam,0);
//  //        wb = FormMain->ExcelApplication1->Workbooks->Add((TVariant) xlWBATWorksheet,0);
//  //        ws = wb->ActiveSheet;   //ExcelApplication1->ActiveSheet;
//  //        //ws->set_Name("Export");
//  //    //    Wbp = ExcelApplication1->ActiveWorkbook;
//  //       // Wbp->Add((TVariant) xlWBATWorksheet,0);
//  ////         Wksh = ws;
//  ////        wb->;
//  ////        Wksh->Name="Eksport";
//  //        FormMain->ExcelApplication1->Workbooks->get_Item((TVariant)1)->set_Title(0,"Eksport");// Name = "E";  //= (TVariant)"Eksport";
//  //        //ws->Activate();
//  //
//  ////        SheetName = ws->get_Name();
//  //        sheets = wb->get_Sheets();
//  //        ws = sheets->get_Item((TVariant)1);
//  //        ws = wb->Sheets->get_Item((TVariant)"Sheet1");
//  ////        R=ws->Cells->get_Item((TVariant)1,(TVariant)1);
//  ////        R->set_Value2((TVariant)"Dopisek");
//  ////        ws->Name = "Export";
//  ////        int i = 0;
//  ////        ws = sheets->get_Item((TVariant)i);
//  //
//  ////        TClipboard* Clip = Clipboard();
//  //// 		TStringList* Bufor= new TStringList;
//  // 		AnsiString tym=""; //= StringGrid1->Cells[0][0];
//  //        AnsiString Label = "Export";
//  ////        if(Tryb.Get()== Spectra)
//  ////        {
//  ////            Label = "PositionY\\WaveLength";
//  ////        }
//  ////        else
//  ////        {
//  ////            Label = "Id\\PositionY";
//  ////        }
//  //
//  //        for(int i=0;i<FormMain->StringGridReport->RowCount;i++)
//  //        {
//  //               for(int j=0;j<FormMain->StringGridReport->ColCount;j++)
//  //               {
//  //                 tym=FormMain->StringGridReport->Cells[j][i];
//  //                 R = ws->Cells->get_Item(TVariant(i+2),TVariant(j+2));// ws->get_Range(ws->Cells[i][j],ws->Cells[i][j],EmptyParam);
//  //                 R->set_Value2((TVariant)tym);
//  //               }
//  //        }
//  //        R = ws->Cells->get_Item(TVariant(2),TVariant(2));// ws->get_Range(ws->Cells[i][j],ws->Cells[i][j],EmptyParam);
//  //        R->set_Value2((TVariant)Label);
//  ////        tym=StringGrid1->Cells[StringGrid1->Selection.Left][StringGrid1->Selection.Top];
//  ////        R = ws->Cells->get_Item(TVariant(1),TVariant(1));// ws->get_Range(ws->Cells[i][j],ws->Cells[i][j],EmptyParam);
//  ////        R->set_Value2((TVariant)tym);
//  ////        tym=StringGrid1->Cells[StringGrid1->Selection.Left][StringGrid1->Selection.Top];
//  ////        R = ws->Cells->get_Item(TVariant(1),TVariant(1));// ws->get_Range(ws->Cells[i][j],ws->Cells[i][j],EmptyParam);
//  ////        R->set_Value2((TVariant)tym);
//  //        ws->Name="Export";
//  //        //ws->Activate();
//  //
//  //        SheetName = ws->get_Name();
//  //        FormMain->ExcelApplication1->Visible[0]=true;
//  //       // ExcelApplication1->Disconnect();
}


void ExportExcel2()
{

	 Variant Temp, Temp1, Temp2;
	 Variant ExcelApp;
	 Variant ExcelDocuments;
	 Variant ExcelSheet;
	 if (ExcelApp.IsEmpty())
		{
			ExcelApp=CreateOleObject("Excel.Application");
		}
		else
		{
			ExcelApp=GetActiveOleObject("Excel.Application");
		}
		if (ExcelApp.IsEmpty())
		{
			ShowMessage("Nie mo¿na otworzyæ programu Excel!\n\nUnable to find Excel application.");
			return;
		}
		if (ExcelApp.IsEmpty()) {
			return;
		}
	 ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");

	 ExcelApp.OlePropertySet("visible",true);
	 ExcelDocuments = ExcelDocuments.OleFunction("Add");
	 ExcelSheet = ExcelDocuments.OlePropertyGet("ActiveSheet");
	 WideString Buf;
	 Buf = "Eksport z Coater Report" ;
	 ExcelSheet.OlePropertySet("Name",Buf);

	 AnsiString a = ExcelSheet.OlePropertyGet("Name");

//	 for(int i=0;i<FormMain->StringGridReport->ColCount;i++)
//	 {
//	  for(int j=0;j<FormMain->StringGridReport->RowCount;j++)
//	  {
//	  Buf = FormMain->StringGridReport->Cells[i][j].c_str();
//	   ExcelSheet.OlePropertySet(L"Cells",j+1 ,i+1,Buf/*FormMain->StringGridReport->Cells[i][j].w_str()*/);
////	   Temp=ExcelSheet.OlePropertyGet(L"Cells",j+1 ,i+1);///*FormMain->StringGridReport->Cells[i][j].w_str()*/);
////	   Temp=Temp.OlePropertyGet(L"Borders");
////	   Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
//	  }
//	 }
	   ExportToClipboard();

	   Temp1 = ExcelSheet.OlePropertyGet(L"Cells",1,1);
	   Temp2 = ExcelSheet.OlePropertyGet(L"Cells",FormMain->StringGridReport->RowCount,FormMain->StringGridReport->ColCount);
	   ExcelSheet.OleProcedure(L"Paste",Temp1);//(
	   Temp=ExcelSheet.OlePropertyGet(L"Range",Temp1,Temp2);///*FormMain->StringGridReport->Cells[i][j].w_str()*/);
	   Temp=Temp.OlePropertyGet(L"Borders");
	   Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
	 for(int i=0;i<FormMain->StringGridReport->ColCount;i++)
	 {

	  //Buf = FormMain->StringGridReport->Cells[i][j].c_str();
	   Temp=ExcelSheet.OlePropertyGet(L"Cells",1 ,i+1);///*FormMain->StringGridReport->Cells[i][j].w_str()*/);
	   Temp=Temp.OlePropertyGet(L"Font");
	   Temp.OlePropertySet(L"Bold",L"True");
	 }

	  Temp=ExcelSheet.OlePropertyGet(L"Columns");//,FormMain->StringGridReport->ColCount);
	  Temp.OleProcedure("AutoFit");
	  Temp=ExcelSheet.OlePropertyGet(L"Rows");//,FormMain->StringGridReport->ColCount);
	  Temp.OleProcedure("AutoFit");

	  ExcelSheet.OleProcedure("Activate");


//	 for(int i=0;i<FormMain->StringGridReport->ColCount;i++)
//	 {
//
//	   Buf = FormMain->StringGridReport->Cols[i]->GetText();  //.c_str();
//	   ExcelSheet.OlePropertySet(L"Columns",i+1,Buf/*FormMain->StringGridReport->Cells[i][j].w_str()*/);
//
//	   Temp=ExcelSheet.OlePropertyGet("Columns",i+1);//,FormMain->StringGridReport->ColCount);
//	   Temp.OleProcedure("AutoFit");
//	 }

//	 for(int j=0;j<FormMain->StringGridReport->RowCount;j++)
//	 {
//		Temp=ExcelSheet.OlePropertyGet("Rows",j+1);//,FormMain->StringGridReport->ColCount);
//		Temp.OleProcedure("AutoFit");
//	 }
//	  for(int j=0;j<FormMain->StringGridReport->RowCount;j++)
//	  {
//	   if(FormMain->StringGridReport->Cells[0][j+1].IsEmpty()) continue;
//	   Buf =  AnsiString(" "+FormMain->StringGridReport->Cells[0][j+1]). c_str();
//	   ExcelSheet.OlePropertySet("Cells",j+2 , 1,Buf );
//	  }
//	  Temp=ExcelSheet.OlePropertyGet("Columns",1);//,FormMain->StringGridReport->ColCount);
//	  Temp.OleProcedure("AutoFit");

	  ExcelApp = Unassigned;
	  ExcelApp= Variant().Empty();
	return;





}


void ExportExcel2(TStringGrid* Sheet)
{

	 Variant Temp, Temp1, Temp2;
	 Variant ExcelApp;
	 Variant ExcelDocuments;
	 Variant ExcelSheet;
	 if (ExcelApp.IsEmpty())
		{
			ExcelApp=CreateOleObject("Excel.Application");
		}
		else
		{
			ExcelApp=GetActiveOleObject("Excel.Application");
		}
		if (ExcelApp.IsEmpty())
		{
			ShowMessage("Nie mo¿na otworzyæ programu Excel!\n\nUnable to find Excel application.");
			return;
		}
		if (ExcelApp.IsEmpty()) {
			return;
		}
	 ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");

	 ExcelApp.OlePropertySet("visible",true);
	 ExcelDocuments = ExcelDocuments.OleFunction("Add");
	 ExcelSheet = ExcelDocuments.OlePropertyGet("ActiveSheet");
	 WideString Buf;
	 Buf = "Eksport z Coater Report" ;
	 ExcelSheet.OlePropertySet("Name",Buf);

	 AnsiString a = ExcelSheet.OlePropertyGet("Name");

//	 for(int i=0;i<Sheet->ColCount;i++)
//	 {
//	  for(int j=0;j<Sheet->RowCount;j++)
//	  {
//	  Buf = Sheet->Cells[i][j].c_str();
//	   ExcelSheet.OlePropertySet(L"Cells",j+1 ,i+1,Buf/*Sheet->Cells[i][j].w_str()*/);
////	   Temp=ExcelSheet.OlePropertyGet(L"Cells",j+1 ,i+1);///*Sheet->Cells[i][j].w_str()*/);
////	   Temp=Temp.OlePropertyGet(L"Borders");
////	   Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
//	  }
//	 }
	   ExportToClipboard(Sheet);

	   Temp1 = ExcelSheet.OlePropertyGet(L"Cells",1,1);
	   Temp2 = ExcelSheet.OlePropertyGet(L"Cells",Sheet->RowCount,Sheet->ColCount);
	   ExcelSheet.OleProcedure(L"Paste",Temp1);//(
	   Temp=ExcelSheet.OlePropertyGet(L"Range",Temp1,Temp2);///*Sheet->Cells[i][j].w_str()*/);
	   Temp=Temp.OlePropertyGet(L"Borders");
	   Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
	 for(int i=0;i<Sheet->ColCount;i++)
	 {

	  //Buf = Sheet->Cells[i][j].c_str();
	   Temp=ExcelSheet.OlePropertyGet(L"Cells",1 ,i+1);///*Sheet->Cells[i][j].w_str()*/);
	   Temp=Temp.OlePropertyGet(L"Font");
	   Temp.OlePropertySet(L"Bold",L"True");
	 }

	  Temp=ExcelSheet.OlePropertyGet(L"Columns");//,Sheet->ColCount);
	  Temp.OleProcedure("AutoFit");
	  Temp=ExcelSheet.OlePropertyGet(L"Rows");//,Sheet->ColCount);
	  Temp.OleProcedure("AutoFit");

	  ExcelSheet.OleProcedure("Activate");


//	 for(int i=0;i<Sheet->ColCount;i++)
//	 {
//
//	   Buf = Sheet->Cols[i]->GetText();  //.c_str();
//	   ExcelSheet.OlePropertySet(L"Columns",i+1,Buf/*Sheet->Cells[i][j].w_str()*/);
//
//	   Temp=ExcelSheet.OlePropertyGet("Columns",i+1);//,Sheet->ColCount);
//	   Temp.OleProcedure("AutoFit");
//	 }

//	 for(int j=0;j<Sheet->RowCount;j++)
//	 {
//		Temp=ExcelSheet.OlePropertyGet("Rows",j+1);//,Sheet->ColCount);
//		Temp.OleProcedure("AutoFit");
//	 }
//	  for(int j=0;j<Sheet->RowCount;j++)
//	  {
//	   if(Sheet->Cells[0][j+1].IsEmpty()) continue;
//	   Buf =  AnsiString(" "+Sheet->Cells[0][j+1]). c_str();
//	   ExcelSheet.OlePropertySet("Cells",j+2 , 1,Buf );
//	  }
//	  Temp=ExcelSheet.OlePropertyGet("Columns",1);//,Sheet->ColCount);
//	  Temp.OleProcedure("AutoFit");

	  ExcelApp = Unassigned;
	  ExcelApp= Variant().Empty();
	return;





}









//Procedure exchange in Str all???? encounters of Sorce by Destiny
void Exchange(UnicodeString& Str,WideChar Source, WideChar Destiny)
{
	int Position;
	Position = Str.Pos(Source);
	while(Position!=0) {
		Str.Delete(Position,1);
		Str.Insert(Destiny,Position);
		Position = Str.Pos(Source);
	}
}

void ExportToClipboard()
{
	UnicodeString Buf="";
	FormMain->RichEditReport->Clear();
	 for(int j=0;j<FormMain->StringGridReport->RowCount;j++)
	 {
	  for(int i=0;i<FormMain->StringGridReport->ColCount;i++)
	  {
	   Buf += FormMain->StringGridReport->Cells[i][j].c_str();
	   Exchange(Buf,'\n',' ');
	   Exchange(Buf,'\r',' ');
	   Buf += '\t';
	  }
	  if(Buf=="\t")
	  {
		  Buf="";
		  continue;
	  }
	  FormMain->RichEditReport->Lines->Add(Buf);
	  Buf="";

	 }
	 FormMain->RichEditReport->SelectAll();
	 FormMain->RichEditReport->CopyToClipboard();

}


void ExportToClipboard(TStringGrid* Sheet)
{
	UnicodeString Buf="";
	FormMain->RichEditReport->Clear();
	 for(int j=0;j<Sheet->RowCount;j++)
	 {
	  for(int i=0;i<Sheet->ColCount;i++)
	  {
	   Buf += Sheet->Cells[i][j].c_str();
	   Exchange(Buf,'\n',' ');
	   Exchange(Buf,'\r',' ');
	   Buf += '\t';
	  }
	  if(Buf=="\t")
	  {
		  Buf="";
		  continue;
	  }
	  FormMain->RichEditReport->Lines->Add(Buf);
	  Buf="";

	 }
	 FormMain->RichEditReport->SelectAll();
	 FormMain->RichEditReport->CopyToClipboard();

}




 Variant MSWord;
  Variant Temp;
  Variant ExcelApp;
  Variant ExcelDocuments;
  Variant ExcelSheet;
  PropertySet VisTrue(L"Visible");
  Procedure QuitFalse(L"Quit");
  PropertyGet GetDocs(L"Documents");
  PropertyGet GetSel(L"Selection");
  Function DocAdd(L"Add");
  Procedure AddText(L"TypeText");
  Procedure AddDate(L"InsertDateTime");
  Procedure AddPara(L"TypeParagraph");

CSummaryLineStatus GettingSummary(int);

void PrintingSummary(CSummaryLineStatus* Summary)
{
   int i=1;
	TComboBox* Combo = FormMain->ComboBoxLineStatus;
   std::pair<int,double> Outcome;
   std::map<int,int>::iterator pos;
   UnicodeString EventStr;
   int CurrWidth, MaxWidth;
   CurrWidth=0;
   MaxWidth=FormCoCExport->StringGridFrom->ColWidths[0];
   Summary->ResetIterator();
   do
   {
	  Outcome =Summary->GetNextEventSummary();
//	  if(!Summary.IsNextEvent)
//	  {
//		  break;
//	  }
	  for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos)
	  {
		  if (pos->second == Outcome.first) {
				  EventStr= Combo->Items->Strings[pos->first];
				  break;
		  }
	  }

	   FormCoCExport->StringGridFrom->Cells[0][i]=EventStr;
	   CurrWidth= FormCoCExport->StringGridFrom->Canvas->TextWidth(EventStr);

	   MaxWidth = MaxWidth > CurrWidth ? MaxWidth : CurrWidth;

	   FormCoCExport->StringGridFrom->ColWidths[0] = MaxWidth+20;
	   FormCoCExport->StringGridFrom->Cells[1][i]=Outcome.second;
	   i++;
   } while(Summary->IsNextEvent);
   Summary->IsNextEvent = true;
   FormCoCExport->StringGrid2->RowCount = FormMain->StringGridLineStatus->RowCount;
   FormCoCExport->StringGrid2->ColCount = FormMain->StringGridLineStatus->ColCount;
   for (int i = 0; i < FormMain->StringGridLineStatus->RowCount; i++) {
	 FormCoCExport->StringGrid2->Rows[i] = FormMain->StringGridLineStatus->Rows[i];
   }
   for (int i = 0; i < FormCoCExport->StringGrid2->ColCount; i++) {
	  FormCoCExport->StringGrid2->ColWidths[i] = FormMain->StringGridLineStatus->ColWidths[i];
   }

   return;

}

void PrintingSummaryProduction(CSummaryProduction* Summary,bool Preliminary=true)
{
   int i=1;
   TComboBox* Combo = FormMain->ComboBoxLineStatus;
   int Outcome;
   std::pair<double,double> InOut ;
   double In, Out;
   double SumIn=0, SumOut=0;
   UnicodeString EventStr;
   TStringGrid* Sheet;

   int CurrWidth[4], MaxWidth[4];
   if (Preliminary)
   {
		Sheet   =  FormCoCExport->StringGridProductsForCoC;
   }
   else
   {
		Sheet   =  FormCoCExport->StringGridProductPrepared;
   }

   CurrWidth[0]=0;
   CurrWidth[1]=0;
   CurrWidth[2]=0;
   CurrWidth[3]=0;
   MaxWidth[0]=Sheet->ColWidths[0];
   MaxWidth[1]=Sheet->ColWidths[1];
   MaxWidth[2]=Sheet->ColWidths[2];
   MaxWidth[3]=Sheet->ColWidths[3];

   Summary->ResetIterator();
   if(!Summary->IsNextProduct)
   {
     return;
   }
   do
   {
	  Outcome =Summary->GetNextProduct();
	  EventStr=Summary->GetProductName(Outcome,Preliminary);
	  InOut = Summary->GetProduct(Outcome);
	  In = InOut.first;
	  Out = InOut.second;
	  SumIn+=In;
	  SumOut+=Out;
	  Sheet->Cells[0][i]=EventStr;

	  CurrWidth[0]= Sheet->Canvas->TextWidth(EventStr);
	  MaxWidth[0] = MaxWidth[0] > CurrWidth[0] ? MaxWidth[0] : CurrWidth[0];
	  Sheet->ColWidths[0] = MaxWidth[0]+20;

	  EventStr = UnicodeString().FormatFloat("0.00",In);
	  CurrWidth[1]= Sheet->Canvas->TextWidth(EventStr);
	  MaxWidth[1] = MaxWidth[1] > CurrWidth[1] ? MaxWidth[1] : CurrWidth[1];
	  Sheet->ColWidths[1] = MaxWidth[1];
	  Sheet->Cells[1][i]=EventStr;


	  EventStr = UnicodeString().FormatFloat("0.00",Out);
	  Sheet->Cells[2][i]=EventStr;
	  CurrWidth[2]= Sheet->Canvas->TextWidth(EventStr);
	  MaxWidth[2] = MaxWidth[2] > CurrWidth[2] ? MaxWidth[2] : CurrWidth[2];
	  Sheet->ColWidths[2] = MaxWidth[2];

	  if (In==0) {
		EventStr = "0.00";
	  }
	  else
	  {
		EventStr = UnicodeString().FormatFloat("0.00",(Out/In)*100);
	  }
	  EventStr += "%";
	  Sheet->Cells[3][i]=EventStr;
	  CurrWidth[3]= Sheet->Canvas->TextWidth(EventStr);
	  MaxWidth[3] = MaxWidth[3] > CurrWidth[3] ? MaxWidth[3] : CurrWidth[3];
	  Sheet->ColWidths[3] = MaxWidth[3];

	   i++;
	   if(Sheet->RowCount<i+3)
	   {
		  Sheet->RowCount = i+3;
       }
   } while(Summary->IsNextProduct);
   Sheet->Cells[0][i+1]="Razem : ";
   EventStr = UnicodeString().FormatFloat("0.00",SumIn);
   Sheet->Cells[1][i+1]=EventStr;
   EventStr = UnicodeString().FormatFloat("0.00",SumOut);
   Sheet->Cells[2][i+1]=EventStr;
   EventStr = UnicodeString().FormatFloat("0.00",(SumOut*100)/SumIn);
   Sheet->Cells[3][i+1]=EventStr;
   Summary->IsNextProduct = true;
   return;
}


static CSummaryLineStatus SumMonthly[31];
void FillUpCoCMonth(TDateTime Date)
{
	//TDateTime Date =  TDateTime().CurrentDate;

	//CSummaryLineStatus SumMonthly[31];
	//TStringGrid* Sheet;
	//int MinuteDiff(const TDateTime, const TDateTime ) ;
	//Sheet = FormCoCExport->StringGridFrom;
	System::Word  day,month, year;

	DecodeDate(Date,year,month,day);
	extern PACKAGE System::TDateTime __fastcall EncodeDateTime(const System::Word AYear, const System::Word AMonth, const System::Word ADay, const System::Word AHour, const System::Word AMinute, const System::Word ASecond, const System::Word AMilliSecond);
	extern PACKAGE System::Word __fastcall DaysInAMonth(const System::Word AYear, const System::Word AMonth);
	TDateTime Start=EncodeDateTime(year,month,1,0,0,0,0);
	TDateTime Stop=EncodeDateTime(year,month,DaysInAMonth(year,month),22,0,0,0);
  //	static TDateTime LastTime;
	try{
		TADOQuery* Query =DataModule1->ADOQueryCreport;
		DataModule1->ADOConnectionCreport->Connected=true;
	   	AnsiString QueryString;
	   //	EmptySheet(Sheet);
		int i=1;
		int ID;
		int MinDiffInt;
		std::map<int,int>::iterator pos;
		Query->SQL->Clear();

				 QueryString =
				  "          Declare @StartDate as DateTime                                        "
				  "          Declare @StopDate as DateTime                                         "
				  "                                                                                "
				  "          Set @StartDate = :Start                                               "
				  "          Set @StopDate  = :Stop                                                "
				  "                                                                                "
				  "			  SELECT	                                                           "
				  "				   WORKSHIFTDAYS.[ID_WORKSHIFTDAY]                                 "
				  "				  ,Day([DATE_OF_WORK]) as Dzien                                    "
				  "				  ,[ID_LINESTATUS]                                                    "
				  "               ,[ID_SHIFTTIME]                                                  "
				  "				  ,sum(iif(Datediff(MINUTE,[START_TIME],[END_TIME])<0,24*60-Datediff(minute,end_time,start_time),Datediff(MINUTE,[START_TIME],[END_TIME]))) as Minutes      "
				  "			  FROM [276CoaterReports].[dbo].[WORKSHIFTDAYS] INNER JOIN [QueryLineStatusRemarks] ON [QueryLineStatusRemarks].ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY "
				  "			  WHERE DATE_OF_WORK BETWEEN @StartDate AND @StopDate                                                                                                           "
				  "			  Group by WORKSHIFTDAYS.ID_WORKSHIFTDAY,  ID_LINESTATUS , Date_Of_Work, Day([DATE_OF_WORK])                                                                       "
				  "			  Order by DATE_OF_WORK Asc                                                                                                                                     "
										;
				 Query->SQL->Add(QueryString);
				 Query->Parameters->ParamByName(":Start")->Value=Start;
				 Query->Parameters->ParamByName(":Stop")->Value = Stop;
			  //	 Query->Parameters->ParamByName("Start_Time")->Value=TDateTime(6,0,0,0);
				 Query->Open();
				  if (Query->RecordCount<=0) {
					ShowMessage("Nie zdefiniowano ¿adnych zdarzeñ na linii.\n\nPrawdopodobnie jest to sytuacja b³êdna!");
					//return NULL;
				 }
				 i=1;
				 EventType EventKind;

				 while(!Query->Eof)
				  {
						day = Query->FieldByName("Dzien")->AsInteger;
						//Stop  = Query->FieldByName("End_Time")->AsDateTime;
					  	//	EventKind = Query->FieldByName("ID_LINESTATUS")->AsInteger;
						MinDiffInt= Query->FieldByName("Minutes")->AsInteger; //MinuteDiff(Start,Stop);
						if (MinDiffInt==0) {                    //For some errors in the database
										 Query->Next();
										 continue;
						}
						ID =  Query->FieldByName("ID_LineStatus")->AsInteger;
						SumMonthly[day].AddEvent(ID, MinDiffInt);
//						switch (EventKind) {
//						case UnscheduledDowntime:
//												SumMonthly[day].AddUnscheduled(ID, MinDiffInt);
//												break;
//						case ScheduledDowntime:
//												SumMonthly[day].AddScheduled(ID, MinDiffInt);
//												break;
//						default:
//						 ;
//						}



				   }
				   Query->Close();
				//   return SumMonthly;


	  }
	  __finally{
				DataModule1->ADOConnectionCreport->Connected=false;
				FormMain->ComboBoxLineStatusSelect(FormMain);
	  }






}


void WhatToExport()
{
   char Monthy[12][4] = {"Jan", "Feb","Mar","Apr","May","Jun","Jul","Aug","Sep","Oct","Nov","Dec"};
   UnicodeString FileName;
   UnicodeString SheetName;
   int ShiftPosition;
   int Shift;
   CSummaryLineStatus Summary;
   CSummaryProduction GettingSummaryProduction(int); // prototype



   if(!Current.Existing())
   {
	   ShowMessage("Wczytaj lub wpisz dane dla prawid³owej zmiany");
	   return;
   }
   FormCoCExport->Visible=true;

   TComboBox* Combo = FormMain->ComboBoxLineStatus;
   FormCoCExport->Memo1->Clear();
   FormCoCExport->Memo1->Lines->Add(Current.GetDescription().TrimLeft());
   EmptySheet(FormCoCExport->StringGridBeforeExport);
   EmptySheet(FormCoCExport->StringGridFrom);
   EmptySheet(FormCoCExport->StringGridProductPrepared);
   EmptySheet(FormCoCExport->StringGridProductsForCoC);
   EmptySheet(FormCoCExport->StringGrid2);
   //EmptySheet(FormCoCExport->StringGridTo);
   unsigned short day,month, year;
   Current.GetDate().DecodeDate(&year,&month,&day);
   FileName =  "CoC_"+UnicodeString(year-2000);
   Shift = Current.GetShiftLetter()-'D'+4;					// 'A' = 1, 'B'=2, 'C'=3, 'D'=4
															//FormatSettings.ShortMonthNames[month-1]
   SheetName = UnicodeString(Monthy[month-1])+UnicodeString(year-2000);
   FormCoCExport->Memo1->Lines->Add("Plik: " + FileName);
   FormCoCExport->Memo1->Lines->Add("Zak³adka: " + SheetName);
   ShiftPosition = (day-1)*4+Shift;
   FormCoCExport->Memo1->Lines->Add("Przesuniêcie = " + UnicodeString(ShiftPosition));
   CurrentSummaryProduction = GettingSummaryProduction(Current.GetID());
   Summary=GettingSummary(Current.GetID());
   CurrentSumka= Summary;
   PrintingSummary(&Summary);
   PrintingSummaryProduction(&CurrentSummaryProduction);
   CoCDataPrepared = false;

//
//   int i=1;
//   std::pair<int,double> Outcome;
//   std::map<int,int>::iterator pos;
//   UnicodeString EventStr;
//   int CurrWidth, MaxWidth;
//   CurrWidth=0;
//   MaxWidth=FormCoCExport->StringGridFrom->ColWidths[0];
//   Summary.ResetIterator();
//   do
//   {
//	  Outcome =Summary.GetNextEventSummary();
//	  for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos)
//	  {
//		  if (pos->second == Outcome.first) {
//				  EventStr= Combo->Items->Strings[pos->first];
//				  break;
//		  }
//	  }
//
//	   FormCoCExport->StringGridFrom->Cells[0][i]=EventStr;
//	   CurrWidth= FormCoCExport->StringGridFrom->Canvas->TextWidth(EventStr);
//
//	   MaxWidth = MaxWidth > CurrWidth ? MaxWidth : CurrWidth;
//
//	   FormCoCExport->StringGridFrom->ColWidths[0] = MaxWidth;
//	   FormCoCExport->StringGridFrom->Cells[1][i]=Outcome.second;
//	   i++;
//   } while(Summary.IsNextEvent);
//   Summary.IsNextEvent = true;
//   FormCoCExport->StringGrid2->RowCount = FormMain->StringGridLineStatus->RowCount;
//   FormCoCExport->StringGrid2->ColCount = FormMain->StringGridLineStatus->ColCount;
//   for (int i = 0; i < FormMain->StringGridLineStatus->RowCount; i++) {
//	 FormCoCExport->StringGrid2->Rows[i] = FormMain->StringGridLineStatus->Rows[i];
//   }
//   for (int i = 0; i < FormCoCExport->StringGrid2->ColCount; i++) {
//	  FormCoCExport->StringGrid2->ColWidths[i] = FormMain->StringGridLineStatus->ColWidths[i];
//   }
//   CurrentSumka=Summary;
   return;
}

void ClearCoCEvents(int Col, Variant Sheet)
{
  std::map<int,int>::iterator it;
  it = CSummaryLineStatus::LineIDToCoCCell.begin();
  int Row;
  Variant Cell;
  while(it!=CSummaryLineStatus::LineIDToCoCCell.end())
  {
   Row = it->second;
   if(Row!=0)
   {
	  Cell = Sheet.OlePropertyGet("Cells",Row,Col);
	  Cell.OleProcedure("ClearContents");
	  //Sheet.OlePropertySet("Cells",Row,Col,NULL);
   }
   it++;
  }
}

void ClearCoCProducts(int ColOffset, int RowOffset, Variant& Sheet) //Col describes shift and Row day in the month
{
   std::map<int,UnicodeString>::iterator it;
   it = CSummaryProduction::IDtoCoCCell.begin();
   std::pair<int,int> Base;
   UnicodeString Cell;
   while(it!=CSummaryProduction::IDtoCoCCell.end())
   {
	 Cell = it->second;
	 Base = ExcelColRow( Cell);
	 Sheet.OlePropertySet("Cells",Base.second+RowOffset,Base.first+ColOffset,0);
	 Sheet.OlePropertySet("Cells",Base.second+RowOffset,Base.first+ColOffset+1,0);
	 it++;
   }
}

void ExportDailyReportToCoC()
{
  try
  {
	if(!CoCDataPrepared)
	{
		Application->MessageBox(L"Przed eksportem danych proszê uruchomiæ przycisk \"Przygotuj dane\" ",L"Uwaga!",MB_OK);
         return;
	}
	UnicodeString FileName;
	UnicodeString SheetName;
	int ShiftPosition;
	int Shift;
	unsigned short day,month, year;
	Current.GetDate().DecodeDate(&year,&month,&day);
	unsigned short YY, MM, DD;
	Current.GetDate().DecodeDate(&YY,&MM,&DD);
	FileName = CGlobalVariables::CoCPath;
	FileName +="\\"+UnicodeString(YY)+"\\";
	FileName +=  "CoC_"+UnicodeString(YY-2000);
	FileName += ".xls";
	Shift = Current.GetShiftLetter()-'D'+4;					// 'A' = 1, 'B'=2, 'C'=3, 'D'=4
	SheetName = UnicodeString(FormatSettings.ShortMonthNames[MM-1])+UnicodeString(YY-2000);
	ShiftPosition = (DD-1)*4+Shift;
	ExcelDocuments.Clear();
	if (ExcelApp.IsEmpty())
	{
		ExcelApp=CreateOleObject("Excel.Application");
	}
	else
	{
		ExcelApp=GetActiveOleObject("Excel.Application");
	}

	if (ExcelApp.IsEmpty())
	{
		ShowMessage("Nie mo¿na otworzyæ programu Excel!\n\nUnable to find Excel application.");
		return;
	}

	Variant secAutomation ;
	Variant Buf;
	secAutomation = ExcelApp.OlePropertyGet(L"AutomationSecurity");
	Buf = "msoAutomationSecurityForceDisable";
	ExcelApp.OlePropertySet(L"AutomationSecurity",3);
	//ExcelApp.OlePropertySet(L"AutomationSecurity",Buf);


	ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");//,\\(Variant)"CoC_17.xls");

	WideString Nazwa=FileName;

	if(!ExcelDocuments.IsEmpty())
	{
	   ExcelDocuments= ExcelDocuments.OleFunction(L"Open",Nazwa,0,0);
	}

	ExcelApp.OlePropertySet(L"AutomationSecurity", (Variant)secAutomation);

	if(!Current.Existing())
	{
		Application->MessageBoxA(L"Proszê o wczytanie danych zmiany, której przebieg ma byæ wpisany do akrusza CoC "
								"\n\nThe data of the Shift you are about to write to CoC spreadsheet should be read first!",L"Uwaga");
		return;
	}




//	 int Column =  ('O'-'A')+1;  //  ('O'-'A') - Offset when the entering data zone begins
//	 Column +=  (DD-1)*4;   //  (DD-1)*4 - Days begins every four cells
//	 Column += Current.GetShiftLetter()-'A'; // Shift of the shift within the day
	 CurrentSumka.ResetIterator();

	 std::pair<int,double> wyn;

	 double LS_Sum;
	 int Row ;  // 14 means column were the data for
	 int Column =  ('O'-'A')+1;  //  ('O'-'A') - Offset when the entering data zone begins

	 Buf = "Sheets";
	 ExcelSheet = ExcelDocuments.OlePropertyGet(Buf,13-MM);

	 //ExcelSheet = ExcelDocuments.OlePropertyGet("Worksheets");
	 ExcelSheet.OleProcedure(L"Activate");
	 Column +=  (DD-1)*4;   //  (DD-1)*4 - Days begins every four cells
	 Column += Current.GetShiftLetter()-'A'; // Shift of the shift within the day
//	 if(!CurrentSumka.IsNextEvent)
//	 {
//	  return;
//	 }
	 if(!CurrentSumka.IsNextCoC)
	 {
	  return;
	 }
//	 do
//	  {
//		  wyn=CurrentSumka.GetNextEventSummary();
//		  Row =CurrentSumka.GetCoCRow(wyn.first);
//		  LS_Sum = wyn.second;
//		//  FormCoCExport->StringGridBeforeExport->Cells[scol][srow] = CurrentSumka->CoCCellToCategory[Row];
//		  ExcelSheet.OlePropertySet("Cells",Row,Column,LS_Sum);
////		  FormCoCExport->StringGridBeforeExport->Cells[scol+1][srow] = Row;
////		  FormCoCExport->StringGridBeforeExport->Cells[scol+2][srow] = ExcelChar(Column,Row);
////		  FormCoCExport->StringGridBeforeExport->Cells[scol+3][srow] = LS_Sum;
////		  srow++;
//	  } while(CurrentSumka.IsNextEvent) ;

	 ClearCoCEvents(Column, ExcelSheet);

//	  std::map<int,int>::iterator it;
//	  it = CSummaryLineStatus::LineIDToCoCCell.begin();
//	  int RowCoC;
//	  while(it!=CSummaryLineStatus::LineIDToCoCCell.end())
//	  {
//	   RowCoC = it->second;
//	   if(RowCoC!=0)
//	   {
//		  ExcelSheet.OlePropertySet("Cells",RowCoC,Column,"");
//	   }
//	   it++;
//	  }




	 do
	  {
		  wyn=CurrentSumka.GetNextEventCoCSummary();
		  Row =wyn.first;
		  LS_Sum = wyn.second;
		  ExcelSheet.OlePropertySet("Cells",Row,Column,LS_Sum);
	  } while(CurrentSumka.IsNextCoC) ;


	   ExcelSheet.OlePropertyGet("Cells",9,Column).OleProcedure(L"Activate");
	  // Writing production data
	  Shift = Current.GetShiftLetter()-'D'+4;					// 'A' = 1, 'B'=2, 'C'=3, 'D'=4
	  CurrentSummaryProduction.ResetIterator();
	  std::pair<double,double> InOut;
	  int StartColumn;   //  (
	  //Column += Current.GetShiftLetter()-'A'; // Shift of the shift within the day
	  int RowOffset, ColOffset;
	  ColOffset  =  7+(Current.GetShiftKind()-1)*5;
	  std::pair<int,int> Offsets;
	  std::pair<double,double> Production;
	  int ProdID;
	  Variant ShiftLett = WideString(Current.GetShiftLetter());
	  ClearCoCProducts(ColOffset+1,DD,ExcelSheet);
	  while(CurrentSummaryProduction.IsNextProduct)
	  {
		  ProdID = CurrentSummaryProduction.GetNextProduct();
		  Offsets = ExcelColRow( CurrentSummaryProduction.GetCoCRow(ProdID));
		  RowOffset = Offsets.second;
		  StartColumn = Offsets.first;
		  Column = StartColumn + ColOffset;
		  Row = RowOffset+DD;

		  Production = CurrentSummaryProduction.GetProduct(ProdID);

		  ExcelSheet.OlePropertySet("Cells",Row,Column,ShiftLett);
		  ExcelSheet.OlePropertySet("Cells",Row,Column+1,Production.first);
		  ExcelSheet.OlePropertySet("Cells",Row,Column+2,Production.second);
	  };





	 ExcelApp.OlePropertySet("visible",true);
  //	 secAutomation = ExcelApp.OlePropertyGet(L"AutomationSecurity");
  //	 Buf = "msoAutomationSecurityForceDisable";

  //********** 4 next lines were uncommented but macro did not run even if
  //********** AutomationSecurity were set to 1 (find msoAutomationSecurity Enumeration)

  //	 ExcelApp.OlePropertySet(L"AutomationSecurity",1);//secAutomation
  //	 Buf = FileName+"!Start";
  //	 ExcelApp.OleProcedure(L"Run",Buf);
  //	 Buf = "Eksport z Coater Report" ;

//	secAutomation = ExcelApp.OlePropertyGet(L"AutomationSecurity");
//	Buf = "msoAutomationSecurityForceDisable";
//	ExcelApp.OlePropertySet(L"AutomationSecurity",3);
//
//	ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");//,\\(Variant)"CoC_17.xls");
//
//	WideString Nazwa=FileName;
//
//	if(!ExcelDocuments.IsEmpty())
//	{
//	   ExcelDocuments= ExcelDocuments.OleFunction(L"Open",Nazwa,0);
//	}

//	ExcelApp.OlePropertySet(L"AutomationSecurity", (Variant)secAutomation);
//	Buf = FileName+"!Start";
//	ExcelApp.OleProcedure(L"Run",Buf);

	 //ExcelSheet.OlePropertySet("Name",Buf);
  }
  __finally
  {
	 if (!ExcelApp.IsEmpty())
	 {
		   ExcelSheet = Unassigned;
		   ExcelSheet.Clear();
		   ExcelDocuments = Unassigned;
		   ExcelDocuments=Variant().Empty();
		   ExcelApp = Unassigned;
		   ExcelApp= Variant().Empty();
	 }
  }
//  }
//  __finally
//  {
//		MSWord.Exec(QuitFalse);
//		MSWord = Unassigned;
//
//  }
}


void ExportShiftPareto()
{
  try
  {
//	if(!CoCDataPrepared)
//	{
//		Application->MessageBox(L"Przed eksportem danych proszê uruchomiæ przycisk \"Przygotuj dane\" ",L"Uwaga!",MB_OK);
//         return;
//	}
	UnicodeString FileName;
	UnicodeString SheetName;
	int ShiftPosition;
	int Shift;
	int OffsetX, OffsetY; //Starting Cells of Excel spreadsheet.
	unsigned short day,month, year,week;

	week = WeekOfTheYear(Current.GetDate());
	day = DayOfTheWeek(Current.GetDate());
	unsigned short YY, MM, DD;
	//Current.GetDate().DecodeDate(&YY,&MM,&DD);
	FileName = CGlobalVariables::ShiftParetoFile;

	Shift = Current.GetShiftLetter()-'D'+4;					// 'A' = 1, 'B'=2, 'C'=3, 'D'=4
	SheetName = "W"+UnicodeString(week);
	OffsetX = 3;
	OffsetY=5;
	ShiftPosition = OffsetX+(day-1)*3+1+(fmod(Current.GetShiftKind(), 3)); //Shift kind 1 - morning 2-afternoon 3-night, and it should be 2-morning, 3-afternoon and 1-night.
	ExcelDocuments.Clear();
	if (ExcelApp.IsEmpty())
	{
		ExcelApp=CreateOleObject("Excel.Application");
	}
	else
	{
		ExcelApp=GetActiveOleObject("Excel.Application");
	}

	if (ExcelApp.IsEmpty())
	{
		ShowMessage("Nie mo¿na otworzyæ programu Excel!\n\nUnable to find Excel application.");
		return;
	}

	Variant secAutomation ;
	Variant Buf;
	//secAutomation = ExcelApp.OlePropertyGet(L"AutomationSecurity");
   //	Buf = "msoAutomationSecurityForceDisable";
   //	ExcelApp.OlePropertySet(L"AutomationSecurity",3);
	//ExcelApp.OlePropertySet(L"AutomationSecurity",Buf);


	ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");//,\\(Variant)"CoC_17.xls");

	WideString Nazwa=FileName;

	if(!ExcelDocuments.IsEmpty())
	{
	   ExcelDocuments= ExcelDocuments.OleFunction(L"Open",Nazwa,0,0);
	}

   //	ExcelApp.OlePropertySet(L"AutomationSecurity", (Variant)secAutomation);

	if(!Current.Existing())
	{
		Application->MessageBoxA(L"Proszê o wczytanie danych zmiany, której przebieg ma byæ wpisany do akrusza CoC "
								"\n\nThe data of the Shift you are about to write to CoC spreadsheet should be read first!",L"Uwaga");
		return;
	}




//	 int Column =  ('O'-'A')+1;  //  ('O'-'A') - Offset when the entering data zone begins
//	 Column +=  (DD-1)*4;   //  (DD-1)*4 - Days begins every four cells
//	 Column += Current.GetShiftLetter()-'A'; // Shift of the shift within the day
	 CurrentSumka.ResetIteratorUnscheduled();

	 std::pair<int,double> wyn;

	 double LS_Sum;
	 int Row ;  // 14 means column were the data for
	 int Column =  ('O'-'A')+1;  //  ('O'-'A') - Offset when the entering data zone begins

	 Buf = "Sheets";
	 ExcelSheet = ExcelDocuments.OlePropertyGet(Buf,SheetName);

	 //ExcelSheet = ExcelDocuments.OlePropertyGet("Worksheets");
	 ExcelSheet.OleProcedure(L"Activate");
//	 Column +=  (DD-1)*4;   //  (DD-1)*4 - Days begins every four cells
//	 Column += Current.GetShiftLetter()-'A'; // Shift of the shift within the day
//	 if(!CurrentSumka.IsNextEvent)
//	 {
//	  return;
//	 }
	 if(!CurrentSumka.IsNextCoC)
	 {
	  return;
	 }
//	 do
//	  {
//		  wyn=CurrentSumka.GetNextEventSummary();
//		  Row =CurrentSumka.GetCoCRow(wyn.first);
//		  LS_Sum = wyn.second;
//		//  FormCoCExport->StringGridBeforeExport->Cells[scol][srow] = CurrentSumka->CoCCellToCategory[Row];
//		  ExcelSheet.OlePropertySet("Cells",Row,Column,LS_Sum);
////		  FormCoCExport->StringGridBeforeExport->Cells[scol+1][srow] = Row;
////		  FormCoCExport->StringGridBeforeExport->Cells[scol+2][srow] = ExcelChar(Column,Row);
////		  FormCoCExport->StringGridBeforeExport->Cells[scol+3][srow] = LS_Sum;
////		  srow++;
//	  } while(CurrentSumka.IsNextEvent) ;

	 ClearCoCEvents(Column, ExcelSheet);

//	  std::map<int,int>::iterator it;
//	  it = CSummaryLineStatus::LineIDToCoCCell.begin();
//	  int RowCoC;
//	  while(it!=CSummaryLineStatus::LineIDToCoCCell.end())
//	  {
//	   RowCoC = it->second;
//	   if(RowCoC!=0)
//	   {
//		  ExcelSheet.OlePropertySet("Cells",RowCoC,Column,"");
//	   }
//	   it++;
//	  }




	 do
	  {
		  wyn=CurrentSumka.GetNextEventCoCSummary();
		  Row =wyn.first;
		  LS_Sum = wyn.second;
		  ExcelSheet.OlePropertySet("Cells",Row,Column,LS_Sum);
	  } while(CurrentSumka.IsNextCoC) ;


	   ExcelSheet.OlePropertyGet("Cells",9,Column).OleProcedure(L"Activate");
	  // Writing production data
	  Shift = Current.GetShiftLetter()-'D'+4;					// 'A' = 1, 'B'=2, 'C'=3, 'D'=4
	  CurrentSummaryProduction.ResetIterator();
	  std::pair<double,double> InOut;
	  int StartColumn;   //  (
	  //Column += Current.GetShiftLetter()-'A'; // Shift of the shift within the day
	  int RowOffset, ColOffset;
	  ColOffset  =  7+(Current.GetShiftKind()-1)*5;
	  std::pair<int,int> Offsets;
	  std::pair<double,double> Production;
	  int ProdID;
	  Variant ShiftLett = WideString(Current.GetShiftLetter());
	  ClearCoCProducts(ColOffset+1,DD,ExcelSheet);
	  while(CurrentSummaryProduction.IsNextProduct)
	  {
		  ProdID = CurrentSummaryProduction.GetNextProduct();
		  Offsets = ExcelColRow( CurrentSummaryProduction.GetCoCRow(ProdID));
		  RowOffset = Offsets.second;
		  StartColumn = Offsets.first;
		  Column = StartColumn + ColOffset;
		  Row = RowOffset+DD;

		  Production = CurrentSummaryProduction.GetProduct(ProdID);

		  ExcelSheet.OlePropertySet("Cells",Row,Column,ShiftLett);
		  ExcelSheet.OlePropertySet("Cells",Row,Column+1,Production.first);
		  ExcelSheet.OlePropertySet("Cells",Row,Column+2,Production.second);
	  };





	 ExcelApp.OlePropertySet("visible",true);
  //	 secAutomation = ExcelApp.OlePropertyGet(L"AutomationSecurity");
  //	 Buf = "msoAutomationSecurityForceDisable";

  //********** 4 next lines were uncommented but macro did not run even if
  //********** AutomationSecurity were set to 1 (find msoAutomationSecurity Enumeration)

  //	 ExcelApp.OlePropertySet(L"AutomationSecurity",1);//secAutomation
  //	 Buf = FileName+"!Start";
  //	 ExcelApp.OleProcedure(L"Run",Buf);
  //	 Buf = "Eksport z Coater Report" ;

//	secAutomation = ExcelApp.OlePropertyGet(L"AutomationSecurity");
//	Buf = "msoAutomationSecurityForceDisable";
//	ExcelApp.OlePropertySet(L"AutomationSecurity",3);
//
//	ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");//,\\(Variant)"CoC_17.xls");
//
//	WideString Nazwa=FileName;
//
//	if(!ExcelDocuments.IsEmpty())
//	{
//	   ExcelDocuments= ExcelDocuments.OleFunction(L"Open",Nazwa,0);
//	}

//	ExcelApp.OlePropertySet(L"AutomationSecurity", (Variant)secAutomation);
//	Buf = FileName+"!Start";
//	ExcelApp.OleProcedure(L"Run",Buf);

	 //ExcelSheet.OlePropertySet("Name",Buf);
  }
  __finally
  {
	 if (!ExcelApp.IsEmpty())
	 {
		   ExcelSheet = Unassigned;
		   ExcelSheet.Clear();
		   ExcelDocuments = Unassigned;
		   ExcelDocuments=Variant().Empty();
		   ExcelApp = Unassigned;
		   ExcelApp= Variant().Empty();
	 }
  }
//  }
//  __finally
//  {
//		MSWord.Exec(QuitFalse);
//		MSWord = Unassigned;
//
//  }
}








bool  CheckForShiftCoC(int Workshift_ID)
{

}



 void CSummaryLineStatus::AddEvent(int IDLineStatus, double Minutes)
 {
	 this->SummaryLine[IDLineStatus]=SummaryLine[IDLineStatus]+Minutes;
	 int Row = this->LineIDToCoCCell[IDLineStatus];
	 if (Row!=0) {
		 this->SummaryCoC[Row] += Minutes;
	 }
	 this->IsNextEvent = true;
 }

 void CSummaryLineStatus::AddUnscheduled(int IDLineStatus, double Minutes)
 {
	this->UnscheduledDowntimeLine[IDLineStatus]=UnscheduledDowntimeLine[IDLineStatus]+Minutes;
	this->IsNextUnscheduled = true;
 }

 void CSummaryLineStatus::AddScheduled(int IDLineStatus, double Minutes)
 {
	this->ScheduledDowntimeLine[IDLineStatus]=ScheduledDowntimeLine[IDLineStatus]+Minutes;
	this->IsNextScheduled = true;
 }

 void CSummaryLineStatus::DeleteEvent(int IDLineStatus)
 {
	   this->SummaryLine.erase(IDLineStatus);
	 //  this->SummaryCoC.erase(LineIDToCoCCell[IDLineStatus]);

 }

//Method returns pair having next shifts  LineStatus in first property and second Summary of minutes for given LineStatus in second property
 std::pair<int,double> CSummaryLineStatus::GetNextEventSummary()
 {
		std::pair<int,double> wyn;
		IsNextEvent=true;
		if(it==SummaryLine.end())
		{
			  IsNextEvent=false;
			  wyn.first= 0;
			  wyn.second=0;
			  it=SummaryLine.begin();
			  return wyn;
		}
		else
		{
			wyn.first= it->first;
			wyn.second=it->second;	//(it.left,it.right);
			it++;
			if(it==SummaryLine.end())
			{
				  IsNextEvent=false;
				  it=SummaryLine.begin();
			}
			return wyn;
		}
 }

std::pair<int,double> CSummaryLineStatus::GetNextEventCoCSummary()
 {
		std::pair<int,double> wyn;
		IsNextCoC=true;
		if(itCoC==SummaryCoC.end())
		{
			  IsNextCoC=false;
			  wyn.first= 0;
			  wyn.second=0;
			  itCoC=SummaryCoC.begin();
			  return wyn;
		}
		else
		{
			  wyn.first=itCoC->first;
			  wyn.second=itCoC->second;
			  itCoC++;
			  if(itCoC==SummaryCoC.end())
			  {
					IsNextCoC=false;
					itCoC=SummaryCoC.begin();
			  }
			  return wyn;
		}
 }



void EmptySheet(TStringGrid* ClearedSheet)
{
	for(int i=1;i<=ClearedSheet->RowCount;i++)
		ClearedSheet->Rows[i]->Clear();
}

CSummaryLineStatus GettingSummary(int WS_ID)
 {
//	Picker = FormMain->DateTimePickerLineStatusStart;
//	PickerStop = FormMain->DateTimePickerLineStatusStop;
//	Contact = FormMain->EditMinuteLineStatus ;//Contact person
//	Remarks = FormMain->MemoLineStatus;
//	Combo = FormMain->ComboBoxLineStatus;
	CSummaryLineStatus Sumka;
	TStringGrid* Sheet;
	int MinuteDiff(const TDateTime, const TDateTime ) ;
	Sheet = FormCoCExport->StringGridFrom;
	TDateTime Start;
	TDateTime Stop;
	static TDateTime LastTime;
	try{
		TADOQuery* Query =DataModule1->ADOQueryCreport;
		DataModule1->ADOConnectionCreport->Connected=true;
		AnsiString QueryString;
		EmptySheet(Sheet);
		int i=1;
		int ID;
		int MinDiffInt;
		std::map<int,int>::iterator pos;
		//Current.SetSumOfMinutes(0);
		Query->SQL->Clear();
//		if(Current.GetShiftKind()==3)  // night shift
//		{
				 QueryString =
							 "SELECT * FROM " +DataQuery + // :Database "
							 " WHERE  ID_Workshiftday = :ID_Workshiftday "
						//	 " AND  Start_Time > :Start_Time "
						//	 " ORDER BY Start_Time "
										;
				 Query->SQL->Add(QueryString);
				 Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=WS_ID;
			  //	 Query->Parameters->ParamByName("Start_Time")->Value=TDateTime(6,0,0,0);
				 Query->Open();
				  if (Query->RecordCount<=0) {
					ShowMessage("Nie zdefiniowano ¿adnych zdarzeñ na linii.\n\nPrawdopodobnie jest to sytuacja b³êdna!");
					//return NULL;
				 }
				 i=1;
				 EventType EventKind;

				 while(!Query->Eof)
				  {
						Start = Query->FieldByName("Start_Time")->AsDateTime;
						Stop  = Query->FieldByName("End_Time")->AsDateTime;
						EventKind = Query->FieldByName("MainID")->AsInteger;
						MinDiffInt=MinuteDiff(Start,Stop);
						if (MinDiffInt==0) {                    //For some errors in the database
										 Query->Next();
										 continue;
						}
						ID =  Query->FieldByName("ID_LineStatus")->AsInteger;
						Sumka.AddEvent(ID, MinDiffInt);
						switch (EventKind) {
						case UnscheduledDowntime:
												Sumka.AddUnscheduled(ID, MinDiffInt);
												break;
						case ScheduledDowntime:
												Sumka.AddScheduled(ID, MinDiffInt);
												break;
						default:
						 ;
						}


//						Sheet->Cells[0][i]= Start.FormatString("hh:nn");   //TimeString();
//						Sheet->Cells[1][i]= Stop.FormatString("hh:nn");//TimeString();
//						Sheet->Cells[2][i]= MinDiffInt=MinuteDiff(Start,Stop);
//						for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos) {
//								if (pos->second == ID) {
//								  Sheet->Cells[3][i]= Combo->Items->Strings[pos->first];
//								  break;
//								}
//						}
				//		Sheet->Cells[4][i]= ProductsExtended.FindByID(Query->FieldByName("ID_ProdType")->AsInteger).ShortName;
				//
				//		Sheet->Cells[5][i]= Query->FieldByName("Additional")->AsString;
				//		Sheet->Cells[4][i]= Query->FieldByName("Remarks")->AsString;
						/* TODO : Add Description */

//						i++;
						Query->Next();
					 //	Current.SetSumOfMinutes(Current.GetSumOfMinutes() + MinDiffInt);
//						if(i>=Sheet->RowCount)
//							Sheet->RowCount=Sheet->RowCount+1;
				   }
				   Query->Close();
				   return Sumka;

//				   QueryString =
//							 "SELECT * FROM " +DataQuery + // :Database "
//							 " WHERE  ID_Workshiftday = :ID_Workshiftday "
//							 " AND  Start_Time < :Start_Time  "
//							 " ORDER BY Start_Time "
//										;
//				 Query->SQL->Clear();
//				 Query->SQL->Add(QueryString);
//				 Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
//				 Query->Parameters->ParamByName("Start_Time")->Value=TDateTime(6,0,0,0);
//				 Query->Open();
//
//				 while(!Query->Eof)
//				   {
//						Start = Query->FieldByName("Start_Time")->AsDateTime;
//						Stop  = Query->FieldByName("End_Time")->AsDateTime;
//						Sheet->Cells[0][i]= Start.FormatString("hh:nn");   //TimeString();
//						Sheet->Cells[1][i]= Stop.FormatString("hh:nn");//TimeString();
//						Sheet->Cells[2][i]= MinDiffInt=MinuteDiff(Start,Stop);
//						ID =  Query->FieldByName("ID_LineStatus")->AsInteger;
//						for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos) {
//								if (pos->second == ID) {
//								  Sheet->Cells[3][i]= Combo->Items->Strings[pos->first];
//								  break;
//								}
//						}
//						Sheet->Cells[4][i]= ProductsExtended.FindByID(Query->FieldByName("ID_ProdType")->AsInteger).ShortName;
//
//						Sheet->Cells[5][i]= Query->FieldByName("Additional")->AsString;
//						Sheet->Cells[6][i]= Query->FieldByName("Remarks")->AsString;
//						i++;
//						Query->Next();
//						Current.SetSumOfMinutes(Current.GetSumOfMinutes() + MinDiffInt);
//						if(i>=Sheet->RowCount)
//							Sheet->RowCount=Sheet->RowCount+1;
//				   }
//		}
//		//------------------   If there are different shifts than night
//		else
//		{
//			 QueryString =
//						 "SELECT * FROM " +DataQuery + // :Database "
//						 " WHERE  ID_Workshiftday = :ID_Workshiftday "
//						 " ORDER BY Start_Time "
//									;
//			 Query->Close();
//			 Query->SQL->Clear();
//			 Query->SQL->Add(QueryString);
//			 Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
//			 Query->Open();
//
//			 while(!Query->Eof)
//			   {
//					Start = Query->FieldByName("Start_Time")->AsDateTime;
//					Stop  = Query->FieldByName("End_Time")->AsDateTime;
//					Sheet->Cells[0][i]= Start.FormatString("hh:nn");   //TimeString();
//					Sheet->Cells[1][i]= Stop.FormatString("hh:nn");//TimeString();
//					Sheet->Cells[2][i]= MinDiffInt = MinuteDiff(Start,Stop);
//					ID =  Query->FieldByName("ID_LineStatus")->AsInteger;
//						for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos) {
//								if (pos->second == ID) {
//								  Sheet->Cells[3][i]= Combo->Items->Strings[pos->first];
//								  break;
//								}
//						}
//					Sheet->Cells[4][i]= ProductsExtended.FindByID(Query->FieldByName("ID_ProdType")->AsInteger).ShortName;
//					Sheet->Cells[5][i]= Query->FieldByName("Additional")->AsString;
//					Sheet->Cells[6][i]= Query->FieldByName("Remarks")->AsString;
//					i++;
//					Query->Next();
//					Current.SetSumOfMinutes(Current.GetSumOfMinutes() + MinDiffInt);
//					if(i>=Sheet->RowCount)
//						Sheet->RowCount=Sheet->RowCount+1;
//			   }
//		}
//	   Picker->Time=Stop;
//	   PickerStop->Time=GetShiftTime(Current.GetShiftKind(),1); //1 means end shift time
//	  // Sheet->Cells[1][i] = "Suma = ";
//	  // Sheet->Font->Style >> fsBold;
//	  // Sheet->Cells[2][i] = Current.GetSumOfMinutes();
//	  FormMain->LabelLineStatusSumOfMinutes->Refresh();
//	  UnicodeString Napis =  Current.GetSumOfMinutes();
//	  FormMain->LabelLineStatusSumOfMinutes->Caption = L"Sumaryczna iloœæ minut = " + Napis;
//
//	   FormMain->LabelLineStatusSumOfMinutes->Refresh();
//	   TGridRect R;
//		R.Left=0;
//		R.Top= i ;
//		R.Right= 5 ;
//		R.Bottom = i  ;
//		FormMain->LabelLineStatusSumOfMinutes->Refresh();
//		FormMain->LabelLineStatusSumOfMinutes->Visible=true;
//		Sheet->Selection=R;
	  }
	  __finally{
				DataModule1->ADOConnectionCreport->Connected=false;
				FormMain->ComboBoxLineStatusSelect(FormMain);
	  }

 }

 //std::pair<CSummaryLineStatus, CSummaryLineStatus>
 void CParetoData::GetLineStatuses(TDateTime StartDate, TDateTime StopDate)
 {
   //	CSummaryLineStatus SumkaUnscheduled;
   //	CSummaryLineStatus SumkaScheduled;
	//TStringGrid* Sheet;
	int MinuteDiff(const TDateTime, const TDateTime ) ;
	//Sheet = FormCoCExport->StringGridFrom;
	TDateTime Start;
	TDateTime Stop;

	static TDateTime LastTime;
	try{
		  TADOQuery* Query =DataModule1->ADOQueryCreport;
		  DataModule1->ADOConnectionCreport->Connected=true;
		  AnsiString QueryString;
//		  EmptySheet(Sheet);
		  int ID, CategoryID,Position;
		  int MinDiffInt;
		  UnicodeString EventStr;
		  std::map<int,int>::iterator pos;
		  Query->SQL->Clear();
		  StartPeriod = StartDate;
		  StopPeriod = StopDate;
		  ParetoDataPrepare();
		  QueryString =        "Exec dbo.Proc_LineStatuses :StartDate, :StopDate "
//							   "SELECT * FROM " +DataQuery + // :Database "
//							   " WHERE  ID_Workshiftday = :ID_Workshiftday "

										  ;
		  Query->SQL->Add(QueryString);
		  Query->Parameters->ParamByName("StartDate")->Value=StartDate;
		  Query->Parameters->ParamByName("StopDate")->Value=StopDate;
		  Query->Open();
		  if (Query->RecordCount<=0) {
			 ShowMessage("Nie zdefiniowano ¿adnych zdarzeñ na linii.\n\nPrawdopodobnie jest to sytuacja b³êdna!");
		  }
		  while(!Query->Eof)
		  {
				 Start = Query->FieldByName("Start_Time")->AsDateTime;
				 Stop  = Query->FieldByName("End_Time")->AsDateTime;

				 MinDiffInt=MinuteDiff(Start,Stop);
				 if (MinDiffInt==0) {                    //For some errors in the database
								  Query->Next();
								  continue;
				 }

				 ID =  Query->FieldByName("ID_LineStatus")->AsInteger;
				 CategoryID = Query->FieldByName("MainID")->AsInteger;
				 EventStr = Query->FieldByName("LINESTATUS_NAME")->AsString;
				 Position = EventStr.Pos("-");
				 if (Position > 0) {
				   EventStr = EventStr.SubString(1,Position-2);
				 }
				 if (EventStr.Pos("Burn")!=0)
				 {
                     EventStr = "BurnIn";
				 }
				 switch (CategoryID)
				 {
				   case 1:                      //Scheduled Downtime
							if(ID!=8) 			// Standby time
							{
								//SumkaScheduled.AddEvent(ID, MinDiffInt);
								ScheduledMap[EventStr]=ScheduledMap[EventStr]+MinDiffInt;
							}
							break;
				   case 6:                      //Unscheduled Downtime
							//SumkaUnscheduled.AddEvent(ID, MinDiffInt);
							UnscheduledMap[EventStr]=UnscheduledMap[EventStr]+MinDiffInt;
							break;

				   default:
					;
				 }

				 Query->Next();
		  }
		  Query->Close();
		  Query->SQL->Clear();
		  QueryString =        "Exec dbo.Proc_Defects :StartDate, :StopDate "
										  ;
		  Query->SQL->Add(QueryString);
		  Query->Parameters->ParamByName("StartDate")->Value=StartDate;
		  Query->Parameters->ParamByName("StopDate")->Value=StopDate;
		  Query->Open();
		  if (Query->RecordCount<=0) {
			 ShowMessage("Nie zapisano ¿adnych defektów w podanym okresie.\n\nPrawdopodobnie jest to sytuacja b³êdna!");
		  }

		  UnicodeString Defect, Additional;
		  double Area;
		  while(!Query->Eof)
		  {
			 Defect = Query->FieldByName("Defect")->AsString;
			 Area = Query->FieldByName("Area")->AsFloat;
			 if (!Query->FieldByName("Eng. Description")->IsNull)
			 {
			   Additional = Query->FieldByName("Eng. Description")->AsString;
			   if(Additional.Pos("Float") !=0)
			   {
				  Defect = Additional;
			   }
			   else
			   {
				  Defect = "Lami - " + Additional;
			   }
			 }
			 DefectsMap[Defect] = DefectsMap[Defect] + Area;
			 Query->Next();
		  }
	  }
	  __finally{
				DataModule1->ADOConnectionCreport->Connected=false;
				FormMain->ComboBoxLineStatusSelect(FormMain);
	  }

 }
 void CParetoData::PrintOut(TStringGrid* Sheet)
 {
	std::map<UnicodeString, double>::iterator ItUnscheduled, ItScheduled, ItDefects;
	int i =3;
	Sheet->Cells[0][2]="Unscheduled Events";
	Sheet->Cells[1][2]="Time (min)";
	Sheet->Cells[3][2]="Scheduled Events";
	Sheet->Cells[4][2]="Time (min)";
	Sheet->Cells[6][2]="Defects";
	Sheet->Cells[7][2]="Area (m^2)";
	Sheet->Cells[0][0]="Data for pareto from " + StartPeriod.DateTimeString() + " to " + StopPeriod.DateTimeString();
	Sheet->ColCount = 8;
	for(ItUnscheduled = UnscheduledMap.begin();ItUnscheduled!=UnscheduledMap.end();ItUnscheduled++)
	{
	  Sheet->Cells[0][i]= ItUnscheduled->first;
	  Sheet->Cells[1][i]= ItUnscheduled->second;

	  if(Sheet->RowCount<i) Sheet->RowCount += 20;
	  i++;
	}
	i=3;
	for( ItScheduled = ScheduledMap.begin(); ItScheduled != ScheduledMap.end();ItScheduled++)
	{
	  Sheet->Cells[3][i]= ItScheduled->first;
	  Sheet->Cells[4][i]= ItScheduled->second;
	  if(Sheet->RowCount<i) Sheet->RowCount += 20;
	  i++;
	}
	i=3;
	for( ItDefects = DefectsMap.begin(); ItDefects != DefectsMap.end();ItDefects++)
	{
	  Sheet->Cells[6][i]= ItDefects->first;
	  Sheet->Cells[7][i]= ItDefects->second;
	  if(Sheet->RowCount<i) Sheet->RowCount += 20;
	  i++;
	}
 }


 void CParetoData::ExportToExcel(UnicodeString ExcelFile)
 {
  try{
	try
	{
	  FormProgress->Show();
	  int StartingElements, MaxElements =  NumberOfElements(CategoryScheduled)+NumberOfElements(CategoryUnscheduled)+NumberOfElements(CategoryDefects);
	  StartingElements = MaxElements>100?90:100-MaxElements;
	  FormProgress->ProgressBar->Max=MaxElements+StartingElements;
	  FormProgress->ProgressBar->Min=0;
	  FormProgress->ProgressBar->Position=0;
	  ExcelDocuments.Clear();
	  ExcelApp.Clear();
	  FormProgress->ProgressBar->Position=FormProgress->ProgressBar->Position+int(StartingElements/3);
	  if (ExcelApp.IsEmpty())
	  {
		  ExcelApp=CreateOleObject("Excel.Application");
	  }
	  else
	  {
		  ExcelApp=GetActiveOleObject("Excel.Application");
	  }

	  if (ExcelApp.IsEmpty())
	  {
		  ShowMessage("Nie mo¿na otworzyæ programu Excel!\n\nUnable to find Excel application.");
		  return;
	  }
	  FormProgress->ProgressBar->Position=FormProgress->ProgressBar->Position+int(StartingElements/3);
	  Variant secAutomation ;
	  Variant Buf;
   //	secAutomation = ExcelApp.OlePropertyGet(L"AutomationSecurity");
   //	Buf = "msoAutomationSecurityForceDisable";
   //	ExcelApp.OlePropertySet(L"AutomationSecurity",3);
	  //ExcelApp.OlePropertySet(L"AutomationSecurity",Buf);


	  ExcelDocuments = ExcelApp.OlePropertyGet("Workbooks");//,\\(Variant)"CoC_17.xls");

	  WideString Nazwa=ExcelFile;

	  if(!ExcelDocuments.IsEmpty())
	  {
		 ExcelDocuments= ExcelDocuments.OleFunction(L"Open",Nazwa,0,0);
	  }

	 //	ExcelApp.OlePropertySet(L"AutomationSecurity", (Variant)secAutomation);
	  FormProgress->ProgressBar->Position=FormProgress->ProgressBar->Position+int(StartingElements/3);


	 //	 std::pair<int,double> wyn;

	//	 double LS_Sum;
	   int MaxRow=0 ;  // 14 means column were the data for
	 //	 int Column =  ('O'-'A')+1;  //  ('O'-'A') - Offset when the entering data zone begins

	   Buf = "Worksheets";
	   ExcelSheet = ExcelDocuments.OlePropertyGet(Buf,1);

	   ExcelSheet.OleProcedure(L"Activate");

	  std::map<UnicodeString, double>::iterator ItUnscheduled, ItScheduled, ItDefects;
	  int i =StartingRow;

	  WideString St;
	  double Number;
	  UnicodeString ForManipulation;
  //	St = "Unscheduled Events" ;
  //	ExcelSheet.OlePropertySet("Cells",1,ColUnscheduled,St);
  //	St = "Time (min)" ;
  //	ExcelSheet.OlePropertySet("Cells",1 ,ColUnscheduled+1,St);
  //	St =  "Scheduled Events";
  //	ExcelSheet.OlePropertySet("Cells",1 ,ColScheduled,St);
  //	St =  "Time (min)";
  //	ExcelSheet.OlePropertySet("Cells", 1,ColScheduled+1,St);
  //	St = "Defects" ;
  //	ExcelSheet.OlePropertySet("Cells",1 ,ColDefects,St);
  //	St = "Area (m^2)" ;
  //	ExcelSheet.OlePropertySet("Cells", 1,ColDefects+1,St);
	  Variant CellRange1, CellRange2;
	  int LastRow;

	  CellRange1 = ExcelSheet.OlePropertyGet("Cells",StartingRow,ColUnscheduled);
  //	CellRange1 = CellRange1.OlePropertyGet("End","xlDown");

	  CellRange2 = ExcelSheet.OlePropertyGet("Cells",MaxNumberOfCategories,ColUnscheduled+1);

	  //CellRange1 =
	  ExcelSheet.OlePropertyGet("Range",CellRange1,CellRange2).OleFunction("ClearContents");

	  CellRange1 = ExcelSheet.OlePropertyGet("Cells",StartingRow,ColScheduled);
	  CellRange2 = ExcelSheet.OlePropertyGet("Cells",MaxNumberOfCategories,ColScheduled+1);
	  ExcelSheet.OlePropertyGet("Range",CellRange1,CellRange2).OleFunction("ClearContents");

	  CellRange1 = ExcelSheet.OlePropertyGet("Cells",StartingRow,ColDefects);
	  CellRange2 = ExcelSheet.OlePropertyGet("Cells",MaxNumberOfCategories,ColDefects+1);
	  ExcelSheet.OlePropertyGet("Range",CellRange1,CellRange2).OleFunction("ClearContents");

	  //int NrOfSteps = NumberOfElements(Scheduled)+ NumberOfElements(Unscheduled)+NumberOfElements(Defects);
	  ExcelApp.OlePropertySet(L"Calculation",L"xlCalculationManual");
	  for(ItUnscheduled = UnscheduledMap.begin();ItUnscheduled!=UnscheduledMap.end();ItUnscheduled++)
	  {
		  St = ItUnscheduled->first ;
		  Number = ItUnscheduled->second ;
		  ExcelSheet.OlePropertySet("Cells",i ,ColUnscheduled,St);
		  ExcelSheet.OlePropertySet("Cells",i ,ColUnscheduled+1,Number);
		  FormProgress->ProgressBar->Position=FormProgress->ProgressBar->Position+1;
		  FormProgress->ProgressBar->Invalidate();
		//Sheet->Cells[0][i]= ;
		//Sheet->Cells[1][i]= ;

		//if(Sheet->RowCount<i) Sheet->RowCount += 20;
		i++;
	  }
	  MaxRow=MaxRow>i?MaxRow:i;
	  i=StartingRow;

	  for( ItScheduled = ScheduledMap.begin(); ItScheduled != ScheduledMap.end();ItScheduled++)
	  {
		  St =  ItScheduled->first ;
		  Number = ItScheduled->second  ;
		  ExcelSheet.OlePropertySet("Cells",i ,ColScheduled,St);
		  ExcelSheet.OlePropertySet("Cells",i ,ColScheduled+1,Number);
		  FormProgress->ProgressBar->Position=FormProgress->ProgressBar->Position+1;
		i++;
	  }
	  MaxRow=MaxRow>i?MaxRow:i;
	  i=StartingRow;
	  for( ItDefects = DefectsMap.begin(); ItDefects != DefectsMap.end();ItDefects++)
	  {

		St =  ItDefects->first ;
		Number =  ItDefects->second;
		ExcelSheet.OlePropertySet("Cells",i ,ColDefects,St);
		ExcelSheet.OlePropertySet("Cells",i ,ColDefects+1,Number);
		FormProgress->ProgressBar->Position=FormProgress->ProgressBar->Position+1;
		i++;
	  }
	  MaxRow=MaxRow>i?MaxRow:i;
	  MaxRow+=3;
	  ForManipulation = "Coater_pareto_from_" + StartPeriod.DateTimeString() + "_to_" + StopPeriod.DateTimeString();
  //	St = ForManipulation;
  //	 ExcelSheet.OlePropertySet("Cells",MaxRow ,ColUnscheduled,St);
	   ExcelApp.OlePropertySet("visible",true);
	   ExcelApp.OlePropertySet(L"Calculation",L"xlCalculationAutomatic");
	}
	catch(...)
	{
	 //throw;
	   ExcelApp.OlePropertySet(L"Calculation",L"xlCalculationAutomatic");
	   FormProgress->Close();
	   ExcelSheet = Unassigned;
	   ExcelSheet.Clear();
	   ExcelDocuments = Unassigned;
	   ExcelDocuments=Variant().Empty();
	   ExcelApp = Unassigned;
	   ExcelApp= Variant().Empty();
	   ExcelApp.Clear();
	}
  }
  __finally
  {
	   Sleep(1000);
	   ExcelApp.OlePropertySet(L"Calculation",L"xlCalculationAutomatic");
	   FormProgress->Close();
	   ExcelSheet = Unassigned;
	   ExcelSheet.Clear();
	   ExcelDocuments = Unassigned;
	   ExcelDocuments=Variant().Empty();
	   ExcelApp = Unassigned;
	   ExcelApp= Variant().Empty();
	   ExcelApp.Clear();
  }
 }


CSummaryProduction GettingSummaryProduction(int WorkShiftID)
{
	CSummaryProduction Sumka;
	double In, Out;
	try{
		TADOQuery* Query =DataModule1->ADOQueryCreport;
		DataModule1->ADOConnectionCreport->Connected=true;
		AnsiString QueryString;
		int i=1;
		int ID;
		Query->SQL->Clear();
		QueryString =
					"SELECT * FROM Prod_Summary                   "
					"WHERE  ID_Workshiftday = :ID_Workshiftday   "
							   ;
		Query->SQL->Add(QueryString);
		Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=WorkShiftID;
		Query->Open();
		i=1;
		if(Query->RecordCount <=0) // No production
		{
           return Sumka;
        }
		while(!Query->Eof)
		{

			  ID =  Query->FieldByName("ID_Prod_Type")->AsInteger;
			  In =  Query->FieldByName("TOTAL_IN")->AsFloat;
			  Out = Query->FieldByName("TOTAL_OUT")->AsFloat;
			  Sumka.AddProduct(ID,In,Out);		//   AddEvent(ID, MinDiffInt);
			  Query->Next();
		 }
		 Query->Close();
		 return Sumka;

	  }
	  __finally{
				DataModule1->ADOConnectionCreport->Connected=false;
				FormMain->ComboBoxLineStatusSelect(FormMain);
	  }

}


 void CSummaryLineStatus::FillUpCoCData()
 {
  try{
	TADOQuery* Query = DataModule1->ADOQueryCreport;
	DataModule1->ADOConnectionCreport->Connected=true;
//	TComboBox* Combo = FormMain->ComboBoxLineStatus;
	AnsiString QueryString,Name,LineCategoryName,CoCCategory,EnDescription;
	int ID, Category,LineID, CoCCell;
	bool IsActive;
	LineIDToCoCCell.clear();
	CoCCellToCategory.clear();
	//QueryString = "Select * from LogToCoC  ;" ;
	QueryString = "Select * from LogToCoC_V  ;" ;
	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
   //	LineStatuses = new TLineStatus(Query->RecordCount);
	for(int i=0;i<Query->RecordCount;i++)
	{
		LineID= Query->FieldByName("IdLog")->AsInteger;
		LineCategoryName = Query->FieldByName("DescriptionLog")->AsString;
		CoCCategory = Query->FieldByName("DescriptionCoC")->AsString;
		CoCCell  = Query->FieldByName("RowNoCoC")->AsInteger;
//		EnDescription = Query->FieldByName("EnglishDescription")->AsString;
//		Category =  Query->FieldByName("CategoryID")->AsInteger;
//		IsActive =  Query->FieldByName("Active")->AsBoolean;
//		if (IsActive) {
//		  Combo->Items->Add(Name) ;
//		  ComboLineStatusToID[Combo->Items->IndexOf(Name)]=ID;
//		}

		//UnicodeString aPolishName, UnicodeString aEnglishName, UnicodeString aPolishDescription, UnicodeString aEnglishDescription, int aCategoryID, bool aActive)
		//LineStatuses->Add(ID, Name, EnglishName, PlDescription,EnDescription,Category,IsActive);
		LineIDToCoCCell[LineID]=CoCCell;
		CoCCellToCategory[CoCCell]=CoCCategory;

		Query->Next();
	}
	}
	__finally{
			//DataModule1->ADOConnectionExchange->Connected=false;
			DataModule1->ADOConnectionCreport->Connected=false;
	}
 }


void CSummaryLineStatus::GatherDataForExport( CSummaryLineStatus* Sum)
 {
	  Sum->ResetIterator();
	  if (!Sum->IsNextEvent) {
		 ShowMessage("Nie zdefiniowano, ¿adnych zdarzeñ na lini.\n\nPrawdopodobnie jest to sytuacja b³êdna!");
		 return;
	  }
	  std::pair<int,double> wyn;
	  EmptySheet(FormCoCExport->StringGridBeforeExport);
	  double LS_Sum;
	  int srow=1,scol=0;
	  int Row ;
	  int Column =  ('O'-'A')+1;  //  ('O'-'A') - Offset when the entering data zone begins
	  unsigned short YY, MM, DD;
	  Current.GetDate().DecodeDate(&YY,&MM,&DD);
	  Column +=  (DD-1)*4;   //  (DD-1)*4 - Days begins every four cells
	  Column += Current.GetShiftLetter()-'A'; // Shift of the shift within the day
	  //ExcelSheet.OlePropertyGet("Cells",9,Column).OleProcedure(L"Activate");

	  do    //Set proper LineEvents i.e. eliminate unsupportet ones
	  {
		  wyn=Sum->GetNextEventSummary();
		  Row = Sum->LineIDToCoCCell[wyn.first];  // if there is no CoC LineStatus which coresponds to E-Loogbook the Row is equal to 0
		  LS_Sum = wyn.second;
		  int ResMod;
		  if(Row==0) // No LineStatus Category in CoC SpreadSheet
		  {
			 FormCoCincompatibility->EditIncompatiblileWithCoC->Text=FormMain->IdLineStatusToLineStatus(wyn.first);
			 //FormCoCincompatibility->Visible=true;
			 ResMod = FormCoCincompatibility->ShowModal();
			 if (ResMod>0) { // There were "przypisz" button choosen (sign to).
				Sum->DeleteEvent(wyn.first);         //delete incomatibile LineStatus
				Sum->AddEvent(ResMod,wyn.second);    // insert alternative one
				GatherDataForExport(Sum) ;
				return;
			 }
		  }
//		  FormCoCExport->StringGridBeforeExport->Cells[scol][srow] = Sum->CoCCellToCategory[Row];
//		  FormCoCExport->StringGridBeforeExport->Cells[scol+1][srow] = Row;
//
//		  FormCoCExport->StringGridBeforeExport->Cells[scol+2][srow] = ExcelChar(Column,Row);
//		  FormCoCExport->StringGridBeforeExport->Cells[scol+3][srow] = LS_Sum;
//		  srow++;
//		  FormCoCExport->StringGridBeforeExport->RowCount=srow;
	  } while(Sum->IsNextEvent) ;
	  srow=1,scol=0;
	  for (Sum->itCoC = Sum->SummaryCoC.begin(); Sum->itCoC != Sum->SummaryCoC.end(); Sum->itCoC++) {
		Row = Sum->itCoC->first;
		LS_Sum = Sum->itCoC->second;
        FormCoCExport->StringGridBeforeExport->Cells[scol][srow] = Sum->CoCCellToCategory[Row];
		FormCoCExport->StringGridBeforeExport->Cells[scol+1][srow] = Row;

		FormCoCExport->StringGridBeforeExport->Cells[scol+2][srow] = ExcelChar(Column,Row);
		FormCoCExport->StringGridBeforeExport->Cells[scol+3][srow] = LS_Sum;
		srow++;
		FormCoCExport->StringGridBeforeExport->RowCount=srow;
	  }

	  CoCDataPrepared = true;
 }

//Methods creates TStringList wich contains all CoC categories.
 TStringList* CSummaryLineStatus::GetCoCCategories()
 {
	TStringList* Lista = new TStringList();
	std::map<int,UnicodeString>::iterator it = CSummaryLineStatus::CoCCellToCategory.begin();
	for (it; it!=CSummaryLineStatus::CoCCellToCategory.end(); it++) {
	  if(it->first>0)
	  {
			Lista->Add(it->second);
	  }
	}
	return Lista;
 }

 int CSummaryLineStatus::ReturnLineStatusID(UnicodeString CoCCategory)
 {
	std::map<int,UnicodeString>::iterator it = CSummaryLineStatus::CoCCellToCategory.begin();
	for (it; it!=CSummaryLineStatus::CoCCellToCategory.end(); it++) {
	  if(it->second==CoCCategory)
	  {
			return ReturnLineStatusID(it->first);
	  }
	}
	return 0;  //If no category found
 }

 int CSummaryLineStatus::ReturnLineStatusID(int CoCRow)
 {
	 std::map<int,int>::iterator it = CSummaryLineStatus::LineIDToCoCCell.begin();
	for (it; it!=CSummaryLineStatus::LineIDToCoCCell.end(); it++) {
	  if(it->second==CoCRow)
	  {
			return it->first;
	  }
	}
	return 0;
 }

 void CSummaryProduction::FillUpProductsData()
 {
	try{
		TADOQuery* Query = DataModule1->ADOQueryCreport;
		DataModule1->ADOConnectionCreport->Connected=true;
		AnsiString QueryString;
		UnicodeString ProdType_Name,ProductsCoC, CoCCell;
		int  ID_PRODTYPE;
		bool IsActive;
		IDtoCoCCell.clear();
		QueryString = "SELECT * FROM CoCToProductID;" ;
		Query->SQL->Clear();
		Query->SQL->Add(QueryString);
		Query->Open();
		for(int i=0;i<Query->RecordCount;i++)
		{
			ID_PRODTYPE= Query->FieldByName("ID_PRODTYPE")->AsInteger;
			ProdType_Name = Query->FieldByName("ProdType_Name")->AsString;
			ProductsCoC = Query->FieldByName("ProductsCoC")->AsString;
			CoCCell  = Query->FieldByName("CoCCell")->AsString;
			IDtoCoCCell[ID_PRODTYPE]=CoCCell;
			IDtoProductName[ID_PRODTYPE]=ProductsCoC;
			IDtoShortProductName[ID_PRODTYPE]=ProdType_Name;

			Query->Next();
		}
	}
	__finally{
			DataModule1->ADOConnectionCreport->Connected=false;
	}


 }


  void CSummaryProduction::AddProduct(int ID_Product, double In, double Out)
  {
		double Num;
		Num = SummaryProduction[ID_Product].first; //get In summary
		Num +=In;
		SummaryProduction[ID_Product].first = Num;
		Num = SummaryProduction[ID_Product].second; //get Out summary
		Num +=Out;
		SummaryProduction[ID_Product].second = Num;

  }

  std::pair<double,double> CSummaryProduction::GetProduct(int ID_Product)
  {
		return SummaryProduction[ID_Product];
  }

  UnicodeString CSummaryProduction::GetProductName(int ID_Product,bool Preliminary)
  {
	if(!Preliminary)
	{
		return IDtoProductName[ID_Product];
	}
	else
	{
		return IDtoShortProductName[ID_Product];
	}
  }

  UnicodeString CSummaryProduction::GetShortProductName(int ID_Product)
  {
		 return IDtoShortProductName[ID_Product];
  }

  int CSummaryProduction::GetNextProduct()         // Return next produced product ID
  {
		std::pair<int, std::pair<double,double> > wyn;
		IsNextProduct=true;
		if(it==SummaryProduction.end())
		{
			  IsNextProduct=false;
			  wyn.first= 0;
			  it=SummaryProduction.begin();
			  return wyn.first;
		}
		else
		{
			wyn.first= it->first;
			it++;
			if(it==SummaryProduction.end())
			{
				  IsNextProduct=false;
				  it=SummaryProduction.begin();
			}
			return wyn.first;
		}
  }
