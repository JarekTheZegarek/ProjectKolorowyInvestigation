//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMainBooth.h"
//#include "UnitShiftReading.h"
#include "DataBooth.h"
#include "UnitStartUp.h"
#include "UnitReadShiftData.h"
#include "GlobalBooth.h"
#include "DetailFormBooth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMain *FormMain;
TMainEntity Current;
TDefectExpanded CurrentSubCoat;
TSuperviosorCompare  SupervisorsCompare;
//---------------------------------------------------------------------------


bool CheckIfShitfNotChoosen()   // Displays communicate if trying to choose operators or supervisor without choosing the shift letter
{                               // returning true if shift letter is choosen
   if(FormMain->ComboBoxShiftLetter->ItemIndex!= -1 || Current.CheckIfIsInDatabase(Current.GetDate(),Current.GetShiftLetter(),Current.GetShiftKind() )  )
	{
		return true;
	}
	int OldHeight, OldWidth;
	try {

		OldHeight = FormMain->ComboBoxShiftLetter->Height;
		OldWidth = FormMain->ComboBoxShiftLetter->Width;
		//FormMain->ComboBoxShiftLetter->Width=FormMain->ComboBoxShiftLetter->Width+150;
		//FormMain->ComboBoxShiftLetter->Height=FormMain->ComboBoxShiftLetter->Height+150;
		FormMain->ComboBoxShiftLetter->Visible=false;
		FormMain->ComboBoxShiftKind->Visible=false;
		FormMain->LabelShiftKind->Visible = false;
		FormMain->LabelShiftLetter->BringToFront();
		FormMain->LabelShiftLetter->Font->Color = clRed;
		for (int i = 0; i < 10; i++) {
		   FormMain->LabelShiftLetter->Font->Size=FormMain->LabelShiftLetter->Font->Size+i;
		   FormMain->LabelShiftLetter->Refresh();
		   Sleep(50);
		}
		Application->MessageBox(L"Podaj najpierw w³aœciw¹ zmianê \n\nFirst fill up relevant shift", L"Uwaga! Attention!",MB_OK);
		for (int i = 0; i < 10; i++) {
		   FormMain->LabelShiftLetter->Font->Size=FormMain->LabelShiftLetter->Font->Size-i;
		   FormMain->LabelShiftLetter->Refresh();
		   Sleep(50);
		}
		FormMain->ComboBoxShiftLetter->Visible=true;
		FormMain->ComboBoxShiftKind->Visible=true;
		FormMain->LabelShiftKind->Visible = true;
		FormMain->LabelShiftLetter->Font->Color = clBlack;

		//FormMain->ComboBoxShiftLetter->Height = OldHeight;
		//FormMain->ComboBoxShiftLetter->Width  = OldWidth;
		return false;

	}
	__finally {
		 FormMain->ComboBoxShiftLetter->Visible=true;
		 FormMain->ComboBoxShiftKind->Visible=true;
		 FormMain->LabelShiftKind->Visible = true;
		 FormMain->ComboBoxShiftLetter->Height = OldHeight;
		 FormMain->ComboBoxShiftLetter->Width  = OldWidth;
		 FormMain->ComboBoxShiftLetter->Color=clWhite;
	}


     return false;


}




__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{

   float OldWidth = Width;
   Width = Screen->WorkAreaWidth-6;
   Height = Screen->WorkAreaHeight-6;
   Left = Screen->WorkAreaLeft+3;
   Top = Screen->WorkAreaTop+3;
   float Factor = Width/OldWidth;//Panel1->Witdh/OldPanelWidth;
   //Fact = Factor;
   if(Width-PanelShift->Width-Image1->Width <0 )
   {
	 Image1->Width = Width - PanelShift->Width;
	 Image1->Left = PanelShift->Left + PanelShift->Width+3;
   }
   if(Width - ScrollBoxSubstrate->Width - ScrollBoxDefects->Width - StringGrid1->Width < 0 )
   {
	 ScrollBoxSubstrate->Width = Width  - ScrollBoxDefects->Width - StringGrid1->Width;
	 ScrollBoxDefects->Left = ScrollBoxSubstrate->Left+ScrollBoxSubstrate->Width+3;
	 StringGrid1->Left = ScrollBoxDefects->Left + ScrollBoxDefects->Width+3;
   }
   Application->Title =L"Booth Defect Registrator - Rejestrator Defektów Na Staowisku Wizualnej Kontroli";

}

