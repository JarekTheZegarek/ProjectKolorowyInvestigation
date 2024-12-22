//---------------------------------------------------------------------------


#pragma hdrstop


#include "UnitStartUpBooth.h"
//#include "DataCoater.h"
#include "IniFiles.hpp"
#include "UnitMainBooth.h"
#include "UnitReadShiftData.h"
#include "DataBooth.h"
#include "GlobalBooth.h"
#include "UnitProductsBooth.h"
//#include "UnitMain.h"
//#include "UnitProducts.h"
//#include "Global.h"
//#include "UnitLineStatus.h"
//#include "AccessCOM.h"


//---------------------------------------------------------------------------

#pragma package(smart_init)

extern TSuperviosorCompare  SupervisorsCompare;
int CurrentId = 0;

extern TCombos Combos;

UnicodeString CGlobalVariables::CreportPath ;
UnicodeString CGlobalVariables::ExchangePath = "";
UnicodeString CGlobalVariables::DefectsPath	= "";
UnicodeString CGlobalVariables::CoCPath =  "";     //used for
//Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source="C:\Documents and Settings\jsolecki\My Documents\Nowy Raport Coater'a\Creport.mdb";Mode=Share Deny None;Extended Properties="";Jet OLEDB:System database="";Jet OLEDB:Registry Path="";Jet OLEDB:Database Password="";Jet OLEDB:Engine Type=5;Jet OLEDB:Database Locking Mode=1;Jet OLEDB:Global Partial Bulk Ops=2;Jet OLEDB:Global Bulk Transactions=1;Jet OLEDB:New Database Password="";Jet OLEDB:Create System Database=False;Jet OLEDB:Encrypt Database=False;Jet OLEDB:Don't Copy Locale on Compact=False;Jet OLEDB:Compact Without Replica Repair=False;Jet OLEDB:SFP=False

 bool Local, Report, TestingPath ; // Local=1 means using local testing database, Report=1 means using reporting Facilities.
 bool TestingM = false;
//TLineStatus* LineStatuses;
TStringList* StringsProducts;        //StringLists for ComboBox depending on needs
TStringList* StringsProductsExtended;//StringLists for ComboBox depending on needs
TProdToLineStatus ProductsOnly;     //Object for ComboBox depending on needs
TProdToLineStatus ProductsExtended;  //Object for ComboBox depending on needs
std::map<int,int> LineIDToCoCCell;
std::map<int,UnicodeString> CoCCellToCategory;

void Read_Mode()
{
   TIniFile *ini= 0;
   try{
	   TestingM = false;
	   UnicodeString IniFile =  ExtractFilePath(Application->ExeName);
	   IniFile = IniFile + "RaportCT.ini"  ;
	   ini = new TIniFile(IniFile ); //;   ("ReportCT.ini");
	   Local = ini->ReadBool("Mode","Local",false);
	   Report = ini->ReadBool("Mode","Report",false);
	   TestingM = ini->ReadBool("Mode","Testing",false);
	   TestingPath=ini->ReadBool("Mode","TestingPath",false);
//	   if (Local) {
//				 FormMain->RadioGroupMode->ItemIndex=1;
//	   } else if (TestingPath) {
//				 FormMain->RadioGroupMode->ItemIndex=2;
//	   } else {
//				 FormMain->RadioGroupMode->ItemIndex=0;
//	   }
//	   if(ini->ReadBool("Mode","Mistrz",false))
//	   {
//		  FormMain->ButtonPrzepiszDoCoc->Visible=true;
//       }
	  //
   }
   __finally {
		delete ini;
   }
}

