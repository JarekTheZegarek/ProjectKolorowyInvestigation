//---------------------------------------------------------------------------
#include <vcl.h>
#ifndef UnitStartUpH
#define UnitStartUpH
#include <map>
#include <list>

void StartingUp();
void ComboOperatorFillUP(AnsiString Shift, bool IsSupervisor, TComboBox* Combo, std::map<AnsiString,int>& Operators, bool All = false ) ;
AnsiString GetGIPDocNum();

void SetPaths(AnsiString ModePath);

struct StructStartUp
{
    AnsiString Creport;
    AnsiString Creport_exchange;
    AnsiString Defects;
};


struct TProdType
{
  TProdType():ID(0),ShortName(""),FullName(""),Description("")
  {
  }
  int ID;
  UnicodeString ShortName;
  UnicodeString FullName;
  UnicodeString Description;
};

using namespace std;
class TProdToLineStatus
{
	  map<int,int> ComboToID; // maps ID of prodtype and non_prodtype to index in ComboBox
	  list<TProdType> Rekordy;
	  list<TProdType>::iterator Counter;
  public:
	  TProdToLineStatus():Counter(Rekordy.begin()){}
	  void Add(int aID,UnicodeString aShortName,UnicodeString aFullName, UnicodeString aDescription);
	  TProdType Find(int aID); // find ProdType by index in ComboBox - in the case of failing it returns empty TPordType (ID==0);
	  void CombineComboWithID(int aIndex,int aID);
	  void CounterReset();
	  TProdType NextType();
	  void ResetComboToID();
	  void Reset();
	  TProdType FindByID(int aID); // find ProdType by ID
	  int FindComboIndex(int aID); // ID is ID of product
};



class CGlobalVariables
{
	public:
	   static UnicodeString ExchangePath;  //CreportPath,
	   static UnicodeString DefectsPath;
	   static UnicodeString CreportPath;
	   static UnicodeString CoCPath;
} ;





//---------------------------------------------------------------------------
#endif
