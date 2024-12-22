//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitOEEForm.h"
#include "Global.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOEE *FormOEE;

static double OldSpeed;
static double OldInput;
static TOEE_Struct Shift;
static TOEE_Struct Monthly;
static TOEE_Struct Arbitrary;
//---------------------------------------------------------------------------
__fastcall TFormOEE::TFormOEE(TComponent* Owner)
	: TForm(Owner)
{
	OpenedBeforePrinting = false;
}

__fastcall TFormOEE::TFormOEE(TComponent* Owner,bool BeforePrinting): TForm(Owner)
{

  if (BeforePrinting) {
	 OpenedBeforePrinting = true;
  }
}
//---------------------------------------------------------------------------
void __fastcall TFormOEE::ButtonCloseOEEClick(TObject *Sender)
{
   Close();
}
//---------------------------------------------------------------------------


void TFormOEE::ShowOEE()
{
  TCursor Save_Cursor = Screen->Cursor;
  Screen->Cursor = crHourGlass;    // Show hourglass cursor .

 try{


	 double Goal = Shift.GoalOEE;
	 double InstantOEE;
	 if (Current.Existing()) {
		 Shift =NewOEE(Current.GetID());
		 InstantOEE = Shift.NewOEE;
		 LabelCurrentShiftOEEResult->Caption=UnicodeString().FormatFloat("0.00",InstantOEE)+" %";
		 LabelYieldResultShift->Caption=UnicodeString().FormatFloat("0.00",Shift.Yield*100)+" %";
		 LabelUptimeResultShift->Caption=UnicodeString().FormatFloat("0.00",Shift.OperationalUptime*100)+" %";
		 LabelPerformanceResultShift->Caption=UnicodeString().FormatFloat("0.00",Shift.NewPerformance*100)+" %";
		 LabelOldShiftOEE->Caption = UnicodeString().FormatFloat("0.00",Shift.OEE)+" %";
		 LabelOldShiftPerformance->Caption=UnicodeString().FormatFloat("0.00",Shift.Performance*100)+" %";
		 LabelCurrentShiftOEEResult->Transparent=false;
		 if(Goal> InstantOEE)
		 {
			  LabelCurrentShiftOEEResult->Color = clRed;
		 }
		 else
		 {
			  LabelCurrentShiftOEEResult->Color = clGreen;
		 }

	 }
	 else
	 {
		 LabelCurrentShiftOEEResult->Caption="?";
		 LabelYieldResultShift->Caption="?";
		 LabelUptimeResultShift->Caption="?";
		 LabelPerformanceResultShift->Caption="?";
		 LabelOldShiftOEE->Caption = UnicodeString().FormatFloat("0.00",Shift.OEE)+" %";
		 LabelOldShiftPerformance->Caption = UnicodeString().FormatFloat("0.00",Shift.Performance*100)+" %";

	 }



	 LabelGoalResult->Caption=UnicodeString().FormatFloat("0.0",Goal)+" %";
	 LabelGoalResult->Font->Color=clTeal;

	 LabelCurrentShiftOEEResult->Transparent=false;
	 if(Goal> InstantOEE)
	 {
		  LabelCurrentShiftOEEResult->Color = clRed;
	 }
	 else
	 {
		  LabelCurrentShiftOEEResult->Color = clGreen;
	 }

	 unsigned short year;
	 unsigned short month;
	 unsigned short day;
	 DecodeDate(TDateTime().CurrentDateTime(),year,month,day);
	 day = 1;
	 TDateTime Begin =  TDateTime(year,month,day);
	 TDateTime End = TDateTime().CurrentDateTime();
	 //TDateTime End = Current.GetDate();
	 if (End <= Begin) {
		End = TDateTime().CurrentDateTime();
		if (End <= Begin)
		{
		   ShowMessage("Start Date > End Date; try to change dates");
		   this->Close();
		}
	 }
	 if (month==1) {
		 month=12;
		 year = year-1;
		 day = 31;

	 }
	 else
	 {
	   Begin = Begin - 1;
	   DecodeDate(Begin,year,month,day);

	 }
	 Begin = TDateTime(year,month,day,22,0,0,0);
//	 std::pair<TDateTime,TDateTime> SetDateShiftWise2(TDateTime , TDateTime);
//	 std::pair<TDateTime,TDateTime> Dates = SetDateShiftWise2(Begin , End);
//	 Begin = Dates.first;
//	 End = Dates.second;
	 Monthly =  NewOEE(Begin,End);
	 InstantOEE = Monthly.NewOEE;
	 LabelMonthlyOEEResult->Caption=UnicodeString().FormatFloat("0.00",InstantOEE)+" %";
	 LabelYieldResultMonthly->Caption=UnicodeString().FormatFloat("0.00",Monthly.Yield*100)+" %";
	 LabelUptimeResultMonthly->Caption=UnicodeString().FormatFloat("0.00",Monthly.OperationalUptime*100)+" %";
	 LabelPerformanceResultMonthly->Caption=UnicodeString().FormatFloat("0.00",Monthly.NewPerformance*100)+" %";
	 LabelMonthFrom->Caption = Monthly.Start;
	 LabelMonthTo->Caption = Monthly.Stop;
	 LabelOldMonthOEE->Caption = UnicodeString().FormatFloat("0.00",Monthly.OEE)+" %";
	 LabelOldMonthPerformance->Caption = UnicodeString().FormatFloat("0.00",Monthly.Performance*100)+" %";

	 LabelMonthlyOEEResult->Transparent=false;
	 if(Goal> InstantOEE)
	 {
		  LabelMonthlyOEEResult->Color = clRed;
	 }
	 else
	 {
		  LabelMonthlyOEEResult->Color = clGreen;
	 }


	 EditCG_LineSpeed->Text = UnicodeString().FormatFloat("0.00",TOEE_Struct::Speed);
	 LabelMaxInputPerHourResult->Caption = UnicodeString().FormatFloat("0.00",TOEE_Struct::MaxInputPerHour);
//	 void SetDateShiftWise();
//	 SetDateShiftWise();
	 //Arbitrary=NewOEE(0);   //NewOEE(0) calculates for data taken from Report Sheet Start Date and End Date
	 TDateTime StartTime();
	 TDateTime StopTime();
	 Arbitrary=NewOEE(StartTime(),StopTime());
	 InstantOEE = Arbitrary.NewOEE;
	 LabelArbitraryPeriodResult->Caption = UnicodeString().FormatFloat("0.00",InstantOEE)+" %" ;
	 LabelYieldResultArbitrary->Caption=UnicodeString().FormatFloat("0.00",Arbitrary.Yield*100)+" %";
	 LabelUptimeResultArbitrary->Caption=UnicodeString().FormatFloat("0.00",Arbitrary.OperationalUptime*100)+" %";
	 LabelPerformanceResultArbitrary->Caption=UnicodeString().FormatFloat("0.00",Arbitrary.NewPerformance*100)+" %";
	 LabelOldArbitraryOEE->Caption = UnicodeString().FormatFloat("0.00",Arbitrary.OEE)+" %";
	 LabelOldArbitraryPerformance->Caption = UnicodeString().FormatFloat("0.00",Arbitrary.Performance*100)+" %";

	 LabelArbFrom->Caption = Arbitrary.Start;
	 LabelArbTo->Caption = Arbitrary.Stop;

	 LabelArbitraryPeriodResult->Transparent=false;
	 if(Goal> InstantOEE)
	 {
		  LabelArbitraryPeriodResult->Color = clRed;
	 }
	 else
	 {
		  LabelArbitraryPeriodResult->Color = clGreen;
	 }
	 if(OpenedBeforePrinting)
	 {
		ButtonCloseOEE->Caption="Dalej/Continue";
		this->LabelMaxInputPerHourResult->Visible=false;
		this->EditCG_LineSpeed->Visible=false;
		this->ButtonCalculateOEEwithGivenSpeed->Visible=false;
	 }
 }
 __finally
 {
   Screen->Cursor = Save_Cursor; // Always restore the cursor.

 }


}
void __fastcall TFormOEE::FormCreate(TObject *Sender)
{
	OldSpeed = TOEE_Struct::Speed;
	OldInput = TOEE_Struct::MaxInputPerHour;
	Top=(FormMain->ClientHeight - (Top+Height))/2.0;
	Left= (FormMain->ClientWidth - (Left+Width))/2.0;
	ShowOEE();
//   LabelCurrentShiftOEEResult->Caption=UnicodeString().FormatFloat("0.00",OEE(Current.GetID()).OEE)+" %";
//   unsigned short year;
//   unsigned short month;
//   unsigned short day;
//   DecodeDate(TDateTime().CurrentDateTime(),year,month,day);
//   day = 1;
//   TDateTime Begin =  TDateTime(year,month,day);
//   TDateTime End = TDateTime().CurrentDateTime();
//   LabelPeriodOEEResult->Caption=UnicodeString().FormatFloat("0.00",OEE(Begin,End).OEE)+" %";
}
//---------------------------------------------------------------------------