void SetPaths(AnsiString ModePath)
{
   TIniFile *ini= 0;
   try{
	   AnsiString CreportConnectionString,ExchangePath, DefectsPath;  //CreportPath,
	   AnsiString Local_CreportPath,Local_ExchangePath, Local_DefectsPath;
	   AnsiString Starting_CreportPath,Starting_ExchangePath, Starting_DefectsPath;
	   AnsiString ExchangeConnectionString, DefectsConnectionString;
	   UnicodeString CoCPath, TempPath;
 //	   CoCPath = GetCurrentDir();
 //	   FormMain->MemoTest->Lines->Add(CoCPath);
	   UnicodeString IniFile =  ExtractFilePath(Application->ExeName);
	   IniFile = IniFile + "RaportCT.ini"  ;
	   ini = new TIniFile(IniFile );

	   TempPath =  ini->ReadString( ModePath.c_str(), "Creport", "j:\\Coater_Report\\Creport.mdb" );
	   CGlobalVariables::CreportPath  =   TempPath ;
	   TempPath =ini->ReadString( ModePath.c_str(), "Creport_exchange", "j:\\Coater_Report\\Creport_exchange.mdb" );

	   CGlobalVariables::ExchangePath = TempPath;
	   TempPath =  ini->ReadString( ModePath.c_str(), "Defects", "j:\\Data\\GIP\\gip.mdb" );
	   CGlobalVariables::DefectsPath	= TempPath;
	   CGlobalVariables::CoCPath =  ini->ReadString( ModePath.c_str(), "CoC", "v:\\Coater_Process\\coater\\Production Issues\\Logbook\\" );
//	   if(TestingM)
//	   {
//			Mode.Set(TestingMode);
//			FormMain->ButtonPrintCurrent->Visible=true;
//			FormMain->PanelDevelopement->Visible= true;
//
//	   }
//	   else
//	   {
//		  FormMain->StringGridReport->Height=FormMain->PageControl1->ClientHeight - FormMain->StringGridReport->Top -10;
//		  FormMain->ScrollBoxReportPrint->Visible=false;
//		  FormMain->Button1->Visible=false;
//		  FormMain->ButtonFastReport->Visible=false;
//		  FormMain->StringGridReport->Width = FormMain->PageControl1->Width - 40;
//		  FormMain->RichEditReport->Visible=false;
//	   }
	   if (Report) {
		  // FormMain->PageControl1->Enabled=true;
	   }

	   CreportConnectionString = CGlobalVariables::CreportPath;
//	   CreportConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source=";
//	   CreportConnectionString+= "\""+CGlobalVariables::CreportPath+"\"";
//	   CreportConnectionString+= ";Mode=Share Deny None;Extended Properties=\"\";Jet OLEDB:System database=\"\";Jet OLEDB:Registry Path=\"\";Jet OLEDB:Database Password=\"\";Jet OLEDB:Engine Type=5;Jet OLEDB:Database Locking Mode=1;Jet OLEDB:Global Partial Bulk Ops=2;Jet OLEDB:Global Bulk Transactions=1;Jet OLEDB:New Database Password=\"\";Jet OLEDB:Create System Database=False;Jet OLEDB:Encrypt Database=False;Jet OLEDB:Don\'t Copy Locale on Compact=False;Jet OLEDB:Compact Without Replica Repair=False;Jet OLEDB:SFP=False";

	   ExchangeConnectionString =  CGlobalVariables::ExchangePath ;
//	   ExchangeConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source=";
//	   ExchangeConnectionString+= "\""+CGlobalVariables::ExchangePath+"\"";
//	   ExchangeConnectionString+= ";Mode=Share Deny None;Extended Properties=\"\";Jet OLEDB:System database=\"\";Jet OLEDB:Registry Path=\"\";Jet OLEDB:Database Password=\"\";Jet OLEDB:Engine Type=5;Jet OLEDB:Database Locking Mode=1;Jet OLEDB:Global Partial Bulk Ops=2;Jet OLEDB:Global Bulk Transactions=1;Jet OLEDB:New Database Password=\"\";Jet OLEDB:Create System Database=False;Jet OLEDB:Encrypt Database=False;Jet OLEDB:Don\'t Copy Locale on Compact=False;Jet OLEDB:Compact Without Replica Repair=False;Jet OLEDB:SFP=False";

	   DefectsConnectionString =  CGlobalVariables::DefectsPath ;
//	   DefectsConnectionString = "Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source=";
//	   DefectsConnectionString+= "\""+CGlobalVariables::DefectsPath+"\"";
//	   DefectsConnectionString+= ";Mode=Share Deny None;Extended Properties=\"\";Jet OLEDB:System database=\"\";Jet OLEDB:Registry Path=\"\";Jet OLEDB:Database Password=\"\";Jet OLEDB:Engine Type=5;Jet OLEDB:Database Locking Mode=1;Jet OLEDB:Global Partial Bulk Ops=2;Jet OLEDB:Global Bulk Transactions=1;Jet OLEDB:New Database Password=\"\";Jet OLEDB:Create System Database=False;Jet OLEDB:Encrypt Database=False;Jet OLEDB:Don\'t Copy Locale on Compact=False;Jet OLEDB:Compact Without Replica Repair=False;Jet OLEDB:SFP=False";
	   if(DataModule2->ADOConnectionCreport->Connected)
	   {
			DataModule2->ADOConnectionCreport->Close();
	   }
//	   DataModule2->ADOConnectionExchange->Close();
//	   DataModule2->ADOConnectionGIP->Close();
	   DataModule2->ADOConnectionCreport->ConnectionString=CreportConnectionString;

//	   DataBooth->ADOConnectionExchange->ConnectionString=ExchangeConnectionString;
//
//	   DataBooth->ADOConnectionGIP->ConnectionString=DefectsConnectionString;
//	   FormMain->MemoTest->Lines->Clear();
//	   FormMain->MemoTest->Lines->Add("Creport Path = " + CGlobalVariables::CreportPath);
//	   FormMain->MemoTest->Lines->Add("ExchangePath = " + CGlobalVariables::ExchangePath);
//	   FormMain->MemoTest->Lines->Add("Defects Paht = " + CGlobalVariables::DefectsPath);
  }
  __finally{
	   if (ini!=0) {
		 delete ini;
	   }

  }
}

