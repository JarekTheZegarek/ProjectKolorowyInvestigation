//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitProduction.h"
#include <map>
#include "Global.h"
#include "DataCoater.h"
#include <math.hpp>
#include "UnitMain.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)


TProduction Total(0);





    void TProduction::SetSpeed(double S)
    {
        Speed=S;
    }

    void TProduction::SetTotalIn(double In)
    {
        Total_in = In;
    }


     void TProduction::SetTotalOut(double Out)
     {
         Total_out=Out;
     }


   	bool TProduction::SetYield()
    {
        if(Total_in!=0 && Total_out<=Total_in)
        {
        		Yield=RoundTo((Total_out/Total_in)*100,-2);
                return true;
        }
        else
        {
            	Yield = 0;
                return false;
        }
    }


     void TProduction::SetID_Product_Type(int ID)
     {
         ID_Product_Type = ID;
     }


     int TProduction::GetID_WorkshiftDay()
     {
        return ID_WorkshiftDay;
     }


    int TProduction::GetID_Product_Type()
    {
    	return ID_Product_Type;
    }

    double TProduction::GetSpeed()
    {
        return RoundTo(Speed,-1);
    }

    double TProduction::GetTotalIn()
    {
        return RoundTo(Total_in,-2);
    }

    double TProduction::GetTotalOut()
    {
        return RoundTo(Total_out,-2);
    }


    double TProduction::GetYield()
    {
       SetYield();
       return RoundTo(Yield,-2);
    }

    void TProduction::SetRemarks(TStringList* List)
    {
        if (Remarks!=0) {
			delete Remarks;
        }
        Remarks = new TStringList;
        Remarks->Text= List->Text;
    }

    void TProduction::SetRemarks(AnsiString AddText)
    {
        if (Remarks!=0) {
            delete Remarks;
        }
        Remarks = new TStringList;
        Remarks->Text= AddText;
    }

TStringList* TProduction::GetRemarks()
{
   if (Remarks!=0) {
      return Remarks;
   }
    return new TStringList();  //just in case
}

void TProduction::DeleteRemarks()
{
   if(Remarks!=0)
      delete Remarks;
   Remarks = 0;
}

void SetProduction()     //Writes data from Form to Object CurrentProduction
{
   if(CurrentProduction!=0)
   			delete CurrentProduction;
   CurrentProduction = new TProduction(Current.GetID());
   try{
	  CurrentProduction->SetTotalIn(AnsiToDouble(FormMain->EditIn->Text));//.ToDouble());
      CurrentProduction->SetTotalOut(AnsiToDouble(FormMain->EditOut->Text));//.ToDouble());
      CurrentProduction->SetID_Product_Type(ComboIndexToProduct[FormMain->ComboBoxProducts->ItemIndex].GetID());
      CurrentProduction->SetSpeed(AnsiToDouble(FormMain->EditSpeed->Text));//.ToDouble());
      if(!FormMain->MemoProductRemarks->Lines->Text.IsEmpty())
         CurrentProduction->SetRemarks(FormMain->MemoProductRemarks->Lines->Text);  //dynamic_cast<TStringList*>(
    //  else
    //     CurrentProduction->DeleteRemarks();
   }
   catch(EConvertError& Blad)
   {
       ShowMessage(Blad.Message);
       return;
   }
}

void WriteScrap()
{
	double CalculatedScrap = Total.GetTotalIn()-Total.GetTotalOut();
	double ReportedScrap = TempDefect.SumArea;
	double ReportedDevelopment = TempDefect.SumAreaDevelopment;
	FormMain->EditCalculatedScrap->Text = CalculatedScrap;
	FormMain->EditReportedScrap->Text = ReportedScrap;
	double DevelopmentDifference = ReportedScrap - CalculatedScrap - ReportedDevelopment;
	if(ReportedDevelopment != 0 )
			FormMain->EditReportedMinusDevelopmentScrap->Text = fabs(ReportedScrap - ReportedDevelopment);
	else
			FormMain->EditReportedMinusDevelopmentScrap->Text = "";
	UnicodeString LabelDiscrepancy;
	FormMain->LabelDiscrepancy->Visible = false;
	if (fabs(DevelopmentDifference)>0.5 ) {
	   FormMain->LabelDiscrepancy->Visible=true;
	}
}

