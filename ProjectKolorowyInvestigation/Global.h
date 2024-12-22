//---------------------------------------------------------------------------
//#ifndef test
//	#define test
//#endif

#ifndef GlobalH
#define GlobalH

#include <map>
//#include "UnitMain.h"
#include "UnitMainEntity.h"
#include "UnitProduction.h"
#include "UnitProducts.h"
#include "UnitTryby.h"
#include "UnitLineStatus.h"
#include "UnitDefects.h"
#include "UnitReport.h"
//---------------------------------------------------------------------------



class TProdToLineStatus;

extern std::map<int,TProduction> GridIndexToProduction;
extern std::map<int,TProducts>  ComboIndexToProduct;
extern std::map<int,int> ComboLineStatusToID;
extern TArrayOfProducts ArrayOfProducts;
extern TProduction* CurrentProduction;
extern int CurrentId;
extern TMainEntity Current;
//extern AnsiString CreportPath;
extern  bool Local  ; // Local=1 means using local testing database, Report=1 means using reporting Facilities.
extern  bool  Report ;
extern TTryb Mode;
class TSuperviosorCompare;
extern TSuperviosorCompare  SupervisorsCompare; // Object to combine Supervisors in Creport_export database with USERS in GIP database;
extern std::map<UnicodeString,char> CurrentSupervisorsGIP;
extern TLineStatus* LineStatuses;
extern TStringList* StringsProducts;        //StringLists for ComboBox depending on needs - defined in UnistStartUp.
extern TStringList* StringsProductsExtended; //StringLists for ComboBox depending on needs - defined in UnistStartUp.
extern TProdToLineStatus ProductsOnly;       //StringLists for ComboBox depending on needs - defined in UnistStartUp.
extern TProdToLineStatus ProductsExtended;   //StringLists for ComboBox depending on needs - defined in UnistStartUp.

extern TTempDefect TempDefect;
extern TProduction Total;
//

char GetShiftLetter(int ShiftLetter);

void AccessPrintCurrent();
void ExportExcel();
void ExportExcel2();
void ExportExcel2(TStringGrid* Sheet);
void ExportToClipboard();
void ExportToClipboard(TStringGrid* Sheet);

void FillingUpSheet(int ID, TStringGrid* Sh,  TComboBox* Com,AnsiString Dbase );

// UnitLineStatus functions
void LineStatusFillUp();
int MinuteDiff(const TDateTime Start, const TDateTime Stop);

// UnitStartUp
void ComboOperatorFillUP(AnsiString Shift, bool IsSupervisor, TComboBox* Combo, std::map<AnsiString,int>& Operators, bool All = false ) ;
AnsiString GetGIPDocNum();

void SetPaths(AnsiString ModePath);


void AddProduction();   // UnitProduction
void AddAlarms();
void WriteScrap();
void SetTemporaryYield(); // UnitProduction
void ReadAllDataFromDatabase(int WorkshiftID);

void EmptyProductionForm();
void EmptyForms();
void EmptyProductionSheet();
void EmptySheet(TStringGrid* Sheet);
void EmptyCTControlForm();
void EmptyAlarmForm();
void EmptyWasherForm();

void FillingUpAlarmsSheet(int IDWorkshift);
void FillingUpLineStackerSheet(int ID);
void FillingUpCTControlSheet(int ID);
void FillingUpWasherSheet(int ID);

void RemoveSingleProductProduction();


//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);


//void FillingUpSheet(int IDWorkshift, TStringGrid* Sh, AnsiString Database,TComboBox* Combo);

void AlarmsFillUpForm(int Index);

void DeleteAlarm();




void AddLineStacker();

void EmptyLineStackerForm();

void LineStackerFillUpForm(int Index);

void DeleteLineStacker();



void EmptyCTControlForm();

void CTControlFillUpForm(int Index);

void DeleteCTControl();

void AddCTControl();

void CTControlFillUpForm(int Index);


void AddWasher();
void DeleteWasher();
void WasherFillUpForm(int Index);
void EmptyWasherForm();

