//---------------------------------------------------------------------------

#include <vcl.h>
#include <list>
#pragma hdrstop

#include "UnitPacks.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

std::map<int,int> RowToDBID;
std::map<int, map<int,int> > CoatIDToRowToDBID;
std::map<int, TStringRow> DBIdToRowData;

TFormPacks *FormPacks;
//---------------------------------------------------------------------------
__fastcall TFormPacks::TFormPacks(TComponent* Owner)
	: TForm(Owner)
{
	 StartingCoatingID=3;
	 Speed=0;
	 ProdAreaIn=0;
	 ProdAreaOut=0;
}
//---------------------------------------------------------------------------

void SetLamiCombos()
{
	Combos.SubstrateClassLamiCombo.Assign(FormPacks->ComboBoxSubstrateClass);
	Combos.SubstrateClassLamiCombo.SetDefaultName("CL");
	Combos.ThicknessLamiCombo.Assign(FormPacks->ComboBoxThickness);
	Combos.ThicknessLamiCombo.SetDefaultName("3.3.1");

}

void SetFloatCombos()
{
	Combos.SubstrateClassFloatCombo.Assign(FormPacks->ComboBoxSubstrateClass);
	Combos.SubstrateClassFloatCombo.SetDefaultName("TRR");
	Combos.ThicknessFloatCombo.Assign(FormPacks->ComboBoxThickness);
	Combos.ThicknessFloatCombo.SetDefaultName("4");

}

double AreaOfPack()
{
 try{
   int NoLites = FormPacks->EditLites->Text.ToInt();
   int NoPacks = FormPacks->EditPacks->Text.ToInt();
   double Area = Combos.SizeCombo.GetArea(Combos.SizeCombo.GetDBId(FormPacks->ComboBoxSize->ItemIndex));
   Area = NoLites*NoPacks*Area;
   return Area;
   }
   catch(EConvertError& E)
   {
	 if(FormPacks->EditLites->Text.IsEmpty() ||   FormPacks->EditPacks->Text.IsEmpty())
	 {
	   return 0;
	 }
	 else throw(E);
   }
}

double AreaOfPack(const TStringRow& R)
{
   int NoLites = R.NoLites;
   int NoPacks = R.NoPacks;
   double Area = Combos.SizeCombo.GetArea(R.IdSize);
   Area = NoLites*NoPacks*Area;
   return Area;
}


void __fastcall TFormPacks::FormShow(TObject *Sender)
{
  Combos.SubstrateClassFloatCombo.Assign(FormPacks->ComboBoxSubstrateClass);
  Combos.SubstrateClassFloatCombo.SetDefaultName("TRR");
  Combos.ThicknessFloatCombo.Assign(FormPacks->ComboBoxThickness);
  Combos.ThicknessFloatCombo.SetDefaultName("4");
  Combos.CoatingsCombo.Assign(ComboBoxProduct);
  ComboBoxProduct->ItemIndex = Combos.CoatingsCombo.GetItemIndexFromIDDD(StartingCoatingID);
  //TComboBox* Combo = FormPacks->ComboBoxProduct
  Combos.SubstrateCombo.Assign(FormPacks->ComboBoxSubstrate);
  Combos.SubstrateCombo.SetDefaultName("GIP");
  Combos.SizeCombo.Assign(FormPacks->ComboBoxSize);
  Combos.SizeCombo.SetDefaultName("JUMBO");
  EditPacksChange(Sender);
  this->StringGrid1->ColWidths[0]=EditLites->Left -  EditPacks->Left; //(UpDownPacks->Left+UpDownPacks->Width);
  StringGrid1->ColWidths[1]= ComboBoxProduct->Left - EditLites->Left;
  StringGrid1->ColWidths[2]= ComboBoxSubstrate->Left - ComboBoxProduct->Left ;
  StringGrid1->ColWidths[3]= ComboBoxSubstrateClass->Left - ComboBoxSubstrate->Left;
  StringGrid1->ColWidths[4]= ComboBoxSize->Left - ComboBoxSubstrateClass->Left ;
  StringGrid1->ColWidths[5]= ComboBoxThickness->Left - ComboBoxSize->Left;
  StringGrid1->ColWidths[6]= ComboBoxThickness->Width;
  StringGrid1->ColWidths[7]= StringGrid1->ClientWidth - StringGrid1->ColWidths[0] - StringGrid1->ColWidths[1] - StringGrid1->ColWidths[2] - StringGrid1->ColWidths[3] - StringGrid1->ColWidths[4] - StringGrid1->ColWidths[5] - StringGrid1->ColWidths[6] ;
  StringGrid1->Cells[0][0] = "No. Packs";
  StringGrid1->Cells[1][0] = "No. Lites";
  StringGrid1->Cells[2][0] = "Coating";
  StringGrid1->Cells[3][0] = "Substrate";
  StringGrid1->Cells[4][0] = "Type";
  StringGrid1->Cells[5][0] = "Size";
  StringGrid1->Cells[6][0] = "Thickness";
  StringGrid1->Cells[7][0] = "Summary Area";
  FillStringGrid();
  StartSpeed = 	FormMain->EditSpeed->Text.IsEmpty()?0: FormMain->EditSpeed->Text.ToDouble(); //Query->FieldByName("Speed")->AsFloat;
  StartAreaIn = FormMain->EditIn->Text.IsEmpty()?0:FormMain->EditIn->Text.ToDouble(); //Query->FieldByName("Total_In")->AsFloat;
  StartAreaOut = FormMain->EditOut->Text.IsEmpty()?0:FormMain->EditOut->Text.ToDouble();//Query->FieldByName("Total_Out")->AsFloat;

  EditStartIn->Text=StartAreaIn;
  EditStartOut->Text=StartAreaOut;
  EditStartSpeed->Text=StartSpeed;
  CalculateDefects();


//  LabelStartIn->Caption=StartAreaIn;
//  LabelStartOut->Caption=StartAreaOut;
//  LabelStartSpeed->Caption=StartSpeed;

}
//---------------------------------------------------------------------------


