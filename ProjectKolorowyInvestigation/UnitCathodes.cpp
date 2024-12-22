//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitCathodes.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"
#include <systdate.h>
#include <map>
//---------------------------------------------------------------------------

#pragma package(smart_init)

//---------------------------------------------------------------------------




static AnsiString Database = "Unplug";
static AnsiString Database2 = "";
static AnsiString DataQuery = "";
static TADOQuery* QueryFill;
static TMemo* Remarks;
static TStringGrid* Sheet;
//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);

TCathodeReport CathReport;


void EmptySheetCathodes()
{
   Sheet = FormMain->StringGridCathodes;
   for(int i=1;i<=Sheet->RowCount;i++)
    	Sheet->Rows[i]->Clear();
}








bool CheckIfExistsCathodes()
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

int RemoveCathodesInfoFromDatabase() //function returns number of removed records
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
		   return NoOfRows;

	}
	__finally{
		 DataModule1->ADOConnectionCreport->Connected=false;
	}

}

bool DeleteCathodes()
{
	int NoRows = RemoveCathodesInfoFromDatabase();
	if(NoRows ==0)
	{
		return false;
	}
	else
	{
		FillUpSheetCathodes();
		EmptyFormCathodes();
		return true;
	}
}




void AddCathodes()
{
    EmptySheetCathodes();
    Remarks = FormMain->MemoCathodes;
    Sheet = FormMain->StringGridCathodes;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       int Quantity,i=1,j=1;

       if(CheckIfExistsCathodes())
       {
		   RemoveCathodesInfoFromDatabase();     //clear before saving
       }

       		Query->Close();
            Query->SQL->Clear();
            QueryString=	" INSERT INTO UNPLUG 		"
       	               		" (ID_WorkShiftDay, "
                            " C1, C2, C3, C4, C5, "
                            " C6, C7, C8, C9, C10, "
                            " C11, C12, C13, C14A, C14B, "
                            " C15A, C15B, C16, C17A, C17B, "
                            " C18, C19, C20, C21, C22, "
                            " C23, C24, C25, C26, C27,  "
                            " C28, C29, C30, C31, C32,  "
                            " C33, C34, C35, C36, "
                            "      Comment) 	"
                       		" VALUES (:ID,      "
                            " :C1, :C2, :C3, :C4, :C5, "
                            " :C6, :C7, :C8, :C9, :C10, "
                            " :C11, :C12, :C13, :C14A, :C14B, "
                            " :C15A, :C15B, :C16, :C17A, :C17B, "
                            " :C18, :C19, :C20, :C21, :C22, "
                            " :C23, :C24, :C25, :C26, :C27,  "
                            " :C28, :C29, :C30, :C31, :C32,  "
                            " :C33, :C34, :C35, :C36, "
                            " :Comment)  	"
                    		;
          //  ExchangeAnsStr(QueryString,":Defect","["+ReturnField(Defect)+"]");
        	Query->SQL->Add(QueryString);
            Query->Parameters->ParamByName("ID")->Value=Current.GetID();
        	Query->Parameters->ParamByName("Comment")->Value=Remarks->Lines->Text;
            Query->Parameters->ParamByName("C1")->Value=FormMain->EditCathode1->Text;
            Query->Parameters->ParamByName("C2")->Value=FormMain->EditCathode2->Text;
            Query->Parameters->ParamByName("C3")->Value=FormMain->EditCathode3->Text;
            Query->Parameters->ParamByName("C4")->Value=FormMain->EditCathode4->Text;
            Query->Parameters->ParamByName("C5")->Value=FormMain->EditCathode5->Text;
            Query->Parameters->ParamByName("C6")->Value=FormMain->EditCathode6->Text;
            Query->Parameters->ParamByName("C7")->Value=FormMain->EditCathode7->Text;
            Query->Parameters->ParamByName("C8")->Value=FormMain->EditCathode8->Text;
            Query->Parameters->ParamByName("C9")->Value=FormMain->EditCathode9->Text;
            Query->Parameters->ParamByName("C10")->Value=FormMain->EditCathode10->Text;
            Query->Parameters->ParamByName("C11")->Value=FormMain->EditCathode11->Text;
            Query->Parameters->ParamByName("C12")->Value=FormMain->EditCathode12->Text;
            Query->Parameters->ParamByName("C13")->Value=FormMain->EditCathode13->Text;
            Query->Parameters->ParamByName("C14A")->Value=FormMain->EditCathode14A->Text;
            Query->Parameters->ParamByName("C14B")->Value=FormMain->EditCathode14B->Text;
            Query->Parameters->ParamByName("C15A")->Value=FormMain->EditCathode15A->Text;
            Query->Parameters->ParamByName("C15B")->Value=FormMain->EditCathode15B->Text;
            Query->Parameters->ParamByName("C16")->Value=FormMain->EditCathode16->Text;
            Query->Parameters->ParamByName("C17A")->Value=FormMain->EditCathode17A->Text;
            Query->Parameters->ParamByName("C17B")->Value=FormMain->EditCathode17B->Text;
            Query->Parameters->ParamByName("C18")->Value=FormMain->EditCathode18->Text;
            Query->Parameters->ParamByName("C19")->Value=FormMain->EditCathode19->Text;
            Query->Parameters->ParamByName("C20")->Value=FormMain->EditCathode20->Text;
            Query->Parameters->ParamByName("C21")->Value=FormMain->EditCathode21->Text;
            Query->Parameters->ParamByName("C22")->Value=FormMain->EditCathode22->Text;
            Query->Parameters->ParamByName("C23")->Value=FormMain->EditCathode23->Text;
            Query->Parameters->ParamByName("C24")->Value=FormMain->EditCathode24->Text;
            Query->Parameters->ParamByName("C25")->Value=FormMain->EditCathode25->Text;
            Query->Parameters->ParamByName("C26")->Value=FormMain->EditCathode26->Text;
            Query->Parameters->ParamByName("C27")->Value=FormMain->EditCathode27->Text;
            Query->Parameters->ParamByName("C28")->Value=FormMain->EditCathode28->Text;
            Query->Parameters->ParamByName("C29")->Value=FormMain->EditCathode29->Text;
            Query->Parameters->ParamByName("C30")->Value=FormMain->EditCathode30->Text;
            Query->Parameters->ParamByName("C31")->Value=FormMain->EditCathode31->Text;
            Query->Parameters->ParamByName("C32")->Value=FormMain->EditCathode32->Text;
            Query->Parameters->ParamByName("C33")->Value=FormMain->EditCathode33->Text;
            Query->Parameters->ParamByName("C34")->Value=FormMain->EditCathode34->Text;
            Query->Parameters->ParamByName("C35")->Value=FormMain->EditCathode35->Text;
            Query->Parameters->ParamByName("C36")->Value=FormMain->EditCathode36->Text;


            Query->ExecSQL();
//        	if (NoOfRows!=0) {
//                return true;
//            }
//            else
//            {
//                return false;
//            }


        FillUpSheetCathodes();

     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}