void FillingUpProductsSheet(int ShiftID)
{
        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        EmptyProductionSheet();
        AnsiString QueryString,Temp;
        int TempIndex;
        TProduction ReadP(ShiftID);
		Total.SetShiftId(ShiftID);
        TProducts CurrentProd;
      	Query->SQL->Clear();
        QueryString =
                   				" SELECT  Prod_Summary.*, ProductionRemarks.Remarks       					"
								" FROM   Prod_Summary LEFT JOIN ProductionRemarks                           "
       							" ON Prod_Summary.ID_Workshiftday = ProductionRemarks.ID_Workshiftday       "
          						"	AND                                                                     "
       							"	Prod_Summary.ID_Prod_Type = ProductionRemarks.ID_Prod_Type              "
								" WHERE                                                                     "
        						"	Prod_Summary.ID_Workshiftday = :ID_Workshiftday                         "
                        ;
           Query->SQL->Add(QueryString);
           int IDW = ShiftID; //CurrentProduction->GetID_WorkshiftDay();
//           int IDP=0;
//           if(CurrentProduction!=0)
//                 IDP = CurrentProduction->GetID_Product_Type();
           Query->Parameters->ParamByName("ID_Workshiftday")->Value=IDW;
	//       Query->Parameters->ParamByName("ID_ProdType")->Value= IDP  ;
    //       Query->Parameters->ParamByName("SPEED")->Value=CurrentProduction->GetSpeed();
    //       Query->Parameters->ParamByName("TOTAL_IN")->Value=CurrentProduction->GetTotalIn();
    //       Query->Parameters->ParamByName("TOTAL_OUT")->Value=CurrentProduction->GetTotalOut();
    //       Query->Parameters->ParamByName("YIELD")->Value=CurrentProduction->GetYield();
           Query->Open();
    //       int RecordsOfTheSameProduct = Query->RecordCount;
    //       if(RecordsOfTheSameProduct==1)  // Update
    //       {
    //            if(Application->MessageBox("Chcesz zmieniæ zapisane dane? \n\nDo you want to change saved data","Uwaga/Attention!!",MB_YESNO)==IDYES)
    //            {
    //
    //            }
           if(Query->RecordCount==0)
           	return;
    	   int i=1;
           std::map<int,TProducts>::iterator pos;
           ArrayOfProducts.ClearMap();
           Total.SetTotalIn(0);
           Total.SetTotalOut(0);
           while(!Query->Eof)
           {
                ReadP.SetSpeed(Query->FieldByName("Speed")->AsFloat);
                ReadP.SetTotalIn(Query->FieldByName("Total_In")->AsFloat);
                ReadP.SetTotalOut(Query->FieldByName("Total_Out")->AsFloat);
                ReadP.SetID_Product_Type(Query->FieldByName("ID_Prod_Type")->AsInteger);
                if(!Query->FieldByName("Remarks")->IsNull)
                {
                   ReadP.SetRemarks(Query->FieldByName("Remarks")->AsString);
                }
                else
                {
                    ReadP.DeleteRemarks();
                }

                // Combine product with its name
                for (pos = ComboIndexToProduct.begin(); pos != ComboIndexToProduct.end(); ++pos) {
                    if (pos->second.GetID() == ReadP.GetID_Product_Type()) {
                                    CurrentProd = pos->second;
                                    break;
                    }
                }
                Total.SetTotalIn(Total.GetTotalIn()+ReadP.GetTotalIn());
                Total.SetTotalOut(Total.GetTotalOut()+ReadP.GetTotalOut());

                FormMain->StringGridProducts->Cells[1][i] = CurrentProd.GetCombinedName();
                FormMain->StringGridProducts->Cells[2][i] = ReadP.GetSpeed();
                FormMain->StringGridProducts->Cells[3][i] = ReadP.GetTotalIn();
                FormMain->StringGridProducts->Cells[4][i] = ReadP.GetTotalOut();
                FormMain->StringGridProducts->Cells[5][i] = ReadP.GetYield();
                if(ReadP.CheckIfRemarksExists())
                {
                    Temp = ReadP.GetRemarks()->Text;
                    while((TempIndex=Temp.Pos("\r\n"))!=0)  //Removing Carriage Return from string
                    {
                        Temp.Delete(TempIndex,2);
                        Temp.Insert(" ",TempIndex);
                    }
                    FormMain->StringGridProducts->Cells[6][i] = Temp ;
                }
                else
                {
                    FormMain->StringGridProducts->Cells[6][i]="";
                }
                //ReadP.SetRemarks();

                ArrayOfProducts.Add(i,ReadP.GetID_Product_Type()); // Identyfing Products with SpreadSheet
                i++;
                Query->Next();
                if(i>=FormMain->StringGridProducts->RowCount)
                	FormMain->StringGridProducts->RowCount=FormMain->StringGridProducts->RowCount+1;
           }
            EmptyProductionForm();
            FormMain->EditInTotal->Text= Total.GetTotalIn();
            FormMain->EditOutTotal->Text = Total.GetTotalOut();
			FormMain->EditYieldTotal->Text = Total.GetYield();
			WriteScrap();
            TGridRect R;
            R.Left=1;
            R.Top= i ;
            R.Right= 6 ;
            R.Bottom = i  ;
            FormMain->StringGridProducts->Selection=R;
            DataModule1->ADOConnectionCreport->Connected=false;
//           if (RecordsOfTheSameProduct!=1) {
//                 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
//                 Message+=" ";
//                 Message+="\n\n";
//                 Message+="There is mistake in database!" ;
//                 Application->MessageBox(Message.c_str(),"Uwaga - Attention!!!",MB_OK);
//                 return;
//           }
}



