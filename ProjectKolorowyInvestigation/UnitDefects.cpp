//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitDefects.h"
#include "Global.h"
#include "DataCoater.h"
#include "UnitMain.h"
#include <systdate.h>
#include <map>
//---------------------------------------------------------------------------

#pragma package(smart_init)

static AnsiString Database = "Defects";
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
TTempDefect TempDefect;
//void EmptyForm(TEdit* Problem, TEdit* Contact, TEdit* MTComment, TComboBox* Combo, TDateTimePicker* Picker);




void EmptySheetDefectsBooth()
{
   Sheet = FormMain->StringGridDefectsBooth;
   for(int i=1;i<=Sheet->RowCount;i++)
    	Sheet->Rows[i]->Clear();
}

void EmptySheetDefectsNative()
{
   Sheet = FormMain->StringGridDefectsNative;
   for(int i=1;i<=Sheet->RowCount;i++)
    	Sheet->Rows[i]->Clear();
}


AnsiString GetDocNum()
{
  try{
       TADOQuery* Query = DataModule1->ADOQueryGIP_Seek;
       //TADOQuery* QueryGip = DataModule1->ADOQueryGIP;
     if(!DataModule1->ADOConnectionGIP->Connected)
     {
         DataModule1->ADOConnectionGIP->Connected=true;
     }
       AnsiString QueryString,Name;
       TDateTime Start =TDateTime(Current.GetDate().DateString()+" " + GetShiftTime(Current.GetShiftKind(),0 ).TimeString());
       TDateTime Stop=TDateTime(Current.GetDate().DateString() + " " + GetShiftTime(Current.GetShiftKind(),1 ).TimeString());;
       if(Current.GetShiftKind()==3)  // if it is nightshift
       {
            Start -=1;
       }
       QueryString= " SELECT DocNum FROM CP_HEAD 			"
       				" WHERE Data BETWEEN :Start AND :Stop  	"
                    ;
       Query->Close();
       Query->SQL->Clear();
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("Start")->Value=Start;
       Query->Parameters->ParamByName("Stop")->Value=Stop;
       int NumberOfRowsAffected;
       Query->Open();
       NumberOfRowsAffected = Query->RecordCount;

       if (NumberOfRowsAffected>0) {
			Name = Query->FieldByName("DocNum")->AsString;
            Query->SQL->Clear();
            Query->Close();
       }
       else
       {
            Query->SQL->Clear();
        	Query->Close();
       		return NULL;
       }


       return Name;//Start.DateTimeString()+" "+Stop.DateTimeString();

     }
     __finally{

         //DataModule1->ADOConnectionGIP->Connected=false;
    }
}

