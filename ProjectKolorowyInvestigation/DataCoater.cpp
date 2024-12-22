//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "DataCoater.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma link "RpCon"
#pragma link "RpConBDE"
#pragma link "RpConDS"
#pragma link "RpDefine"
#pragma link "RpRave"
#pragma link "frxClass"
#pragma link "frxDBSet"
#pragma resource "*.dfm"
TDataModule1 *DataModule1;
TOEE_Struct CurrentOEE;
TOEE_Struct CurrentMonthOEE;


//---------------------------------------------------------------------------
__fastcall TDataModule1::TDataModule1(TComponent* Owner)
	: TDataModule(Owner)
{

}
//---------------------------------------------------------------------------



int ReportLineCounter; //counter of data labels used in report
void __fastcall TDataModule1::frxReport1GetValue(const UnicodeString VarName, Variant &Value)

{
	 extern TCathodeReport CathReport;
	 int TempInt;
//	 ReportLineCounter++;
//	 if(ReportLineCounter >= FormMain->StringGridReport->RowCount)
//	 {
//       FormMain->StringGridReport->RowCount=FormMain->StringGridReport->RowCount+20;
//     }
//	 FormMain->StringGridReport->Cells[1][ReportLineCounter]= VarName;
//	 if (VarName.Pos("Cath")!=0) {
//		 CathReport.GetComment();
//	 }
	 if(CompareText(VarName,"SwitchedCath")==0 )
	 {
	   Value = CathReport.GetCathodeNames()[frxUserDataSetCathodes->RecNo];
	  return;
	 }
	 if(CompareText(VarName,"NoSwitchedCath")==0 )
	 {
	   TempInt = CathReport.GetNoSwitched()[frxUserDataSetCathodes->RecNo];
	   Value = TempInt;
	  return;
	 }
	 if (CompareText(VarName,"CathComment")==0) {
		Value = CathReport.GetComment();
		return;
	 }

//	if (CompareText(VarName,"MB")==0) {
//		Value =FormMain->EditEpuroMB->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"R1")==0) {
//		Value =FormMain->EditEpuroR1->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"R2")==0) {
//		Value =FormMain->EditEpuroR2->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"AD1")==0) {
//		Value =FormMain->EditEpuroAD1->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"AD2")==0) {
//		Value =FormMain->EditEpuroAD2->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"V0")==0) {
//		Value =FormMain->EditStartWasherSpeed->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"VK")==0) {
//		Value =FormMain->EditStopWasherSpeed->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"EDI")==0) {
//		Value =FormMain->EditEpuroEdi->Text;
//		return;
//	 }
//
//	 if (CompareText(VarName,"VS0")==0) {
//		Value =FormMain->EditSeparatorSettingsStart->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"PS0")==0) {
//		Value =FormMain->EditSeparatorSettingsStart2->Text;
//		return;
//	 }
//	  if (CompareText(VarName,"VSK")==0) {
//		Value =FormMain->EditSeparatorSettingsStop->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"PSK")==0) {
//		Value =FormMain->EditSeparatorSettingsStop2->Text;
//		return;
//	 }
//
//	 if (CompareText(VarName,"VS")==0) {
//		Value = 	FormMain->EditSeparatorVSStart->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"CE")==0) {
//		Value = FormMain->EditSeparatorCEStart->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"PS")==0) {
//		Value = 	FormMain->EditSeparatorPSStart->Text;
//		return;
//	 }
//
//	 if (CompareText(VarName,"VS1")==0) {
//		Value = 	FormMain->EditSeparatorVSStop->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"CE1")==0) {
//		Value = FormMain->EditSeparatorCEStop->Text;
//		return;
//	 }
//	  if (CompareText(VarName,"PS1")==0) {
//		Value = 	FormMain->EditSeparatorPSStop->Text;
//		return;
//	 }
//
//
//
//
//	 if (CompareText(VarName,"YO")==0) {
//		Value = FormMain->EditMeasurementsOptoY->Text;
//		return;
//	 }
//
//	 if (CompareText(VarName,"aO")==0) {
//		Value =   FormMain->EditMeasurementsOptoA->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"bO")==0) {
//		Value = FormMain->EditMeasurementsOptoB->Text;
//		return;
//	 }
//	  if (CompareText(VarName,"CommentO")==0) {
//		Value = FormMain->MemoMeasurementsOpto->Lines->Text;
//		return;
//	 }
//
//	 if (CompareText(VarName,"YT")==0) {
//		Value = FormMain->EditMeasurementsHunterFY->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"aT")==0) {
//		Value =   FormMain->EditMeasurementsHunterFA->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"bT")==0) {
//		Value = FormMain->EditMeasurementsHunterFB->Text;
//		return;
//	 }
//	  if (CompareText(VarName,"CommentT")==0) {
//		Value = FormMain->MemoMeasurementsHunterTop->Lines->Text;
//		return;
//	 }
//
//	 if (CompareText(VarName,"YB")==0) {
//		Value = FormMain->EditMeasurementsHunterGY->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"aB")==0) {
//		Value =   FormMain->EditMeasurementsHunterGA->Text;
//		return;
//	 }
//	 if (CompareText(VarName,"bB")==0) {
//		Value = FormMain->EditMeasurementsHunterGB->Text;
//		return;
//	 }
//	  if (CompareText(VarName,"CommentB")==0) {
//		Value = FormMain->MemoMeasurementsHunterBottom->Lines->Text;
//		return;
//	 }
	// TProduction Prod(Current.GetID());


	 if (CompareText(VarName,"OEE")==0) {
		Value = UnicodeString().FormatFloat("0.00",CurrentOEE.NewOEE)+" %";
		return;
	 }
	 if (CompareText(VarName,"MonthOEE")==0) {
		Value = UnicodeString().FormatFloat("0.00",CurrentMonthOEE.NewOEE)+" %";
		return;
	 }
	  if (CompareText(VarName,"Uptime")==0) {
		Value = UnicodeString().FormatFloat("0.00",CurrentOEE.OperationalUptime*100)+" %";
		return;
	 }
	 if (CompareText(VarName,"Yield")==0) {
		Value = UnicodeString().FormatFloat("0.00",CurrentOEE.OperationalYield*100)+" %";
		return;
	 }
	 if (CompareText(VarName,"Performance")==0) {
		Value = UnicodeString().FormatFloat("0.00",CurrentOEE.NewPerformance*100)+" %";
		return;
	 }
}
//---------------------------------------------------------------------------



