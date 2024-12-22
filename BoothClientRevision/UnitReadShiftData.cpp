//---------------------------------------------------------------------------


#pragma hdrstop

//#include "UnitShiftReading.h"
#include "UnitMainBooth.h"
//#include "UnitMain.h"
#include "DataBooth.h"
//#include "Global.h"
#include  "UnitReadShiftData.h"
#include "UnitProductsBooth.h"
#include "GlobalBooth.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

extern TSuperviosorCompare SupervisorsCompare;


TMainEntity::TMainEntity()
{
    Remarks = 0;
    ExistsInDatabase=false;
	Query = 0;
	IdGIP=0;
	Id =0;
}

TMainEntity::~TMainEntity()
{
	if(Remarks!=0)
		delete Remarks;
}

bool TMainEntity::ReadFromForm()
{
	Date= FormMain->DateTimePickerMain->DateTime;
	ShiftLetter=FormMain->ComboBoxShiftLetter->ItemIndex+1;
	ShiftType=FormMain->ComboBoxShiftKind->ItemIndex+1;
	//ShiftSupervisor = FormMain->Supervisors[FormMain->ComboBoxSuperVisors->Text];
	//ShiftSupervisorGIP= FormMain->SupervisorsGIP[FormMain->ComboBoxSuperVisorsGIP->Text];
	//ProcessOperator = FormMain->ProcessOperators[FormMain->ComboBoxProcessOperator->Text];
	//BoothOperator = FormMain->BoothOperators[FormMain->ComboBoxBoothOperator->Text];
	//NoOfOperators=FormMain->EditNumberOfOperators->Text.ToInt();
//    if(Remarks!=0)
//        	delete Remarks;
//    if(!FormMain->MemoGeneralRemarks->Text.IsEmpty())
//    {
//
//        Remarks = new TStringList;
//        Remarks->Text=  FormMain->MemoGeneralRemarks->Text;
//       // Remarks =dynamic_cast<TStringList*>(FormMain->MemoProductRemarks->Lines);
//    }
//    else
//    {
//        Remarks = 0;
//    }
	return true;
}

//methode returns true if given by form workshift entity exists in database;
// otherwise returns false
bool TMainEntity::CheckIfIsInDatabase(TDateTime DateR,int ShiftLetterR, int ShiftR)
{
try{
	Query = DataModule2->ADOQueryCreport;
   // DataBooth->ADOConnectionCreport->Connected=true;

	AnsiString QueryString,Name;
	Query->SQL->Clear();
	int TymID;
	QueryString = "SELECT * FROM WorkshiftDays " //LEFT JOIN WorkshiftDaysRemarks ON WorkshiftDays.ID_WORKSHIFTDAY = WorkshiftDaysRemarks.ID_WorkshiftDay "
				  "WHERE DateFromParts(DatePart(year,WorkshiftDays.Date_OF_WORK),DatePart(month,WorkshiftDays.Date_OF_WORK),DatePart(day,WorkshiftDays.Date_OF_WORK)) = "
				  "		 DateFromParts(DatePart(year,:DateR),DatePart(month,:DateR1),DatePart(day,:DateR2)) ";
	 Query->SQL->Add(QueryString);
	 Query->Parameters->ParamByName("DateR")->Value=DateR;
	 Query->Parameters->ParamByName("DateR1")->Value=DateR;
	 Query->Parameters->ParamByName("DateR2")->Value=DateR;
	 if(ShiftLetterR>0  && ShiftR==0) // Only if we do not know the shift i.e. night,day or afternoon
	 {
	   QueryString =  " AND   ID_SHIFTGROUP = :ShiftLetterR  ";
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("ShiftLetterR")->Value=ShiftLetterR;
	 }
	 if(ShiftR>0)
	 {
	   QueryString =  " AND ID_SHIFTTIME  = :ShiftR   ";
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("ShiftR")->Value=ShiftR;
	 }

	Query->Open();
	AnsiString Message;
	if (Query->RecordCount>1) {
		 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
		 Message+=" Istnieje wiêcej ni¿ jeden zapis o tej samej dacie (dniu) i rodzaju zmiany.";
		 Message+="\nOdczytany zostanie tylko pierwszy zapis!\n\n";
		 Message+="There is mistake in database!  More than one entity \n";
		 Message+="with the same date and shift kind exists. There only one will be read !";
		 wchar_t* Buf;
		 Application->MessageBox(Message.WideChar(Buf,Message.WideCharBufSize()),L"Uwaga - Attention!!!",MB_OK);
		 if(ShiftLetter>0) // if we know the seekeing shift letter and there are more than one database with given date and shift kind
		 {
			 Query->Locate("ID_ShiftGroup",ShiftLetter,TLocateOptions());        //Make this record current
		 }
	}
	if(Query->RecordCount>=1)
	{
		ExistsInDatabase = true;
		Id = Query->FieldByName("ID_Workshiftday")->AsInteger;
		return true;
	}
	else
	{
		ExistsInDatabase = false;
		return false;
	}
  }
  __finally{
	 //  DataBooth->ADOConnectionCreport->Connected=false;
  }
}

//char GetShiftLetter(int ShiftLetter) // general purpose function
//   {
//	   char* Sh="ABCD";
//	   char Letter =  Sh[ShiftLetter-1];
//	   return  Letter;
//   }


 char GetShiftLetter(int ShiftLetter) // general purpose function
   {
	   char* Sh="ABCD";
	   char Letter =  Sh[ShiftLetter-1];
	   return  Letter;
   }


