//---------------------------------------------------------------------------


#pragma hdrstop

#include "Global.h"
#include "DataCoater.h"
#include <locale.h>
#include "UnitPacks.h"
#include <map>
#include "UnitMain.h"
std::map<int,TProduction> GridIndexToProduction;
std::map<int,TProducts>  ComboIndexToProduct;
std::map<int,int> ComboLineStatusToID;
//std::map<UnicodeString,char> CurrentSupervisorsGIP;

TSuperviosorCompare  SupervisorsCompare;

TProduction* CurrentProduction=0;
int CurrentId = 0;
TMainEntity Current; // Data of the current shift
TArrayOfProducts ArrayOfProducts;

TCombos Combos;
TOption Options;





void FillingUpProductsSheet(int); //Definied in UnitProduction


void ReadAllDataFromDatabase(int WorkshiftID)
{
            
	try{
        FormMain->LabelSavingWarrning->Visible=true;
        FormMain->Refresh();
        EmptyForms();

        FillingUpProductsSheet(WorkshiftID);

        FillingUpAlarmsSheet(WorkshiftID);

        FillingUpLineStackerSheet(WorkshiftID);

        FillingUpCTControlSheet(WorkshiftID);

        FillingUpWasherSheet(WorkshiftID);

        FillingUpSheetLineStatus();

        ReadDefectsFromDataBase();

        FillingUpGeneralRemarks();

        FillingUpEpuro();
        FillUpFormEpuro();

        FillUpSheetCathodes();
        FillUpFormCathodes();

        FillUpSheetMeasurements();
        FillUpFormMeasurements();

        FillUpSheetSeparator();
        FillUpFormSeparator();

     }
     __finally {
           FormMain->LabelSavingWarrning->Visible=false;
     }
}


void EmptyForms()
{
    EmptyProductionForm();
    EmptyProductionSheet();
    EmptyAlarmForm();
    EmptyLineStackerForm();
    EmptyCTControlForm();
    EmptyWasherForm();
    EmptyFormMeasurements();
    EmptyFormCathodes();
    EmptyFormEpuro();
    EmptyFormSeparator();
}


void EmptySheet(TStringGrid* Sheet)
{
	 for(int i=1;i<=Sheet->RowCount;i++)
		Sheet->Rows[i]->Clear();
}




TDateTime GetShiftTime(int ShiftType, int Kind) //Kind means start=0 stop=1;
{
     //if(StringGridLineStatus->Cells[0][1].IsEmpty())
     //  {
           switch (ShiftType) {

              case 1:
                    	if(Kind ==0)
                        	return TDateTime(6,0,0,0);
                    	else
                            return TDateTime(14,0,0,0);

                        break;
                      //	break;
              case 2:
                    	if(Kind ==0)
              				return TDateTime(14,0,0,0);
                        else
                        	return  TDateTime(22,0,0,0);

              case 3:
                    	if(Kind ==0)
              				return TDateTime(22,0,0,0);
                        else
                        	return TDateTime(6,0,0,0);
                      //	break;

           default:
                	return TDateTime(0,0,0,0);
               ;
           }
}



AnsiString ExchangeAnsStr(AnsiString& A, AnsiString Previous, AnsiString Next)
{
    int Position =  A.Pos(Previous);
    if(Position==0) // Seeking string not found.
    {
        return A;
    }
    int length =  AnsiString(Previous).Length();
    A.Delete(Position,length);

    A.Insert(Next,Position);
    return A;
}

TDateTime StringToDateTime(UnicodeString DateStr, int TypeOfDate)
//Changing specific string into datetime value
// Type of data described:
//   1 - "YYYY/MM/DD hh:mm"
{
  unsigned short year=0, month=0, day=0, hour=0, min=0, sec=0, msec=0;
  int pos;
  UnicodeString Temp, Temp0;
  try {
	   switch (TypeOfDate) {
		   case 1:
				   pos = DateStr.Pos("/");
				   Temp = DateStr.SubString(1,pos-1);
				   year = Temp.ToInt();
				   Temp0 = DateStr.SubString(pos+1,DateStr.Length());
				   pos = Temp0.Pos("/");
				   Temp = Temp0.SubString(1,pos-1);
				   month = Temp.ToInt();
				   Temp = Temp0.SubString(pos+1,2);
				   day =  Temp.ToInt();

				   Temp0 = Temp0.SubString(pos+3,Temp0.Length());
				   pos = Temp0.Pos(":");
				   Temp = Temp0.SubString(1,pos-1);
				   hour = Temp.ToInt();

				   Temp0 = Temp0.SubString(pos+1,Temp0.Length());
				   Temp = Temp0.SubString(1,2);
				   min = Temp.ToInt();
				   break;

		   default:
			 ;
	   }
	   if (year+month+day+hour+min+sec+msec==0)
	   {
		  EConvertError* T = new EConvertError("No valid data was detected");

		  throw *T;
	   }
	   return TDateTime(year,month,day,hour,min,sec,msec);
  } catch (EConvertError& E) {
	 ShowMessage("B³¹d przy dekodowaniu daty (fuction StringToDateTime)"+E.Message);
	 throw E;
  }
}

