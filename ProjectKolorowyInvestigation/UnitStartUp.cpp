//---------------------------------------------------------------------------


#pragma hdrstop


#include "UnitStartUp.h"
#include "DataCoater.h"
#include "IniFiles.hpp"
#include "UnitMain.h"
#include "UnitProducts.h"
#include "Global.h"
#include "UnitLineStatus.h"
#include "AccessCOM.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
double TOEE_Struct::MaxInputPerHour;
double TOEE_Struct::Speed;
double TOEE_Struct::GoalOEE;
double TOEE_Struct::NewMaxLineSpeedByProduct[4];
UnicodeString CGlobalVariables::CreportPath ;
UnicodeString CGlobalVariables::ExchangePath = "";
UnicodeString CGlobalVariables::DefectsPath	= "";
UnicodeString CGlobalVariables::CoCPath =  "";  //used for
//Provider=Microsoft.Jet.OLEDB.4.0;User ID=Admin;Data Source="C:\Documents and Settings\jsolecki\My Documents\Nowy Raport Coater'a\Creport.mdb";Mode=Share Deny None;Extended Properties="";Jet OLEDB:System database="";Jet OLEDB:Registry Path="";Jet OLEDB:Database Password="";Jet OLEDB:Engine Type=5;Jet OLEDB:Database Locking Mode=1;Jet OLEDB:Global Partial Bulk Ops=2;Jet OLEDB:Global Bulk Transactions=1;Jet OLEDB:New Database Password="";Jet OLEDB:Create System Database=False;Jet OLEDB:Encrypt Database=False;Jet OLEDB:Don't Copy Locale on Compact=False;Jet OLEDB:Compact Without Replica Repair=False;Jet OLEDB:SFP=False
UnicodeString CGlobalVariables::ShiftParetoFile="";



 bool Local, Report, TestingPath ; // Local=1 means using local testing database, Report=1 means using reporting Facilities.
 bool TestingM = false;
//TLineStatus* LineStatuses;
TStringList* StringsProducts;        //StringLists for ComboBox depending on needs
TStringList* StringsProductsExtended;//StringLists for ComboBox depending on needs
TProdToLineStatus ProductsOnly;     //Object for ComboBox depending on needs
TProdToLineStatus ProductsExtended;  //Object for ComboBox depending on needs
std::map<int,int> LineIDToCoCCell;
std::map<int,UnicodeString> CoCCellToCategory;
std::map<int,TProducts> Products;
std::list<UnicodeString> ProdExtensions;