bool TMainEntity::ReadEntity()
{
	AnsiString QueryString;
	Query->SQL->Clear();
	QueryString = "SELECT * FROM Workshiftdays WHERE WorkshiftDays.ID_WorkshiftDay=:ID";
	Query->SQL->Add(QueryString);
	Query->Parameters->ParamByName("ID")->Value=Id;
	Query->Open();
  //  int TymID =Query->FieldByName("WorkshiftDays.ID_WorkshiftDay")->AsInteger;

  //   Id = TymID;
	ShiftLetter=Query->FieldByName("ID_SHIFTGROUP")->AsInteger;
	ShiftType = Query->FieldByName("ID_SHIFTTIME")->AsInteger;
   //	ShiftSupervisor=Query->FieldByName("ID_SUPERVISOR")->AsInteger;
   //	NoOfOperators= Query->FieldByName("OPERATORS")->AsInteger;
	Date=Query->FieldByName("DATE_OF_WORK")->AsDateTime;
	TDateTime Start = Date;
	char Shiftchar[4]={'A','B','C','D'};
   //	void ComboOperatorFillUP(AnsiString Shift, bool IsSupervisor,TComboBox* Combo, std::map<AnsiString,int>& Operators, bool All=false ); //function prototype
	/* TODO 1 : Poprawiæ uzupe³nianie Combo */
   //	ComboOperatorFillUP(Shiftchar[ShiftLetter-1],true,FormMain->ComboBoxSuperVisorsGIP,FormMain->SupervisorsGIP,true);
   //	ComboOperatorFillUP(Shiftchar[ShiftLetter-1],false,FormMain->ComboBoxProcessOperator,FormMain->ProcessOperators,true);
   //	ComboOperatorFillUP(Shiftchar[ShiftLetter-1],false,FormMain->ComboBoxBoothOperator,FormMain->BoothOperators,true);
	QueryString = "WorkshiftID\tShift\tShiftTime\tSupervisorID\tNoOfOperators\t\tDateOfWork\n\n";
	QueryString+=AnsiString(Id)+"\t\t"+AnsiString(ShiftLetter)+"\t\t"+AnsiString(ShiftType)+"\t\t"+AnsiString(ShiftSupervisor)+"\t\t"+AnsiString(NoOfOperators)+"\t\t"+Date.DateTimeString();
	ExistsInDatabase=true;
   // ShowMessage(QueryString);
	bool SelfConnectedGIP = false;       // Close connection if it were connected in this methode
  //	bool SelfConnected = false;
	try{
		 TADOQuery* QueryGIP = DataModule2->ADOQueryGIP_Seek;

		 if(!DataModule2->ADOConnectionCreport->Connected)
		 {
			 DataModule2->ADOConnectionCreport->Connected=true;
			 SelfConnectedGIP = true;
		 }


		IdGIP=  GetGIPDocNum();
		int SvGIPNo;
	 //	QueryGIP->Parameters->Clear();
		QueryGIP->SQL->Clear();

		//if there is no GIP data then make new GIP record
//		if (IdGIP==0) {
//			IdGIP = NewIdGIP();
//			SvGIPNo = SupervisorsCompare.GetGIP_No(ShiftSupervisor);
//			if(GetShiftKind()==3)  // if it is nightshift
//			{
//					Start -=1;
//			}
//			if (IdGIP!=0 && SvGIPNo>0) {
//
//			  QueryString = "Insert into CP_HEAD  (USER_SV, DocNum) select :SvGIPNo, :IdGIP ";
//			  QueryString = "Insert into CP_HEAD  (USER_SV, DocNum) Values( " + AnsiString(SvGIPNo);
//			  QueryString += " ,\"" + AnsiString(IdGIP) + "\" ) ";
//			  QueryString =	"INSERT INTO CP_HEAD(  DOCNUM,  USER_SV,     "
//							"  SHIFTSTARTDATE,    DATA, [TIME] )         "
//							"VALUES   ( :IdGIP,                          "
//							"			 :ShiftSupervisorGIP "
//							"		 , :SHIFTSTARTDATE "
//							"        , :Datee, :Time);" ;
//
//			  AnsiString ExchangeAnsStr(AnsiString& A, AnsiString Previous, AnsiString Next);   //function prototype
//			  ExchangeAnsStr(QueryString,":IdGIP","\""+IdGIP+"\"");
//			  ExchangeAnsStr(QueryString,":ShiftSupervisorGIP",SvGIPNo);
//			  ExchangeAnsStr(QueryString,":Datee","\""+Start.DateTimeString()+"\"");
//			  ExchangeAnsStr(QueryString,":Time","\""+Start.TimeString()+"\"");
//			  ExchangeAnsStr(QueryString,":SHIFTSTARTDATE","\""+Date.CurrentDateTime().DateTimeString()+"\"");
//			  QueryGIP->SQL->Clear();
//			  QueryGIP->SQL->Add(QueryString);
////			  QueryGIP->Parameters->ParamByName("IdGIP")->Value=  IdGIP;
////			  QueryGIP->Parameters->ParamByName("SvGIPNo")->Value=  SvGIPNo;
//			  QueryGIP->ExecSQL();
//
//			} else
//			{
//				return false;
//			}
//
//		}

		if(IdGIP!=0)
		{
				QueryString = "SELECT DocNum,USER_SV, USER_CO, USER_DO  FROM CP_Head WHERE DocNum=:DocNum";
				QueryGIP->Parameters->Clear();
				QueryGIP->SQL->Clear();
				QueryGIP->SQL->Add(QueryString);
				QueryGIP->Parameters->ParamByName("DocNum")->Value=  IdGIP;
				QueryGIP->Open();
				ProcessOperator=QueryGIP->FieldByName("User_Co")->AsInteger;
				ShiftSupervisorGIP = QueryGIP->FieldByName("USER_SV")->AsInteger;
				BoothOperator= QueryGIP->FieldByName("User_Do")->AsInteger;
				UnicodeString SupervisorG = SupervisorsCompare.GetGIPName(ShiftSupervisorGIP);
				UnicodeString SuposedSuper = FormMain->CurrentSupervisorsGIP[::GetShiftLetter(ShiftLetter)];
				SuposedSuper = SupervisorsCompare.GetGIP(SuposedSuper);
				if (SupervisorG!=SuposedSuper)
				{
					FormMain->CheckBoxLeadman->Checked=true;
				   //q1	FormMain->CheckBoxLeadmanClick(this);
				}
				else
				{
					FormMain->CheckBoxLeadman->Checked=false;
				}
		}
//		if(!DataModule2->ADOConnectionCreport->Connected)
//		 {
//			 DataModule2->ADOConnectionCreport->Connected=true;
//			 SelfConnectedGIP = true;
//		 }
		return true;
	}
	__finally{
		if(SelfConnectedGIP)
		{
		   DataModule2->ADOConnectionCreport->Connected=false;
		}
	}
}

void TMainEntity::WriteToForm(bool operators)
{
	std::map<AnsiString,int>::iterator pos;
	for (pos = FormMain->Supervisors.begin(); pos != FormMain->Supervisors.end(); ++pos) {
			if (pos->second == ShiftSupervisor) {
						FormMain->ComboBoxSuperVisors->ItemIndex=FormMain->ComboBoxSuperVisors->Items->IndexOf(pos->first);
					  //	FormMain->ComboBoxSuperVisorsGIP->Text =FormMain->ComboBoxSuperVisors->Text;
						break;
			}
	}
	if (IdGIP==0) {    // if there are no data for GIP database.

						FormMain->ComboBoxSuperVisorsGIP->Text =FormMain->ComboBoxSuperVisors->Text;

	}
	else
	{
		for (pos = FormMain->SupervisorsGIP.begin(); pos != FormMain->SupervisorsGIP.end(); ++pos) {
			if (pos->second == ShiftSupervisorGIP) {
					  //	FormMain->ComboBoxSuperVisorsGIP->ItemIndex=FormMain->ComboBoxSuperVisorsGIP->Items->IndexOf(pos->first);
							FormMain->ComboBoxSuperVisorsGIP->Text= pos->first;
						break;
			}
		}
		if(pos == FormMain->SupervisorsGIP.end())  // probably the user is inactive
		{
			/* TODO : FillUp the lacking code for dealing with previous supervisors */
			FormMain->ComboBoxSuperVisorsGIP->Text = "Inactive user";
		}
	}



	for (pos = FormMain->ProcessOperators.begin(); pos != FormMain->ProcessOperators.end(); ++pos) {
		if (pos->second == ProcessOperator) {
					FormMain->ComboBoxProcessOperator->ItemIndex=FormMain->ComboBoxProcessOperator->Items->IndexOf(pos->first);
					break;
		}
	}


	for (pos = FormMain->BoothOperators.begin(); pos != FormMain->BoothOperators.end(); ++pos) {
		if (pos->second == BoothOperator) {
					FormMain->ComboBoxBoothOperator->ItemIndex=FormMain->ComboBoxBoothOperator->Items->IndexOf(pos->first);
					break;
		}
	}



	//FormMain->ComboBoxSuperVisors->ItemIndex = FormMain->ComboBoxSuperVisors->Items->IndexOf(FormMain->Supervisors.   [ShiftSupervisor]);  // ShiftSupervisor=Query->FieldByName("ID_SUPERVISOR")->AsInteger;
	if (operators) {                          // if filling up only operators position then exit
		 return;
	}
	FormMain->ComboBoxShiftLetter->ItemIndex=  ShiftLetter-1;
	FormMain->ComboBoxShiftKind->ItemIndex=ShiftType-1;
	//FormMain->UpDownNumberOfOperators->Position =  NoOfOperators;
	FormMain->DateTimePickerMain->DateTime= Date;
//	if(Remarks!=0)
//	{
//		FormMain->MemoGeneralRemarks->Text=Remarks->Text;
//	}
//	else
//	{
//		FormMain->MemoGeneralRemarks->Text="";//.Delete(0,FormMain->MemoGeneralRemarks->Text.Length());
//	}

}

