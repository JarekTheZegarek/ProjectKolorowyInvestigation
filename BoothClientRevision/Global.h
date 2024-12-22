//---------------------------------------------------------------------------
//#ifndef test
//	#define test
//#endif

#ifndef GlobalH
#define GlobalH
#include <map>
#include "UnitShiftReading.h"
#include "UnitMainBooth.h"
#include <vcl.h>
//#include "UnitProduction.h"
//#include "UnitProducts.h"
//#include "UnitTryby.h"
//#include "UnitLineStatus.h"
//#include "UnitDefects.h"
//---------------------------------------------------------------------------
class TProdToLineStatus;

//extern std::map<int,TProduction> GridIndexToProduction;
//extern std::map<int,TProducts>  ComboIndexToProduct;
//extern std::map<int,int> ComboLineStatusToID;
//extern TArrayOfProducts ArrayOfProducts;
//extern TProduction* CurrentProduction;
extern int CurrentId;
extern TMainEntity Current;
//extern AnsiString CreportPath;
extern  bool Local  ; // Local=1 means using local testing database, Report=1 means using reporting Facilities.
extern  bool  Report ;
//extern TTryb Mode;
class TSuperviosorCompare;
extern TSuperviosorCompare  SupervisorsCompare; // Object to combine Supervisors in Creport_export database with USERS in GIP database;
extern std::map<UnicodeString,char> CurrentSupervisorsGIP;
//extern TLineStatus* LineStatuses;
//extern TStringList* StringsProducts;        //StringLists for ComboBox depending on needs - defined in UnistStartUp.
//extern TStringList* StringsProductsExtended; //StringLists for ComboBox depending on needs - defined in UnistStartUp.
//extern TProdToLineStatus ProductsOnly;       //StringLists for ComboBox depending on needs - defined in UnistStartUp.
//extern TProdToLineStatus ProductsExtended;   //StringLists for ComboBox depending on needs - defined in UnistStartUp.
//
//extern TTempDefect TempDefect;
//extern TProduction Total;
//

char GetShiftLetter(int ShiftLetter);

//void AccessPrintCurrent();
//void ExportExcel();
//void ExportExcel2();
//void ExportExcel2(TStringGrid* Sheet);
//void ExportToClipboard();
//void ExportToClipboard(TStringGrid* Sheet);

void FillingUpSheet(int ID, TStringGrid* Sh,  TComboBox* Com, AnsiString Dbase );

// UnitLineStatus functions
void LineStatusFillUp();
int MinuteDiff(const TDateTime Start, const TDateTime Stop);

// UnitStartUp
void ComboOperatorFillUP(AnsiString Shift, bool IsSupervisor, TComboBox* Combo, std::map<AnsiString,int>& Operators, bool All = false ) ;
AnsiString GetGIPDocNum();

void SetPaths(AnsiString ModePath);


//void AddProduction();   // UnitProduction
//void AddAlarms();
//void WriteScrap();
//void SetTemporaryYield(); // UnitProduction
//void ReadAllDataFromDatabase(int WorkshiftID);
//
//void EmptyProductionForm();
//void EmptyForms();
//void EmptyProductionSheet();
//void EmptySheet(TStringGrid* Sheet);
//void EmptyCTControlForm();
//void EmptyAlarmForm();
//void EmptyWasherForm();
//
//void FillingUpAlarmsSheet(int IDWorkshift);
//void FillingUpLineStackerSheet(int ID);
//void FillingUpCTControlSheet(int ID);
//void FillingUpWasherSheet(int ID);
//
//void RemoveSingleProductProduction();


//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);


//void FillingUpSheet(int IDWorkshift, TStringGrid* Sh, AnsiString Database,TComboBox* Combo);

//void AlarmsFillUpForm(int Index);
//
//void DeleteAlarm();
//
//
//
//
//void AddLineStacker();
//
//void EmptyLineStackerForm();
//
//void LineStackerFillUpForm(int Index);
//
//void DeleteLineStacker();
//
//
//
//void EmptyCTControlForm();
//
//void CTControlFillUpForm(int Index);
//
//void DeleteCTControl();
//
//void AddCTControl();
//
//void CTControlFillUpForm(int Index);
//
//
//void AddWasher();
//void DeleteWasher();
//void WasherFillUpForm(int Index);
//void EmptyWasherForm();
//
//void AddLineStatus();
//void FillingUpSheetLineStatus();
//void EmptyFormLineStatus();
//void LineStatusFillUpForm(int Index);
//void DeleteLineStatus() ;


TDateTime GetShiftTime(int ShiftType, int Kind); //Kind means start=0 stop=1;
AnsiString ExchangeAnsStr(AnsiString& A, AnsiString Previous, AnsiString Next);
double AnsiToDouble(AnsiString NrStr);

void ExportFromBooth(bool CurrentShift);


void WriteToDatabaseDefects();
void ReadDefectsFromDataBase();

//void FillingUpGeneralRemarks();
//void AddGeneralRemarks();
//void DeleteGeneralRemarks();
//
//
//
//
//void FillingUpEpuro();
//void DeleteEpuro();
//void AddEpuro();
//void FillUpFormEpuro();
//void EmptyFormEpuro();
//
//void FillUpSheetCathodes();
//void FillUpFormCathodes();
//void AddCathodes();
//bool DeleteCathodes();
//void EmptyFormCathodes();
//
//
//void FillUpSheetMeasurements();
//void FillUpFormMeasurements();
//void AddMeasurements();
//bool DeleteMeasurements();
//void EmptyFormMeasurements();
//
//
//void FillUpSheetSeparator();
//void FillUpFormSeparator();
//void AddSeparator();
//bool DeleteSeparator();
//void EmptyFormSeparator();
//
//// UnitReport functions
//void ReportHeader();
//void BedUtilization();
//void LineStatusInfo(int ID_LineStatus);
//void SunGuardsTime();
//void ShowDefects();
//void GenerateCOC_Time();
//void LineStatusSummary();
//void  V2_Production();
//void 	V2_ProductionOut();
//void PopulateAllowedModes();
//void ProductInfo();
//void Overall();
//void ClearSheet(TStringGrid* S);
//
//void GenerateCOC_Products();
//void DefectsDiffrences_Products();
//
// class CGlobalVariables
//{
//	public:
//	   static UnicodeString ExchangePath;  //CreportPath,
//	   static UnicodeString DefectsPath;
//	   static UnicodeString CreportPath;
//	   static UnicodeString CoCPath;
//} ;
//
//


struct Language
{
  UnicodeString LabelDiscrepancy;
};







#endif