void Read_Mode()
{
   TIniFile *ini= 0;
   try{
	   TestingM = false;
	   UnicodeString IniFile = ChangeFileExt( Application->ExeName, ".INI" ) ;
	   ini = new TIniFile(IniFile ); //;   ("ReportCT.ini");
	   Local = ini->ReadBool("Mode","Local",false);
	   Report = ini->ReadBool("Mode","Report",false);
	   TestingM = ini->ReadBool("Mode","Testing",false);
	   TestingPath=ini->ReadBool("Mode","TestingPath",false);
	   if (Local) {
				 FormMain->RadioGroupMode->ItemIndex=1;
	   } else if (TestingPath) {
				 FormMain->RadioGroupMode->ItemIndex=2;
	   } else {
				 FormMain->RadioGroupMode->ItemIndex=0;
	   }
	   if(ini->ReadBool("Mode","Mistrz",false))
	   {
		  FormMain->ButtonPrzepiszDoCoc->Visible=true;
	   }
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
	   ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ) );

	   TempPath =  ini->ReadString( ModePath.c_str(), "Creport", "j:\\Coater_Report\\Creport.mdb" );
	   CGlobalVariables::CreportPath  =   TempPath ;
	   TempPath =ini->ReadString( ModePath.c_str(), "Creport_exchange", "j:\\Coater_Report\\Creport_exchange.mdb" );

	   CGlobalVariables::ExchangePath = TempPath;
	   TempPath =  ini->ReadString( ModePath.c_str(), "Defects", "j:\\Data\\GIP\\gip.mdb" );
	   CGlobalVariables::DefectsPath	= TempPath;
	   CGlobalVariables::CoCPath =  ini->ReadString( ModePath.c_str(), "CoC", "v:\\Coater_Process\\coater\\Production Issues\\Logbook\\" );
	   TempPath = ini->ReadString( ModePath.c_str(), "ShiftParetoFile","");
	   CGlobalVariables::ShiftParetoFile = TempPath;

	   if(TestingM)
	   {
			Mode.Set(TestingMode);
			FormMain->ButtonPrintCurrent->Visible=true;
			FormMain->PanelDevelopement->Visible= true;
            FormMain->ButtonImportFromExcel->Visible=true;
	   }
	   else
	   {
		  FormMain->StringGridReport->Height=FormMain->PageControl1->ClientHeight - FormMain->StringGridReport->Top -10;
		  FormMain->ScrollBoxReportPrint->Visible=false;
		  FormMain->Button1->Visible=false;
		  FormMain->ButtonFastReport->Visible=false;
		  FormMain->StringGridReport->Width = FormMain->PageControl1->Width - 40;
		  FormMain->RichEditReport->Visible=false;
		  FormMain->ButtonImportFromExcel->Visible=false;
	   }
	   if (Report) {
		   FormMain->PageControl1->Enabled=true;
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

	   DataModule1->ADOConnectionCreport->Close();
	   DataModule1->ADOConnectionExchange->Close();
	   DataModule1->ADOConnectionGIP->Close();
	   DataModule1->ADOConnectionCreport->ConnectionString=CreportConnectionString;
		DataModule1->ADOConnectionReport2->ConnectionString=CreportConnectionString;
	   DataModule1->ADOConnectionExchange->ConnectionString=ExchangeConnectionString;

	   DataModule1->ADOConnectionGIP->ConnectionString=DefectsConnectionString;
	   FormMain->MemoTest->Lines->Clear();
	   FormMain->MemoTest->Lines->Add("Creport Path = " + CGlobalVariables::CreportPath);
	   FormMain->MemoTest->Lines->Add("ExchangePath = " + CGlobalVariables::ExchangePath);
	   FormMain->MemoTest->Lines->Add("Defects Paht = " + CGlobalVariables::DefectsPath);
	   FormMain->MemoTest->Lines->Add("ShiftPartoFile =" +CGlobalVariables::ShiftParetoFile);

  }
  __finally{
	   if (ini!=0) {
		 delete ini;
	   }

  }
}

double ConvertStringToDoubleFromIniFile(UnicodeString Value,double DefaultValue)
{
   double Number;
   TFormatSettings LocaleStruct;
  // LocaleStruct.Create(GetThreadLocale());
   GetLocaleFormatSettings(LOCALE_SYSTEM_DEFAULT, LocaleStruct);
   int LenthValue = Value.Length();
   for (int i =1;i < Value.Length(); i++) {
			 if(!IsDigit(Value[i]) && !IsSeparator(Value[i])&& Value[i]!=LocaleStruct.DecimalSeparator )
			 {
				Value[i]=LocaleStruct.DecimalSeparator;
				break;
			 }
	   }
	   try {
		Number = Value.ToDouble(); //= ini->ReadFloat( "OEEAssumption","MaxInputPerHour" , 2048 );
	   }
	   catch(EConvertError& E)
	   {
		 ShowMessage("Nastąpił błąd w trakcie interpretacji wartości "+ Value +" z pliku *.ini, prawdopodobnie spowodowany złym serparatorem dziesiętnym \n\n"
					 "There was an problem while reading value of "+Value+" from ini file, probably not valid decimal separator were used");

		 Number = DefaultValue;
	   }
	   return Number;
}