void FillUpSheetCathodes()
{
    EmptySheetCathodes();
    std::map<AnsiString,int> Cath;
    Remarks = FormMain->MemoCathodes;
    Sheet = FormMain->StringGridCathodes;
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
			CathReport.Clean();
            if(Query->RecordCount==0)
                	return;   // if there are no results


        	Name=Query->FieldByName("Comment")->AsString;
            Cath["C1"] =Query->FieldByName("C1")->AsInteger;
            Cath["C2"] =Query->FieldByName("C2")->AsInteger;
            Cath["C3"] =Query->FieldByName("C3")->AsInteger;
            Cath["C4"] =Query->FieldByName("C4")->AsInteger;
            Cath["C5"] =Query->FieldByName("C5")->AsInteger;
            Cath["C6"] =Query->FieldByName("C6")->AsInteger;
            Cath["C7"] =Query->FieldByName("C7")->AsInteger;
            Cath["C8"] =Query->FieldByName("C8")->AsInteger;
            Cath["C9"] =Query->FieldByName("C9")->AsInteger;
            Cath["C10"] =Query->FieldByName("C10")->AsInteger;
            Cath["C11"] =Query->FieldByName("C11")->AsInteger;
            Cath["C12"] =Query->FieldByName("C12")->AsInteger;
            Cath["C13"] =Query->FieldByName("C13")->AsInteger;
            Cath["C14A"] =Query->FieldByName("C14A")->AsInteger;
            Cath["C14B"] =Query->FieldByName("C14B")->AsInteger;
            Cath["C15A"] =Query->FieldByName("C15A")->AsInteger;
            Cath["C15B"] =Query->FieldByName("C15B")->AsInteger;
            Cath["C16"] =Query->FieldByName("C16")->AsInteger;
            Cath["C17A"] =Query->FieldByName("C17A")->AsInteger;
            Cath["C17B"] =Query->FieldByName("C17B")->AsInteger;
            Cath["C18"] =Query->FieldByName("C18")->AsInteger;
            Cath["C19"] =Query->FieldByName("C19")->AsInteger;
            Cath["C20"] =Query->FieldByName("C20")->AsInteger;
            Cath["C21"] =Query->FieldByName("C21")->AsInteger;
            Cath["C22"] =Query->FieldByName("C22")->AsInteger;
            Cath["C23"] =Query->FieldByName("C23")->AsInteger;
            Cath["C24"] =Query->FieldByName("C24")->AsInteger;
            Cath["C25"] =Query->FieldByName("C25")->AsInteger;
            Cath["C26"] =Query->FieldByName("C26")->AsInteger;
            Cath["C27"] =Query->FieldByName("C27")->AsInteger;
            Cath["C28"] =Query->FieldByName("C28")->AsInteger;
            Cath["C29"] =Query->FieldByName("C29")->AsInteger;
            Cath["C30"] =Query->FieldByName("C30")->AsInteger;
            Cath["C31"] =Query->FieldByName("C31")->AsInteger;
            Cath["C32"] =Query->FieldByName("C32")->AsInteger;
            Cath["C33"] =Query->FieldByName("C33")->AsInteger;
            Cath["C34"] =Query->FieldByName("C34")->AsInteger;
            Cath["C35"] =Query->FieldByName("C35")->AsInteger;
            Cath["C36"]=Query->FieldByName("C36")->AsInteger;
           int i=1;
           std::map<AnsiString,int>::iterator pos;
        	for(pos=Cath.begin();pos!=Cath.end();pos++)
            {
                if(pos->second!=0)
                {
                    Sheet->Cells[0][i]=pos->first;
                    Sheet->Cells[1][i]=pos->second;
                    i++;
                }
                if(i>Sheet->RowCount)
                  Sheet->RowCount = i+1;
			}
			Sheet->Cells[0][i]=Name;
			int NoCathodes = i-1;
			UnicodeString U1, U2;
		   //	CathReport.Clean();
			CathReport.SetNoCathodes(NoCathodes);
			for (i = 0; i < NoCathodes; i++) {
			  U1 = Sheet->Cells[0][i+1] ;
			  U2 = Sheet->Cells[1][i+1].ToInt();
			  CathReport.GetCathodeNames()[i]= U1;
			  CathReport.GetNoSwitched()[i]= U2.ToInt();
			}
			CathReport.SetComment(Name);


     }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }

}

