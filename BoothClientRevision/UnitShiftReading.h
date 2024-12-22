// ---------------------------------------------------------------------------

#ifndef UnitMainEntityH
#define UnitMainEntityH
#include <vcl.h>
#include <ADODB.hpp>
#include "CommonTableClass.h"
#include <list>



class ErrorMain;  // prototype
class TMainEntity : public TCommonTable {
	TDateTime Date;
	int Id;
	AnsiString IdGIP;
	int ShiftLetter;    //1 = A ,..., 4 = D
	int ShiftType;      // 1 - morning, 2-afternoon, 3-night
	int ShiftSupervisorGIP;
	int ShiftSupervisor;
	int ProcessOperator;
	int BoothOperator;
	int NoOfOperators;
	TStringList* Remarks;
	bool ExistsInDatabase;
	TADOQuery* Query;
	int SumOfMinutes;
	AnsiString GetGIPDocNum();

	TComboBox* ComboSuperVisor;

public:


	// std::map<AnsiString,int> Visors;
	AnsiString NewIdGIP();

	TMainEntity();
	~TMainEntity();
	bool WriteEntity(); // Writing to DataBase
	bool CheckIfIsInDatabase(TDateTime DateR, int ShiftLetterR,
		int ShiftTypeR); // true - means that seeking entity exists in DBMS
	bool CheckIfIsRead(TDateTime D, int Shift);
	bool CheckIfHasChanged(int Letter, AnsiString Supervisor,
		int NoOperators); // true if any of arguments differs from saved ones.
	bool ReadEntity(); // Reading from database;

	// true - means that seeking entity exists
	bool Existing() {
		return ExistsInDatabase;
	}
	bool ReadFromForm();
	void WriteToForm(bool operators = false);

	int GetID() {
		return Id;
	}

	int GetShiftKind() {
		return ShiftType;
	}
	char GetShiftLetter();
	int GetSumOfMinutes();

	void SetSumOfMinutes(int Sum) throw (ErrorMain) ;
	void AddMinutes(int) throw (ErrorMain);
	void SubstractMinutes(int minutes);
	TDateTime GetDate();
	AnsiString GetDescription();

	void Remove(int) {
	};
};

// ---------------------------------------------------------------------------

class ErrorMain
{
	UnicodeString Name;
public:
	ErrorMain(UnicodeString Nazwa);
	void Print();
};


struct TCompareRecord
{
	UnicodeString NameGIP;
	UnicodeString NameCreport;
	int NoGIP;
	int NoCreport;
};

//class which ease filling up of remaining after old system Supervisors ComboBox
class TSuperviosorCompare
{  public:
	std::list<TCompareRecord> CompareRecords;


	void add(UnicodeString NGip, UnicodeString NCreport, int NoG, int NoCreport);

	 // Methode returning matching Creports and GIP supervisors
	 // If no match found then return "Supervisor unknown"
	UnicodeString GetCreport(UnicodeString NGip);

	// similar to previous
	UnicodeString GetGIP(UnicodeString NCreport);
	int GetGIP_No(UnicodeString NCreport);
	int GetGIP_No(int NoCreport);
	UnicodeString GetGIPName(int GIP_No);
};


#endif