AnsiString TMainEntity::GetDescription()
{
	char LettersShift[4]={'A','B','C','D'};
	char ShiftL = LettersShift[ShiftLetter-1]; // Leters are counted from 1;
	AnsiString ShiftString, SupVis="Unknown";
	std::map<AnsiString,int>::iterator pos;
//	for(pos= FormMain->SupervisorsG.begin(); pos!=FormMain->Supervisors.end(); pos++)
//	{
//		if(pos->second==ShiftSupervisor)
//		{
//			SupVis = pos->first;
//			break;
//		}
//	}
	for(pos= FormMain->SupervisorsGIP.begin(); pos!=FormMain->SupervisorsGIP.end(); pos++)
	{
		if(pos->second==ShiftSupervisorGIP)
		{
			SupVis = pos->first;
			break;
		}
	}
	//FormMain->Supervisors[FormMain->ComboBoxSuperVisors->Text]
	switch (ShiftType) {
		case 1:
				ShiftString = "Zmiana ranna - morning shift";
				break;
		case 2:
				ShiftString = "Zmiana popo³udniowa - afternoon shift";
				break;
		case 3:
				ShiftString = "Zmiana nocna - night shift";
				break;
    default:
        ;
    }
    return "      "+Date.DateString()+"   "+ShiftL+"    "+ShiftString + "  "+ SupVis + "    " + NoOfOperators;
}

