//---------------------------------------------------------------------------

#ifndef DetailFormBoothH
#define DetailFormBoothH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormAdditionalDetails : public TForm
{
__published:	// IDE-managed Components
	TComboBox *ComboBoxThickness;
	TComboBox *ComboBoxSize;
	TComboBox *ComboBoxSubstrateClass;
	TComboBox *ComboBoxSubstrate;
	TComboBox *ComboBoxProduct;
	TButton *ButtonDelete;
	TLabel *LabelThickness;
	TLabel *Label4;
	TLabel *LabelType;
	TLabel *LabelKind;
	TLabel *LabelCoating;
	TComboBox *ComboBoxDefectDetails;
	TComboBox *ComboBoxDefect;
	TLabel *LabelDefectAdditional;
	TLabel *LabelDefect;
	TComboBox *ComboBoxDate;
	TLabel *Label1;
	TStringGrid *StringGridAdditional;
	TLabel *LabelComment;
	TEdit *EditDate;
	TButton *ButtonAddComent;
	TRichEdit *RichEditComment;
	TEdit *EditDateDate;
	TButton *ButtonClose;
	TButton *ButtonUpdate;
	void __fastcall StringGridAdditionalSelectCell(TObject *Sender, int ACol, int ARow,
          bool &CanSelect);
	void __fastcall ComboBoxSubstrateChange(TObject *Sender);
	void __fastcall ButtonAddComentClick(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall ButtonUpdateClick(TObject *Sender);
private:	// User declarations
public:		// User declarations
	__fastcall TFormAdditionalDetails(TComponent* Owner);
	bool IsGlobal;      //true if there was comment written in global sheet
	UnicodeString GlobalComment; //comment written i global sheet.
	void _fastcall FillRow(int i);
	void _fastcall FillRows();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormAdditionalDetails *FormAdditionalDetails;
//---------------------------------------------------------------------------
#endif
