//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitFormCoCincompatibility.h"
#include "AccessCOM.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormCoCincompatibility *FormCoCincompatibility;
//---------------------------------------------------------------------------
__fastcall TFormCoCincompatibility::TFormCoCincompatibility(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormCoCincompatibility::FormActivate(TObject *Sender)
{
   ComboBoxCoCCategories->Items= CSummaryLineStatus::GetCoCCategories();
}
//---------------------------------------------------------------------------
void __fastcall TFormCoCincompatibility::ButtonNewStatusCompatibileWithCoCClick(TObject *Sender)

{
   ModalResult=CSummaryLineStatus::ReturnLineStatusID(ComboBoxCoCCategories->Text);
   this->CloseModal();

}
//---------------------------------------------------------------------------
