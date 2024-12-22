//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitLineStatus.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"
#include <systdate.h>
#include "UnitStartUp.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

static AnsiString Database = "Line_Status";
static AnsiString Database2 = "LineStatusRemarks";
static AnsiString DataQuery = "QueryLineStatusRemarks";
static TEdit* Contact ;//Contact person
static TEdit* MTComment;// = FormMain->EditLineStackerMTComment;
static TEdit* Problem; // = FormMain->EditLineStackerProblem;
static TComboBox* Combo; // = FormMain->ComboBoxLineStackerStatus;
static TStringGrid* Sheet; // = FormMain->StringGridLineStacker;
static TDateTimePicker* Picker;
static TDateTimePicker* PickerStop;
static TMemo* Remarks;
static int LastProdIndex=-1;

TLineStatus* LineStatuses;

//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);

void EmptySheetLineStatus()
{
   Sheet = FormMain->StringGridLineStatus;
   for(int i=1;i<=Sheet->RowCount;i++)
		Sheet->Rows[i]->Clear();
   FormMain->ComboBoxLineStatusSelect(FormMain);
}


void DeleteLineStatus()
{
    Picker = FormMain->DateTimePickerLineStatusStart;
    PickerStop = FormMain->DateTimePickerLineStatusStop;
    Contact = FormMain->EditMinuteLineStatus ;//Contact person
// 	MTComment = FormMain->EditMTCommentWasher;
// 	Problem = FormMain->EditProblemWasher;
    Remarks = FormMain->MemoLineStatus;
 	Combo = FormMain->ComboBoxLineStatus;
    Sheet = FormMain->StringGridLineStatus;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       Query->SQL->Clear();
	   QueryString = " DELETE FROM   " + Database +
               		 " WHERE ID_Workshiftday = :ID_Workshiftday AND Start_Time =:DateTime  	"
                     " AND   End_Time = :EndTime AND  ID_LineStatus = :ID_LineStatus        "
                           ;
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
       Query->Parameters->ParamByName("DateTime")->Value=TDateTime(Sheet->Cells[0][Sheet->Selection.Top],TDateTime::Time);
       Query->Parameters->ParamByName("EndTime")->Value=TDateTime(Sheet->Cells[1][Sheet->Selection.Top],TDateTime::Time);
       Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->Items->IndexOf(Sheet->Cells[3][Sheet->Selection.Top])];

       DataModule1->ADOConnectionCreport->BeginTrans();
       {
               Query->ExecSQL();
               Query->Close();
               Query->SQL->Clear();

			   QueryString = " DELETE FROM   " + Database2 +
                             " WHERE ID_Workshiftday = :ID_Workshiftday AND Start_Time =:DateTime  	"
                             " AND   End_Time = :EndTime AND  ID_LineStatus = :ID_LineStatus        "
                                   ;
               Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Parameters->ParamByName("DateTime")->Value=TDateTime(Sheet->Cells[0][Sheet->Selection.Top],TDateTime::Time);
               Query->Parameters->ParamByName("EndTime")->Value=TDateTime(Sheet->Cells[1][Sheet->Selection.Top],TDateTime::Time);
			   Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->Items->IndexOf(Sheet->Cells[3][Sheet->Selection.Top])];

			   Query->ExecSQL();
			   Query->Close();
			   Query->SQL->Clear();
	   }
	   DataModule1->ADOConnectionCreport->CommitTrans();

	   FillingUpSheetLineStatus();
	   EmptyFormLineStatus();
	}
	 __finally{
		 DataModule1->ADOConnectionCreport->Connected=false;
		 FormMain->ComboBoxLineStatusSelect(FormMain);
	}
}