bool TMainEntity::WriteEntity()
{

//	ReadFromForm();     //Populate Current with datasheet data.
//	FormMain->PageControl1->Enabled=true;
//try{
//try{
//	if(!DataModule2->ADOConnectionCreport->Connected)
//		 {
//			 DataModule2->ADOConnectionCreport->Connected=true;
//		 }
//	TADOQuery* QueryGIP = DataModule2->ADOQueryGIP_Seek;
//	AnsiString QueryString,QueryStringGIP,Name; //,QueryMemo
//	Query->SQL->Clear();
//	int NoOfRows 		= 0;
//	int NoOfRowsGIP		= 0;
//	Query->SQL->Clear();
//	Query->Parameters->Clear();
//	QueryGIP->SQL->Clear();
//
//	if(ExistsInDatabase)
//	{
//		QueryString = "UPDATE WorkshiftDays  "
//					  "SET  ID_ShiftGroup = :ShiftLetter, ID_ShiftTime = :ShiftType, "
//					  "		ID_Supervisor = :ShiftSupervisor, Operators =:NoOfOperators, "
//					  "     Date_of_work  = :Datee 							"
//					  "WHERE   ID_Workshiftday = :ID_Workshiftday ;			"
//					  ;
//		QueryStringGIP= "UPDATE CP_HEAD		"
//						"SET 	User_Co =:ProcessOperator     "
//						", 	 	USER_SV = :ShiftSupervisorGIP "
//						", 		User_Do = :BoothOperator      "
//						", 		Data = :Datee		  	 	  "
//						",		[Time] = :Time			 	  "
//						",		Shift = :ShiftLongG			  "
//						",		SHIFTSTARTDATE = :SHIFTSTARTDATE "
//						" WHERE  DocNum = :IdGIP			  "
//						;
//	}
//	else     /* TODO :
//In the GIP table the shift is recognized by date,
//so try to organize not only the date but also correct time and combine them into date */
//	{
//		 QueryString = "INSERT INTO WorkshiftDays(ID_ShiftGroup, ID_ShiftTime, "
//					   " ID_Supervisor,  Operators,  Date_of_work)   "
//					   "VALUES  ( :ShiftLetter, :ShiftType, 	        "
//					   "		 :ShiftSupervisor,  :NoOfOperators,     "
//
//					   "         :Datee )								"
//					   ;
//
//		 QueryStringGIP= 	"INSERT INTO CP_HEAD(  DOCNUM, USER_CO, USER_SV,   "
//							"  SHIFTSTARTDATE,   USER_DO, DATA, [TIME], SHIFT ) "
//							"VALUES   ( :IdGIP,								   "
//							"			:ProcessOperator, :ShiftSupervisorGIP  "
//							"		 , :SHIFTSTARTDATE						            "
//							"           , :BoothOperator, :Datee, :Time, :ShiftLongG ); "
//						;
//					IdGIP=NewIdGIP();
//
//	}
//	Query->SQL->Add(QueryString);
//
//	if(ExistsInDatabase)
//	{
//	  Query->Parameters->ParamByName("ID_Workshiftday")->Value = Id;
//	}
//   // Query->Parameters->ParamByName("Id")->Value=Id+1;
//	Query->Parameters->ParamByName("ShiftLetter")->Value=ShiftLetter;
//	Query->Parameters->ParamByName("ShiftType")->Value=ShiftType;
//	Query->Parameters->ParamByName("ShiftSupervisor")->Value=ShiftSupervisor;
//	Query->Parameters->ParamByName("NoOfOperators")->Value=NoOfOperators;
//	Query->Parameters->ParamByName("Datee")->Value=Date;
//
//
////	QueryGIP->Parameters->Clear();
////	QueryGIP->Parameters->Capacity=20;
//
////	ExchangeAnsStr(QueryStringGIP,":IdGIP","\""+IdGIP+"\"");
////	ExchangeAnsStr(QueryStringGIP,":IdGIP","\""+IdGIP+"\"");
////	ExchangeAnsStr(QueryStringGIP,":IdGIP","\""+IdGIP+"\"");
//	/* TODO 1 : Dokoñczyæ */
////	QueryGIP->SQL->Add(QueryStringGIP);
//	int Liczba = QueryGIP->Parameters->Count, CheckValue;
////	AnsiString NamePar;
////	for (int i = 0; i < Liczba; i++) {
////		 NamePar = QueryGIP->Parameters->Items[i]->Name;
////	}
//	char Shiftchar[4]={'A','B','C','D'};
//	AnsiString ShiftLetterStr = Shiftchar[ShiftLetter-1];
//
//
////	QueryGIP->Parameters->ParamByName("ProcessOperator")->Value=ProcessOperator;
////	QueryGIP->Parameters->ParamByName("ShiftSupervisorGIP")->Value=ShiftSupervisorGIP;
////	QueryGIP->Parameters->ParamByName("BoothOperator")->Value=BoothOperator;
////	QueryGIP->Parameters->ParamByName("ShiftLongG")->Value=ShiftLetterStr;//.c_str();
//
////	QueryGIP->Parameters->ParamByName("IdGIP")->Value= IdGIP ;
////	TParameter* Param;
////	Param =  QueryGIP->Parameters->ParamByName("IdGIP");
////	Param->DataType = ftString;
////	Param->Value =  IdGIP ;
////	QueryGIP->Parameters->ParamValues["IdGIP"]=IdGIP;
//
////	QueryGIP->Parameters->ParamByName("Datee")->Value=Date.DateString();
////	QueryGIP->Parameters->ParamByName("Time")->Value=Date.TimeString();
////	QueryGIP->Parameters->ParamByName("SHIFTSTARTDATE")->Value=Date.CurrentDateTime().DateTimeString();
////
////	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("ProcessOperator")->Value);
////	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("ShiftSupervisorGIP")->Value);
////	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("BoothOperator")->Value );
////	FormMain->MemoTest->Lines->Add(	QueryGIP->Parameters->ParamByName("ShiftLongG")->Value );
////	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("Datee")->Value);
////	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("Time")->Value);
//
//
//	AnsiString ExchangeAnsStr(AnsiString& A, AnsiString Previous, AnsiString Next);
//	ExchangeAnsStr(QueryStringGIP,":IdGIP","\'"+IdGIP+"\'");
//	ExchangeAnsStr(QueryStringGIP,":ProcessOperator",ProcessOperator);
//	ExchangeAnsStr(QueryStringGIP,":ShiftSupervisorGIP",ShiftSupervisorGIP);
//	ExchangeAnsStr(QueryStringGIP,":BoothOperator",BoothOperator);
//	ExchangeAnsStr(QueryStringGIP,":ShiftLongG","\'"+ShiftLetterStr+"\'");
//	ExchangeAnsStr(QueryStringGIP,":Datee","\'"+Date.DateTimeString()+"\'");
//	ExchangeAnsStr(QueryStringGIP,":Time","\'"+Date.TimeString()+"\'");
//	ExchangeAnsStr(QueryStringGIP,":SHIFTSTARTDATE","\'"+Date.CurrentDateTime().DateTimeString()+"\'");
//
//
//	QueryGIP->SQL->Clear();
//	QueryGIP->SQL->Add(QueryStringGIP);
//
//	DataModule2->ADOConnectionCreport->BeginTrans();
//	DataModule2->ADOConnectionCreport->BeginTrans();
//	NoOfRows = Query->ExecSQL();
//	NoOfRowsGIP= QueryGIP->ExecSQL();
////	if(DataModule2->ADOConnectionCreport->InTransaction==true || NoOfRows!=1 \
////	   || DataModule2->ADOConnectionCreport->InTransaction==true || NoOfRowsGIP!=1)
//	if( NoOfRows!=1 ||  NoOfRowsGIP!=1)
//	{
//		ShowMessage("Nie uda³o siê dodaæ danych, proszê powiadomiæ osobê odpowiedzialn¹ za program."
//					"\n\n"
//					"The update of database has been unsuccessful. Contact with person responsible for program, please.");
//		DataModule2->ADOConnectionCreport->RollbackTrans();
//		DataModule2->ADOConnectionCreport->RollbackTrans();
//	}
//	else
//	{
//		DataModule2->ADOConnectionCreport->CommitTrans();
//		DataModule2->ADOConnectionCreport->CommitTrans();
//	}
//	Query->Close();
//	QueryGIP->Close();
//	Query->SQL->Clear();
//	QueryGIP->SQL->Clear();
//
//    if(!ExistsInDatabase) //Retrieving the ID number after writting  to database
//    {                     // ID is autonumber therefore it is unknown
//      QueryString = "Select ID_Workshiftday FROM WorkshiftDays  WHERE "
//					" 			ID_ShiftTime = :ShiftType 			"
//                    " AND		ID_Supervisor = :ShiftSupervisor 	"
//                    " AND		Operators = :NoOfOperators 			"
//                    " AND    	Date_of_work  = :Date            	"
//                    ;
//	  Query->SQL->Add(QueryString);
//      Query->Parameters->ParamByName("ShiftType")->Value=ShiftType;
//	  Query->Parameters->ParamByName("ShiftSupervisor")->Value=ShiftSupervisor;
//	  Query->Parameters->ParamByName("NoOfOperators")->Value=NoOfOperators;
//	  Query->Parameters->ParamByName("Date")->Value=Date;
//
//	  QueryStringGIP= 	"Select DocNum FROM CP_HEAD			      "
//						"WHERE 	User_Co = :ProcessOperator     	  "
//						"AND 	 	USER_SV = :ShiftSupervisorGIP "
//						"AND 		User_Do = :BoothOperator       "
//						"AND 		Data = :Date		  			  "
//						"AND		Time = :Time					  "
//						"AND		Shift = :Shift				  "
//						;
//
//	  QueryGIP->Parameters->Clear();
//	  QueryGIP->SQL->Add(QueryStringGIP);
//	  QueryGIP->ParamCheck =  true;
//	  TParameter* Param;
//	  AnsiString PName;
//	  for (int i=0; i < QueryGIP->Parameters->Count; i++) {
//	  #ifdef test
//		   PName = FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->Items[i]->Name);
//	  #endif
//		   if  (PName == "Shift")
//		   {
//			   QueryGIP->Parameters->Items[i]->Value = ShiftLetter;
//		   }
//		   if (PName=="Data") {
//			   QueryGIP->Parameters->Items[i]->Value = Date;
//		   }
//		   if (PName=="Time") {
//			   QueryGIP->Parameters->Items[i]->Value = Date.TimeString();
//		   }
//	  }
//	//  Param = QueryGIP->Parameters->ParamByName("Shift");
//	//  Param->DataType = ftInteger;
//	//  Param->Value=ShiftLetter;
//
//	  QueryGIP->Parameters->ParamByName("ProcessOperator")->Value=ProcessOperator;
//	  QueryGIP->Parameters->ParamByName("ShiftSupervisorGIP")->Value=ShiftSupervisorGIP;
//	  QueryGIP->Parameters->ParamByName("BoothOperator")->Value=BoothOperator;
////	  QueryGIP->Parameters->ParamByName("Data")->Value=Date;
////	  QueryGIP->Parameters->ParamByName("Time")->Value=Date.TimeString();
//
//	  QueryGIP->Open();
//	  Query->Open();
//	  Id = Query->FieldByName("ID_Workshiftday")->AsInteger;
//	  IdGIP = QueryGIP->FieldByName("DocNum")->AsString;
//	}
//	ExistsInDatabase = true;
//
//return true;
}
//catch(EDatabaseError& E){
////	AnsiString StrMsg = "Wyst¹pi³ b³¹d zapisu g³ównego rekordu do bazy danych\n\n";
////	StrMsg+=E.Message;
////	wchar_t* Buf;
////	Application->MessageBoxA(StrMsg.WideChar(Buf,StrMsg.WideCharBufSize()),L"B³¹d Zapisu!",MB_OK);
////	return false;
//	// DataModule2->ADOConnectionCreport->Connected=false;
//}
//} __finally{
//	DataModule2->ADOConnectionCreport->Connected=false;
//}
//}