void ReadOEEAssumption()
{
   TIniFile *ini= 0;
   UnicodeString MaxInputString ="2048";
   UnicodeString MaxSpeedStringCGA ="11";
   UnicodeString MaxSpeedStringCGHT="8";
   UnicodeString MaxSpeedStringSGA="6";
   UnicodeString MaxSpeedStringSGHT="6";

   try{

	   ini = new TIniFile(ChangeFileExt( Application->ExeName, ".INI" ) );
	   MaxInputString = ini->ReadString( "OEEAssumption","MaxInputPerHour" , MaxInputString );
	   MaxSpeedStringCGA = ini->ReadString( "OEEAssumption","MaxSpeedCGA",MaxSpeedStringCGA);
	   MaxSpeedStringCGHT= ini->ReadString( "OEEAssumption","MaxSpeedCGHT",MaxSpeedStringCGHT);
	   MaxSpeedStringSGA = ini->ReadString( "OEEAssumption","MaxSpeedSGA",MaxSpeedStringSGA);
	   MaxSpeedStringSGHT = ini->ReadString( "OEEAssumption","MaxSpeedSGHT",MaxSpeedStringSGHT);
	   double MaxInput = ConvertStringToDoubleFromIniFile(MaxInputString,2048);
	   TOEE_Struct::MaxInputPerHour=MaxInput;
	   TOEE_Struct::NewMaxLineSpeedByProduct[0]= ConvertStringToDoubleFromIniFile(MaxSpeedStringCGA, 11);
	   TOEE_Struct::NewMaxLineSpeedByProduct[1]= ConvertStringToDoubleFromIniFile(MaxSpeedStringCGHT,8);
	   TOEE_Struct::NewMaxLineSpeedByProduct[2]= ConvertStringToDoubleFromIniFile(MaxSpeedStringSGA,6);
	   TOEE_Struct::NewMaxLineSpeedByProduct[3]= ConvertStringToDoubleFromIniFile(MaxSpeedStringSGHT,6);
		//double Speed = ini->ReadFloat( "OEEAssumption","MaxCGSpeed" , 10.4 );
	   double Speed = ConvertStringToDoubleFromIniFile(ini->ReadString( "OEEAssumption","MaxCGSpeed" , "10.4" ),10.4);
	   TOEE_Struct::Speed=Speed;
	   TOEE_Struct::GoalOEE=ConvertStringToDoubleFromIniFile(ini->ReadString( "OEEAssumption","TargetOEE" , "59.5" ),59.5);
	   MaxInput = TOEE_Struct::GoalOEE;
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
   PopulateAllowedModes();

   Read_Mode();

   if (Local) {
				 ModePathStart = "Local Path";
	   } else if (TestingPath) {
				 ModePathStart = "Testing Path";
	   } else {
				 ModePathStart = "Global";
	   }



//   Starting_CreportPath = CreportPath   =  ini->ReadString( "Path", "Creport", "j:\\Coater_Report\\Creport.mdb" );
//   Starting_ExchangePath = ExchangePath =  ini->ReadString( "Path", "Creport_exchange", "j:\\Coater_Report\\Creport_exchange.mdb" );
//   Starting_DefectsPath = DefectsPath	=  ini->ReadString( "Path", "Defects", "j:\\Data\\GIP\\gip.mdb" );
//   Local_CreportPath =  ini->ReadString( "Local Path", "Creport", "j:\\Coater_Report\\Creport.mdb" );
//   Local_ExchangePath = ini->ReadString( "Local Path", "Creport_exchange", "j:\\Coater_Report\\Creport_exchange.mdb" );
//   Local_DefectsPath  =  ini->ReadString( "Local Path", "Defects", "j:\\Data\\GIP\\gip.mdb" );
//


//   if(Local)
//   {
//	   CreportPath=Local_CreportPath;
//	   ExchangePath=Local_ExchangePath;
//	   DefectsPath=Local_DefectsPath;
//   }
   if (Report) {
	   FormMain->PageControl1->Enabled=true;
   }

   SetPaths(ModePathStart);
   ReadOEEAssumption();
  // DataModule1->ADOConnectionExchange->Connected=true;
}


void ComboSuperVisorsFillUP()         // Old version
{
   try{
	TADOQuery* Query = DataModule1->ADOQueryExchange;
	TADOQuery* QueryGIP_New = DataModule1->ADOQueryGIP;
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
		TADOQuery* Query = DataModule1->ADOQueryGIP; //ADOQueryExchange;
		if (!DataModule1->ADOConnectionGIP->Connected) {
		   DataModule1->ADOConnectionGIP->Connected=true;
		   ConnectedLocal = true;
		}
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
			DataModule1->ADOConnectionGIP->Connected=false;  // Disconnect if connected in function
		}
   }
}

 void ProductsFillUp()
 {
  try{
	TADOQuery* Query = DataModule1->ADOQueryCreport;

	AnsiString QueryString,Name,FullName,Tym,CheckStr;
	UnicodeString Category, Part;
	TStringList* StringList;
	int ID;
	QueryString = "Select *  from PartToProdTypeViewCategorized order by Prodtype_Name asc ;"  ;

    Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();

	std::map<int,TProducts>::iterator ProductsIt;
	for(int i=0;i<Query->RecordCount;i++)
	{

		StringList= new TStringList();
		Name= Query->FieldByName("Prodtype_Name")->AsString;
		FullName = Query->FieldByName("FullName")->AsString;
		ID  = Query->FieldByName("ID_ProdType")->AsInteger;
		CheckStr = Query->FieldByName("Description")->AsString;
		Part =  Query->FieldByName("ProdPart")->AsString;
		Category =   Query->FieldByName("Category")->AsString;
		StringList->Text = CheckStr ;
		if (Products[ID].GetID()!=ID)
		{
          	if(StringList->Text.Length()!=0)
			  Products[ID].SetRemarks(StringList);
			Products[ID].SetID(ID);
			Products[ID].SetShortName(Name);
			Products[ID].SetFullName(FullName);
			Products[ID].SetPart(Part);
		}
		Products[ID].AddProdKind(Category);
		delete StringList;
		Query->Next();
	}

	ProductsOnly.Reset();
	ProductsExtended.Reset();
	if(StringsProducts!=NULL)
		delete StringsProducts;
	if(StringsProductsExtended!=NULL)
		delete StringsProductsExtended;
	StringsProducts = new TStringList();
	StringsProductsExtended = new TStringList();

	for(ProductsIt=Products.begin();ProductsIt !=Products.end();ProductsIt++)
	{

		std::unique_ptr<TProducts> Product(new TProducts());
		StringList= new TStringList();
		Name= ProductsIt->second.GetShortName();
		FullName = ProductsIt->second.GetFullName();
		ID  = ProductsIt->second.GetID();
		Tym = Name +" - "+FullName;
		Product->SetShortName(Name);
		Product->SetFullName(FullName);
		Product->SetID(ID);
		StringList = ProductsIt->second.GetRemarks();
		if(StringList!=0)
			Product->SetRemarks(StringList);
		delete StringList;
		FormMain->ComboBoxProducts->Items->Add(Tym);
		FormMain->ProductNames[Tym] = ID;
		ComboIndexToProduct[FormMain->ComboBoxProducts->Items->IndexOf(Tym)]=*Product;
		ProductsOnly.Add(ID,Name,FullName,CheckStr);
		ProductsExtended.Add(ID,Name,FullName,CheckStr);

	}

//	#ifndef VersionExtended1
//	//QueryString = "Select Prodtype_Name,ID_ProdType,FULLNAME,Description  from Prodtype order by Prodtype_Name asc ;"  ;
//	QueryString = "Select *  from PartToProdTypeView order by Prodtype_Name asc ;"  ;
//	#else
////	if(Local)
////		QueryString = "Select Prodtype_Name,ID_ProdType,FULLNAME,Description  from Prodtype where IsProduct=true order by Prodtype_Name asc ;";
////	else
//		//QueryString = "Select Prodtype_Name,ID_ProdType,FULLNAME,Description  from Prodtype order by Prodtype_Name asc ;"  ;
//		QueryString = "Select *  from PartToProdTypeView order by Prodtype_Name asc ;"  ;
//	#endif
//	Query->SQL->Clear();
//	Query->SQL->Add(QueryString);
//	Query->Open();


//	for(int i=0;i<Query->RecordCount;i++)
//	{
//		TProducts* Product=new TProducts();
//		StringList= new TStringList();
//		Name= Query->FieldByName("Prodtype_Name")->AsString;
//		FullName = Query->FieldByName("FullName")->AsString;
//		ID  = Query->FieldByName("ID_ProdType")->AsInteger;
//		Tym = Name +" - "+FullName;
//		Product->SetShortName(Name);
//		Product->SetFullName(FullName);
//		Product->SetID(ID);
//		CheckStr = Query->FieldByName("Description")->AsString;
//		StringList->Text = CheckStr ;
//
//		if(StringList->Text.Length()!=0)
//			Product->SetRemarks(StringList);
//		else
//			delete StringList;
//		FormMain->ComboBoxProducts->Items->Add(Tym);
//	   //	FormMain->ComboBoxProdFrom->Items->Add(Name);
//	   //	FormMain->ComboBoxProdTo->Items->Add(Name);
//		FormMain->ProductNames[Tym] = ID;
//		ComboIndexToProduct[FormMain->ComboBoxProducts->Items->IndexOf(Tym)]=*Product;
//		ProductsOnly.Add(ID,Name,FullName,CheckStr);
//		ProductsExtended.Add(ID,Name,FullName,CheckStr);
//		Query->Next();
//	}
	 QueryString = "SELECT * FROM  NON_PRODTYPE ORDER BY ID_PRODTYPE DESC";
	 Query->Close();
	 Query->SQL->Clear();
	 Query->SQL->Add(QueryString);
	 Query->Open();
	 while(!Query->Eof)
	 {
		ProductsExtended.Add(Query->FieldByName("ID_PRODTYPE")->AsInteger,
					   Query->FieldByName("ProdType_Name")->AsString,
					   Query->FieldByName("FullName")->AsString,
					   Query->FieldByName("Description")->AsString);
		Query->Next();
	 }






	ProductsOnly.CounterReset();
	ProductsExtended.CounterReset();
	TProdType TymProd = ProductsOnly.NextType();
	TProdType TymExt = ProductsExtended.NextType();
	int i=0;

	while(TymProd.ID!=0)
	{
		ProductsOnly.CombineComboWithID(i,TymProd.ID) ;
		StringsProducts->Insert(i,TymProd.ShortName);
		i++;
		TymProd=ProductsOnly.NextType();
	}
    i=0;
	while(TymExt.ID!=0)
	{
		ProductsExtended.CombineComboWithID(i,TymExt.ID) ;
		StringsProductsExtended->Insert(i,TymExt.ShortName);
		i++;
		TymExt=ProductsExtended.NextType();
	}
	FormMain->ComboBoxProdFrom->Text="";
	FormMain->ComboBoxProdFrom->Items = StringsProducts;
	FormMain->ComboBoxProdTo->Text="";
	FormMain->ComboBoxProdTo->Items = StringsProducts;

	QueryString = "Select Extension  from ProdPartExtensions"  ;

	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
	UnicodeString Extent;
	for(int i=0;i<Query->RecordCount;i++)
	{
		Extent = Query->FieldByName("Extension")->AsString;
		Extent = Extent.Trim();
		ProdExtensions.push_back(Extent);
		Query->Next();
	}




	//FormMain->TabSheet3->Brush->Color=clBlue;
   FormMain->PageControl1->OwnerDraw=true;
  //  FormMain->TabSheet3->Repaint();
	//FormMain->TabSheet3->Dr
	//FormMain->TabSheet3->PageControl->Canvas->FillRect(TRect(0,0,FormMain->TabSheet3->ClientWidth,FormMain->TabSheet3->ClientHeight));
  }
	__finally{
//       // DataModule1->ADOConnectionCreport->Connected=false;
	}
 }


 void LineStatusFillUp()
 {
   try{
	TADOQuery* Query = DataModule1->ADOQueryCreport;
	DataModule1->ADOConnectionCreport->Connected=true;
	TComboBox* Combo = FormMain->ComboBoxLineStatus;
	AnsiString QueryString,Name,EnglishName,PlDescription,EnDescription;
	int ID, Category, MainCategory;
	bool IsActive, StatusIsScheduled;
	QueryString = "Select * from LineStatus_Type left join CategoriesLS on LineStatus_Type.CategoryID=CategoriesLS.CategoryID "
				  "where Active='true';" ;
	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
	LineStatuses = new TLineStatus(Query->RecordCount);
	for(int i=0;i<Query->RecordCount;i++)
	{
		Name= Query->FieldByName("LineStatus_Name")->AsString;
		EnglishName = Query->FieldByName("LineStatus_Name_English")->AsString;
		PlDescription = Query->FieldByName("PolishDescription")->AsString;
		ID  = Query->FieldByName("ID_LineStatus")->AsInteger;
		EnDescription = Query->FieldByName("EnglishDescription")->AsString;
		Category =  Query->FieldByName("CategoryID")->AsInteger;
		MainCategory = Query->FieldByName("MainID")->AsInteger;
		IsActive =  Query->FieldByName("Active")->AsBoolean;
		if (IsActive) {
		  Combo->Items->Add(Name) ;
		  ComboLineStatusToID[Combo->Items->IndexOf(Name)]=ID;
		}
		if (MainCategory!=6) {    // 6 is unsceduled
			StatusIsScheduled = true;
		}
		else
		{
		   StatusIsScheduled = false;
		}
		//UnicodeString aPolishName, UnicodeString aEnglishName, UnicodeString aPolishDescription, UnicodeString aEnglishDescription, int aCategoryID, bool aActive)
		LineStatuses->Add(ID, Name, EnglishName, PlDescription,EnDescription,Category,IsActive, StatusIsScheduled);

		Query->Next();
	}
	Combo->Text="";
	}
	__finally{
			//DataModule1->ADOConnectionExchange->Connected=false;
			DataModule1->ADOConnectionCreport->Connected=false;
	}
 }