void AddLineStatus()
{

 	Picker = FormMain->DateTimePickerLineStatusStart;
    PickerStop = FormMain->DateTimePickerLineStatusStop;
	Contact = FormMain->EditMinuteLineStatus ;
	Remarks = FormMain->MemoLineStatus;
	Combo = FormMain->ComboBoxLineStatus;
	Sheet = FormMain->StringGridLineStatus;
	if (Combo->Text.IsEmpty() || Combo->ItemIndex==-1) { //No item is choosen
	  ShowMessage(L"Proszę wypełnić status");
	  return;
	}
	int DecentCommentLength = 10;
//   SetProduction();
	if(ComboLineStatusToID[Combo->ItemIndex]==6 && Remarks->Lines->Text.Length()<DecentCommentLength )  //When there is developement Combo->Text.Pos("Develop")!=0
	{
		ShowMessage(L"Proszę wpisać komentarz! \nKto przeprowadzał badania?\nCo było robione - cel?");
		return;
	}
	if(ComboLineStatusToID[Combo->ItemIndex]==4) //              Combo->Text.Pos("Changeover")!=0
	{

		if (FormMain->ComboBoxProdFrom->Text.IsEmpty() || FormMain->ComboBoxProdTo->Text.IsEmpty()) {
		   //ShowMessage("Proszê wpisaæ komentarz! \nZ jakiego produktu na jaki przechodzi³a zmiana.\nJe¿eli przekroczono \"Standardowe\" czasy to dlaczego?");
		   ShowMessage(L"Proszę wypełnić rubryki \"z\" i\"Na\"");
		   return;
		}
		UnicodeString MessageChange =  "Zmiana z produktu "+FormMain->ComboBoxProdFrom->Text+" na produkt "+FormMain->ComboBoxProdTo->Text ;
		if(FormMain->MemoLineStatus->Lines->Text.Pos(MessageChange)==0 ) // If the same message do not exists
			FormMain->MemoLineStatus->Lines->Add(MessageChange);

    }
	if( ComboLineStatusToID[Combo->ItemIndex]==2 && Remarks->Lines->Text.Length()<DecentCommentLength)   //Combo->Text.Pos("Overhaul")!=0
	{
		ShowMessage("Proszę pamiętać o dodaniu komentarza w razie kłopotów w trakcie serwisu");
	}
try{
try{
	   TADOQuery* Query = DataModule1->ADOQueryCreport;
	   DataModule1->ADOConnectionCreport->Connected=true;
	   AnsiString QueryString,Name;
		TDateTime Start = Picker->Time;
		TDateTime Stop=PickerStop->Time;
		int CurrentMinutes = Current.GetSumOfMinutes();
		int MinDiffInt=MinuteDiff(Start,Stop);

	   // Checking integrity - i.e. if update data or insert new
	   // beneath checking of existence;

	   AnsiString Message;

	   if( !Sheet->Cells[1][Sheet->Selection.Top].IsEmpty() )  // Update ?
	   {
				Query->Close();  //Updating in this case means Deleting and inserting
				Query->SQL->Clear();
				QueryString = "SELECT * FROM   " + DataQuery +
							 " WHERE ID_Workshiftday = :ID_Workshiftday AND Start_Time =:DateTime  	"
							 " AND   End_Time = :EndTime AND  ID_LineStatus = :ID_LineStatus            "
						   //	 " AND   ID_ProdType= :ID_ProdType ";//AND (Additional = :Additional OR IsNull(Additional) ) "
						   ;
			   Query->SQL->Add(QueryString);
			   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
			   Query->Parameters->ParamByName("DateTime")->Value=Picker->Time.FormatString("h:nn");//TDateTime(Picker->Time.TimeString(),TDateTime::Time);
			   Query->Parameters->ParamByName("EndTime")->Value=PickerStop->Time.FormatString("h:nn");  //TDateTime(PickerStop->Time.TimeString(),TDateTime::Time);
			   Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->ItemIndex];
			   //Query->Parameters->ParamByName("ID_ProdType")->Value= ProductsExtended.Find(FormMain->ComboBoxProdFrom->ItemIndex).ID;

			   Query->Open();
			   bool IsThereAnyRemark = false;
			   bool AdditionalChange = false;
			   switch(ComboLineStatusToID[Combo->ItemIndex])
			   {
					case 1:
							if(Query->FieldByName("Additional")->IsNull && FormMain->MaskEdit1->Text!="  . ")
								AdditionalChange=true;
							else
							{
							   AdditionalChange=(Query->FieldByName("Additional")->AsString!= FormMain->MaskEdit1->Text);
							}
							break;
					case 4:
							if(Query->FieldByName("Additional")->IsNull && FormMain->ComboBoxProdTo->Text=="")
								AdditionalChange=true;
							else
							{
							   AdditionalChange=(Query->FieldByName("Additional")->AsString!= FormMain->ComboBoxProdTo->Text);
							}
							break;
					default:
							AdditionalChange=false;
							break;
							;
			   }




			   TStringList* Trial = new TStringList();
			   if(!Query->FieldByName("Remarks")->IsNull)
			   {
				  Trial->Text= Query->FieldByName("Remarks")->AsString;
				  IsThereAnyRemark = true;
			   }
			   else
			   {
				   IsThereAnyRemark = false;
			   }
//			   if(Query->RecordCount>0 && !AdditionalChange) // Can't update the same
//				{
//					 if(Remarks->Lines->Text.IsEmpty() && !IsThereAnyRemark)
//					 {
//						 return;
//					 }
//				}


		   if(Application->MessageBox(L"Chcesz zmienić zapisane dane? \n\nDo you want to change saved data",L"Uwaga / Attention!!",MB_YESNO)==IDYES)
		   {
			   DataModule1->ADOConnectionCreport->BeginTrans();
			   Query->Close();  //Updating in this case means Deleting and inserting
			   Query->SQL->Clear();

			   QueryString = "DELETE FROM   " + Database +
							 " WHERE ID_Workshiftday = :ID_Workshiftday AND Start_Time =:DateTime  	"
							 " AND   End_Time = :EndTime AND  ID_LineStatus = :ID_LineStatus            "
						   ;
			   Query->SQL->Add(QueryString);
			   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
			  // Query->Parameters->ParamByName("DateTime")->Value=Sheet->Cells[0][Sheet->Selection.Top];
			   Query->Parameters->ParamByName("DateTime")->Value=TDateTime(Sheet->Cells[0][Sheet->Selection.Top],TDateTime::Time);
			   Query->Parameters->ParamByName("EndTime")->Value=TDateTime(Sheet->Cells[1][Sheet->Selection.Top],TDateTime::Time);
			   Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->Items->IndexOf(Sheet->Cells[3][Sheet->Selection.Top])];





			   int NumberOfRowsAffected;
			   NumberOfRowsAffected = Query->ExecSQL();
			   Query->Close();
			   Query->SQL->Clear();
			   Query->Close();
			   Query->SQL->Clear();
			   MinDiffInt= MinuteDiff(TDateTime(Sheet->Cells[0][Sheet->Selection.Top],TDateTime::Time),TDateTime(Sheet->Cells[1][Sheet->Selection.Top],TDateTime::Time));
			   if (NumberOfRowsAffected>0) {
				   Current.SubstractMinutes(MinDiffInt);
			   }
			   if(IsThereAnyRemark)
			   {
					QueryString = " DELETE FROM   " + Database2 +
								 " WHERE ID_Workshiftday = :ID_Workshiftday AND Start_Time =:DateTime  	"
								 " AND   End_Time = :EndTime AND  ID_LineStatus = :ID_LineStatus        "
									   ;
				   Query->SQL->Add(QueryString);
				   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
				   Query->Parameters->ParamByName("DateTime")->Value=TDateTime(Sheet->Cells[0][Sheet->Selection.Top],TDateTime::Time);
				   Query->Parameters->ParamByName("EndTime")->Value=TDateTime(Sheet->Cells[1][Sheet->Selection.Top],TDateTime::Time);
				   Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->Items->IndexOf(Sheet->Cells[3][Sheet->Selection.Top])];

				   NumberOfRowsAffected = Query->ExecSQL();
				   Query->Close();
				   Query->SQL->Clear();
			   }
			   Current.AddMinutes(MinDiffInt);
			   QueryString =" INSERT INTO   " + Database +
							" (ID_Workshiftday, Start_Time, End_Time, ID_LineStatus,ID_ProdType,Additional) "
							" VALUES   (:ID_Workshiftday, :DateTime, :EndTime, :ID_LineStatus,:ID_ProdType,:Additional) "
															   ;
			   Query->SQL->Add(QueryString);

			   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
			   Query->Parameters->ParamByName("DateTime")->Value=Picker->Time.FormatString("h:nn");//TDateTime(Picker->Time.TimeString(),TDateTime::Time);
			   Query->Parameters->ParamByName("EndTime")->Value=PickerStop->Time.FormatString("h:nn");  //TDateTime(PickerStop->Time.TimeString(),TDateTime::Time);
			   Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->ItemIndex];
			   Query->Parameters->ParamByName("ID_ProdType")->Value= ProductsExtended.Find(FormMain->ComboBoxProdFrom->ItemIndex).ID;

			   switch(ComboLineStatusToID[Combo->ItemIndex])
			   {
					case 1:
							Query->Parameters->ParamByName("Additional")->Value= FormMain->MaskEdit1->Text; //put speed
							break;
					case 4:
							Query->Parameters->ParamByName("Additional")->Value= FormMain->ComboBoxProdTo->Text;
							break;
					default:
							Query->Parameters->ParamByName("Additional")->Value= 0;
							break;
							;
			   }

			   Query->ExecSQL();
			   Query->Close();
			   Query->SQL->Clear();

			   if(!Remarks->Lines->Text.IsEmpty())
			   {
				   QueryString =" INSERT INTO   " + Database2 +
							   " (ID_Workshiftday, Start_Time, End_Time, ID_LineStatus, Remarks ) "
							   " VALUES   (:ID_Workshiftday, :DateTime, :EndTime, :ID_LineStatus, :Remarks) "
																   ;
				   Query->SQL->Add(QueryString);

				   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
				   Query->Parameters->ParamByName("DateTime")->Value=Picker->Time.FormatString("h:nn");//TDateTime(Picker->Time.TimeString(),TDateTime::Time);
				   Query->Parameters->ParamByName("EndTime")->Value=PickerStop->Time.FormatString("h:nn");  //TDateTime(PickerStop->Time.TimeString(),TDateTime::Time);
				   Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->ItemIndex];
				   Query->Parameters->ParamByName("Remarks")->Value = Remarks->Lines->Text;

				   Query->ExecSQL();
				   Query->Close();
				   Query->SQL->Clear();
			   }
			   DataModule1->ADOConnectionCreport->CommitTrans();
		   }         //------------------   I do not want to upgrade or change any data
		   else                 // After Not updating any data
		   {
				   FillingUpSheetLineStatus();
				   EmptyFormLineStatus();
				   return;
		   }
	   }   // ---------------------  End of dealing with upgrade   --------------------
	   else   // Cursor is on empty cell - adding new batch
	   {
		   Query->SQL->Clear();
		   Current.AddMinutes(MinDiffInt);
		   QueryString =" INSERT INTO   " + Database +
				   " (ID_Workshiftday, Start_Time, End_Time, ID_LineStatus, ID_ProdType, Additional)				"
				   " VALUES  (:ID_Workshiftday, :DateTime, :EndTime, :ID_LineStatus,:ID_ProdType,:Additional) 	"
													   ;
		   Query->SQL->Add(QueryString);
		   int CheckID;
		   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
		   Query->Parameters->ParamByName("DateTime")->Value=Picker->Time.FormatString("h:nn");//TDateTime(Picker->Time.TimeString(),TDateTime::Time);
		   Query->Parameters->ParamByName("EndTime")->Value=PickerStop->Time.FormatString("h:nn");  //TDateTime(PickerStop->Time.TimeString(),TDateTime::Time);
		   Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->ItemIndex];
		   CheckID=  ProductsExtended.Find(FormMain->ComboBoxProdFrom->ItemIndex).ID ;
		   Query->Parameters->ParamByName("ID_ProdType")->Value= CheckID;
		   switch(ComboLineStatusToID[Combo->ItemIndex])
			   {
					case 1:
							Query->Parameters->ParamByName("Additional")->Value= FormMain->MaskEdit1->Text; //put speed
							break;
					case 4:
							Query->Parameters->ParamByName("Additional")->Value= FormMain->ComboBoxProdTo->Text;
							break;
					default:
							Query->Parameters->ParamByName("Additional")->Value= "0";
							break;
							;
			   }
         //  Query->Parameters->ParamByName("Remarks")->Value = Remarks->Lines->Strings;
		 //  Query->Prepared=true;
		 //  QueryString=Query->Parameters->ParseSQL(Query->SQL->Text,true);
           Query->ExecSQL();
           Query->Close();
           Query->SQL->Clear();

            if(!Remarks->Lines->Text.IsEmpty())
            {
			   QueryString =" INSERT INTO   " + Database2 +
                           " (ID_Workshiftday, Start_Time, End_Time, ID_LineStatus, Remarks) "
                           " VALUES   (:ID_Workshiftday, :DateTime, :EndTime, :ID_LineStatus, :Remarks) "
                                                               ;
               Query->SQL->Add(QueryString);

               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
               Query->Parameters->ParamByName("DateTime")->Value=Picker->Time.FormatString("h:nn");//TDateTime(Picker->Time.TimeString(),TDateTime::Time);
               Query->Parameters->ParamByName("EndTime")->Value=PickerStop->Time.FormatString("h:nn");  //TDateTime(PickerStop->Time.TimeString(),TDateTime::Time);
               Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->ItemIndex];
               Query->Parameters->ParamByName("Remarks")->Value = Remarks->Lines->Text;

               Query->ExecSQL();
               Query->Close();
               Query->SQL->Clear();
            }


    	}

	} // End of first try block

	catch(ErrorMain& K)
	{
		K.Print();
		return;
	}
	}  // End of second block
	__finally{
		 LastProdIndex=FormMain->ComboBoxProdFrom->ItemIndex ;
		 FillingUpSheetLineStatus();
		 EmptyFormLineStatus();
		 DataModule1->ADOConnectionCreport->Connected=false;
		 FormMain->ComboBoxLineStatusSelect(FormMain);

	}
}