void __fastcall TFormOEE::ButtonCalculateOEEwithGivenSpeedClick(TObject *Sender)
{
   TOEE_Struct::Speed=EditCG_LineSpeed->Text.ToDouble();
   TOEE_Struct::MaxInputPerHour=3.21*(6/6.1)*TOEE_Struct::Speed*60;
   ShowOEE();

}
//---------------------------------------------------------------------------

void __fastcall TFormOEE::FormClose(TObject *Sender, TCloseAction &Action)
{
  TStringGrid* Sheet =  FormMain->StringGridReport;
 // const ColumnCounting = 5;
  Sheet->RowCount = 50;
  Sheet->ColCount = 5;
  int Row = 1;
  for(int i=0;i<Sheet->ColCount;i++)
  { Sheet->Cols[i]->Clear(); }
  UnicodeString TempAnsi;
  int* FieldWidth = new int[Sheet->ColCount+1];
  for (int i = 0; i < Sheet->ColCount+1; i++) {
   FieldWidth[i]=0;
  }





  TempAnsi = "Shift OEE";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelCurrentShiftOEEResult->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Shift Yield";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelYieldResultShift->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Shift Uptime";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
 FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelUptimeResultShift->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Shift Performance";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelPerformanceResultShift->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Monthly OEE";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelMonthlyOEEResult->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Monthly Yield";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelYieldResultMonthly->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Monthly Uptime";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelUptimeResultMonthly->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Monthly Performance";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelPerformanceResultMonthly->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Arbitrary OEE";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelArbitraryPeriodResult->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Arbitrary Yield";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelYieldResultArbitrary->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Arbitrary Uptime";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelUptimeResultArbitrary->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Arbitrary Performance";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = LabelPerformanceResultArbitrary->Caption;
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  TempAnsi = "Performance CGA";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = Arbitrary.NewPerformanceByProduct[0];
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

   TempAnsi = "Performance CGHT";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = Arbitrary.NewPerformanceByProduct[1];
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

   TempAnsi = "Performance SGA";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = Arbitrary.NewPerformanceByProduct[2];
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

   TempAnsi = "Performance SGHT";
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
  Sheet->Cells[0][Row ] = TempAnsi;
  TempAnsi = Arbitrary.NewPerformanceByProduct[3];
  Width =  Sheet->Canvas->TextWidth(TempAnsi);
  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
  Sheet->Cells[1][Row ] = TempAnsi;
  Row++;

  for(int i=0;i<Sheet->ColCount;i++)
  {
	  Sheet->ColWidths[i]=FieldWidth[i]+6;
  }

  TOEE_Struct::Speed=OldSpeed  ;
  TOEE_Struct::MaxInputPerHour=	OldInput  ;
}
//---------------------------------------------------------------------------


