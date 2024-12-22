//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitMainEntity.h"
#include "UnitMain.h"
#include "DataCoater.h"
#include "Global.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)

TMainEntity::TMainEntity()
{
    Remarks = 0;
    ExistsInDatabase=false;
    Query = 0;
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
	ShiftSupervisor = FormMain->Supervisors[FormMain->ComboBoxSuperVisors->Text];
	ShiftSupervisorGIP= FormMain->SupervisorsGIP[FormMain->ComboBoxSuperVisorsGIP->Text];
	ProcessOperator = FormMain->ProcessOperators[FormMain->ComboBoxProcessOperator->Text];
	BoothOperator = FormMain->BoothOperators[FormMain->ComboBoxBoothOperator->Text];
	NoOfOperators=FormMain->EditNumberOfOperators->Text.ToInt();
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
	FormMain->MemoTest->Lines->Add("Date = " + Date);
	FormMain->MemoTest->Lines->Add("Supervisor = " + ShiftSupervisor);
	return true;
}

//methode returns true if given by form workshift entity exists in database;
// otherwise returns false
bool TMainEntity::CheckIfIsInDatabase(TDateTime DateR,int ShiftLetterR, int ShiftR)
{
try{
	Query = DataModule1->ADOQueryCreport;
   // DataModule1->ADOConnectionCreport->Connected=true;

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
	 //  DataModule1->ADOConnectionCreport->Connected=false;
  }
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
	ShiftSupervisor=Query->FieldByName("ID_SUPERVISOR")->AsInteger;
	NoOfOperators= Query->FieldByName("OPERATORS")->AsInteger;
	Date=Query->FieldByName("DATE_OF_WORK")->AsDateTime;
	TDateTime Start = Date;
	char Shiftchar[4]={'A','B','C','D'};
	void ComboOperatorFillUP(AnsiString Shift, bool IsSupervisor,TComboBox* Combo, std::map<AnsiString,int>& Operators, bool All=false ); //function prototype
	/* TODO 1 : Poprawiæ uzupe³nianie Combo */
	ComboOperatorFillUP(Shiftchar[ShiftLetter-1],true,FormMain->ComboBoxSuperVisorsGIP,FormMain->SupervisorsGIP,true);
	ComboOperatorFillUP(Shiftchar[ShiftLetter-1],false,FormMain->ComboBoxProcessOperator,FormMain->ProcessOperators,true);
	ComboOperatorFillUP(Shiftchar[ShiftLetter-1],false,FormMain->ComboBoxBoothOperator,FormMain->BoothOperators,true);
	QueryString = "WorkshiftID\tShift\tShiftTime\tSupervisorID\tNoOfOperators\t\tDateOfWork\n\n";
	QueryString+=AnsiString(Id)+"\t\t"+AnsiString(ShiftLetter)+"\t\t"+AnsiString(ShiftType)+"\t\t"+AnsiString(ShiftSupervisor)+"\t\t"+AnsiString(NoOfOperators)+"\t\t"+Date.DateTimeString();
	ExistsInDatabase=true;
   // ShowMessage(QueryString);
	bool SelfConnectedGIP = false;       // Close connection if it were connected in this methode
  //	bool SelfConnected = false;
	try{
		 TADOQuery* QueryGIP = DataModule1->ADOQueryGIP_Seek;

		 if(!DataModule1->ADOConnectionGIP->Connected)
		 {
			 DataModule1->ADOConnectionGIP->Connected=true;
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
//		if(!DataModule1->ADOConnectionGIP->Connected)
//		 {
//			 DataModule1->ADOConnectionGIP->Connected=true;
//			 SelfConnectedGIP = true;
//		 }
		return true;
	}
	__finally{
		if(SelfConnectedGIP)
		{
		   DataModule1->ADOConnectionGIP->Connected=false;
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
	if (IdGIP=="0") {    // if there are no data for GIP database.

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
	FormMain->UpDownNumberOfOperators->Position =  NoOfOperators;
	FormMain->DateTimePickerMain->DateTime= Date;
//    if(Remarks!=0)
//    {
//        FormMain->MemoGeneralRemarks->Text=Remarks->Text;
//    }
//    else
//    {
//        FormMain->MemoGeneralRemarks->Text="";//.Delete(0,FormMain->MemoGeneralRemarks->Text.Length());
//    }

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

    ReadFromForm();     //Populate Current with datasheet data.
	FormMain->PageControl1->Enabled=true;
try{
try{
	if(!DataModule1->ADOConnectionGIP->Connected)
		 {
			 DataModule1->ADOConnectionGIP->Connected=true;
		 }
	TADOQuery* QueryGIP = DataModule1->ADOQueryGIP_Seek;
	AnsiString QueryString,QueryStringGIP,Name; //,QueryMemo
	Query->SQL->Clear();
	int NoOfRows 		= 0;
	int NoOfRowsGIP		= 0;
	Query->SQL->Clear();
	Query->Parameters->Clear();
	QueryGIP->SQL->Clear();

	if(ExistsInDatabase)
	{
		QueryString = "UPDATE WorkshiftDays  "
					  "SET  ID_ShiftGroup = :ShiftLetter, ID_ShiftTime = :ShiftType, "
					  "		ID_Supervisor = :ShiftSupervisor, Operators =:NoOfOperators, "
					  "     Date_of_work  = :Datee 							"
					  "WHERE   ID_Workshiftday = :ID_Workshiftday ;			"
					  ;
		if(IdGIP==0)         //  When mistakes in GIP definition - no valid record has been created
		{
		   IdGIP=NewIdGIP();
		   QueryStringGIP= 	"INSERT INTO CP_HEAD(  DOCNUM, USER_CO, USER_SV,   "
							" SHIFTSTARTDATE,   USER_DO, DATA, [TIME], SHIFT)  "
							"VALUES   ( :IdGIP,                                "
							"           :ProcessOperator, :ShiftSupervisorGIP  "
							"         , :SHIFTSTARTDATE                        "
							"         , :BoothOperator, :Datee, :Time, :ShiftLongG ); "
						;
		}
		else
		{
		  QueryStringGIP= "UPDATE CP_HEAD		"
						"SET    User_Co =:ProcessOperator     "
						",      USER_SV = :ShiftSupervisorGIP "
						",      User_Do = :BoothOperator      "
						",      Data = :Datee                 "
						",      [Time] = :Time                "
						",      Shift = :ShiftLongG           "
						",      SHIFTSTARTDATE = :SHIFTSTARTDATE "
						"       WHERE  DocNum = :IdGIP           "
						;
	   }
	}
	else     /* TODO :
In the GIP table the shift is recognized by date,
so try to organize not only the date but also correct time and combine them into date */
	{
		 QueryString = "INSERT INTO WorkshiftDays(ID_ShiftGroup, ID_ShiftTime, "
					   " ID_Supervisor,  Operators,  Date_of_work)   "
					   "VALUES  ( :ShiftLetter, :ShiftType, 	        "
					   "		 :ShiftSupervisor,  :NoOfOperators,     "

					   "         :Datee )								"
					   ;

		 QueryStringGIP= 	"  INSERT INTO CP_HEAD(DOCNUM, USER_CO, USER_SV,          "
							"  SHIFTSTARTDATE, USER_DO, DATA, [TIME], SHIFT)          "
							"  VALUES ( :IdGIP,                                       "
							"           :ProcessOperator, :ShiftSupervisorGIP         "
							"         , :SHIFTSTARTDATE                               "
							"         , :BoothOperator, :Datee, :Time, :ShiftLongG ); "
						;
					IdGIP=NewIdGIP();

	}
	Query->SQL->Add(QueryString);

	if(ExistsInDatabase)
	{
	  Query->Parameters->ParamByName("ID_Workshiftday")->Value = Id;
	}
   // Query->Parameters->ParamByName("Id")->Value=Id+1;
	Query->Parameters->ParamByName("ShiftLetter")->Value=ShiftLetter;
	Query->Parameters->ParamByName("ShiftType")->Value=ShiftType;
	Query->Parameters->ParamByName("ShiftSupervisor")->Value=ShiftSupervisor;
	Query->Parameters->ParamByName("NoOfOperators")->Value=NoOfOperators;
	Query->Parameters->ParamByName("Datee")->Value=Date;


//	QueryGIP->Parameters->Clear();
//	QueryGIP->Parameters->Capacity=20;

//	ExchangeAnsStr(QueryStringGIP,":IdGIP","\""+IdGIP+"\"");
//	ExchangeAnsStr(QueryStringGIP,":IdGIP","\""+IdGIP+"\"");
//	ExchangeAnsStr(QueryStringGIP,":IdGIP","\""+IdGIP+"\"");
	/* TODO 1 : Dokoñczyæ */
//	QueryGIP->SQL->Add(QueryStringGIP);
	int Liczba = QueryGIP->Parameters->Count, CheckValue;
//	AnsiString NamePar;
//	for (int i = 0; i < Liczba; i++) {
//		 NamePar = QueryGIP->Parameters->Items[i]->Name;
//	}
	char Shiftchar[4]={'A','B','C','D'};
	AnsiString ShiftLetterStr = Shiftchar[ShiftLetter-1];


//	QueryGIP->Parameters->ParamByName("ProcessOperator")->Value=ProcessOperator;
//	QueryGIP->Parameters->ParamByName("ShiftSupervisorGIP")->Value=ShiftSupervisorGIP;
//	QueryGIP->Parameters->ParamByName("BoothOperator")->Value=BoothOperator;
//	QueryGIP->Parameters->ParamByName("ShiftLongG")->Value=ShiftLetterStr;//.c_str();

//	QueryGIP->Parameters->ParamByName("IdGIP")->Value= IdGIP ;
//	TParameter* Param;
//	Param =  QueryGIP->Parameters->ParamByName("IdGIP");
//	Param->DataType = ftString;
//	Param->Value =  IdGIP ;
//	QueryGIP->Parameters->ParamValues["IdGIP"]=IdGIP;

//	QueryGIP->Parameters->ParamByName("Datee")->Value=Date.DateString();
//	QueryGIP->Parameters->ParamByName("Time")->Value=Date.TimeString();
//	QueryGIP->Parameters->ParamByName("SHIFTSTARTDATE")->Value=Date.CurrentDateTime().DateTimeString();
//
//	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("ProcessOperator")->Value);
//	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("ShiftSupervisorGIP")->Value);
//	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("BoothOperator")->Value );
//	FormMain->MemoTest->Lines->Add(	QueryGIP->Parameters->ParamByName("ShiftLongG")->Value );
//	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("Datee")->Value);
//	FormMain->MemoTest->Lines->Add(QueryGIP->Parameters->ParamByName("Time")->Value);


//	AnsiString ExchangeAnsStr(AnsiString& A, AnsiString Previous, AnsiString Next);
//	ExchangeAnsStr(QueryStringGIP,":IdGIP","\'"+IdGIP+"\'");
//	ExchangeAnsStr(QueryStringGIP,":ProcessOperator",ProcessOperator);
//	ExchangeAnsStr(QueryStringGIP,":ShiftSupervisorGIP",ShiftSupervisorGIP);
//	ExchangeAnsStr(QueryStringGIP,":BoothOperator",BoothOperator);
//	ExchangeAnsStr(QueryStringGIP,":ShiftLongG","\'"+ShiftLetterStr+"\'");
//	ExchangeAnsStr(QueryStringGIP,":Datee","\'"+Date.DateTimeString()+"\'");
//	ExchangeAnsStr(QueryStringGIP,":","\'"+Date.TimeString()+"\'");
//	ExchangeAnsStr(QueryStringGIP,":SHIFTSTARTDATE","\'"+Date.CurrentDateTime().DateTimeString()+"\'");

   // Query->Parameters->ParamByName("Id")->Value=Id+1;



	QueryGIP->SQL->Clear();
	QueryGIP->SQL->Add(QueryStringGIP);
	QueryGIP->Parameters->ParamByName("IdGIP")->Value=IdGIP;

	QueryGIP->Parameters->ParamByName("ShiftSupervisorGIP")->Value= ShiftSupervisorGIP   ;
	QueryGIP->Parameters->ParamByName("BoothOperator")->Value=BoothOperator ;
	QueryGIP->Parameters->ParamByName("ProcessOperator")->Value=ProcessOperator ;
	QueryGIP->Parameters->ParamByName("ShiftLongG")->Value=ShiftLetterStr;
	QueryGIP->Parameters->ParamByName("Datee")->Value= Date;
	QueryGIP->Parameters->ParamByName("Time")->Value= Date.TimeString();
	QueryGIP->Parameters->ParamByName("SHIFTSTARTDATE")->Value=  Date.CurrentDateTime() ;

//	QueryGIP->Parameters->ParamByName("ProcessOperator")->Value=ProcessOperator;
//	QueryGIP->Parameters->ParamByName("ShiftLetter")->Value=ShiftLetter;
//	QueryGIP->Parameters->ParamByName("ShiftType")->Value=ShiftType;
//	QueryGIP->Parameters->ParamByName("ShiftSupervisor")->Value=ShiftSupervisor;
//	QueryGIP->Parameters->ParamByName("NoOfOperators")->Value=NoOfOperators;
//	QueryGIP->Parameters->ParamByName("Datee")->Value=Date;


	DataModule1->ADOConnectionCreport->BeginTrans();
	DataModule1->ADOConnectionGIP->BeginTrans();
	NoOfRows = Query->ExecSQL();
	NoOfRowsGIP= QueryGIP->ExecSQL();
	QueryStringGIP=QueryGIP->SQL->GetText();
	QueryStringGIP=QueryGIP->Parameters->ParamByName("Datee")->Value;//	ParseSQL( QueryStringGIP, false) ;
//	if(DataModule1->ADOConnectionCreport->InTransaction==true || NoOfRows!=1 \
//	   || DataModule1->ADOConnectionGIP->InTransaction==true || NoOfRowsGIP!=1)
	if( NoOfRows!=1 ||  NoOfRowsGIP!=1)
	{
		ShowMessage("Nie uda³o siê dodaæ danych, proszê powiadomiæ osobê odpowiedzialn¹ za program."
					"\n\n"
					"The update of database has been unsuccessful. Contact with person responsible for program, please.");
		DataModule1->ADOConnectionCreport->RollbackTrans();
		DataModule1->ADOConnectionGIP->RollbackTrans();
	}
	else
	{
		DataModule1->ADOConnectionCreport->CommitTrans();
		DataModule1->ADOConnectionGIP->CommitTrans();
	}
	Query->Close();
	QueryGIP->Close();
	Query->SQL->Clear();
	QueryGIP->SQL->Clear();

    if(!ExistsInDatabase) //Retrieving the ID number after writting  to database
    {                     // ID is autonumber therefore it is unknown
      QueryString = "Select ID_Workshiftday FROM WorkshiftDays  WHERE "
                	" 			ID_ShiftTime = :ShiftType 			"
                    " AND		ID_Supervisor = :ShiftSupervisor 	"
                    " AND		Operators = :NoOfOperators 			"
                    " AND    	Date_of_work  = :Date            	"
                    ;
      Query->SQL->Add(QueryString);
      Query->Parameters->ParamByName("ShiftType")->Value=ShiftType;
	  Query->Parameters->ParamByName("ShiftSupervisor")->Value=ShiftSupervisor;
	  Query->Parameters->ParamByName("NoOfOperators")->Value=NoOfOperators;
	  Query->Parameters->ParamByName("Date")->Value=Date;

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
	//  Param = QueryGIP->Parameters->ParamByName("Shift");
	//  Param->DataType = ftInteger;
	//  Param->Value=ShiftLetter;

//	  QueryGIP->Parameters->ParamByName("ProcessOperator")->Value=ProcessOperator;
//	  QueryGIP->Parameters->ParamByName("ShiftSupervisorGIP")->Value=ShiftSupervisorGIP;
//	  QueryGIP->Parameters->ParamByName("BoothOperator")->Value=BoothOperator;
//	  QueryGIP->Parameters->ParamByName("Data")->Value=Date;
//	  QueryGIP->Parameters->ParamByName("Time")->Value=Date.TimeString();

	 // QueryGIP->Open();
	  Query->Open();
	  Id = Query->FieldByName("ID_Workshiftday")->AsInteger;
	  //IdGIP = QueryGIP->FieldByName("DocNum")->AsString;
	}
	ExistsInDatabase = true;
	
return true;
}
catch(EDatabaseError& E){
	AnsiString StrMsg = "Wyst¹pi³ b³¹d zapisu g³ównego rekordu do bazy danych\n\n";
	StrMsg+=E.Message;
	wchar_t* Buf;
	Application->MessageBoxA(StrMsg.WideChar(Buf,StrMsg.WideCharBufSize()),L"B³¹d Zapisu!",MB_OK);
	return false;
	// DataModule1->ADOConnectionCreport->Connected=false;
}
} __finally{
	DataModule1->ADOConnectionGIP->Connected=false;
}
}

AnsiString TMainEntity::NewIdGIP()
{
	try{

		 int NoRec =0;
		 AnsiString DocNum;
		 int ConsecutiveNum;
		 if(!DataModule1->ADOConnectionGIP->Connected)
		 {
			 DataModule1->ADOConnectionGIP->Connected=true;
		 }
		 TADOQuery* QueryGIP = DataModule1->ADOQueryGIP_Seek;
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
			  IdGIP = DocNum;
			  return DocNum;
		 }
		 else {
			 throw EDatabaseError("Problem z odczytem ostatniego numeru zmiany z bazy GIP");
		 }
	}
	__finally {
		//DataModule1->ADOConnectionGIP->Connected=false;
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
	int ShiftSuper = FormMain->Supervisors[FormMain->ComboBoxSuperVisors->Text];
	int BoothOp =  FormMain->BoothOperators[FormMain->ComboBoxBoothOperator->Text];
	int ShiftSuperGIP = FormMain->SupervisorsGIP[FormMain->ComboBoxSuperVisorsGIP->Text];
	int ProcOp	=  FormMain->ProcessOperators[FormMain->ComboBoxProcessOperator->Text];
	if(Letter == ShiftLetter && NoOperators == NoOfOperators && ShiftSuper == ShiftSupervisor \
	&& BoothOp == BoothOperator && ProcOp == ProcessOperator && ShiftSuperGIP == ShiftSupervisorGIP)
	{
		return false;
	}
	else
	{
		return true;
	}
}

TDateTime TMainEntity::GetDate()
{
   return Date;
}

AnsiString TMainEntity::GetGIPDocNum()
{
  try{
	  TDateTime GetShiftTime(int ShiftType, int Kind); // Zdefiniowany w Global   - definied in Global.cpp     Kind = 0 its start, Kind = 0 its end od shift
	  TADOQuery* QueryG = DataModule1->ADOQueryGIP;
	 if(!DataModule1->ADOConnectionGIP->Connected)
	 {
		 DataModule1->ADOConnectionGIP->Connected=true;
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
	   char ShiftLetter;

	   QueryG->Open();
	   NumberOfRowsAffected = QueryG->RecordCount;

	   if (NumberOfRowsAffected>0) {
			Name = QueryG->FieldByName("DocNum")->AsString;
			QueryG->SQL->Clear();
			QueryG->Close();
	   }
	   else
//	   {
//		if (NumberOfRowsAffected>1) {
//		  QueryString= " SELECT DocNum, Shift FROM CP_HEAD 			"
//					" WHERE Data BETWEEN :Start AND :Stop  	"
//
//
//		}
//		else
		{
			QueryG->SQL->Clear();
			QueryG->Close();
			return NULL;
		}
//	   }

	   IdGIP = Name;
	   return Name;//Start.DateTimeString()+" "+Stop.DateTimeString();

	 }
	 __finally{

		 //DataModule1->ADOConnectionGIP->Connected=false;
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

   char GetShiftLetter(int ShiftLetter) // general purpose function
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


void EmptyMainEntityForm()
{
	FormMain->ComboBoxShiftLetter->ItemIndex=-1;
	//FormMain->ComboBoxShiftKind->ItemIndex=-1;
//	FormMain->ComboBoxSuperVisors->ItemIndex=0;
//	FormMain->ComboBoxSuperVisorsGIP->ItemIndex=0;
//	FormMain->ComboBoxProcessOperator->ItemIndex=0;
//	FormMain->ComboBoxBoothOperator->ItemIndex=0;
	FormMain->ComboBoxSuperVisors->Text="";
	FormMain->ComboBoxSuperVisorsGIP->Text="";
	FormMain->ComboBoxProcessOperator->Text="";
	FormMain->ComboBoxBoothOperator->Text="";
	FormMain->CheckBoxLeadman->Checked = false;
	FormMain->CheckBoxDeputyBooth->Checked = false;
	FormMain->CheckBoxDeputyProcess->Checked = false;
	FormMain->CheckBoxDeputySupervisor->Checked = false;
	FormMain->EditNumberOfOperators->Text = 11;
}