void EmptyFormLineStatus()
{
    Picker = FormMain->DateTimePickerLineStatusStart;
    PickerStop = FormMain->DateTimePickerLineStatusStop;
    Contact = FormMain->EditMinuteLineStatus ;//Contact person
    Remarks = FormMain->MemoLineStatus;
 	Combo = FormMain->ComboBoxLineStatus;
    Sheet = FormMain->StringGridLineStatus;
   // Picker->
    Contact->Text="";
    Remarks->Clear();
	Combo->ItemIndex=-1;
	FormMain->ComboBoxLineStatusSelect(FormMain);
   //Picker->Time=PickerStop->Time;
   //Combo->Text="";

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


int MinuteDiff(const TDateTime Start, const TDateTime Stop)
{
    unsigned short MinutesDifference,HourStart,MinStart, HourStop,MinStop, Dummy;
    Start.DecodeTime(&HourStart,&MinStart,&Dummy,&Dummy);
    Stop.DecodeTime(&HourStop,&MinStop,&Dummy,&Dummy);
    int StartMin,StopMin;
    StartMin=HourStart*60 + MinStart ;
	StopMin =HourStop*60  + MinStop;
	if (StartMin > StopMin) {
        MinutesDifference = 24*60 - (StartMin-StopMin);
    }
    else
    {
        MinutesDifference = StopMin-StartMin;
    }
    return  MinutesDifference;
}
class TDefault
{
	double D;
	int I;
  public:
	TDefault(double L) {D=L;I=L; }
	TDefault(int L) {I =L;D=L;}
	TDefault() {D=0;I=0;}
	double GetDouble() {return D; }
	int GetInt() { return I; }


};

 void FillingUpSheetLineStatus()
 {
    Picker = FormMain->DateTimePickerLineStatusStart;
    PickerStop = FormMain->DateTimePickerLineStatusStop;
    Contact = FormMain->EditMinuteLineStatus ;//Contact person
    Remarks = FormMain->MemoLineStatus;
 	Combo = FormMain->ComboBoxLineStatus;
    Sheet = FormMain->StringGridLineStatus;
	TDateTime Start;
	TDateTime Stop;
	double TymDoub;
	int TymInt;
	std::map<int,TDefault> LineSpeed; //first int means ProdID
	std::map<int,TDefault> NumberOfProductionStatuses; // first int means ProdID, second as name
	int ProdID;
	TProdType Prod;
	//static TDateTime LastTime;
	try{
    try{
        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        AnsiString QueryString;
        EmptySheetLineStatus();
		int i=1;
		int ID;
		int MinDiffInt;
		std::map<int,int>::iterator pos;
		Current.SetSumOfMinutes(0);
       	Query->SQL->Clear();
		if(Current.GetShiftKind()==3)  // night shift
		{ 		QueryString =
							 "SELECT * FROM " +DataQuery + // :Database "
							 " WHERE  ID_Workshiftday = :ID_Workshiftday "
							 " AND  Start_Time > :Start_Time "
							 " ORDER BY Start_Time "
										;
				 Query->SQL->Add(QueryString);
				 Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
				 Query->Parameters->ParamByName("Start_Time")->Value=TDateTime(6,0,0,0);
				 Query->Open();
				 i=1;
				 while(!Query->Eof)
				   {
						Start = Query->FieldByName("Start_Time")->AsDateTime;
						Stop  = Query->FieldByName("End_Time")->AsDateTime;
						Sheet->Cells[0][i]= Start.FormatString("hh:nn");   //TimeString();
						Sheet->Cells[1][i]= Stop.FormatString("hh:nn");//TimeString();
						Sheet->Cells[2][i]= MinDiffInt=MinuteDiff(Start,Stop);
						ID =  Query->FieldByName("ID_LineStatus")->AsInteger;

						for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos) {
								if (pos->second == ID) {
								  Sheet->Cells[3][i]= Combo->Items->Strings[pos->first];
								  break;
								}
						}
						Prod =  ProductsExtended.FindByID(Query->FieldByName("ID_ProdType")->AsInteger);
						Sheet->Cells[4][i] = Prod.ShortName;

						Sheet->Cells[5][i]= Query->FieldByName("Additional")->AsString;
						Sheet->Cells[6][i]= Query->FieldByName("Remarks")->AsString;
						/* TODO : Add Description */

						i++;
						if (ID==1 && Prod.ID!=0) { // ID = 1 means production , ProdID==0 means that there were no prod definied in additional field
						try{
						   TymDoub = LineSpeed[Prod.ID].GetDouble();
						   TymDoub += MinDiffInt*Query->FieldByName("Additional")->AsFloat;
						   LineSpeed[Prod.ID]=TymDoub;
						   TymInt = NumberOfProductionStatuses[Prod.ID].GetInt();
						   TymInt+= MinDiffInt;
						   NumberOfProductionStatuses[Prod.ID]=TymInt;
						   }
						   catch(EConvertError &E)
						   {
							 // do nothing throw E;
						   }

						}
						Query->Next();
						Current.SetSumOfMinutes(Current.GetSumOfMinutes() + MinDiffInt);


						if(i>=Sheet->RowCount)
							Sheet->RowCount=Sheet->RowCount+1;
				   }
				   Query->Close();
				   QueryString =
							 "SELECT * FROM " +DataQuery + // :Database "
							 " WHERE  ID_Workshiftday = :ID_Workshiftday "
							 " AND  Start_Time < :Start_Time  "
							 " ORDER BY Start_Time "
										;
				 Query->SQL->Clear();
				 Query->SQL->Add(QueryString);
				 Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
				 Query->Parameters->ParamByName("Start_Time")->Value=TDateTime(6,0,0,0);
				 Query->Open();

				 while(!Query->Eof)
				   {
						Start = Query->FieldByName("Start_Time")->AsDateTime;
						Stop  = Query->FieldByName("End_Time")->AsDateTime;
						Sheet->Cells[0][i]= Start.FormatString("hh:nn");   //TimeString();
						Sheet->Cells[1][i]= Stop.FormatString("hh:nn");//TimeString();
						Sheet->Cells[2][i]= MinDiffInt=MinuteDiff(Start,Stop);
						ID =  Query->FieldByName("ID_LineStatus")->AsInteger;
						for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos) {
								if (pos->second == ID) {
								  Sheet->Cells[3][i]= Combo->Items->Strings[pos->first];
								  break;
								}
						}
						Prod =  ProductsExtended.FindByID(Query->FieldByName("ID_ProdType")->AsInteger);
						Sheet->Cells[4][i] = Prod.ShortName;

						Sheet->Cells[5][i]= Query->FieldByName("Additional")->AsString;
						Sheet->Cells[6][i]= Query->FieldByName("Remarks")->AsString;
						i++;
						if (ID==1 && Prod.ID!=0) { // ID = 1 means production , ProdID==0 means that there were no prod definied in additional field
						try{
						   TymDoub = LineSpeed[Prod.ID].GetDouble();
						   TymDoub += MinDiffInt*Query->FieldByName("Additional")->AsFloat;
						   LineSpeed[Prod.ID]=TymDoub;
						   TymInt = NumberOfProductionStatuses[Prod.ID].GetInt();
						   TymInt+= MinDiffInt;
						   NumberOfProductionStatuses[Prod.ID]=TymInt;
						   }
						   catch(EConvertError &E)
						   {
							 // do nothing throw E;
						   }

						}
						Query->Next();
						Current.SetSumOfMinutes(Current.GetSumOfMinutes() + MinDiffInt);
						if(i>=Sheet->RowCount)
							Sheet->RowCount=Sheet->RowCount+1;
				   }
		}
		//------------------   If there are different shifts than night
		else
		{
			 QueryString =
						 "SELECT * FROM " +DataQuery + // :Database "
						 " WHERE  ID_Workshiftday = :ID_Workshiftday "
						 " ORDER BY Start_Time "
									;
			 Query->Close();
			 Query->SQL->Clear();
			 Query->SQL->Add(QueryString);
			 Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=Current.GetID();
			 Query->Open();

			 while(!Query->Eof)
			   {
					Start = Query->FieldByName("Start_Time")->AsDateTime;
					Stop  = Query->FieldByName("End_Time")->AsDateTime;
					Sheet->Cells[0][i]= Start.FormatString("hh:nn");   //TimeString();
					Sheet->Cells[1][i]= Stop.FormatString("hh:nn");//TimeString();
					Sheet->Cells[2][i]= MinDiffInt = MinuteDiff(Start,Stop);
					ID =  Query->FieldByName("ID_LineStatus")->AsInteger;
						for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos) {
								if (pos->second == ID) {
								  Sheet->Cells[3][i]= Combo->Items->Strings[pos->first];
								  break;
								}
						}
					Prod =  ProductsExtended.FindByID(Query->FieldByName("ID_ProdType")->AsInteger);
					Sheet->Cells[4][i] = Prod.ShortName;
					Sheet->Cells[5][i]= Query->FieldByName("Additional")->AsString;
					Sheet->Cells[6][i]= Query->FieldByName("Remarks")->AsString;
					i++;
					if (ID==1 && Prod.ID!=0) { // ID = 1 means production , ProdID==0 means that there were no prod definied in additional field
						  try{
						   TymDoub = LineSpeed[Prod.ID].GetDouble();
						   TymDoub += MinDiffInt*Query->FieldByName("Additional")->AsFloat;
						   LineSpeed[Prod.ID]=TymDoub;
						   TymInt = NumberOfProductionStatuses[Prod.ID].GetInt();
						   TymInt+= MinDiffInt;
						   NumberOfProductionStatuses[Prod.ID]=TymInt;
						   }
						   catch(EConvertError &E)
						   {
							 // do nothing throw E;
						   }

						}
					Query->Next();
					Current.SetSumOfMinutes(Current.GetSumOfMinutes() + MinDiffInt);
					if(i>=Sheet->RowCount)
						Sheet->RowCount=Sheet->RowCount+1;
			   }
		}
	   Picker->Time=Stop;
	   PickerStop->Time=GetShiftTime(Current.GetShiftKind(),1); //1 means end shift time
	  // Sheet->Cells[1][i] = "Suma = ";
	  // Sheet->Font->Style >> fsBold;
	  // Sheet->Cells[2][i] = Current.GetSumOfMinutes();
	  FormMain->LabelLineStatusSumOfMinutes->Refresh();
	  UnicodeString Napis =  Current.GetSumOfMinutes();
	  FormMain->LabelLineStatusSumOfMinutes->Caption = L"Sumaryczna ilość minut = " + Napis;

	   FormMain->LabelLineStatusSumOfMinutes->Refresh();
	   TGridRect R;
		R.Left=0;
		R.Top= i ;
		R.Right= 5 ;
		R.Bottom = i  ;
		FormMain->LabelLineStatusSumOfMinutes->Refresh();
		FormMain->LabelLineStatusSumOfMinutes->Visible=true;
		Sheet->Selection=R;
//		if (NumberOfProductionStatuses!=0) {
//			LineSpeed = LineSpeed/NumberOfProductionStatuses; // Average LineSpeed
//		}
		std::map<int, TDefault>::iterator it;
		for(it=LineSpeed.begin();it!=LineSpeed.end();it++)
		{
		   if(NumberOfProductionStatuses[it->first].GetInt()!=0)
		   {
			 Current.SetAverageLineSpeed(it->first,it->second.GetDouble()/NumberOfProductionStatuses[it->first].GetInt());
		   }
		}

	  }

	  catch(EConvertError &E)
	  {
		throw E;
	  }
	  }
	  __finally{
				DataModule1->ADOConnectionCreport->Connected=false;
				FormMain->ComboBoxLineStatusSelect(FormMain);
	  }

 }



 //EmptyFormSheetLineStatus();


