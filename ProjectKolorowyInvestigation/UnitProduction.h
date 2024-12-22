//---------------------------------------------------------------------------

#ifndef UnitProductionH
#define UnitProductionH
#include <vcl.h>
#include "CommonTableClass.h"
#include <map>
//---------------------------------------------------------------------------

class TProduction
{
    int ID_WorkshiftDay;
    double Speed;
    double Total_in;
    double Total_out;
    double Yield;
    int ID_Product_Type;
    TStringList* Remarks;
public:
	TProduction(int ID):ID_WorkshiftDay(ID),Total_in(0),Remarks(0){};
  //  ~TProduction()
  //  {
        //	if(Remarks!=0)            /* TODO : Remove Memory Leakage */
        //		delete Remarks;
  //  }
    void SetSpeed(double S);
    void SetTotalIn(double In);
    void SetTotalOut(double Out);
	bool SetYield();
	void SetShiftId(int ID) {ID_WorkshiftDay=ID; Total_in=0;Total_out=0;  }
    void SetID_Product_Type(int ID);
    int GetID_WorkshiftDay();
    int GetID_Product_Type();
    double GetSpeed();
    double GetTotalIn();
    double GetTotalOut();
    double GetYield();
    void SetRemarks(TStringList*);
    void SetRemarks(AnsiString);
    bool CheckIfRemarksExists() {
    		if (Remarks!=0) {
                   return true;
            } else return false;
    }
    void DeleteRemarks();
    void FillUpForm();
    TStringList* GetRemarks();
};

class TArrayOfProducts:public TCommonTable
{
	std::map<int,int>  Products; // first - Sheet index, second - ProductID
 public:
    void Add(int Index, int ID);
    int GetID(int Index);
    void ClearMap();
    void Remove(int ID){};   /* TODO : Define Remove for Products */
};

class TSubstrate
{
	int ID;
	UnicodeString Name;  // Lami or Float if float then origin of the glass e.g. GIP, Ohroshaza
	int ClassID;
	UnicodeString Class; //e.g. for float "TRR", "Architecture" ...
	int ThicknessID;
	UnicodeString Thickness; //UnicodeString because it could be also Lami composition e.g. "3.3.1"
	UnicodeString Size;
	int SizeID;
	int Width;
	int Length;
public:
	TSubstrate(): ID(0), Name("Unknown"),ClassID(0),Class("Unknown"),ThicknessID(0), Thickness("Unknown"), SizeID(0), Size("Unknown")
	{}
	void Add(int SubstrateID, UnicodeString NameOfSubstrate, int SubstrateClassID, UnicodeString SubstrateClass,int SubstrateThicknessID, UnicodeString SubstrateThickness, UnicodeString SubstrateSizeName, int SubstrateSizeID, int SizeWidth, int SizeLength);
	bool operator==(const TSubstrate& Sub)
	{ return (ID==Sub.ID) && (ClassID == Sub.ClassID) && (ThicknessID == Sub.ThicknessID) && (SizeID == Sub.SizeID)  ; }
	int GetID() { return ID; }
	UnicodeString GetSubName() { return Name; }
	int GetThicknessID() { return ThicknessID; }
	UnicodeString GetThicknessName() { return Thickness; }
	int GetSubClassID() { return ClassID; }
	UnicodeString GetSubClassName() { return Class;}
	int GetSizeID() {return SizeID; }
	UnicodeString GetSizeName() { return Size; }
	int GetWidth() { return Width; }
	int GetLength() { return Length; }
	double GetAreaM2() { return (Width*Length)/1000000.0 ; }
	void Reset() {ID=0; Name="Unknown"; ClassID=0; Class="Unknown"; ThicknessID=0; Thickness="Unknown"; Width=Length=SizeID=0; Size="Unknown";}
};




#endif
