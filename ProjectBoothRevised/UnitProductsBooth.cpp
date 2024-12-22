//---------------------------------------------------------------------------

#pragma hdrstop

#include "UnitProductsBooth.h"
#include "UnitMainBooth.h"
#include "GlobalBooth.h"
#include "UnitAdditionalDefectDetailsFormBooth.h"
#include "DataBooth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

//---------------------------------------------------------------------------


#pragma hdrstop

#include <map>
//---------------------------------------------------------------------------

#pragma package(smart_init)


std::vector<TDefectExpanded> ChoosenSubCoats(50);
std::vector<TDefectExpanded> Entities(1000);
//std::vector<TSummaryDefectExpanded> SummarizedEntities(1000); //        TSummarizedEntitiesVector
 TSummarizedEntitiesVector	SummarizedEntities; //(1000);

bool AddToDB(TDefectExpanded Def)
{
  try{
   try{
	 TADOQuery* Query = DataModule2->ADOQueryCreport;
	 DataModule2->ADOConnectionCreport->Connected=true;

	 // Relation CP_DEFECTEXPANDED
	 int SubstrateID;  				//ID_SUBSTRATE       //GIP, Bascharage, Flachglass etc.
	 int ProdTypeID;   				//ID_PRODTYPE
	 int SubstrateClassID; 			//ID_SUBSTRATECLASS
	 int ThicknessID;      			//ID_THICKNESS
	 int AdditionalDefectID; 	   	//ID_ADDITIONALDEFECT
	 UnicodeString Comment;			//COMMENT

	 int ForeignKey ;       //Connected by

	 // Relation CP_DEFECT
	 int SizeID;    				//SIZEID        //Jumbo, Split etc.
	 int DefectID;  				//DEFECTID
	 int Quantity;  				//QUANTITY
	 TDateTime EventDate; 			//DATETIME
	 UnicodeString DocNum;			//DOCNUM
	 int Workshift ;					//ID_WORKSHIFTDAY

	SubstrateID=Def.GetSubstrate().GetID();
	ProdTypeID=Def.GetCoating().GetID();
	SubstrateClassID=Def.GetSubstrate().GetSubClassID();
	ThicknessID=Def.GetSubstrate().GetThicknessID();
	AdditionalDefectID=Def.GetDefect().GetAddDefID();
	Comment=Def.GetComment();

	SizeID=Def.GetSubstrate().GetSizeID();
	DefectID=Def.GetDefect().GetKindNumber();
	Quantity=Def.GetDefect().GetNumberOfDefects();
	EventDate = Def.GetDate() ; 			//DATETIME
	DocNum=Current.GetID_GIP();
	Workshift= Current.GetID();

	UnicodeString QueryString,Name;

	int TymID;

	QueryString = " INSERT INTO CP_DEFECT ([DOCNUM], [DATETIME],[SIZEID],DEFECTID,[QUANTITY],[ID_WORKSHIFTDAY]) "
				  " VALUES (:DOCNUM, :DAT, :SIZEID,:DEFECTID,:QUANTITY,  :ID_WORKSHIFTDAY) ";

	 Query->SQL->Clear();
	 Query->Parameters->Clear();
	 Query->SQL->Add(QueryString);
	 Query->Parameters->ParamByName("DOCNUM")->Value= DocNum;
	 Query->Parameters->ParamByName("DAT")->DataType=ftDateTime;
	 Query->Parameters->ParamByName("DAT")->Value= EventDate;
	 Query->Parameters->ParamByName("SIZEID")->Value= SizeID;
	 Query->Parameters->ParamByName("DEFECTID")->Value= DefectID;
	 Query->Parameters->ParamByName("QUANTITY")->Value= Quantity ;
	 Query->Parameters->ParamByName("ID_WORKSHIFTDAY")->Value= Workshift;

	 DataModule2->ADOConnectionCreport->BeginTrans();
	 int NoOfRowAffected = Query->ExecSQL();

	 Query->SQL->Clear();
	 Query->Parameters->Clear();

	 QueryString = "SELECT ID_DEFECT FROM CP_DEFECT WHERE [DATETIME] = :DAT ;";
	 Query->SQL->Add(QueryString);
	 Query->Parameters->ParamByName("DAT")->Value= EventDate;
	  Query->Open();


	 UnicodeString Message;
	 if (Query->RecordCount!=1 || NoOfRowAffected ==0 ) {
		 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
		 Message+=" Nie uda³o siê zapisaæ defektów (funkcja AddToDB) ";
		 Message+="\n\n\n";
		 Message+="There is mistake in database!   \n";
		 Message+="The writting of the data for defect was impossible!";
		 //wchar_t* Buf;
		 Application->MessageBox(Message.w_str(),L"Uwaga - Attention!!!",MB_OK);
		 DataModule2->ADOConnectionCreport->RollbackTrans();
		 return false;
	}

	ForeignKey = Query->FieldByName("ID_DEFECT")->AsInteger;

	Query->SQL->Clear();



	 QueryString = " INSERT INTO CP_DEFECTEXPANDED (ID_DEFECT,[ID_PRODTYPE], [ID_SUBSTRATE],[ID_SUBSTRATECLASS],ID_THICKNESS,[ID_ADDITIONALDEFECT],[COMMENT]) "
				  " VALUES (:ID_DEFECT,:ID_PRODTYPE, :ID_SUBSTRATE, :ID_SUBSTRATECLASS,:ID_THICKNESS,  :ID_ADDITIONALDEFECT, :COMMENT) ";


	 Query->SQL->Add(QueryString);
	 Query->Parameters->ParamByName("ID_DEFECT")->Value= ForeignKey;
	 Query->Parameters->ParamByName("ID_PRODTYPE")->Value= ProdTypeID;
	 Query->Parameters->ParamByName("ID_SUBSTRATE")->Value= SubstrateID;
	 Query->Parameters->ParamByName("ID_SUBSTRATECLASS")->Value= SubstrateClassID;
	 Query->Parameters->ParamByName("ID_THICKNESS")->Value=  ThicknessID;
	 Query->Parameters->ParamByName("ID_ADDITIONALDEFECT")->Value= AdditionalDefectID;
	 Query->Parameters->ParamByName("COMMENT")->Value= Comment;

	 NoOfRowAffected=Query->ExecSQL();

	 if (NoOfRowAffected ==0) {
		 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
		 Message+=" Nie uda³o siê zapisaæ defektów (function AddToDB - inserting into CP_DEFECTEXPANDED) ";
		 Message+="\n\n\n";
		 Message+="There is mistake in database!   \n";
		 Message+="The writting of the data for defect was impossible!";
		 //wchar_t* Buf;
		 Application->MessageBox(Message.w_str(),L"Uwaga - Attention!!!",MB_OK);
		 DataModule2->ADOConnectionCreport->RollbackTrans();
		 return false;
	}
	DataModule2->ADOConnectionCreport->CommitTrans();
	return true;
//	AnsiString Message;
//	if (Query->RecordCount>1) {
//		 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
//		 Message+=" Istnieje wiêcej ni¿ jeden zapis o tej samej dacie (dniu) i rodzaju zmiany.";
//		 Message+="\nOdczytany zostanie tylko pierwszy zapis!\n\n";
//		 Message+="There is mistake in database!  More than one entity \n";
//		 Message+="with the same date and shift kind exists. There only one will be read !";
//		 wchar_t* Buf;
//		 Application->MessageBox(Message.WideChar(Buf,Message.WideCharBufSize()),L"Uwaga - Attention!!!",MB_OK);
//		 if(ShiftLetter>0) // if we know the seekeing shift letter and there are more than one database with given date and shift kind
//		 {
//			 Query->Locate("ID_ShiftGroup",ShiftLetter,TLocateOptions());        //Make this record current
//		 }
//	}
//	if(Query->RecordCount>=1)
//	{
//		ExistsInDatabase = true;
//		Id = Query->FieldByName("ID_Workshiftday")->AsInteger;
//		return true;
//	}
//	else
//	{
//		ExistsInDatabase = false;
//		return false;
//	}
  }
  catch(...)
  {
	 DataModule2->ADOConnectionCreport->RollbackTrans();
  }
  }
  __finally{
	   DataModule2->ADOConnectionCreport->Connected=false;
  }
}