void LineStatusFillUpForm(int Index)
{
	Picker = FormMain->DateTimePickerLineStatusStart;
	PickerStop = FormMain->DateTimePickerLineStatusStop;
	Contact = FormMain->EditMinuteLineStatus ;//Contact person
	Remarks = FormMain->MemoLineStatus;
	Combo = FormMain->ComboBoxLineStatus;
	Sheet = FormMain->StringGridLineStatus;
	TDateTime Start,Stop;
	int StatusID;
	try{
		TADOQuery* Query =DataModule1->ADOQueryCreport;
		DataModule1->ADOConnectionCreport->Connected=true;
		AnsiString QueryString, Message;
		Query->SQL->Clear();
		QueryString =
					 "SELECT * FROM " +DataQuery + // :Database "
					 " WHERE  ID_Workshiftday = :ID_Workshiftday AND Start_Time =:DateTime   "
					 " AND  End_Time  =  :EndTime     AND  ID_LineStatus =   :ID_LineStatus   "
								;
		Query->SQL->Add(QueryString);
	   // TDateTimeFlag Flaga = Time;
		Start = TDateTime(Sheet->Cells[0][Index],TDateTime::Time);
		Stop = TDateTime(Sheet->Cells[1][Index],TDateTime::Time);
		StatusID = ComboLineStatusToID[Combo->Items->IndexOf(Sheet->Cells[3][Index])];
		Query->Parameters->ParamByName("ID_Workshiftday")->Value=Current.GetID();
		Query->Parameters->ParamByName("DateTime")->Value=TDateTime(Sheet->Cells[0][Index],TDateTime::Time);
		Query->Parameters->ParamByName("EndTime")->Value=TDateTime(Sheet->Cells[1][Index],TDateTime::Time);
		Query->Parameters->ParamByName("ID_LineStatus")->Value= ComboLineStatusToID[Combo->Items->IndexOf(Sheet->Cells[3][Index])];
		Query->Open();
		if(Query->RecordCount!=1)
		{
			Message = "Wystąpił błąd. Zapytanie po selekcji w akuszu wyników zwróciło " + AnsiString(Query->RecordCount);
			Message+= " wyników ";
			Message+="\n\n";
			Message+= "There is error. Query when selecting on result spreadsheet received " + AnsiString(Query->RecordCount) ;
			Message+= " results";
			ShowMessage(Message);
		}
		PickerStop->Time= Query->FieldByName("End_Time")->AsDateTime;
		Picker->Time=  Query->FieldByName("Start_Time")->AsDateTime;	//TDateTime(Query->FieldByName("Start_Time")->AsString,TDateTime::Time);
		int ID = Query->FieldByName("ID_LineStatus")->AsInteger  ;
		std::map<int,int>::iterator pos;
		for (pos = ComboLineStatusToID.begin(); pos != ComboLineStatusToID.end(); ++pos) {
			if (pos->second == Query->FieldByName("ID_LineStatus")->AsInteger) {
						Combo->ItemIndex=pos->first;
						break;
			}
		}
		Remarks->Lines->Text=Query->FieldByName("Remarks")->AsString;
		FormMain->ComboBoxProdFrom->ItemIndex=LastProdIndex;/* TODO : Usunąć */
		if(Query->FieldByName("ID_ProdType")->AsInteger !=0 )
				FormMain->ComboBoxProdFrom->ItemIndex = ProductsExtended.FindComboIndex(Query->FieldByName("ID_ProdType")->AsInteger);
		if(ID==1) //Production
		{
		   if(!Query->FieldByName("Additional")->IsNull)
			   FormMain->MaskEdit1->Text = Query->FieldByName("Additional")->AsString;
		}
		else
		{
		  if(!Query->FieldByName("Additional")->IsNull)
			FormMain->ComboBoxProdTo->Text=Query->FieldByName("Additional")->AsString;
		}
		Query->Close();
		FormMain->EditMinuteLineStatus->Text=Sheet->Cells[2][Index];


	}   // end of try
	__finally{
		DataModule1->ADOConnectionCreport->Connected=false;
		FormMain->ComboBoxLineStatusSelect(FormMain);
	}
}

