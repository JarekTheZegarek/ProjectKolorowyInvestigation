//---------------------------------------------------------------------------


#pragma hdrstop

#include "Alarms.h"
#include "Global.h"
#include "UnitMain.h"
#include "DataCoater.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)
static AnsiString Database = "Alarms";
static TEdit* Contact ;//Contact person
static TEdit* MTComment;// = FormMain->EditLineStackerMTComment;
static TEdit* Problem; // = FormMain->EditLineStackerProblem;
static TComboBox* Combo; // = FormMain->ComboBoxLineStackerStatus;
static TStringGrid* Sheet; // = FormMain->StringGridLineStacker;
static TDateTimePicker* Picker;

 void DeleteAlarm()
{
    Picker = FormMain->DateTimePickerAlarms;
    Contact = FormMain->EditAlarmWorkDone ;//Contact person
    MTComment = FormMain->EditAlarmMTComment;
    Problem = FormMain->EditAlarmComment;
    Combo = FormMain->ComboBoxAlarmStatus;
    Sheet = FormMain->StringGridAlarms;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       Query->Close();
       Query->SQL->Clear();
	   QueryString = "DELETE FROM "+Database +// :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime   "
                      ;
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
       Query->Parameters->ParamByName("DateTime")->Value=Sheet->Cells[0][Sheet->Selection.Top];//   Picker->DateTime;
       Query->ExecSQL();
       Query->Close();
       FillingUpAlarmsSheet(Current.GetID());
      // EmptyForm(Problem,Contact,MTComment,Combo,Picker);
    }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}


void AddAlarms()
{
//   SetProduction();
   try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
      //
       bool RemarksAreWritten = false;

       // Checking integrity - i.e. if update data or insert new
       // beneath checking of existence;

       Query->SQL->Clear();
       QueryString = "SELECT ID_WorkshiftDay, Comment_DateTime FROM Alarms "
       				 "WHERE  ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime   "
                      ;
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
       Query->Parameters->ParamByName("DateTime")->Value=FormMain->DateTimePickerAlarms->DateTime;
       Query->Open();
//       if(Query->RecordCount!=0)           // Are there remarks for current product?
//            RemarksAreWritten = true;

//       Query->SQL->Clear();
       AnsiString Message;
//           if (Query->RecordCount>1) {
//                 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
//                 Message+=" Isnieje wiêcej ni¿ jeden zapis dla tego samego produktu i tej samej zmiany.";
//                 Message+="\n\n															  ";
//                 Message+="There is mistake in database! There are more than one entity \n";
//                 Message+="for the same product and shift kind.";
//                 Application->MessageBox(Message.c_str(),"Uwaga - Attention!!!",MB_OK);
//                 return;
//           }
       if(Query->RecordCount==1)  // Update as there is given product for one given shift
       {
            if(Application->MessageBox(L"Chcesz zmieniæ zapisane dane? \n\nDo you want to change saved data",L"Uwaga/Attention!!",MB_YESNO)==IDYES)
            {
               Query->SQL->Clear();
               QueryString = " UPDATE Alarms  																"
                      		 " SET Comment = :Comment, Work_Done=:Work_Done, ID_StatusType=:ID_StatusType, 	"
                             "     MT_Comment = :MT_Comment 												"
                      		 " WHERE ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime  	"
                           ;
               Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Parameters->ParamByName("DateTime")->Value=FormMain->DateTimePickerAlarms->DateTime;
               Query->Parameters->ParamByName("Comment")->Value=FormMain->EditAlarmComment->Text;
               Query->Parameters->ParamByName("Work_Done")->Value=FormMain->EditAlarmWorkDone->Text;
               Query->Parameters->ParamByName("ID_StatusType")->Value=FormMain->ComboBoxAlarmStatus->ItemIndex;
               Query->Parameters->ParamByName("MT_Comment")->Value=FormMain->EditAlarmMTComment->Text;
               Query->ExecSQL();
               Query->SQL->Clear();



            }         //------------------   I do not want to upgrade or change any data
            else                 // After Not updating any data
            {
               FillingUpAlarmsSheet(Current.GetID());
               return;
            }
       }   // ---------------------  End of dealing with upgrade   --------------------
       else if(Query->RecordCount==0)  // No data were found - adding new batch
       {
           Query->SQL->Clear();
           QueryString = "INSERT INTO Alarms  "
                  " (Comment, Work_Done, ID_StatusType, MT_Comment,ID_Workshiftday, Comment_DateTime)  "
                  " VALUES   (:Comment, :Work_Done, :ID_StatusType,:MT_Comment, :ID_Workshiftday, :DateTime) "
                  ;
           Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Parameters->ParamByName("DateTime")->Value=FormMain->DateTimePickerAlarms->DateTime;
               Query->Parameters->ParamByName("Comment")->Value=FormMain->EditAlarmComment->Text;
               Query->Parameters->ParamByName("Work_Done")->Value=FormMain->EditAlarmWorkDone->Text;
               Query->Parameters->ParamByName("ID_StatusType")->Value=FormMain->ComboBoxAlarmStatus->ItemIndex;
               Query->Parameters->ParamByName("MT_Comment")->Value=FormMain->EditAlarmMTComment->Text;//ParamByName("YIELD")->Value=CurrentProduction->GetYield();
           Query->ExecSQL();

        }

        FillingUpAlarmsSheet(Current.GetID());

	}
    __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}