void AddDefect(int DBId)
{
	if(FormMain->RadioGroupSubstrate->ItemIndex < 0) // No substrate choosen
	   return;
	if(Current.GetID()==0)
	{
	  ShowMessage("Proszê wczytaæ dane zmiany!\n\nThe Shift data need to be read");
	  return;
	}
	static int AdditionalIndex=0;
   //	static i=0;
   //	i++;
//   FormMain->StringGrid1->Cells[1][i] =DBId;
//   FormMain->StringGrid1->Cells[2][i] =Combos.DefectsCombo.GetNameByDBId(DBId);
//   FormMain->StringGrid1->Cells[3][i] =Combos.DefectsCombo.GetEdit(DBId)->Text;
//   FormMain->StringGrid1->Cells[4][i] =ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetSubName();
//   FormMain->StringGrid1->Cells[5][i] =ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetSubClassName();
//   FormMain->StringGrid1->Cells[6][i] =ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetThicknessName();
//   FormMain->StringGrid1->Cells[7][i] =ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetWidth();
//   FormMain->StringGrid1->Cells[8][i] =ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetLength();
//   FormMain->StringGrid1->Cells[9][i] =ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetAreaM2()*Combos.DefectsCombo.GetEdit(DBId)->Text.ToInt();
//   if( i >= FormMain->StringGrid1->RowCount)
//	   FormMain->StringGrid1->RowCount=FormMain->StringGrid1->RowCount+5;

	 TSubstrate Sub;
	 TCoating Coat;
	 TDefectDetail Def;
	 TDefectExpanded LastRead;

	 int SubstrateID;
	 UnicodeString SubstrateName;
	 int ProdTypeID;
	 UnicodeString ProdTypeName;
	 int SubstrateClassID;
	 UnicodeString SubClassName;
	 int ThicknessID;
	 UnicodeString ThicknessName;
	 int AdditionalDefectID=0;
	 UnicodeString AdDefectName;
	 int SizeID;
	 UnicodeString SizeName;
	 int DefectID;
	 UnicodeString DefectName;
	 int Quantity;
	 UnicodeString Comment;
	 TDateTime EventDate=TDateTime().CurrentDateTime();
	 //ClearSheet(FormMain->StringGrid1);
	 DefectSheetHeader(FormMain->StringGrid1);
	 double OverallArea=0;
	 double Area;
	 int SheetIndex=1;
	 int EntityIndex = 1;
	 int Width, Length;
	 bool Increased = false;

//	 for(int i=1;i<SummarizedEntities.size();i++)
//	 {
//	   if( SummarizedEntities[i].GetDefExpanded().GetDefect().GetKindNumber()==0)  //End of Entities sequence
//	   {
//		   SheetIndex = i;
//		   break;
//	   }
//	 }

	 //SheetIndex = SummarizedEntities.size();
//	 for(int i=1;i<Entities.size();i++)
//	 {
//	   if( Entities[i].GetDefect().GetKindNumber()==0)  //End of Entities sequence
//	   {
//		   EntityIndex = i;
//		   break;
//	   }
//	 }


			DefectID  = DBId;
			ThicknessID = ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetThicknessID();
			SubstrateClassID = ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetSubClassID();
			SubstrateID  = ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetID();
			SizeName=ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetSizeName();
			SizeID=ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetSizeID();
			DefectName=Combos.DefectsCombo.GetNameByDBId(DBId);
			AdDefectName=""; //SummarizedEntities[i].GetDefect().GetAddidtionalDescription();

			Quantity=Combos.DefectsCombo.GetEdit(DBId)->Text.ToInt();
			SubstrateName=ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetSubName();
			SubClassName=ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetSubClassName();
			ThicknessName=ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetThicknessName();
			Width=ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetWidth();
			Length=ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetLength();
			Area = ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetSubstrate().GetAreaM2()*Quantity;
			ProdTypeID = ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetCoating().GetID();
			ProdTypeName = ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetCoating().GetShortName();
			if(DefectID==16 ) // Defect of float (generally substrate)
			{
			  FormAdditionalDefectDetails = new TFormAdditionalDefectDetails(FormMain);
			  //FormAdditionalDefectDetails->SetIDandComment(AdditionalIndex,"");
			  FormAdditionalDefectDetails->SetComboStart(AdditionalIndex);
			  FormAdditionalDefectDetails->ShowModal();
			  Comment = FormAdditionalDefectDetails->ReturnComment();
			  LastRead.AddComment(Comment);
			  AdditionalDefectID = FormAdditionalDefectDetails->ReturnAddDefID();
			  AdditionalIndex =  AdditionalDefectID;
			  //LastRead.AddAdditionalDefect(AdditionalDefectID);
			  delete FormAdditionalDefectDetails;//->Free();
			}
			//AdditionalDefectID =ChoosenSubCoats[FormMain->RadioGroupSubstrate->ItemIndex].GetDefect().GetAddDefID();  ;
			Sub.Add(
					 SubstrateID,SubstrateName,
					 SubstrateClassID, SubClassName,
					 ThicknessID, ThicknessName,
					 SizeName, SizeID,
					 Width, Length
				   );

			Coat.SetID(ProdTypeID);
			Coat.SetShortName(ProdTypeName);
			Def.Add(DefectName,AdDefectName,DefectID,Quantity,AdditionalDefectID);
			LastRead.AddCoating(Coat);
			LastRead.AddSubstrate(Sub);
			LastRead.AddDefect(Def);

			//LastRead.AddComment(Comment);
			LastRead.AddDate(EventDate);
//			Entities[EntityIndex]=LastRead;
			Increased = false;
//			for(int zz=1;zz<SheetIndex;zz++)
//			{
//			  if(SummarizedEntities[zz]==LastRead)
//			  {
//				SummarizedEntities[zz]=LastRead;
//				Increased = true;
//				break;
//			  }
//			}
//			if (!Increased) {
//			   SummarizedEntities[SheetIndex++]=LastRead;
//            }
			SummarizedEntities.push_back(LastRead);
			SheetIndex=SummarizedEntities.size();
			int ShiftID = Current.GetID();
			if(ShiftID!=0)
			{
				AddToDB(LastRead);
				ReadDefectFromDBToSheet(Current.GetID_GIP());
			}
			else
			{
			   FillUpDefectSheet(SheetIndex);
			}

}

 void TCoating::SetID(int ID)
 {
	 ID_ProdType = ID;
 }

 int TCoating::GetID()
 {
	 return ID_ProdType;
 }