void TStatus::AddRecord(int aID, UnicodeString aPolishName, UnicodeString aEnglishName, UnicodeString aPolishDescription, UnicodeString aEnglishDescription, int aCategoryID, bool aActive, bool IsItScheduled)
{
   ID = aID;
   PolishName = aPolishName;
   EnglishName = aEnglishName;
   PolishDescription = aPolishDescription;
   EnglishDescription = aEnglishDescription;
   CategoryID = aCategoryID;
   Active = aActive;
   IsScheduled = IsItScheduled;
}

bool TStatus::IsItScheduled()
{
  return IsScheduled;
}

int TStatus::GetID()
{
  if (ID!=0) {
	   return ID;
  }
	return ID;
}

UnicodeString TStatus::GetPolishName()
{
	return PolishName;
}

UnicodeString TStatus::GetEnglishName()
{
	return EnglishName;
}


UnicodeString TStatus::GetPolishDescription()
{
	return PolishDescription;
}

UnicodeString TStatus::GetEnglishDescription()
{
	return EnglishDescription;
}

bool TStatus::GetActiveStatus()
{
	return Active;
}
bool TStatus::operator==(TStatus Por)
{
	return ID == Por.GetID();
}

void TLineStatus::Add(int aID, UnicodeString aPolishName, UnicodeString aEnglishName, UnicodeString aPolishDescription, UnicodeString aEnglishDescription, int aCategoryID, bool aActive, bool IsItScheduled)
{
   TStatus Temp;
   Temp.AddRecord(aID, aPolishName, aEnglishName, aPolishDescription, aEnglishDescription,aCategoryID, aActive, IsItScheduled);
   std::list<TStatus>::iterator iter;
   int IDS ;
   UnicodeString napis;
   for(iter = Entity.begin();iter != Entity.end() ;iter++ )
   {
	  IDS = iter->GetID();
	  napis = iter->GetPolishName();
	  if (Temp==*iter) {
		   return;				//insert only unique ID's.
	  }
   }
	  Entity.push_back(Temp);
}

TLineStatus::TLineStatus(int a)
{
	Entity.resize(a+5);
}



TStatus* TLineStatus::GetStatus(int ID)
//Return pointer to status with given ID and 0 when appropirate status not found
{
   std::list<TStatus>::iterator iter;

   for(iter = Entity.begin();iter != Entity.end() ;iter++ )
   {
	  int indeks = iter->GetID()   ;
	  UnicodeString Opis = iter->GetPolishName();
	  if (indeks == ID) {
		   return &(*iter);				//insert only unique ID's.
	  }
   }
   return 0;
}