void StartingUp()
{

   AnsiString ModePathStart;
 //  PopulateAllowedModes();

   Read_Mode();

   if (Local) {
				 ModePathStart = "Local Path";
	   } else if (TestingPath) {
				 ModePathStart = "Testing Path";
	   } else {
				 ModePathStart = "Global";
	   }
   DefectSheetHeader(FormMain->StringGrid1);
   ChoosenSubCoats.reserve(200);//capacity();

   SetPaths(ModePathStart);

}


void ComboSuperVisorsFillUP()         // Old version
{
   try{
	TADOQuery* Query = DataModule2->ADOQueryCreport;
	TADOQuery* QueryGIP_New = DataModule2->ADOQueryCreport;
	AnsiString QueryString, QueryStringGIP,Name;
	int ID;
	QueryString = "Select First_Name +'  ' + Last_Name as Name,ID_Supervisor from Supervisors order by Last_Name ;" ;
	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
	FormMain->ComboBoxSuperVisors->Items->Clear();
	for(int i=0;i<Query->RecordCount;i++)
	{
		Name= Query->FieldByName("Name")->AsString;
		ID  = Query->FieldByName("ID_Supervisor")->AsInteger;
		FormMain->ComboBoxSuperVisors->Items->Add(Name);
		FormMain->Supervisors[Name] = ID;
		Query->Next();
	}
	FormMain->ComboBoxSuperVisors->Text=" ";

	//Populating CompareRecords - Supervisors Creport and GIP
	QueryStringGIP = " SELECT SUPERVISORS.First_Name +'  ' + SUPERVISORS.Last_Name as NameSup, USERS.NAME, USERS.NUM, SUPERVISORS.ID_SUPERVISOR "
						 " FROM SUPERVISORS INNER JOIN (USERS INNER JOIN UsersToSupervisor ON USERS.NUM = UsersToSupervisor.NUM) "
						 " ON SUPERVISORS.ID_SUPERVISOR = UsersToSupervisor.ID_SUPERVISOR										 ";
					   //	 " GROUP BY SUPERVISORS.FIRST_NAME, SUPERVISORS.LAST_NAME, USERS.NAME;									 ";
		QueryGIP_New->SQL->Clear();
		QueryGIP_New->SQL->Add(QueryStringGIP);

		//UnicodeString Temp = QueryGIP_New->Connection->ConnectionString; //   ConnectionString;
		//QueryGIP->Connection = DataModule1->ADOConnectionGIP;
		//QueryGIP_New->Connection->Connected=false;
		QueryGIP_New->Open();

		for(int i=0;i<QueryGIP_New->RecordCount;i++)
		{
			SupervisorsCompare.add(QueryGIP_New->FieldByName("NAME")->AsString,QueryGIP_New->FieldByName("NameSup")->AsString,
								  QueryGIP_New->FieldByName("NUM")->AsInteger, QueryGIP_New->FieldByName("ID_SUPERVISOR")->AsInteger);

			QueryGIP_New->Next();
		}


		QueryStringGIP = " SELECT USERS.NAME, USERS.PRDSHIFT "
						 " FROM USERS  "
						 " WHERE INSV='True' 										 ";
					   //	 " GROUP BY SUPERVISORS.FIRST_NAME, SUPERVISORS.LAST_NAME, USERS.NAME;									 ";
		QueryGIP_New->SQL->Clear();
		QueryGIP_New->SQL->Add(QueryStringGIP);
		QueryGIP_New->Open();

		for(int i=0;i<QueryGIP_New->RecordCount;i++)
		{
			UnicodeString Temp = QueryGIP_New->FieldByName("PRDSHIFT")->AsString   ;
			char a = Temp[1];
			FormMain->CurrentSupervisorsGIP[a] = SupervisorsCompare.GetCreport(QueryGIP_New->FieldByName("Name")->AsString);
			QueryGIP_New->Next();
		}



	}
   __finally{
			//DataModule1->ADOConnectionExchange->Connected=false;
		   //	DataModule1->ADOConnectionCreport->Connected=false;
   }
}

