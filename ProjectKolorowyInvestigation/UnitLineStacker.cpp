//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitLineStacker.h"

//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------





#include "Global.h"
#include "UnitMain.h"
#include "DataCoater.h"

//---------------------------------------------------------------------------

static AnsiString Database = "Problems_Line1";
static TEdit* Contact ;//Contact person
static TEdit* MTComment;// = FormMain->EditLineStackerMTComment;
static TEdit* Problem; // = FormMain->EditLineStackerProblem;
static TComboBox* Combo; // = FormMain->ComboBoxLineStackerStatus;
static TStringGrid* Sheet; // = FormMain->StringGridLineStacker;
static TDateTimePicker* Picker;

void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);

void FillingUpSheet(int ID, TStringGrid* Sh, TComboBox* Com, AnsiString Database )
{
      


        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        EmptySheet(Sh);
        AnsiString QueryString,Temp;
        int TempIndex;

      	Query->SQL->Clear();
        QueryString ="SELECT * FROM "+Database +
       				 " WHERE  ID_Workshiftday = :ID_Workshiftday    "
                      ;
                        ;
           Query->SQL->Add(QueryString);
           int IDW = ID; //CurrentProduction->GetID_WorkshiftDay();
           Query->Parameters->ParamByName("ID_Workshiftday")->Value=IDW;
//           Query->Parameters->ParamByName("Database")->Value=Database;
           Query->Open();

           if(Query->RecordCount==0)
           	return;
    	   int i=1;

           while(!Query->Eof)
           {
                Sh->Cells[0][i]=(Query->FieldByName("Comment_DateTime")->AsString);
                Sh->Cells[1][i]=(Query->FieldByName("Comment")->AsString);
                Sh->Cells[2][i]=(Query->FieldByName("Contact_Person")->AsString);
                Sh->Cells[3][i]=Combo->Items->Strings[Query->FieldByName("ID_StatusType")->AsInteger];
                Sh->Cells[4][i]=(Query->FieldByName("MT_Comment")->AsString);

//                    Temp = ReadP.GetRemarks()->Text;
//                    while((TempIndex=Temp.Pos("\r\n"))!=0)  //Removing Carriage Return from string
//                    {
//                        Temp.Delete(TempIndex,2);
//                        Temp.Insert(" ",TempIndex);
//                    }
//                    FormMain->StringGridProducts->Cells[6][i] = Temp ;

                i++;
                Query->Next();
                if(i>=Sh->RowCount)
                	Sh->RowCount=Sh->RowCount+1;
           }


            TGridRect R;
            R.Left=0;
            R.Top= i ;
            R.Right= 5 ;
            R.Bottom = i  ;
            Sh->Selection=R;
            DataModule1->ADOConnectionCreport->Connected=false;
}

void DeleteLineStacker()
{
    Picker = FormMain->DateTimePickerLineStacker;
    Contact = FormMain->EditLineStackerContact ;//Contact person
    MTComment = FormMain->EditLineStackerMTComment;
    Problem = FormMain->EditLineStackerProblem;
    Combo = FormMain->ComboBoxLineStackerStatus;
    Sheet = FormMain->StringGridLineStacker;
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

void AddLineStacker()
{

 Picker = FormMain->DateTimePickerLineStacker;
 Contact = FormMain->EditLineStackerContact ;//Contact person
 MTComment = FormMain->EditLineStackerMTComment;
 Problem = FormMain->EditLineStackerProblem;
 Combo = FormMain->ComboBoxLineStackerStatus;
 Sheet = FormMain->StringGridLineStacker;

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

        FillingUpSheet(Current.GetID(),Sheet,  Combo,Database);
        EmptyForm(Problem, Contact, MTComment, Combo, Picker);
	}
    __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

void EmptyLineStackerForm()
{
     Picker = FormMain->DateTimePickerLineStacker;
  Contact = FormMain->EditLineStackerContact ;//Contact person
 MTComment = FormMain->EditLineStackerMTComment;
 Problem = FormMain->EditLineStackerProblem;
 Combo = FormMain->ComboBoxLineStackerStatus;
 Sheet = FormMain->StringGridLineStacker;
     EmptyForm(Problem, Contact, MTComment, Combo, Picker);
}





void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker)
{
    Problem->Text="";
    Contact->Text="";
    MTComment->Text="";
    Picker->DateTime.CurrentDateTime();
    Combo->ItemIndex=0;
}



void FillingUpLineStackerSheet(int ID)
{

 Picker = FormMain->DateTimePickerLineStacker;
 Contact = FormMain->EditLineStackerContact ;//Contact person
 MTComment = FormMain->EditLineStackerMTComment;
 Problem = FormMain->EditLineStackerProblem;
 Combo = FormMain->ComboBoxLineStackerStatus;
 Sheet = FormMain->StringGridLineStacker;
 FillingUpSheet(ID,Sheet, Combo, Database);
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
void LineStackerFillUpForm(int Index)
{
 Picker = FormMain->DateTimePickerLineStacker;
 Contact = FormMain->EditLineStackerContact ;//Contact person
 MTComment = FormMain->EditLineStackerMTComment;
 Problem = FormMain->EditLineStackerProblem;
 Combo = FormMain->ComboBoxLineStackerStatus;
 Sheet = FormMain->StringGridLineStacker;
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
