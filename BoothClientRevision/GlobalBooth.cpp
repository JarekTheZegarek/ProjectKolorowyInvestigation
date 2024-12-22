//---------------------------------------------------------------------------

#pragma hdrstop

#include "GlobalBooth.h"
#include "UnitMainBooth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)

TCombos Combos;


TComboBoxItems::TComboBoxItems(TComboBox* AppropriateCombo):Combo(AppropriateCombo)
{
	InternalItems = new TStringList();
}

TComboBoxItems::TComboBoxItems(): Combo(0)
{
	InternalItems = new TStringList();
}

UnicodeString TComboBoxItems::GetNameByItemIndex(int ItemIndex)
{
   int DBId = this->ItemIndexToDBId[ItemIndex];
   return this->DBIdToName[DBId];
}

UnicodeString TComboBoxItems::GetNameByDBId(int DBId)
{
   return this->DBIdToName[DBId];
}

int TComboBoxItems::AddItem(UnicodeString Name, int DBId) //return ItemIndex
{
	 this->DBIdToName[DBId]=Name;
	 int ItemIndex = InternalItems->Add(Name);

	 ItemIndexToDBId[ItemIndex]=DBId;
	 return ItemIndex;
}

void TComboBoxItems::Assign(TComboBox* NewCombo)
{
	 Combo = NewCombo;
	 Assign();
}

void TComboBoxItems::Assign()
{
     Combo->Items->Clear();
	 Combo->Items->AddStrings(InternalItems);
}

void TComboBoxItems::ClearItems()
{
  InternalItems->Clear();
  ItemIndexToDBId.clear();
  DBIdToName.clear();
  DBIdToObject.clear();
}

void TComboBoxItems::SetDefaultName(UnicodeString DefaultName)
{
	int IndexFound;
	if(Combo) //if Combo is not null
	{
	  IndexFound = InternalItems->IndexOf(DefaultName);
	  if(IndexFound!=-1)
		 Combo->ItemIndex = IndexFound;
	}
}

void TComboBoxItems::AddObject(int DBId, TCoating SomeObject)
{
  DBIdToObject[DBId]= SomeObject;
}

TCoating TComboBoxItems::GetObject(int DBId)
{
   return DBIdToObject[DBId];
}

void TComboBoxItems::AddWidthLength(int DBId, int Width, int Length)
{
	 std::pair<int,int> SizePair;
	 SizePair.first =  Width;
	 SizePair.second = Length;
     DBIdToWidthLength[DBId] = SizePair;


}

double TComboBoxItems::GetArea(int DBId)
{
   return static_cast<double> (DBIdToWidthLength[DBId].first * DBIdToWidthLength[DBId].second)/1000000.0 ;
}


void TComboBoxItems::AddExtra(int DBId, UnicodeString Extra)
{
	DBIdToExtra[DBId]=Extra;
}

UnicodeString  TComboBoxItems::GetExtra(int DBId)
{
	return DBIdToExtra[DBId];
}

void TComboBoxItems::AddEdit(int DBId, TEdit* Ed)
{
   DBIdToEdit[DBId]=Ed;
}


TEdit* TComboBoxItems::GetEdit(int DBId)
{
	return DBIdToEdit[DBId];
}


void ClearSheet(TStringGrid* S)
{
	for(int i=0;i<S->ColCount;i++)
		for(int j=0;j<S->RowCount;j++)
		{
			S->Cells[i][j]="";
		}
}

void DefectSheetHeader(TStringGrid* Sheet)
{
   Sheet->Cells[0][0] ="Data    ";
   Sheet->Cells[1][0] ="Coating ";
   Sheet->Cells[2][0] ="Rozmiar ";
   Sheet->Cells[3][0] ="Nazwa Defektu ";
   Sheet->Cells[4][0] ="Dodatkowe szczegó³y ";
   Sheet->Cells[5][0] ="Iloœæ ";
   Sheet->Cells[6][0] ="Powierzchnia ";
   Sheet->Cells[7][0] = "Typ szk³a ";
   Sheet->Cells[8][0] = "Rodzaj szk³a ";
   Sheet->Cells[9][0] = "Gruboœæ ";
   Sheet->Cells[10][0] = "Szerokoœæ ";
   Sheet->Cells[11][0] ="D³ugoœæ ";
   Sheet->Cells[12][0] ="Uwagi ";
   int SheetWidth=0;
	for(int i=0;i<Sheet->ColCount-1;i++)
	{
	   Sheet->ColWidths[i]=	Sheet->Canvas->TextWidth(Sheet->Cells[i][0])+25;
	   SheetWidth += Sheet->ColWidths[i];
	}
	Sheet->ColWidths[Sheet->ColCount-1]= Sheet->ClientWidth - SheetWidth;
}

int TComboBoxItems::GetItemIndexFromIDDD(int IDDB)
{
	std::map<int, int>::iterator it;
	it = ItemIndexToDBId.begin();
	while(it!=ItemIndexToDBId.end())
	{
	   if(it->second==IDDB)
			return it->first;
	   it++;
	}
	return 0;
}

int  TComboBoxItems::GetIDDBFromName(UnicodeString Name)
{
   std::map<int, UnicodeString>::iterator it;
	it = DBIdToName.begin();
	while(it!=DBIdToName.end())
	{
	   if(it->second==Name)
			return it->first;
	   it++;
	}
	return 0;
}
