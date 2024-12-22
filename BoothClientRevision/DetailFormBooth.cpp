//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DetailFormBooth.h"
#include "GlobalBooth.h"
#include "UnitMainBooth.h"
#include "UnitProductsBooth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAdditionalDetails *FormAdditionalDetails;
//---------------------------------------------------------------------------
__fastcall TFormAdditionalDetails::TFormAdditionalDetails(TComponent* Owner)
	: TForm(Owner)
{
	IsGlobal=false;
	GlobalComment="";
}
//---------------------------------------------------------------------------


void _fastcall TFormAdditionalDetails::FillRow(int i)
{
		i--;
		int Index = FormMain->StringGrid1->Row-1;
		FormAdditionalDetails->StringGridAdditional->Cells[0][i+1]=	   SummarizedEntities[Index].GetDefExpanded(i).GetDate().TimeString();
		FormAdditionalDetails->StringGridAdditional->Cells[1][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetCoating().GetShortName();
		FormAdditionalDetails->StringGridAdditional->Cells[2][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetSizeName();
		FormAdditionalDetails->StringGridAdditional->Cells[3][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetDefect().GetKindName();
		FormAdditionalDetails->StringGridAdditional->Cells[4][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetDefect().GetAddidtionalDescription();
		FormAdditionalDetails->StringGridAdditional->Cells[5][i+1]=    SummarizedEntities[Index].GetDefExpanded(i).GetNumberOfDefects();
		FormAdditionalDetails->StringGridAdditional->Cells[6][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetAreaM2();
		FormAdditionalDetails->StringGridAdditional->Cells[7][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetSubName();
		FormAdditionalDetails->StringGridAdditional->Cells[8][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetSubClassName();
		FormAdditionalDetails->StringGridAdditional->Cells[9][i+1]=    SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetThicknessName();
		FormAdditionalDetails->StringGridAdditional->Cells[10][i+1]=   SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetWidth();
		FormAdditionalDetails->StringGridAdditional->Cells[11][i+1]=   SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetLength();
		FormAdditionalDetails->StringGridAdditional->Cells[12][i+1]=   SummarizedEntities[Index].GetDefExpanded(i).GetComment();
}

void _fastcall TFormAdditionalDetails::FillRows()
{
  int Index = FormMain->StringGrid1->Row-1;
  int NoRows=SummarizedEntities[Index].NumberOfElements();
  StringGridAdditional->RowCount = NoRows+1;
  for(int i=1;i<=NoRows;i++)
  {
	FillRow(i);
  }
}

void __fastcall TFormAdditionalDetails::StringGridAdditionalSelectCell(TObject *Sender,
		  int ACol, int ARow, bool &CanSelect)
{
	 int SubstrateID;
	 UnicodeString SubstrateName;
	 int ProdTypeID;
	 UnicodeString ProdTypeName;
	 int SubstrateClassID;
	 UnicodeString SubClassName;
	 int ThicknessID;
	 UnicodeString ThicknessName;
	 int AdditionalDefectID;
	 UnicodeString AdDefectName;
	 int SizeID;
	 UnicodeString SizeName;
	 int DefectID;
	 UnicodeString DefectName;
	 int Quantity;
	 UnicodeString Comment;
	 TDateTime EventDate;
	 //ClearSheet(FormMain->StringGrid1);
	 //DefectSheetHeader(FormMain->StringGrid1);
	 double OverallArea=0;
	 double Area;
	 int SheetIndex = ARow-1;
	 int DefectSheetIndex = FormMain->StringGrid1->Row -1;
	 int Width, Length;
	 bool Increased = false;
			 ComboBoxSize->Text = SizeName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetSizeName();
			ComboBoxDefect->Text =DefectName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetDefect().GetKindName();
			ComboBoxDefectDetails->Text = AdDefectName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetDefect().GetAddidtionalDescription();
			Quantity=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetDefect().GetNumberOfDefects();
			ComboBoxSubstrate->Text = SubstrateName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetSubName();
			ComboBoxSubstrateClass->Text =  SubClassName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetSubClassName();
			ComboBoxThickness->Text = ThicknessName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetThicknessName();
			Width=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetWidth();
			Length=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetLength();
			EventDate=SummarizedEntities[DefectSheetIndex].GetDefExpanded(ARow-1).GetDate();
			EditDate->Text = EventDate.TimeString();
			//EventDate.FormatString("dd.mm.yyyy");
			EditDateDate->Text = EventDate.FormatString("dd.mm.yyyy"); EventDate.DateString(); //SummarizedEntities[DefectSheetIndex].GetDefExpanded(ARow-1).GetDate().DateString();
			ComboBoxProduct->Text = SummarizedEntities[DefectSheetIndex].GetDefExpanded(ARow-1).GetCoating().GetShortName();
			Comment =  SummarizedEntities[DefectSheetIndex].GetDefExpanded(ARow-1).GetComment();
			//MemoComments->Lines->Clear();
			RichEditComment->Lines->Clear();
//			if(IsGlobal)
//			{
//			   if( Comment.Pos(GlobalComment)== 0) // If global comment exists in local one do not show it
//			   {
//					MemoComments->Font->Color=clRed;
//					MemoComments->Lines->Add(GlobalComment);
//					MemoComments->Font->Color=clBlack;
//			   }
//			}
//			MemoComments->Lines->Add(Comment);
			int pos;
			if(IsGlobal)
			{
			   pos=Comment.Pos(GlobalComment);
			   if( pos== 0) // If global comment exists in local one do not show it
			   {
					RichEditComment->SelAttributes->Color=clRed;
					//RichEditComment->SelText
					RichEditComment->Lines->Add(GlobalComment);
//					pos=Comment.Pos(GlobalComment);
//					RichEditComment->SelStart=pos;
//					RichEditComment->SelLength=GlobalComment.Length();
//					RichEditComment->SelAttributes
					RichEditComment->SelAttributes->Color=clBlack;
			   }
			}
			RichEditComment->Lines->Add(Comment);
			FillRow(ARow);
//			ComboBoxDate->Text = EventDate;
//			FormMain->StringGrid1->Cells[0][i] =i;
//			FormMain->StringGrid1->Cells[1][i] =SizeName;
//			FormMain->StringGrid1->Cells[2][i] =DefectName;
//			FormMain->StringGrid1->Cells[3][i] =AdDefectName;
//			FormMain->StringGrid1->Cells[4][i] =Quantity;
//			FormMain->StringGrid1->Cells[5][i] =SubstrateName;
//			FormMain->StringGrid1->Cells[6][i] =SubClassName;
//			FormMain->StringGrid1->Cells[7][i] =ThicknessName;
//			FormMain->StringGrid1->Cells[8][i] =Width; //Combos.SizeCombo.GetWidthLength(SizeID).first;
//			FormMain->StringGrid1->Cells[9][i] =Length;//Combos.SizeCombo.GetWidthLength(SizeID).second;
//			Area = SummarizedEntities[i].GetSummaryArea();//   Sub.GetAreaM2()*Quantity;
			//FormMain->StringGrid1->Cells[10][i] = Area;
//			FormMain->StringGrid1->Cells[10][i]=UnicodeString().FormatFloat("#.##",Area);
//			OverallArea += Area;


//	 		FormMain->EditSummary->Text = OverallArea;
}
//---------------------------------------------------------------------------

void __fastcall TFormAdditionalDetails::ComboBoxSubstrateChange(TObject *Sender)
{
   if(ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
   {
	   Combos.SubstrateClassLamiCombo.Assign(FormAdditionalDetails->ComboBoxSubstrateClass);
	   Combos.ThicknessLamiCombo.Assign(FormAdditionalDetails->ComboBoxThickness);
   }
   else
   {
	  Combos.SubstrateClassFloatCombo.Assign(FormAdditionalDetails->ComboBoxSubstrateClass);
	  Combos.ThicknessFloatCombo.Assign(FormAdditionalDetails->ComboBoxThickness);
   }
//   switch(Combos.   .GetSubstrate().GetID()) //100  is ID of Lami
//	  {
//		case 100:
//					Combos.SubstrateClassLamiCombo.Assign(FormAdditionalDetails->ComboBoxSubstrateClass);
//					Combos.ThicknessLamiCombo.Assign(FormAdditionalDetails->ComboBoxThickness);
//					break;
//		default :
//					Combos.SubstrateClassFloatCombo.Assign(FormAdditionalDetails->ComboBoxSubstrateClass);
//					Combos.ThicknessFloatCombo.Assign(FormAdditionalDetails->ComboBoxThickness);
//					break;
//	  }
}
//---------------------------------------------------------------------------

void __fastcall TFormAdditionalDetails::ButtonAddComentClick(TObject *Sender)
{
	 int DefectSheetIndex = FormMain->StringGrid1->Row -1;
	 //SummarizedEntities[DefectSheetIndex].GetDefExpanded(StringGridAdditional->Row-1).AddComment(RichEditComment->Text);
	 int EntityID= SummarizedEntities[DefectSheetIndex].GetDefExpanded(StringGridAdditional->Row-1).GetDefect().GetEntityID();
	 UpdateComment(EntityID,RichEditComment->Text);
	 ReadDefectFromDBToSheet(Current.GetID_GIP());
	 FillRow(StringGridAdditional->Row);
}
//---------------------------------------------------------------------------


void __fastcall TFormAdditionalDetails::ButtonDeleteClick(TObject *Sender)
{
   int DefectSheetIndex = FormMain->StringGrid1->Row -1;
   int EntityID = SummarizedEntities[DefectSheetIndex].GetDefExpanded(StringGridAdditional->Row-1).GetDefect().GetEntityID();
   SummarizedEntities[DefectSheetIndex].Delete(StringGridAdditional->Row-1);
   DeleteEntity(EntityID);
   if(SummarizedEntities[DefectSheetIndex].NumberOfElements()==0)
   {
	 ReadDefectFromDBToSheet(Current.GetID_GIP());
	 //SummarizedEntities.erase(SummarizedEntities.begin()+DefectSheetIndex);
	 this->Close();
   }
   ReadDefectFromDBToSheet(Current.GetID_GIP()); //it need to be called twice because this changes the SummarizedEnties indexing
   FillRows();
}
//---------------------------------------------------------------------------

void __fastcall TFormAdditionalDetails::ButtonCloseClick(TObject *Sender)
{
	Close();
}
//---------------------------------------------------------------------------

void __fastcall TFormAdditionalDetails::ButtonUpdateClick(TObject *Sender)
{
	 int SubstrateID;
	 UnicodeString SubstrateName;
	 int ProdTypeID;
	 UnicodeString ProdTypeName;
	 int SubstrateClassID;
	 UnicodeString SubClassName;
	 int ThicknessID;
	 UnicodeString ThicknessName;
	 int AdditionalDefectID;
	 UnicodeString AdDefectName;
	 int SizeID;
	 UnicodeString SizeName;
	 int DefectID;
	 UnicodeString DefectName;
	 int Quantity;
	 UnicodeString Comment;
	 TDateTime EventDate;
	 double OverallArea=0;
	 double Area;
	// int SheetIndex = ARow-1;
	 int DefectSheetIndex = FormMain->StringGrid1->Row -1;
	 int Width, Length;
	 bool Increased = false;

	 int EntityID;// = SummarizedEntities[DefectSheetIndex].GetDefExpanded(StringGridAdditional->Row-1).GetDefect().GetEntityID();

	 SizeID = Combos.SizeCombo.GetIDDBFromName(ComboBoxSize->Text);// = SizeName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetSizeName();
	 DefectID = Combos.DefectsCombo.GetIDDBFromName(ComboBoxDefect->Text);// =DefectName=SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetDefect().GetKindName();
	 AdditionalDefectID= Combos.AdditionalDefectsCombo.GetIDDBFromName(ComboBoxDefectDetails->Text);// = SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetDefect().GetAddidtionalDescription();
	 SubstrateID = Combos.SubstrateCombo.GetIDDBFromName(ComboBoxSubstrate->Text);// = SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetSubName();
	 SubstrateClassID = Combos.SubstrateClassCombo.GetIDDBFromName(ComboBoxSubstrateClass->Text);// =  SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetSubClassName();
	 ThicknessID= Combos.ThicknessCombo.GetIDDBFromName(ComboBoxThickness->Text); // = SummarizedEntities[DefectSheetIndex].GetDefExpanded(SheetIndex).GetSubstrate().GetThicknessName();
	 ProdTypeID = Combos.CoatingsCombo.GetIDDBFromName(ComboBoxProduct->Text);// = SummarizedEntities[DefectSheetIndex].GetDefExpanded(ARow-1).GetCoating().GetShortName();


	 TDefectExpanded DefExp;
	 TSubstrate Sub;
	 TDefectDetail Def;
	 TCoating Coat;
	 Def.Add("","",DefectID,1,AdditionalDefectID);
	 Coat.SetID(ProdTypeID);
	 Sub.Add(SubstrateID,"",SubstrateClassID,"", ThicknessID,"","",SizeID,0,0);
	 DefExp.AddSubstrate(Sub);
	 DefExp.AddCoating(Coat);

	 int SelectedRow,StartRow,StopRow;
	 StartRow=StringGridAdditional->Selection.Top;
	 StopRow =StringGridAdditional->Selection.Bottom;

	 for(SelectedRow=StartRow-1;SelectedRow<StopRow;SelectedRow++)
	 {
		   EntityID = SummarizedEntities[DefectSheetIndex].GetDefExpanded(SelectedRow).GetDefect().GetEntityID();
		   Def.AddID(EntityID);
		   DefExp.AddDefect(Def);
		   UpdateEntity(DefExp);
	 }

   ReadDefectFromDBToSheet(Current.GetID_GIP()); //it need to be called twice because this changes the SummarizedEnties indexing
   SelectRow(EntityID);
   FillRows();
}
//---------------------------------------------------------------------------