void FillUpFormCathodes()
{
   Remarks = FormMain->MemoCathodes;
    Sheet = FormMain->StringGridCathodes;
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


        	Remarks->Lines->Text=Query->FieldByName("Comment")->AsString;
            FormMain->EditCathode1->Text=Query->FieldByName("C1")->AsInteger;
            FormMain->EditCathode2->Text=Query->FieldByName("C2")->AsInteger;
            FormMain->EditCathode3->Text=Query->FieldByName("C3")->AsInteger;
            FormMain->EditCathode4->Text=Query->FieldByName("C4")->AsInteger;
            FormMain->EditCathode5->Text=Query->FieldByName("C5")->AsInteger;
            FormMain->EditCathode6->Text=Query->FieldByName("C6")->AsInteger;
            FormMain->EditCathode7->Text=Query->FieldByName("C7")->AsInteger;
            FormMain->EditCathode8->Text=Query->FieldByName("C8")->AsInteger;
            FormMain->EditCathode9->Text=Query->FieldByName("C9")->AsInteger;
            FormMain->EditCathode10->Text=Query->FieldByName("C10")->AsInteger;
            FormMain->EditCathode11->Text=Query->FieldByName("C11")->AsInteger;
            FormMain->EditCathode12->Text=Query->FieldByName("C12")->AsInteger;
            FormMain->EditCathode13->Text=Query->FieldByName("C13")->AsInteger;
            FormMain->EditCathode14A->Text=Query->FieldByName("C14A")->AsInteger;
            FormMain->EditCathode14B->Text=Query->FieldByName("C14B")->AsInteger;
            FormMain->EditCathode15A->Text=Query->FieldByName("C15A")->AsInteger;
            FormMain->EditCathode15B->Text=Query->FieldByName("C15B")->AsInteger;
            FormMain->EditCathode16->Text=Query->FieldByName("C16")->AsInteger;
            FormMain->EditCathode17A->Text=Query->FieldByName("C17A")->AsInteger;
            FormMain->EditCathode17B->Text=Query->FieldByName("C17B")->AsInteger;
            FormMain->EditCathode18->Text=Query->FieldByName("C18")->AsInteger;
            FormMain->EditCathode19->Text=Query->FieldByName("C19")->AsInteger;
            FormMain->EditCathode20->Text=Query->FieldByName("C20")->AsInteger;
            FormMain->EditCathode21->Text=Query->FieldByName("C21")->AsInteger;
            FormMain->EditCathode22->Text=Query->FieldByName("C22")->AsInteger;
            FormMain->EditCathode23->Text=Query->FieldByName("C23")->AsInteger;
            FormMain->EditCathode24->Text=Query->FieldByName("C24")->AsInteger;
            FormMain->EditCathode25->Text=Query->FieldByName("C25")->AsInteger;
            FormMain->EditCathode26->Text=Query->FieldByName("C26")->AsInteger;
            FormMain->EditCathode27->Text=Query->FieldByName("C27")->AsInteger;
            FormMain->EditCathode28->Text=Query->FieldByName("C28")->AsInteger;
            FormMain->EditCathode29->Text=Query->FieldByName("C29")->AsInteger;
            FormMain->EditCathode30->Text=Query->FieldByName("C30")->AsInteger;
            FormMain->EditCathode31->Text=Query->FieldByName("C31")->AsInteger;
            FormMain->EditCathode32->Text=Query->FieldByName("C32")->AsInteger;
            FormMain->EditCathode33->Text=Query->FieldByName("C33")->AsInteger;
            FormMain->EditCathode34->Text=Query->FieldByName("C34")->AsInteger;
            FormMain->EditCathode35->Text=Query->FieldByName("C35")->AsInteger;
            FormMain->EditCathode36->Text=Query->FieldByName("C36")->AsInteger;


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



void EmptyFormCathodes()
{

 			Remarks = FormMain->MemoCathodes;

        	Remarks->Lines->Text="";//Query->FieldByName("Comment")->AsString;
            FormMain->EditCathode1->Text=0;//Query->FieldByName("C1")->AsInteger;
            FormMain->EditCathode2->Text=0;//Query->FieldByName("C2")->AsInteger;
            FormMain->EditCathode3->Text=0;//Query->FieldByName("C3")->AsInteger;
            FormMain->EditCathode4->Text=0;//Query->FieldByName("C4")->AsInteger;
            FormMain->EditCathode5->Text=0;//Query->FieldByName("C5")->AsInteger;
            FormMain->EditCathode6->Text=0;//Query->FieldByName("C6")->AsInteger;
            FormMain->EditCathode7->Text=0;//Query->FieldByName("C7")->AsInteger;
            FormMain->EditCathode8->Text=0;//Query->FieldByName("C8")->AsInteger;
            FormMain->EditCathode9->Text=0;//Query->FieldByName("C9")->AsInteger;
            FormMain->EditCathode10->Text=0;//Query->FieldByName("C10")->AsInteger;
            FormMain->EditCathode11->Text=0;//Query->FieldByName("C11")->AsInteger;
            FormMain->EditCathode12->Text=0;//Query->FieldByName("C12")->AsInteger;
            FormMain->EditCathode13->Text=0;//Query->FieldByName("C13")->AsInteger;
            FormMain->EditCathode14A->Text=0;//Query->FieldByName("C14A")->AsInteger;
            FormMain->EditCathode14B->Text=0;//Query->FieldByName("C14B")->AsInteger;
            FormMain->EditCathode15A->Text=0;//Query->FieldByName("C15A")->AsInteger;
            FormMain->EditCathode15B->Text=0;//Query->FieldByName("C15B")->AsInteger;
            FormMain->EditCathode16->Text=0;//Query->FieldByName("C16")->AsInteger;
            FormMain->EditCathode17A->Text=0;//Query->FieldByName("C17A")->AsInteger;
            FormMain->EditCathode17B->Text=0;//Query->FieldByName("C17B")->AsInteger;
            FormMain->EditCathode18->Text=0;//Query->FieldByName("C18")->AsInteger;
            FormMain->EditCathode19->Text=0;//Query->FieldByName("C19")->AsInteger;
            FormMain->EditCathode20->Text=0;//Query->FieldByName("C20")->AsInteger;
            FormMain->EditCathode21->Text=0;//Query->FieldByName("C21")->AsInteger;
            FormMain->EditCathode22->Text=0;//Query->FieldByName("C22")->AsInteger;
            FormMain->EditCathode23->Text=0;//Query->FieldByName("C23")->AsInteger;
            FormMain->EditCathode24->Text=0;//Query->FieldByName("C24")->AsInteger;
            FormMain->EditCathode25->Text=0;//Query->FieldByName("C25")->AsInteger;
            FormMain->EditCathode26->Text=0;//Query->FieldByName("C26")->AsInteger;
            FormMain->EditCathode27->Text=0;//Query->FieldByName("C27")->AsInteger;
            FormMain->EditCathode28->Text=0;//Query->FieldByName("C28")->AsInteger;
            FormMain->EditCathode29->Text=0;//Query->FieldByName("C29")->AsInteger;
            FormMain->EditCathode30->Text=0;//Query->FieldByName("C30")->AsInteger;
            FormMain->EditCathode31->Text=0;//Query->FieldByName("C31")->AsInteger;
            FormMain->EditCathode32->Text=0;//Query->FieldByName("C32")->AsInteger;
            FormMain->EditCathode33->Text=0;//Query->FieldByName("C33")->AsInteger;
            FormMain->EditCathode34->Text=0;//Query->FieldByName("C34")->AsInteger;
            FormMain->EditCathode35->Text=0;//Query->FieldByName("C35")->AsInteger;
            FormMain->EditCathode36->Text=0;//Query->FieldByName("C36")->AsInteger;



}


