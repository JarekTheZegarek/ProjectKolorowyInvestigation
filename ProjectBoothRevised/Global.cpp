//---------------------------------------------------------------------------


#pragma hdrstop

#include "Global.h"
#include "DataBooth.h"
#include <locale.h>

std::map<int,TProduction> GridIndexToProduction;
std::map<int,TProducts>  ComboIndexToProduct;
std::map<int,int> ComboLineStatusToID;
//std::map<UnicodeString,char> CurrentSupervisorsGIP;

TSuperviosorCompare  SupervisorsCompare;

TProduction* CurrentProduction=0;
int CurrentId = 0;
TMainEntity Current; // Data of the current shift
TArrayOfProducts ArrayOfProducts;


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



#pragma package(smart_init)
