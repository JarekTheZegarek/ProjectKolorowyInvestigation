//---------------------------------------------------------------------------

#ifndef UnitMainBoothH
#define UnitMainBoothH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include <Vcl.ExtCtrls.hpp>
#include <Vcl.ImgList.hpp>
#include <Vcl.Grids.hpp>
#include <Vcl.Graphics.hpp>
#include <Vcl.Imaging.jpeg.hpp>
#include <map>
//---------------------------------------------------------------------------
class TFormMain : public TForm
{
__published:	// IDE-managed Components
	TPanel *PanelShift;
	TShape *Shape2;
	TLabel *LabelDateEnd;
	TLabel *LabelShiftLetter;
	TLabel *LabelShiftKind;
	TLabel *LabelSuperVisors;
	TLabel *Label40;
	TComboBox *ComboBoxSuperVisors;
	TCheckBox *CheckBoxDeputyProcess;
	TComboBox *ComboBoxStartTime;
	TDateTimePicker *DateTimePickerStartTime;
	TComboBox *ComboBoxShiftKind;
	TComboBox *ComboBoxShiftLetter;
	TComboBox *ComboBoxEndTime;
	TDateTimePicker *DateTimePickerMain;
	TComboBox *ComboBoxSuperVisorsGIP;
	TCheckBox *CheckBoxDeputySupervisor;
	TCheckBox *CheckBoxLeadman;
	TComboBox *ComboBoxProcessOperator;
	TCheckBox *CheckBoxDeputyBooth;
	TComboBox *ComboBoxBoothOperator;
	TLabel *LabelDateStart;
	TButton *ButtonShiftRead;
	TButton *ButtonAddSubstrate;
	TComboBox *ComboBoxProduct;
	TComboBox *ComboBoxSubstrate;
	TComboBox *ComboBoxSubstrateClass;
	TComboBox *ComboBoxThickness;
	TComboBox *ComboBoxSize;
	TScrollBox *ScrollBoxSubstrate;
	TRadioGroup *RadioGroupSubstrate;
	TLabel *LabelSubstrate;
	TLabel *LabelCoating;
	TLabel *LabelKind;
	TLabel *LabelType;
	TLabel *Label4;
	TLabel *LabelThickness;
	TScrollBox *ScrollBoxDefects;
	TLabel *LabelDefectKind;
	TEdit *EditNoOf;
	TUpDown *UpDownNoOf;
	TButton *ButtonAddDefect;
	TImageList *ImageList1;
	TStringGrid *StringGrid1;
	TImage *Image1;
	TImage *ImagePreviousShift;
	TImage *ImageNextShift;
	TMemo *MemoComment;
	TButton *ButtonAddComment;
	TEdit *EditSummary;
	TLabel *Label1;
	TLabel *Label2;
	TButton *ButtonQuit;
	void __fastcall ComboBoxShiftKindChange(TObject *Sender);
	void __fastcall ComboBoxShiftLetterChange(TObject *Sender);
	void __fastcall ComboBoxSuperVisorsGIPEnter(TObject *Sender);
	void __fastcall ComboBoxProcessOperatorEnter(TObject *Sender);
	void __fastcall ComboBoxBoothOperatorEnter(TObject *Sender);
	void __fastcall DateTimePickerMainChange(TObject *Sender);
	void __fastcall ComboBoxSuperVisorsEnter(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonShiftReadClick(TObject *Sender);
	void __fastcall ComboBoxSubstrateChange(TObject *Sender);
	void __fastcall ButtonAddSubstrateClick(TObject *Sender);
	void __fastcall ImagePreviousShiftClick(TObject *Sender);
	void __fastcall ImageNextShiftClick(TObject *Sender);
	void __fastcall RadioGroupSubstrateClick(TObject *Sender);
	void __fastcall StringGrid1DblClick(TObject *Sender);
	void __fastcall ButtonAddCommentClick(TObject *Sender);
	void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall ButtonQuitClick(TObject *Sender);
	void __fastcall ComboBoxEndTimeChange(TObject *Sender);

private:	// User declarations
    void DateTimeSet();
	bool NextShift();  // Sets the next shift parameters and return true if such a shift exists in database
	bool PrevShift();  // Similar ...
	void ShiftReviewer();
	void FillingUP();
	//void __fastcall ButtonAddDefectClicked(TObject *Sender);
public:		// User declarations
    void __fastcall ButtonAddDefectClicked(TObject *Sender);
	void Function(TButton* B)
	{
	   B->OnClick = ButtonAddDefectClicked;
    }
	std::map<AnsiString,int> Supervisors;
	std::map<AnsiString,int> SupervisorsGIP;
	std::map<AnsiString,int> ProcessOperators;
	std::map<AnsiString,int> BoothOperators;
	std::map<AnsiString,int> ProductNames;
	std::map<char,UnicodeString> CurrentSupervisorsGIP;
	__fastcall TFormMain(TComponent* Owner);
};

void ClearSheet( TStringGrid* S);
//---------------------------------------------------------------------------
extern PACKAGE TFormMain *FormMain;
//---------------------------------------------------------------------------








#endif