double AnsiToDouble(AnsiString NrStr)
{
   struct lconv ll;
   struct lconv *conv = &ll;

/* read the locality conversion structure */
   conv = localeconv();
   char DecimalSeparator = *conv->decimal_point;
   if(DecimalSeparator == ',')
     NrStr = ExchangeAnsStr(NrStr,".",DecimalSeparator);
   else
   	 NrStr = ExchangeAnsStr(NrStr,",",DecimalSeparator);
   return NrStr.ToDouble();

}

 //void AddProduction();   // UnitProduction
 //void SetTemporaryYield(); // UnitProduction


//---------------------------------------------------------------------------


UnicodeString ChangeDecimalSeparator(UnicodeString Source)
{
	TFormatSettings Syst =  TFormatSettings::Create(LOCALE_SYSTEM_DEFAULT);
	for(int i=1;i<=Source.Length();i++)
	{
	 if(  (!IsDigit(Source,i) ) && Source[i]!=Syst.DecimalSeparator)
	 {
	   Source[i]=Syst.DecimalSeparator;
	   break;
	 }
	}
	return Source;
}




#pragma package(smart_init)




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

//void TComboBoxItems::AddWidthLength(int DBId, int Width, int Length)

void TComboBoxSizes::AddWidthLength(int DBId, int Width, int Length)
{
	 std::pair<int,int> SizePair;
	 SizePair.first =  Width;
	 SizePair.second = Length;
	 DBIdToWidthLength[DBId] = SizePair;


}

//double TComboBoxItems::GetArea(int DBId)
double TComboBoxSizes::GetArea(int DBId)
{
   double TempArea = static_cast<double> (DBIdToWidthLength[DBId].first * DBIdToWidthLength[DBId].second)/1000000.0;

   if (Options.GetRestrictedLiteArea()) {
	   TempArea = SimpleRoundTo( static_cast<double> (DBIdToWidthLength[DBId].first * DBIdToWidthLength[DBId].second)/1000000.0,-2);
   }
   return TempArea ;
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

//void TCoating::SetID(int ID)
// {
//	 ID_ProdType = ID;
// }
//
// int TCoating::GetID()
// {
//	 return ID_ProdType;
// }
//
//void TCoating::SetShortName(AnsiString Name)
//{
//	ProdType_Name = Name;
//}
//
//void TCoating::SetFullName(AnsiString Name)
//{
//	FullName = Name;
//}
//
//TCoating::TCoating():ID_ProdType(0), Remarks(0)
//{
//   //	Remarks = 0;
//}
//
//void TCoating::SetRemarks(TStringList* R)
//{
//	if(Remarks!=0)
//		delete Remarks;
//	Remarks = new TStringList();
//	Remarks->Text = R->Text;
//}
//
//TCoating::~TCoating()
//{
////   if(Remarks!=0)
////    delete Remarks;
//}
//
//AnsiString TCoating::GetFullName()
//{
//  return FullName;
//}
//
//AnsiString TCoating::GetShortName()
//{
//	return ProdType_Name;
//}
//
//AnsiString TCoating::GetCombinedName()
//{
//   AnsiString Tym;
//   Tym = "("+ProdType_Name+") "+FullName;
//   return Tym;
//}
//
//TStringList* TCoating::GetRemarks()
// {
//	return Remarks;
// }
//
//
//void TCoating::operator=(TCoating Source)
//{
//	ID_ProdType = Source.ID_ProdType;
//	ProdType_Name= Source.ProdType_Name;
//	FullName = Source.FullName;
//
//	if(Remarks != 0)
//	{
//		delete Remarks;
//	}
//	if (Source.Remarks==0) {
//	  Remarks = 0;
//	  return;
//	}
//	Remarks = new TStringList;
//	Remarks->AddStrings(Source.Remarks);
//}
//
//
//bool TCoating::operator==(const TCoating& Coat)
//{
//   return (ID_ProdType == Coat.ID_ProdType);
//}
