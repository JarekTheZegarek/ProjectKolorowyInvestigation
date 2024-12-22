//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitMain.h"
#include "DataCoater.h"
#include "AccessCOM.h"
#include "UnitMainEntity.h"
#include "UnitStartUp.h"
#include "Global.h"
#include "UnitOEEForm.h"
#include "UnitOption.h"
#include "system.ioutils.hpp"
//---------------------------------------------------------------------------
#pragma package(smart_init)
//#pragma link "Excel_XP_srvr"
//#pragma link "frxClass"
//#pragma link "frxClass"
//#pragma link "frxClass"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma resource "*.dfm"

TFormMain *FormMain;

//---------------------------------------------------------------------------
float Fact;
void MF(int& S)
{
	S=  int(S*Fact) ;
}
__fastcall TFormMain::TFormMain(TComponent* Owner)
	: TForm(Owner)
{
   float OldWidth = Width;
   float ControlWidth;
   Width = Screen->WorkAreaWidth;
   Height = Screen->WorkAreaHeight;
   Left = Screen->WorkAreaLeft;
   Top = Screen->WorkAreaTop;
   Panel1->Width = ClientWidth - Panel1->Left*2;
   Panel1->Height = ClientHeight - Panel1->Top*2;

   float Factor = Width/OldWidth;//Panel1->Witdh/OldPanelWidth;
   Fact = Factor;
   PageControl1->Width= Panel1->Width-32;
   ControlWidth = Panel1->Width;
   if(Width-(NextShiftArrow->Left + NextShiftArrow->Width)-Image1->Width <0 )
   {
	 Image1->Width = ControlWidth - (NextShiftArrow->Left + NextShiftArrow->Width)-3;
	 Image1->Left = (NextShiftArrow->Left + NextShiftArrow->Width)+3;
   }


   Application->Title =L"Coater E-Logbook  czyli KOLOROWY ŚWIAT COATER'A";
   ComboBoxReportPredefMode->ItemIndex=3;
   ComboBoxReportPredefModeChange(0);

}
//---------------------------------------------------------------------------

void FillUp();

void TFormMain::FillingUP()
{
  //ComboSuperVisorsFillUP();
  //ProductsFillUp();
  FillUp();
}

void AccessConnect();
void __fastcall TFormMain::ButtonPrintClick(TObject *Sender)
{
	//AccessConnect();
	if(Current.GetSumOfMinutes()!=480)
	{
		ShowMessage("Liczba minut w raporcie \"Stan Linii\" jest różna od 480 \nProszę poprawić tę część raportu" );
		return;
	}
	FormOEE=new TFormOEE(this,true);
	FormOEE->ShowModal();
	delete FormOEE;
	this->ButtonFastReportClick(Sender);
   //	AccessPrintCurrent();
	//ComboSuperVisorsFillUP();
}
//---------------------------------------------------------------------------

