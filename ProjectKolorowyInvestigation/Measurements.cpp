//---------------------------------------------------------------------------


#pragma hdrstop

#include "Measurements.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"
#include <systdate.h>
#include <map>
//---------------------------------------------------------------------------

#pragma package(smart_init)


//---------------------------------------------------------------------------




static AnsiString Database = "Measurement";
static AnsiString Database2 = "";
static AnsiString DataQuery = "";
static TADOQuery* QueryFill;
static TMemo* Remarks;
static TStringGrid* Sheet;
//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);




void EmptySheetMeasurements()
{
   Sheet = FormMain->StringGridMeasurements;
   for(int i=1;i<=Sheet->RowCount;i++)
    	Sheet->Rows[i]->Clear();
}








bool CheckIfExistsMeasurements()
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

bool DeleteMeasurements()
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
		   FillUpSheetMeasurements();
		   FillUpFormMeasurements();
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




void AddMeasurements()
{
    EmptySheetMeasurements();
 //   Remarks = FormMain->MemoMeasurementsHunterBottom;
    Sheet = FormMain->StringGridMeasurements;
    
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       int Quantity,i=1,j=1;

       if(CheckIfExistsMeasurements())
       {
           DeleteMeasurements();     //clear before saving
       }

       		Query->Close();
            Query->SQL->Clear();
            QueryString=	" INSERT INTO " + Database + " "
       	               		" (ID_WorkShiftDay, "
                            " Optoplex_Y, Optoplex_A, Optoplex_B "
                            " , Hunter_Top_Y, Hunter_Top_A, Hunter_Top_B "
                            " , [Hunter_ Bottom_Y], [Hunter_ Bottom_A], [Hunter_ Bottom_B] "
                            " , Optoplex_Comment, Hunter_Top_Comment, [Hunter_ Bottom_Comment] )"
                       		" VALUES       "
							" (:ID_WorkShiftDay, "
                            " :Optoplex_Y, :Optoplex_A, :Optoplex_B "
                            " , :Hunter_Top_Y, :Hunter_Top_A, :Hunter_Top_B "
                            " , :Hunter_Bottom_Y, :Hunter_Bottom_A, :Hunter_Bottom_B "
                            " , :Optoplex_Comment, :Hunter_Top_Comment, :Hunter_Bottom_Comment )"
                    		;
          //  ExchangeAnsStr(QueryString,":Defect","["+ReturnField(Defect)+"]");
        	Query->SQL->Add(QueryString);
            Query->Parameters->ParamByName("ID_WorkShiftDay")->Value=Current.GetID();
        	Query->Parameters->ParamByName("Optoplex_Y")->Value=AnsiToDouble(FormMain->EditMeasurementsOptoY->Text);
            Query->Parameters->ParamByName("Optoplex_A")->Value=AnsiToDouble(FormMain->EditMeasurementsOptoA->Text);
            Query->Parameters->ParamByName("Optoplex_B")->Value=AnsiToDouble(FormMain->EditMeasurementsOptoB->Text);
            Query->Parameters->ParamByName("Hunter_Top_Y")->Value=AnsiToDouble(FormMain->EditMeasurementsHunterFY->Text);
            Query->Parameters->ParamByName("Hunter_Top_A")->Value=AnsiToDouble(FormMain->EditMeasurementsHunterFA->Text);
            Query->Parameters->ParamByName("Hunter_Top_B")->Value=AnsiToDouble(FormMain->EditMeasurementsHunterFB->Text);
            Query->Parameters->ParamByName("Hunter_Bottom_Y")->Value=AnsiToDouble(FormMain->EditMeasurementsHunterGY->Text);
            Query->Parameters->ParamByName("Hunter_Bottom_A")->Value=AnsiToDouble(FormMain->EditMeasurementsHunterGA->Text);
            Query->Parameters->ParamByName("Hunter_Bottom_B")->Value=AnsiToDouble(FormMain->EditMeasurementsHunterGB->Text);
            Query->Parameters->ParamByName("Optoplex_Comment")->Value=FormMain->MemoMeasurementsOpto->Lines->Text;
            Query->Parameters->ParamByName("Hunter_Top_Comment")->Value=FormMain->MemoMeasurementsHunterTop->Lines->Text;
            Query->Parameters->ParamByName("Hunter_Bottom_Comment")->Value=FormMain->MemoMeasurementsHunterBottom->Lines->Text;

            Query->ExecSQL();
//        	if (NoOfRows!=0) {
//                return true;
//            }
//            else
//            {
//                return false;
//            }


        FillUpSheetMeasurements();

     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}