void AddLineStatus();
void FillingUpSheetLineStatus();
void EmptyFormLineStatus();
void LineStatusFillUpForm(int Index);
void DeleteLineStatus() ;


TDateTime GetShiftTime(int ShiftType, int Kind); //Kind means start=0 stop=1;
AnsiString ExchangeAnsStr(AnsiString& A, AnsiString Previous, AnsiString Next);
double AnsiToDouble(AnsiString NrStr);

void ExportFromBooth(bool CurrentShift);


void WriteToDatabaseDefects();
void ReadDefectsFromDataBase();

void FillingUpGeneralRemarks();
void AddGeneralRemarks();
void DeleteGeneralRemarks();




void FillingUpEpuro();
void DeleteEpuro();
void AddEpuro();
void FillUpFormEpuro();
void EmptyFormEpuro();

void FillUpSheetCathodes();
void FillUpFormCathodes();
void AddCathodes();
bool DeleteCathodes();
void EmptyFormCathodes();


void FillUpSheetMeasurements();
void FillUpFormMeasurements();
void AddMeasurements();
bool DeleteMeasurements();
void EmptyFormMeasurements();


void FillUpSheetSeparator();
void FillUpFormSeparator();
void AddSeparator();
bool DeleteSeparator();
void EmptyFormSeparator();

// UnitReport functions
void ReportHeader();
void BedUtilization();
void LineStatusInfo(int ID_LineStatus);
void SunGuardsTime();
void ShowDefects();
void GenerateCOC_Time();
void LineStatusSummary();
void  V2_Production();
void 	V2_ProductionOut();
void PopulateAllowedModes();
void ProductInfo();
void Overall();
void ClearSheet(TStringGrid* S);

void GenerateCOC_Products();
void DefectsDiffrences_Products();
void DefectExtednded();

struct TShiftProd;
void ProductionSummary(TShiftProd& Prod, int ShiftID);

struct TOEE_Struct
{
	   double Uptime;
	   double OperationalUptime;
	   int ProductionTime; //in Minuts
	   double Yield;
	   double OperationalYield;
	   static double Speed;				// = 11; // theoretical maximum line speed for ClimaGuard;
	   static double GoalOEE;
	   static double MaxInputPerHour; 		// = 2084;
	   double Performance;			// = Production.In/TheoreticalCGInput;
	   double NewPerformance;
	   double NewPerformanceByProduct[4]; //0 - CG A ; 1- CG HT; 2 - SG A; 3 - SG HT
	   static double NewMaxLineSpeedByProduct[4]; // m/min
	   double OEE;
	   double NewOEE;
	   TDateTime Start;
	   TDateTime Stop;

	   TOEE_Struct():Uptime(0),Yield(0),Performance(0),OEE(0),NewOEE(0), OperationalUptime(0),OperationalYield(0), ProductionTime(0)
	   {
	   }
};

TOEE_Struct OEE(int ShiftID);
TOEE_Struct OEE(TDateTime Begin,TDateTime End);
TOEE_Struct NewOEE(int ShiftID);
TOEE_Struct NewOEE(TDateTime Begin,TDateTime End);

//extern TOEE_Struct OEE_Struct;

 class CGlobalVariables
{
	public:
	   static UnicodeString ExchangePath;  //CreportPath,
	   static UnicodeString DefectsPath;
	   static UnicodeString CreportPath;
	   static UnicodeString CoCPath;
	   static UnicodeString ShiftParetoPath;
	   static UnicodeString ShiftParetoFile;
} ;




struct Language
{
  UnicodeString LabelDiscrepancy;
};



UnicodeString ChangeDecimalSeparator(UnicodeString Source);

typedef  TProducts TCoating;
extern std::map<int,TProducts> Products; // ProdID to Product