void FillUp();

void TFormMain::FillingUP()
{
  //ComboSuperVisorsFillUP();
  //ProductsFillUp();
  FillUp();
}


//---------------------------------------------------------------------------
bool TFormMain::NextShift()
{
	TDateTime CurrentDate =DateTimePickerMain->Date;
	bool ShiftExists=false;
	switch (ComboBoxShiftKind->ItemIndex) {
	case 0:  ComboBoxShiftKind->ItemIndex = 1;
			  break;
	case 2:  ComboBoxShiftKind->ItemIndex = 0;
			 break;
	case 1: {
				CurrentDate = CurrentDate + 1;
				DateTimePickerMain->Date = CurrentDate;
				ComboBoxShiftKind->ItemIndex = 2;
				break;
			}
	default:
		;
	}
	ShiftExists =   Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,0,ComboBoxShiftKind->ItemIndex+1);
	if (ShiftExists) {
	   Current.ReadEntity();
	   ComboOperatorFillUP("Deputy", true,ComboBoxSuperVisorsGIP,SupervisorsGIP,true);
	   Current.WriteToForm();
	   //PageControl1->Enabled=true;
	   Caption = "   "+Application->Title+"  "+Current.GetDescription();
	   ReadDataFromDatabase(Current.GetID_GIP());
	   // ImagePreviousShift->Enabled=true;
	   ComboBoxShiftKindChange(this);
		PanelShift->Color = clCream;   //clBtnFace;
		 PanelShift->Refresh();
   }
   else
   {
	   ShowMessage("Nie znaleziono danych zmiany o podanych parametrach (dzień, rodzaj zmiany!"
				   "\n\n"
				   "The shift of given parameters has not been found (day, shift kind)!");
	   // ImageNextShift->Enabled=false;
		PanelShift->Color = clBtnFace;
		Current.Reset();
		SummarizedEntities.clear();
		ClearSubstrates();
		FillUpDefectSheet();
		 PanelShift->Refresh();

   }

   return ShiftExists;
}


bool TFormMain::PrevShift()
{
	TDateTime CurrentDate =DateTimePickerMain->Date;
	bool ShiftExists=false;
	switch (ComboBoxShiftKind->ItemIndex) {
	case 0:  ComboBoxShiftKind->ItemIndex = 2;
			  break;
	case 1:  ComboBoxShiftKind->ItemIndex = 0;
			 break;
	case 2: {
				CurrentDate = CurrentDate - 1;
				DateTimePickerMain->Date = CurrentDate;
				ComboBoxShiftKind->ItemIndex = 1;
				break;
			}
	default:
		;
	}
	ShiftExists =   Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,0,ComboBoxShiftKind->ItemIndex+1);
	if (ShiftExists) {
	   Current.ReadEntity();
	   ComboOperatorFillUP("Deputy", true,ComboBoxSuperVisorsGIP,SupervisorsGIP,true);
	   Current.WriteToForm();
	   //PageControl1->Enabled=true;
	   Caption = "   "+Application->Title+"  "+Current.GetDescription();
	  ReadDataFromDatabase(Current.GetID_GIP());
	   // ImageNextShift->Enabled=true;
	   ComboBoxShiftKindChange(this);
		PanelShift->Color = clCream;   //clBtnFace;
		PanelShift->Refresh();
   }
   else
   {
	   ShowMessage("Nie znaleziono danych zmiany o podanych parametrach (dzień, rodzaj zmiany!"
				   "\n\n"
				   "The shift of given parameters has not been found (day, shift kind)!");
	   // ImagePreviousShift->Enabled=false;
		PanelShift->Color = clBtnFace;
		Current.Reset();
		SummarizedEntities.clear();
		ClearSubstrates();
		FillUpDefectSheet();
		PanelShift->Refresh();
   }

   return ShiftExists;
}
//---------------------------------------------------------------------------
void TFormMain::ShiftReviewer()
{

}
void TFormMain::DateTimeSet()
{
	if(TDateTime().CurrentTime()>TDateTime(22,0,0,0) )
	{

	   DateTimePickerMain->DateTime=TDateTime().CurrentDateTime()+1;
	   //DateTimePickerSeparator->Time=TDateTime(0,1,0,0);
	   DateTimePickerMain->Time=TDateTime(0,1,0,0);
	   ComboBoxShiftKind->ItemIndex=2;
	}
	else
	{
		DateTimePickerMain->DateTime=TDateTime().CurrentDateTime();
		//DateTimePickerSeparator->Time=TDateTime().CurrentTime();
		if (TDateTime().CurrentTime()<TDateTime(6,0,0,0) ) {
		   ComboBoxShiftKind->ItemIndex=2;
		}
		else if (TDateTime().CurrentTime()<TDateTime(14,0,0,0)) {
		   ComboBoxShiftKind->ItemIndex=0;
		} else {
			ComboBoxShiftKind->ItemIndex=1;
		}

	}
	DateTimePickerMainChange(this);
	ComboBoxShiftKindChange(0);
}

