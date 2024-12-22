//---------------------------------------------------------------------------

#ifndef UnitAdditionalDefectDetailsFormBoothH
#define UnitAdditionalDefectDetailsFormBoothH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
//---------------------------------------------------------------------------
class TFormAdditionalDefectDetails : public TForm
{
__published:	// IDE-managed Components
	TLabel *LabelDefectAdditional;
	TComboBox *ComboBoxDefectDetails;
	TMemo *MemoComment;
	TButton *ButtonAddDetails;
	TLabel *LabelComment;
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonAddDetailsClick(TObject *Sender);
private:	// User declarations
	 int ID;
	 UnicodeString Comment;
public:		// User declarations
	__fastcall TFormAdditionalDefectDetails(TComponent* Owner);
	void SetIDandComment(int AddID, UnicodeString Com) { ID = AddID; Comment = Com; }
	void SetComboStart(int AddID);
	UnicodeString ReturnComment() { return Comment; }
	int ReturnAddDefID() { return ID; }

};
//---------------------------------------------------------------------------
extern PACKAGE TFormAdditionalDefectDetails *FormAdditionalDefectDetails;
//---------------------------------------------------------------------------
#endif