void LineToCoC();

void FillUp()
{
	ComboSuperVisorsFillUP();
	LineStatusFillUp();
	ProductsFillUp();
	CSummaryLineStatus::FillUpCoCData();
	CSummaryProduction::FillUpProductsData();

	SubstratesFillUp();
	CoatingsFillUp();
	SizeFillUp();
	Combos.ThicknessCombo.ClearItems();
	Combos.SubstrateClassCombo.ClearItems();
	SubstrateClassFloatFillUp();
	SubstrateClassLamiFillUp();
	ThicknessFloatFillUp();
	ThicknessLamiFillUp();




	//LineToCoC();
}


 
//---------------------------------------------------------------------------


void TProdToLineStatus::Add(int aID, UnicodeString aShortName, UnicodeString aFullName, UnicodeString aDescription)
{
	TProdType Rekord;
	Rekord.ID=aID;
	Rekord.ShortName=aShortName;
	Rekord.FullName=aFullName;
	Rekord.Description=aDescription;
	if(Rekordy.size()>=Rekordy.max_size())
	{
		Rekordy.resize(Rekordy.size()+100);
	}
	Rekordy.push_back(Rekord);
}

void TProdToLineStatus::CombineComboWithID(int aIndex,int aID)
{
	if(ComboToID.size()>ComboToID.max_size())
	{
		Application->MessageBoxA(L"Błąd w funkcji CombineComboWithID - przekroczony maksymalny rozmiar mapy",L"Błąd",MB_OK)   ;
	}
	this->ComboToID[aIndex]=aID;
}