void TCoating::SetShortName(AnsiString Name)
{
	ProdType_Name = Name;
}

void TCoating::SetFullName(AnsiString Name)
{
	FullName = Name;
}

TCoating::TCoating():ID_ProdType(0), Remarks(0)
{
   //	Remarks = 0;
}

void TCoating::SetRemarks(TStringList* R)
{
	if(Remarks!=0)
		delete Remarks;
	Remarks = new TStringList();
	Remarks->Text = R->Text;
}

TCoating::~TCoating()
{
//   if(Remarks!=0)
//    delete Remarks;
}

AnsiString TCoating::GetFullName()
{
  return FullName;
}

AnsiString TCoating::GetShortName()
{
	return ProdType_Name;
}

AnsiString TCoating::GetCombinedName()
{
   AnsiString Tym;
   Tym = "("+ProdType_Name+") "+FullName;
   return Tym;
}

TStringList* TCoating::GetRemarks()
 {
	return Remarks;
 }

//void TCoating::operator=(TCoating& Source)
//{
//	ID_ProdType = Source.ID_ProdType;
//	ProdType_Name= Source.ProdType_Name;
//	FullName = Source.FullName;
//
//	if(Remarks != 0)
//	{
//		delete Remarks;
//	}
//	if (Source.Remarks==0) {
//	  Remarks = 0;
//	  return;
//	}
//	Remarks = new TStringList;
//	Remarks->AddStrings(Source.Remarks);
//}

