//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitCTControl.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

static AnsiString Database = "Problems_Line2";
static TEdit* Contact ;//Contact person
static TEdit* MTComment;// = FormMain->EditLineStackerMTComment;
static TEdit* Problem; // = FormMain->EditLineStackerProblem;
static TComboBox* Combo; // = FormMain->ComboBoxLineStackerStatus;
static TStringGrid* Sheet; // = FormMain->StringGridLineStacker;
static TDateTimePicker* Picker;

void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);


void DeleteCTControl()
{
    Picker = FormMain->DateTimePickerCTControl;

 		 Contact = FormMain->EditContactCTControl ;//Contact person
 	    MTComment = FormMain->EditMTCommentCTControl;
 		 Problem = FormMain->EditProblemCTControl;
 		 Combo = FormMain->ComboBoxCTControl;
        Sheet = FormMain->StringGridCTControl;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       Query->Close();
       Query->SQL->Clear();
       QueryString = "DELETE * FROM "+Database +// :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime   "
                      ;
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
       Query->Parameters->ParamByName("DateTime")->Value=Sheet->Cells[0][Sheet->Selection.Top];//   Picker->DateTime;
       Query->ExecSQL();
       Query->Close();
       FillingUpSheet(Current.GetID(),Sheet,Combo, Database);
       EmptyForm(Problem,Contact,MTComment,Combo,Picker);
    }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

void AddCTControl()
{

 Picker = FormMain->DateTimePickerCTControl;

 		 Contact = FormMain->EditContactCTControl ;//Contact person
 	    MTComment = FormMain->EditMTCommentCTControl;
 		 Problem = FormMain->EditProblemCTControl;
 		 Combo = FormMain->ComboBoxCTControl;
        Sheet = FormMain->StringGridCTControl;

//   SetProduction();
   try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
      //


       // Checking integrity - i.e. if update data or insert new
       // beneath checking of existence;

       Query->SQL->Clear();
       QueryString = "SELECT ID_WorkshiftDay, Comment_DateTime FROM "+Database +// :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime   "
                      ;
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
       Query->Parameters->ParamByName("DateTime")->Value=Picker->DateTime;
       Query->Open();

       AnsiString Message;

       if(Query->RecordCount==1)  // Update as there is given product for one given shift
       {
            if(Application->MessageBox(L"Chcesz zmieniæ zapisane dane? \n\nDo you want to change saved data",L"Uwaga/Attention!!",MB_YESNO)==IDYES)
            {
               Query->SQL->Clear();
               QueryString = " UPDATE "+Database +//:Database;  																"
                      		 " SET Comment = :Comment, Contact_Person=:Contact_Person, ID_StatusType=:ID_StatusType, 	"
                             "     MT_Comment = :MT_Comment 												"
                      		 " WHERE ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime  	"
                           ;
               Query->SQL->Add(QueryString);
//               Query->Parameters->ParamByName("Database")->Value=Database;
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Parameters->ParamByName("DateTime")->Value=Picker->DateTime;
               Query->Parameters->ParamByName("Comment")->Value=Problem->Text;
               Query->Parameters->ParamByName("Contact_Person")->Value=Contact->Text;
               Query->Parameters->ParamByName("ID_StatusType")->Value=Combo->ItemIndex;
               Query->Parameters->ParamByName("MT_Comment")->Value=MTComment->Text;
               Query->ExecSQL();
               Query->SQL->Clear();



            }         //------------------   I do not want to upgrade or change any data
            else                 // After Not updating any data
            {
               FillingUpSheet(Current.GetID(),Sheet,Combo, Database);
               EmptyForm(Problem,Contact,MTComment,Combo,Picker);
               return;
            }
       }   // ---------------------  End of dealing with upgrade   --------------------
       else if(Query->RecordCount==0)  // No data were found - adding new batch
       {
           Query->SQL->Clear();
           QueryString = "INSERT INTO "+Database +// :Database  "
                  " (Comment, Contact_Person, ID_StatusType, MT_Comment,ID_Workshiftday, Comment_DateTime)  "
                  " VALUES   (:Comment, :Contact_Person, :ID_StatusType,:MT_Comment, :ID_Workshiftday, :DateTime) "
                  ;
           Query->SQL->Add(QueryString);
                Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Parameters->ParamByName("DateTime")->Value=Picker->DateTime;
               Query->Parameters->ParamByName("Comment")->Value=Problem->Text;
               Query->Parameters->ParamByName("Contact_Person")->Value=Contact->Text;
               Query->Parameters->ParamByName("ID_StatusType")->Value=Combo->ItemIndex;
               Query->Parameters->ParamByName("MT_Comment")->Value=MTComment->Text;
           Query->ExecSQL();

        }

        FillingUpSheet(Current.GetID(),Sheet,  Combo, Database);
        EmptyForm(Problem, Contact, MTComment, Combo, Picker);
	}
    __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

