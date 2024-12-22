//---------------------------------------------------------------------------

#ifndef UnitOptionH
#define UnitOptionH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormOption : public TForm
{
__published:	// IDE-managed Components
	TCheckBox *CheckBoxRestrictedArea;
	void __fastcall CheckBoxRestrictedAreaClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormOption(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOption *FormOption;
//---------------------------------------------------------------------------
#endif
