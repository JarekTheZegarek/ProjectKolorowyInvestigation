//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitStringGridMagnification.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormMagnification *FormMagnification;
//---------------------------------------------------------------------------
__fastcall TFormMagnification::TFormMagnification(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormMagnification::ButtonCloseClick(TObject *Sender)
{
  FormMagnification->Close();
}
//---------------------------------------------------------------------------

void ExportExcel2(TStringGrid*);

void __fastcall TFormMagnification::ButtonExportClick(TObject *Sender)
{
   ExportExcel2(StringGrid1);
}
//---------------------------------------------------------------------------