void FillUpSheetMeasurements()
{
    EmptySheetMeasurements();
    Sheet = FormMain->StringGridMeasurements;
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

            Sheet->Cells[0][1]=Query->FieldByName("Optoplex_Y")->AsFloat;
            Sheet->Cells[0][2]=Query->FieldByName("Optoplex_A")->AsFloat;
            Sheet->Cells[0][3]=Query->FieldByName("Optoplex_B")->AsFloat;
            Sheet->Cells[1][1]=Query->FieldByName("Hunter_Top_Y")->AsFloat;
            Sheet->Cells[1][2]=Query->FieldByName("Hunter_Top_A")->AsFloat;
            Sheet->Cells[1][3]=Query->FieldByName("Hunter_Top_B")->AsFloat;
            Sheet->Cells[2][1]=Query->FieldByName("Hunter_ Bottom_Y")->AsFloat;
            Sheet->Cells[2][2]=Query->FieldByName("Hunter_ Bottom_A")->AsFloat;
            Sheet->Cells[2][3]=Query->FieldByName("Hunter_ Bottom_B")->AsFloat;
            Sheet->Cells[0][4]=Query->FieldByName("Optoplex_Comment")->AsString;
            Sheet->Cells[1][4]=Query->FieldByName("Hunter_Top_Comment")->AsString;
            Sheet->Cells[2][4]=Query->FieldByName("Hunter_ Bottom_Comment")->AsString;
     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }

}

void FillUpFormMeasurements()
{
   //Remarks = FormMain->MemoCathodes;
   // Sheet = FormMain->StringGridCathodes;
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
			{
				 FormMain->EditMeasurementsOptoY->Text="";
				 FormMain->EditMeasurementsOptoA->Text="";
				 FormMain->EditMeasurementsOptoB->Text="";
				 FormMain->EditMeasurementsHunterFY->Text="";
				 FormMain->EditMeasurementsHunterFA->Text="";
				 FormMain->EditMeasurementsHunterFB->Text="";
				 FormMain->EditMeasurementsHunterGY->Text="";
				 FormMain->EditMeasurementsHunterGA->Text="";
				 FormMain->EditMeasurementsHunterGB->Text="";
				 FormMain->MemoMeasurementsOpto->Lines->Text="";
				 FormMain->MemoMeasurementsHunterTop->Lines->Text="";
				 FormMain->MemoMeasurementsHunterBottom->Lines->Text="";

				 return;   // if there are no results
			}

			FormMain->EditMeasurementsOptoY->Text=Query->FieldByName("Optoplex_Y")->AsFloat;
			FormMain->EditMeasurementsOptoA->Text=Query->FieldByName("Optoplex_A")->AsFloat;
			FormMain->EditMeasurementsOptoB->Text=Query->FieldByName("Optoplex_B")->AsFloat;
			FormMain->EditMeasurementsHunterFY->Text=Query->FieldByName("Hunter_Top_Y")->AsFloat;
			FormMain->EditMeasurementsHunterFA->Text=Query->FieldByName("Hunter_Top_A")->AsFloat;
			FormMain->EditMeasurementsHunterFB->Text=Query->FieldByName("Hunter_Top_B")->AsFloat;
			FormMain->EditMeasurementsHunterGY->Text=Query->FieldByName("Hunter_ Bottom_Y")->AsFloat;
			FormMain->EditMeasurementsHunterGA->Text=Query->FieldByName("Hunter_ Bottom_A")->AsFloat;
			FormMain->EditMeasurementsHunterGB->Text=Query->FieldByName("Hunter_ Bottom_B")->AsFloat;
			FormMain->MemoMeasurementsOpto->Lines->Text=Query->FieldByName("Optoplex_Comment")->AsString;
			FormMain->MemoMeasurementsHunterTop->Lines->Text=Query->FieldByName("Hunter_Top_Comment")->AsString;
			FormMain->MemoMeasurementsHunterBottom->Lines->Text=Query->FieldByName("Hunter_ Bottom_Comment")->AsString;


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


void EmptyFormMeasurements()
{

        	FormMain->EditMeasurementsOptoY->Text="";//Query->FieldByName("Optoplex_Y")->AsFloat;
            FormMain->EditMeasurementsOptoA->Text="";//Query->FieldByName("Optoplex_A")->AsFloat;
            FormMain->EditMeasurementsOptoB->Text="";//Query->FieldByName("Optoplex_B")->AsFloat;
            FormMain->EditMeasurementsHunterFY->Text="";//Query->FieldByName("Hunter_Top_Y")->AsFloat;
            FormMain->EditMeasurementsHunterFA->Text="";//Query->FieldByName("Hunter_Top_A")->AsFloat;
            FormMain->EditMeasurementsHunterFB->Text="";//Query->FieldByName("Hunter_Top_B")->AsFloat;
            FormMain->EditMeasurementsHunterGY->Text="";//Query->FieldByName("Hunter_ Bottom_Y")->AsFloat;
            FormMain->EditMeasurementsHunterGA->Text="";//Query->FieldByName("Hunter_ Bottom_A")->AsFloat;
            FormMain->EditMeasurementsHunterGB->Text="";//Query->FieldByName("Hunter_ Bottom_B")->AsFloat;
            FormMain->MemoMeasurementsOpto->Lines->Text="";//Query->FieldByName("Optoplex_Comment")->AsString;
            FormMain->MemoMeasurementsHunterTop->Lines->Text="";//Query->FieldByName("Hunter_Top_Comment")->AsString;
            FormMain->MemoMeasurementsHunterBottom->Lines->Text="";//Query->FieldByName("Hunter_ Bottom_Comment")->AsString;








}