void ExportFromBooth(bool CurrentShift) // if True try to Determine CurrentShift
										// document number else read given
                                        // in Edit field DocNo
{
     EmptySheetDefectsBooth();
     Contact = FormMain->EditDefectDocNum;//Contact person
    Remarks = FormMain->MemoDefects;
 	Combo = FormMain->ComboBoxDefects;
    TComboBox* Combo2 = FormMain->ComboBoxDefectsSize;
    Sheet = FormMain->StringGridDefectsBooth;
    TStringGrid* SheetNative = FormMain->StringGridDefectsNative;
    try{
       TADOQuery* Query = DataModule1->ADOQueryGIP;
       DataModule1->ADOConnectionGIP->Connected=true;
       AnsiString QueryString,Name;
        if (CurrentShift)
        {
                Name = GetDocNum();
                Contact->Text=Name;
        }
        else
        {
                Name = Contact->Text;
        }
       Query->SQL->Clear();
       Query->Close();
//       QueryString =
//                     " SELECT DEFECTS.Value as DefectType, CP_Param.Value AS Size,Sum(Quantity) AS NumberOf, Sum((Quantity*CP_Param.Val1*CP_Param.Val2)/1000000) AS Area 	"
//                     " FROM (CP_Defect INNER JOIN CP_Param ON CP_Defect.SizeID = CP_Param.ID) 																				"
//     				 " INNER JOIN (SELECT * FROM CP_Param WHERE TYPE = 'DEFECT') AS DEFECTS   																				"
//     				 " ON CP_DEFECT.DefectID = DEFECTS.ID                                                                                                                   "
//                     ;
//
//       QueryString+=" WHERE DocNum = ";
//       QueryString+="\'";
//       QueryString+=Name;
//       QueryString+="\'";
//       QueryString+=" GROUP BY DEFECTS.Value, CP_Param.Value ";
//					 //:DocNum
       QueryString = "select * from QueryDefectSum where DocNum = \'"+ Name +"\'";
       Query->SQL->Add(QueryString);
        Query->Open();
		int i=1;
		int NoOfLites;
		double Area;
		TempDefect.SumArea =0;
		TempDefect.SumAreaDevelopment=0;
		UnicodeString DefectType;
		while(!Query->Eof)
		{
			DefectType =  Query->FieldByName("DefectType")->AsString;
			Sheet->Cells[0][i] = DefectType;
			Sheet->Cells[1][i] = Query->FieldByName("Size")->AsString;
			Sheet->Cells[2][i]= Query->FieldByName("NumberOf")->AsInteger;
			//NoOfLites=
			Area = Query->FieldByName("Area")->AsFloat;
			if (Options.GetRestrictedLiteArea()) {
			  Area = SimpleRoundTo(Area,-2);
			}
			Sheet->Cells[3][i]= Area;
			TempDefect.SumArea += Area;
			if (DefectType == "Development") {
			  TempDefect.SumAreaDevelopment+=Area;
			}
			i++;
			Query->Next();
			if(i>=Sheet->RowCount)
				Sheet->RowCount=Sheet->RowCount+1;
		}
		i++;
		if(i>=Sheet->RowCount)
				Sheet->RowCount=Sheet->RowCount+1;
		Sheet->Cells[0][i]= "Summary : ";
		Sheet->Cells[3][i]= TempDefect.SumArea;
		WriteScrap();
		TGridRect R;
        R.Left=0;
        R.Top= i ;
        R.Right= 5 ;
        R.Bottom = i  ;
        Sheet->Selection=R;
    }
     __finally{
         DataModule1->ADOConnectionGIP->Connected=false;
    }

}

AnsiString ReturnField(AnsiString GIP)
{
	if(GIP=="Float") return "Flot";
	if(GIP=="Flaking")	return "Flacking";
	if(GIP=="Arcing") return "Arcing";
	if(GIP=="Out of specification") return "OUT OF SPEC";
	if(GIP== "Color search"  ) return "Color Searech"   ;
	if(GIP== "Test QC") return  "Test";
	if(GIP== "Ping Pong"  ) return "Ping Pong"   ;
	if(GIP=="Flash"   ) return "Flasch"   ;
	if(GIP=="Development" ) return "Development"    ;
	if(GIP=="Equipment"   ) return "Equipment"  ;
	if(GIP=="Other"		)  	return "Other" ;
	return "Other" ; // if name of defect not found


}

AnsiString ReturnFieldGIP(AnsiString Crep)
{
	if(Crep=="Flot") return  "Float";
	if(Crep=="Flacking")	return  "Flaking";
	if(Crep=="Arcing") return "Arcing";
	if(Crep=="OUT OF SPEC") return "Out of specification";
	if(Crep==  "Color Searech"   ) return  "Color search";
	if(Crep== "Test") return  "Test QC";
	if(Crep== "Ping Pong"  ) return "Ping Pong"   ;
	if(Crep== "Flasch" ) return     "Flash" ;
	if(Crep== "Development") return   "Development"  ;
	if(Crep=="Equipment"   ) return "Equipment"  ;
	if(Crep=="Other"		)	return "Other" ;
	return Crep;   // if any of values weren't valid
}