void ComboOperatorFillUP(AnsiString Shift, bool IsSupervisor,TComboBox* Combo, std::map<AnsiString,int>& Operators, bool All) //  dividing operators on SuperVisors and Not Supervisors
{                                                                                                                             //  All default value is false
   bool ConnectedLocal = false;
   try{
		TADOQuery* Query = DataModule2->ADOQueryCreport; //ADOQueryExchange;
//		if (!DataModule1->ADOConnectionGIP->Connected) {
//		   DataModule1->ADOConnectionGIP->Connected=true;
//		   ConnectedLocal = true;
//		}
		AnsiString QueryString,Name, QueryStringGIP;

		int ID;
		if (All) {
		  QueryString = "Select  Name, Num from Users order by Name ;"  ;
		} else
		{
				/* TODO : add universal user map to fill also inactive users */
			if (IsSupervisor) {
			   QueryString = "Select  Name, Num, Active from Users  where InCoat='true' and InInsert='true' and INSV='true' and Active = 'true' and PrdShift = :Shift  order by Name ;"  ;
			}
			else if (Shift!="Deputy")
			{
			   QueryString = "Select  Name, Num, Active from Users  where InCoat='true' and InInsert='true' and InCo='true' and Active = 'true' and PrdShift = :Shift  order by Name ;"  ;
			}
			else
			{
			   QueryString = "Select  Name, Num, Active from Users  where InCoat='true' and InInsert='true' and InCo='true' and Active = 'true'  order by Name ;"  ;
			}
		}

		Query->SQL->Clear();
		Query->SQL->Add(QueryString);
		if(Shift !="Deputy" && !All)
		{
		   Query->Parameters->ParamByName("Shift")->Value=Shift.c_str();
		}
		Query->Open();
		Operators.clear();
		Combo->Clear();
		for(int i=0;i<Query->RecordCount;i++)
		{
			Name= Query->FieldByName("Name")->AsString;
			ID  = Query->FieldByName("Num")->AsInteger;
			Combo->Items->Add(Name);
			Operators[Name] = ID;
			Query->Next();
		}



   }
   __finally{
		if (ConnectedLocal) {
			DataModule2->ADOConnectionCreport->Connected=false;  // Disconnect if connected in function
		}
   }
}

 void CoatingsFillUp(TComboBox* Combo = FormMain->ComboBoxProduct)
 {
  try{
	TADOQuery* Query = DataModule2->ADOQueryTemp;
	DataModule2->ADOConnectionCreport->Connected=true;
	AnsiString QueryString,Name,FullName,Tym,CheckStr;
	TStringList* StringList;
	int ID;
	QueryString = "Select Prodtype_Name,ID_ProdType,FULLNAME,Description  from Prodtype order by Prodtype_Name asc ;"  ;
	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
	if(StringsProducts!=NULL)
		delete StringsProducts;
	if(StringsProductsExtended!=NULL)
		delete StringsProductsExtended;
	StringsProducts = new TStringList();
	StringsProductsExtended = new TStringList();
	Combos.CoatingsCombo.ClearItems();
	for(int i=0;i<Query->RecordCount;i++)
	{
		TCoating* Product=new TCoating();
		StringList= new TStringList();
		Name= Query->FieldByName("Prodtype_Name")->AsString;
		FullName = Query->FieldByName("FullName")->AsString;
		ID  = Query->FieldByName("ID_ProdType")->AsInteger;
		Tym = Name +" - "+FullName;
		Product->SetShortName(Name);
		Product->SetFullName(FullName);
		Product->SetID(ID);
		CheckStr = Query->FieldByName("Description")->AsString;
		StringList->Text = CheckStr ;

		if(StringList->Text.Length()!=0)
			Product->SetRemarks(StringList);
		else
			delete StringList;
		Combos.CoatingsCombo.AddItem(Name, ID);
		Combos.CoatingsCombo.AddObject(ID, *Product);
		delete Product;
		Query->Next();
	}
	Combos.CoatingsCombo.Assign(Combo);
	Combos.CoatingsCombo.SetDefaultName("CGP");
  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
	}
 }