TProdType TProdToLineStatus::Find(int aIndex) //if not found return zero ProdType
{
	int aID = ComboToID[aIndex];
	list<TProdType>::iterator it;
	for(it=Rekordy.begin();	it!=Rekordy.end();it++)
	{
		if(it->ID==aID)
		return *it;
	}
	return TProdType();

}

void TProdToLineStatus::CounterReset()
{
	Counter = Rekordy.begin();
}

TProdType TProdToLineStatus::NextType()
{
	if(Counter == Rekordy.end() ) //No next element
	{
		return TProdType();
	}
	TProdType RetType = *Counter;
	Counter++;
	return RetType;
}

void TProdToLineStatus::ResetComboToID()
{
	ComboToID.clear();
}
void TProdToLineStatus::Reset()
{
	ResetComboToID();
	CounterReset();
	Rekordy.clear();
}

TProdType TProdToLineStatus::FindByID(int aID)
{
	CounterReset();
	TProdType Tym = NextType();
	while(Tym.ID!=0 && Tym.ID!=aID)
	{
	   Tym = NextType();
	}
	return Tym;
}

int TProdToLineStatus::FindComboIndex(int aID)
{
	map<int,int>::iterator it= ComboToID.begin();
	for(it;it!=ComboToID.end();it++)
	{
	   if(it->second == aID)
		return  it->first;
	}
	return -1; // in case of failure
}