void __fastcall TFormPacks::ComboBoxSubstrateChange(TObject *Sender)
{
   if(ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
   {
	   SetLamiCombos();
   }
   else
   {
	  SetFloatCombos();
   }

}
//---------------------------------------------------------------------------

TDateTime StringToDateTime(UnicodeString DateStr, int TypeOfDate);

void ImportFromExcel()
// function imports data from Excel spreadsheet obtainded from r1_production_detail1 report
// on production
{
  if( FormMain->OpenDialogImportFromExcel->Execute())
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

		 ExcelApp.OlePropertySet("visible",false);
		 //ExcelDocuments =

		 ExcelDocuments = ExcelDocuments.OleFunction(L"Open",(OleVariant)FormMain->OpenDialogImportFromExcel->FileName);
		 ExcelSheet = ExcelDocuments.OlePropertyGet("ActiveSheet");
		 UnicodeString a;
		 a = ExcelSheet.OlePropertyGet("Name");
//		 ShowMessage(a);

		 int StartRow=-1;
		 int LitesCol=-1;
		 int PartCol =-1;
		 int DimmensionCol=-1;
		 int EventCol=-1;
		 int EventDateCol=-1;
		 int TransactionDateCol=-1;
		 int ShiftCol =-1;
		 int AreaCol = -1;

		 ExcelApp.OlePropertySet("visible",true);
		 for(int i=0;i<20;i++) //search of square area for tags characteristics for reports generated by web application
		  for(int j=0;j<20;j++)
		  {
			 Temp=ExcelSheet.OlePropertyGet(L"Cells",j+1 ,i+1);
			 a = Temp.OlePropertyGet(L"Value");
			 if (a.Pos("TagNo")!=0)
			 {
				StartRow = j+1;
				break;
			 }
		  }
		 if (StartRow==-1)
		 {
			ShowMessage("Nierozpoznano pliku! Proszê o wybranie pliku z raportem produkcji! \n\nUnrecognizable file! Choose the right production report file, please!");
			return;
		 }
		 // Setting proper columns for given values
		 for(int i=1;i<20;i++)
		 {
			 Temp=ExcelSheet.OlePropertyGet(L"Cells",StartRow ,i);
			 a = Temp.OlePropertyGet(L"Value");
			 if (a.Pos("Lites")!=0)
			 {
				LitesCol = i;
			 }
			 if (a.Pos("Part")!=0)
			 {
				PartCol = i;
			 }
			 if (a.Pos("Dimmension")!=0)
			 {
				DimmensionCol = i;
			 }
			 if (a.Pos("Type")!=0)
			 {
				EventCol = i;
			 }
			 if (a.Pos("EventDate")!=0)
			 {
				EventDateCol = i;
			 }
			 if (a.Pos("Transaction")!=0)
			 {
				TransactionDateCol = i;
			 }
			 if (a.Pos("Shift")!=0)
			 {
				ShiftCol = i;
			 }
			 if (a.Pos("Area")!=0)
			 {
				AreaCol = i;
			 }
		 }
		 StructReportFile TempRecord;

		 UnicodeString Event, Lites;
		 int i= StartRow+1;
		 Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,LitesCol);
		 a = Temp.OlePropertyGet(L"Value");
		 Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,EventCol);
		 Event = Temp.OlePropertyGet(L"Value");
		 Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,LitesCol);
		 Lites =  Temp.OlePropertyGet(L"Value");
		 std::list<StructReportFile> TempList;
		 std::map<UnicodeString, std::list<StructReportFile> > Mapa;
		 std::map<UnicodeString, std::map<TPack,int> > Packs;
		 TPack TempPack;
		 double SumAreaStart = 0;
		 while (!Lites.IsEmpty() )  //Lites column is always occupied unless there are end of data
		 {
		   if (Event.Pos("CREATE")!=0 || Event.Pos("QUANTITY")!=0) {
			  Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,TransactionDateCol);
			  a = Temp.OlePropertyGet(L"Value");
			  TempRecord.TransactionDate = StringToDateTime(a, 1);

			  Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,EventDateCol);
			  a = Temp.OlePropertyGet(L"Value");
			  TempRecord.EventDate = StringToDateTime(a, 1);

			  Temp=ExcelSheet.OlePropertyGet(L"Cells", i, LitesCol);
			  a = Temp.OlePropertyGet(L"Value");
			  TempRecord.Lites =  a.ToInt();

			  Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,DimmensionCol);
			  a = Temp.OlePropertyGet(L"Value");
			  TempRecord.Dimmension = a;

			  Temp=ExcelSheet.OlePropertyGet(L"Cells", i , PartCol);
			  a = Temp.OlePropertyGet(L"Value");
			  TempRecord.Part = a;

			  Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,ShiftCol);
			  a = Temp.OlePropertyGet(L"Value");
			  TempRecord.Shift = a;

			  Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,AreaCol);
			  a = Temp.OlePropertyGet(L"Value");
			  TempRecord.AvArea = a.ToDouble();

			  SumAreaStart+= TempRecord.AvArea;
			  TempPack.Add(TempRecord);
			  Packs[TempRecord.Part][TempPack]++;
			  Mapa[TempRecord.Part].push_back(TempRecord) ;


		   }

		   i++;
		   Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,LitesCol);
		   Lites =  Temp.OlePropertyGet(L"Value");
           Temp=ExcelSheet.OlePropertyGet(L"Cells",i ,EventCol);
		   Event = Temp.OlePropertyGet(L"Value");
		 }

		 std::map<UnicodeString, std::map<TPack,int> >::iterator PacksIter;
		 std::map<TPack,int> NoPacks;
		 std::map<TPack,int>::iterator NoPacksIter;
		 double SumArea=0, Area;
		 int NumberOfPacks, NumberOfLites;
		 for(PacksIter = Packs.begin();PacksIter!=Packs.end();PacksIter++)
		 {
			FormMain->MemoProductRemarks->Lines->Add(PacksIter->first);
			NoPacks = PacksIter->second;
			for(NoPacksIter=NoPacks.begin();NoPacksIter!=NoPacks.end();NoPacksIter++)
			{
			  Lites = "  ** ";
			  NumberOfPacks = NoPacksIter->second;
			  Lites += NumberOfPacks;
			  Lites += " x ";
			  NumberOfLites = NoPacksIter->first.GetNoLites();
			  Lites += NumberOfLites;
			  Lites += " Lites of dimmension  ";
			  Lites += NoPacksIter->first.GetWidth();
			  Lites += " x ";
			  Lites += NoPacksIter->first.GetLength();
			  Area = NumberOfPacks*NoPacksIter->first.GetAvArea();
			  SumArea+= Area;
			  Lites += " Area = ";
			  Lites += Area; //UnicodeString().FormatFloat("#.00",Area)
			  FormMain->MemoProductRemarks->Lines->Add(Lites);
			}
		 }
		 FormMain->MemoProductRemarks->Lines->Add("");
		 Lites = "Total Area = ";
		 Lites += SumArea;
		 FormMain->MemoProductRemarks->Lines->Add(Lites);
		 Lites = "Total Detailed Area = ";
		 Lites += SumAreaStart;
		 FormMain->MemoProductRemarks->Lines->Add(Lites);
