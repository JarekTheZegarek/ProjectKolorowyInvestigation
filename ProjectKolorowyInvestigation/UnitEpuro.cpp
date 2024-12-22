//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitEpuro.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"
#include <systdate.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

static AnsiString Database = "Epuro_Station";
static AnsiString Database2 ="Washer_Status";
static TMemo* Remarks;

//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);



void DeleteEpuro()
{
    

   // Remarks = FormMain->MemoGeneralRemarks;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       Query->SQL->Clear();
	   QueryString = " DELETE FROM   " + Database +
               		 " WHERE ID_Workshiftday = :ID_Workshiftday "
                           ;
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();

       DataModule1->ADOConnectionCreport->BeginTrans();

               Query->ExecSQL();
               Query->Close();
               Query->SQL->Clear();

               QueryString = " DELETE FROM   " + Database2 +
               		 " WHERE ID_Workshiftday = :ID_Workshiftday "
                           ;
               Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->ExecSQL();
               Query->Close();
               Query->SQL->Clear();

       DataModule1->ADOConnectionCreport->CommitTrans();

       FillingUpEpuro();
       FillUpFormEpuro();
    }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

void AddEpuro()
{
    TEdit* AD1=FormMain->EditEpuroAD1 ;
    TEdit* AD2=FormMain->EditEpuroAD2 ;
    TEdit* MB=FormMain->EditEpuroMB;
    TEdit* R1=FormMain->EditEpuroR1;
    TEdit* R2=FormMain->EditEpuroR2;
    TEdit* Edi=FormMain->EditEpuroEdi;
    TEdit* StartSpeed = FormMain->EditStartWasherSpeed;
    TEdit* StopSpeed=FormMain->EditStopWasherSpeed;
    bool EpuroWritten = false;
    bool WasherWritten = false;

//   SetProduction();
try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;

       // Checking integrity - i.e. if update data or insert new
       // beneath checking of existence;

       AnsiString Message;


            	Query->Close();  //Updating in this case means Deleting and inserting
               	Query->SQL->Clear();
                QueryString = "SELECT * FROM   " + Database +
                      		 " WHERE ID_Workshiftday = :ID_Workshiftday "
                           ;
               Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Open();

        if(Query->RecordCount>0)
 		{
           EpuroWritten = true;
        }
         		Query->Close();  //Updating in this case means Deleting and inserting
               	Query->SQL->Clear();
                QueryString = "SELECT * FROM   " + Database2 +
                      		 " WHERE ID_Workshiftday = :ID_Workshiftday "
                           ;
               Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Open();
        if(Query->RecordCount>0)
 		{
           WasherWritten = true;
        }

        if(WasherWritten || EpuroWritten)
        {
          if(Application->MessageBox(L"Chcesz zmieniæ zapisane dane? \n\nDo you want to change saved data",L"Uwaga / Attention!!",MB_YESNO)!=IDYES)
           {
            	return;
           }
        }
        DataModule1->ADOConnectionCreport->BeginTrans();
        if (EpuroWritten)
        {

               Query->Close();  //Updating in this case means Deleting and inserting
               Query->SQL->Clear();

			   QueryString = "DELETE FROM   " + Database +
                      		 " WHERE ID_Workshiftday = :ID_Workshiftday "
                           ;
               Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();

               Query->ExecSQL();
               Query->Close();
               Query->SQL->Clear();

                   QueryString =" INSERT INTO   " + Database +
                                        " (ID_Workshiftday, MB, Edi, R1, R2, AD1, AD2) "
                                        " VALUES (:ID_Workshiftday, :MB, :Edi, :R1, :R2, :AD1, :AD2) "
                                                                   ;
                   Query->SQL->Add(QueryString);

                   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
                   Query->Parameters->ParamByName("MB")->Value= AnsiToDouble(MB->Text);
                   Query->Parameters->ParamByName("Edi")->Value=AnsiToDouble(Edi->Text);
                   Query->Parameters->ParamByName("R1")->Value= AnsiToDouble(R1->Text);
                   Query->Parameters->ParamByName("R2")->Value= AnsiToDouble(R2->Text);
                   Query->Parameters->ParamByName("AD1")->Value= AnsiToDouble(AD1->Text);
                   Query->Parameters->ParamByName("AD2")->Value= AnsiToDouble(AD2->Text);
                   Query->ExecSQL();
                   Query->Close();
                   Query->SQL->Clear();
          }
          if(WasherWritten)
          {
                   QueryString =" UPDATE " + Database2 +
                                        " SET Start_Speed = :Start_Speed, End_Speed= :End_Speed "
                                        " WHERE ID_Workshiftday = :ID_Workshiftday "
                                         ;
                   Query->SQL->Add(QueryString);

                   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
                   Query->Parameters->ParamByName("Start_Speed")->Value= AnsiToDouble(StartSpeed->Text);
                   Query->Parameters->ParamByName("End_Speed")->Value= AnsiToDouble(StopSpeed->Text);
                   Query->ExecSQL();
                   Query->Close();
           }
           DataModule1->ADOConnectionCreport->CommitTrans();
          DataModule1->ADOConnectionCreport->BeginTrans();
          if(!EpuroWritten)
          {
                   Query->SQL->Clear();

                   QueryString =        " INSERT INTO   " + Database +
                   						" (ID_Workshiftday, MB, Edi, R1, R2, AD1, AD2) "
                                        " VALUES (:ID_Workshiftday, :MB, :Edi, :R1, :R2, :AD1, :AD2) "
                                                                   ;
                   Query->SQL->Add(QueryString);

                   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
                   Query->Parameters->ParamByName("MB")->Value= AnsiToDouble(MB->Text);
                   Query->Parameters->ParamByName("Edi")->Value= AnsiToDouble(Edi->Text);
                   Query->Parameters->ParamByName("R1")->Value= AnsiToDouble(R1->Text);
                   Query->Parameters->ParamByName("R2")->Value= AnsiToDouble(R2->Text);
                   Query->Parameters->ParamByName("AD1")->Value= AnsiToDouble(AD1->Text);
                   Query->Parameters->ParamByName("AD2")->Value= AnsiToDouble(AD2->Text);

                   Query->ExecSQL();
                   Query->Close();
                   Query->SQL->Clear();
          }
          if(!WasherWritten)
          {
                   QueryString =		" INSERT INTO " + Database2 +
                                        " (ID_Workshiftday, Start_Speed, End_Speed)  "
                                        " VALUES (  :ID_Workshiftday,  :Start_Speed,  :End_Speed ) "
                                         ;
                   Query->SQL->Add(QueryString);

                   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
                   Query->Parameters->ParamByName("Start_Speed")->Value= AnsiToDouble(StartSpeed->Text);
                   Query->Parameters->ParamByName("End_Speed")->Value= AnsiToDouble(StopSpeed->Text);
                   Query->ExecSQL();
                   Query->Close();
    		}
            DataModule1->ADOConnectionCreport->CommitTrans();
           FillingUpEpuro();
	}
    __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}




 void FillingUpEpuro()
 {

    TEdit* AD1_read=FormMain->EditEpuroAD1_read ;
    TEdit* AD2_read=FormMain->EditEpuroAD2_read ;
    TEdit* MB_read=FormMain->EditEpuroMB_read;
    TEdit* R1_read=FormMain->EditEpuroR1_read;
    TEdit* R2_read=FormMain->EditEpuroR2_read;
    TEdit* Edi_read=FormMain->EditEpuroEdi_read;
    TEdit* StartSpeed_read = FormMain->EditEpuroStartSpeed_read;
    TEdit* StopSpeed_read=FormMain->EditEpuroStopSpeed_read;

    try{
       MB_read->Text ="";
       Edi_read->Text ="";
       R1_read->Text  ="";
       R2_read->Text  ="";
       AD1_read->Text  ="";
       AD2_read->Text  ="";
       StartSpeed_read->Text  ="";
       StopSpeed_read->Text ="";

        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        AnsiString QueryString;
       	Query->SQL->Clear();
        QueryString =
                   	 "SELECT * FROM " +Database + // :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday "
      							;
         Query->SQL->Add(QueryString);
         Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
         Query->Open();

         while(!Query->Eof)
           {


                   MB_read->Text  = Query->FieldByName("MB")->AsString ;
                   Edi_read->Text =  Query->FieldByName("Edi")->AsString ;
                   R1_read->Text  =  Query->FieldByName("R1")->AsString ;
                   R2_read->Text  =  Query->FieldByName("R2")->AsString ;
                   AD1_read->Text  = Query->FieldByName("AD1")->AsString ;
                   AD2_read->Text  = Query->FieldByName("AD2")->AsString ;



                Query->Next();
           }

           Query->SQL->Clear();
        QueryString =
                   	 "SELECT * FROM " +Database2 + // :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday "
      							;
         Query->SQL->Add(QueryString);
         Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
         Query->Open();

         while(!Query->Eof)
           {
                   StartSpeed_read->Text  = Query->FieldByName("Start_Speed")->AsString ;
                   StopSpeed_read->Text =  Query->FieldByName("End_Speed")->AsString ;

                Query->Next();
           }

       }
       __finally{
                DataModule1->ADOConnectionCreport->Connected=false;
       }
 }


 void FillUpFormEpuro()
 {
    TEdit* AD1=FormMain->EditEpuroAD1 ;
    TEdit* AD2=FormMain->EditEpuroAD2 ;
    TEdit* MB=FormMain->EditEpuroMB;
    TEdit* R1=FormMain->EditEpuroR1;
    TEdit* R2=FormMain->EditEpuroR2;
    TEdit* Edi=FormMain->EditEpuroEdi;
    TEdit* StartSpeed = FormMain->EditStartWasherSpeed;
    TEdit* StopSpeed=FormMain->EditStopWasherSpeed;


    try{
       MB->Text ="";
       Edi->Text ="";
       R1->Text  ="";
       R2->Text  ="";
       AD1->Text  ="";
       AD2->Text  ="";
       StartSpeed->Text  ="";
       StopSpeed->Text ="";


        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        AnsiString QueryString;
       	Query->SQL->Clear();
        QueryString =
                   	 "SELECT * FROM " +Database + // :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday "
      							;
         Query->SQL->Add(QueryString);
         Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
         Query->Open();

         while(!Query->Eof)
           {
                   MB->Text  = Query->FieldByName("MB")->AsString ;
                   Edi->Text =  Query->FieldByName("Edi")->AsString ;
                   R1->Text  =  Query->FieldByName("R1")->AsString ;
                   R2->Text  =  Query->FieldByName("R2")->AsString ;
                   AD1->Text  = Query->FieldByName("AD1")->AsString ;
                   AD2->Text  = Query->FieldByName("AD2")->AsString ;
                Query->Next();
           }

           Query->SQL->Clear();
        QueryString =
                   	 "SELECT * FROM " +Database2 + // :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday "
      							;
         Query->SQL->Add(QueryString);
         Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
         Query->Open();

         while(!Query->Eof)
           {
                   StartSpeed->Text  = Query->FieldByName("Start_Speed")->AsString ;
                   StopSpeed->Text =  Query->FieldByName("End_Speed")->AsString ;
                Query->Next();
           }

       }
       __finally{
                DataModule1->ADOConnectionCreport->Connected=false;
       }

       }



void EmptyFormEpuro()
 {
    TEdit* AD1=FormMain->EditEpuroAD1 ;
    TEdit* AD2=FormMain->EditEpuroAD2 ;
    TEdit* MB=FormMain->EditEpuroMB;
    TEdit* R1=FormMain->EditEpuroR1;
    TEdit* R2=FormMain->EditEpuroR2;
    TEdit* Edi=FormMain->EditEpuroEdi;
    TEdit* StartSpeed = FormMain->EditStartWasherSpeed;
    TEdit* StopSpeed=FormMain->EditStopWasherSpeed;

       MB->Text  ="";// Query->FieldByName("MB")->AsString ;
       Edi->Text ="";//  Query->FieldByName("Edi")->AsString ;
       R1->Text  ="";//  Query->FieldByName("R1")->AsString ;
       R2->Text  ="";//  Query->FieldByName("R2")->AsString ;
       AD1->Text  ="";// Query->FieldByName("AD1")->AsString ;
       AD2->Text  ="";// Query->FieldByName("AD2")->AsString ;
       StartSpeed->Text  ="";// Query->FieldByName("Start_Speed")->AsString ;
       StopSpeed->Text ="";//  Query->FieldByName("End_Speed")->AsString ;

 }