//class TCoating
//{
//	int ID_ProdType;
//	AnsiString ProdType_Name;
//	AnsiString FullName;
//	TStringList* Remarks;
//public:
//	TCoating();
//	~TCoating();
//	void SetID(int ID);
//	void SetShortName(AnsiString Name);
//	void SetFullName(AnsiString Name);
//	void SetRemarks(TStringList* Remarks);
//	AnsiString GetFullName();
//	AnsiString GetShortName();
//	AnsiString GetCombinedName();
//	TStringList* GetRemarks();
//	//void operator=(TCoating& In);
//	void operator=(TCoating In);
//
//	bool operator==(const TCoating& Coat);
//	int GetID();
//	void Reset() { ID_ProdType=0; ProdType_Name = "Unknown"; FullName="Unknown"; if(Remarks!=0) Remarks->Clear(); }
//};

class TComboBoxItems
{
   std::map<int, UnicodeString>  DBIdToName;
   std::map<int, int> ItemIndexToDBId;
   std::map<int, TCoating> DBIdToObject;
   //std::map<int, std::pair<int,int> > DBIdToWidthLength;
   std::map<int, UnicodeString> DBIdToExtra;  //Additional data
   std::map<int, TEdit* > DBIdToEdit;   //Edit control with number of defects.
   TComboBox* Combo;
   TStringList* InternalItems;
public:
	TComboBoxItems(TComboBox* AppropriateCombo);
	TComboBoxItems();
	UnicodeString GetNameByItemIndex(int ItemIndex);
	UnicodeString GetNameByDBId(int DBId);
	int AddItem(UnicodeString Name, int BDId); //return ItemIndex
	void AddObject(int DBId, TCoating SomeObject);
	void AddExtra(int DBId, UnicodeString Extra);

//	void AddWidthLength(int DBId, int Width, int Length);
//	double GetArea(int DBId);
//	std::pair<int,int> GetWidthLength(int DBId) { return DBIdToWidthLength[DBId]; }



	int GetDBId(int ItemIndex) { return ItemIndexToDBId[ItemIndex]; }
	int GetItemIndexFromIDDD(int IDDB);
	int GetIDDBFromName(UnicodeString Name);
	UnicodeString GetExtra(int DbId);

	void AddEdit(int DBId, TEdit* Ed);
	TEdit* GetEdit(int DBId);

	TCoating GetObject(int DBId);
	void Assign(TComboBox* NewCombo);  // Assign InternalItems to NewComboBox;
	void Assign(); //// Assign InternalItems to Combo
	void SetDefaultName(UnicodeString DefaultName);
	void ClearItems();

};


//class TComboBoxSizes
//{
//   std::map<int, UnicodeString>  DBIdToName;
//   std::map<int, int> ItemIndexToDBId;
//   std::map<int, TCoating> DBIdToObject;
//   std::map<int, std::pair<int,int> > DBIdToWidthLength;
//   std::map<int, UnicodeString> DBIdToExtra;  //Additional data
//   std::map<int, TEdit* > DBIdToEdit;   //Edit control with number of defects.
//   TComboBox* Combo;
//   TStringList* InternalItems;
//public:
//	TComboBoxItems(TComboBox* AppropriateCombo);
//	TComboBoxItems();
//	UnicodeString GetNameByItemIndex(int ItemIndex);
//	UnicodeString GetNameByDBId(int DBId);
//	int AddItem(UnicodeString Name, int BDId); //return ItemIndex
//
//	void AddObject(int DBId, TCoating SomeObject);
//	void AddWidthLength(int DBId, int Width, int Length);
//	void AddExtra(int DBId, UnicodeString Extra);
//	double GetArea(int DBId);
//	int GetDBId(int ItemIndex) { return ItemIndexToDBId[ItemIndex]; }
//	int GetItemIndexFromIDDD(int IDDB);
//	int GetIDDBFromName(UnicodeString Name);
//	UnicodeString GetExtra(int DbId);
//	std::pair<int,int> GetWidthLength(int DBId) { return DBIdToWidthLength[DBId]; }
//	void AddEdit(int DBId, TEdit* Ed);
//	TEdit* GetEdit(int DBId);
//
//	TCoating GetObject(int DBId);
//	void Assign(TComboBox* NewCombo);  // Assign InternalItems to NewComboBox;
//	void Assign(); //// Assign InternalItems to Combo
//	void SetDefaultName(UnicodeString DefaultName);
//	void ClearItems();
//
//};