void TCoating::operator=(TCoating Source)
{
	ID_ProdType = Source.ID_ProdType;
	ProdType_Name= Source.ProdType_Name;
	FullName = Source.FullName;

	if(Remarks != 0)
	{
		delete Remarks;
	}
	if (Source.Remarks==0) {
	  Remarks = 0;
	  return;
	}
	Remarks = new TStringList;
	Remarks->AddStrings(Source.Remarks);
}


bool TCoating::operator==(const TCoating& Coat)
{
   return (ID_ProdType == Coat.ID_ProdType);
}


void TSubstrate::Add(int SubstrateID, UnicodeString NameOfSubstrate, int SubstrateClassID, UnicodeString SubstrateClass,int SubstrateThicknessID, UnicodeString SubstrateThickness, UnicodeString SubstrateSizeName, int SubstrateSizeID, int SizeWidth, int SizeLength)
{
 ID = SubstrateID;
 Name = NameOfSubstrate;
 ClassID = SubstrateClassID;
 Class = SubstrateClass;
 ThicknessID = SubstrateThicknessID;
 Thickness = SubstrateThickness;
 Size = SubstrateSizeName;
 SizeID = SubstrateSizeID;
 Width = SizeWidth;
 Length = SizeLength;
}


//void TDefectsSummary::Add(UnicodeString KindNa,UnicodeString AdditionalN,int KindNr, int NumberOfElements,int AddNr)
//{
//   TDefectDetail K = TDefectDetail(KindNa,AdditionalN,KindNr,NumberOfElements,AddNr);
//   DefectsList.push_back(K);
//}

