//---------------------------------------------------------------------------

#ifndef GlobalBoothH
#define GlobalBoothH
//---------------------------------------------------------------------------
#include <vcl.h>
#include <map>
#include "UnitProductsBooth.h"
#include "UnitMainBooth.h"
#include <vcl.grids.hpp>
#include  "UnitReadShiftData.h"


class TComboBoxItems
{
   std::map<int, UnicodeString>  DBIdToName;
   std::map<int, int> ItemIndexToDBId;
   std::map<int, TCoating> DBIdToObject;
   std::map<int, std::pair<int,int> > DBIdToWidthLength;
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
	void AddWidthLength(int DBId, int Width, int Length);
	void AddExtra(int DBId, UnicodeString Extra);
	double GetArea(int DBId);
	int GetDBId(int ItemIndex) { return ItemIndexToDBId[ItemIndex]; }
	int GetItemIndexFromIDDD(int IDDB);
	int GetIDDBFromName(UnicodeString Name);
	UnicodeString GetExtra(int DbId);
	std::pair<int,int> GetWidthLength(int DBId) { return DBIdToWidthLength[DBId]; }
	void AddEdit(int DBId, TEdit* Ed);
	TEdit* GetEdit(int DBId);

	TCoating GetObject(int DBId);
	void Assign(TComboBox* NewCombo);  // Assign InternalItems to NewComboBox;
	void Assign(); //// Assign InternalItems to Combo
	void SetDefaultName(UnicodeString DefaultName);
	void ClearItems();

};

struct TCombos
{
	TComboBoxItems SubstrateCombo;
	TComboBoxItems CoatingsCombo;
	TComboBoxItems SubstrateTypeCombo;
	TComboBoxItems SizeCombo;
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

extern TCombos Combos;

extern TMainEntity Current;

void  ReadDataFromDatabase(AnsiString IDGIP);

void DefectSheetHeader(TStringGrid* Sheet);


void FillUpDefectSheet(int NoOfEntities=0);


void ClearSubstrates();


void DeleteEntity(int EntityID);

void UpdateComment(int EntityID, UnicodeString Comment);

void UpdateEntity(TDefectExpanded Def);

void SelectRow(int EntityID);



void ReadDefectFromDBToSheet(UnicodeString IDGIP);

#endif