class TComboBoxSizes: public  TComboBoxItems
{
   std::map<int, std::pair<int,int> > DBIdToWidthLength;
  public:
   TComboBoxSizes():TComboBoxItems(){}
   void AddWidthLength(int DBId, int Width, int Length);
   double GetArea(int DBId);
   std::pair<int,int> GetWidthLength(int DBId) { return DBIdToWidthLength[DBId]; }
};

struct TCombos
{
	TComboBoxItems SubstrateCombo;
	TComboBoxItems CoatingsCombo;
	TComboBoxItems SubstrateTypeCombo;
	//TComboBoxItems SizeCombo;
	TComboBoxSizes SizeCombo;
	TComboBoxItems ThicknessFloatCombo;
	TComboBoxItems ThicknessLamiCombo;
	TComboBoxItems ThicknessCombo;
	TComboBoxItems SubstrateClassLamiCombo;
	TComboBoxItems SubstrateClassFloatCombo;
	TComboBoxItems SubstrateClassCombo;
	TComboBoxItems DefectsCombo;
	TComboBoxItems AdditionalDefectsLami;
	TComboBoxItems AdditionalDefectsFloat;
	TComboBoxItems AdditionalDefectsCombo;

};

extern TCombos Combos; //definied in Global.cpp

void SubstratesFillUp();
void CoatingsFillUp();
void SizeFillUp();
void SubstrateClassFloatFillUp();
void SubstrateClassLamiFillUp();
void ThicknessFloatFillUp();
void ThicknessLamiFillUp();



class TCathodeReport
{
   int NoCathodes; //Amount of cathedes switched off
   std::map<UnicodeString,int> Cathodes; //UnicodeString - name of switched off cathode ; int number of switch off's
   UnicodeString* CathodeNames;
   int* NoSwitched;
   UnicodeString Comment;
public:
   TCathodeReport()
   {
		NoCathodes = 0;
		CathodeNames = 0;
		NoSwitched = 0;
		Comment="";
   }
   ~TCathodeReport()
   {
	 if (CathodeNames!=0) {
		delete[] CathodeNames;
		CathodeNames=0;
	 }
	 if (NoSwitched!=0) {
		delete[] NoSwitched;
		NoSwitched=0;
	 }
   }
   void Clean()
   {
     if (CathodeNames!=0 && NoSwitched!=0) {
		delete[] CathodeNames;
		delete[] NoSwitched;
	  }
	  NoCathodes = 0;
	  CathodeNames = 0;
	  NoSwitched = 0;
	  Comment="";
   }
   void SetNoCathodes(int No)
   {
	  NoCathodes = No;
	  if (No==0) {
		  return;
	  }
	  if (CathodeNames!=0 && NoSwitched!=0) {
		delete[] CathodeNames;
		delete[] NoSwitched;
		CathodeNames = new UnicodeString[No];
		NoSwitched = new int[No];
	  }
	  else if (CathodeNames==0 && NoSwitched==0) {  //both arrays has to be mantained simultanously
		CathodeNames = new UnicodeString[No];
		NoSwitched = new int[No];
	  }
	  return;

   }

   int* GetNoSwitched() { return NoSwitched; }
   UnicodeString* GetCathodeNames()  { return CathodeNames; }

   int GetNoCathodes() { return NoCathodes; }

   void SetComment(UnicodeString Remark) {Comment = Remark; }
   UnicodeString GetComment() { return Comment;}
};


class TOption
{
   bool RestrictedLiteArea;
public:
   TOption(): RestrictedLiteArea(false){}
   void SetRestrictedLiteArea(bool Restrict)
   {
	 RestrictedLiteArea = Restrict;
   }
   bool GetRestrictedLiteArea() { return RestrictedLiteArea; }
};

extern TOption Options;
#endif