void SubstratesFillUp()
{
  try{
	DataModule2->ADOConnectionCreport->Connected = true;
	TADOQuery* Query = DataModule2->ADOQueryTemp;

	AnsiString QueryString,Name,FullName,Tym,CheckStr;
	TStringList* StringList;
	int ID;
	QueryString = "Select ID, Value  from CP_PARAM_SUBSTRATE_VIEW order by Value asc ;"  ;
	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
	Combos.SubstrateCombo.ClearItems();

	for(int i=0;i<Query->RecordCount;i++)
	{

		Name= Query->FieldByName("Value")->AsString;
		ID  = Query->FieldByName("ID")->AsInteger;
		Combos.SubstrateCombo.AddItem(Name,ID);
		Query->Next();
	}
	Combos.SubstrateCombo.Assign(FormMain->ComboBoxSubstrate);
	Combos.SubstrateCombo.SetDefaultName("GIP");


  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
  }
}

void SubstrateClassLamiFillUp()
{
   try{
	  DataModule2->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule2->ADOQueryTemp;

	  AnsiString QueryString,Name,FullName,Tym,CheckStr;
	  TStringList* StringList;
	  int ID;
	  QueryString = "Select ID, Value  from CP_PARAM_SUBSTRATECLASS_LAMI_VIEW order by Value asc ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.SubstrateClassLamiCombo.ClearItems();

	  for(int i=0;i<Query->RecordCount;i++)
	  {

		  Name= Query->FieldByName("Value")->AsString;
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.SubstrateClassLamiCombo.AddItem(Name,ID);
		  Combos.SubstrateClassCombo.AddItem(Name,ID);
		  Query->Next();
	  }
	  //SubstrateClassCombo.Assign(FormMain->ComboBoxSubstrate);
	  //SubstrateClassCombo.SetDefaultName("CL");
  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
  }
}


void SubstrateClassFloatFillUp()
{
   try{
	  DataModule2->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule2->ADOQueryTemp;

	  AnsiString QueryString,Name,FullName,Tym,CheckStr;
	  TStringList* StringList;
	  int ID;
	  QueryString = "Select ID, Value  from CP_PARAM_SUBSTRATECLASS_FLOAT_VIEW order by Value asc ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.SubstrateClassFloatCombo.ClearItems();

	  for(int i=0;i<Query->RecordCount;i++)
	  {

		  Name= Query->FieldByName("Value")->AsString;
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.SubstrateClassFloatCombo.AddItem(Name,ID);
		  Combos.SubstrateClassCombo.AddItem(Name,ID);
		  Query->Next();
	  }
	  //SubstrateClassFloatCombo.Assign(FormMain->ComboBoxSubstrate);
	  //SubstrateClassFloatCombo.SetDefaultName("TRR");
  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
  }
}