void AddProduction()
{
   SetProduction();
   try{
       TADOQuery* Query = DataModule1->ADOQueryCreport;
       DataModule1->ADOConnectionCreport->Connected=true;
       AnsiString QueryString,Name;
      //
       bool RemarksAreWritten = false;

       // Checking integrity - i.e. if update data or insert new
       // beneath checking of existence;

	   Query->SQL->Clear();
	   QueryString = "SELECT ID_WorkshiftDay, ID_Prod_Type FROM ProductionRemarks "
					 "WHERE  ID_Workshiftday = :ID_Workshiftday AND ID_Prod_Type =:ID_ProdType   "
					  ;
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=CurrentProduction->GetID_WorkshiftDay();
	   Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
	   Query->Open();
	   if(Query->RecordCount!=0)           // Are there remarks for current product?
			RemarksAreWritten = true;

	   Query->SQL->Clear();

	   double In,Out,Speed;


	   QueryString = "SELECT * FROM PROD_SUMMARY "
					  "WHERE ID_Workshiftday = :ID_WorkshiftDay and ID_Prod_Type =:ID_ProdType   "
					  ;
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=CurrentProduction->GetID_WorkshiftDay();
	   Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
	   Query->Open();
							   //Exists given product in database i.e.  are we dealing with correction?
	   AnsiString Message;
	   if (Query->RecordCount>1) {
			 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
			 Message+=" Isnieje wiêcej ni¿ jeden zapis dla tego samego produktu i tej samej zmiany.";
			 Message+="\n\n															  ";
			 Message+="There is mistake in database! There are more than one entity \n";
			 Message+="for the same product and shift kind.";
			 wchar_t* Buf;
			 Application->MessageBox(Message.WideChar(Buf,Message.WideCharBufSize()),L"Uwaga - Attention!!!",MB_OK);
			 return;
	   }
	   In = Query->FieldByName("TOTAL_IN")->AsFloat;
	   Out = Query->FieldByName("TOTAL_OUT")->AsFloat;
	   Speed = Query->FieldByName("SPEED")->AsFloat;
	   double CheckSum = fabs(Speed-CurrentProduction->GetSpeed())+fabs(In-CurrentProduction->GetTotalIn())+fabs(Out-CurrentProduction->GetTotalOut());
	   if(Query->RecordCount==1 && CheckSum>0.01)  // Update as there is given product for one given shift
	   {
			if(Application->MessageBox(L"Chcesz zmieniæ zapisane dane? \n\nDo you want to change saved data",L"Uwaga/Attention!!",MB_YESNO)==IDYES)
            {
               Query->SQL->Clear();
               QueryString = " UPDATE PROD_SUMMARY  														"
                      		 " SET SPEED = :SPEED, TOTAL_IN=:TOTAL_IN, TOTAL_OUT=:TOTAL_OUT, 				"
                             "     YIELD = :YIELD 															"
                      		 " WHERE ID_Workshiftday = :ID_Workshiftday AND ID_Prod_Type =:ID_ProdType   	"
                      ;
               Query->SQL->Add(QueryString);
               Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=CurrentProduction->GetID_WorkshiftDay();
               Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
               Query->Parameters->ParamByName("SPEED")->Value=CurrentProduction->GetSpeed();
               Query->Parameters->ParamByName("TOTAL_IN")->Value=CurrentProduction->GetTotalIn();
               Query->Parameters->ParamByName("TOTAL_OUT")->Value=CurrentProduction->GetTotalOut();
               Query->Parameters->ParamByName("YIELD")->Value=CurrentProduction->GetYield();
               Query->ExecSQL();
               Query->SQL->Clear();


               if(RemarksAreWritten)   // There is entity in the ProductionRemarks relation
               {
                   Query->SQL->Clear();
                   QueryString = "UPDATE ProductionRemarks  "
                              "SET Remarks = :Remarks "
                   "WHERE ID_Workshiftday = :ID_Workshiftday AND ID_Prod_Type =:ID_ProdType   "
                              ;
                   Query->SQL->Add(QueryString);
                   if(CurrentProduction->GetRemarks()!=0)
                            Query->Parameters->ParamByName("Remarks")->Value=CurrentProduction->GetRemarks()->Text;
                   else  // No remarks to update means delete remark in database
                   {
                     Query->SQL->Clear();
                     QueryString = " DELETE  FROM ProductionRemarks "
                                   " WHERE ID_Workshiftday = :ID_Workshiftday AND ID_Prod_Type =:ID_ProdType   ";
                     Query->SQL->Add(QueryString);
                   }
                   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=CurrentProduction->GetID_WorkshiftDay();
                   Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
                   Query->ExecSQL();
               }
               else if(!FormMain->MemoProductRemarks->Text.IsEmpty())   // ADD new remark if previous were not existing
               {
                   Query->SQL->Clear();
                   QueryString = "INSERT INTO ProductionRemarks  				 "
                                 " ( ID_Workshiftday, Remarks, ID_Prod_Type )    "
                                 "VALUES 										 "
                                 "(:ID_Workshiftday ,:Remarks, :ID_ProdType )    "
                          ;
                   Query->SQL->Add(QueryString);
                   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=CurrentProduction->GetID_WorkshiftDay();
                   Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
                   Query->Parameters->ParamByName("Remarks")->Value=CurrentProduction->GetRemarks()->Text;
                   Query->ExecSQL();
                   RemarksAreWritten = true;
               } else
               {
                   RemarksAreWritten = false;
               }
            }         //------------------   I do not want to upgrade or change any data
            else                 // After Not updating any data
            {
               FillingUpProductsSheet(CurrentProduction->GetID_WorkshiftDay());
               return;
            }
       }   // ---------------------  End of dealing with upgrade   --------------------
       else if(Query->RecordCount==0)  // No data were found - adding new batch
        {
           Query->SQL->Clear();
           QueryString = "INSERT INTO PROD_SUMMARY  "
                  " (SPEED, TOTAL_IN, TOTAL_OUT, YIELD,ID_Workshiftday, ID_Prod_Type)  "
                  "VALUES   (:SPEED, :TOTAL_IN, :TOTAL_OUT,:YIELD, :ID_Workshiftday, :ID_ProdType) "
                  ;
           Query->SQL->Add(QueryString);
           Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=CurrentProduction->GetID_WorkshiftDay();
           Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
           Query->Parameters->ParamByName("SPEED")->Value=CurrentProduction->GetSpeed();
           Query->Parameters->ParamByName("TOTAL_IN")->Value=CurrentProduction->GetTotalIn();
           Query->Parameters->ParamByName("TOTAL_OUT")->Value=CurrentProduction->GetTotalOut();
           Query->Parameters->ParamByName("YIELD")->Value=CurrentProduction->GetYield();
           Query->ExecSQL();
           if(!FormMain->MemoProductRemarks->Text.IsEmpty())    // if any remarks exists :)
               {
                   Query->SQL->Clear();
                   QueryString = "INSERT INTO ProductionRemarks  				 "
                                 " ( ID_Workshiftday, Remarks, ID_Prod_Type )    "
                                 "VALUES 										 "
                                 "(:ID_Workshiftday ,:Remarks, :ID_ProdType )    "
                          ;
                   Query->SQL->Add(QueryString);
                   Query->Parameters->ParamByName("ID_WorkshiftDay")->Value=CurrentProduction->GetID_WorkshiftDay();
                   Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
                   Query->Parameters->ParamByName("Remarks")->Value=CurrentProduction->GetRemarks()->Text;
                   Query->ExecSQL();
                   RemarksAreWritten = true;
               }
        }

        // Filling up the StringGrid
        FillingUpProductsSheet(CurrentProduction->GetID_WorkshiftDay());

//        Query->SQL->Clear();
//        QueryString = 	"Select   *     from   PROD_SUMMARY 		"
//                        "WHERE                                		"
//                        "  ID_Workshiftday = :ID_Workshiftday  		"
//                        "AND                                    	"
//                        "  ID_Prod_Type = :ID_ProdType            	"
//                        ;
//           Query->SQL->Add(QueryString);
//           int IDW = CurrentProduction->GetID_WorkshiftDay();
//           int IDP = CurrentProduction->GetID_Product_Type();
//           Query->Parameters->ParamByName("ID_Workshiftday")->Value=IDW;
//           Query->Parameters->ParamByName("ID_ProdType")->Value= IDP  ;
//    //       Query->Parameters->ParamByName("SPEED")->Value=CurrentProduction->GetSpeed();
//    //       Query->Parameters->ParamByName("TOTAL_IN")->Value=CurrentProduction->GetTotalIn();
//    //       Query->Parameters->ParamByName("TOTAL_OUT")->Value=CurrentProduction->GetTotalOut();
//    //       Query->Parameters->ParamByName("YIELD")->Value=CurrentProduction->GetYield();
//           Query->Open();
//           int RecordsOfTheSameProduct = Query->RecordCount;
//           if(RecordsOfTheSameProduct==1)  // Update
//           {
//    //            if(Application->MessageBox("Chcesz zmieniæ zapisane dane? \n\nDo you want to change saved data","Uwaga/Attention!!",MB_YESNO)==IDYES)
//    //            {
//    //
//    //            }
//                ReadP.SetSpeed(Query->FieldByName("Speed")->AsFloat);
//                ReadP.SetTotalIn(Query->FieldByName("Total_In")->AsFloat);
//                ReadP.SetTotalOut(Query->FieldByName("Total_Out")->AsFloat);
//                ReadP.SetID_Product_Type(Query->FieldByName("ID_Prod_Type")->AsInteger);
//                if(RemarksAreWritten)
//                {
//                   Query->SQL->Clear();
//                   QueryString = 	"Select   *     from   ProductionRemarks "
//                        "WHERE                                		"
//                        "  ID_Workshiftday = :ID_Workshiftday  		"
//                        "AND                                    	"
//                        "  ID_Prod_Type = :ID_ProdType            	"
//                        ;
//                   Query->SQL->Add(QueryString);
//                   //IDW = CurrentProduction->GetID_WorkshiftDay();
//                   //IDP = CurrentProduction->GetID_Product_Type();
//                   Query->Parameters->ParamByName("ID_Workshiftday")->Value=IDW;
//                   Query->Parameters->ParamByName("ID_ProdType")->Value= IDP  ;
//                   Query->Open();
//                   ReadP.SetRemarks(Query->FieldByName("Remarks")->AsString);
//                }
//                TProducts CurrentProd;
//                std::map<int,TProducts>::iterator pos;
//                for (pos = ComboIndexToProduct.begin(); pos != ComboIndexToProduct.end(); ++pos) {
//                    if (pos->second.GetID() == ReadP.GetID_Product_Type()) {
//                                    CurrentProd = pos->second;
//                                    break;
//                    }
//                }
//                FormMain->StringGridProducts->Cells[1][FormMain->StringGridProducts->Row] = CurrentProd.GetCombinedName();
//                FormMain->StringGridProducts->Cells[2][FormMain->StringGridProducts->Row] = ReadP.GetSpeed();
//                FormMain->StringGridProducts->Cells[3][FormMain->StringGridProducts->Row] = ReadP.GetTotalIn();
//                FormMain->StringGridProducts->Cells[4][FormMain->StringGridProducts->Row] = ReadP.GetTotalOut();
//                FormMain->StringGridProducts->Cells[5][FormMain->StringGridProducts->Row] = ReadP.GetYield();
//                if(ReadP.GetRemarks()!=0)
//                    FormMain->StringGridProducts->Cells[6][FormMain->StringGridProducts->Row] = ReadP.GetRemarks()->Text;
//                //ReadP.SetRemarks();
//
//           }
//           if (RecordsOfTheSameProduct!=1) {
//                 Message="Wyst¹pi³a b³êdna konfiguracja danych w bazie danych!\n";
//                 Message+=" ";
//                 Message+="\n\n";
//                 Message+="There is mistake in database!" ;
//                 Application->MessageBox(Message.c_str(),"Uwaga - Attention!!!",MB_OK);
//                 return;
//           }
	}
    __finally{
         DataModule1->ADOConnectionCreport->Connected=false;
    }
}