void FillingUpAlarmsSheet(int ID)
{
        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        EmptySheet(FormMain->StringGridAlarms);
        AnsiString QueryString,Temp;
        int TempIndex;

      	Query->SQL->Clear();
        QueryString ="SELECT * FROM Alarms "
       				 "WHERE  ID_Workshiftday = :ID_Workshiftday    "
                      ;
                        ;
           Query->SQL->Add(QueryString);
           int IDW = ID; //CurrentProduction->GetID_WorkshiftDay();
           Query->Parameters->ParamByName("ID_Workshiftday")->Value=IDW;
           Query->Open();

           if(Query->RecordCount==0)
           	return;
    	   int i=1;

           while(!Query->Eof)
           {
                FormMain->StringGridAlarms->Cells[0][i]=(Query->FieldByName("Comment_DateTime")->AsString);
                FormMain->StringGridAlarms->Cells[1][i]=(Query->FieldByName("Comment")->AsString);
                FormMain->StringGridAlarms->Cells[2][i]=(Query->FieldByName("Work_Done")->AsString);
                FormMain->StringGridAlarms->Cells[3][i]=FormMain->ComboBoxAlarmStatus->Items->Strings[Query->FieldByName("ID_StatusType")->AsInteger];
                FormMain->StringGridAlarms->Cells[4][i]=(Query->FieldByName("MT_Comment")->AsString);

//                    Temp = ReadP.GetRemarks()->Text;
//                    while((TempIndex=Temp.Pos("\r\n"))!=0)  //Removing Carriage Return from string
//                    {
//                        Temp.Delete(TempIndex,2);
//                        Temp.Insert(" ",TempIndex);
//                    }
//                    FormMain->StringGridProducts->Cells[6][i] = Temp ;

                i++;
                Query->Next();
                if(i>=FormMain->StringGridAlarms->RowCount)
                	FormMain->StringGridAlarms->RowCount=FormMain->StringGridAlarms->RowCount+1;
           }
            EmptyAlarmForm();

            TGridRect R;
            R.Left=1;
            R.Top= i ;
            R.Right= 6 ;
            R.Bottom = i  ;
            FormMain->StringGridAlarms->Selection=R;
            DataModule1->ADOConnectionCreport->Connected=false;
}


void EmptyAlarmForm()
{
    FormMain->EditAlarmComment->Text="";
    FormMain->EditAlarmWorkDone->Text="";
    FormMain->EditAlarmMTComment->Text="";
    FormMain->DateTimePickerAlarms->DateTime.CurrentDateTime();
    FormMain->ComboBoxAlarmStatus->ItemIndex=0;
}

void AlarmsFillUpForm(int Index)
{
    try{


        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        AnsiString QueryString;
       	Query->SQL->Clear();
        QueryString =
                   	 "SELECT * FROM Alarms "
       				 "WHERE  ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime   "

      							;
      Query->SQL->Add(QueryString);
      Query->Parameters->ParamByName("ID_Workshiftday")->Value=Current.GetID();
      Query->Parameters->ParamByName("DateTime")->Value=FormMain->StringGridAlarms->Cells[0][Index];
      Query->Open();


//                FormMain->StringGridAlarms->Cells[0][i]=(Query->FieldByName("Comment_DateTime")->AsString);
//                FormMain->StringGridAlarms->Cells[1][i]=(Query->FieldByName("Comment")->AsString);
//                FormMain->StringGridAlarms->Cells[2][i]=(Query->FieldByName("Work_Done")->AsString);
//                FormMain->StringGridAlarms->Cells[3][i]=FormMain->ComboBoxAlarmStatus->Items->Strings[Query->FieldByName("ID_StatusType")->AsInteger];
//                FormMain->StringGridAlarms->Cells[4][i]=(Query->FieldByName("MT_Comment")->AsString);


    FormMain->EditAlarmComment->Text=(Query->FieldByName("Comment")->AsString);
    FormMain->EditAlarmWorkDone->Text=(Query->FieldByName("Work_Done")->AsString);
    FormMain->EditAlarmMTComment->Text=(Query->FieldByName("MT_Comment")->AsString);
    FormMain->DateTimePickerAlarms->DateTime=(Query->FieldByName("Comment_DateTime")->AsDateTime);
    FormMain->ComboBoxAlarmStatus->ItemIndex=(Query->FieldByName("ID_StatusType")->AsInteger);;


    }   // end of try
    __finally{
        DataModule1->ADOConnectionCreport->Connected=false;
    }
}
