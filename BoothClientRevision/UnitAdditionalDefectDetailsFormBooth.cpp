//---------------------------------------------------------------------------

#include <vcl.h>
#pragma hdrstop

#include "UnitAdditionalDefectDetailsFormBooth.h"
#include "GlobalBooth.h"
#include "UnitMainBooth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma resource "*.dfm"
TFormAdditionalDefectDetails *FormAdditionalDefectDetails;
//---------------------------------------------------------------------------
__fastcall TFormAdditionalDefectDetails::TFormAdditionalDefectDetails(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TFormAdditionalDefectDetails::FormCreate(TObject *Sender)
{
   if(FormMain->ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
   {
	   Combos.AdditionalDefectsLami.Assign(ComboBoxDefectDetails);
   }
   else
   {
	  Combos.AdditionalDefectsFloat.Assign(ComboBoxDefectDetails);
   }
//   if(ReturnAddDefID()==0)
//   {
//	ComboBoxDefectDetails->ItemIndex=0;
//   }
//   else
//   {
//	 ComboBoxDefectDetails->Text = Text;
//   }
}
//---------------------------------------------------------------------------
void __fastcall TFormAdditionalDefectDetails::ButtonAddDetailsClick(TObject *Sender)

{
  UnicodeString Com;
  Com = MemoComment->Text;
  if(FormMain->ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
   {
	   SetIDandComment(Combos.AdditionalDefectsLami.GetDBId(ComboBoxDefectDetails->ItemIndex),Com);
   }
   else
   {
	  SetIDandComment(Combos.AdditionalDefectsFloat.GetDBId(ComboBoxDefectDetails->ItemIndex),Com);
   }
   Close();
}

void TFormAdditionalDefectDetails::SetComboStart(int ID)
{
   UnicodeString Text;
   Text = Combos.AdditionalDefectsCombo.GetNameByDBId(ID);
   int ItemIndex = 0; //Combos.AdditionalDefectsCombo.GetItemIndexFromIDDD(ID);
//   if(ID==0)
//   {
//	ComboBoxDefectDetails->ItemIndex=0;
//   }
//   else
//   {
//	 ComboBoxDefectDetails->Text = Text;
//   }

   if(FormMain->ComboBoxSubstrate->Text.Pos("Lami")!=0) // There is lami substrate
   {
	   ItemIndex = Combos.AdditionalDefectsLami.GetItemIndexFromIDDD(ID);

   }
   else
   {
	  ItemIndex = Combos.AdditionalDefectsFloat.GetItemIndexFromIDDD(ID);;
   }
   ComboBoxDefectDetails->ItemIndex=ItemIndex;


}




//---------------------------------------------------------------------------