//TDefectDetail TDefectsSummary::GetNextDefect()
//{
//  static TDefectDetail T= *DefectsList.begin();
//  if (NextEfectExists() ) {
//	T = *it++;
//	return T;
//  }
//  else
//	return T;
//}
//TDefectDetail TDefectsSummary::GetCurrentDefect()
//{
//  static TDefectDetail T= *DefectsList.begin();
//  if (NextEfectExists() ) {
//	T = *it;
//  }
//	return T;
//}
//
//bool TDefectsSummary::NextEfectExists()
//{
//  return it!=DefectsList.end();
//}
//
//
//bool TDefectsSummary::StartIteration()
//{
//	it = DefectsList.begin();
//	return it== DefectsList.begin();
//}

void TDefectDetail::Add(UnicodeString KindNa,UnicodeString AddDescr,int KindNr, int NumberOfElements,int AddDefectID)
{
 KindName = KindNa;
 AdditionalDescription = AddDescr;
 Kind = KindNr;
 Number = NumberOfElements;
 AddDefID = AddDefectID;
}

void TDefectDetail::Add(UnicodeString KindNa,UnicodeString AddDescr,int KindNr, int NumberOfElements,int AddDefectID, int ID_Def)
{
 KindName = KindNa;
 AdditionalDescription = AddDescr;
 Kind = KindNr;
 Number = NumberOfElements;
 AddDefID = AddDefectID;
 ID = ID_Def;
}

UnicodeString TDefectDetail::GetAddidtionalDescription()
{
  if (AdditionalDescription.IsEmpty() && AddDefID!=0)
  {
	 AdditionalDescription = Combos.AdditionalDefectsCombo.GetNameByDBId(AddDefID);
  }
  return AdditionalDescription;

}

UnicodeString TDefectDetail::GetKindName()
{
  if (KindName.IsEmpty() && Kind!=0)
  {
	 KindName = Combos.DefectsCombo.GetNameByDBId(AddDefID);
  }
  return KindName;
}


double TSummaryDefectExpanded::GetSummaryArea()
{
	 SumArea =0;
	 for (int i = 0; i < DefectExpanded.size(); i++) {
	  SumArea += DefectExpanded[i].GetSubstrate().GetAreaM2() * DefectExpanded[i].GetDefect().GetNumberOfDefects();
	 }
	 return SumArea;
}

int TSummaryDefectExpanded::GetNoOfDefects()
{
   int NoOfDefects=0;
   for (int i = 0; i < DefectExpanded.size(); i++) {
	  NoOfDefects +=  DefectExpanded[i].GetDefect().GetNumberOfDefects();
	 }
   return NoOfDefects;
}



UnicodeString TSummaryDefectExpanded::SummarizeString()
{
	UnicodeString Napis = DefectExpanded[0].GetSubstrate().GetSubName();
	Napis +=", ";
	Napis += DefectExpanded[0].GetSubstrate().GetSubName();
	Napis +=", ";
	Napis += DefectExpanded[0].GetSubstrate().GetThicknessName();;
	Napis +=", ";
	Napis += DefectExpanded[0].GetSubstrate().GetSubClassName();
	Napis +=", ";
	Napis += DefectExpanded[0].GetSubstrate().GetAreaM2();
	Napis +=", ";
	Napis += DefectExpanded[0].GetDefect().GetKindName();
	Napis +=", ";
	Napis += DefectExpanded[0].GetDefect().GetNumberOfDefects();
	Napis +=", ";
	Napis += DefectExpanded[0].GetCoating().GetShortName();
	return Napis;
}