//		 WideString Buf;
//		 Buf = "Eksport z Coater Report" ;
//		 ExcelSheet.OlePropertySet("Name",Buf);
//
//		 AnsiString a = ExcelSheet.OlePropertyGet("Name");
//
//	//	 for(int i=0;i<Sheet->ColCount;i++)
//	//	 {
//	//	  for(int j=0;j<Sheet->RowCount;j++)
//	//	  {
//	//	  Buf = Sheet->Cells[i][j].c_str();
//	//	   ExcelSheet.OlePropertySet(L"Cells",j+1 ,i+1,Buf/*Sheet->Cells[i][j].w_str()*/);
//	////	   Temp=ExcelSheet.OlePropertyGet(L"Cells",j+1 ,i+1);///*Sheet->Cells[i][j].w_str()*/);
//	////	   Temp=Temp.OlePropertyGet(L"Borders");
//	////	   Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
//	//	  }
//	//	 }
//		   ExportToClipboard(Sheet);
//
//		   Temp1 = ExcelSheet.OlePropertyGet(L"Cells",1,1);
//		   Temp2 = ExcelSheet.OlePropertyGet(L"Cells",Sheet->RowCount,Sheet->ColCount);
//		   ExcelSheet.OleProcedure(L"Paste",Temp1);//(
//		   Temp=ExcelSheet.OlePropertyGet(L"Range",Temp1,Temp2);///*Sheet->Cells[i][j].w_str()*/);
//		   Temp=Temp.OlePropertyGet(L"Borders");
//		   Temp.OlePropertySet(L"LineStyle",L"xlContinuous");
//		 for(int i=0;i<Sheet->ColCount;i++)
//		 {
//
//		  //Buf = Sheet->Cells[i][j].c_str();
//		   Temp=ExcelSheet.OlePropertyGet(L"Cells",1 ,i+1);///*Sheet->Cells[i][j].w_str()*/);
//		   Temp=Temp.OlePropertyGet(L"Font");
//		   Temp.OlePropertySet(L"Bold",L"True");
//		 }
//
//		  Temp=ExcelSheet.OlePropertyGet(L"Columns");//,Sheet->ColCount);
//		  Temp.OleProcedure("AutoFit");
//		  Temp=ExcelSheet.OlePropertyGet(L"Rows");//,Sheet->ColCount);
//		  Temp.OleProcedure("AutoFit");
//
//		  ExcelSheet.OleProcedure("Activate");
//
//
//	//	 for(int i=0;i<Sheet->ColCount;i++)
//	//	 {
//	//
//	//	   Buf = Sheet->Cols[i]->GetText();  //.c_str();
//	//	   ExcelSheet.OlePropertySet(L"Columns",i+1,Buf/*Sheet->Cells[i][j].w_str()*/);
//	//
//	//	   Temp=ExcelSheet.OlePropertyGet("Columns",i+1);//,Sheet->ColCount);
//	//	   Temp.OleProcedure("AutoFit");
//	//	 }
//
//	//	 for(int j=0;j<Sheet->RowCount;j++)
//	//	 {
//	//		Temp=ExcelSheet.OlePropertyGet("Rows",j+1);//,Sheet->ColCount);
//	//		Temp.OleProcedure("AutoFit");
//	//	 }
//	//	  for(int j=0;j<Sheet->RowCount;j++)
//	//	  {
//	//	   if(Sheet->Cells[0][j+1].IsEmpty()) continue;
//	//	   Buf =  AnsiString(" "+Sheet->Cells[0][j+1]). c_str();
//	//	   ExcelSheet.OlePropertySet("Cells",j+2 , 1,Buf );
//	//	  }
//	//	  Temp=ExcelSheet.OlePropertyGet("Columns",1);//,Sheet->ColCount);
//	//	  Temp.OleProcedure("AutoFit");

		  ExcelApp = Unassigned;
		  ExcelApp= Variant().Empty();
		return;

  }
}

