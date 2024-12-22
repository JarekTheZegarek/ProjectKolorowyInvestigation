//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitSeparator.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"
#include <systdate.h>
#include <map>
//---------------------------------------------------------------------------

#pragma package(smart_init)


static AnsiString Database = "Puder_Box_Measurement";
static AnsiString Database2 = "";
static AnsiString DataQuery = "";
static TADOQuery* QueryFill;
static TMemo* Remarks;
static TStringGrid* Sheet;


void EmptySheetSeparator()
{
   Sheet = FormMain->StringGridSeparator;
   for(int i=1;i<=Sheet->RowCount;i++)
    	Sheet->Rows[i]->Clear();
}








bool CheckIfExistsSeparator()
{
	try{
           AnsiString QueryString, Name;
           int ID=Current.GetID(), NoOfRows;

           TADOQuery* Query = DataModule1->ADOQueryCreport;
           DataModule1->ADOConnectionCreport->Connected=true;

           Query->Close();
           Query->SQL->Clear();

           QueryString = "SELECT * FROM " + Database + " WHERE ID_WorkShiftDay = :ID";
           Query->SQL->Add(QueryString);
           Query->Parameters->ParamByName("ID")->Value=ID;
           Query->Open();

           if(Query->RecordCount==0)
           {
               return false;
           }
           else
           {
               return true;
           }
    }
    __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

bool DeleteSeparator()
{
   try{
           AnsiString QueryString, Name;
           int ID=Current.GetID(), NoOfRows;

           TADOQuery* Query = DataModule1->ADOQueryCreport;
           DataModule1->ADOConnectionCreport->Connected=true;

           Query->Close();
           Query->SQL->Clear();

           QueryString = "DELETE FROM " + Database + " WHERE ID_WorkShiftDay = :ID";
           Query->SQL->Add(QueryString);
           Query->Parameters->ParamByName("ID")->Value=ID;
           NoOfRows=Query->ExecSQL();

           if(NoOfRows==0)
           {
               return false;
           }
           else
           {
               
               return true;
           }
           
    }
    __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}




void AddSeparator()
{
    EmptySheetSeparator();
    Sheet = FormMain->StringGridSeparator;
    
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       int Quantity,i=1,j=1;

       if(CheckIfExistsSeparator())
       {
           DeleteSeparator();     //clear before saving
       }

       		Query->Close();
            Query->SQL->Clear();
            QueryString=	" INSERT INTO " + Database + " "
       	               		" (ID_WorkShiftDay "
                            " , Start_Date_Time, End_Date_Time, Start_Puder_Box_Type "
                            " , End_Puder_Box_Type, Start_Setup, End_Setup  "
							" , Start_VS, End_VS, Start_CS, End_CS, Start_PS, End_PS,Start_Setup_PS,End_Setup_PS) "
                       		" VALUES       "
							" (:ID_WorkShiftDay "
                             " , :Start_Date_Time, :End_Date_Time, :Start_Puder_Box_Type "
                            " , :End_Puder_Box_Type, :Start_Setup, :End_Setup  "
							" , :Start_VS, :End_VS, :Start_CS, :End_CS, :Start_PS, :End_PS,:Start_Setup_PS,:End_Setup_PS) "
                    		;
          //  ExchangeAnsStr(QueryString,":Defect","["+ReturnField(Defect)+"]");
        	Query->SQL->Add(QueryString);
            Query->Parameters->ParamByName("ID_WorkShiftDay")->Value=Current.GetID();
        	Query->Parameters->ParamByName("Start_Date_Time")->Value=FormMain->DateTimePickerSeparator->DateTime;
            Query->Parameters->ParamByName("End_Date_Time")->Value=FormMain->DateTimePickerSeparatorChange->DateTime;
            Query->Parameters->ParamByName("Start_Puder_Box_Type")->Value=FormMain->ComboBoxSeparatorStart->Text;
            Query->Parameters->ParamByName("End_Puder_Box_Type")->Value=FormMain->ComboBoxSeparatorStop->Text;
			Query->Parameters->ParamByName("Start_Setup")->Value=FormMain->EditSeparatorSettingsStart->Text.ToInt();
			Query->Parameters->ParamByName("End_Setup")->Value=FormMain->EditSeparatorSettingsStop->Text.ToInt();
			Query->Parameters->ParamByName("Start_Setup_PS")->Value=FormMain->EditSeparatorSettingsStart2->Text.ToInt();
			Query->Parameters->ParamByName("End_Setup_PS")->Value=FormMain->EditSeparatorSettingsStop2->Text.ToInt();
            Query->Parameters->ParamByName("Start_VS")->Value=FormMain->EditSeparatorVSStart->Text.ToInt();
            Query->Parameters->ParamByName("End_VS")->Value=FormMain->EditSeparatorVSStop->Text.ToInt();
            Query->Parameters->ParamByName("Start_CS")->Value=FormMain->EditSeparatorCEStart->Text.ToInt();
            Query->Parameters->ParamByName("End_CS")->Value=FormMain->EditSeparatorCEStop->Text.ToInt();
            Query->Parameters->ParamByName("Start_PS")->Value=FormMain->EditSeparatorPSStart->Text.ToInt();
            Query->Parameters->ParamByName("End_PS")->Value=FormMain->EditSeparatorPSStop->Text.ToInt();

            Query->ExecSQL();
//        	if (NoOfRows!=0) {
//                return true;
//            }
//            else
//            {
//                return false;
//            }


        FillUpSheetSeparator();

     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}


void FillUpSheetSeparator()
{
    EmptySheetSeparator();
    Sheet = FormMain->StringGridSeparator;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;



       		Query->Close();
            Query->SQL->Clear();
            QueryString = "SELECT * FROM " + Database + " WHERE ID_WorkShiftDay = :ID";
          //  ExchangeAnsStr(QueryString,":Defect","["+ReturnField(Defect)+"]");

            Query->SQL->Add(QueryString);
            Query->Parameters->ParamByName("ID")->Value=Current.GetID();
            Query->Open();

            if(Query->RecordCount==0)
                	return;   // if there are no results

			UnicodeString TempString;
			Sheet->Cells[0][1]=Query->FieldByName("Start_Date_Time")->AsDateTime;
			Sheet->Cells[1][1]=Query->FieldByName("End_Date_Time")->AsDateTime;
			Sheet->Cells[0][2]=Query->FieldByName("Start_Puder_Box_Type")->AsString;
			Sheet->Cells[1][2]=Query->FieldByName("End_Puder_Box_Type")->AsString;

			TempString = Query->FieldByName("Start_Setup")->AsString;
			TempString += " - ";
			TempString += Query->FieldByName("Start_Setup_PS")->AsString;
			Sheet->Cells[0][3]= TempString; //Query->FieldByName("Start_Setup")->AsString;

			TempString = Query->FieldByName("End_Setup")->AsString;
			TempString += " - ";
			TempString += Query->FieldByName("End_Setup_PS")->AsString;
			Sheet->Cells[1][3]= TempString;

			//Sheet->Cells[1][3]=Query->FieldByName("End_Setup")->AsString;
            Sheet->Cells[0][4]=Query->FieldByName("Start_VS")->AsString;
            Sheet->Cells[1][4]=Query->FieldByName("End_VS")->AsString;
            Sheet->Cells[0][5]=Query->FieldByName("Start_CS")->AsString;
            Sheet->Cells[1][5]=Query->FieldByName("End_CS")->AsString;
            Sheet->Cells[0][6]=Query->FieldByName("Start_PS")->AsString;
            Sheet->Cells[1][6]=Query->FieldByName("End_PS")->AsString;




     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }

}

