//---------------------------------------------------------------------------

#ifndef UnitFormCoCincompatibilityH
#define UnitFormCoCincompatibilityH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormCoCincompatibility : public TForm
{
__published:	// IDE-managed Components
	TEdit *EditIncompatiblileWithCoC;
	TComboBox *ComboBoxCoCCategories;
	TButton *ButtonNewStatusCompatibileWithCoC;
	TLabel *Label1;
	void __fastcall FormActivate(TObject *Sender);
	void __fastcall ButtonNewStatusCompatibileWithCoCClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCoCincompatibility(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCoCincompatibility *FormCoCincompatibility;
//---------------------------------------------------------------------------
#endif
