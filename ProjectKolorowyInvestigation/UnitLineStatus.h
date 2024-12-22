//---------------------------------------------------------------------------

#ifndef UnitLineStatusH
#define UnitLineStatusH
#include <list>
#include <vcl.h>
#include <map>

class TStatus
{
	int ID;
	UnicodeString PolishName;
	UnicodeString EnglishName;
	UnicodeString PolishDescription;
	UnicodeString EnglishDescription;
	int CategoryID;
	bool Active;
	bool IsScheduled;
public:
	TStatus():IsScheduled(true){}
	void AddRecord(int aID, UnicodeString aPolishName, UnicodeString aEnglishName, UnicodeString aPolishDescription, UnicodeString aEnglishDescription, int aCategoryID, bool aActive, bool IsItScheduled);
	int GetID();
	UnicodeString GetPolishName();
	UnicodeString GetEnglishName();
	UnicodeString GetPolishDescription();
	UnicodeString GetEnglishDescription();
	bool GetActiveStatus();
	bool IsItScheduled();
	bool operator==(TStatus a);   // Two Status are equal if their ID's are equal
//    bool operator!=(TStatus a);
};

class TLineStatus
{
   std::list<TStatus> Entity;
public:
	void Add(int aID, UnicodeString aPolishName, UnicodeString aEnglishName, UnicodeString aPolishDescription, UnicodeString aEnglishDescription, int aCategoryID, bool aActive, bool IsItScheduled);
	TLineStatus(int a);
	TStatus* GetStatus(int ID); //Return pointer to status with given ID and 0 when appropirate status not found
};



//---------------------------------------------------------------------------

 class TLineStatusCurrent
 {
     TDateTime Start;
	 public:

 };


//---------------------------------------------------------------------------
#endif