void __fastcall TFormPacks::ButtonAddClick(TObject *Sender)
{
	int NoLites;
	int NoPacks;
	int IdWorkshift;
	int Id;
	int IdCoating;
	int IdSubstrate;
	int IdSubstrateClass;
	int IdSize;
	int IdThickness;
	NoLites = EditLites->Text.ToInt();
	NoPacks = EditPacks->Text.ToInt();
	IdWorkshift = Current.GetID();
	IdCoating = Combos.CoatingsCombo.GetDBId(ComboBoxProduct->ItemIndex);
	IdSubstrate = Combos.SubstrateCombo.GetDBId(ComboBoxSubstrate->ItemIndex);

	if(ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
	{
	   IdSubstrateClass = Combos.SubstrateClassLamiCombo.GetDBId(ComboBoxSubstrateClass->ItemIndex);
	   IdThickness = Combos.ThicknessLamiCombo.GetDBId(ComboBoxThickness->ItemIndex);
	}
	else
	{
	   IdSubstrateClass = Combos.SubstrateClassFloatCombo.GetDBId(ComboBoxSubstrateClass->ItemIndex);
	   IdThickness = Combos.ThicknessFloatCombo.GetDBId(ComboBoxThickness->ItemIndex);
	}
	IdSize = Combos.SizeCombo.GetDBId(ComboBoxSize->ItemIndex);

	try {
	  if(!DataModule1->ADOConnectionCreport->Connected)
		 DataModule1->ADOConnectionCreport->Connected=true;
	  TADOQuery* Query = DataModule1->ADOQueryInsertPack;
	  Query->Parameters->ParamByName("ID_Workshiftday")->Value =IdWorkshift;
	  Query->Parameters->ParamByName("ID_PROD_TYPE")->Value=IdCoating;
	  Query->Parameters->ParamByName("ID_SUBSTRATE")->Value=IdSubstrate;
	  Query->Parameters->ParamByName("ID_SUBSTRATECLASS")->Value=IdSubstrateClass;
	  Query->Parameters->ParamByName("ID_THICKNESS")->Value=IdThickness;
	  Query->Parameters->ParamByName("ID_SIZE")->Value =  IdSize;
	  Query->Parameters->ParamByName("NO_PACKS")->Value =  NoPacks;
	  Query->Parameters->ParamByName("NO_PANES")->Value =  NoLites;
	  DataModule1->ADOConnectionCreport->BeginTrans();
	  Query->ExecSQL();
	  if(Query->RowsAffected<1 )      //Id = 0 means last identity insert wasn't sucessfull
	  {
		DataModule1->ADOConnectionCreport->RollbackTrans();
		ShowMessage("Dodanie danych do bazy danych w funkcji TFormPacks::ButtonAddClick nie uda³o siê\n\n"
					"Data insert into database in function TFormPacks::ButtonAddClick failed");
		DataModule1->ADOConnectionCreport->RollbackTrans();
		return;
	  }
	  StartingCoatingID = IdCoating;
	  DataModule1->ADOConnectionCreport->CommitTrans();
	  TADOQuery* QueryID = DataModule1->ADOQueryPacksID;
	  QueryID->Close();
//	  UnicodeString QueryString = "SELECT IDENT_CURRENT('PACKED') AS LastIdentity";
//	  QueryID->SQL->Add(QueryString);
	  QueryID->Open();
	  Id = QueryID->FieldByName("LastIdentity")->AsInteger;


	  if( Id==0 )      //Id = 0 means last identity insert wasn't sucessfull
	  {
		//DataModule1->ADOConnectionCreport->RollbackTrans();
		ShowMessage("Dodanie danych do bazy danych w funkcji TFormPacks::ButtonAddClick nie uda³o siê - Identity issue\n\n"
					"Data insert into database in function TFormPacks::ButtonAddClick failed");
		return;
	  }


//	  int i=1;
//	  while (!StringGrid1->Cells[0][i].IsEmpty()) //find first empty row in StringGrid
//	  {
//		i++;
//	  }
//	  RowToDBID[i]=Id;
//
//	  StringGrid1->Cells[0][i] = NoPacks;
//	  StringGrid1->Cells[1][i] = NoLites;
//	  StringGrid1->Cells[2][i] = Combos.CoatingsCombo.GetNameByDBId(IdCoating);
//	  StringGrid1->Cells[3][i] = Combos.SubstrateCombo.GetNameByDBId(IdSubstrate);
//	  StringGrid1->Cells[4][i] = Combos.SubstrateClassCombo.GetNameByDBId(IdSubstrateClass);
//	  StringGrid1->Cells[5][i] = Combos.SizeCombo.GetNameByDBId(IdSize);
//	  StringGrid1->Cells[6][i] = Combos.ThicknessCombo.GetNameByDBId(IdThickness);
//	  StringGrid1->Cells[7][i] = AreaOfPack();

	   FillStringGrid();




	}
	__finally{
			DataModule1->ADOConnectionCreport->Connected=false;
	}



}
//---------------------------------------------------------------------------

void __fastcall TFormPacks::EditPacksChange(TObject *Sender)
{
//   int NoLites = EditLites->Text.ToInt();
//   int NoPacks = EditPacks->Text.ToInt();
//   double Area = Combos.SizeCombo.GetArea(Combos.SizeCombo.GetDBId(ComboBoxSize->ItemIndex));
//   Area = NoLites*NoPacks*Area;
   LabelResultantArea->Caption = AreaOfPack();
}
//---------------------------------------------------------------------------

void TFormPacks::CalculateDefects()
{
 try{
  if(!DataModule1->ADOConnectionCreport->Connected)
			DataModule1->ADOConnectionCreport->Connected=true;
  TADOQuery* QueryInput = DataModule1->ADOQueryPacksInputGlass;
  QueryInput->Parameters->ParamByName("ID_Prod_Type")->Value = StartingCoatingID;
  UnicodeString Doc = Current.GetDOCNUM();
  QueryInput->Parameters->ParamByName("STR_DOCNUM")->Value = Doc;
  QueryInput->Open();
  int i=1;
  AreaDef = 0;
  int Length, Width, Quantity, Develop;
  while (!QueryInput->Eof)
  {
	 Develop = QueryInput->FieldByName("DEFECTID")->AsInteger;
	 if(Develop == 14 && !CheckBoxDevelopment->Checked)    //Skip Development;
	 {
		QueryInput->Next();
		continue;
	 }
	 Length = QueryInput->FieldByName("Length")->AsInteger ;
	 Width = QueryInput->FieldByName("Width")->AsInteger;
	 Quantity = QueryInput->FieldByName("Quantity")->AsInteger;
	 i++;
	 AreaDef += Length*Width*Quantity/1000000.0;
	 QueryInput->Next();
  }
  QueryInput->Close();
  ProdAreaIn = AreaDef + ProdAreaOut;
  EditOut->Text = ProdAreaOut;
  EditIn->Text = ProdAreaIn;
  Speed = Current.GetAverageLineSpeed(ProdID);
  EditSpeed->Text = Speed;
  }
  __finally {
			DataModule1->ADOConnectionCreport->Connected=false;
  }
}

void TFormPacks::FillStringGrid()
{
//	  int i=1;
	  TStringRow GridRow;
//	  TStringRow
//	  while (!StringGrid1->Cells[0][i].IsEmpty()) //find first empty row in StringGrid
//	  {
//		i++;
//	  }
	 // RowToDBID[i]=GridRow->Id;
	  TADOQuery* Query = DataModule1->ADOQueryPacksWorkProdID;
	  TADOQuery* QueryInput = DataModule1->ADOQueryPacksInputGlass;
	  EmptySheet(StringGrid1);
	  try {
		 if(!DataModule1->ADOConnectionCreport->Connected)
			DataModule1->ADOConnectionCreport->Connected=true;
		 Query->Parameters->ParamByName("ID_Prod_Type")->Value = StartingCoatingID;
		 Query->Parameters->ParamByName("ID_Workshiftday")->Value = Current.GetID();
		 Query->Open();
		 std::map<int,int>::iterator it;
//		 for(it=CoatIDToRowToDBID[StartingCoatingID].begin();it!=CoatIDToRowToDBID[StartingCoatingID].end();it++) //fill up StringGrid with data previously written to StringGrid
//		 {
		 int i=1;
		 //CoatIDToRowToDBID[StartingCoatingID].clear();
		 RowToDBID.clear();
		 double Area = 0;
		 StringGrid1->RowCount=Query->RecordCount+2;
		 while (!Query->Eof)
		 {
			GridRow.IdWorkshift = Query->FieldByName("ID_Workshiftday")->AsInteger ;
			GridRow.IdCoating = Query->FieldByName("ID_PROD_TYPE")->AsInteger;
			GridRow.IdSubstrate = Query->FieldByName("ID_SUBSTRATE")->AsInteger;
			GridRow.IdSubstrateClass = Query->FieldByName("ID_SUBSTRATECLASS")->AsInteger;
			GridRow.IdThickness=Query->FieldByName("ID_THICKNESS")->AsInteger;
			GridRow.IdSize = Query->FieldByName("ID_SIZE")->AsInteger ;
			GridRow.NoPacks = Query->FieldByName("NO_PACKS")->AsInteger ;
			GridRow.NoLites = Query->FieldByName("NO_PANES")->AsInteger ;
			GridRow.Id =  Query->FieldByName("ID")->AsInteger ;



			InsertRow(i,  GridRow);
			//CoatIDToRowToDBID[StartingCoatingID][i]=   GridRow.Id;
			RowToDBID[i]= GridRow.Id ;
			DBIdToRowData[GridRow.Id]=GridRow;
			i++;
			Area+=AreaOfPack(GridRow);
			Query->Next();
		 }


		 Query->Close();
		 StringGrid1->Row=i;

//		 QueryInput->Parameters->ParamByName("ID_Prod_Type")->Value = StartingCoatingID;
//		 UnicodeString Doc = Current.GetDOCNUM();
//		 QueryInput->Parameters->ParamByName("STR_DOCNUM")->Value = Doc;
//		// QueryInput->Parameters->ParamByName("ID_Workshiftday")->Value = Current.GetID();
//		 QueryInput->Open();
//		 i=1;
//		 AreaDef = 0;
//		 int Length, Width, Quantity, Develop;
//		 while (!QueryInput->Eof)
//		 {
//			Develop = QueryInput->FieldByName("DEFECTID")->AsInteger;
//			if(Develop == 14 && !CheckBoxDevelopment->Checked)    //Skip Development;
//			{
//			   QueryInput->Next();
//			   continue;
//			}
//			Length = QueryInput->FieldByName("Length")->AsInteger ;
//			Width = QueryInput->FieldByName("Width")->AsInteger;
//			Quantity = QueryInput->FieldByName("Quantity")->AsInteger;
//			i++;
//			AreaDef += Length*Width*Quantity/1000000.0;
//			QueryInput->Next();
//		 }
		 ProdAreaOut = Area;
		 ProdAreaIn = AreaDef + ProdAreaOut;
		 //ProdAreaIn = AreaIn + ProdAreaOut;
		 //LabelInData->Caption= ProdAreaIn;
		 //LabelOutData->Caption=Area;
		 EditOut->Text = Area;
		 EditIn->Text = ProdAreaIn;
		 ProdID = StartingCoatingID;
		 //Speed = Current.GetAverageLineSpeed(ProdID);
		 //LabelSpeed->Caption = Speed;
		 //EditSpeed->Text = Speed;

	  } __finally {
			DataModule1->ADOConnectionCreport->Connected=false;
	  }

}

void TFormPacks::InsertRow(int RowNo, TStringRow GridRowData)
{
	  StringGrid1->Cells[0][RowNo] = GridRowData.NoPacks;
	  StringGrid1->Cells[1][RowNo] = GridRowData.NoLites;
	  StringGrid1->Cells[2][RowNo] = Combos.CoatingsCombo.GetNameByDBId(GridRowData.IdCoating);
	  StringGrid1->Cells[3][RowNo] = Combos.SubstrateCombo.GetNameByDBId(GridRowData.IdSubstrate);
	  StringGrid1->Cells[4][RowNo] = Combos.SubstrateClassCombo.GetNameByDBId(GridRowData.IdSubstrateClass);
	  StringGrid1->Cells[5][RowNo] = Combos.SizeCombo.GetNameByDBId(GridRowData.IdSize);
	  StringGrid1->Cells[6][RowNo] = Combos.ThicknessCombo.GetNameByDBId(GridRowData.IdThickness);
	  StringGrid1->Cells[7][RowNo] = AreaOfPack(GridRowData);
}



void __fastcall TFormPacks::StringGrid1SelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
	if(StringGrid1->Cells[ACol][ARow].IsEmpty())
		return;
	ComboBoxSubstrate->ItemIndex  =	Combos.SubstrateCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdSubstrate);
	if(ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
	{
		SetLamiCombos();
		ComboBoxThickness->ItemIndex  =	Combos.ThicknessLamiCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdThickness);
		FormPacks->ComboBoxSubstrateClass->ItemIndex =	Combos.SubstrateClassLamiCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdSubstrateClass);
	}
	else
	{
	   SetFloatCombos();
	   ComboBoxThickness->ItemIndex  =	Combos.ThicknessFloatCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdThickness);
	   FormPacks->ComboBoxSubstrateClass->ItemIndex =	Combos.SubstrateClassFloatCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdSubstrateClass);
	}
	FormPacks->ComboBoxSubstrateClass->ItemIndex =	Combos.SubstrateClassFloatCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdSubstrateClass);
	//ComboBoxThickness->ItemIndex  =	Combos.ThicknessFloatCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdThickness);
	   //ComboBoxProduct->ItemIndex = Combos.CoatingsCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].);
	   //TComboBox* Combo = FormPacks->ComboBoxProduct
	ComboBoxSize->ItemIndex  =	Combos.SizeCombo.GetItemIndexFromIDDD(DBIdToRowData[RowToDBID[ARow]].IdSize);
	UpDownPacks->Position =  DBIdToRowData[RowToDBID[ARow]].NoPacks;
	UpDownLites->Position = DBIdToRowData[RowToDBID[ARow]].NoLites;
	EditPacksChange(Sender);
}


