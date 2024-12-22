//---------------------------------------------------------------------------

#ifndef UnitStringGridMagnificationH
#define UnitStringGridMagnificationH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
//---------------------------------------------------------------------------
class TFormMagnification : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonExport;
	TButton *ButtonClose;
	TStringGrid *StringGrid1;
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall ButtonExportClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormMagnification(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormMagnification *FormMagnification;
//---------------------------------------------------------------------------
#endif
