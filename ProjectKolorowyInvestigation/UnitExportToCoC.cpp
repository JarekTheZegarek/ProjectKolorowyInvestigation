//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitExportToCoC.h"
#include "AccessCOM.h"
#include "UnitFormCoCincompatibility.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"

 extern CSummaryLineStatus CurrentSumka;

TFormCoCExport *FormCoCExport;
//---------------------------------------------------------------------------
__fastcall TFormCoCExport::TFormCoCExport(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------

//static void CSummaryLineStatus::GatherDataForExport(const CSummaryLineStatus* Sum);
void __fastcall TFormCoCExport::ButtonPrepareForExportClick(TObject *Sender)
{
	extern CSummaryProduction CurrentSummaryProduction;
	void PrintingSummaryProduction(CSummaryProduction* Summary,bool Preliminary=true);
	CSummaryLineStatus::GatherDataForExport(&CurrentSumka);
	PrintingSummaryProduction(&CurrentSummaryProduction, false);

}
//---------------------------------------------------------------------------
void ExportDailyReportToCoC();
void __fastcall TFormCoCExport::ButtonExportToCoCClick(TObject *Sender)
{
	ExportDailyReportToCoC();
}
//---------------------------------------------------------------------------

void __fastcall TFormCoCExport::ButtonExitCoCClick(TObject *Sender)
{
// FormCoCincompatibility->Visible=true;
// FormCoCincompatibility->ShowModal();
     FormCoCExport->Close();
}
//---------------------------------------------------------------------------




