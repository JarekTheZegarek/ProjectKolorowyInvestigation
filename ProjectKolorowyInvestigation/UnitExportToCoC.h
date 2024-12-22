//---------------------------------------------------------------------------

#ifndef UnitExportToCoCH
#define UnitExportToCoCH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Datasnap.Win.MConnect.hpp>
#include <Datasnap.Win.ObjBrkr.hpp>
//---------------------------------------------------------------------------
class TFormCoCExport : public TForm
{
__published:	// IDE-managed Components
	TStringGrid *StringGridFrom;
	TStringGrid *StringGridBeforeExport;
	TMemo *Memo1;
	TLabel *LabelOpisZmiany;
	TStringGrid *StringGrid2;
	TButton *ButtonPrepareForExport;
	TButton *ButtonExportToCoC;
	TButton *ButtonExitCoC;
	TStringGrid *StringGridProductsForCoC;
	TStringGrid *StringGridProductPrepared;
	TLabel *Label1;
	TSimpleObjectBroker *SimpleObjectBroker1;
	TLabel *Label2;
	TLabel *Label3;
	void __fastcall ButtonPrepareForExportClick(TObject *Sender);
	void __fastcall ButtonExportToCoCClick(TObject *Sender);
	void __fastcall ButtonExitCoCClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormCoCExport(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormCoCExport *FormCoCExport;
//---------------------------------------------------------------------------
#endif