//---------------------------------------------------------------------------

void __fastcall TFormPacks::ButtonUpdateClick(TObject *Sender)
{

	int ARow = StringGrid1->Row;
	if(StringGrid1->Cells[0][ARow].IsEmpty())
		return;
	int NoLites;
	int NoPacks;
//	int IdWorkshift;
	int Id;
	int IdCoating;
	int IdSubstrate;
	int IdSubstrateClass;
	int IdSize;
	int IdThickness;
	NoLites = EditLites->Text.ToInt();
	NoPacks = EditPacks->Text.ToInt();
	Id = RowToDBID[ARow];
//	IdWorkshift = Current.GetID();
//	IdCoating = Combos.CoatingsCombo.GetDBId(ComboBoxProduct->ItemIndex);
	IdSubstrate = Combos.SubstrateCombo.GetDBId(ComboBoxSubstrate->ItemIndex);
    if(ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
	{
	   IdSubstrateClass = Combos.SubstrateClassLamiCombo.GetDBId(ComboBoxSubstrateClass->ItemIndex);
	   IdThickness = Combos.ThicknessLamiCombo.GetDBId(ComboBoxThickness->ItemIndex);
	}
	else
	{
	   IdSubstrateClass = Combos.SubstrateClassFloatCombo.GetDBId(ComboBoxSubstrateClass->ItemIndex);
	   IdThickness = Combos.ThicknessFloatCombo.GetDBId(ComboBoxThickness->ItemIndex);
	}

	//IdSubstrateClass = Combos.SubstrateClassCombo.GetDBId(ComboBoxSubstrateClass->ItemIndex);
	IdSize = Combos.SizeCombo.GetDBId(ComboBoxSize->ItemIndex);
	//IdThickness = Combos.ThicknessCombo.GetDBId(ComboBoxThickness->ItemIndex);
	try {
	  if(!DataModule1->ADOConnectionCreport->Connected)
		 DataModule1->ADOConnectionCreport->Connected=true;
	  TADOQuery* Query = DataModule1->ADOQueryUpdatePacks;
	  //Query->Parameters->ParamByName("ID_Workshiftday")->Value =IdWorkshift;
	  //Query->Parameters->ParamByName("ID_PROD_TYPE")->Value=IdCoating;
	  Query->Parameters->ParamByName("ID_SUBSTRATE")->Value=IdSubstrate;
	  Query->Parameters->ParamByName("ID_SUBSTRATECLASS")->Value=IdSubstrateClass;
	  Query->Parameters->ParamByName("ID_THICKNESS")->Value=IdThickness;
	  Query->Parameters->ParamByName("ID_SIZE")->Value =  IdSize;
	  Query->Parameters->ParamByName("NO_PACKS")->Value =  NoPacks;
	  Query->Parameters->ParamByName("NO_PANES")->Value =  NoLites;
	  Query->Parameters->ParamByName("ID")->Value = Id;
	  DataModule1->ADOConnectionCreport->BeginTrans();
	  Query->ExecSQL();
	  if(Query->RowsAffected<1 )      //
	  {
		DataModule1->ADOConnectionCreport->RollbackTrans();
		ShowMessage("Dodanie danych do bazy danych w funkcji TFormPacks::ButtonUpdateClick nie uda³o siê\n\n"
					"Data insert into database in function TFormPacks::ButtonUpdateClick failed");
		DataModule1->ADOConnectionCreport->RollbackTrans();
		return;
	  }
	  //StartingCoatingID = IdCoating;
	  DataModule1->ADOConnectionCreport->CommitTrans();
//	  QueryID->Close();
//	  UnicodeString QueryString = "SELECT IDENT_CURRENT('PACKED') AS LastIdentity";
//	  QueryID->SQL->Add(QueryString);
	  //QueryID->Open();
//	  Id = QueryID->FieldByName("LastIdentity")->AsInteger;
//
//
//	  if( Id==0 )      //Id = 0 means last identity insert wasn't sucessfull
//	  {
//		//DataModule1->ADOConnectionCreport->RollbackTrans();
//		ShowMessage("Dodanie danych do bazydanych w funkcji TFormPacks::ButtonAddClick nie uda³o siê - Identity issue\n\n"
//					"Data insert into database in function TFormPacks::ButtonAddClick failed");
//		return;
//	  }

	   FillStringGrid();
	}
	__finally{
			DataModule1->ADOConnectionCreport->Connected=false;
	}





}
//---------------------------------------------------------------------------

void __fastcall TFormPacks::ButtonDeleteClick(TObject *Sender)
{

	int ARow = StringGrid1->Row;
    if(StringGrid1->Cells[0][ARow].IsEmpty())
		return;

	int 	Id = RowToDBID[ARow];


	try {
	  if(!DataModule1->ADOConnectionCreport->Connected)
		 DataModule1->ADOConnectionCreport->Connected=true;
	  TADOQuery* Query = DataModule1->ADOQueryDeletePack;
	  Query->Parameters->ParamByName("ID")->Value = Id;
	  DataModule1->ADOConnectionCreport->BeginTrans();
	  Query->ExecSQL();
	  if(Query->RowsAffected<1 )      //
	  {
		DataModule1->ADOConnectionCreport->RollbackTrans();
		ShowMessage("Kasowanie danych w bazie danych w funkcji TFormPacks::ButtonDeleteClick nie uda³o siê\n\n"
					"Data deletion from database in function TFormPacks::ButtonDeleteClick failed");
		DataModule1->ADOConnectionCreport->RollbackTrans();
		return;
	  }
	  DataModule1->ADOConnectionCreport->CommitTrans();

	   FillStringGrid();
	}
	__finally{
			DataModule1->ADOConnectionCreport->Connected=false;
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormPacks::ButtonCloseClick(TObject *Sender)
{
		Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormPacks::FormCloseQuery(TObject *Sender, bool &CanClose)
{
	FormMain->EditOut->Text=ProdAreaOut;
	FormMain->EditIn->Text=ProdAreaIn;
	FormMain->EditSpeed->Text = Speed;
	//void AddProduction();
	AddProduction();
	CanClose = true;
}
//---------------------------------------------------------------------------



void __fastcall TFormPacks::CheckBoxDevelopmentClick(TObject *Sender)
{
   CalculateDefects();
}
//---------------------------------------------------------------------------



//---- Import from Excel ----------------


int ProdIDFromPart(UnicodeString Part)
{
   UnicodeString Start;
   UnicodeString Extended;
   extern std::list<UnicodeString> ProdExtensions;
   std::map<int, UnicodeString> ExtLengthToExtension;
   ExtLengthToExtension.clear();
   int ProdID=0;
   std::map<int,TProducts>::iterator ProductsIt;
   std::list<UnicodeString>::iterator it;
   for(ProductsIt=Products.begin();ProductsIt !=Products.end();ProductsIt++) //looking for ProdID of product included in part no
   {
	   Start = ProductsIt->second.GetPart();
	   if (Part.Pos(Start)!=0)
	   {
		  ProdID =  ProductsIt->second.GetID();
		  ExtLengthToExtension[Start.Length()]=Start;
		  for(it = ProdExtensions.begin();it!=ProdExtensions.end();it++)
		  {
			 Extended = Start + *it;
			 if (Part.Pos(Extended)!=0) {
				   ExtLengthToExtension[Extended.Length()]=Extended;
			 }
		  }
		  Extended = ( --ExtLengthToExtension.end() )->second; //The longest part is most valid
		  if(Start == Extended) //first found is right
		  {
			break;   //found is ProdID
		  }
		  else  //Find product with part equal extended
		  {
			// Szukaj Produktu zawieraj¹cego Extended
			for(;ProductsIt !=Products.end();ProductsIt++)
			{
			  if (ProductsIt->second.GetPart().Pos(Extended)!=0)
			  {
				   ProdID =  ProductsIt->second.GetID();
				   break;
			  }
			}
			break;
		  }
	   }
   }
   if (ProductsIt==Products.end()) //Prod not find
   {
	   ShowMessage("Could not locate valid product for given partID\n\nPartID = "+Part);
	   return ProdID;
   }
   return ProdID;
}



PackStruct ThicknessFloat(UnicodeString Part,int ProdID)
{
   UnicodeString ProdPart = Products[ProdID].GetPart() ;
   UnicodeString NumStr="";
   PackStruct Pack;
   int Pos = Part.Pos(ProdPart);
   int Length = ProdPart.Length();
   Pos = Pos + Length ;
   Pack.SubType="";
   while (!IsDigit(Part[Pos]))
   {
	  Pack.SubType+=Part[Pos];
	  Pos++;
   }

  // Pos--;

   while ( Pos<=Part.Length() && IsDigit(Part[Pos]) )
   {
	 NumStr +=Part[Pos];
	 Pos++;
   }
   Pack.Thickness= NumStr.ToInt()/100;
   Pack.Origin="";
   if (Pos!=Part.Length()) {   //there is something else
	 //
	 Pos = Part.Pos("P_R");
	 if (Pos==0) {
			Pack.SomethingUnknown=true;
	 }
	 else
	 {
		   ProdPart = Part.SubString(Pos+UnicodeString("P_R").Length(),Part.Length());
		   Pos = ProdPart.Pos("_");
		   Pos++;
		   while(Pos<=ProdPart.Length() && IsLetterOrDigit(ProdPart[Pos]))
		   {
			 Pack.Origin +=ProdPart[Pos];
			 Pos++;
		   }
	 }
   }
   Pos = Part.Pos("SAMPLE");
   if (Pos!=0) {
			Pack.IsSample =true;
   }

   return  Pack;
}

PackStruct ThicknessLami(UnicodeString Part,int ProdID)
{
   UnicodeString ProdPart = Products[ProdID].GetPart() ;
   UnicodeString First="";
   UnicodeString Second="", Foil="";
   PackStruct Pack;
   int Pos = Part.Pos(ProdPart);
   int Length = ProdPart.Length();
   Pos = Pos + Length ;
//   Pack.SubType="";
//   while (!IsDigit(Part[Pos]))
//   {
//	  Pack.SubType+=Part[Pos];
//	  Pos++;
//   }
//   Pos--;

   ProdPart = Part.SubString(Pos,Part.Length());
   Pos = ProdPart.Pos("/");
   while (!IsDigit(ProdPart[Pos++]))
		;
   Pos--;

   while ( Pos<=ProdPart.Length() && IsDigit(ProdPart[Pos]) )
   {
	 First +=ProdPart[Pos];
	 Pos++;
   }
   Foil="";
   Pos = ProdPart.Pos("_");
   if (Pos!=0 && ProdPart[Pos-1] != 'P')
   {
	  Pos++;
	  if (IsDigit(ProdPart[Pos]))
	  {
		while ( Pos<=ProdPart.Length() && IsDigit(ProdPart[Pos]) )
		{
		  Foil += ProdPart[Pos];
		  Pos++;
		}
	  }
	  else
	  {
		 Foil="1";
	  }
      Pack.SubType ="";
	  int PosPlus = ProdPart.Pos("+");
	  while (Pos!= PosPlus)
	  {
		 Pack.SubType+=ProdPart[Pos];
		 Pos++;
	  }
   }
   else
   {
	  Foil="1";
   }

//   Pos--;
//
   Pos = ProdPart.Pos("+");
   while (!IsDigit(ProdPart[Pos++]))
		;
   Pos--;

   while ( Pos<=ProdPart.Length() && IsDigit(ProdPart[Pos]) )
   {
	 Second +=ProdPart[Pos];
	 Pos++;
   }

   Pack.Thickness= First+"."+Second+"."+Foil;
   Pack.Origin="";
   if (Pos!=Part.Length()) {   //there is something else
	 //
	 Pos = Part.Pos("P_R");
	 if (Pos==0) {
			Pack.SomethingUnknown=true;
	 }
	 else
	 {
			ProdPart = Part.SubString(Pos+UnicodeString("P_R").Length(),Part.Length());
			Pos = ProdPart.Pos("_");
			Pos++;
			while(Pos<=ProdPart.Length() && IsLetterOrDigit(ProdPart[Pos]))
			{
			  Pack.Origin +=ProdPart[Pos];
			  Pos++;
			}
	 }
   }
   Pos = Part.Pos("SAMPLE");
   if (Pos!=0) {
			Pack.IsSample =true;
   }
   return  Pack;


}

PackStruct DecodePart(UnicodeString Part)
{
   PackStruct PackP;
   int ProdID=0;
   UnicodeString Start;

   Start = Part.SubString(1,1);
   ProdID = ProdIDFromPart(Part);

   if (Start.Pos("L")!=0)
   {
	 PackP = ThicknessLami(Part,ProdID);
	 PackP.IsLami =  true;
   }
   else
   {
	 PackP =  ThicknessFloat(Part,ProdID);
	 PackP.IsLami =  false;
   }
   PackP.CoatID=ProdID;
   return PackP;
}




// ---------- Import from Excel ------------ end
