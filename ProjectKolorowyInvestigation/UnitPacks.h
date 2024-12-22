//---------------------------------------------------------------------------

#ifndef UnitPacksH
#define UnitPacksH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
#include "Global.h"
#include <Vcl.Grids.hpp>
#include "DataCoater.h"
#include "UnitMain.h"
//---------------------------------------------------------------------------
struct TStringRow;

class TFormPacks : public TForm
{
__published:	// IDE-managed Components
	TLabel *Label104;
	TEdit *EditPacks;
	TUpDown *UpDownPacks;
	TEdit *EditLites;
	TLabel *Label105;
	TUpDown *UpDownLites;
	TLabel *LabelCoating;
	TComboBox *ComboBoxProduct;
	TLabel *LabelKind;
	TComboBox *ComboBoxSubstrate;
	TLabel *LabelType;
	TComboBox *ComboBoxSubstrateClass;
	TLabel *Label100;
	TComboBox *ComboBoxSize;
	TLabel *LabelThickness;
	TComboBox *ComboBoxThickness;
	TButton *ButtonAdd;
	TButton *ButtonUpdate;
	TStringGrid *StringGrid1;
	TLabel *LabelResultantArea;
	TLabel *LabelArea;
	TLabel *LabelIn;
	TLabel *LabelInData;
	TLabel *LabelOutData;
	TLabel *LabelOut;
	TLabel *Label1;
	TLabel *LabelStartIn;
	TLabel *LabelStartOut;
	TLabel *Label4;
	TButton *ButtonDelete;
	TButton *ButtonClose;
	TLabel *Label5;
	TLabel *LabelSpeed;
	TLabel *LabelStartSpeed;
	TLabel *Label8;
	TLabel *LabelFinalValues;
	TLabel *Label2;
	TEdit *EditStartSpeed;
	TEdit *EditStartIn;
	TEdit *EditStartOut;
	TEdit *EditSpeed;
	TEdit *EditIn;
	TEdit *EditOut;
	TCheckBox *CheckBoxDevelopment;
	void __fastcall FormShow(TObject *Sender);
	void __fastcall ComboBoxSubstrateChange(TObject *Sender);
	void __fastcall ButtonAddClick(TObject *Sender);
	void __fastcall EditPacksChange(TObject *Sender);
	void __fastcall StringGrid1SelectCell(TObject *Sender, int ACol, int ARow, bool &CanSelect);
	void __fastcall ButtonUpdateClick(TObject *Sender);
	void __fastcall ButtonDeleteClick(TObject *Sender);
	void __fastcall ButtonCloseClick(TObject *Sender);
	void __fastcall FormCloseQuery(TObject *Sender, bool &CanClose);
	void __fastcall CheckBoxDevelopmentClick(TObject *Sender);

private:	// User declarations
	double ProdAreaIn;
	double ProdAreaOut;
	double Speed;
	int ProdID;
	double StartSpeed;
	double StartAreaIn;
	double StartAreaOut;
	double AreaDef;

public:		// User declarations
	__fastcall TFormPacks(TComponent* Owner);
	void FillStringGrid();
	void CalculateDefects();
	int StartingCoatingID;
	void InsertRow(int RowNo, TStringRow GridRowData);
};


struct TStringRow
{
	int NoLites;
	int NoPacks;
	int IdWorkshift;
	int Id;
	int IdCoating;
	int IdSubstrate;
	int IdSubstrateClass;
	int IdSize;
	int IdThickness;
	double Area;
};


struct StructReportFile{
 TDateTime TransactionDate;
 TDateTime EventDate;
 int Lites;
 UnicodeString Dimmension;
 UnicodeString Part;
 UnicodeString Shift;
 double AvArea;
};


class TPack
{
   UnicodeString Dim;
   int Width;
   int Length;
   int NoLites;
   UnicodeString Part;
   double AvArea;
   void SetWidthLength(UnicodeString Dimm)
	{
	   int pos = Dimm.Pos("x");
	   UnicodeString Temp = Dimm.SubString(1,pos-1);
	   Temp=Temp.Trim();
	   Width = Temp.ToInt();
	   Temp = Dimm.SubString(pos+1,Dimm.Length());
	   Temp=Temp.Trim();
	   Length = Temp.ToInt();
	}
 public:
	TPack():Dim(0),Width(0),Length(0),NoLites(0),Part(0),AvArea(0) {}
	TPack(UnicodeString Dimm,int Lites, UnicodeString PartName):Dim(Dimm),NoLites(Lites),Part(PartName)
	{
	  SetWidthLength(Dim);
	}
	
	double LiteArea() const { return Width*Length; }
	double Area() const { return NoLites*LiteArea(); }
	double GetAvArea() const { return AvArea; }
	int GetWidth() const { return Width; }
	int GetLength() const { return Length; }
	int GetNoLites() const { return NoLites; }
	void Add(UnicodeString Dimm,int Lites, UnicodeString PartName)
	{
	  Dim=Dimm;
	  NoLites=Lites;
	  Part=PartName;
	  SetWidthLength(Dim);
	}
	void Add(StructReportFile S)
	{
	  Dim=S.Dimmension;
	  NoLites=S.Lites;
	  Part=S.Part;
	  SetWidthLength(Dim);
	  AvArea = S.AvArea;
	}
	void Add(UnicodeString Dimm,int Lites, UnicodeString PartName,double Area)
	{
	  AvArea = Area;
	  Add(Dimm,Lites, PartName);
	}
	void Add(StructReportFile S, double Area)
	{
		//AvArea = Area;
		S.AvArea = Area;
		Add(S);
	}
	 bool operator==(const TPack& P)const
	{
      return Dim==P.Dim && P.NoLites==NoLites && P.Part == Part ;
	}
	bool operator!=(TPack& P) const
	{
     	return !operator==(P);
	}
	 bool operator>(const TPack& P)  const
	{
	  return NoLites > P.NoLites;
	}
	 bool operator<( const TPack& P) const
	{
	  return ! ( (*this>P) || (*this==P) );
	}
	bool operator>=(TPack& P) const
	{
	  return *this>P || *this==P;
	}
	bool operator<=(TPack& P) const
	{
	  return *this<P || *this==P;
	}
};

//class TPackMap
//{
//  std::map<TPack,int> NoPacks;
//  public:
//   TPackList();
//   void Add(TPack Pack)
//   {
//	 NoPacks[Pack]++;
//   }
//
//};


struct PackStruct
{
	int CoatID;
	UnicodeString SubType;
	UnicodeString Thickness;
	UnicodeString Origin;
	bool SomethingUnknown;
	bool IsSample;
	bool IsLami;
	PackStruct():SomethingUnknown(false),IsSample(false),IsLami(false){}
};



//---------------------------------------------------------------------------
extern PACKAGE TFormPacks *FormPacks;
//---------------------------------------------------------------------------


//extern std::map<int,int> RowToDBID;

#endif