AnsiString TMainEntity::NewIdGIP()
{
	try{

		 int NoRec =0;
		 AnsiString DocNum;
		 int ConsecutiveNum;
		 if(!DataModule2->ADOConnectionCreport->Connected)
		 {
			 DataModule2->ADOConnectionCreport->Connected=true;
		 }
		 TADOQuery* QueryGIP = DataModule2->ADOQueryGIP_Seek;
		 AnsiString QueryStringGIP;
			 QueryStringGIP = 	" SELECT TOP 1 CP_HEAD.DOCNUM	"
								" FROM CP_HEAD                  "
								" ORDER BY CP_HEAD.DOCNUM DESC; "
					;
		 QueryGIP->SQL->Clear();
		 QueryGIP->SQL->Add(QueryStringGIP);
		 QueryGIP->Open();
		 NoRec=QueryGIP->RecNo;
		 if (NoRec==1) {      //Form of the DocNum in GIP database is
							  //"PC" + six digit numer
			  DocNum=QueryGIP->FieldByName("DocNum")->AsString;
			  DocNum=DocNum.SubString(3,6);
			  ConsecutiveNum=DocNum.ToInt();
			  ConsecutiveNum++;
			  DocNum="PC"+AnsiString().FormatFloat("000000",ConsecutiveNum);
			  return DocNum;
		 }
		 else {
			 throw EDatabaseError("Problem z odczytem ostatniego numeru zmiany z bazy GIP");
		 }
	}
	__finally {
		//DataModule2->ADOConnectionCreport->Connected=false;
	}
}

//Functions compare data read to MainEntity object  (usually Current) with that wich are currently in Form
bool TMainEntity::CheckIfIsRead(TDateTime D, int Shift)
{
   if (D.DateString()==Date.DateString() && Shift==ShiftType ) {
	  return true;
   }
   else
   {
       return false;
   }

}


bool TMainEntity::CheckIfHasChanged(int Letter, AnsiString Supervisor, int NoOperators)
{
//	int ShiftSuper = FormMain->Supervisors[FormMain->ComboBoxSuperVisors->Text];
//	int BoothOp =  FormMain->BoothOperators[FormMain->ComboBoxBoothOperator->Text];
//	int ShiftSuperGIP = FormMain->SupervisorsGIP[FormMain->ComboBoxSuperVisorsGIP->Text];
//	int ProcOp	=  FormMain->ProcessOperators[FormMain->ComboBoxProcessOperator->Text];
//	if(Letter == ShiftLetter && NoOperators == NoOfOperators && ShiftSuper == ShiftSupervisor \
//	&& BoothOp == BoothOperator && ProcOp == ProcessOperator && ShiftSuperGIP == ShiftSupervisorGIP)
//	{
//		return false;
//	}
//	else
//	{
//		return true;
//	}
}

