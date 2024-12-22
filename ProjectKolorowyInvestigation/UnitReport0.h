			//---------------------------------------------------------------------------

#ifndef UnitReportH
#define UnitReportH
#include <vcl.h>
#include <list>
#include <utility>
#include <vector>
#include "global.h"
//---------------------------------------------------------------------------

class SunGuardsTimeEvent
{
	TDateTime StartTime;
	TDateTime EndTime;
	int ProdType;
	UnicodeString ProductFull;
	UnicodeString ProductShort;
	UnicodeString LineStatus;
	int LineStatusID;
	int CategoryID;
	TDateTime DateOfWork;
	int ElapsedTime; //Minute difference of end and start times
	UnicodeString Remarks;
	char ShiftC;
	UnicodeString AdditionalStr;
	double AdditionalID; //Could have been
	int WorkShiftID;
 public:
	SunGuardsTimeEvent() {};
	void SetValues(TDateTime Date, char Shift, TDateTime StartT, TDateTime EndT,UnicodeString Full, UnicodeString Short,UnicodeString LineStat,UnicodeString Remark, int Prod,int LineStatID);
	void SetAdditional(UnicodeString Add);
	void SetWorkshiftID(int ShiftID);
	void SetCategoryID(int CatID) { CategoryID = CatID; }
	TDateTime GetStartTime() { return StartTime;}
	TDateTime GetEndTime() { return EndTime;}
	TDateTime GetDate() { return DateOfWork; }
	UnicodeString GetProductFull() { return ProductFull; }
	UnicodeString GetProductShort() { return ProductShort; }
	int GetProductID() { return ProdType; }
	UnicodeString GetLineStatus() {return LineStatus; }
	int GetLineStatusID() { return LineStatusID; }
	UnicodeString GetRemarks() { return Remarks; }
	int GetElapsedTime() { return ElapsedTime; }
	char GetShift() { return ShiftC;}
	UnicodeString GetAddStr() { return AdditionalStr; }
	double GetAddID() { return AdditionalID; }
	int GetWorkshiftID() { return WorkShiftID; }

	bool operator>(SunGuardsTimeEvent second)  ;
	bool operator==(SunGuardsTimeEvent second) ;
	bool operator<(SunGuardsTimeEvent second) ;
	bool operator>=(SunGuardsTimeEvent second);
	bool operator<=(SunGuardsTimeEvent second);
	bool operator!=(SunGuardsTimeEvent second);
};

using namespace std;
class SunGuardsTimeClass
{
	list<list<SunGuardsTimeEvent> > lista;
	list<list<SunGuardsTimeEvent> > Temp2;
	list<SunGuardsTimeEvent> Temp;
	TDateTime TempDate;
	std::map<int, int> ProdCathegories; // First is Id of product and then ID of main cathegory for "main" cathegories CG A, CG HT, SG A, SG HT  - 6, 5, 4, 3
public:
	void Clear();
	SunGuardsTimeClass();
	void Add(TADOQuery* Q);
	int GetNoMin(int LineStatusID);
	void PrintAll(TStringGrid* Sheet);
	void ExportToReportDB();
	void ReportPrepare();
};

struct TShiftProd
{
  int ShiftType;
  char Shift;
  double In;
  double Out;
  double InCorrected; //Input + Development - European stuctres demand;
};

class TMonthlyData
{
	//Day
	//ShiftType, Shift, In, Out
	std::vector< std::vector<TShiftProd> > ShiftProd;
 public:
	 TMonthlyData()
	 {
		std::vector<TShiftProd> Daily(3);
		ShiftProd.resize(32);
		for(int i=0;i<32;i++)
			ShiftProd[i]=Daily;
	 }
	void Put(int Day,int ShiftType,char Shift,double In, double Out);
	void PutIn(int Day,int ShiftType,char Shift,double In);
	void PutOut(int Day,int ShiftType,char Shift, double Out);
	double GetYield(int Day,int ShiftType,char Shift);
	double GetWholeYield();
};


class TKPIReport
{
  int NumberOfOperatorsPerShift;
  std::vector<int> NumberOfOperatorsPerProdCategory; //Indexed by different categories;
  std::vector<bool> ActiveProdCategories;
  std::vector<bool> ActiveLineStatusCategories;

 // std::vector<std::map<int,int>>
public:
  TKPIReport();
  double BedUtil;
  int WholeTime;
  struct {
   int SGA;
   int SGHT;
   int CGA;
   int CGHT;
   int Uptime() { return SGA+SGHT+CGA+CGHT; }
  }Uptime;
  struct
  {
   struct{
	 int Vent;
	 int Pumping;
   }Overhaul;
   int Setup;
   int Developement;
   int MachineUpgrades;
   int StandBy;
   int ScheduledDowntime()
	   {
		  return  Overhaul.Vent + Overhaul.Pumping   + Setup   + Developement   + MachineUpgrades   + StandBy;
	   }
   int ScheduledOperationalDowntime() { return ScheduledDowntime()-StandBy; }
  }ScheduledDowntime;

  struct {
   int ProcessIssues;
   int AncillaryEquipment;
   int ProductionOrganization;
   int WasherAndWatertreatment;
   int Coater;
   int UnscheduledDowntime()
		{
		  return  ProcessIssues   + AncillaryEquipment   + ProductionOrganization   + WasherAndWatertreatment   + Coater;
		}
  }UnscheduledDowntime;

  struct {
   double SGA;
   double SGHT;
   double CGA;
   double CGHT;
   double Input() { return SGA+SGHT+CGA+CGHT; }
  }Input;

  struct {
   double SGA;
   double SGHT;
   double CGA;
   double CGHT;
   double Input() { return SGA+SGHT+CGA+CGHT; }
  }InputConverted;

  struct {
   double SGA;
   double SGHT;
   double CGA;
   double CGHT;
   double Output() { return SGA+SGHT+CGA+CGHT; }
  }Output;

  struct {
   double InputQuality;
   double ProcessQuality;
   double Equipment;
   double QCBurnIns;
   double Development;
   double Reject() { return InputQuality+ProcessQuality+Equipment+QCBurnIns+Development; }
  }Reject;

 struct
 {
   double Commercial;
   double Residential;
 }ProductionMix;

  void Generate();
  TKPIReport(int NoProdCategory, int NoLineStatusCategories);
  void SetLineCategoryToProductCategory();
  void Clear();
};

class TKPICategories
{
	std::map<UnicodeString,std::list<int> > ProdByCategories; // products list of given type;
	std::map<int,double> Input; // Input by products
	std::map<int,double> InputConverted; //
	std::map<int,double> Output;
	std::map<int,double> OutputConverted;
	std::map<int,int> OperatorsByProdCategories; //
	std::map<int, std::list<int> > EventsByCategories;
	std::map<int, int> TimeByEvent; //Time of each event minutes wise.
	std::map<int, std::list<int> > DefectsByCategories;
	std::map<int, double> Defects; //Area scraped defects wise.

  public:
	KPIProdCategories(); // populating data from database //To be done

};
extern TKPIReport KPIReport;

#endif