void LineToCoC()
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
	QueryString = "Select * from LogToCoC  ;" ;
	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
	LineStatuses = new TLineStatus(Query->RecordCount);
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



 void CoatingsFillUp()
 {
  try{
	TADOQuery* Query = DataModule1->ADOQueryTemp;
	DataModule1->ADOConnectionCreport->Connected=true;
	AnsiString QueryString,Name,FullName,Tym,CheckStr;
	TStringList* StringList;
	int ID;
	QueryString = "Select Prodtype_Name,ID_ProdType,FULLNAME,Description  from Prodtype order by Prodtype_Name asc ;"  ;
	Query->SQL->Clear();
	Query->SQL->Add(QueryString);
	Query->Open();
//	if(StringsProducts!=NULL)
//		delete StringsProducts;
//	if(StringsProductsExtended!=NULL)
//		delete StringsProductsExtended;
//	StringsProducts = new TStringList();
//	StringsProductsExtended = new TStringList();
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

  }
	__finally{
		DataModule1->ADOConnectionCreport->Connected=false;
	}
 }


void SubstratesFillUp()
{
  try{
	DataModule1->ADOConnectionCreport->Connected = true;
	TADOQuery* Query = DataModule1->ADOQueryTemp;

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
	

  }
	__finally{
		DataModule1->ADOConnectionCreport->Connected=false;
  }
}

