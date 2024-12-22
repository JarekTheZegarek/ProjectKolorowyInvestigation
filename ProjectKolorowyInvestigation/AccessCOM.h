//---------------------------------------------------------------------------

#ifndef AccessCOMH
#define AccessCOMH
//---------------------------------------------------------------------------
#include <map>
enum  EventType {Uptime = 2, ScheduledDowntime = 1, UnscheduledDowntime = 6} ;
class  CSummaryLineStatus
{
	std::map<int,double> SummaryLine;     //Summary minuts of separate events      int IDLineStatus, double Minutes
	std::map<int,double> SummaryCoC;	  // Summary minuts of separate CoC categories      int CoC row, double Minutes
	std::map<int,double> ScheduledDowntimeLine;
	std::map<int,double> UnscheduledDowntimeLine;
	std::map<int,double>::iterator itScheduled;
	std::map<int,double>::iterator itUnscheduled;
	std::map<int,double>::iterator it;    // used when reaching NextEvent
	std::map<int,double>::iterator itCoC;    // used when reaching NextEvent


	static    std::map<int,UnicodeString> CoCCellToCategory;

public:
	static	std::map<int,int> LineIDToCoCCell; // if there is no CoC LineStatus which coresponds to E-Loogbook the Row is equal to 0
	bool IsNextEvent;     //true if running through GetNextEventSummary and when adding event (AddEvent).
						  // its value is false when reaching end of events.
	bool IsNextCoC;
	bool IsNextUnscheduled;
	bool IsNextScheduled;
	CSummaryLineStatus()
	{
		it = SummaryLine.begin();
		itCoC = SummaryCoC.begin();
		itScheduled = ScheduledDowntimeLine.begin();
		itUnscheduled =  UnscheduledDowntimeLine.begin();
		IsNextEvent=false;
		IsNextCoC=false;
		IsNextUnscheduled=false;
		IsNextScheduled=false;
	}
	void AddEvent(int IDLineStatus, double Minutes);
	void AddUnscheduled(int IDLineStatus, double Minutes);
    void AddScheduled(int IDLineStatus, double Minutes);
	void DeleteEvent(int IDLineStatus);
	std::pair<int,double> GetNextEventSummary();
	std::pair<int,double> GetNextEventCoCSummary();
	void ResetIterator()
	{
		it=SummaryLine.begin();
		itCoC = SummaryCoC.begin();
		if (SummaryLine.begin()!=SummaryLine.end()) {
			  IsNextEvent= true;
		}
		if (SummaryCoC.begin()!=SummaryCoC.end()) {
			  IsNextCoC= true;
		}
	}
	void ResetIteratorUnscheduled()
	{
		itUnscheduled=UnscheduledDowntimeLine.begin();
		if (UnscheduledDowntimeLine.begin()!=UnscheduledDowntimeLine.end()) {
			  IsNextUnscheduled= true;
		}
	}
	void ResetIteratorScheduled()
	{
		itScheduled=ScheduledDowntimeLine.begin();
		if (ScheduledDowntimeLine.begin()!=ScheduledDowntimeLine.end()) {
			  IsNextScheduled= true;
		}
	}
	static void FillUpCoCData(); // Assings all line status to proper row in CoC excel sheet
	static void GatherDataForExport(CSummaryLineStatus* Sum) ;
	int GetCoCRow(int LineStatusID)
	{
	  return LineIDToCoCCell[LineStatusID];
	}
	static TStringList* GetCoCCategories();
	static int ReturnLineStatusID(UnicodeString CoCCategory);
	static int ReturnLineStatusID(int CoCRow);
//	void AddLineIDToCoCCell(int,int);
//	void AddCoCCellToCategory(int,UnicodeString);
};


class CSummaryProduction
{
  std::map<int,std::pair<double,double> > SummaryProduction; //int ID_Product, glass in, glass out
  std::map<int,std::pair<double,double> >::iterator it;
  static    std::map<int,UnicodeString> IDtoShortProductName;
  static    std::map<int,UnicodeString> IDtoProductName;

public:
  static    std::map<int,UnicodeString> IDtoCoCCell;      // static void CSummaryProduction::FillUpProductsData();
  bool IsNextProduct;
  CSummaryProduction()
  {
	 it = SummaryProduction.begin();
	 IsNextProduct = false;
  }
  void ResetIterator()
	{
		it=SummaryProduction.begin();
		if (SummaryProduction.begin()!=SummaryProduction.end()) {
			  IsNextProduct= true;
		}
	}
  static void FillUpProductsData();
  void AddProduct(int ID_Product, double In, double Out);
  std::pair<double,double> GetProduct(int ID_Product);
  UnicodeString GetProductName(int ID_Product,bool Preliminary=true);
  UnicodeString GetShortProductName(int ID_Product);
  int GetNextProduct();         // Return next produced product ID
  UnicodeString GetCoCRow(int ProductID)
  {
	return IDtoCoCCell[ProductID];
  }


};

enum EnumParetoReportMainCategories {CategoryDefects, CategoryScheduled, CategoryUnscheduled };

class CParetoData
{
//	CSummaryLineStatus SumkaUnscheduled;
//	CSummaryLineStatus SumkaScheduled;
	std::map<UnicodeString, double> UnscheduledMap;
	std::map<UnicodeString, double> ScheduledMap;
	std::map<UnicodeString, double> DefectsMap;		// Map containing  defects
	TDateTime StartPeriod, StopPeriod;
	int ColUnscheduled, ColScheduled, ColDefects;
	int StartingRow ;
	int MaxNumberOfCategories; //Maximum number of possible categories (it enables to clear default place in spreadsheet)
	UnicodeString TemplateFile;
	UnicodeString TargetFile;
   //	EnumParetoReportMainCategories Cat;
public:
	CParetoData():ColUnscheduled(9),ColScheduled(5),ColDefects(1), StartingRow(2), MaxNumberOfCategories(500)
	{
		TemplateFile = "\\Pareto Coater and Grenzebach.xlsm";
	}

	UnicodeString GetTargetFile()
	{
	  return GetTargetFile( StartPeriod, StopPeriod);
	}
	UnicodeString GetTargetFile(TDateTime Start, TDateTime Stop)
	{
		return "\\Pareto_from_"+Start.FormatString("yyyymmdd(hhmm)")+"_till_"+Stop.FormatString("yyyymmdd(hhmm)")+".xlsm";
	}
	UnicodeString GetTemplateFile()
	{
		return TemplateFile;
	}
	void ParetoDataPrepare()
	{
		DefectsMap.clear();
		UnscheduledMap.clear();
		ScheduledMap.clear();
	}

	int NumberOfElements(EnumParetoReportMainCategories Cat1)
	{
		switch(Cat1)
		{
		 case CategoryUnscheduled: return UnscheduledMap.size();
						   break;
		 case CategoryScheduled: return ScheduledMap.size();
						   break;
		 case CategoryDefects: return DefectsMap.size();
						   break;

		}
	}
	void GetLineStatuses(TDateTime StartDate, TDateTime StopDate);
	void PrintOut(TStringGrid* Sheet);
	void ExportToExcel(UnicodeString ExcelFile);
};

#endif