void TFormMain::DateTimeSet()
{
	if(TDateTime().CurrentTime()>TDateTime(22,0,0,0) )
	{

	   DateTimePickerMain->DateTime=TDateTime().CurrentDateTime()+1;
	   DateTimePickerSeparator->Time=TDateTime(0,1,0,0);
	   DateTimePickerMain->Time=TDateTime(0,1,0,0);
	   ComboBoxShiftKind->ItemIndex=2;
	}
	else
	{
		DateTimePickerMain->DateTime=TDateTime().CurrentDateTime();
		DateTimePickerSeparator->Time=TDateTime().CurrentTime();
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
void __fastcall TFormMain::FormCreate(TObject *Sender)
{
	StartingUp();

	if(!Report)
	{
	   TabSheetReport->TabVisible=false;
	}


	DataModule1->ADOConnectionExchange->Connected=true;
	DataModule1->ADOConnectionCreport->Connected=true;
	FillingUP();
	DataModule1->ADOConnectionExchange->Connected=false;
	DataModule1->ADOConnectionCreport->Connected=false;

	DateTimeSet();

	StringGridProducts->ColCount = 7;
	StringGridProducts->ColWidths[0]=20;
	int Temp,AllWidth=StringGridProducts->ColWidths[0] ;
	Temp = ComboBoxProducts->Width ;
	AllWidth+=Temp;
	StringGridProducts->ColWidths[1]=Temp;
	Temp =  EditSpeed->Width;
	AllWidth+=Temp;
	StringGridProducts->ColWidths[2]=Temp;
	Temp =  EditIn->Width;
	AllWidth+=Temp;
	StringGridProducts->ColWidths[3]=Temp;
	Temp = EditOut->Width ;
	AllWidth+=Temp;
	StringGridProducts->ColWidths[4]=Temp;
	Temp = EditYield->Width ;
	AllWidth+=Temp;
	StringGridProducts->ColWidths[5]=Temp;
	Temp = 20 ;
	AllWidth+=Temp;

	if((Temp=(StringGridProducts->ClientWidth-AllWidth)) < 100 )
			StringGridProducts->ColWidths[6]=100;
	else
			StringGridProducts->ColWidths[6]=Temp;



	StringGridLineStatus->ColCount = 7;
	AllWidth=0 ;
	Temp = DateTimePickerLineStatusStart->Width ;
	AllWidth+=Temp;
	StringGridLineStatus->ColWidths[0]=Temp;
	StringGridLineStatus->Cells[0][0]="Start";
	Temp =  DateTimePickerLineStatusStop->Width;
	AllWidth+=Temp;
	StringGridLineStatus->ColWidths[1]=Temp;
	StringGridLineStatus->Cells[1][0]="Stop";
	Temp =  EditMinuteLineStatus->Width;
	AllWidth+=Temp;
	StringGridLineStatus->ColWidths[2]=Temp;
	StringGridLineStatus->Cells[2][0]="Min.";

	Temp =  ComboBoxLineStatus->Width/2;
	AllWidth+=Temp;
	StringGridLineStatus->ColWidths[3]=Temp;
	StringGridLineStatus->Cells[3][0]="Line status";

	Temp =  ComboBoxProdFrom->Width;
	AllWidth+=Temp;
	StringGridLineStatus->ColWidths[4]=Temp;
	StringGridLineStatus->Cells[4][0]="Prod.";

	Temp =  ComboBoxProdTo->Width;
	AllWidth+=Temp;
	StringGridLineStatus->ColWidths[5]=Temp;
	StringGridLineStatus->Cells[5][0]="Addit.";

	if((Temp=(StringGridLineStatus->ClientWidth-AllWidth)) < 100 )
			StringGridLineStatus->ColWidths[6]=100;
	else
			StringGridLineStatus->ColWidths[6]=Temp;
    StringGridLineStatus->Cells[6][0]="Opis/Description";

	TabSheetLineStatusEnter(Sender);
   if (Report) {
	  PageControl1->Enabled=true;
   }
   else
   {
	PageControl1->Enabled=false;
   }
}
//---------------------------------------------------------------------------

bool DoDelete()     // true if you want to delete
{
	WideString Msg = "Ta operacja powoduje N I E O D W O £ A L N E skasowanie wprowadzonych danych";
	Msg+="\n\n";
	Msg+="Na pewno chcesz kontynuowaæ? \n\n" ;
	Msg+="This operation removes I R R E V E R S I B L Y data. \n\n";
	Msg+="Are you sure you want to proccede? " ;
	return Application->MessageBoxA(Msg.c_bstr(),L"Uwaga! Attention!",MB_YESNO)==IDYES ;

}

void __fastcall TFormMain::Button1Click(TObject *Sender)
{

	static bool read = true;
	read=read?false:true;
	DataModule1->ADOConnectionCreport->Connected=true;
	TADOTable* Tabela=  DataModule1->ADOTable2;
	Tabela->TableName="PRODTYPE";
	if(read)
	{
		Tabela->Open();

//    	TLocateOptions Opts;
//    	Opts.Clear();
//		Opts << loPartialKey;

        if(Tabela->Locate("ID_PRODTYPE",2,TLocateOptions()<<loCaseInsensitive))
        {
                Tabela->Edit();
            //    Tabela->FieldValues["Description"]=Memo1->Text;
        }
        Tabela->Post();
	}
    else
    {
        Tabela->Open();
        if(Tabela->Locate("ID_PRODTYPE",2,TLocateOptions()<<loCaseInsensitive))
        {
           //	Memo1->Clear();
           //	Memo1->Text =  Tabela->FieldValues["Description"];
        }
    }

    Tabela->Close();
    DataModule1->ADOConnectionCreport->Connected=false;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxSuperVisorsChange(TObject *Sender)
{
	ComboBoxSuperVisors->Text= SupervisorsCompare.GetCreport(ComboBoxSuperVisorsGIP->Text);

	AnsiString Nazwa = ComboBoxSuperVisors->Text;// ComboBoxSuperVisors->Items->Strings[ComboBoxSuperVisors->ItemIndex]; //ComboBoxSuperVisors->Text;
#ifdef test
	MemoTest->Lines->Add(Nazwa);
#endif
	int ID = Supervisors[Nazwa];
//    for(int i=0;i<FormMain->ComboBoxSuperVisors->Items->Count;i++)
//    {
//       ID = Supervisors[FormMain->ComboBoxSuperVisors->Items->Strings[i]];
//    }
	Edit5->Text= ID;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::KoniecExit1Click(TObject *Sender)
{
	Application->Terminate();
}
//---------------------------------------------------------------------------


bool TFormMain::CheckFillingOfMainEntity() throw(CheckError)
{
  try{
     if(ComboBoxShiftLetter->Text.IsEmpty())
     {
         throw CheckError(CheckError::ShiftLetter);    
     }
     else if (ComboBoxShiftKind->Text.IsEmpty()) {
         throw CheckError(CheckError::ShiftType);
     }
	 else if (ComboBoxSuperVisorsGIP->Text.IsEmpty()) {
         throw CheckError(CheckError::ShiftSupervisor);
     }

  return true;
  }
  catch(CheckError& Blad)
  {
   WideString Buf = Blad.Message();
   Application->MessageBox(Buf.c_bstr(),L"Uwaga/Attention!",MB_OK);
   return false;

  }
}

AnsiString CheckError::Message()
{
    AnsiString ErrorMessage = "Nie wype³niono rubryki ";
	AnsiString ErrorMessageEng = "Unfilled space ";
	switch (TypeOfError) {
          ShiftLetter:
                    	ErrorMessage = ErrorMessage + "\"Zmiana\" \n\n";
                        ErrorMessage = ErrorMessage + ErrorMessageEng+ "\"Shift\"\n\n";
                    	break;
          ShiftKind:
                    	ErrorMessage = ErrorMessage + "\"Typ Zmiany\" \n\n";
                        ErrorMessage = ErrorMessage + ErrorMessageEng+ "\"Shift Kind\"\n\n";
                    	break;
          ShiftSupervisor:
                    	ErrorMessage = ErrorMessage + "\"Majster\" \n\n";
                        ErrorMessage = ErrorMessage + ErrorMessageEng+ "\"Supervisor\"\n\n";
                    	break;
          unknown:
    default:
          ErrorMessage = "Nieznany b³¹d \n \nUnknown Error";
        ;
    }
    return ErrorMessage;
}

CheckError::CheckError(TErrorType Type)
{
    TypeOfError = Type;
}


void __fastcall TFormMain::ComboBoxProductsEnter(TObject *Sender)
{
		CheckFillingOfMainEntity();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EditOutExit(TObject *Sender)
{
  double In;
  try{
  EditOut->Text = ChangeDecimalSeparator(EditOut->Text);
  if(!EditIn->Text.IsEmpty())
  {
	In = AnsiToDouble(EditIn->Text);//.ToDouble();
  }
  else
  {
	Application->MessageBox(L"Uzupe³nij rubrykê \"In\"\n\nFill in the space \"In\"",L"Attention! - Uwaga",MB_OK);
	return;
  }
  if(EditOut->Text.IsEmpty())
  {
	Application->MessageBox(L"Uzupe³nij rubrykê \"Out\"\n\nFill in the space \"Out\"",L"Attention! - Uwaga",MB_OK);
	return;
  }
  if(In==0) {
		Application->MessageBox(L"Dzielenie przez zero\n\nDivision by zero",L"Attention! - Uwaga",MB_OK);
    return;
  }
  SetTemporaryYield();
 //  Yield = Out*100/In;
//  EditYield->Text=AnsiString().FormatFloat("#.00",Yield)+"%";
  }
  catch(EConvertError& Blad){
	  Application->MessageBox(L"Prawdopodbnie z³y format liczby.\n\nWrong number format.",L"Attention! - Uwaga",MB_OK);
    return;
  }
  catch(...){
	 Application->MessageBox(L"(Funkcja:EditOutExit) Nieznany b³¹d.\n\nUnknown error.",L"Attention! - Uwaga",MB_OK);
    return;
  }

}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonAddMainClick(TObject *Sender)
{

try{
 DataModule1->ADOConnectionCreport->Connected=true;
 WideString TempMessage;
 static bool ExistingEntity = false;
 bool EmptyFields = true;
 static int LastReadID = -1;
 if(DateTimePickerMain->Date > TDateTime().CurrentDateTime()+1)
 {
	 ShowMessage("Nie można dodać przyszłej daty (ewentualnie proszę sprawdzić czas systemowy )"
				"\n\nIt is impossible to choose future date (alternatively check the system time)" );
	 return;
 }

 if(Current.CheckIfIsRead(DateTimePickerMain->DateTime,ComboBoxShiftKind->ItemIndex+1))
 {
	 if(Current.CheckIfHasChanged(ComboBoxShiftLetter->ItemIndex+1,ComboBoxSuperVisors->Text,EditNumberOfOperators->Text.ToInt()))
	 {

		TempMessage = "Dane zapisanej w bazie danych zmiany zosta³y zmodyfikowane. Czy chcesz je nadpisać?"
					  "\n\nAlready existing in Database shift data has been changed. Do you want to overwrite them?";
		if(Application->MessageBox(TempMessage.c_bstr(),L"Uwaga!  Attention!",MB_YESNO)==IDYES)
		{
			Current.WriteEntity();
			Current.WriteToForm();
			Caption = "   "+Application->Title+"  "+Current.GetDescription();
			return;
		}
		else
		{
			TempMessage = "Skoro nie będziesz nadpisywać danych, czy chcesz skorygować zapis zgodnie z ich stanem w bazie danych?"
					  "\n\nIf you do not want to overwrite existing data do you want to correct the data according to Database state?";
			if(Application->MessageBox(TempMessage.c_bstr(),L"Uwaga!  Attention!",MB_YESNO)==IDYES)
			{
           //     Current.
            	Current.WriteToForm();
                Caption = "   "+Application->Title+"  "+Current.GetDescription();
                return;
            }
            else
            {
               Caption = "   "+Application->Title+"  "+Current.GetDescription();
               return;
            }
        }
     }
     else
       Caption = "   "+Application->Title+"  "+Current.GetDescription();
       return;
 }
 //ItemIndexes are one less than the IDs in relation WorkshiftDays
 if(ComboBoxShiftLetter->Text.IsEmpty()  && ComboBoxShiftKind->Text.IsEmpty())
 {
	 ShowMessage("Wypełnij co najmniej jedną z rubryk - \"Shift\" lub \"Shift Kind\""
                 "\n\nFill in at least one of the spaces \"Shift\" or \"Shift Kind\", please.");
    Caption = "   "+Application->Title+"  "+Current.GetDescription();
 	 return;
 }else if (ComboBoxShiftKind->Text.IsEmpty()) {
           ExistingEntity = Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,ComboBoxShiftLetter->ItemIndex+1,0);
 }else if (ComboBoxShiftLetter->Text.IsEmpty()){
           ExistingEntity=Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,0,ComboBoxShiftKind->ItemIndex+1);
 }
 else
 {
 		   ExistingEntity = Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,ComboBoxShiftLetter->ItemIndex+1,ComboBoxShiftKind->ItemIndex+1);
           if(!ComboBoxSuperVisors->Text.IsEmpty())
           {
               EmptyFields = false;
           }
 }

 //------------------- we have now checked if it is new entity or existing --------------------


 if (ExistingEntity && LastReadID!= Current.GetID() ) // Entity in database but not under proccessing
 {
	WideString Message="Zmiana o podanych parametrach (Data,Czas) zapisywała już dane. Chcesz wczytać zapisane w bazie dane?";
	Message+="\n\nThere are data introduced by the given shift. Would you like to correct them?";
	if(Application->MessageBoxA(Message.c_bstr(),L"UWAGA!!! - ATTENTION!!!",MB_YESNO)==IDYES)
	{
       Current.ReadEntity();
       Current.WriteToForm();
       LastReadID = Current.GetID();
       PageControl1->Enabled=true;
       Caption = "   "+Application->Title+"  "+Current.GetDescription();
 	}
    else
    {
        return;
    }
 }
 else if(!EmptyFields)    // if data not in database or already processessed data (in this session) then write to database or update
 {
     Current.WriteEntity();
     Current.WriteToForm();
     LastReadID = Current.GetID();
     Caption = "   "+Application->Title+"  "+Current.GetDescription();
 }
 else{       // Still having empty fields and non existing in database;
	 ShowMessage("Proszê uzupełnić puste rubryki i zapisać ponownie!\n\nPlease, update empty spaces and write again!");
	 Caption = "   "+Application->Title+"  "+Current.GetDescription();
	 return;
 }
 ReadAllDataFromDatabase(Current.GetID());
 Current.WriteToForm();
 Caption = "   "+Application->Title+"  "+Current.GetDescription();
 //FillingUpProductsSheet(Current.GetID());
 TabSheetLineStatusEnter(Sender);

}
 __finally{
       DataModule1->ADOConnectionCreport->Connected=false;
       Caption = "   "+Application->Title+"  "+Current.GetDescription();	
  }
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::FormResize(TObject *Sender)
{
//   float OldPanelWidth = Panel1->Width;
//   float OldPageControlWidth = PageControl1->Width;
//   float OldWidth = Width;
   //Width = Screen->WorkAreaWidth;
   //Height = Screen->WorkAreaHeight;
   //Left = Screen->WorkAreaLeft;
   //Top = Screen->WorkAreaTop;
//   float Factor = Width/OldWidth;//Panel1->Witdh/OldPanelWidth;
//   Fact = Factor;
//   DateTimePickerMain->Width=Factor*DateTimePickerMain->Width;
//   DateTimePickerMain->Left = Factor*DateTimePickerMain->Left;
//   ComboBoxShiftKind->Width=int(ComboBoxShiftKind->Width*Factor);
//   ComboBoxShiftKind->Left = Factor*ComboBoxShiftKind->Left;
//   ComboBoxShiftLetter->Width=int(ComboBoxShiftLetter->Width*Factor);
//   ComboBoxShiftLetter->Left=ComboBoxShiftLetter->Left*Factor;
//   ComboBoxSuperVisors->Width=int(ComboBoxSuperVisors->Width*Factor);
//   ComboBoxSuperVisors->Left=ComboBoxSuperVisors->Left*Factor;
//   EditNumberOfOperators->Width=EditNumberOfOperators->Width*Factor;
//   EditNumberOfOperators->Left=EditNumberOfOperators->Left*Factor;
//   UpDownNumberOfOperators->Left=UpDownNumberOfOperators->Left*Factor;
//   LabelDate->Left=LabelDate->Left*Factor;
//   LabelOperators->Left=LabelOperators->Left*Factor;
//   LabelShiftKind->Left=LabelShiftKind->Left*Factor;
//   LabelShiftLetter->Left=LabelShiftLetter->Left*Factor;
//   LabelSuperVisors->Left=LabelSuperVisors->Left*Factor;
//   ButtonAddMain->Left=ButtonAddMain->Left*Factor;
//   ButtonPrint->Left=ButtonPrint->Left*Factor;
   PageControl1->Width= Panel1->Width-32;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAddProductionClick(TObject *Sender)
{
if( ComboBoxProducts->Text.IsEmpty())
			 {
				 ShowMessage("Wybierz produkt\n\nChoose product");
				 return;
			 }

if (!CheckBoxNewProduction->Checked) {
	try{
		 FormPacks = new TFormPacks(this);
		 FormPacks->StartingCoatingID=ComboIndexToProduct[FormMain->ComboBoxProducts->ItemIndex].GetID();
		 FormPacks->ShowModal();
		}
	__finally {
		FormPacks->Free();
	 }

}
else
{
	//FillingUpProductsSheet()
	try{
//		 FormPacks = new TFormPacks(this);
//		 FormPacks->ShowModal();
		 try {
			 LabelSavingWarrning->Visible=true;
			 Refresh();
			 if( ComboBoxProducts->Text.IsEmpty())
			 {
				 ShowMessage("Wybierz produkt\n\nChoose product");
				 return;
			 }
			 if( EditSpeed->Text.IsEmpty()|| EditIn->Text.IsEmpty()||EditOut->Text.IsEmpty())
			 {
				 ShowMessage("Wype³nij dane\n\nFill up data");
				 return;
			 }
			 if(EditIn->Text.ToDouble()<EditOut->Text.ToDouble())
			 {
				 ShowMessage("Czy¿by coater wyprodukowa³ szk³o?\n\n"
							 "Iloœæ szk³a na wejœciu jest wiêksza od tej na wyjœciu."
							 "\nProszê o skorygowanie wpisywanych wartoœci!");
				 return;
			 }
			 if(EditSpeed->Text.ToDouble()<=0 || EditSpeed->Text.ToDouble()>20)
			 {
				 ShowMessage("Rozs¹dne prêdkoœci coatera le¿¹ pomiedzy 0 a 20 m/min");
				 return;
			 }
			 AddProduction();
		 }
		 catch(EConvertError& E)
		 {
			AnsiString Message = "Zapewne, Ÿle wpisano liczbê. "
								 "\nW pola edycyjne proszê wpisywaæ tylko liczby.";
			Message+="\n\n";
			Message+=E.Message;
			ShowMessage(Message);
		 }
	 }
	 __finally {

//		FormPacks->Free();
		   LabelSavingWarrning->Visible=false;
	 }

}
}
//---------------------------------------------------------------------------

enum TTabs { Production = 0, LineStatus=1, Defects=2, Epuro=12,  \
			 Cathodes=3, Measurements = 6, Separator = 7,    \
	   Alarms =8 ,	LineStacker = 9, CTControl = 10,  Washer=11, GeneralRemarks = 4,Reports=5};


void __fastcall TFormMain::PageControl1DrawTab(TCustomTabControl *Control,
	  int TabIndex, const TRect &Rect, bool Active)
{
	  AnsiString AText;
	  TPoint APoint;
	  TColor MaintenanceCol = clRed;

	  AText = PageControl1->Pages[TabIndex]->Caption;
	  APoint.x=(Rect.Right-Rect.Left)/2 - Control->Canvas->TextWidth(AText)/2;
	  APoint.y= (Rect.Bottom-Rect.Top)/2 - Control->Canvas->TextHeight(AText)/2;
		  switch (TabIndex) {
			case Alarms :    //            PageControl1->Pages

                    	Control->Canvas->Brush->Color = ScrollBoxAlarms->Color;
                    	Control->Canvas->Font->Color=  MaintenanceCol;
                        break;
            case Production :
            			Control->Canvas->Brush->Color = ScrollBox1->Color;
                    	break;
            case LineStacker :
                        Control->Canvas->Brush->Color =ScrollBoxLineStacker->Color; //
                        Control->Canvas->Font->Color=  MaintenanceCol;
                    	break;
            case GeneralRemarks :
                        Control->Canvas->Brush->Color = ScrollBoxGeneralRemarks->Color;
                    	break;
            case CTControl      :
                    	Control->Canvas->Brush->Color =ScrollBoxCTControl->Color; //
                        Control->Canvas->Font->Color= MaintenanceCol;
                    	break;
            case Washer   :
                        Control->Canvas->Brush->Color =ScrollBoxWasher->Color; //
                        Control->Canvas->Font->Color= MaintenanceCol;
                    	break;
            case LineStatus   :
                        Control->Canvas->Brush->Color = ScrollBoxLineStatus->Color;
                    	break;
            case Defects   :
                        Control->Canvas->Brush->Color = ScrollBoxDefects->Color;
                    	break;
            case Epuro   :
                        Control->Canvas->Brush->Color = ScrollBoxEpuro->Color;
                    	break;
            case Cathodes   :
                        Control->Canvas->Brush->Color = ScrollBoxCathodes->Color;
                    	break;
            case Measurements   :
                        Control->Canvas->Brush->Color = ScrollBoxMeasurements->Color;
                    	break;
            case Separator   :
                        Control->Canvas->Brush->Color = ScrollBoxSeparator->Color;
                    	break;
      default:
          ;
      }
      Control->Canvas->FillRect(Rect);
     
//      if (Control->Canvas->TextHeight(AText) * 2 > PageControl1->TabHeight)
//            PageControl1->TabHeight = Control->Canvas->TextHeight(AText) * 2;
      Control->Canvas->TextRect(Rect,Rect.Left + APoint.x, Rect.Top + APoint.y, AText);
//      switch (TabIndex) {
//          	case Alarms :    //            PageControl1->Pages
////            			Control->Canvas->Brush->Color = ScrollBoxAlarms->Color;
////                    	Control->Canvas->Brush->Style =
////                    	Control->Canvas->Pen->Style= psClear;
////                        Control->Canvas->Rectangle(Rect.left+(Rect.right-Rect.left)/2,Rect.top,Rect.right,Rect.bottom);
//      //                  Control->Canvas->Brush->Color = MaintenanceCol;
//                        Control->Canvas->Font->Color=ScrollBoxAlarms->Color;
//						Control->Canvas->TextRect(Rect,Rect.Left + APoint.x, Rect.Top + APoint.y, AText);
//                        Control->Canvas->Font->Color=clBlack;
//      					break;
//      }
       Control->Canvas->Font->Color=clBlack;

      for (int i = 0; i < PageControl1->PageCount; i++)
      {
         AnsiString s = PageControl1->Pages[i]->Caption;
//         if (Canvas->TextWidth(s) * 2 > PageControl1->TabWidth)
//            PageControl1->TabWidth = Canvas->TextWidth(s) * 2;
          if (Control->Canvas->TextHeight(s) * 2 > PageControl1->TabHeight)
            PageControl1->TabHeight = Control->Canvas->TextHeight(s) * 2;
      }




      //      if(TabIndex==2)
//      {
//          Control->Canvas->Brush->Color = clBlue;
//          Control->Canvas->FillRect(Rect);
//          AText = TabSheet3->Caption;
//          APoint.x=(Rect.Right-Rect.Left)/2 - Control->Canvas->TextWidth(AText)/2;
//          APoint.y= (Rect.Right-Rect.Left)/2 - Control->Canvas->TextWidth(AText)/2;
//          Control->Canvas->TextRect(Rect,Rect.Left + APoint.x, Rect.Top + APoint.y, AText);
//          var
//  AText: string;
//  APoint: TPoint;
//begin
//  with (Control as TPageControl).Canvas do
//  begin
//    Brush.Color := ClGreen;
//    FillRect(Rect);
//    AText := TPageControl(Control).Pages[TabIndex].Caption;
//    with Control.Canvas do
//    begin
//      APoint.x := (Rect.Right - Rect.Left) div 2 - TextWidth(AText) div 2;
//      APoint.y := (Rect.Bottom - Rect.Top) div 2 - TextHeight(AText) div 2;
//      TextRect(Rect, Rect.Left + APoint.x, Rect.Top + APoint.y, AText);
//    end;
//  end;
//end;


   //   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridProductsSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
//  SelectedCell[0]=ACol;
//   SelectedCell[1]=ARow;
//   if(StringGrid1->Cells[ACol][ARow].IsEmpty())
//   	Selected = false;
//   else
//   	Selected = true;


/* TODO :
Improve deleting by designated choosen data in temporary table to allow updating
It not working when trying to change only product name */

if(StringGridProducts->Cells[1][ARow].IsEmpty())//If empty cell were choosen empty form
{
    EmptyProductionForm();
    return;
}
  int ProdId = ArrayOfProducts.GetID(ARow);
  TProduction Prod(Current.GetID());
  Prod.SetID_Product_Type(ProdId);
  Prod.FillUpForm();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EditInExit(TObject *Sender)
{
  if(EditIn->Text.IsEmpty())
	return;
  else
  {
	EditIn->Text = ChangeDecimalSeparator(EditIn->Text);
	//EditOutExit(Sender);
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DrukujPrint1Click(TObject *Sender)
{
	    ButtonPrintClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonRemoveProductionClick(TObject *Sender)
{
    RemoveSingleProductProduction();	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetAlarmsEnter(TObject *Sender)
{
	   DateTimePickerAlarms->DateTime=TDateTime().CurrentDateTime();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAddAlarmsClick(TObject *Sender)
{

   try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddAlarms();
   }
     __finally {
           LabelSavingWarrning->Visible=false;
     }

	

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridAlarmsSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
	   if(StringGridAlarms->Cells[1][ARow].IsEmpty())//If empty cell were choosen empty form
		{
    		EmptyAlarmForm();
    		return;
		}

  AlarmsFillUpForm(ARow);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonLineStackerAddClick(TObject *Sender)
{

     try {
         LabelSavingWarrning->Visible=true;
         Refresh();
          AddLineStacker();
     }
     __finally {
           LabelSavingWarrning->Visible=false;
     }


      
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridLineStackerSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
     if(StringGridLineStacker->Cells[1][ARow].IsEmpty())//If empty cell were choosen empty form
		{
    		EmptyLineStackerForm();
    		return;
		}
      LineStackerFillUpForm(ARow);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetLineStackerEnter(TObject *Sender)
{
  DateTimePickerLineStacker->DateTime=TDateTime().CurrentDateTime();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonRemoveLineStackerClick(TObject *Sender)
{

	WideString Message = "Ta operacja usunie nieodwracalnie dane. Chcesz kontynuowaæ? ";
	Message+="\n\nThe data will be removed irretrievably. Do you want to continue?";

	if(Application->MessageBox(Message.c_bstr(),L"Uwaga!  Attention!",MB_YESNO) == IDYES)
    {
                DeleteLineStacker();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAlarmDeleteClick(TObject *Sender)
{
	WideString Message = "Ta operacja usunie nieodwracalnie dane. Chcesz kontynuowaæ? ";
	Message+="\n\nThe data will be removed irretrievably. Do you want to continue?";

	if(Application->MessageBox(Message.c_bstr(),L"Uwaga!  Attention!",MB_YESNO) == IDYES)
    {
                DeleteAlarm();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridCTControlSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
         if(StringGridCTControl->Cells[1][ARow].IsEmpty())//If empty cell were choosen empty form
		{
    		EmptyCTControlForm();
    		return;
		}
      CTControlFillUpForm(ARow);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetCTControlEnter(TObject *Sender)
{
	  DateTimePickerCTControl->DateTime=TDateTime().CurrentDateTime();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAddCTControlClick(TObject *Sender)
{
   

	try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddCTControl();
         }

     
     __finally {
           LabelSavingWarrning->Visible=false;
     }

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonDeleteCTControlClick(TObject *Sender)
{
	WideString Message = "Ta operacja usunie nieodwracalnie dane. Chcesz kontynuowaæ? ";
	Message+="\n\nThe data will be removed irretrievably. Do you want to continue?";

	if(Application->MessageBox(Message.c_bstr(),L"Uwaga!  Attention!",MB_YESNO) == IDYES)
    {
                DeleteCTControl();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAddWasherClick(TObject *Sender)
{

         try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddWasher();
         }
     __finally {
           LabelSavingWarrning->Visible=false;
     }

	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonDeleteWasherClick(TObject *Sender)
{
	WideString Message = "Ta operacja usunie nieodwracalnie dane. Chcesz kontynuowaæ? ";
    Message+="\n\nThe data will be removed irretrievably. Do you want to continue?";

	if(Application->MessageBox(Message.c_bstr(),L"Uwaga!  Attention!",MB_YESNO) == IDYES)
    {
                DeleteWasher();
    }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::StringGridWasherSelectCell(TObject *Sender, int ACol,
      int ARow, bool &CanSelect)
{
	     if(StringGridCTControl->Cells[1][ARow].IsEmpty())//If empty cell were choosen empty form
		{
    		EmptyWasherForm();
    		return;
		}
      WasherFillUpForm(ARow);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetWasherEnter(TObject *Sender)
{
   DateTimePickerWasher->DateTime=TDateTime().CurrentDateTime();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EditMinuteLineStatusEnter(TObject *Sender)
{
    unsigned short MinutesDifference,HourStart,MinStart, HourStop,MinStop, Dummy;
    DateTimePickerLineStatusStart->Time.DecodeTime(&HourStart,&MinStart,&Dummy,&Dummy);
    DateTimePickerLineStatusStop->Time.DecodeTime(&HourStop,&MinStop,&Dummy,&Dummy);
    int StartMin,StopMin;
    StartMin=HourStart*60 + MinStart ;
	StopMin =HourStop*60  + MinStop;
	if (StartMin > StopMin) {
        MinutesDifference = 24*60 - (StartMin-StopMin);
    }
    else
    {
        MinutesDifference = StopMin-StartMin;
    }
    EditMinuteLineStatus->Text=MinutesDifference;
//    if( > DateTimePickerLineStatuStop->Time)
//       Minutes = Date
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetLineStatusEnter(TObject *Sender)
{
	   if(StringGridLineStatus->Cells[0][1].IsEmpty())
       {
           switch (Current.GetShiftKind()) {

              case 1:
                        DateTimePickerLineStatusStart->Time=TDateTime(6,0,0,0);
                    	DateTimePickerLineStatusStop->Time=TDateTime(14,0,0,0);
                    	break;
              case 2:
              			DateTimePickerLineStatusStart->Time=TDateTime(14,0,0,0);
                    	DateTimePickerLineStatusStop->Time=TDateTime(22,0,0,0);
                    	break;
              case 3:
              			DateTimePickerLineStatusStart->Time=TDateTime(22,0,0,0);
                        DateTimePickerLineStatusStop->Time=TDateTime(6,0,0,0);
                    	break;

           default:     DateTimePickerLineStatusStart->Time=TDateTime().CurrentTime();
                        DateTimePickerLineStatusStop->Time=TDateTime().CurrentTime();
                    	break;


               ;
           }
       }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PageControl1Change(TObject *Sender)
{
	switch (PageControl1->ActivePageIndex) {
        case LineStatus:
            	TabSheetLineStatusEnter(Sender);
            	break;
        case Alarms:
            	TabSheetAlarmsEnter(Sender);
                break;
    	case LineStacker:
            	TabSheetLineStackerEnter(Sender);
            	break;
        case  CTControl:
            	TabSheetCTControlEnter(Sender);
            	break;
    	case  Washer:
                TabSheetWasherEnter(Sender);
                break;
    	case Cathodes:
            	FillUpFormCathodes();
                break;
    default:
        ;
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAddLineStatusClick(TObject *Sender)
{

    try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddLineStatus();
    }
     __finally {
           LabelSavingWarrning->Visible=false;
     }

	

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridLineStatusSelectCell(TObject *Sender,
      int ACol, int ARow, bool &CanSelect)
{
		if(StringGridLineStatus->Cells[0][ARow].IsEmpty())//If empty cell were choosen empty form
		{
    		FillingUpSheetLineStatus();
            EmptyFormLineStatus();
            ComboBoxLineStatusChange(Sender);
    		return;
		}
        LineStatusFillUpForm(ARow);
		ComboBoxLineStatusChange(Sender);
		ComboBoxLineStatusSelect(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonDeleteLineStatusClick(TObject *Sender)
{
    DeleteLineStatus();
}
//---------------------------------------------------------------------------


AnsiString GetDocNum();

void __fastcall TFormMain::ButtonDefectsAddClick(TObject *Sender)
{

     try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             EditDefectsArea->Text = GetDocNum();
     }

     __finally {
           LabelSavingWarrning->Visible=false;
     }


  
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonDefectsAddBoothClick(TObject *Sender)
{

   try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             ExportFromBooth(true);
    }
     __finally {
           LabelSavingWarrning->Visible=false;
     }




	 
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonReadDocNoClick(TObject *Sender)
{
     try{
             LabelSavingWarrning->Visible=true;
             Refresh();
             ExportFromBooth(false);
      }
     __finally {
           LabelSavingWarrning->Visible=false;
     }

       
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonDefectsSaveClick(TObject *Sender)
{
	  
	try{
             LabelSavingWarrning->Visible=true;
             Refresh();
             WriteToDatabaseDefects();
    }
     __finally {
           LabelSavingWarrning->Visible=false;
     }


}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAddGeneralRemarksClick(TObject *Sender)
{
	try{
         try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddGeneralRemarks();
         }
         catch(...)
         {
            AnsiString Message = "Wyst¹pi³ nieznany b³¹d";

            ShowMessage(Message);
         }
     }
     __finally {
           LabelSavingWarrning->Visible=false;
     }

	

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonGeneralDeleteClick(TObject *Sender)
{
	DeleteGeneralRemarks() ;	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::TabSheetGeneralRemarksExit(TObject *Sender)
{
  if(!MemoGeneralRemarksDatabase->Lines->Text.IsEmpty())
  {
      if(MemoGeneralRemarks->Lines->Text != MemoGeneralRemarksDatabase->Lines->Text)
      {
          AnsiString Message =  "Zmieniono uwagi ogólne. Proszê nie zapomnieæ o ich zapisaniu.";
          Message+="\n\n";
          Message+="The General remarks have been changed. Please, do not forget to save them.";
          ShowMessage(Message);
      }
  }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonAddEpuroClick(TObject *Sender)
{
    try{
		AddEpuro();
    }
    catch(EConvertError& E)
    {
        AnsiString Message = "Zapewne, Ÿle wpisano liczbê. "
                             "\nW pola edycyjne proszê wpisywaæ tylko liczby. ";
    	Message+="\n\n";
    	Message+=E.Message;
    	ShowMessage(Message);
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonDeleteEpuroClick(TObject *Sender)
{
	DeleteEpuro();	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EditStartWasherSpeedExit(TObject *Sender)
{
	      if(EditStopWasherSpeed->Text.IsEmpty())
          {
                EditStopWasherSpeed->Text = EditStartWasherSpeed->Text;
          }
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonAddCathodesClick(TObject *Sender)
{
  	try{
         try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddCathodes();
         }
         catch(...)
         {
			AnsiString Message = "Nieznany błąd";
            ShowMessage(Message);
         }
     }
     __finally {
           LabelSavingWarrning->Visible=false;
     }

  
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::EditCathode1Change(TObject *Sender)
{
    //AddCathodes();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Button6Click(TObject *Sender)
{


	try{
         try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddMeasurements();
         }
         catch(EConvertError& E)
		 {
			AnsiString Message = "Zapewne źle wpisano liczbę. "
								 "\nW pola edycyjne proszę wpisywać tylko liczby.";
            Message+="\n\n";
            Message+=E.Message;
            ShowMessage(Message);
         }
     }
     __finally {
           LabelSavingWarrning->Visible=false;
     }
   
}
//---------------------------------------------------------------------------

//try{
//         LabelSavingWarrning->Visible=true;
//
//
//     }
//     __finally {
//           LabelSavingWarrning->Visible=false;
//     }
void __fastcall TFormMain::ButtonMeasurementsDeleteClick(TObject *Sender)
{
	WideString Msg = "Ta operacja powoduje N I E O D W O Ł A L N E skasowanie wprowadzonych danych";
	Msg+="\n\n";
	Msg+="Na pewno chcesz kontynuować? \n\n" ;
	Msg+="This operation removes I R R E V E R S I B L Y data. \n\n";
	Msg+="Are you sure you want to proccede? " ;
	if(!Application->MessageBoxA(Msg.c_bstr(),L"Uwaga! Attention!",MB_YESNO)==IDYES )
	{
		return;
	}
    DeleteMeasurements();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DateTimePickerSeparatorChangeEnter(TObject *Sender)
{
	DateTimePickerSeparatorChange->DateTime=TDateTime().CurrentTime();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonAddSeparatorClick(TObject *Sender)
{
	
	try{
         try {
             LabelSavingWarrning->Visible=true;
             Refresh();
             AddSeparator();
         }
         catch(EConvertError& E)
         {
			AnsiString Message = "Zapewne żle wpisano liczbę. "
								 "\nW pola edycyjne proszę wpisywać tylko liczby całkowite.";
			Message+="\n\n";
			Message+=E.Message;
            ShowMessage(Message);
         }
     }
     __finally {
           LabelSavingWarrning->Visible=false;
     }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonDeleteSeparatorClick(TObject *Sender)
{

	if(!DoDelete() )
	{
        return;
    }
	if(DeleteSeparator())
    {
        EmptyFormSeparator();
        FillUpSheetSeparator();
    }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::DateTimePickerSeparatorEnter(TObject *Sender)
{
  // DateTimePickerSeparator->DateTime=TDateTime().CurrentTime();
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::WyjcieExit1Click(TObject *Sender)
{
	        Application->Terminate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonMainReadClick(TObject *Sender)
{
   void EmptyMainEntityForm(); //prototype
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
	   ComboOperatorFillUP("Deputy", true,ComboBoxSuperVisorsGIP,SupervisorsGIP,true); //Shift Deputy means it could be every one (not only operators)
	   Current.WriteToForm();
	//   LastReadID = Current.GetID();
	   PageControl1->Enabled=true;
	   Caption = "   "+Application->Title+"  "+Current.GetDescription();

	   ReadAllDataFromDatabase(Current.GetID());
   }
   else
   {
	   EmptyForms();//
	   EmptyMainEntityForm();
	   ShowMessage("Nie znaleziono danych zmiany o podanych parametrach (dzień, rodzaj zmiany!"
				   "\n\n"
				   "The shift of given parameters has not been found (day, shift kind)!");
   }
   return;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonReportClick(TObject *Sender)
{
 void LineStatusInfo(std::list<int>); //prototype
 void LineStatusInfo(int); //prototype
 void UnscheduledMainStatuses(int); //prototype
int Choice =  ComboBoxReportMode->ItemIndex ;

if (Choice!=14) {
	ClearSheet(StringGridReport);
}
std::list<int> LineStatuses;

switch (Choice) {
	case 0:   //Bed Utilization
			BedUtilization();
			break;
	case 1:  //Developement
			LineStatuses.push_back(6);
			//Process setup
			LineStatuses.push_back(39);
			LineStatusInfo(LineStatuses);
			break;
    case 2:  //Change of product
            LineStatusInfo(4);
        	break;
    case 3:  //Overhaul
			//LineStatusInfo(2);
			Overall() ;
        	break;
    case 4: SunGuardsTime();
        	break;
    case 5: ShowDefects();
        	break;
    case 6: ProductInfo();
        	break;
    case 7: // Production restore
    		LineStatusInfo(29);
        	break;
    case 8: // Production
    		LineStatusInfo(1);
        	break;
	case 9: // V2 Report Summary
			V2_Production();
        //    GenerateCOC();
        	break;
    case 10: // Time scheduling

        	break;
    case 11: // LineStatus Summary
            LineStatusSummary();
        	break;
    case 12:
            GenerateCOC_Time();
        	break;
    case 13:    // CoC like report production - daily resolution
    			GenerateCOC_Products();
				break;
	case 14:
				V2_ProductionOut();
				break;
	case 15:
				DefectsDiffrences_Products();
				break;
	case 16:    DefectExtednded();
				break;
	case 17:    UnscheduledMainStatuses(6);
				break;

default:
    ;
}


// ReportHeader();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonPrintReportClick(TObject *Sender)
{
//   RichEditReport->Print("zobaczymy");
TPrinter* Prnt = Printer();
 Prnt->BeginDoc();

  Prnt->Canvas->CopyRect(TRect(0,0,Prnt->PageWidth,Prnt->PageHeight),PaintBox1->Canvas, TRect(0,0,  PaintBox1->ClientWidth, PaintBox1->ClientHeight));//->TextHeight(Memo1->Lines->Strings[i])),
//  Memo1->Lines->Strings[i]);
//  Prntr->Canvas->Brush->Color = clBlack;
//  Prntr->Canvas->FrameRect(r);
  Prnt->EndDoc();

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::PaintBox1Paint(TObject *Sender)
{
   if(Current.GetID()==0)
    	return;
   int wys;
   TCanvas* Canv  = FormMain->PaintBox1->Canvas;
   Canv->Font->Style = TFontStyles()<< fsBold  ;
   Canv->TextOutA(20,20,Current.GetDescription());
   TPrinter* Prnt = Printer();
    Prnt->Canvas->Font = Canv->Font;  //Prnt->Canvas
    wys = Canv->TextHeight("------------- Separator ------------------------");
    Canv->TextOutA(20,20+wys,wys);
    Canv->TextOutA(20,20+2*wys,Prnt->PageHeight);
    Canv->TextOutA(20,20+3*wys,Prnt->PageWidth);
    PaintBox1->Width=Prnt->PageWidth;
	PaintBox1->Height=Prnt->PageHeight;
    Canv->MoveTo(0,0);
    Canv->LineTo(PaintBox1->Width,PaintBox1->Height);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonPrintCurrentClick(TObject *Sender)
{
    if(Current.GetSumOfMinutes()!=480)
	{
		ShowMessage("Liczba minut w raporcie \"Stan Linii\" jest różna od 480 \nProszę poprawić tę część raportu" );
		return;
	}
	AccessPrintCurrent();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::EditCathode12Change(TObject *Sender)
{
 // ShowMessage("Zmieniono katodê 12 na wartoœæ "+EditCathode12->Text);	
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxReportPredefModeChange(TObject *Sender)
{
  TDateTimePicker* Start = DateTimePickerReportStart;
  TDateTimePicker* StartT = DateTimePickerReportStartTime;
  TDateTimePicker* Stop = DateTimePickerReportStop;
  TDateTimePicker* StopT = DateTimePickerReportStopTime;
  TDateTime Temp;
  unsigned short year;
  unsigned short month;
  unsigned short day;
  unsigned short hour;
  unsigned short min;
  unsigned short sec;
  unsigned short msec;
  int Dayy;
  switch (ComboBoxReportPredefMode->ItemIndex) {
       case 0:  //  Ostatni miesi¹c / Last month
                TDateTime().CurrentDate().DecodeDate(&year, &month, &day);
            //    StartT->DateTime.DecodeTime(&hour, &min, &sec, &msec);
                if(month == 1 )
                {
                    month = 12;
                    year = year - 1;
                }
                else
                {
                    month=month - 1;
                }
                day =1;
                hour = 22;
                min = 0;
                Start->DateTime = TDateTime(year,month,day)-1;
                StartT->Time = TDateTime(hour, min, 0,0);
                Temp = TDateTime().CurrentDate();
                Temp.DecodeDate(&year, &month, &day);
                Stop->DateTime = TDateTime(year,month,1)-1;
                StopT->Time = TDateTime(21,59,59,59);
            	break;
       case 1:   //	Bie¿¹cy miesi¹c / Current month
                TDateTime().CurrentDate().DecodeDate(&year, &month, &day);
            //    StartT->DateTime.DecodeTime(&hour, &min, &sec, &msec);
                day =1;
                hour = 22;
                min = 0;
                Start->DateTime = TDateTime(year,month,day)-1;
                StartT->Time = TDateTime(hour, min, 0,0);
                Stop->DateTime = TDateTime().CurrentDate();
                StopT->Time = TDateTime().CurrentTime();
            	break;
       case 2:   //	Ostatni dzieñ / Last Day
				Start->DateTime =TDateTime().CurrentDate()-2;
				StartT->Time = TDateTime(22,0,0,0);
				Stop->DateTime = TDateTime().CurrentDate()-1 ;
				StopT->Time = TDateTime(21,59,59,59);
                break;
       case 3:    //	Bie¿¹cy dzieñ / Current day
                Start->DateTime =TDateTime().CurrentDate()-1;
                StartT->Time = TDateTime(22,0,0,0);
                Stop->DateTime = TDateTime().CurrentDate() ;
                StopT->Time = TDateTime().CurrentTime();
				break;
	   case 4:  // Raport poranny/Usual Morning Report
				Dayy =  TDateTime().CurrentDate().DayOfWeek();
				if( Dayy == 2 ) // if day is monday
				{
				   Start->DateTime =TDateTime().CurrentDate()-3;
				}
				else
				{
					Start->DateTime =TDateTime().CurrentDate()-1;
				}
				StartT->Time = TDateTime(6,0,0,0);
				Stop->DateTime = TDateTime().CurrentDate() ;
				StopT->Time = TDateTime(5,59,59,99);
				ComboBoxReportMode->ItemIndex=4;
				break;
   default:
	   ;
   }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonExportReportClick(TObject *Sender)
{
  if(ComboBoxReportMode->ItemIndex != 4 ) // 4 means SunGuardsTimes options
  {
	ExportExcel();
  }
  else
  {
	  ExportExcel2();
  }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::FormClose(TObject *Sender, TCloseAction &Action)
{
//if(ExcelApplication1->Visible)
//     {
//        ExcelApplication1->set_Visible(0,1);
//	    ExcelApplication1->Disconnect();
//     }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxLineStatusChange(TObject *Sender)
{

		LineStatusChangingControls(ComboBoxLineStatus->ItemIndex);
       //Doubled from ComboBoxLineStatusDrawItem - to serve programatical change of combobox
		




//	 if(ComboLineStatusToID[ComboBoxLineStatus->ItemIndex]==4)//ComboBoxLineStatus->Text.Pos("Changeover")!=0)
//	 {
//		 LabelProdFrom->Visible=true;
//		 LabelProdTo->Visible=true;
//		 ComboBoxProdFrom->Visible=true;
//		 ComboBoxProdTo->Visible=true;
//	 }
//	 else
//	 {
//		 LabelProdFrom->Visible=false;
//		 LabelProdTo->Visible=false;
//		 ComboBoxProdFrom->Visible=false;
//		 ComboBoxProdTo->Visible=false;
//
//	 }
}
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
		Application->MessageBoxA(L"Podaj najpierw w³aœciw¹ zmianê \n\nFirst fill up relevant shift", L"Uwaga! Attention!",MB_OK);
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


void __fastcall TFormMain::CheckBoxLeadmanClick(TObject *Sender)
{
	  ComboBoxSuperVisorsEnter(Sender);
	  ComboBoxSuperVisors->ItemIndex=0;
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

void __fastcall TFormMain::CheckBoxDeputyProcessClick(TObject *Sender)
{
	  ComboBoxProcessOperatorEnter(Sender);
	  ComboBoxProcessOperator->ItemIndex=0;
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

void __fastcall TFormMain::CheckBoxDeputyBoothClick(TObject *Sender)
{
	  ComboBoxBoothOperatorEnter(Sender);
	  ComboBoxBoothOperator->ItemIndex=0;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::RadioGroupModeClick(TObject *Sender)
{
  AnsiString Mode;
  switch (RadioGroupMode->ItemIndex) {
	 case 0:
			  Mode = "Global";
			  break;
	 case 1:
			  Mode = "Local Path";
			  break;
	 case 2:
			  Mode = "Testing Path";
			  break;
  default:
			  Mode = "Path";
			  break;
	  ;
  }
  SetPaths(Mode);
}
//---------------------------------------------------------------------------

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
bool TFormMain::NextShift()
{
	TDateTime CurrentDate =DateTimePickerMain->Date;
	void EmptyMainEntityForm();//prototype
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
	ComboBoxShiftKindChange(this);
	ShiftExists =   Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,0,ComboBoxShiftKind->ItemIndex+1);
	if (ShiftExists) {
	   Current.ReadEntity();
	   ComboOperatorFillUP("Deputy", true,ComboBoxSuperVisorsGIP,SupervisorsGIP,true);
	   Current.WriteToForm();
	   PageControl1->Enabled=true;
	   Caption = "   "+Application->Title+"  "+Current.GetDescription();
	   ReadAllDataFromDatabase(Current.GetID());
	   // ImagePreviousShift->Enabled=true;
	   ComboBoxShiftKindChange(this);
   }
   else
   {
	   EmptyForms();//
	   EmptyMainEntityForm();
	   ShowMessage("Nie znaleziono danych zmiany o podanych parametrach (dzień, rodzaj zmiany!"
				   "\n\n"
				   "The shift of given parameters has not been found (day, shift kind)!");

	   // ImageNextShift->Enabled=false;

   }

   return ShiftExists;
}
void __fastcall TFormMain::ImageNextShiftClick(TObject *Sender)
{
	NextShift();
}

bool TFormMain::PrevShift()
{
    void EmptyMainEntityForm(); //prototype
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
	ComboBoxShiftKindChange(this);
	ShiftExists =   Current.CheckIfIsInDatabase(DateTimePickerMain->DateTime,0,ComboBoxShiftKind->ItemIndex+1);
	if (ShiftExists) {
	   Current.ReadEntity();
       ComboOperatorFillUP("Deputy", true,ComboBoxSuperVisorsGIP,SupervisorsGIP,true);
	   Current.WriteToForm();
	   PageControl1->Enabled=true;
	   Caption = "   "+Application->Title+"  "+Current.GetDescription();
	   ReadAllDataFromDatabase(Current.GetID());
	   // ImageNextShift->Enabled=true;
	   ComboBoxShiftKindChange(this);
   }
   else
   {
	   EmptyForms();//
	   EmptyMainEntityForm();
	   ShowMessage("Nie znaleziono danych zmiany o podanych parametrach (dzień, rodzaj zmiany!"
				   "\n\n"
				   "The shift of given parameters has not been found (day, shift kind)!");
	   // ImagePreviousShift->Enabled=false;
   }

   return ShiftExists;
}
//---------------------------------------------------------------------------
void TFormMain::ShiftReviewer()
{

}
void __fastcall TFormMain::ImagePreviousShiftClick(TObject *Sender)
{
	PrevShift();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::LabelDateEndClick(TObject *Sender)
{
 #ifdef test
	MemoTest->Lines->Add(DateTimePickerMain->DateTime.DateTimeString());
 #endif
}
//---------------------------------------------------------------------------

void PrepareDefectsReport(std::list<int> WorkShiftIDsList, TDefects& DL);

void __fastcall TFormMain::ButtonFastReportClick(TObject *Sender)
{
	TADOQuery* Query = DataModule1->ADOQueryHeader;
	DataModule1->ADOConnectionCreport->Connected=true;
	UnicodeString QueryString;
	Query->Close();
	QueryString = Query->SQL->Text;
	RichEditReport->Text = QueryString;

//	Query->SQL->Clear();

//	QueryString =   "DECLARE @SHIFT AS char(2)"
//	"DECLARE @SUPERVISOR AS VARCHAR(50) "
//	"DECLARE @PROCES AS VARCHAR(50)"
//	"DECLARE @BOOTH AS VARCHAR(50) "
//	"DECLARE @DAT AS DATE "
//	"DECLARE @DOCNUM AS VARCHAR(50)"
//	"SET @DOCNUM = \'" + Current.GetDOCNUM() +
//	"\' SET @SHIFT = (SELECT [SHIFT] FROM CP_HEAD  WHERE DOCNUM = @DOCNUM) "
//	"SET @SUPERVISOR = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS] INNER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HEAD].USER_SV "
//	"WHERE [dbo].[CP_HEAD].DOCNUM = @DOCNUM )"
//	"SET @PROCES = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS] INNER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HEAD].USER_CO "
//	"WHERE [dbo].[CP_HEAD].DOCNUM = @DOCNUM) "
//	"SET @BOOTH = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS] INNER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HEAD].USER_DO "
//	"WHERE [dbo].[CP_HEAD].DOCNUM = @DOCNUM )"
//	"SELECT @DAT = [DATA] FROM CP_HEAD WHERE [dbo].[CP_HEAD].DOCNUM = @DOCNUM "
//	"SELECT  @SHIFT as SHIFT, @SUPERVISOR as SuperVisor ,@PROCES as Proces, @BOOTH  AS Booth, @Dat as StartDate  ";
//	Query->SQL->Add(QueryString);
	Query->Parameters->ParamByName("DOCN")->Value= Current.GetDOCNUM();
	Query->Prepared=true;
	void LineStatusQuery(int);
	LineStatusQuery(Current.GetID());
	UnicodeString Filter = "ID_WORKSHIFTDAY = "+UnicodeString(Current.GetID());
	DataModule1->ADOTableProd->Filter=Filter;
	DataModule1->ADOTableProd->Filtered=true;
	DataModule1->ADOTableWorkshitfdays->Filter=Filter;
	DataModule1->ADOTableWorkshitfdays->Filtered=true;
	DataModule1->ADOQueryDefectsShiftReport->Parameters->ParamByName("SHIFTID")->Value = Current.GetID();
	DataModule1->ADOQueryDefectsShiftReport->Open();
	extern TCathodeReport CathReport;
	DataModule1->frxUserDataSetCathodes->RangeEnd = reCount;
	DataModule1->frxUserDataSetCathodes->RangeEndCount= CathReport.GetNoCathodes();
	EmptySheet(FormMain->StringGridReport);
	extern int ReportLineCounter; //counter of data labels used in report
	ReportLineCounter =0;

	TDefects TempDefListLike;
	std::list<int> WorkShiftsList;
	WorkShiftsList.push_back(Current.GetID());
	PrepareDefectsReport(WorkShiftsList,TempDefListLike);

	DataModule1->ADOTableWorkshiftRemarks->Filter="ID_WorkshiftDay = "+UnicodeString(Current.GetID());
	DataModule1->ADOTableWorkshiftRemarks->Filtered = true;
	extern  TOEE_Struct CurrentOEE;
	extern TOEE_Struct CurrentMonthOEE;
	CurrentOEE =  NewOEE(Current.GetID());
    unsigned short year;
	 unsigned short month;
	 unsigned short day;
	 DecodeDate(TDateTime().CurrentDateTime(),year,month,day);
	 day = 1;
	 TDateTime Begin =  TDateTime(year,month,day);
	 //TDateTime End = TDateTime().CurrentDateTime();
	 TDateTime End = Current.GetDate();
	 if (End <= Begin) {
		End = TDateTime().CurrentDateTime();
		if (End <= Begin)
		{
		   ShowMessage("Start Date > End Date; try to change dates");
		   this->Close();
		}
	 }
	CurrentMonthOEE = NewOEE(Begin,End);
	DataModule1->frxReport1->ShowReport(true);
	DataModule1->ADOQueryDefectsShiftReport->Close();
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::ComboBoxLineStatusSelect(TObject *Sender)
{
	  //LabelLineStatus->Caption = ComboBoxLineStatus->ItemIndex;
	  TStatus* Status = LineStatuses->GetStatus(ComboLineStatusToID[ComboBoxLineStatus->ItemIndex]);
	  if(Status!=0)
	  {
		  if(Status->IsItScheduled())
		  {
			MemoLineStatusDescription->Color = clWindow;
		  }
		  else
		  {
			MemoLineStatusDescription->Color = clAqua;
          }
		  MemoLineStatusDescription->Text=Status->GetPolishDescription();
		  MemoLineStatusDescription->Lines->Add("____________________________________________________");
		  MemoLineStatusDescription->Lines->LineBreak;
		  MemoLineStatusDescription->Lines->Add(Status->GetEnglishDescription());
		  //MemoLineStatusDescription->Text = MemoLineStatusDescription->Text + Status->GetEnglishDescription();
		  LabelLineStatusEnglishDescription->Caption=Status->GetEnglishName();
	  }
}
//---------------------------------------------------------------------------

void TFormMain::LineStatusChangingControls(Indeks)
{

   static int Prod=-1;
   static int Extend=-1;
   static int To = -1;
   switch (ComboLineStatusToID[Indeks]) { //ComboLineStatusToID[ComboBoxLineStatus->ItemIndex]
		case 4:                                     //Changeover - register from which product to which was the change
				 LabelProdFrom->Visible=true;
				 LabelProdTo->Visible=true;
				 LabelProdFrom->Caption = "Z / From";
				 LabelProdTo->Caption = "Na / To";
				 ComboBoxProdFrom->Visible=true;
				 ComboBoxProdTo->Visible=true;
				 MaskEdit1->Visible=false;
				 UpDownSpeed->Visible=false;
				 Prod=FormMain->ComboBoxProdFrom->ItemIndex;
				 To=FormMain->ComboBoxProdTo->ItemIndex;
				 FormMain->ComboBoxProdFrom->Items = StringsProducts;
				 FormMain->ComboBoxProdTo->Items = StringsProducts;
				 FormMain->ComboBoxProdFrom->ItemIndex= Prod;
				 FormMain->ComboBoxProdTo->ItemIndex= To;
				 break;
		case 1:  LabelProdFrom->Visible=true;        // Production - registering which product and with which speed.
				 LabelProdTo->Visible=true;
				 LabelProdFrom->Caption = "Prod.";
				 LabelProdTo->Caption = "Pręd. / Speed";
				 ComboBoxProdFrom->Visible=true;
				 ComboBoxProdTo->Visible=false;
				 MaskEdit1->Visible=true;
				 UpDownSpeed->Visible=true;
                 Prod=FormMain->ComboBoxProdFrom->ItemIndex;

				 FormMain->ComboBoxProdFrom->Items = StringsProducts;
				 FormMain->ComboBoxProdFrom->ItemIndex= Prod;
				 break;

	default:
				 LabelProdFrom->Visible=true;
				 LabelProdFrom->Caption = "Prod.";
				 LabelProdTo->Visible=false;
				 ComboBoxProdFrom->Visible=true;
				 ComboBoxProdTo->Visible=false;
				 MaskEdit1->Visible=false;
				 UpDownSpeed->Visible=false;
				 Extend=FormMain->ComboBoxProdFrom->ItemIndex;

				 FormMain->ComboBoxProdFrom->Items = StringsProductsExtended;
				 FormMain->ComboBoxProdFrom->ItemIndex= Extend;
			   //	 FormMain->ComboBoxProdTo->Items = StringsProducts;
				 break;
		;
	}
}


void __fastcall TFormMain::ComboBoxLineStatusDrawItem(TWinControl *Control, int Index,
		  TRect &Rect, TOwnerDrawState State)
{
     //   if(ComboBox1->ItemIndex==-1)
	//	 return;
//   for(int i=0;i<ComboBox1->Items->Count;i++)
//   {
//	  ComboBox1->ItemIndex=i;
//	  ComboBox1->Canvas->TextRect(Rect,Rect.left+3,Rect.top+1,ComboBox1->ItemIndex);
//   }
	//FormMain->Caption=UnicodeString(Index) +" - "+ UnicodeString(Rect.TopLeft().X)+";" + UnicodeString(Rect.TopLeft().Y);
	//ComboBox1->Canvas->Rectangle(Rect);
	//ComboBox1->Canvas->TextRect(Rect,5,5,"Cos")  ;       // ComboBox1->Items[ComboBox1->ItemIndex].ToString()
//	ComboBoxLineStatus->Height = ComboBoxLineStatus->Font->Size * float(ComboBoxLineStatus->Font->PixelsPerInch / 72) + 10;
;
	UnicodeString Alfa = ComboBoxLineStatus->Items->Strings[Index];//[Index].;
	//FormMain->Caption = FormMain->Caption + " || " + Alfa;
	TStatus* Status = LineStatuses->GetStatus(ComboLineStatusToID[Index]); //ComboBoxLineStatus->ItemIndex
	  if(Status!=0 && State.Contains(odSelected))
	  {
          if(Status->IsItScheduled())
		  {
			MemoLineStatusDescription->Color = clWindow;
		  }
		  else
		  {
			MemoLineStatusDescription->Color = clAqua;
          }
		  MemoLineStatusDescription->Text=Status->GetPolishDescription();
		  MemoLineStatusDescription->Lines->Add("____________________________________________________");
		  MemoLineStatusDescription->Lines->LineBreak;
		  MemoLineStatusDescription->Lines->Add(Status->GetEnglishDescription());
		  //MemoLineStatusDescription->Text = MemoLineStatusDescription->Text + Status->GetEnglishDescription();
		  LabelLineStatusEnglishDescription->Caption=Status->GetEnglishName();

	  }
	ComboBoxLineStatus->Canvas->TextRect(Rect,Rect.left+3,Rect.top+1,Alfa /*ComboBox1->Items[Index].ToString()*/);
	//************* Changing behaviour of different controls depending on kind of line status *****************************
	LineStatusChangingControls(Index);

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonTrialTimeComparisionClick(TObject *Sender)
{
 void IsGreater();
 IsGreater();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::Label41Click(TObject *Sender)
{
	DateTimePickerRight->DateTime = DateTimePickerLeft->DateTime;
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxLineStatusMeasureItem(TWinControl *Control, int Index,
		  int &Height)
{

	Height=(ComboBoxLineStatus->Font->Size * ComboBoxLineStatus->Font->PixelsPerInch / 72)*1.5   ;//EditMinuteLineStatus->Height;

}
//---------------------------------------------------------------------------

void __fastcall TFormMain::UpDownSpeedChangingEx(TObject *Sender, bool &AllowChange,
		  short NewValue, TUpDownDirection Direction)
{
  int start= MaskEdit1->SelStart;
  int length =MaskEdit1->SelLength;
  double DoubDelta=0.1;
  double DoubTym;
  int IntTym;
  int IntDelta=1;
  try
  {
	 switch(length)
	 {
		 case 0: break;
		 case 4: DoubTym = MaskEdit1->SelText.ToDouble();
				 if(Direction==updUp) // if updirection
				 {
				   DoubTym+=DoubDelta;
				   if(DoubTym>=100)
					   DoubTym=0;
				 }
				 else
				 {
				   DoubTym-=DoubDelta;
				   if(DoubTym<0)
					   DoubTym=99.9;
				 }
				 MaskEdit1->SelText=UnicodeString().FormatFloat("00.0",DoubTym);
				 break;
		 case 1: if(start==2) return;
				 IntTym = MaskEdit1->SelText.ToInt();
				 if(Direction==updUp) // if updirection
				 {
				   IntTym+=IntDelta;
				   if(IntTym>=10)
					   IntTym=0;
				 }
				 else
				 {
				   IntTym-=IntDelta;
				   if(IntTym<0)
					   IntTym=9;
				 }
				 MaskEdit1->SelText=IntTym;
				 break;
		 case 2: if(start!=0) return;
				 IntTym = MaskEdit1->SelText.ToInt();
				 if(Direction==updUp) // if updirection
				 {
				   IntTym+=IntDelta;
				   if(IntTym>=100)
					   IntTym=0;
				 }
				 else
				 {
				   IntTym-=IntDelta;
				   if(IntTym<0)
					   IntTym=99;
				 }
				 MaskEdit1->SelText=UnicodeString().FormatFloat("00",IntTym);
				 break;
	 }
  }
  catch(EConvertError& E)
  {
	 ShowMessage(E.Message + "\n\nProszę o sprawdzenie poprwności wprowadzonej liczby!");
	 return;
  }
  catch(...)
  {
	throw;
  }
  MaskEdit1->SelStart= start;
  MaskEdit1->SelLength=length;
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonCopyToClipboardClick(TObject *Sender)
{
  ExportToClipboard();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxStartTimeChange(TObject *Sender)
{
	ComboBoxShiftKind->ItemIndex = ComboBoxStartTime->ItemIndex;
	ComboBoxEndTime->ItemIndex = ComboBoxStartTime->ItemIndex;
	DateTimePickerStartTimeChange(Sender);
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ComboBoxEndTimeChange(TObject *Sender)
{
	ComboBoxShiftKind->ItemIndex = ComboBoxEndTime->ItemIndex;
	ComboBoxStartTime->ItemIndex = ComboBoxEndTime->ItemIndex;
	DateTimePickerMainChange(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::DateTimePickerStartTimeChange(TObject *Sender)
{
	if(ComboBoxShiftKind->ItemIndex==2) // NightShift
	{
		   DateTimePickerMain->Date=  DateTimePickerStartTime->Date+1;
	}
	else
	{
			DateTimePickerMain->Date=  DateTimePickerStartTime->Date;
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
//Procedures defined in AccessCOM.cpp file
void ExportDailyReportToCoC();
void WhatToExport();
void ExportShiftPareto();
void __fastcall TFormMain::ButtonPrzepiszDoCocClick(TObject *Sender)
{
		//ExportDailyReportToCoC();
        //ExportShiftPareto();
		WhatToExport();
}
//---------------------------------------------------------------------------


UnicodeString TFormMain::IdLineStatusToLineStatus(int ID)
{
   std::map<int,int>::iterator pos;
   UnicodeString EventStr="Unknown";
   for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos)
	  {
		  if (pos->second == ID) {
				  EventStr= ComboBoxLineStatus->Items->Strings[pos->first];
				  break;
		  }
	  }
   return  EventStr;
}
void __fastcall TFormMain::ScrollBoxLineStatusResize(TObject *Sender)
{
	 MemoLineStatusDescription->Width = ScrollBoxLineStatus->Width - MemoLineStatusDescription->Left -20;
	 if (MemoLineStatusDescription->Width<217) {   //Design width of Memo control treated as minimum
		MemoLineStatusDescription->Width = 217;
	 }
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonExportToExcelDefectsGIPClick(TObject *Sender)
{
  ExportExcel2(FormMain->StringGridDefectsBooth);
}
//---------------------------------------------------------------------------



void __fastcall TFormMain::Button5Click(TObject *Sender)
{
	WideString Msg = L"Ta operacja powoduje N I E O D W O Ł A L N E skasowanie wprowadzonych danych";
	Msg+=L"\n\n";
	Msg+=L"Na pewno chcesz kontynuować? \n\n" ;
	Msg+=L"This operation removes I R R E V E R S I B L Y data. \n\n";
	Msg+=L"Are you sure you want to proccede? " ;
	if(!Application->MessageBoxA(Msg.c_bstr(),L"Uwaga! Attention!",MB_YESNO)==IDYES )
	{
		return;
	}

	DeleteCathodes();
}
//---------------------------------------------------------------------------




void __fastcall TFormMain::EditSpeedExit(TObject *Sender)
{
	EditSpeed->Text = ChangeDecimalSeparator(EditSpeed->Text);
}
//---------------------------------------------------------------------------





void __fastcall TFormMain::CheckBoxNewProductionClick(TObject *Sender)
{
 if (CheckBoxNewProduction->Checked) {
	 EditSpeed->Enabled=true;
	 EditIn->Enabled=true;
	 EditOut->Enabled=true;
	 EditYield->Enabled=true;
 }
 else
 {
	EditSpeed->Enabled=false;
	EditIn->Enabled=false;
	EditOut->Enabled=false;
	EditYield->Enabled=false;
 }

}

//---------------------------------------------------------------------------

void __fastcall TFormMain::StringGridProductsDblClick(TObject *Sender)
{
	ButtonAddProductionClick(Sender);
}
//---------------------------------------------------------------------------


void __fastcall TFormMain::ButtonImportFromExcelClick(TObject *Sender)
{
void ImportFromExcel(); //definied in UnitPacks - function imports data from Excel spreadsheet obtainded from r1_production_detail1 report
						// on production
ImportFromExcel();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonOEEClick(TObject *Sender)
{
  FormOEE=new TFormOEE(this);
  FormOEE->ShowModal();
  delete FormOEE;
  //ShowMessage(UnicodeString().FormatFloat("0.00",OEE(Current.GetID()).OEE)+" %");
}
//---------------------------------------------------------------------------
class TKPIReport;
extern TKPIReport KPIReport;
void __fastcall TFormMain::ReportClick(TObject *Sender)
{
    KPIReport.Generate();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::OptionClick(TObject *Sender)
{
   FormOption->CheckBoxRestrictedArea->Checked=Options.GetRestrictedLiteArea();
   FormOption->ShowModal();
}
//---------------------------------------------------------------------------

void __fastcall TFormMain::ButtonParetoDataClick(TObject *Sender)
{
  CParetoData Pareto;
  TDateTime StartTime();  //defined in "UnitReport.cpp"
  TDateTime StopTime();
  Screen->Cursor = crHourGlass;
  Pareto.GetLineStatuses(StartTime(),StopTime());
//  UnicodeString StopT, StartT;
//  StartT = StartTime().FormatString("yyyymmdd(hhmm)");
//  StopT = StopTime().FormatString("yyyymmdd(hhmm)");

  Pareto.PrintOut(StringGridReport);
  OpenDialogImportFromExcel->InitialDir=ExtractFilePath(Application->ExeName);
  OpenDialogImportFromExcel->Filter = "Excel files with macros (*.xlsm)|*.xlsm|Excel files (*.xlsx)|*.xlsx|Excel 2003 files (*.xls)|*.xls| All Files (*.*)|*.*";
  Screen->Cursor = crDefault;
  UnicodeString TemplateFile = Pareto.GetTemplateFile();					//"\\Template Pareto Coater and Grenzebach June 2016.xlsm";
  UnicodeString TargetFile=Pareto.GetTargetFile();		// ="\\Pareto_from_"+StartT+"_till_"+StopT+".xlsm";
  TargetFile =  OpenDialogImportFromExcel->InitialDir + TargetFile;
  TemplateFile = OpenDialogImportFromExcel->InitialDir + TemplateFile;

   ShowMessage("Proszę wybrać plik Excela, do którego pierwszego arkusza wczytane zostaną dane potrzebne do tworzenia wykresu typu Pareto");
	if(OpenDialogImportFromExcel->Execute())
	{
	  Pareto.ExportToExcel(OpenDialogImportFromExcel->FileName);
	}

//  if (TFile::Exists(TemplateFile))
//  {
////   try{
////	TFile::Copy(TemplateFile,TargetFile);
////	}
////	catch(...)
////	{
////
////	  UnicodeString Messsss ="Prawdopodobnie plik:\n\n\""+TargetFile+"\"\n\n istnieje.\n\n- Wybranie \"OK\" nadpisze go, \n- Wybranie \"Cancel\" umożliwi zmianę nazwy.";
////	  if(Application->MessageBox(Messsss.w_str(),L"Uwaga",MB_OKCANCEL)==IDOK)
////	  {
////		  TFile::Copy(TemplateFile,TargetFile,true);
////
////	  }
////	  else
////	  {
////		if(OpenDialogImportFromExcel->Execute())
////		{
////		  TargetFile =  OpenDialogImportFromExcel->FileName;
////		  OpenDialogImportFromExcel->DefaultExt ="xlsm";
////		  TFile::Copy(TemplateFile,TargetFile);
////		}
////	  }
////	}
//	Pareto.ExportToExcel(TemplateFile);
//  }
//  else
//  {
//	ShowMessage("Proszę wybrać plik Excela do którego pierwszego arkusza wczytane zostaną dane potrzebne do tworzenia wykresu typu Pareto");
//	if(OpenDialogImportFromExcel->Execute())
//	{
//	  Pareto.ExportToExcel(OpenDialogImportFromExcel->FileName);
//	}
//  }

}
//---------------------------------------------------------------------------