void SubstrateClassLamiFillUp()
{
   try{
	  DataModule1->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule1->ADOQueryTemp;

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
		DataModule1->ADOConnectionCreport->Connected=false;
  }
}


void SubstrateClassFloatFillUp()
{
   try{
	  DataModule1->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule1->ADOQueryTemp;

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
		DataModule1->ADOConnectionCreport->Connected=false;
  }
}


void SizeFillUp()
{
	try{
	  DataModule1->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule1->ADOQueryTemp;

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

  }
	__finally{
		DataModule1->ADOConnectionCreport->Connected=false;
  }
}

void ThicknessFloatFillUp()
{
   try{
	  DataModule1->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule1->ADOQueryTemp;

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
		DataModule1->ADOConnectionCreport->Connected=false;
  }
}

void ThicknessLamiFillUp()
{
   try{
	  DataModule1->ADOConnectionCreport->Connected = true;
	  TADOQuery* Query = DataModule1->ADOQueryTemp;

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
		DataModule1->ADOConnectionCreport->Connected=false;
  }
}

//void AdditonalDefectFillUp()
//{
//   try{
//	  DataModule1->ADOConnectionCreport->Connected = true;
//	  TADOQuery* Query = DataModule1->ADOQueryTemp;
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
//		DataModule1->ADOConnectionCreport->Connected=false;
//  }
//}



 void LineToCoC();


 