void FillUpFormSeparator()
{
  // Remarks = FormMain->MemoCathodes;
  //  Sheet = FormMain->StringGridCathodes;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;



       		Query->Close();
            Query->SQL->Clear();
            QueryString = "SELECT * FROM "+ Database + " WHERE ID_WorkShiftDay = :ID";
          //  ExchangeAnsStr(QueryString,":Defect","["+ReturnField(Defect)+"]");

            Query->SQL->Add(QueryString);
            Query->Parameters->ParamByName("ID")->Value=Current.GetID();
            Query->Open();

            if(Query->RecordCount==0)
                	return;   // if there are no results


        	FormMain->DateTimePickerSeparator->DateTime=Query->FieldByName("Start_Date_Time")->AsDateTime;
            FormMain->DateTimePickerSeparatorChange->DateTime=Query->FieldByName("End_Date_Time")->AsDateTime;
            FormMain->ComboBoxSeparatorStart->Text=Query->FieldByName("Start_Puder_Box_Type")->AsString;
            FormMain->ComboBoxSeparatorStop->Text=Query->FieldByName("End_Puder_Box_Type")->AsString;
			FormMain->EditSeparatorSettingsStart->Text=Query->FieldByName("Start_Setup")->AsString;
			FormMain->EditSeparatorSettingsStop->Text=Query->FieldByName("End_Setup")->AsString;
			FormMain->EditSeparatorSettingsStart2->Text=Query->FieldByName("Start_Setup_PS")->AsString;
			FormMain->EditSeparatorSettingsStop2->Text=Query->FieldByName("End_Setup_PS")->AsString;

			FormMain->EditSeparatorVSStart->Text=Query->FieldByName("Start_VS")->AsString;
            FormMain->EditSeparatorVSStop->Text=Query->FieldByName("End_VS")->AsString;
            FormMain->EditSeparatorCEStart->Text=Query->FieldByName("Start_CS")->AsString;
            FormMain->EditSeparatorCEStop->Text=Query->FieldByName("End_CS")->AsString;
            FormMain->EditSeparatorPSStart->Text=Query->FieldByName("Start_PS")->AsString;
            FormMain->EditSeparatorPSStop->Text=Query->FieldByName("End_PS")->AsString;


//        	if (NoOfRows!=0) {
//                return true;
//            }
//            else
//            {
//                return false;
//            }




     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }

}