void __fastcall TFormMain::ComboBoxShiftKindChange(TObject *Sender)
{
		switch (ComboBoxShiftKind->ItemIndex) {
	case 1: if (DateTimePickerMain->Time < TDateTime(14,0,0,0) || DateTimePickerMain->Time > TDateTime(22,0,0,0) ) {
				DateTimePickerMain->Time = TDateTime(14,1,0,0);

			}
			break;
	case 2: if (DateTimePickerMain->Time > TDateTime(6,0,0,0) ) {
				DateTimePickerMain->Time =TDateTime(0,1,0,0) ;
			}
			break;
	case 0:
			if (DateTimePickerMain->Time < TDateTime(6,0,0,0) || DateTimePickerMain->Time > TDateTime(14,0,0,0) ) {
				DateTimePickerMain->Time = TDateTime(6,1,0,0);
			}
			break;
	default:
			;
		}
	ComboBoxStartTime->ItemIndex =  ComboBoxShiftKind->ItemIndex;
	ComboBoxEndTime->ItemIndex = ComboBoxShiftKind->ItemIndex;
	DateTimePickerMainChange(Sender);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxShiftLetterChange(TObject *Sender)
{
	ComboBoxSuperVisorsEnter(Sender);
	ComboBoxProcessOperatorEnter(Sender);
	ComboBoxBoothOperatorEnter(Sender);
	if (ComboBoxShiftLetter->Text != Current.GetShiftLetter()) {
		   FormMain->ComboBoxBoothOperator->Text="";
		   FormMain->ComboBoxProcessOperator->Text="";
		   FormMain->ComboBoxSuperVisors->Text="";
		   FormMain->ComboBoxSuperVisorsGIP->Text="";
	}
	else
	{
		Current.WriteToForm(true);
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxSuperVisorsGIPEnter(TObject *Sender)
{
if (CheckIfShitfNotChoosen() )  // True if we know shift (letter)
	{
//	   char ShLetter = ComboBoxSuperVisors->Text; //.c_str())[0];
	   AnsiString Napis = ComboBoxShiftLetter->Text;
	   if(CheckBoxDeputySupervisor->Checked && CheckBoxLeadman->Checked)
	   {
		   Napis="Deputy";
	   }
	   ComboOperatorFillUP(Napis, !CheckBoxLeadman->Checked,ComboBoxSuperVisorsGIP,SupervisorsGIP); //CheckBoxLeadman checked then all operators
//	   ComboBoxSuperVisorsChange(Sender);
	}

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxProcessOperatorEnter(TObject *Sender)
{
  	if (CheckIfShitfNotChoosen())  // True if we know shift (letter)
	{
	   AnsiString Napis = ComboBoxShiftLetter->Text;
	   if(CheckBoxDeputyProcess->Checked)
	   {
		   Napis="Deputy";
	   }
	   ComboOperatorFillUP(Napis, false,ComboBoxProcessOperator,ProcessOperators); //CheckBoxLeadman checked then all operators
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxBoothOperatorEnter(TObject *Sender)
{
	 if (CheckIfShitfNotChoosen())  // True if we know shift (letter)
	{
	   AnsiString Napis = ComboBoxShiftLetter->Text;
	   if(CheckBoxDeputyBooth->Checked)
	   {
		   Napis="Deputy";
	   }
	   ComboOperatorFillUP(Napis, false,ComboBoxBoothOperator,BoothOperators ); //CheckBoxLeadman checked then all operators
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DateTimePickerMainChange(TObject *Sender)
{
		if(ComboBoxShiftKind->ItemIndex==2) // NightShift
	{
		  DateTimePickerStartTime->Date = DateTimePickerMain->Date  -1;
	}
	else
	{
			DateTimePickerStartTime->Date = DateTimePickerMain->Date;
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ComboBoxSuperVisorsEnter(TObject *Sender)
{
	if (CheckIfShitfNotChoosen() )  // True if we know shift (letter)
	{
//	   char ShLetter = ComboBoxSuperVisors->Text; //.c_str())[0];
	   AnsiString Napis = ComboBoxShiftLetter->Text;
	   if(CheckBoxDeputySupervisor->Checked && CheckBoxLeadman->Checked)
	   {
		   Napis="Deputy";
	   }
	   ComboOperatorFillUP(Napis, !CheckBoxLeadman->Checked,ComboBoxSuperVisorsGIP,SupervisorsGIP); //CheckBoxLeadman checked then all operators
//	   ComboBoxSuperVisorsChange(Sender);
	}
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	StartingUp();



	DataModule2->ADOConnectionCreport->Connected=true;
	FillingUP();

	DataModule2->ADOConnectionCreport->Connected=false;

	DateTimeSet();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonShiftReadClick(TObject *Sender)
{
   bool  ExistingEntity;

   if (ComboBoxShiftKind->Text.IsEmpty()) {
		   ExistingEntity = Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,ComboBoxShiftLetter->ItemIndex+1,0);
   }else if (ComboBoxShiftLetter->Text.IsEmpty()){
			 ExistingEntity=Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,0,ComboBoxShiftKind->ItemIndex+1);
   }
   else
   {
			 ExistingEntity = Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,ComboBoxShiftLetter->ItemIndex+1,ComboBoxShiftKind->ItemIndex+1);
   }

   if(ExistingEntity)
	 {

		 Current.ReadEntity();
		 ComboOperatorFillUP("Deputy", true,ComboBoxSuperVisorsGIP,SupervisorsGIP,true);
		 ComboOperatorFillUP("Deputy", true,ComboBoxBoothOperator,BoothOperators,true);
		 ComboOperatorFillUP("Deputy", true,ComboBoxProcessOperator,ProcessOperators,true);
		 Current.WriteToForm();
		 //PageControl1->Enabled=true;
		 Caption = "   "+Application->Title+"  "+Current.GetDescription();
		 PanelShift->Color = clCream;   //clBtnFace;
		 PanelShift->Refresh();
		 ReadDataFromDatabase(Current.GetID_GIP());
	 }
	 else
	 {
		 ShowMessage("Nie znaleziono danych zmiany o podanych parametrach (dzień, rodzaj zmiany!"
					 "\n\n"
					 "The shift of given parameters has not been found (day, shift kind)!");
					 PanelShift->Color = clBtnFace;
					 Current.Reset();
					 SummarizedEntities.clear();
					 ClearSubstrates();
					 FillUpDefectSheet();

	 }
		 return;
}
//---------------------------------------------------------------------------


void SetLamiCombos();
void SetFloatCombos();

void __fastcall TFormMain::ComboBoxSubstrateChange(TObject *Sender)
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
UnicodeString AddTerm(UnicodeString Term)
{
  if(Term.IsEmpty())
   return Term;
  else
   return Term+", ";
}

void __fastcall TFormMain::ButtonAddSubstrateClick(TObject *Sender)
{
	UnicodeString Substrate="";
	Substrate += AddTerm(ComboBoxProduct->Text);
	Substrate += AddTerm(ComboBoxSubstrate->Text);
	Substrate += AddTerm(ComboBoxSubstrateClass->Text);
	Substrate += AddTerm(ComboBoxSize->Text);
	Substrate += AddTerm(ComboBoxThickness->Text);
	Substrate = Substrate.Trim();
	if(*Substrate.LastChar() ==',')
		Substrate=Substrate.SetLength(Substrate.Length()-1);

	TDefectExpanded Temp;
	int IDSubClass;
	UnicodeString SubClass;
	int IDThickness;
	UnicodeString Thickness;
	if(ComboBoxSubstrate->Text.Pos("Lami")!=0)
	{
	   IDSubClass = Combos.SubstrateClassLamiCombo.GetDBId(ComboBoxSubstrateClass->ItemIndex);
	   SubClass = Combos.SubstrateClassLamiCombo.GetNameByItemIndex(ComboBoxSubstrateClass->ItemIndex);
	   IDThickness = Combos.ThicknessLamiCombo.GetDBId(ComboBoxThickness->ItemIndex);
	   Thickness =  Combos.ThicknessLamiCombo.GetNameByItemIndex(ComboBoxThickness->ItemIndex);
	}
	else
	{
	   IDSubClass = Combos.SubstrateClassFloatCombo.GetDBId(ComboBoxSubstrateClass->ItemIndex);
	   SubClass = Combos.SubstrateClassFloatCombo.GetNameByItemIndex(ComboBoxSubstrateClass->ItemIndex);
	   IDThickness = Combos.ThicknessFloatCombo.GetDBId(ComboBoxThickness->ItemIndex);
	   Thickness =  Combos.ThicknessFloatCombo.GetNameByItemIndex(ComboBoxThickness->ItemIndex);

	}
	TSubstrate Sub;
	TCoating Coat;
	Sub.Add(Combos.SubstrateCombo.GetDBId(ComboBoxSubstrate->ItemIndex) ,Combos.SubstrateCombo.GetNameByItemIndex(ComboBoxSubstrate->ItemIndex),
							IDSubClass, SubClass,
							IDThickness, Thickness,
							Combos.SizeCombo.GetNameByItemIndex(ComboBoxSize->ItemIndex), Combos.SizeCombo.GetDBId(ComboBoxSize->ItemIndex),
							Combos.SizeCombo.GetWidthLength(Combos.SizeCombo.GetDBId(ComboBoxSize->ItemIndex)).first, Combos.SizeCombo.GetWidthLength(Combos.SizeCombo.GetDBId(ComboBoxSize->ItemIndex)).second );

	Coat.SetID(Combos.CoatingsCombo.GetDBId(ComboBoxProduct->ItemIndex));
	Coat.SetShortName(ComboBoxProduct->Text);
	Temp.AddCoating(Coat);
	Temp.AddSubstrate(Sub);
	//bool IsIdentical = false;    '
	int Dimmension = ChoosenSubCoats.size();
	for(unsigned int i=0; i<ChoosenSubCoats.size();i++ ) // if one of the choosen substrates is equal to one added eariel then return
	{
		if(Temp == ChoosenSubCoats[i]) return;
		Dimmension = ChoosenSubCoats[i].GetCoating().GetID();
		if( Dimmension == 0 )
			   break;
	}
	CurrentSubCoat = Temp;



	RadioGroupSubstrate->Height= RadioGroupSubstrate->Height - 1.5*RadioGroupSubstrate->Font->Height;
	int Index=RadioGroupSubstrate->Items->Add(Substrate);
	ChoosenSubCoats[Index]=Temp;
	//ChoosenSubCoats.push_back(Temp);
	RadioGroupSubstrate->ItemIndex = Index;

	//extern TComboBoxItems Defects;
	//ComboBoxProcessOperator->Text= Defects.GetNameByDBId(EditNoOf->Text.ToInt());
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonAddDefectClicked(TObject *Sender)
{
   TButton* Buttino = static_cast<TButton*> (Sender);
   void AddDefect(int);
   AddDefect(Buttino->Tag);

}
void __fastcall TFormMain::ImagePreviousShiftClick(TObject *Sender)
{
    PrevShift();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ImageNextShiftClick(TObject *Sender)
{
	NextShift();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioGroupSubstrateClick(TObject *Sender)
{
	CurrentSubCoat=ChoosenSubCoats[RadioGroupSubstrate->ItemIndex];
	ComboBoxSubstrate->Text = CurrentSubCoat.GetSubstrate().GetSubName();
	ComboBoxProduct->Text = CurrentSubCoat.GetCoating().GetShortName();
	ComboBoxSubstrateClass->Text =  CurrentSubCoat.GetSubstrate().GetSubClassName();
	ComboBoxThickness->Text = CurrentSubCoat.GetSubstrate().GetThicknessName();
	ComboBoxSize->Text = CurrentSubCoat.GetSubstrate().GetSizeName();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGrid1DblClick(TObject *Sender)
{
	if (StringGrid1->Cells[0][StringGrid1->Row].IsEmpty()) {
           return;
	}
	FormAdditionalDetails = new TFormAdditionalDetails( this );
	int Index = StringGrid1->Row-1;  // Selected row on the StringGrid
	if(FormAdditionalDetails->Width > Screen->WorkAreaWidth || FormAdditionalDetails->Height > Screen->WorkAreaHeight)
	{
	  FormAdditionalDetails->Width = Screen->WorkAreaWidth-6;
	  FormAdditionalDetails->Height = Screen->WorkAreaHeight-6;
	  FormAdditionalDetails->Left = Screen->WorkAreaLeft+3;
	  FormAdditionalDetails->Top = Screen->WorkAreaTop+3;
	}
	try {
	  DefectSheetHeader(FormAdditionalDetails->StringGridAdditional);

	  FormAdditionalDetails->StringGridAdditional->Cells[1][1]= SummarizedEntities[Index].GetSummaryArea();
	  //FormAdditionalDetails->LabelAnnouncement->Caption=SummarizedEntities[Index].SummarizeString();
	  Combos.CoatingsCombo.Assign(FormAdditionalDetails->ComboBoxProduct);

	  Combos.SubstrateCombo.Assign(FormAdditionalDetails->ComboBoxSubstrate);
	  Combos.SizeCombo.Assign(FormAdditionalDetails->ComboBoxSize);
	  Combos.DefectsCombo.Assign(FormAdditionalDetails->ComboBoxDefect);
	  switch(SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetID()) //100  is ID of Lami
	  {
		case 100:
					Combos.SubstrateClassLamiCombo.Assign(FormAdditionalDetails->ComboBoxSubstrateClass);
					Combos.ThicknessLamiCombo.Assign(FormAdditionalDetails->ComboBoxThickness);
					break;
		default :
					Combos.SubstrateClassFloatCombo.Assign(FormAdditionalDetails->ComboBoxSubstrateClass);
					Combos.ThicknessFloatCombo.Assign(FormAdditionalDetails->ComboBoxThickness);
					break;
	  }
	  switch(SummarizedEntities[Index].GetDefExpanded().GetDefect().GetKindNumber())
	  {
	   case 16:												//16 is Float defect ID
				if(SummarizedEntities[Index].GetDefExpanded().GetSubstrate().GetID()==100) //if substrate was Lami
				{
				  Combos.AdditionalDefectsLami.Assign(FormAdditionalDetails->ComboBoxDefectDetails);
				}
				else
				{
				  Combos.AdditionalDefectsFloat.Assign(FormAdditionalDetails->ComboBoxDefectDetails);
				}
				break;
	   default:
				FormAdditionalDetails->ComboBoxDefectDetails->Visible=false;
				FormAdditionalDetails->LabelDefectAdditional->Visible=false;
				break;
	  }
	  int Quantity = SummarizedEntities[Index].NumberOfElements();
	  FormAdditionalDetails->StringGridAdditional->RowCount=Quantity+1;
	  for(int i=0;i<Quantity;i++)
	  {
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
		bool CanSelect=true;;

		if (!MemoComment->Lines->Text.IsEmpty()) {
		   FormAdditionalDetails->IsGlobal = true;
		   FormAdditionalDetails->GlobalComment = MemoComment->Lines->Text.Trim();
		}
		//FormAdditionalDetails->MemoComments->Lines->AddStrings(MemoComment->Lines);
		FormAdditionalDetails->StringGridAdditionalSelectCell(Sender,1,1,CanSelect);  //Set Combos
		FormAdditionalDetails->ShowModal();
	} __finally {
        int ShiftID = Current.GetID();
		if(ShiftID!=0)
		{
			UnicodeString ID_GIP = Current.GetID_GIP();
			ReadDefectFromDBToSheet(ID_GIP);
		}
		else
		{
		   FillUpDefectSheet();
        }
		FormAdditionalDetails->Free();
	}

}
//---------------------------------------------------------------------------





void __fastcall TFormMain::ButtonAddCommentClick(TObject *Sender)
{
  if (StringGrid1->Cells[0][StringGrid1->Row].IsEmpty()) {
		   return;
	}
  int EntityID;
  int NoOfDefects =  SummarizedEntities[FormMain->StringGrid1->Row-1].GetNoOfDefects();//  GetDefExpanded().GetDefect().GetNumberOfDefects();
  if (NoOfDefects>1) {
	 StringGrid1DblClick(Sender);
	 return;
  }
  else
  {
	 //SummarizedEntities[FormMain->StringGrid1->Row-1].GetDefExpanded().AddComment(MemoComment->Lines->Text);
	 EntityID = SummarizedEntities[FormMain->StringGrid1->Row-1].GetDefExpanded().GetDefect().GetEntityID();
	 UpdateComment(EntityID,MemoComment->Lines->Text);
  }
  ReadDefectFromDBToSheet(Current.GetID_GIP());
  //FillUpDefectSheet();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGrid1SelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect)
{
    if (StringGrid1->Cells[0][ARow].IsEmpty()) {
		   return;
	}
	MemoComment->Lines->Text =  SummarizedEntities[ARow-1].GetDefExpanded().GetComment();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonQuitClick(TObject *Sender)
{
	if(Application->MessageBox(L"Czy jesteś pewien, że chciesz wyjść z programu?\n\nAre you sure you want to quit programm?",L"Uwaga! - Attention!",MB_YESNO)==IDYES)
	{
		Application->Terminate();
	}
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxEndTimeChange(TObject *Sender)
{
   switch (ComboBoxEndTime->ItemIndex) {
	case 1:
			ComboBoxShiftKind->ItemIndex= 1;
//			if (DateTimePickerMain->Time < TDateTime(14,0,0,0) || DateTimePickerMain->Time > TDateTime(22,0,0,0) ) {
//				DateTimePickerMain->Time = TDateTime(14,1,0,0);
//
//			}
			break;
	case 2:
			ComboBoxShiftKind->ItemIndex= 2;
//			if (DateTimePickerMain->Time > TDateTime(6,0,0,0) ) {
//				DateTimePickerMain->Time =TDateTime(0,1,0,0) ;
//			}
			break;
	case 0:
	        ComboBoxShiftKind->ItemIndex= 0;
//			if (DateTimePickerMain->Time < TDateTime(6,0,0,0) || DateTimePickerMain->Time > TDateTime(14,0,0,0) ) {
//				DateTimePickerMain->Time = TDateTime(6,1,0,0);
//			}
			break;
	default:
			;
		}
	ComboBoxShiftKindChange(Sender);
	//ComboBoxStartTime->ItemIndex =  ComboBoxShiftKind->ItemIndex;
	//ComboBoxEndTime->ItemIndex = ComboBoxShiftKind->ItemIndex;
	//DateTimePickerMainChange(Sender);

}
//---------------------------------------------------------------------------