void SizeFillUp()
{
	try{
	  DataModule2->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule2->ADOQueryTemp;

	  AnsiString QueryString,Name,FullName,Tym,CheckStr;
	  TStringList* StringList;
	  int ID;
	  QueryString = "Select ID, Value, Val1, Val2  from CP_PARAM_SIZE_VIEW where Active='True' order by Value asc  ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.SizeCombo.ClearItems();

	  for(int i=0;i<Query->RecordCount;i++)
	  {

		  Name= Query->FieldByName("Value")->AsString;
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.SizeCombo.AddItem(Name,ID);
		  Combos.SizeCombo.AddWidthLength(ID,Query->FieldByName("Val1")->AsInteger,Query->FieldByName("Val2")->AsInteger);
		  Query->Next();
	  }
	  Combos.SizeCombo.Assign(FormMain->ComboBoxSize);
	  Combos.SizeCombo.SetDefaultName("JUMBO");
  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
  }
}

void ThicknessFloatFillUp()
{
   try{
	  DataModule2->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule2->ADOQueryTemp;

	  AnsiString QueryString,Name,FullName,Tym,CheckStr;
	  TStringList* StringList;
	  int ID;
	  QueryString = "Select ID, Value  from CP_PARAM_THICKNESS_FLOAT_VIEW order by Value asc ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.ThicknessFloatCombo.ClearItems();
	  for(int i=0;i<Query->RecordCount;i++)
	  {

		  Name= Query->FieldByName("Value")->AsString;
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.ThicknessFloatCombo.AddItem(Name,ID);
		  Combos.ThicknessCombo.AddItem(Name, ID);
		  Query->Next();
	  }
	  //ThicknessFloatCombo.Assign(FormMain->ComboBoxSubstrate);
	  //ThicknessFloatCombo.SetDefaultName("4");
  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
  }
}

void ThicknessLamiFillUp()
{
   try{
	  DataModule2->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule2->ADOQueryTemp;

	  AnsiString QueryString,Name,FullName,Tym,CheckStr;
	  TStringList* StringList;
	  int ID;
	  QueryString = "Select ID, Value  from CP_PARAM_THICKNESS_LAMI_VIEW order by Value asc ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.ThicknessLamiCombo.ClearItems();

	  for(int i=0;i<Query->RecordCount;i++)
	  {

		  Name= Query->FieldByName("Value")->AsString;
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.ThicknessLamiCombo.AddItem(Name,ID);
		  Combos.ThicknessCombo.AddItem(Name, ID);
		  Query->Next();
	  }
	  //ThicknessLamiCombo.Assign(FormMain->ComboBoxSubstrate);
	  //ThicknessLamiCombo.SetDefaultName("3.3.1");
  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
  }
}

//void AdditonalDefectFillUp()
//{
//   try{
//	  DataModule2->ADOConnectionCreport->Connected = true;
//	  TADOQuery* Query = DataModule2->ADOQueryTemp;
//	  UnicodeString Temp;
//	  AnsiString QueryString,Name,FullName,Tym,CheckStr;
//	  TStringList* StringList;
//	  int ID;
//	  QueryString = "Select ID, Value, Val1 from CP_PARAM_THICKNESS_LAMI_VIEW order by Value asc ;"  ;
//	  Query->SQL->Clear();
//	  Query->SQL->Add(QueryString);
//	  Query->Open();
//	  Combos.AdditionalDefectsCombo.ClearItems();
//	  Combos.AdditionalDefectsLami.ClearItems();
//	  for(int i=0;i<Query->RecordCount;i++)
//	  {
//
//		  Name= Query->FieldByName("Value")->AsString;
//		  ID  = Query->FieldByName("ID")->AsInteger;
//		  Combos.AdditionalDefectsLami.AddItem(Name,ID);
//		  Combos.AdditionalDefectsCombo.AddItem(Name, ID);
//		  Temp = Combos.AdditionalDefectsCombo.GetNameByDBId(ID);
//		  Temp += ", ";
//		  Temp +=ID;
//		  FormMain->MemoComment->Lines->Add(Temp);
//		  Query->Next();
//	  }
//	  QueryString = "Select ID, Value, Val1 from CP_PARAM_THICKNESS_LAMI_VIEW order by Value asc ;"  ;
//	  Query->SQL->Clear();
//	  Query->SQL->Add(QueryString);
//	  Query->Open();
//
//	  Combos.AdditionalDefectsFloat.ClearItems();
//	  for(int i=0;i<Query->RecordCount;i++)
//	  {
//
//		  Name= Query->FieldByName("Value")->AsString;
//		  ID  = Query->FieldByName("ID")->AsInteger;
//		  Combos.AdditionalDefectsFloat.AddItem(Name,ID);
//		  Combos.AdditionalDefectsCombo.AddItem(Name, ID);
//		  Temp = Combos.AdditionalDefectsCombo.GetNameByDBId(ID);
//		  Temp += ", ";
//		  Temp +=ID;
//		  FormMain->MemoComment->Lines->Add(Temp);
//		  Query->Next();
//	  }
//	  //ThicknessLamiCombo.Assign(FormMain->ComboBoxSubstrate);
//	  //ThicknessLamiCombo.SetDefaultName("3.3.1");
//  }
//	__finally{
//		DataModule2->ADOConnectionCreport->Connected=false;
//  }
//}



 void LineToCoC();