void EmptyFormSeparator()
{
        	FormMain->DateTimePickerSeparator->DateTime.CurrentDateTime();
            FormMain->DateTimePickerSeparatorChange->DateTime.CurrentDateTime();
            FormMain->ComboBoxSeparatorStart->Text="";//Query->FieldByName("Start_Puder_Box_Type")->AsString;
            FormMain->ComboBoxSeparatorStop->Text="";//Query->FieldByName("End_Puder_Box_Type")->AsString;
			FormMain->EditSeparatorSettingsStart->Text=0;//Query->FieldByName("Start_Setup")->AsString;
			FormMain->EditSeparatorSettingsStop2->Text=0;//Query->FieldByName("End_Setup")->AsString;
			FormMain->EditSeparatorSettingsStart2->Text=0;//Query->FieldByName("Start_Setup")->AsString;
			FormMain->EditSeparatorSettingsStop->Text=0;//Query->FieldByName("End_Setup")->AsString;
			FormMain->EditSeparatorVSStart->Text=0;//Query->FieldByName("Start_VS")->AsString;
            FormMain->EditSeparatorVSStop->Text=0;//Query->FieldByName("End_VS")->AsString;
            FormMain->EditSeparatorCEStart->Text=0;//Query->FieldByName("Start_CS")->AsString;
            FormMain->EditSeparatorCEStop->Text=0;//Query->FieldByName("End_CS")->AsString;
            FormMain->EditSeparatorPSStart->Text=0;//Query->FieldByName("Start_PS")->AsString;
            FormMain->EditSeparatorPSStop->Text=0;//Query->FieldByName("End_PS")->AsString;
}