void SetTemporaryYield()
{
   float Yield;
   TProduction Temp(CurrentId);
   Temp.SetTotalIn(AnsiToDouble(FormMain->EditIn->Text));//.ToDouble());
   Temp.SetTotalOut(AnsiToDouble(FormMain->EditOut->Text));//.ToDouble());
   if(!Temp.SetYield())
   {
      ShowMessage("Yield mo¿e byæ nieprawid³owy \n\n It is possible that Yield is invalid.");
   }
   Yield =  Temp.GetYield();
   FormMain->EditYield->Text=AnsiString().FormatFloat("#.00%",Yield);
}


void TProduction::FillUpForm()
{
   try{

      std::map<int,TProducts>::iterator pos;
      for (pos = ComboIndexToProduct.begin(); pos != ComboIndexToProduct.end(); ++pos)
      {
      	if (pos->second.GetID() == GetID_Product_Type()) {
                break;
        }
      }
      if(pos!=ComboIndexToProduct.end()) // no products was find
      {
        FormMain->ComboBoxProducts->ItemIndex = pos->first;
      }
        TADOQuery* Query =DataModule1->ADOQueryCreport;
        DataModule1->ADOConnectionCreport->Connected=true;
        AnsiString QueryString;
        TProducts CurrentProd;
      	Query->SQL->Clear();
        QueryString =
                   				" SELECT  Prod_Summary.*, ProductionRemarks.Remarks       					"
								" FROM   Prod_Summary LEFT JOIN ProductionRemarks                           "
       							" ON Prod_Summary.ID_Workshiftday = ProductionRemarks.ID_Workshiftday       "
          						"	AND                                                                     "
       							"	Prod_Summary.ID_Prod_Type = ProductionRemarks.ID_Prod_Type              "
								" WHERE                                                                     "
        						"	Prod_Summary.ID_Workshiftday = :ID_Workshiftday                         "
      							" AND                                                                       "
                                "   Prod_Summary.ID_Prod_Type =  :IDProdType                                 "
      							;
      Query->SQL->Add(QueryString);
      Query->Parameters->ParamByName("ID_Workshiftday")->Value=GetID_WorkshiftDay();
      Query->Parameters->ParamByName("IDProdType")->Value=GetID_Product_Type();
      Query->Open();
      SetSpeed(Query->FieldByName("Speed")->AsFloat);
      SetTotalIn(Query->FieldByName("Total_In")->AsFloat);
      SetTotalOut(Query->FieldByName("Total_Out")->AsFloat);
      SetID_Product_Type(Query->FieldByName("ID_Prod_Type")->AsInteger);
      if(!Query->FieldByName("Remarks")->IsNull)
      {
          SetRemarks(Query->FieldByName("Remarks")->AsString);
      }
      else
      {
          DeleteRemarks();
      }
      FormMain->EditIn->Text = GetTotalIn();
      FormMain->EditOut->Text = GetTotalOut();
      FormMain->EditSpeed->Text = GetSpeed();
      FormMain->EditYield->Text = AnsiString().FormatFloat("#.00%",GetYield());
      if(Remarks!=0)
            FormMain->MemoProductRemarks->Lines->Text = GetRemarks()->Text;
	  else
			FormMain->MemoProductRemarks->Clear();
	}   // end of try
	__finally{
		DataModule1->ADOConnectionCreport->Connected=false;
	}
}