void SetLamiCombos()
{
	Combos.SubstrateClassLamiCombo.Assign(FormMain->ComboBoxSubstrateClass);
	Combos.SubstrateClassLamiCombo.SetDefaultName("CL");
	Combos.ThicknessLamiCombo.Assign(FormMain->ComboBoxThickness);
	Combos.ThicknessLamiCombo.SetDefaultName("3.3.1");
}

void SetFloatCombos()
{
	Combos.SubstrateClassFloatCombo.Assign(FormMain->ComboBoxSubstrateClass);
	Combos.SubstrateClassFloatCombo.SetDefaultName("TRR");
	Combos.ThicknessFloatCombo.Assign(FormMain->ComboBoxThickness);
	Combos.ThicknessFloatCombo.SetDefaultName("4");

}

void DefectsFillUp()
{

  try{
	  DataModule2->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule2->ADOQueryTemp;
	  UnicodeString Temp;
	  AnsiString QueryString,Name,FullName,Tym,CheckStr;
	  TStringList* StringList;
	  int ID;
	  QueryString = "Select ID, Value, Val1  from CP_PARAM_ADDEFECT_LAMI_VIEW order by Value asc ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.AdditionalDefectsLami.ClearItems();
	  Combos.AdditionalDefectsCombo.ClearItems();
	  for(int i=0;i<Query->RecordCount;i++)
	  {

		  Name= Query->FieldByName("Value")->AsString;      //Additional description in Polish
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.AdditionalDefectsLami.AddItem(Name,ID);
		  Combos.AdditionalDefectsCombo.AddItem(Name, ID);
//		  Temp = Combos.AdditionalDefectsCombo.GetNameByDBId(ID);
//		  Temp += ", ";
//		  Temp +=ID;
//		  FormMain->MemoComment->Lines->Add(Temp);
		  Combos.AdditionalDefectsLami.AddExtra(ID,Query->FieldByName("Val1")->AsString);  //Additional description in English
		  Query->Next();
	  }

	  QueryString = "Select ID, Value, Val1  from CP_PARAM_ADDEFECT_FLOAT_VIEW order by Value asc ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.AdditionalDefectsFloat.ClearItems();

	  for(int i=0;i<Query->RecordCount;i++)
	  {
		  Name= Query->FieldByName("Value")->AsString;      //Additional description in Polish
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.AdditionalDefectsFloat.AddItem(Name,ID);
		  Combos.AdditionalDefectsFloat.AddExtra(ID,Query->FieldByName("Val1")->AsString);  //Additional description in English
          Combos.AdditionalDefectsCombo.AddItem(Name, ID);
//		  Temp = Combos.AdditionalDefectsCombo.GetNameByDBId(ID);
//		  Temp += ", ";
//		  Temp +=ID;
//		  FormMain->MemoComment->Lines->Add(Temp);
		  Query->Next();
	  }

	  QueryString = "Select ID, Value  from CP_PARAM_DEFECT_VIEW order by Value asc ;"  ;
	  Query->SQL->Clear();
	  Query->SQL->Add(QueryString);
	  Query->Open();
	  Combos.DefectsCombo.ClearItems();
	  int TopLabel = FormMain->LabelDefectKind->Top;
	  int TopEdit  = FormMain->EditNoOf->Top;
	  int TopUpDown = FormMain->UpDownNoOf->Top;
	  int TopButton = FormMain->ButtonAddDefect->Top;
	  TLabel* LabelTemp;
	  TEdit* EditTemp;
	  TUpDown* UpDownTemp;
	  TButton* ButtonTemp;
	  int HeightStep = FormMain->ButtonAddDefect->Height * 1.5;
	  for(int i=0;i<Query->RecordCount;i++)
	  {
		  Name= Query->FieldByName("Value")->AsString;      //Additional description in Polish
		  ID  = Query->FieldByName("ID")->AsInteger;
		  Combos.DefectsCombo.AddItem(Name,ID);
		  //Defects.AddExtra(ID,Query->FieldByName("Val1")->AsString;  //Additional description in English

		  LabelTemp = new TLabel(FormMain);
		  LabelTemp->Font = FormMain->LabelDefectKind->Font;
		  LabelTemp->Parent = FormMain->LabelDefectKind->Parent;
		  LabelTemp->Caption=Name;
		  LabelTemp->Top = TopLabel;
		  LabelTemp->Left = FormMain->LabelDefectKind->Left;
		  LabelTemp->Enabled = true;
		  TopLabel += HeightStep;


		  EditTemp = new TEdit(FormMain);
		  EditTemp->Parent = FormMain->EditNoOf->Parent;
		  EditTemp->Left = FormMain->EditNoOf->Left;
		  EditTemp->Width = FormMain->EditNoOf->Width;
		  EditTemp->Font = FormMain->EditNoOf->Font;
		  EditTemp->Height = 31; //FormMain->EditNoOf->Height;
		  EditTemp->Top=TopEdit;
		  Combos.DefectsCombo.AddEdit(ID, EditTemp);
		  TopEdit+=HeightStep;


		  UpDownTemp = new TUpDown(FormMain);
		  UpDownTemp->Parent = FormMain->UpDownNoOf->Parent;
		  UpDownTemp->Associate = EditTemp;
		  UpDownTemp->Top=TopUpDown;
		  UpDownTemp->Left=FormMain->UpDownNoOf->Left;
		  UpDownTemp->Min = FormMain->UpDownNoOf->Min;
		  UpDownTemp->Max = FormMain->UpDownNoOf->Max;
		  UpDownTemp->Position = FormMain->UpDownNoOf->Position;
		  TopUpDown+=HeightStep;

		  ButtonTemp = new TButton(FormMain);
		  ButtonTemp->Left  = FormMain->ButtonAddDefect->Left;
		  ButtonTemp->Top  = TopButton;
		  TopButton+=HeightStep;
		  ButtonTemp->Images  = FormMain->ButtonAddDefect->Images;
		  ButtonTemp->ImageIndex   = FormMain->ButtonAddDefect->ImageIndex;
		  ButtonTemp->Parent  = FormMain->ButtonAddDefect->Parent;
		  ButtonTemp->Width  = FormMain->ButtonAddDefect->Width;
		  ButtonTemp->Height  = FormMain->ButtonAddDefect->Height;
		  ButtonTemp->Tag  = ID; //FormMain->ButtonAddDefect->
		  ButtonTemp->ImageAlignment = iaCenter;
		  ButtonTemp->OnClick = FormMain->ButtonAddDefectClicked;
          //FormMain->Function(ButtonTemp);

		  Query->Next();
	  }


  }
	__finally{
		DataModule2->ADOConnectionCreport->Connected=false;
  }






}


void FillUp()
{
	ComboSuperVisorsFillUP();
	SubstratesFillUp();
	CoatingsFillUp();
	SizeFillUp();
	Combos.ThicknessCombo.ClearItems();
	Combos.SubstrateClassCombo.ClearItems();
	SubstrateClassFloatFillUp();
	Combos.SubstrateClassFloatCombo.Assign(FormMain->ComboBoxSubstrateClass);
	Combos.SubstrateClassFloatCombo.SetDefaultName("TRR");
	SubstrateClassLamiFillUp();
	ThicknessFloatFillUp();
	Combos.ThicknessFloatCombo.Assign(FormMain->ComboBoxThickness);
	Combos.ThicknessFloatCombo.SetDefaultName("4");
	ThicknessLamiFillUp();
	DefectsFillUp();
   //	LineStatusFillUp();
  //	ProductsFillUp();
  //	CSummaryLineStatus::FillUpCoCData();
  //	CSummaryProduction::FillUpProductsData();
	//LineToCoC();
}



//---------------------------------------------------------------------------



