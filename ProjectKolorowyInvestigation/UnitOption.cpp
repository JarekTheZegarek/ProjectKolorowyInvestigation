//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitOption.h"
#include "Global.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormOption *FormOption;
//---------------------------------------------------------------------------
__fastcall TFormOption::TFormOption(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormOption::CheckBoxRestrictedAreaClick(TObject *Sender)
{
  if(CheckBoxRestrictedArea->Checked)
  {
		Options.SetRestrictedLiteArea(true);
  }
  else
  {
	   Options.SetRestrictedLiteArea(false);
  }
}
//---------------------------------------------------------------------------