void RemoveSingleProductProduction()
{
	try{
	  //int ProdID = ArrayOfProducts.GetID(FormMain->StringGridProducts->Selection.Top);
	  int ID=Current.GetID();
	  TADOQuery* Query =DataModule1->ADOQueryCreport;
	  AnsiString QueryString;
	 try {
		if(FormMain->ComboBoxProducts->ItemIndex==-1) // If no product selected - posibly fault writing
		{
		  DataModule1->ADOConnectionCreport->Connected=true;
		  DataModule1->ADOConnectionCreport->BeginTrans();

		  Query->Close();
		  Query->SQL->Clear();
		  QueryString =
						"    DELETE FROM Prod_Summary  "
						"    WHERE ";

		  Query->SQL->Add(QueryString);
		  QueryString=" ID_Workshiftday = "+UnicodeString(ID);
		  Query->SQL->Add(QueryString);
		  QueryString=" AND SPEED = "+ FormMain->EditSpeed->Text;
		  Query->SQL->Add(QueryString);
		  QueryString=  " AND TOTAL_IN = "+ FormMain->EditIn->Text;
		  Query->SQL->Add(QueryString);
		  QueryString= "  AND TOTAL_OUT = "+ FormMain->EditOut->Text;
		  Query->SQL->Add(QueryString);

  //		Query->Parameters->ParamByName("ID_Workshiftday")->DataType=ftInteger ;
  //		Query->Parameters->ParamByName("ID_Workshiftday")->Value=ID;
  //		float readdata;
  //		readdata =  FormMain->EditIn->Text.ToDouble();
  //		Query->Parameters->ParamByName("TOTAL_INa")->DataType = ftFloat;
  //		Query->Parameters->ParamByName("TOTAL_INa")->Value= readdata;
  //		Query->Parameters->ParamByName("SPEEDY")->DataType= ftFloat;
  //		readdata =  FormMain->EditSpeed->Text.ToDouble();
  //		Query->Parameters->ParamByName("SPEEDY")->Value=readdata;//FormMain->EditSpeed->Text.ToDouble();
  //
  //		Query->Parameters->ParamByName("TOTAL_OUTa")->DataType = ftFloat;
  //		readdata = FormMain->EditOut->Text.ToDouble();
  //		Query->Parameters->ParamByName("TOTAL_OUTa")->Value= readdata;
  //		Query->Prepared=true;
		  QueryString=Query->Parameters->ParseSQL(Query->SQL->Text, True);
		  int resultno;
		  resultno=Query->ExecSQL();
		  Query->Close();
		  Query->SQL->Clear();
		  DataModule1->ADOConnectionCreport->CommitTrans();
		  DataModule1->ADOConnectionCreport->Close();
		  DataModule1->ADOConnectionCreport->Open();
		}
		else
		{
		 int ProdID = ComboIndexToProduct[FormMain->ComboBoxProducts->ItemIndex].GetID();
		  DataModule1->ADOConnectionCreport->Connected=true;
		  DataModule1->ADOConnectionCreport->BeginTrans();
		  Query->Close();
		  Query->SQL->Clear();
		  QueryString =
						"    DELETE FROM Prod_Summary                    "
						"    WHERE                                         "
						"            ID_Workshiftday = :ID_Workshiftday    "
						"     AND                                          "
						"            ID_Prod_Type = :ID_ProdType           "
						;
		  Query->SQL->Add(QueryString);
		  Query->Parameters->ParamByName("ID_Workshiftday")->Value=ID;
		  Query->Parameters->ParamByName("ID_ProdType")->Value=ProdID;
		  Query->Prepared=true;
		  Query->ExecSQL();

		  Query->Close();
		  Query->SQL->Clear();
		  QueryString =
						"    DELETE FROM ProductionRemarks               "
						"    WHERE                                         "
						"            ID_Workshiftday = :ID_Workshiftday    "
						"     AND                                          "
						"            ID_Prod_Type = :ID_ProdType           "
						;
		  Query->SQL->Add(QueryString);
		  Query->Parameters->ParamByName("ID_Workshiftday")->Value=Current.GetID();
		  Query->Parameters->ParamByName("ID_ProdType")->Value=ProdID;
		  Query->ExecSQL();
		  Query->Close();
		  Query->SQL->Clear();
		  QueryString =
						"    DELETE FROM PACKED               "
						"    WHERE                                         "
						"            ID_Workshiftday = :ID_Workshiftday    "
						"     AND                                          "
						"            ID_Prod_Type = :ID_ProdType           "
						;
		  Query->SQL->Add(QueryString);
		  Query->Parameters->ParamByName("ID_Workshiftday")->Value=Current.GetID();
		  Query->Parameters->ParamByName("ID_ProdType")->Value=ProdID;
		  Query->ExecSQL();
		  Query->Close();




		  DataModule1->ADOConnectionCreport->CommitTrans();
		  DataModule1->ADOConnectionCreport->Close();
		  DataModule1->ADOConnectionCreport->Open();
		}

		  FillingUpProductsSheet(Current.GetID());
	  }   // end of try
	  catch (...) {
         DataModule1->ADOConnectionCreport->RollbackTrans();
	  }
	}
	__finally{
		DataModule1->ADOConnectionCreport->Connected=false;
	}
}