bool CheckIfExistsDefects()
{
	try{
           AnsiString QueryString, Name;
           int ID=Current.GetID(), NoOfRows;

           TADOQuery* Query = DataModule1->ADOQueryCreport;
           DataModule1->ADOConnectionCreport->Connected=true;

           Query->Close();
           Query->SQL->Clear();

           QueryString = "SELECT * FROM Defects WHERE ID_WorkShiftDay = :ID";
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

bool DeleteEntityDefects()
{
   try{
           AnsiString QueryString, Name;
           int ID=Current.GetID(), NoOfRows;

           TADOQuery* Query = DataModule1->ADOQueryCreport;
           DataModule1->ADOConnectionCreport->Connected=true;

           Query->Close();
           Query->SQL->Clear();

           QueryString = "DELETE * FROM Defects WHERE ID_WorkShiftDay = :ID";
           Query->SQL->Add(QueryString);
           Query->Parameters->ParamByName("ID")->Value=ID;
           NoOfRows=Query->ExecSQL();

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

bool InsertRemarksDefects() // There must be no entity in Defects table;
{
	try{
        	AnsiString QueryString, Name;
       		int ID=Current.GetID(), NoOfRows;

            TADOQuery* Query = DataModule1->ADOQueryCreport;
            DataModule1->ADOConnectionCreport->Connected=true;

    		Query->Close();
            Query->SQL->Clear();
            QueryString=	" INSERT INTO DEFECTS 		"
       	               		" (ID_WorkShiftDay, Comment) 	"
                       		" VALUES (:ID, :Comment)  	"
                    		;
          //  ExchangeAnsStr(QueryString,":Defect","["+ReturnField(Defect)+"]");
        	Query->SQL->Add(QueryString);
            Query->Parameters->ParamByName("ID")->Value=ID;
        	Query->Parameters->ParamByName("Comment")->Value=Remarks->Lines->Text;
            NoOfRows=Query->ExecSQL();
        	if (NoOfRows!=0) {
                return true;
            }
            else
            {
                return false;
            }
            }
       __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

bool WriteEntity(AnsiString Defect, int Quantity)  //return true if successful
{
	  bool ConnectedLocaly = false;
	   //	TStringList* Fields;
	  try{


	   AnsiString QueryString, Name;
	   int ID=Current.GetID(), NoOfRows;

	   TADOQuery* Query = DataModule1->ADOQueryCreport;
	   if (!DataModule1->ADOConnectionCreport->Connected) {
			DataModule1->ADOConnectionCreport->Connected=true;
			ConnectedLocaly = true;
	   }


//       Query->Close();
//       Query->SQL->Clear();
//
//       QueryString = "SELECT * FROM Defects WHERE ID_WorkShiftDay = :ID";
//       Query->SQL->Add(QueryString);
//       Query->Parameters->ParamByName("ID")->Value=ID;
//       Query->Open();
//
//
//
//       if(Query->RecordCount==0)
//       {
//        }
//        else
//        {

// **************   block bellow commented at  26.06.2012, it seams not to have reason

//			Fields = new TStringList;
//			DataModule1->ADOConnectionCreport->GetFieldNames("Defects",Fields);
//			Fields->CaseSensitive=false;
//			Fields->Sort();
//			AnsiString Lista = Fields->Text;
//			int index;
//			if (Fields->Find(Defect,index)) { // if Defect not found among Fields
//			   //	  Defect = "Other";
//			   return false;
//			}

// **************   correction added 26.06.2012    ***************************
int OtherInt;
if (ReturnField(Defect)== "Other" ) {
			Query->Close();
			Query->SQL->Clear();
			QueryString=" SELECT Other FROM DEFECTS "
						" WHERE ID_WorkShiftDay = :ID  	"
						;
			ExchangeAnsStr(QueryString,":ID",ID);
			Query->SQL->Add(QueryString);
			Query->Open();
			if (Query->RecNo!=1) { // if so there is an error
				return false;
			}
			OtherInt = Query->FieldByName("Other")->AsInteger;
			Quantity = Quantity + OtherInt;
}


// ***********  end of   correction added 26.06.2012    ***********************


			Query->Close();
			Query->SQL->Clear();
			QueryString=" UPDATE DEFECTS 				"
						" SET  ZapDefect = :Quantity  	"
						" WHERE ID_WorkShiftDay = :ID  	"
						;
			ExchangeAnsStr(QueryString,"ZapDefect","["+ReturnField(Defect)+"]");
			ExchangeAnsStr(QueryString,":Quantity", Quantity);
			ExchangeAnsStr(QueryString,":ID",ID);
			Query->SQL->Add(QueryString);
		 //  Remarks->Lines->Add(QueryString);
		 //   Query->Parameters->ParamByName("ID")->Value=ID;
		 //	Query->Parameters->ParamByName("Quantity")->Value=Quantity;
			NoOfRows=Query->ExecSQL();
			if (NoOfRows!=0) {
				return true;
			}
			else
			{
				return false;
			}
//        }
	  }
	   __finally{
	   if (ConnectedLocaly) {
		  DataModule1->ADOConnectionCreport->Connected=false;

	   }

	}
}

struct DatabaseErr { } E;

void ReadNative();

void FillUpDefectsNative() //throw(DatabaseErr)
{
  ReadNative();
  return;

//  TStringGrid* SheetNative = FormMain->StringGridDefectsNative;
//  bool ConnectedLocaly = false;
//  TStringList* Fields=0;
//  AnsiString QueryString, Name;
//
//
//  try{
//	  try{
//		int ID=Current.GetID(), NoOfRows;
//		TADOQuery* Query = DataModule1->ADOQueryCreport;
//		if (!DataModule1->ADOConnectionCreport->Connected) {
//			DataModule1->ADOConnectionCreport->Connected=true;
//			ConnectedLocaly = true;
//		}
//		Fields = new TStringList;
//		DataModule1->ADOConnectionCreport->GetFieldNames("Defects",Fields);
//		Fields->CaseSensitive=false;
//		Fields->Sort();
//		AnsiString Pole = Fields->Text;
//
//		Query->Close();
//		Query->SQL->Clear();
//		QueryString=" SELECT * FROM DEFECTS "
//					" WHERE ID_WorkShiftDay = :ID  	"
//					;
//		ExchangeAnsStr(QueryString,":ID",ID);
//		Query->SQL->Add(QueryString);
//		Query->Open();
//		if (Query->RecNo!=1) { // if so there is an error
//			throw DatabaseErr();
//		}
//	  int DefectValue,j=1;
//	  /* TODO : Dokoñczyæ 20120626 */
//	  if((Fields->Count+2)>=SheetNative->RowCount)
//				SheetNative->RowCount=Fields->Count+2;
//		for (int i = 0; i < Fields->Count; i++) {
//			Pole = Fields->Strings[i];
//			if ( (Pole== "ID_WORKSHIFTDAY") || (Pole =="COMMENT" )) {
//				continue;
//			}
//			DefectValue=Query->FieldByName(Pole)->AsInteger;
//			if (DefectValue==0) {
//				continue;
//			}
//			SheetNative->Cells[0][j]=Pole;//ReturnField(pos->first);
//			SheetNative->Cells[1][j]=Query->FieldByName(Pole)->AsInteger;
//			j++;
//		}
//
//	  }
//	  catch(DatabaseErr)
//	  {
//			ShowMessage("Database error in function \"FillUpDefectsNative\" ");
//			throw;
//	  }
//	  catch(...)
//	  {
//			ShowMessage("Undefinied error in function \"FillUpDefectsNative\" ");
//			throw;
//	  }
//
//  }
//  __finally{
//	if (ConnectedLocaly) {
//		  DataModule1->ADOConnectionCreport->Connected=false;
//	}
//	if(Fields!=0) delete Fields;
//  }
}


void WriteToDatabaseDefects()
{
	EmptySheetDefectsNative();
	Contact = FormMain->EditDefectDocNum;
	Remarks = FormMain->MemoDefects;
	Combo = FormMain->ComboBoxDefects;
	TComboBox* Combo2 = FormMain->ComboBoxDefectsSize;
	Sheet = FormMain->StringGridDefectsBooth;
	TStringGrid* SheetNative = FormMain->StringGridDefectsNative;
	bool ConnectedLocaly = false;
	try{
	   TADOQuery* Query = DataModule1->ADOQueryGIP;
	   if( !DataModule1->ADOConnectionGIP->Connected)
	   {
				DataModule1->ADOConnectionGIP->Connected=true;
				ConnectedLocaly = true;
	   }
       AnsiString QueryString,Name;
       int Quantity,i=1,j=1;
       std::map<AnsiString,int> DefectsMap;
       while(!Sheet->Cells[0][i].IsEmpty())
       {
            Name = Sheet->Cells[0][i] ;
            Quantity = Sheet->Cells[2][i].ToInt();
            DefectsMap[Name]+=Quantity;
            i++;
       }
       if(CheckIfExistsDefects())
       {
           DeleteEntityDefects();     //clear before saving
       }
       InsertRemarksDefects();
       std::map<AnsiString,int>::iterator pos;
	   for(pos=DefectsMap.begin(); pos != DefectsMap.end();pos++)
       {
          if (WriteEntity(pos->first,pos->second)) {
//			  SheetNative->Cells[0][j]=pos->first;//ReturnField(pos->first);
//			  SheetNative->Cells[1][j]=pos->second;
			  j++;
          }
//		  if(i>=SheetNative->RowCount)
//				SheetNative->RowCount=Sheet->RowCount+1;
	   }
	   FillUpDefectsNative();

//        while(!Sheet->Cells[0][i].IsEmpty())
//        {
//
//            if (WriteEntity(Name,Quantity)) {
//              SheetNative->Cells[0][j]=ReturnField(Name);
//              SheetNative->Cells[1][j]=Quantity;
//              j++;
//            }
//            i++;
//            if(i>=SheetNative->RowCount)
//                SheetNative->RowCount=Sheet->RowCount+1;
//        }

        TGridRect R;
        R.Left=0;
        R.Top= j ;
        R.Right= 5 ;
        R.Bottom = j  ;
        SheetNative->Selection=R;

     }
	 __finally{
		if (ConnectedLocaly) {
		   DataModule1->ADOConnectionGIP->Connected=false;
		}

    }
}

void ReadNative()
{

    EmptySheetDefectsNative();
    Contact = FormMain->EditDefectDocNum;
    Remarks = FormMain->MemoDefects;
 	Combo = FormMain->ComboBoxDefects;
    TComboBox* Combo2 = FormMain->ComboBoxDefectsSize;
    Sheet = FormMain->StringGridDefectsBooth;
    TStringGrid* SheetNative = FormMain->StringGridDefectsNative;
    try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
       int Quantity,i=1,j=1;
       std::map<AnsiString,int> DefectsMap;
    	Query->Close();
           Query->SQL->Clear();

           QueryString = "SELECT * FROM Defects WHERE ID_WorkShiftDay = :ID";
           Query->SQL->Add(QueryString);
           Query->Parameters->ParamByName("ID")->Value=Current.GetID();
           Query->Open();

        while(!Query->Eof)
        {
            Quantity = Query->FieldByName("Flot")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Flot"]=Quantity;

            Quantity = Query->FieldByName("Flacking")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Flacking"]=Quantity;

            Quantity = Query->FieldByName("Arcing")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Arcing"]=Quantity;

            Quantity = Query->FieldByName("Ping Pong")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Ping Pong"]=Quantity;

            Quantity = Query->FieldByName("Color Searech")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Color Searech"]=Quantity;

            Quantity = Query->FieldByName("Test")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Test"]=Quantity;

            Quantity = Query->FieldByName("Equipment")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Equipment"]=Quantity;

            Quantity = Query->FieldByName("Flasch")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Flasch"]=Quantity;

            Quantity = Query->FieldByName("Out of Spec")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Out of Spec"]=Quantity;

            Quantity = Query->FieldByName("Development")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Development"]=Quantity;

            Quantity = Query->FieldByName("Other")->AsInteger;
            if(Quantity>0)
            	DefectsMap["Other"]=Quantity;

            Remarks->Lines->Text = Query->FieldByName("Comment")->AsString;
            Query->Next();
        }
       std::map<AnsiString,int>::iterator pos;
       for(pos=DefectsMap.begin(); pos != DefectsMap.end();pos++)
       {

			  SheetNative->Cells[0][j]=ReturnFieldGIP(pos->first);
              SheetNative->Cells[1][j]=pos->second;
              j++;

          if(j>=SheetNative->RowCount)
                SheetNative->RowCount=Sheet->RowCount+1;
       }
       SheetNative->Cells[0][j]=Remarks->Lines->Text;
  }
     __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}

void ReadDefectsFromDataBase()
{
  ExportFromBooth(true);
  ReadNative();
}


 void TDefectDetail::Add(UnicodeString KindNa,UnicodeString SizeN,int KindNr, int NumberOfElements,double DArea,
			   double W,   double L,   int AddID,   UnicodeString AddInfo,
				UnicodeString AddInfoEng,   int PID,    UnicodeString PName,   UnicodeString FullPName,
				UnicodeString Comm,    int SubID,   UnicodeString SubName,   int SubTypeID,
				UnicodeString SubType,   int ThickID,   UnicodeString ThickName,
				TDateTime ShDate, char Sh)
 {
   KindName = KindNa;
   SizeName = SizeN;
   Kind = KindNr;
   int	Number = NumberOfElements; //Quantity of defects
   Area = DArea;
   Width = W ;
   Length = L ;
   if(Options.GetRestrictedLiteArea())
   {
	  Area = SimpleRoundTo((W*L)/1000000.0,-2)*NumberOfElements  ;
   }
   AdditionalID = AddID ;
   AdditionalInfo = AddInfo;
   AdditionalInfoEng = AddInfoEng;
   ProdID =PID;
   ProdName =PName;
   FullProdName = FullPName;
   Comment = Comm;
   SubstrateID =SubID;
   SubstrateName =SubName;
   SubstrateTypeID =SubTypeID;
   SubstrateType =SubType;
   ThicknessID=ThickID;
   ThicknessName=ThickName;
   ShiftDate = ShDate;
   Shift = Sh;
 }

 bool GreaterThickness(UnicodeString ThickName, UnicodeString ThickName2)
 {
	 int Thickness, Thickness2;
	 double ThickLami, ThickLami2;
	 UnicodeString SecondPart;
	 UnicodeString ThirdPart;
	 if (ThickName.Pos(".")!=0 && ThickName2.Pos(".")==0) {  //ThickName is Lami and ThickName2 not - Lami is greater than float
		return true;
	 }
	 if (ThickName.Pos(".")==0 && ThickName2.Pos(".")!=0) {  //ThickName2 is Lami and ThickName not - Lami is greater than float
		return false;
	 }
	 if (ThickName.Pos(".")==0 && ThickName2.Pos(".")==0) {  //booth are float
		Thickness = ThickName.ToInt();
		Thickness2 = ThickName2.ToInt();
		return (Thickness>Thickness2);
	 }
	 // then both shouldb be lamii
	 Thickness=UnicodeString(ThickName.SubString(1,ThickName.Pos(".")-1)).ToInt();
	 SecondPart =  ThickName.SubString(ThickName.Pos(".")+1,ThickName.Length()-ThickName.Pos("."));
	 Thickness = Thickness + UnicodeString(SecondPart.SubString(1,SecondPart.Pos(".")-1)).ToInt();
	 ThirdPart = SecondPart.SubString(SecondPart.Pos(".")+1,SecondPart.Length()-SecondPart.Pos("."));
	 Thickness2=  ThirdPart.ToInt();
	 ThickLami=Thickness + Thickness2*0.38;

	 Thickness=UnicodeString(ThickName2.SubString(1,ThickName2.Pos(".")-1)).ToInt();
	 SecondPart =  ThickName2.SubString(ThickName2.Pos(".")+1,ThickName2.Length()-ThickName2.Pos("."));
	 Thickness = Thickness + UnicodeString(SecondPart.SubString(1,SecondPart.Pos(".")-1)).ToInt();
	 ThirdPart = SecondPart.SubString(SecondPart.Pos(".")+1,SecondPart.Length()-SecondPart.Pos("."));
	 Thickness2=  ThirdPart.ToInt();
	 ThickLami2=Thickness + Thickness2*0.38;

	 return ThickLami > ThickLami2;
 }

bool TDefectDetail::operator<(const TDefectDetail& D)
{
  if (*this == D) {
	  return false;
  }
  if (ProdID > D.ProdID) {
	  return false;
  }
  if (ProdID == D.ProdID) {
	   if (Kind > D.Kind) {
		  return false;
	   }
	   if (Kind == D.Kind) {
		  if (!GreaterThickness(ThicknessName,D.ThicknessName)) {
			  return false;                                       // for sorting from lesser to greater thickness
		  }
		  if (ThicknessID == D.ThicknessID) {


			  if (Length > D.Length) {
				 return false;
			  }
			  if (Length == D.Length)
			  {
				 if (Width > D.Width) {
					return false;
				 }
				 if (Width == D.Width) {
					   if (SubstrateID > D.SubstrateID) {
						   return false;
					   }
					   if (SubstrateID == D.SubstrateID) {
						 if (SubstrateTypeID > D.SubstrateTypeID) {
							 return false;
						 }
						 if (SubstrateTypeID == D.SubstrateTypeID) {
							if (AdditionalID > D.AdditionalID) {
								return false;
							}
							if (AdditionalID == D.AdditionalID) {
								 return false;
							}
							return true;
						 }
						 return true;
					   }
					   return true;
				 }
				 return true;
			  }
			  return true;
		  }
		  return true;
	   }
	   return true;
  }
  else
  {
	return true;
  }
}

void TDefects::Add(UnicodeString KindNa,UnicodeString SizeN,int KindNr, int NumberOfElements,double DArea)
{
   TDefectDetail K = TDefectDetail(KindNa,SizeN,KindNr,NumberOfElements,DArea);
   DefectsList.push_back(K);
}

void TDefects::Add(TDefectDetail DefDet)  //insert into sequenced list
{
	std::list<TDefectDetail>::iterator iter;
	std::list<TDefectDetail>& CurrList = DefectsList;

	iter = CurrList.begin();

	while(*iter < DefDet || iter!=CurrList.end())
	  iter++;
	if (*iter==DefDet) {      //if the same defect then increase quantity and add comment
	   iter->IncreaseQuantity(DefDet.GetNumber()); // GetNumber returns number (quantity) of defect of given kind
	   iter->AddComment(DefDet.GetComment());
	}
	else
	{
	  CurrList.insert(iter,DefDet);
	}
}

void TDefects::Add(TDefectDetail DefDet, int ShiftID)  //insert into sequenced list
{
	std::list<TDefectDetail>::iterator iter;
//	std::list<TDefectDetail> TempList;
//	if (Lists.count(ShiftID)==0) { // ShiftID do not exists
//
//	  Lists[ShiftID] =  TempList;
//	}
	std::list<TDefectDetail>& CurrList = Lists[ShiftID];

	iter = CurrList.begin();

	while(*iter < DefDet && iter!=CurrList.end())
	  iter++;
	if (*iter==DefDet) {      //if the same defect then increase quantity and add comment
	   iter->IncreaseQuantity(DefDet.GetNumber()); // GetNumber returns number (quantity) of defect of given kind
	   iter->AddComment(DefDet.GetComment());
	}
	else
	{
	  CurrList.insert(iter,DefDet);
	}
	std::list<int>::iterator IntIter = ShiftsIDListsDateOrdered.begin();
	while ( IntIter!=ShiftsIDListsDateOrdered.end() && Lists[*IntIter].begin()->GetDate() < DefDet.GetDate() )
	{
	 	IntIter++;
	}
	ShiftsIDListsDateOrdered.insert(IntIter,ShiftID);

}

TDefectDetail TDefects::GetNextDefect()
{
  static TDefectDetail T= *DefectsList.begin();
  if (NextEfectExists() ) {
	T = *it++;
	return T;
  }
  else
	return T;
}
TDefectDetail TDefects::GetCurrentDefect()
{
  static TDefectDetail T= *DefectsList.begin();
  if (NextEfectExists() ) {
	T = *it;
  }
	return T;
}

bool TDefects::NextEfectExists()
{
  return it!=DefectsList.end();
}


bool TDefects::StartIteration()
{
	it = DefectsList.begin();
	return it== DefectsList.begin();
}


void PrepareDefectsReport(std::list<int> WorkShiftIDsList, TDefects& DL)
{
	TDefectDetail  Defect;
	UnicodeString KindNa;
	UnicodeString SizeN;
	int KindNr;
	int NumberOfElements;
	double DArea;
	double Width;
	double Length;
	int AdditionalID;
	UnicodeString AdditionalInfo;
	UnicodeString AdditionalInfoEng;
	int ProdID;
	UnicodeString ProdName;
	UnicodeString FullProdName;
	UnicodeString Comment;
	int SubstrateID;
	UnicodeString SubstrateName;
	int SubstrateTypeID;
	UnicodeString SubstrateType;
	int ThicknessID;
	UnicodeString ThicknessName;
	TDateTime ShDate;
	char Sh;

	UnicodeString Temp1, Temp2;
	try{
	 if(DataModule1->ADOConnectionCreport->Connected==false)
	 {
		DataModule1->ADOConnectionCreport->Connected = true;
	 }
	 TADOTable* Table = DataModule1->ADOTableDefectsExpanded;
	 TADOQuery* Query = DataModule1->ADOQueryTemp;
	 UnicodeString QueryString = "SELECT *   FROM [CP_DEFECT_EXPANDED_VIEW]";



	 std::list<int>::iterator it;
	 it = WorkShiftIDsList.begin();

	 UnicodeString ShiftsList="";
	 while(it!=WorkShiftIDsList.end())
	 {
	   ShiftsList+= ", "+ UnicodeString(*it);
	   it++;
	 }
	 ShiftsList.Delete(1,1); //remove first comma
	 ShiftsList.Insert("(",1);

	 ShiftsList +=")";
	 ShiftsList = " Where [ID_WORKSHIFTDAY] IN " + ShiftsList;
	 ShiftsList += "\n" ;
	 ShiftsList += " ORDER BY [Data] DESC ";
	 QueryString += ShiftsList;
//	 Table->Filter =  ShiftsList;
//	 Table->Filtered=true;
	 Query->SQL->Clear();
	 Query->SQL->Add(QueryString);
	 Query->Open();
	 while (!Query->Eof)
	 {
	   KindNa = Query->FieldByName("Defect")->AsString;
	   Temp1 = Query->FieldByName("Width")->AsString;
	   Temp2 = Query->FieldByName("Length")->AsString;
	   SizeN = Temp1 + " x " + Temp2;
	   Width = Temp1.ToInt();
	   Length = Temp2.ToInt();
	   NumberOfElements =  Query->FieldByName("Quantity")->AsInteger;
	   DArea =  Query->FieldByName("Area")->AsFloat;
	   if (Options.GetRestrictedLiteArea()) {
		  DArea = SimpleRoundTo((Length*Width)/10000000,-2)*NumberOfElements;
	   }
	   AdditionalID = Query->FieldByName("ID_ADDITIONALDEFECT")->AsInteger;
	   AdditionalInfo = Query->FieldByName("AddDefect")->AsString;
	   AdditionalInfoEng = Query->FieldByName("EngAddDefect")->AsString;
	   ProdID = Query->FieldByName("ID_PRODTYPE")->AsInteger;
	   ProdName = Query->FieldByName("ProdType_Name")->AsString;
	   FullProdName = Query->FieldByName("FULLNAME")->AsString;
	   Comment = Query->FieldByName("COMMENT")->AsString;
	   SubstrateID = Query->FieldByName("ID_SUBSTRATE")->AsInteger;
	   SubstrateName = Query->FieldByName("Substrate Origin")->AsString;
	   SubstrateTypeID = Query->FieldByName("ID_SUBSTRATECLASS")->AsInteger;
	   SubstrateType = Query->FieldByName("Substrate Type")->AsString;
	   ThicknessID = Query->FieldByName("ID_THICKNESS")->AsInteger;
	   ThicknessName = Query->FieldByName("Thickness")->AsString;
	   ShDate = Query->FieldByName("DATA")->AsDateTime;
	   UnicodeString TempSh = Query->FieldByName("SHIFT")->AsString;
	   Sh = TempSh[1];
	   Defect.Add(KindNa, SizeN, KindNr, NumberOfElements, DArea,
				  Width, Length, AdditionalID, AdditionalInfo,
				  AdditionalInfoEng, ProdID, ProdName, FullProdName,
				  Comment, SubstrateID, SubstrateName, SubstrateTypeID,
				  SubstrateType, ThicknessID, ThicknessName, ShDate, Sh);
	   DL.Add(Defect,Query->FieldByName("ID_WORKSHIFTDAY")->AsInteger );
	   Query->Next();
	 }
	}
	__finally{
		 if(DataModule1->ADOConnectionCreport->Connected==true)
		 {
			DataModule1->ADOConnectionCreport->Connected = false;
		 }
	}
}
