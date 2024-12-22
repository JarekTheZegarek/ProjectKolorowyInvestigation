//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitGerneralRemark.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"
#include <systdate.h>

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------



static AnsiString Database = "WorkshiftDaysRemarks";
static TMemo* Remarks;

//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);



void DeleteGeneralRemarks()
{

    Remarks = FormMain->MemoGeneralRemarks;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       Query->SQL->Clear();
	   QueryString = " DELETE  FROM   " + Database +
					 " WHERE ID_Workshiftday = :ID_Workshiftday "
						   ;
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();

	   DataModule1->ADOConnectionCreport->BeginTrans();
			   Query->ExecSQL();
			   Query->Close();
			   Query->SQL->Clear();
	   DataModule1->ADOConnectionCreport->CommitTrans();

	   FillingUpGeneralRemarks();
	}
	 __finally{
		 DataModule1->ADOConnectionCreport->Connected=false;
	}
}

void AddGeneralRemarks()
{

	Remarks = FormMain->MemoGeneralRemarks;
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
			   bool IsThereAnyRemark = false;
	   if(Query->RecordCount>0)
	   {
		   IsThereAnyRemark = true;
		   if(Application->MessageBox(L"Chcesz zmieniæ zapisane dane? \n\nDo you want to change saved data",L"Uwaga / Attention!!",MB_YESNO)==IDYES)
		   {
			   DataModule1->ADOConnectionCreport->BeginTrans();
			   Query->Close();  //Updating in this case means Deleting and inserting
			   Query->SQL->Clear();

			   QueryString = "DELETE FROM   " + Database +
							 " WHERE ID_Workshiftday = :ID_Workshiftday "
						   ;
			   Query->SQL->Add(QueryString);
			   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();

			   int NumberOrRowsAffected;
			   NumberOrRowsAffected = Query->ExecSQL();
			   Query->Close();
			   Query->SQL->Clear();
			   if(!Remarks->Lines->Text.IsEmpty())
			   {
				   QueryString =" INSERT INTO   " + Database +
										" (ID_Workshiftday, Remarks) "
										" VALUES   (:ID_Workshiftday, :Remarks) "
																   ;
				   Query->SQL->Add(QueryString);

				   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
				   Query->Parameters->ParamByName("Remarks")->Value= Remarks->Lines->Text;

				   Query->ExecSQL();
				   Query->Close();
				   Query->SQL->Clear();
			   }
		   DataModule1->ADOConnectionCreport->CommitTrans();


		   }         //------------------   I do not want to upgrade or change any data
		   else                 // After Not updating any data
		   {
				   FillingUpGeneralRemarks();
				   return;
		   }
	   }   // ---------------------  End of dealing with upgrade   --------------------
	   else   // Cursor is on empty cell - adding new batch
	   {
		   Query->SQL->Clear();
		   if(!Remarks->Lines->Text.IsEmpty())
			   {
				   QueryString =" INSERT INTO   " + Database +
										" (ID_Workshiftday, Remarks) "
										" VALUES   (:ID_Workshiftday, :Remarks) "
																   ;
				   Query->SQL->Add(QueryString);

				   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
				   Query->Parameters->ParamByName("Remarks")->Value= Remarks->Lines->Text;

				   Query->ExecSQL();
				   Query->Close();
				   Query->SQL->Clear();
			   }
		}
		   FillingUpGeneralRemarks();
	}
	__finally{
		 DataModule1->ADOConnectionCreport->Connected=false;
	}
}



 void FillingUpGeneralRemarks()
 {

	Remarks = FormMain->MemoGeneralRemarks;
	TMemo* RemarksDB = FormMain->MemoGeneralRemarksDatabase;
	try{
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
		 Remarks->Clear() ;
		 RemarksDB->Clear();
		 while(!Query->Eof)
		   {

				RemarksDB->Lines->Text=Query->FieldByName("Remarks")->AsString;
				Query->Next();

		   }
		 Remarks->Lines->Text=RemarksDB->Lines->Text;
	   }
	   __finally{
				DataModule1->ADOConnectionCreport->Connected=false;
	   }
 }




