//---------------------------------------------------------------------------
#include <vcl.h>
#ifndef UnitStartUpH
#define UnitStartUpH
#include <map>
#include <list>

void StartingUp();

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








//---------------------------------------------------------------------------
#endif
