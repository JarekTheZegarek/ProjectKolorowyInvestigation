//---------------------------------------------------------------------------

#ifndef DataCoaterH
#define DataCoaterH
//---------------------------------------------------------------------------
#include <Classes.hpp>
#include <Controls.hpp>
#include <StdCtrls.hpp>
#include <Forms.hpp>
#include <ADODB.hpp>
#include <DB.hpp>
#include "RpCon.hpp"
#include "RpConBDE.hpp"
#include "RpConDS.hpp"
#include "RpDefine.hpp"
#include "RpRave.hpp"
#include "frxClass.hpp"
#include "frxDBSet.hpp"
#include "Global.h"

//---------------------------------------------------------------------------
class TDataModule1 : public TDataModule
{
__published:	// IDE-managed Components
	TADOConnection *ADOConnectionCreport;
	TADOTable *ADOTableWorkshiftDays;
	TADOConnection *ADOConnectionExchange;
	TDataSource *DataSourceCreport;
	TADODataSet *ADODataSetCreport;
	TADOTable *ADOTable2;
	TADOQuery *ADOQueryCreport;
	TADOQuery *ADOQueryExchange;
	TADOTable *ADOTableShiftsGroup;
	TADODataSet *ADODataSetTableWorkshiftDays;
	TDataSource *DataSourceTableWorkshiftDays;
	TADODataSet *ADODataSetTableShiftsGroup;
	TDataSource *DataSourceTableShiftsGroup;
	TADOConnection *ADOConnectionGIP;
	TADOQuery *ADOQueryGIP;
	TADOQuery *ADOQueryGIP_Seek;
	TADOConnection *ADOConnectionV2;
	TADOQuery *ADOQueryBedUtilization;
	TADOQuery *ADOQueryLineStatusReport;
	TADOQuery *ADOQueryV2Production;
	TADOQuery *ADOQueryConsumedMorning;
	TADOQuery *ADOQueryConsumedAfternoon;
	TADOQuery *ADOQueryConsumedNight;
	TADOQuery *ADOQueryOutputMorning;
	TADOQuery *ADOQueryOutputAfternoon;
	TADOQuery *ADOQueryOutputNight;
	TADOQuery *ADOQueryPacksID;
	TADOConnection *ADOConnectionGIPAlternative;
	TADOQuery *ADOQueryGIPAlternative;
	TADOQuery *ADOQueryDefectsSummary;
	TADOQuery *ADOQueryDefectsParam;
	TADOQuery *ADOQueryDefCategoryNames;
	TADOQuery *ADOQueryDefToCategory;
	TADOQuery *ADOQueryTemp;
	TADOQuery *ADOQueryInsertPack;
	TADOQuery *ADOQueryPacksWorkProdID;
	TADOQuery *ADOQueryPacksInputGlass;
	TADOQuery *ADOQueryUpdatePacks;
	TADOQuery *ADOQueryDeletePack;
	TADOQuery *ADOQuery1;
	TADOQuery *ADOQueryHeader;
	TADOTable *ADOTableEventReport;
	TADOQuery *ADOQueryPacksView;
	TADODataSet *ADODataSet1;
	TADOTable *ADOTablePacksView;
	TADOTable *ADOTableProd;
	TDataSource *DataSourceProdPacks;
	TfrxReport *frxReport1;
	TfrxUserDataSet *frxUserDataSetCathodes;
	TfrxDBDataset *frxDBDataset1;
	TfrxDBDataset *frxDBDataset2;
	TfrxDBDataset *frxDBDatasetProduction;
	TfrxReport *frxReport2;
	TfrxUserDataSet *frxUserDataSetMeasurements;
	TfrxDBDataset *frxDBDatasetPacksView;
	TfrxDBDataset *frxDBDatasetDefects;
	TfrxDBDataset *frxDBDatasetWorkshiftdays;
	TADOQuery *ADOQueryDefectsShiftReport;
	TADOTable *ADOTableWorkshitfdays;
	TADOTable *ADOTableDefectsExpanded;
	TADOTable *ADOTableWorkshiftRemarks;
	TfrxDBDataset *frxDBDataseWorkshiftRemarks;
	TfrxUserDataSet *frxUserDataSetOEE;
	TADOQuery *ADOQueryInitializeAdditionalToProdID;
	TADOConnection *ADOConnectionReport2;
	void __fastcall frxReport1GetValue(const UnicodeString VarName, Variant &Value);

private:	// User declarations
public:		// User declarations
	__fastcall TDataModule1(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule1 *DataModule1;
//---------------------------------------------------------------------------
// Own functions and structures

void ComboSuperVisorsFillUP();
void ProductsFillUp();
void FillingUpProductsSheet(int ShiftID);
//---------------------------------------------------------------------------
#endif