void EmptyProductionForm()
{
	  FormMain->ComboBoxProducts->ItemIndex = -1;
	  FormMain->EditIn->Text = "";//GetTotalIn();
	  FormMain->EditOut->Text = "";//GetTotalOut();
	  FormMain->EditSpeed->Text = "";//  GetSpeed();
	  FormMain->EditYield->Text = "";//GetYield();
	  FormMain->MemoProductRemarks->Clear();// = GetRemarks()->Text;
}

void TArrayOfProducts::Add(int Index, int ID)
{
	Products[Index] = ID;
}

int TArrayOfProducts::GetID(int Index)
{
	return  Products[Index];
}

void TArrayOfProducts::ClearMap()
{
	Products.clear();
}

void EmptyProductionSheet()
{
	for(int i=1;i<=FormMain->StringGridProducts->RowCount;i++)
		FormMain->StringGridProducts->Rows[i]->Clear();
	FormMain->EditInTotal->Text= "";
	FormMain->EditOutTotal->Text = "";
	FormMain->EditYieldTotal->Text = "";
}

void TSubstrate::Add(int SubstrateID, UnicodeString NameOfSubstrate, int SubstrateClassID, UnicodeString SubstrateClass,int SubstrateThicknessID, UnicodeString SubstrateThickness, UnicodeString SubstrateSizeName, int SubstrateSizeID, int SizeWidth, int SizeLength)
{
 ID = SubstrateID;
 Name = NameOfSubstrate;
 ClassID = SubstrateClassID;
 Class = SubstrateClass;
 ThicknessID = SubstrateThicknessID;
 Thickness = SubstrateThickness;
 Size = SubstrateSizeName;
 SizeID = SubstrateSizeID;
 Width = SizeWidth;
 Length = SizeLength;
}