void EmptyCTControlForm()
{
 Picker = FormMain->DateTimePickerCTControl;

 		 Contact = FormMain->EditContactCTControl ;//Contact person
 	    MTComment = FormMain->EditMTCommentCTControl;
 		 Problem = FormMain->EditProblemCTControl;
 		 Combo = FormMain->ComboBoxCTControl;
        Sheet = FormMain->StringGridCTControl;
     EmptyForm(Problem, Contact, MTComment, Combo, Picker);
}









void FillingUpCTControlSheet(int ID)
{
 Picker = FormMain->DateTimePickerCTControl;
 Contact = FormMain->EditContactCTControl ;//Contact person
 MTComment = FormMain->EditMTCommentCTControl;
 Problem = FormMain->EditProblemCTControl;
 Combo = FormMain->ComboBoxCTControl;
 Sheet = FormMain->StringGridCTControl;
 FillingUpSheet(ID,Sheet,  Combo, Database);
}

//
//void EmptyLineStackerForm()
//{
//    FormMain->EditLineStackerProblem->Text="";
//    FormMain->EditLineStackerContact->Text="";
//    FormMain->EditLineStackerMTComment->Text="";
//    FormMain->DateTimePickerAlarms->DateTime.CurrentDateTime();
//    FormMain->ComboBoxLineStackerStatus->ItemIndex=0;
//}
//
void CTControlFillUpForm(int Index)
{
 Picker = FormMain->DateTimePickerCTControl;

 		 Contact = FormMain->EditContactCTControl ;//Contact person
 	    MTComment = FormMain->EditMTCommentCTControl;
 		 Problem = FormMain->EditProblemCTControl;
 		 Combo = FormMain->ComboBoxCTControl;
        Sheet = FormMain->StringGridCTControl;
    try{


        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        AnsiString QueryString;
       	Query->SQL->Clear();
        QueryString =
                   	 "SELECT * FROM " +Database + // :Database "
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday AND Comment_DateTime =:DateTime   "

      							;
      Query->SQL->Add(QueryString);
    //  Query->Parameters->ParamByName("Database")->Value=Database;
      Query->Parameters->ParamByName("ID_Workshiftday")->Value=Current.GetID();
      Query->Parameters->ParamByName("DateTime")->Value=Sheet->Cells[0][Index];
      Query->Open();



    Problem->Text=(Query->FieldByName("Comment")->AsString);
    Contact->Text=(Query->FieldByName("Contact_Person")->AsString);
    MTComment->Text=(Query->FieldByName("MT_Comment")->AsString);
    Picker->DateTime=(Query->FieldByName("Comment_DateTime")->AsDateTime);
    Combo->ItemIndex=(Query->FieldByName("ID_StatusType")->AsInteger);;


    }   // end of try
    __finally{
        DataModule1->ADOConnectionCreport->Connected=false;
    }
}