TDateTime TMainEntity::GetDate()
{
   return Date;
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


AnsiString TMainEntity::GetGIPDocNum()
{
  try{
	  TDateTime GetShiftTime(int ShiftType, int Kind);
	  TADOQuery* QueryG = DataModule2->ADOQueryCreport;
	 if(!DataModule2->ADOConnectionCreport->Connected)
	 {
		 DataModule2->ADOConnectionCreport->Connected=true;
	 }
	   AnsiString QueryString,Name;
	   TDateTime Start =TDateTime(GetDate().DateString()+" " + GetShiftTime(GetShiftKind(),0 ).TimeString());
	   TDateTime Stop=TDateTime(GetDate().DateString() + " " + GetShiftTime(GetShiftKind(),1 ).TimeString());
	   Name = Start.DateTimeString();
	   Name = Stop.DateTimeString();
	   if(GetShiftKind()==3)  // if it is nightshift
	   {
			Start -=1;
	   }
	   QueryString= " SELECT DocNum FROM CP_HEAD 			"
					" WHERE Data BETWEEN :Start AND :Stop  	"
			//		" WHERE Data > \#04/01/2012 \# AND \#04/02/2012 \#	"
					;
	   QueryG->Close();
	   QueryG->SQL->Clear();
	   QueryG->SQL->Add(QueryString);
	   QueryG->Parameters->ParamByName("Start")->Value=Start.DateTimeString();
	   QueryG->Parameters->ParamByName("Stop")->Value=Stop.DateTimeString();
	   Name = QueryG->Parameters->ParseSQL(QueryG->SQL->Text,false);
	   QueryG->Prepared=true;
	   Name = QueryG->SQL->Text;
	   int NumberOfRowsAffected;
	   QueryG->Open();
	   NumberOfRowsAffected = QueryG->RecordCount;

	   if (NumberOfRowsAffected>0) {
			Name = QueryG->FieldByName("DocNum")->AsString;
			QueryG->SQL->Clear();
			QueryG->Close();
	   }
	   else
	   {
			QueryG->SQL->Clear();
			QueryG->Close();
			return NULL;
	   }


	   return Name;//Start.DateTimeString()+" "+Stop.DateTimeString();

	 }
	 __finally{

		 //DataModule2->ADOConnectionCreport->Connected=false;
	}
}

	void TMainEntity::SetSumOfMinutes(int Sum) throw (ErrorMain)
	{
	   if (Sum > 480) {               // Number of minutes in 8 hours (one shift)
		  throw ErrorMain(L"Przekroczono liczbê minut przypadaj¹c¹ na jedn¹ zmianê (480)");
	   }
	   else
	   {
		SumOfMinutes = Sum;
	   }
	}
	void TMainEntity::AddMinutes(int minutes) throw (ErrorMain)
	{
		int Sum = SumOfMinutes + minutes;
		if (Sum > 480) {               // Number of minutes in 8 hours (one shift)
		  throw ErrorMain(L"Przekroczono liczbê minut przypadaj¹c¹ na jedn¹ zmianê (480)");
	   }
	   else
	   {
		   SumOfMinutes = Sum;
	   }

    }

	void TMainEntity::SubstractMinutes(int minutes)
	{
		int Sum = SumOfMinutes - minutes;
		if (Sum <0) {               // Number of minutes in 8 hours (one shift)
		  throw ErrorMain(L"Sumaryczna liczba minut nie mo¿e byæ mniejsza od zera");
	   }
	   else
	   {
		   SumOfMinutes = Sum;
	   }
    }

	int TMainEntity::GetSumOfMinutes()
	{
        return SumOfMinutes;
    }


   ErrorMain::ErrorMain(UnicodeString Nazwa)
   {
	 Name = Nazwa;

   }

   void ErrorMain::Print()
   {

	  // Application->MessageBox          UnicodeString(L"Wyst¹pi³ b³ad: " + Name).c_str()
	   ShowMessage( L"Wyst¹pi³ b³ad: "+ Name);
   }


   char TMainEntity::GetShiftLetter()
   {
	   char* Sh="ABCD";
	   char Letter =  Sh[ShiftLetter-1];
	   return  Letter;
   }




	void TSuperviosorCompare::add(UnicodeString NGip, UnicodeString NCreport, int NoG, int NoCreport)
	{
			TCompareRecord Record;
			Record.NameGIP=NGip;
			Record.NameCreport = NCreport;
			Record.NoGIP = NoG;
			Record.NoCreport = NoCreport;
			CompareRecords.push_back(Record);
	}

	UnicodeString TSuperviosorCompare::GetCreport(UnicodeString NGip)
	{
	   std::list<TCompareRecord>::iterator RecIter;
	   for (RecIter=CompareRecords.begin(); RecIter!=CompareRecords.end() ; RecIter++) {
			if(RecIter->NameGIP.CompareIC(NGip)==0)          // If equivalent of GIP supervisor name will be found
			{
				 return RecIter->NameCreport;
			}
	   }
	//   UnicodeString Super = FormMain->CurrentSupervisorsGIP[Current.GetShiftLetter()];
	   char a =  FormMain->ComboBoxShiftLetter->Text[1];
	   UnicodeString Super = FormMain->CurrentSupervisorsGIP[a];
	   return Super; //"Supervisor unknown";  // If no match found then return "Supervisor unknown"
	}

	UnicodeString TSuperviosorCompare::GetGIP(UnicodeString NCreport)
	{
	   std::list<TCompareRecord>::iterator RecIter;
	   for (RecIter=CompareRecords.begin(); RecIter!=CompareRecords.end() ; RecIter++) {
			if(RecIter->NameCreport.CompareIC(NCreport)==0)          // If equivalent of Creport supervisor name will be found
			{
				 return RecIter->NameGIP;
			}
	   }
//	   UnicodeString Super = FormMain->CurrentSupervisorsGIP[Current.GetShiftLetter()];
//	   char a =  FormMain->ComboBoxShiftLetter->Text[1];
//	   UnicodeString Super = FormMain->CurrentSupervisorsGIP[a];
	   return "Supervisor unknown"; // Super; // If no match found then return "Supervisor unknown"
	}

	int TSuperviosorCompare::GetGIP_No(UnicodeString NCreport)
	{
		std::list<TCompareRecord>::iterator RecIter;
	   for (RecIter=CompareRecords.begin(); RecIter!=CompareRecords.end() ; RecIter++) {
			if(RecIter->NameCreport.CompareIC(NCreport)==0)          // If equivalent of Creport supervisor name will be found
			{
				 return RecIter->NoGIP;
			}
	   }
	   return -1;
	}

	int TSuperviosorCompare::GetGIP_No(int NoCreport)
	{
		std::list<TCompareRecord>::iterator RecIter;
	   for (RecIter=CompareRecords.begin(); RecIter!=CompareRecords.end() ; RecIter++) {
			if(RecIter->NoCreport==NoCreport)          // If equivalent of Creport supervisor name will be found
			{
				 return RecIter->NoGIP;
			}
	   }
	   return -1;
	}


UnicodeString TSuperviosorCompare::GetGIPName(int GIP_No)
{
   std::list<TCompareRecord>::iterator RecIter;
	   for (RecIter=CompareRecords.begin(); RecIter!=CompareRecords.end() ; RecIter++) {
			if(RecIter->NoGIP==GIP_No) //   NameCreport.CompareIC(NCreport)==0)          // If equivalent of Creport supervisor name will be found
			{
				 return RecIter->NameGIP;
			}
	   }
//	   UnicodeString Super = FormMain->CurrentSupervisorsGIP[Current.GetShiftLetter()];
//	   char a =  FormMain->ComboBoxShiftLetter->Text[1];
//	   UnicodeString Super = FormMain->CurrentSupervisorsGIP[a];
	   return "Supervisor unknown";
}

void FillUpDefectSheet(int NoOfEntities)
{
	 int SubstrateID;
	 UnicodeString SubstrateName;
	 int ProdTypeID;
	 UnicodeString ProdTypeName;
	 int SubstrateClassID;
	 UnicodeString SubClassName;
	 int ThicknessID;
	 UnicodeString ThicknessName;
	 int AdditionalDefectID;
	 UnicodeString AdDefectName;
	 int SizeID;
	 UnicodeString SizeName;
	 int DefectID;
	 UnicodeString DefectName;
	 int Quantity;
	 UnicodeString Comment;
	 TDateTime EventDate;
	 ClearSheet(FormMain->StringGrid1);
	 DefectSheetHeader(FormMain->StringGrid1);
	 FormMain->MemoComment->Lines->Clear();
	 double OverallArea=0;
	 double Area;
	 int SheetIndex = 1;
	 int Width, Length;
	 bool Increased = false;
	 if (NoOfEntities==0) {
		NoOfEntities = SummarizedEntities.size()+1;
	 }
	 //for (int i = 1; i < NoOfEntities; i++)
	 int i = 0;
	 while(i<SummarizedEntities.size() )    //SummarizedEntities[i].GetDefExpanded().GetDefect().GetKindNumber()!=0)  //KindNumber equal to zero means end of seqence. Reset fills vector with zeros
	 {
			Quantity=SummarizedEntities[i].GetNoOfDefects();
			if(Quantity==0)
			{
			  SummarizedEntities.erase(SummarizedEntities.begin()+i);
			  i++;
			  continue;
			}
			SizeName=SummarizedEntities[i].GetDefExpanded().GetSubstrate().GetSizeName();
			DefectName=SummarizedEntities[i].GetDefExpanded().GetDefect().GetKindName();
			AdDefectName=SummarizedEntities[i].GetDefExpanded().GetDefect().GetAddidtionalDescription();
			//  GetDefExpanded().GetDefect().GetNumberOfDefects();
			SubstrateName=SummarizedEntities[i].GetDefExpanded().GetSubstrate().GetSubName();
			SubClassName=SummarizedEntities[i].GetDefExpanded().GetSubstrate().GetSubClassName();
			ThicknessName=SummarizedEntities[i].GetDefExpanded().GetSubstrate().GetThicknessName();
			Width=SummarizedEntities[i].GetDefExpanded().GetSubstrate().GetWidth();
			Length=SummarizedEntities[i].GetDefExpanded().GetSubstrate().GetLength();
//			FormMain->MemoComment->Lines->Add(i);
//			for (int j=0; j < SummarizedEntities[i].ReadDateVector().size(); j++) {
//			   FormMain->MemoComment->Lines->Add(SummarizedEntities[i].ReadDateVector()[j]);
//			}
			EventDate = SummarizedEntities[i].GetDefExpanded().GetDate();// ReadDateVector()[0];
			ProdTypeName = SummarizedEntities[i].GetDefExpanded().GetCoating().GetShortName();
			Area = SummarizedEntities[i].GetSummaryArea();//   Sub.GetAreaM2()*Quantity;
			Comment = SummarizedEntities[i].GetDefExpanded().GetComment();
//			if(!Comment.IsEmpty())
//			{
//			  FormMain->MemoComment->Lines->Add(Comment);
//			}

			FormMain->StringGrid1->Cells[0][i+1] =EventDate.TimeString();
			FormMain->StringGrid1->Cells[1][i+1] = ProdTypeName;
			FormMain->StringGrid1->Cells[2][i+1] =SizeName;
			FormMain->StringGrid1->Cells[3][i+1] =DefectName;
			FormMain->StringGrid1->Cells[4][i+1] =AdDefectName;
			FormMain->StringGrid1->Cells[5][i+1] =Quantity;
			//FormMain->StringGrid1->Cells[6][i] =Area;
			FormMain->StringGrid1->Cells[7][i+1] =SubstrateName;
			FormMain->StringGrid1->Cells[8][i+1] =SubClassName;
			FormMain->StringGrid1->Cells[9][i+1] =ThicknessName;
			FormMain->StringGrid1->Cells[10][i+1] =Width; //Combos.SizeCombo.GetWidthLength(SizeID).first;
			FormMain->StringGrid1->Cells[11][i+1] =Length;//Combos.SizeCombo.GetWidthLength(SizeID).second;
			FormMain->StringGrid1->Cells[12][i+1] = Comment;

			//FormMain->StringGrid1->Cells[10][i] = Area;
			FormMain->StringGrid1->Cells[6][i+1]=UnicodeString().FormatFloat("#.##",Area);
			OverallArea += Area;
			if( i >= FormMain->StringGrid1->RowCount+1)
				FormMain->StringGrid1->RowCount=FormMain->StringGrid1->RowCount+5;
			i++;

	 }
	 FormMain->EditSummary->Text = OverallArea;
	 bool Can = true;
	 FormMain->StringGrid1SelectCell(FormMain,FormMain->StringGrid1->Col,FormMain->StringGrid1->Row,Can);
}

void ReadDefectFromDBToSheet(UnicodeString IDGIP)
{

   TDefectExpanded LastRead;
   for(int i=0;i<SummarizedEntities.size();i++)
	 {
		 SummarizedEntities[i].Reset();
	 }
	 if (!DataModule2->ADOConnectionCreport->Connected)
	 {
		DataModule2->ADOConnectionCreport->Connected = true;
	 }

	 TADOQuery* Query = DataModule2->ADOQueryCreport;
	 Query->Close();
	 Query->SQL->Clear();
	 UnicodeString QueryContent;
	 QueryContent =
	 " SELECT * FROM CP_DEFECTSFULL_VIEW WHERE DOCNUM= :ID ";
	 Query->SQL->Add(QueryContent);
	 Query->Parameters->ParamByName("ID")->Value = IDGIP;
	 Query->Open();

	 int NumberOfRowsAffected = Query->RecordCount;


	 TSubstrate Sub;
	 TCoating Coat;
	 TDefectDetail Def;
	 int SubstrateID;
	 UnicodeString SubstrateName;
	 int ProdTypeID;
	 UnicodeString ProdTypeName;
	 int SubstrateClassID;
	 UnicodeString SubClassName;
	 int ThicknessID;
	 UnicodeString ThicknessName;
	 int AdditionalDefectID;
	 UnicodeString AdDefectName;
	 int SizeID;
	 UnicodeString SizeName;
	 int DefectID;
	 UnicodeString DefectName;
	 int Quantity;
	 UnicodeString Comment;
	 TDateTime EventDate;
	 int EntityID;         // ID_DEFECT - Identity number for whole of defect - key and foreign key
	 double SummaryArea=0;
	 double Area;
	 int SheetIndex = 1;
	 bool Increased = false;
	 TDefectExpanded Temp,Current;
	 int Dimmension,DefectReadID;
	 //bool ExistedSubstrate;
	 //Dimmension = ChoosenSubCoats.size();
	 //ChoosenSubCoats.assign(Dimmension,Temp); //Clear ChoosenSubCoats and intitialize to have opportunity to put substrates in array
	 //FormMain->RadioGroupSubstrate->Items->Clear();
	 //static OldHeight = FormMain->RadioGroupSubstrate->Height;
	 //FormMain->RadioGroupSubstrate->Height = OldHeight;
	 SummarizedEntities.clear();
	 for(int i=1;i<=NumberOfRowsAffected;i++) {   //i starts from 1 because of string grid indexing

			SubstrateID = Query->FieldByName("ID_SUBSTRATE")->AsInteger;
			SubstrateName = Combos.SubstrateCombo.GetNameByDBId(SubstrateID);
			ProdTypeID = Query->FieldByName("ID_PRODTYPE")->AsInteger;
			ProdTypeName = Combos.CoatingsCombo.GetNameByDBId(ProdTypeID);
			SubstrateClassID = Query->FieldByName("ID_SUBSTRATECLASS")->AsInteger;
			SubClassName = Combos.SubstrateClassCombo.GetNameByDBId(SubstrateClassID);
			ThicknessID = Query->FieldByName("ID_THICKNESS")->AsInteger;
			ThicknessName = Combos.ThicknessCombo.GetNameByDBId(ThicknessID);
			AdditionalDefectID = Query->FieldByName("ID_ADDITIONALDEFECT")->AsInteger;
			AdDefectName = Combos.AdditionalDefectsCombo.GetNameByDBId(AdditionalDefectID);
			SizeID = Query->FieldByName("SIZEID")->AsInteger;
			SizeName = Combos.SizeCombo.GetNameByDBId(SizeID);
			DefectID = Query->FieldByName("DEFECTID")->AsInteger;
			DefectName = Combos.DefectsCombo.GetNameByDBId(DefectID);
			Quantity = Query->FieldByName("QUANTITY")->AsInteger ;
			Comment = Query->FieldByName("COMMENT")->AsString;
			EventDate = Query->FieldByName("DATETIME")->AsDateTime;
			EntityID = Query->FieldByName("ID_DEFECT")->AsInteger;
			Sub.Add(
					 SubstrateID,SubstrateName,
					 SubstrateClassID, SubClassName,
					 ThicknessID, ThicknessName,
					 SizeName, SizeID,
					 Combos.SizeCombo.GetWidthLength(SizeID).first, Combos.SizeCombo.GetWidthLength(SizeID).second
				   );

			Coat.SetID(ProdTypeID);
			Coat.SetShortName(ProdTypeName);
			Def.Add(DefectName,AdDefectName,DefectID,Quantity,AdditionalDefectID,EntityID);
			LastRead.AddCoating(Coat);
			LastRead.AddSubstrate(Sub);
			LastRead.AddDefect(Def);
			LastRead.AddComment(Comment);
			LastRead.AddDate(EventDate);
			//Increased = false;
			SummarizedEntities.push_back(LastRead);
			Query->Next();
	   }
	   FillUpDefectSheet();
}

void ClearSubstrates()
{
	 TDefectExpanded Temp;
	 int Dimmension = ChoosenSubCoats.size();
	 ChoosenSubCoats.assign(Dimmension,Temp); //Clear ChoosenSubCoats and intitialize to have opportunity to put substrates in array
	 FormMain->RadioGroupSubstrate->Items->Clear();
}

void  ReadDataFromDatabase(AnsiString IDGIP)
{
  try
  {

	 TDefectExpanded LastRead;
	 for(int i=0;i<Entities.size();i++)
	 {
		 Entities[i].Reset();
	 }
	 for(int i=0;i<SummarizedEntities.size();i++)
	 {
		 SummarizedEntities[i].Reset();
	 }
	 if (!DataModule2->ADOConnectionCreport->Connected)
	 {
		DataModule2->ADOConnectionCreport->Connected = true;
	 }

	 TADOQuery* Query = DataModule2->ADOQueryCreport;
	 Query->Close();
	 Query->SQL->Clear();
	 UnicodeString QueryContent;
	 QueryContent =
	 " SELECT * FROM CP_DEFECTSFULL_VIEW WHERE DOCNUM= :ID ";
	 Query->SQL->Add(QueryContent);
	 Query->Parameters->ParamByName("ID")->Value = IDGIP;
	 Query->Open();

	 int NumberOfRowsAffected = Query->RecordCount;


	 TSubstrate Sub;
	 TCoating Coat;
	 TDefectDetail Def;
	 int SubstrateID;
	 UnicodeString SubstrateName;
	 int ProdTypeID;
	 UnicodeString ProdTypeName;
	 int SubstrateClassID;
	 UnicodeString SubClassName;
	 int ThicknessID;
	 UnicodeString ThicknessName;
	 int AdditionalDefectID;
	 UnicodeString AdDefectName;
	 int SizeID;
	 UnicodeString SizeName;
	 int DefectID;
	 UnicodeString DefectName;
	 int Quantity;
	 UnicodeString Comment;
	 TDateTime EventDate;
	 int EntityID; //it define whole entity
	 double SummaryArea=0;
	 double Area;
	 int SheetIndex = 1;
	 bool Increased = false;
	 TDefectExpanded Temp,Current;
	 int Dimmension,DefectReadID;
	 bool ExistedSubstrate;
//	 Dimmension = ChoosenSubCoats.size();
//	 ChoosenSubCoats.assign(Dimmension,Temp); //Clear ChoosenSubCoats and intitialize to have opportunity to put substrates in array
//	 FormMain->RadioGroupSubstrate->Items->Clear();
	 ClearSubstrates();
	 static OldHeight = FormMain->RadioGroupSubstrate->Height;
	 FormMain->RadioGroupSubstrate->Height = OldHeight;
	 SummarizedEntities.clear();
	 for(int i=1;i<=NumberOfRowsAffected;i++) {   //i starts from 1 because of string grid indexing

			SubstrateID = Query->FieldByName("ID_SUBSTRATE")->AsInteger;
			SubstrateName = Combos.SubstrateCombo.GetNameByDBId(SubstrateID);
			ProdTypeID = Query->FieldByName("ID_PRODTYPE")->AsInteger;
			ProdTypeName = Combos.CoatingsCombo.GetNameByDBId(ProdTypeID);
			SubstrateClassID = Query->FieldByName("ID_SUBSTRATECLASS")->AsInteger;
			SubClassName = Combos.SubstrateClassCombo.GetNameByDBId(SubstrateClassID);
			ThicknessID = Query->FieldByName("ID_THICKNESS")->AsInteger;
			ThicknessName = Combos.ThicknessCombo.GetNameByDBId(ThicknessID);
			AdditionalDefectID = Query->FieldByName("ID_ADDITIONALDEFECT")->AsInteger;
			AdDefectName = Combos.AdditionalDefectsCombo.GetNameByDBId(AdditionalDefectID);
			SizeID = Query->FieldByName("SIZEID")->AsInteger;
			SizeName = Combos.SizeCombo.GetNameByDBId(SizeID);
			DefectID = Query->FieldByName("DEFECTID")->AsInteger;
			DefectName = Combos.DefectsCombo.GetNameByDBId(DefectID);
			Quantity = Query->FieldByName("QUANTITY")->AsInteger ;
			Comment = Query->FieldByName("COMMENT")->AsString;
			EventDate = Query->FieldByName("DATETIME")->AsDateTime;
			EntityID = Query->FieldByName("ID_DEFECT")->AsInteger;
			Sub.Add(
					 SubstrateID,SubstrateName,
					 SubstrateClassID, SubClassName,
					 ThicknessID, ThicknessName,
					 SizeName, SizeID,
					 Combos.SizeCombo.GetWidthLength(SizeID).first, Combos.SizeCombo.GetWidthLength(SizeID).second
				   );

			Coat.SetID(ProdTypeID);
			Coat.SetShortName(ProdTypeName);
			Def.Add(DefectName,AdDefectName,DefectID,Quantity,AdditionalDefectID,EntityID);
			LastRead.AddCoating(Coat);
			LastRead.AddSubstrate(Sub);
			LastRead.AddDefect(Def);
			LastRead.AddComment(Comment);
			LastRead.AddDate(EventDate);
			Entities[i]=LastRead;
			Increased = false;
//			for(int zz=1;zz<SheetIndex;zz++)
//			{
//			  if(SummarizedEntities[zz]==LastRead)
//			  {
//				SummarizedEntities[zz]=LastRead;
//				Increased = true;
//				break;
//			  }
//			}
//			if (!Increased) {
//			   SummarizedEntities[SheetIndex++]=LastRead;
//            }
			SummarizedEntities.push_back(LastRead);

			//************* Adding substrates
			Temp.AddCoating(Coat);
			Temp.AddSubstrate(Sub);
			 Dimmension = ChoosenSubCoats.size();
			ExistedSubstrate = false;
			for(unsigned int ii=0; ii<ChoosenSubCoats.size();ii++ ) // if one of the choosen substrates is equal to one added eariel then return
			{
				Current = ChoosenSubCoats[ii] ;
				if(Temp == Current)
				{
					ExistedSubstrate=true;
					break;
				}
				DefectReadID = ChoosenSubCoats[ii].GetSubstrate().GetSizeID();   //  Size is only substrate data saved for old dataset.
				if( DefectReadID == 0 )   //End of the ChoosenSubCoats array (everyother element has all IDs set to zero
				{
					   ExistedSubstrate=false;
					   break;
				}
			}
			UnicodeString AddTerm(UnicodeString); //function prototype;
			if(!ExistedSubstrate)
			{
					UnicodeString Substrate="";
					Substrate += AddTerm(Coat.GetShortName());
					Substrate += AddTerm(Sub.GetSubName());
					Substrate += AddTerm(Sub.GetSubClassName());
					Substrate += AddTerm(Sub.GetSizeName());
					Substrate += AddTerm(Sub.GetThicknessName());
					Substrate = Substrate.Trim();
					if(*Substrate.LastChar() ==',')
						Substrate=Substrate.SetLength(Substrate.Length()-1);



					FormMain->RadioGroupSubstrate->Height= FormMain->RadioGroupSubstrate->Height - 1.5*FormMain->RadioGroupSubstrate->Font->Height;
					int Index=FormMain->RadioGroupSubstrate->Items->Add(Substrate);
					ChoosenSubCoats[Index]=Temp;
					//ChoosenSubCoats.push_back(Temp);
					FormMain->RadioGroupSubstrate->ItemIndex = Index;
			}





//			FormMain->StringGrid1->Cells[0][i] =i;
//			FormMain->StringGrid1->Cells[1][i] =SizeName;
//			FormMain->StringGrid1->Cells[2][i] =DefectName;
//			FormMain->StringGrid1->Cells[3][i] =AdDefectName;
//			FormMain->StringGrid1->Cells[4][i] =Quantity;
//			FormMain->StringGrid1->Cells[5][i] =SubstrateName;
//			FormMain->StringGrid1->Cells[6][i] =SubClassName;
//			FormMain->StringGrid1->Cells[7][i] =ThicknessName;
//			FormMain->StringGrid1->Cells[8][i] =Combos.SizeCombo.GetWidthLength(SizeID).first;
//			FormMain->StringGrid1->Cells[9][i] =Combos.SizeCombo.GetWidthLength(SizeID).second;
//			Area = Sub.GetAreaM2()*Quantity;
//			//FormMain->StringGrid1->Cells[10][i] = Area;
//			FormMain->StringGrid1->Cells[10][i].FormatFloat("#.##",Area);
//			SummaryArea += Area;
//			if( i >= FormMain->StringGrid1->RowCount)
//				FormMain->StringGrid1->RowCount=FormMain->StringGrid1->RowCount+5;

			Query->Next();
	   }
			SheetIndex=SummarizedEntities.size();
			FillUpDefectSheet(SheetIndex);
			Query->SQL->Clear();
			Query->Close();
			return;

  }
  __finally
  {
	 DataModule2->ADOConnectionCreport->Connected = false;
  }
}


void SelectRow(int EntityID)
{
  for (int i = 0; i < SummarizedEntities.size(); i++) {
   for(int j=0;j < SummarizedEntities[i].NumberOfElements();j++)
   {
	 if (SummarizedEntities[i].GetDefExpanded(j).GetDefect().GetEntityID()==EntityID) {
		 FormMain->StringGrid1->Row = i+1;
		 return;
	 }
   }
  }
  FormMain->StringGrid1->Row = 1;

}
