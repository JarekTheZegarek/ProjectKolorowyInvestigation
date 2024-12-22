//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitReport.h"
#include "Global.h"
#include "DataCoater.h"
#include "DateUtils.hpp"
#include "UnitMain.h"
#include <set>
//---------------------------------------------------------------------------

#pragma package(smart_init)

SunGuardsTimeClass Schedule;
TCathodeReport RecCathodes;
static TDateTime Start;
static TDateTime  Stop;


TKPIReport KPIReport;

void ClearSheet(TStringGrid* S)
{
	for(int i=0;i<S->ColCount;i++)
		for(int j=0;j<S->RowCount;j++)
		{
			S->Cells[i][j]="";
		}
}

void ReportHeader()
{
	int wys;
	TRichEdit* Ed = FormMain->RichEditReport;
	TMemo* Mem;
	TScrollBox* PrnBox = FormMain->ScrollBoxReportPrint;
	TCanvas* Canv  = FormMain->PaintBox1->Canvas;


   // static TFontStyles Previous = Ed->SelAttributes->Style;
	Ed->SelAttributes->Style = TFontStyles()<< fsBold ;
	Canv->Font->Style = TFontStyles()<< fsBold  ;
	Canv->TextOutA(20,20,Current.GetDescription());
	FormMain->PaintBox1->Refresh();
	Ed->Lines->Add(Current.GetDescription());
	Ed->SelAttributes->Style = Ed->DefAttributes->Style; //Previous ;
	Ed->Lines->Add("------------- Separator ------------------------");
	TPrinter* Prnt = Printer();
	Prnt->Canvas->Font = Ed->Font;
	wys = Prnt->Canvas->TextHeight("------------- Separator ------------------------");
	Ed->Lines->Add(wys);
	FormMain->PaintBox1->ClientHeight=Prnt->PageHeight;
	FormMain->PaintBox1->ClientWidth=Prnt->PageWidth;
 //   Ed->Font
}

TDateTime StartTime()
{
	TDateTimePicker* DTP = FormMain->DateTimePickerReportStart;
	TDateTimePicker* DTP_T = FormMain->DateTimePickerReportStartTime;
	DTP->Time = DTP_T->Time;
	return DTP->DateTime;
//    TDateTime T;
//    int Day = int(DTP->DateTime.Val);
//    double Time = DTP_T->DateTime.Val -   int(DTP->DateTime.Val);
//    T.Val = Day + Time;
}

TDateTime StopTime()
{
   TDateTimePicker* DTP = FormMain->DateTimePickerReportStop;
	TDateTimePicker* DTP_T = FormMain->DateTimePickerReportStopTime;
	DTP->Time = DTP_T->Time;
	return DTP->DateTime;
}





void SetDateShiftWise(TDateTime Begin, TDateTime End) // Set local for this file variables Start, Stop shiftwise  .
//reporting is shiftwise therfore we need to change start and stop date.
{
	  unsigned short year;
	  unsigned short month;
	  unsigned short day;
	  unsigned short hour;
	  unsigned short min;
	  unsigned short sec;
	  unsigned short msec;

	   Start =  Begin ;
	   Stop =   End;
	   Start.DecodeDate(&year, &month, &day);
	   Start.DecodeTime(&hour, &min, &sec, &msec);
	   if (hour >= 22) {
		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,0,0,0).TimeString());
	   }
	   Stop.DecodeDate(&year, &month, &day);
	   Stop.DecodeTime(&hour, &min, &sec, &msec);
	   if (hour > 22) {
		   Stop = TDateTime(TDateTime(Stop+1).DateString()+" "+TDateTime(6,59,0,0).TimeString());  //night shift is counted as next day
		   return;
	   }
	   if (hour>=6 && hour<14)
	   {
		   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(13,59,0,0).TimeString());  //night shift is counted as next day
		   return;
	   }
	   if (hour>=14 && hour<22)
	   {
		   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(21,59,0,0).TimeString());
		   return;
	   }
//		  Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString());
//	   }
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.


}


void SetDateShiftWise() // Set local for this file variables Start, Stop as values from DatePickUps - ShiftWise
//reporting is shiftwise therfore we need to change start and stop date.
{
   SetDateShiftWise(StartTime(), StopTime());
}


void BedUtilization()
{

try {

	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	   std::map <std::pair<double,double>,int> LengthWidthToQuantityAll;
	   std::map <std::pair<double,double>,int> LengthWidthToQuantitySG;
	   std::map <std::pair<double,double>,int> LengthWidthToQuantityCG;
	   UnicodeString QueryString =
	   "DECLARE @STARTDATE AS DATETIME                                               "
	   "DECLARE @STOPDATE AS DATETIME                                                                             "
	   "SET @STARTDATE = :Startdate                                                                               "
	   "SET @STOPDATE =  :Stopdate                                                                                "
	   "SELECT [WIDTH]/1000.0 AS 'Lite Width', [LENGTH]/1000.0 as 'Lite Length', SUM([NO OF LITES]) AS [NO OF LITES] "
	   "FROM (                                                                                                       "
	   "		   select Val1 AS WIDTH, Val2 AS LENGTH, SUM(NO_PACKS*NO_PANES) AS [NO OF LITES]                     "
	   "		   from ([PACKED]                                                                                    "
	   "		   INNER JOIN                                                                                        "
	   "		   [276CoaterReports].[dbo].[WORKSHIFTDAYS]                                                          "
	   "		   ON [WORKSHIFTDAYS].ID_WORKSHIFTDAY = PACKED.ID_WORKSHIFTDAY)                                      "
	   "		   INNER JOIN                                                                                        "
	   "		   [dbo].[CP_PARAM]                                                                                  "
	   "		   ON CP_PARAM.ID = PACKED.ID_SIZE                                                                   "
	   "		   WHERE DATE_OF_WORK  between @STARTDATE and @STOPDATE                                              "
	   "		   GROUP BY VAL2, VAL1                                                                               "
	   "	   UNION                                                                                                 "
	   "		   SELECT [WIDTH], [LENGTH],  SUM(QUANTITY) AS [NO OF LITES]                                         "
	   "		   FROM [dbo].[CP_PARAM_DEFECT_EXPANDED_VIEW]                                                        "
	   "		   WHERE [DATA]  between @STARTDATE and @STOPDATE                                                    "
	   "		                AND DEFECTID <> 14                                                                   "
	   "		   GROUP BY [Length], [WIDTH]                                                                        "
	   "		 ) AS TABELKA                                                                                        "
	   "	 GROUP BY [LENGTH], [WIDTH]                                                                              "
	   "	 ORDER BY [LENGTH] DESC                                                                                  ";
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("StartDate")->Value = StartTime();// "#08/01/2011 12:01:00 AM#";
	   Query->Parameters->ParamByName("StopDate")->Value = StopTime(); // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;
	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+10;
	   Sheet->ColCount=6;
	   AnsiString TempAnsi;
	   int FieldWidth[6] ={0,0,0,0,0,0};
	   int Width;
	   std::map<int,int>::iterator pos;
	   Sheet->Cells[0][0] = "Length";
	   Sheet->Cells[1][0] = "Width";
	   Sheet->Cells[2][0] = "Lite No";
	   Sheet->Cells[3][0] = "In ";
	   Sheet->Cells[4][0] = "No Batches";
	   Sheet->Cells[5][0] = "Bed Utilization";
	   FieldWidth[1]=Sheet->Canvas->TextWidth("Length")+5;
	   FieldWidth[2]=Sheet->Canvas->TextWidth("Width")+5;
	   FieldWidth[3]=Sheet->Canvas->TextWidth("Lite No")+5;
	   FieldWidth[4]=Sheet->Canvas->TextWidth("In")+5;
	   FieldWidth[5]=Sheet->Canvas->TextWidth("No Batches")+5;
	   FieldWidth[6]=Sheet->Canvas->TextWidth("Bed Utilization")+5;
	   double In, Quantity, LiteWidth, Length,Bed;
	   In=Bed=0;
	   int i;

	   for(i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {

		  Length = Query->FieldByName("Lite Length")->AsFloat;
		  TempAnsi= Length;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  LiteWidth =  Query->FieldByName("Lite Width")->AsFloat;
		  TempAnsi= LiteWidth ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ;

		  Quantity = Query->FieldByName("NO OF LITES")->AsInteger;
		  TempAnsi=   Quantity  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  Sheet->Cells[2][i] = TempAnsi;

		  LengthWidthToQuantityAll[std::pair<double,double>(Length,LiteWidth)]=Quantity;

		  In += Length*LiteWidth*Quantity;

		  TempAnsi=   Quantity*LiteWidth*Length  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  Sheet->Cells[3][i] = TempAnsi;



		  if (Length<3) {
			 Quantity=Quantity/2 ;
		  }
		  Bed += (int)Quantity*19.26;






		  TempAnsi=   Quantity*19.26  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];

		  Sheet->Cells[4][i] = TempAnsi;

		  TempAnsi=   (LiteWidth*Length*100.0)/19.26  ;
		  if (Length<3) {
			 TempAnsi=   (2*LiteWidth*Length*100.0)/19.26  ;
		  }
		  TempAnsi += " %" ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

		  Sheet->Cells[5][i] = TempAnsi;





		  Query->Next();
	   }
	   i++;
	   Sheet->Cells[0][i] = "In = ";
	   Sheet->Cells[1][i] = In;
	   i++;
	   Sheet->Cells[0][i] = "Bed = ";
	   Sheet->Cells[1][i] = Bed;
	   i++;
	   Sheet->Cells[0][i] = "BedUtil = ";
	   Sheet->Cells[1][i] = UnicodeString((In/Bed)*100);
	   KPIReport.BedUtil = (In/Bed)*100;
	   i+=2;

	   Sheet->Cells[1][i] = "SUNGUARDS";
		i+=2;
	   Width =  Sheet->Canvas->TextWidth("BedUtil = ");
	   FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

	   Query->Close();
	   for(int i=0;i<6;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }

	   QueryString =
	   "DECLARE @STARTDATE AS DATETIME                                               "
	   "DECLARE @STOPDATE AS DATETIME                                                                             "
	   "SET @STARTDATE = :Startdate                                                                               "
	   "SET @STOPDATE =  :Stopdate                                                                                "
	   " SELECT [WIDTH]/1000.0 AS 'Lite Width',[LENGTH] as 'Lite Length', SUM([NO OF LITES]) AS [NO OF LITES]                                   "
	   " FROM (                                                                                                                "
	   " 		   select Val1 AS WIDTH, Val2/1000.0 AS LENGTH, SUM(NO_PACKS*NO_PANES) AS [NO OF LITES]                        "
	   " 		   from ([276CoaterReports].[dbo].[PACKED]                                                                     "
	   " 		   INNER JOIN                                                                                                  "
	   " 		   [276CoaterReports].[dbo].[WORKSHIFTDAYS]                                                                    "
	   " 		   ON [276CoaterReports].[dbo].[WORKSHIFTDAYS].ID_WORKSHIFTDAY = PACKED.ID_WORKSHIFTDAY)                       "
	   " 		   INNER JOIN                                                                                                  "
	   " 		   [dbo].[CP_PARAM]                                                                                            "
	   " 	   ON CP_PARAM.ID = PACKED.ID_SIZE                                                                                 "
	   " 		   WHERE DATE_OF_WORK  between  @STARTDATE and @STOPDATE                                                       "
	   " 			   AND ID_PROD_TYPE IN (SELECT ID_PRODTYPE FROM [dbo].[PartToProdTypeViewCategorized] WHERE [ID] = 1)      "
	   " 												                                                                       "
	   " 			   GROUP BY VAL2, VAL1                                                                                     "
	   " 		   UNION                                                                                                       "
	   " 			   SELECT [WIDTH],[LENGTH]/1000.0,  SUM(QUANTITY) AS [NO OF LITES]                                         "
	   " 			   FROM [dbo].[CP_PARAM_DEFECT_EXPANDED_VIEW]                                                              "
	   " 			   WHERE [DATA]  between  @STARTDATE and @STOPDATE                                                         "
	   " 							 AND DEFECTID <> 14                                                                        "
	   " 							 AND ID_PRODTYPE IN (SELECT ID_PRODTYPE FROM [dbo].[PartToProdTypeViewCategorized] WHERE [ID] = 1)        "
	   " 												                                                        "
	   " 			   GROUP BY [Length], [WIDTH]                                                                              "
	   " 			 ) AS TABELKA                                                                                              "
	   " 		 GROUP BY [LENGTH], [WIDTH]                                                                                    "
	   " 		 ORDER BY [LENGTH] DESC                                                                                        " ;
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("StartDate")->Value = StartTime();// "#08/01/2011 12:01:00 AM#";
	   Query->Parameters->ParamByName("StopDate")->Value = StopTime(); // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;
	   NoRec = Query->RecordCount;
	   if (NoRec==0) { //No Sunguards
          return;
	   }
	   FormMain->StringGridReport->RowCount = FormMain->StringGridReport->RowCount + NoRec+10;
	   In=Bed=0;

	   while(!Query->Eof)
	 //  for(int j=0;j<=NoRec;j++, i++)               //  , round(sum(Rozm),0) Rozmiar
	   {

		  Length = Query->FieldByName("Lite Length")->AsFloat;
		  TempAnsi= Length;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  LiteWidth =  Query->FieldByName("Lite Width")->AsFloat;
		  TempAnsi= LiteWidth ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ;

		  Quantity = Query->FieldByName("NO OF LITES")->AsInteger;
		  TempAnsi=   Quantity  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  Sheet->Cells[2][i] = TempAnsi;

		  LengthWidthToQuantitySG[std::pair<double,double>(Length,LiteWidth)]=Quantity;

		  In += Length*LiteWidth*Quantity;

		  TempAnsi=   Quantity*LiteWidth*Length  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  Sheet->Cells[3][i] = TempAnsi;



		  if (Length<3) {
			 Quantity=Quantity/2 ;
		  }
		  Bed += (int)Quantity*19.26;






		  TempAnsi=   Quantity*19.26  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];

		  Sheet->Cells[4][i] = TempAnsi;

		  TempAnsi=   (LiteWidth*Length*100.0)/19.26  ;
		  if (Length<3) {
			 TempAnsi=   (2*LiteWidth*Length*100.0)/19.26  ;
		  }
		  TempAnsi += " %" ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

		  Sheet->Cells[5][i] = TempAnsi;
		  i++;
		  Query->Next();
	   }
	   Query->Close();
	   i++;
	   Sheet->Cells[0][i] = "In = ";
	   Sheet->Cells[1][i] = In;
	   i++;
	   Sheet->Cells[0][i] = "Bed = ";
	   Sheet->Cells[1][i] = Bed;
	   i++;
	   Sheet->Cells[0][i] = "BedUtil = ";
	   Sheet->Cells[1][i] = UnicodeString((In/Bed)*100);
	   i++;

	   FormMain->StringGridReport->RowCount = FormMain->StringGridReport->RowCount + NoRec+10;
	   In=Bed=0;
	   std::map <std::pair<double,double>,int>::iterator LengthWidthToQuantityIt;

		i+=2;

		Sheet->Cells[1][i] = "CLIMAGUARDS";
		i+=2;

		In=Bed=0;
	   for (LengthWidthToQuantityIt= LengthWidthToQuantityAll.begin(); LengthWidthToQuantityIt != LengthWidthToQuantityAll.end(); LengthWidthToQuantityIt++,i++)
	   {
		 if(LengthWidthToQuantitySG.count(LengthWidthToQuantityIt->first)==1 )
		 {
			LengthWidthToQuantityCG[LengthWidthToQuantityIt->first] = LengthWidthToQuantityIt->second -  LengthWidthToQuantitySG[LengthWidthToQuantityIt->first];
		 }
		 else
		 {
			LengthWidthToQuantityCG[LengthWidthToQuantityIt->first] = LengthWidthToQuantityIt->second;
		 }

		  Length = LengthWidthToQuantityIt->first.first ;
		  TempAnsi= Length    ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  LiteWidth =  LengthWidthToQuantityIt->first.second   ;
		  TempAnsi= LiteWidth   ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi;

		  Quantity= LengthWidthToQuantityCG[LengthWidthToQuantityIt->first];
		  TempAnsi=  Quantity  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		   //In += LengthWidthToQuantityIt->first.first*LengthWidthToQuantityIt->first.second *LengthWidthToQuantityCG[LengthWidthToQuantityIt->first];
			In += Length*LiteWidth*Quantity;

			if (Length<3) {
			 Quantity=Quantity/2 ;
		  }
		  Bed += (int)Quantity*19.26;


		  Sheet->Cells[2][i] = TempAnsi;

	   }

		 i++;
	   Sheet->Cells[0][i] = "In = ";
	   Sheet->Cells[1][i] = In;
	   i++;
	   Sheet->Cells[0][i] = "Bed = ";
	   Sheet->Cells[1][i] = Bed;
	   i++;
	   Sheet->Cells[0][i] = "BedUtil = ";
	   Sheet->Cells[1][i] = UnicodeString((In/Bed)*100);
	   i++;



		Width =  Sheet->Canvas->TextWidth("BedUtil = ");
		FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

	   for(int i=0;i<6;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }


} __finally {
  DataModule1->ADOConnectionCreport->Connected=false;
}




//    try{
//
//       DataModule1->ADOConnectionV2->Connected=true;
//       if(DataModule1->ADOConnectionV2->Connected==false)
//       {
//		   ShowMessage("Nie udało się połączyć z bazą V2, \nprawdopodobnie brak uprawnień");
//           return;
//	   }
//       TADOQuery* Query = DataModule1->ADOQueryBedUtilization;
//       TStringGrid* Sheet =  FormMain->StringGridReport;
//   //    ClearSheet(Sheet);
//       Query->Parameters->ParamByName("DateStart")->Value = StartTime();
//       Query->Parameters->ParamByName("DateStop")->Value = StopTime();
////       ShowMessage(Query->Parameters->ParamByName("DateStart")->Value);
////       ShowMessage(Query->Parameters->ParamByName("DateStop")->Value);
//       Query->Open();
//       int NoRec = Query->RecordCount;
//       FormMain->StringGridReport->RowCount=NoRec+3;
//	   Sheet->ColCount=7;
//	   double WidthL,LengthL,  Area;
//	   int No;
//	   double SumArea = 0;
//	   Sheet->Cells[0][0] = "Dimensions";
//	   Sheet->Cells[1][0] = "Lites No";
//	   Sheet->Cells[4][0] = "Width";
//	   Sheet->Cells[5][0] = "Lenth";
//	   Sheet->Cells[6][0] = "Area [m^2]";
//
//	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
//	   {
//		  Sheet->Cells[0][i] = Query->FieldByName("Dimensions")->AsString;
//		  No =  Query->FieldByName("BedSum")->AsInteger;
//		  Sheet->Cells[1][i] =  No;
//		  WidthL =  Query->FieldByName("WidthDim")->AsFloat/1000;
//		  LengthL=  Query->FieldByName("LengthDim")->AsFloat/1000;
//		  Area =    WidthL*LengthL*No;
//		  Sheet->Cells[4][i] = WidthL;
//		  Sheet->Cells[5][i] = LengthL;
//		  Sheet->Cells[6][i]= Area;
//
//		  SumArea += Area;
//		  //Sheet->Cells[3][i]= SumArea;
//		  Query->Next();
//	   }
//	   Sheet->Cells[5][NoRec+2] = "Sum [m^2]";
//	   Sheet->Cells[6][NoRec+2] = SumArea;
//	}
//    __finally{
//       DataModule1->ADOConnectionV2->Connected=false;
//    }
}

void Overall()
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage(L"Nie udało się połączyć z bazą Creport, \nprawdopodobnie brak uprawnień");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
   //    ClearSheet(Sheet);
//	   TDateTime Start, Stop;
	   AnsiString QueryString;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
////       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
////                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
////     Query->SQL->Add(QueryString);
////     Query->Parameters->ParamByName("DateR")->Value=DateR;
////     Query->Parameters->ParamByName("DateR1")->Value=DateR;
////     Query->Parameters->ParamByName("DateR2")->Value=DateR;
//
//
//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.
 SetDateShiftWise();
 QueryString =         // 5, 2 ID of Sevice and Pumping conditioning
			   " SELECT Workshiftdays.*, ShiftGroup.Name, QueryLineStatusRemarks.*, LineStatus_Type.LineStatus_Name "
			   " FROM ((QueryLineStatusRemarks  INNER JOIN LineStatus_Type                                          "
			   "         ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus)                   "
			   "      INNER JOIN Workshiftdays ON Workshiftdays.ID_Workshiftday = QueryLineStatusRemarks.ID_Workshiftday)  "
			   "      INNER JOIN SHIFTGROUP ON Workshiftdays.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP                      "
			   " WHERE                                                                                                     "
			   "  Date_Of_Work Between :StartDate and :StopDate                                                            "
			   " AND                                                                                                       "
			  // " LineStatus_Name LIKE :LineStatus_Name                                                                     "
			   " (QueryLineStatusRemarks.ID_LineStatus = 5 OR  QueryLineStatusRemarks.ID_LineStatus = 2)                   "
			   " ORDER BY Date_Of_Work ASC                                                                                 "
			   ;
	//   ExchangeAnsStr(QueryString,":StartDate","#08/01/2011 12:01:00 AM#");
   //    ExchangeAnsStr(QueryString,":StopDate","#08/31/2011 9:59:59 PM#");
   //    ExchangeAnsStr(QueryString,":LineStatus_Name","\'*"+Type+"*\'");
	   Query->SQL->Clear();
	  // Query->ParamCheck=false;
	   Query->SQL->Add(QueryString);
 //      FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
//       01/08/2011 12:01:00 AM
//31/08/2011 9:59:59 PM
//'*Develop*'
	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
 //	  Query->Parameters->ParamByName("ID_LineStatus")->Value = ID_LineStatus; //"\'*"+Type+"*\'";
//       ShowMessage(Query->Parameters->ParamByName("DateStart")->Value);
//       ShowMessage(Query->Parameters->ParamByName("DateStop")->Value);
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParseSQL(Query->SQL->GetText(), false));
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("StartDate")->Value);// = Start ;
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("StopDate")->Value);// =  Stop ;
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("LineStatus_Name")->Value);// = "\'%"+Type+"%\'";

	   Query->Open();
	   Query->ParamCheck=true;
	//   FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParseSQL()

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+1;
	   Sheet->ColCount=8;
	   AnsiString TempAnsi;
	   int FieldWidth[7] ={0,0,0,0,0,0,0};
	   int Width;
	   std::map<int,int>::iterator pos;
	   Sheet->Cells[0][0] = "Date";
	   Sheet->Cells[1][0] = "Shift";
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  Start = Query->FieldByName("Start_Time")->AsDateTime;
		  Stop  = Query->FieldByName("End_Time")->AsDateTime;

		  TempAnsi= TDateTime(Query->FieldByName("Date_Of_Work")->AsDateTime).DateString();
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  TempAnsi= Query->FieldByName("Name")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter

		  TempAnsi= FormMain->ComboBoxShiftKind->Items->Strings[Query->FieldByName("ID_ShiftTime")->AsInteger -1 ];
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  Sheet->Cells[2][i] = TempAnsi;


		  TempAnsi= Start.FormatString("hh:nn");
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  Sheet->Cells[3][i]= TempAnsi;   //TimeString();

		  TempAnsi= Stop.FormatString("hh:nn");
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];

		  Sheet->Cells[4][i]= TempAnsi;//TimeString();

		  TempAnsi= MinuteDiff(Start,Stop);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

		  Sheet->Cells[5][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("Remarks")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];

		  Sheet->Cells[6][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("LineStatus_Name")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];

		  Sheet->Cells[7][i]= TempAnsi;
		  Query->Next();
	   }
	   Query->Close();
	   for(int i=0;i<7;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }
	}
	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}
}



void LineStatusInfo(std::list<int> Statuses)
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage(L"Nie udało się połączyć z bazą Creport, \nprawdopodobnie brak uprawnień");
		   return;
	   }
	   UnicodeString InClause = " (";
	   std::list<int>::iterator it=Statuses.begin();
	   if (it==Statuses.end()) {
		   return;
	   }
	   InClause += *it;
	   it++ ;
	   while (it!=Statuses.end())
	   {
		 InClause +=", ";
		 InClause += *it;
		 it++ ;
	   }
	   InClause += ") ";

	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	   AnsiString QueryString;
	   SetDateShiftWise();
	   QueryString =
			   " SELECT Workshiftdays.*, ShiftGroup.Name, QueryLineStatusRemarks.*, LineStatus_Type.LineStatus_Name,UNITED.PRODTYPE_NAME "
			   " FROM ((QueryLineStatusRemarks  INNER JOIN LineStatus_Type                                          "
			   "         ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus)                   "
			   "      INNER JOIN Workshiftdays ON Workshiftdays.ID_Workshiftday = QueryLineStatusRemarks.ID_Workshiftday)  "
			   "      INNER JOIN SHIFTGROUP ON Workshiftdays.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP                      "
			   " INNER JOIN                                                                                                "
			   "		   (SELECT * FROM PRODTYPE                                                                         "
			   "			UNION                                                                                          "
			   "			SELECT * FROM NON_PRODTYPE)                                                                    "
			   "			AS UNITED  ON QueryLineStatusRemarks.ID_ProdType = UNITED.ID_PRODTYPE                          "
			   " WHERE                                                                                                     "
			   "  Date_Of_Work Between :StartDate and :StopDate                                                            "
			   " AND                                                                                                       "    ;
	   QueryString +=  " QueryLineStatusRemarks.ID_LineStatus IN " + InClause;

	   QueryString += " ORDER BY Date_Of_Work ASC                                                                          ";
			   ;
	//   ExchangeAnsStr(QueryString,":StartDate","#08/01/2011 12:01:00 AM#");
   //    ExchangeAnsStr(QueryString,":StopDate","#08/31/2011 9:59:59 PM#");
   //    ExchangeAnsStr(QueryString,":LineStatus_Name","\'*"+Type+"*\'");
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	  // Query->Parameters->ParamByName("ID_LineStatus")->Value = ID_LineStatus; //"\'*"+Type+"*\'";
	   Query->Open();
	   Query->ParamCheck=true;

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+1;
	   Sheet->ColCount=10;
	   AnsiString TempAnsi;
	   int FieldWidth[9] ={0,0,0,0,0,0,0,0,0};
	   int Width;
	   std::map<int,int>::iterator pos;
	   Sheet->Cells[0][0] = "Date";
	   Sheet->Cells[1][0] = "Shift";
	   Sheet->Cells[2][0] = "Shift Kind";
	   Sheet->Cells[3][0] = "Start Time";
	   Sheet->Cells[4][0] = "Stop Time";
	   Sheet->Cells[5][0] = "Duration [min]";
	   Sheet->Cells[6][0] = "Product";
	   Sheet->Cells[7][0] = "Additional";
	   Sheet->Cells[8][0] = "Remarks";
	   Sheet->Cells[9][0]= "Status" ;

	   for(int i=0;i<9;i++)
	   {
		 Width =  Sheet->Canvas->TextWidth(Sheet->Cells[i][0]);
		 FieldWidth[i]=Width>FieldWidth[i]?Width:FieldWidth[i];
	   }

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  Start = Query->FieldByName("Start_Time")->AsDateTime;
		  Stop  = Query->FieldByName("End_Time")->AsDateTime;

		  TempAnsi= TDateTime(Query->FieldByName("Date_Of_Work")->AsDateTime).DateString();
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  TempAnsi= Query->FieldByName("Name")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter

		  TempAnsi= FormMain->ComboBoxShiftKind->Items->Strings[Query->FieldByName("ID_ShiftTime")->AsInteger -1 ];
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  Sheet->Cells[2][i] = TempAnsi;


		  TempAnsi= Start.FormatString("hh:nn");
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  Sheet->Cells[3][i]= TempAnsi;   //TimeString();

		  TempAnsi= Stop.FormatString("hh:nn");
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];

		  Sheet->Cells[4][i]= TempAnsi;//TimeString();

		  TempAnsi= MinuteDiff(Start,Stop);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

		  Sheet->Cells[5][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("PRODTYPE_NAME")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];

		  Sheet->Cells[6][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("Additional")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];

		  Sheet->Cells[7][i]= TempAnsi;


		  TempAnsi= Query->FieldByName("Remarks")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[8]=Width>FieldWidth[8]?Width:FieldWidth[8];

		  Sheet->Cells[8][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("LineStatus_Name")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[9]=Width>FieldWidth[9]?Width:FieldWidth[9];

		  Sheet->Cells[9][i]= TempAnsi;
		  Query->Next();
	   }
	   Query->Close();
	   for(int i=0;i<9;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }
	}
	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}
}


void UnscheduledMainStatuses(int MStatus)
{
  try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   TADOQuery* Query = DataModule1->ADOQueryCreport;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage(L"Nie udało się połączyć z bazą Creport, \nprawdopodobnie brak uprawnień");
		   return;
	   }

	   std::list<int> MStatuses;
	   UnicodeString QueryString = 	" Select ID_LINESTATUS                 																								 "
									" FROM [LINESTATUS_TYPE] INNER JOIN [CategoriesLS] ON [LINESTATUS_TYPE].CategoryID = CategoriesLS.CategoryID                         "
									" Where CategoriesLS.MainID = :MainStatus                                                                                            "
									;
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("MainStatus")->Value = MStatus;// Main status for example: 6 is for Unscheduled;
	   Query->Open();
	   Query->ParamCheck=true;
	   int NoRec = Query->RecordCount;
	   for(int i=0;i< NoRec;i++)
	   {
		 MStatuses.push_back(Query->FieldByName("ID_LINESTATUS")->AsInteger);
		 Query->Next();
	   }
	   LineStatusInfo(MStatuses);
   }
	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}
}


void LineStatusInfo(int ID_LineStatus)
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage(L"Nie udało się połączyć z bazą Creport, \nprawdopodobnie brak uprawnień");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
   //    ClearSheet(Sheet);
//	   TDateTime Start, Stop;
	   AnsiString QueryString;
//    unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
////       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',DATE_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
////                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
////     Query->SQL->Add(QueryString);
////     Query->Parameters->ParamByName("DateR")->Value=DateR;
////     Query->Parameters->ParamByName("DateR1")->Value=DateR;
////     Query->Parameters->ParamByName("DateR2")->Value=DateR;
//
//
//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.
 SetDateShiftWise();
 QueryString =
			   " SELECT Workshiftdays.*, ShiftGroup.Name, QueryLineStatusRemarks.*, LineStatus_Type.LineStatus_Name,UNITED.PRODTYPE_NAME "
			   " FROM ((QueryLineStatusRemarks  INNER JOIN LineStatus_Type                                          "
			   "         ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus)                   "
			   "      INNER JOIN Workshiftdays ON Workshiftdays.ID_Workshiftday = QueryLineStatusRemarks.ID_Workshiftday)  "
			   "      INNER JOIN SHIFTGROUP ON Workshiftdays.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP                      "
			   " INNER JOIN                                                                                                "
			   "		   (SELECT * FROM PRODTYPE                                                                         "
			   "			UNION                                                                                          "
			   "			SELECT * FROM NON_PRODTYPE)                                                                    "
			   "			AS UNITED  ON QueryLineStatusRemarks.ID_ProdType = UNITED.ID_PRODTYPE                          "
			   " WHERE                                                                                                     "
			   "  Date_Of_Work Between :StartDate and :StopDate                                                            "
			   " AND                                                                                                       "
			  // " LineStatus_Name LIKE :LineStatus_Name                                                                     "
			   " QueryLineStatusRemarks.ID_LineStatus = :ID_LineStatus                                                     "
			   " ORDER BY Date_Of_Work ASC                                                                                 "
			   ;
	//   ExchangeAnsStr(QueryString,":StartDate","#08/01/2011 12:01:00 AM#");
   //    ExchangeAnsStr(QueryString,":StopDate","#08/31/2011 9:59:59 PM#");
   //    ExchangeAnsStr(QueryString,":LineStatus_Name","\'*"+Type+"*\'");
	   Query->SQL->Clear();
	  // Query->ParamCheck=false;
	   Query->SQL->Add(QueryString);
 //      FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
//       01/08/2011 12:01:00 AM
//31/08/2011 9:59:59 PM
//'*Develop*'
	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	  Query->Parameters->ParamByName("ID_LineStatus")->Value = ID_LineStatus; //"\'*"+Type+"*\'";
//       ShowMessage(Query->Parameters->ParamByName("DateStart")->Value);
//       ShowMessage(Query->Parameters->ParamByName("DateStop")->Value);
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParseSQL(Query->SQL->GetText(), false));
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("StartDate")->Value);// = Start ;
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("StopDate")->Value);// =  Stop ;
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("LineStatus_Name")->Value);// = "\'%"+Type+"%\'";

	   Query->Open();
	   Query->ParamCheck=true;
	//   FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParseSQL()

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+1;
	   Sheet->ColCount=10;
	   AnsiString TempAnsi;
	   int FieldWidth[9] ={0,0,0,0,0,0,0,0,0};
	   int Width;
	   std::map<int,int>::iterator pos;
	   Sheet->Cells[0][0] = "Date";
	   Sheet->Cells[1][0] = "Shift";
	   Sheet->Cells[2][0] = "Shift Kind";
	   Sheet->Cells[3][0] = "Start Time";
	   Sheet->Cells[4][0] = "Stop Time";
	   Sheet->Cells[5][0] = "Duration [min]";
	   Sheet->Cells[6][0] = "Product";
	   Sheet->Cells[7][0] = "Additional";
	   Sheet->Cells[8][0] = "Remarks";
	   Sheet->Cells[9][0]= "Status" ;

	   for(int i=0;i<9;i++)
	   {
		 Width =  Sheet->Canvas->TextWidth(Sheet->Cells[i][0]);
		 FieldWidth[i]=Width>FieldWidth[i]?Width:FieldWidth[i];
	   }

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  Start = Query->FieldByName("Start_Time")->AsDateTime;
		  Stop  = Query->FieldByName("End_Time")->AsDateTime;

		  TempAnsi= TDateTime(Query->FieldByName("Date_Of_Work")->AsDateTime).DateString();
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  TempAnsi= Query->FieldByName("Name")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter

		  TempAnsi= FormMain->ComboBoxShiftKind->Items->Strings[Query->FieldByName("ID_ShiftTime")->AsInteger -1 ];
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  Sheet->Cells[2][i] = TempAnsi;


		  TempAnsi= Start.FormatString("hh:nn");
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  Sheet->Cells[3][i]= TempAnsi;   //TimeString();

		  TempAnsi= Stop.FormatString("hh:nn");
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];

		  Sheet->Cells[4][i]= TempAnsi;//TimeString();

		  TempAnsi= MinuteDiff(Start,Stop);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

		  Sheet->Cells[5][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("PRODTYPE_NAME")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];

		  Sheet->Cells[6][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("Additional")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];

		  Sheet->Cells[7][i]= TempAnsi;


		  TempAnsi= Query->FieldByName("Remarks")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[8]=Width>FieldWidth[8]?Width:FieldWidth[8];

		  Sheet->Cells[8][i]= TempAnsi;

		  TempAnsi= Query->FieldByName("LineStatus_Name")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[9]=Width>FieldWidth[9]?Width:FieldWidth[9];

		  Sheet->Cells[9][i]= TempAnsi;
		  Query->Next();
	   }
	   Query->Close();
	   for(int i=0;i<9;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }
	}
	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}
}

void LineStatusQuery(int ShiftID=0)
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
//	   TDateTime Start, Stop;
	   AnsiString QueryString, QueryA1, QueryA2;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
	   Query->Close();
		Query->SQL->Clear();

//	   Start = StartTime() ;  //FormMain->DateTimePickerStartTime->DateTime;
//	   Stop = StopTime();  //FormMain->DateTimePickerMain->DateTime;
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.

	   SetDateShiftWise();
	   QueryA1 =" Select  * from QueryGather "
				" WHERE (ID_WORKSHIFTDAY = :ShiftID) "
				;
	   QueryA2=	" Select  * from QueryGather "
				" WHERE (DATE_OF_WORK Between :StartDate and :StopDate) "
				;
	   if (ShiftID==0) {
		 QueryString = QueryA2;
		 Query->SQL->Add(QueryString);
		 Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
		 Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   }
	   else
	   {
		 QueryString = QueryA1;
		 Query->SQL->Add(QueryString);
		 Query->Parameters->ParamByName("ShiftID")->Value = ShiftID;
	   }



	   Query->Open();
	   Query->ParamCheck=true;

	   Schedule.Clear();
	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+1;
	   AnsiString TempAnsi;
	   std::map<int,int>::iterator pos;
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  Schedule.Add(Query) ;
		   Query->Next();
	   }
	   Query->Close();
	   Schedule.ExportToReportDB();
	}
	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}

}
/*
switch (ComboBoxShiftKind->ItemIndex) {
	case 1: if (DateTimePickerMain->Time < TDateTime(14,0,0,0) || DateTimePickerMain->Time > TDateTime(22,0,0,0) ) {
				DateTimePickerMain->Time = TDateTime(14,1,0,0);

			}
			break;
	case 2: if (DateTimePickerMain->Time > TDateTime(6,0,0,0) ) {
				DateTimePickerMain->Time =TDateTime(0,1,0,0) ;
			}
			break;
	case 0:
			if (DateTimePickerMain->Time < TDateTime(6,0,0,0) || DateTimePickerMain->Time > TDateTime(14,0,0,0) ) {
				DateTimePickerMain->Time = TDateTime(6,1,0,0);
			}
			break;
	default:
			;
	}
*/

TOEE_Struct OEE(int ShiftID=0)
{
   try{
	   TOEE_Struct OEE_Struct;
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return OEE_Struct;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;

	   int AvailableTime;
	   AnsiString QueryString, QueryA1, QueryA2;
//	   TDateTime Start, Stop;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;

//	   Start = StartTime() ;  //FormMain->DateTimePickerStartTime->DateTime;
//	   Stop = StopTime();  //FormMain->DateTimePickerMain->DateTime;
//
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.

	  // SetDateShiftWise();
	   AvailableTime = MinutesBetween(Stop,Start)+1;
	   Query->Close();
	   Query->SQL->Clear();
	   QueryA1 = "SELECT [dbo].[PrevShiftID]( :ID ) As PreviousID ";
	   QueryString = QueryA1;
	   Query->SQL->Add(QueryString);
	   Query->Parameters->ParamByName("ID")->Value = ShiftID;
	   Query->Open();
	   Query->ParamCheck=true;
       int PrevID = Query->FieldByName("PreviousID")->AsInteger;


	   Query->Close();
	   Query->SQL->Clear();



	   QueryA1 =" Select  * from QueryGather "
				" WHERE (ID_WORKSHIFTDAY = :ShiftID) "
		//		" WHERE (ID_WORKSHIFTDAY IN ( :ShiftID, :PrevID ) "
				;
	   QueryA2=	" Select  * from QueryGather "
				" WHERE (DATE_OF_WORK Between :StartDate and :StopDate) "
				;
	   if (ShiftID==0) {
		 QueryString = QueryA2;
		 Query->SQL->Add(QueryString);
		 Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
		 Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
		 UnicodeString TempTime = Start.TimeString();
		 TempTime = TTime(0,0,0,0).TimeString();
		 if (Start.TimeString()==TTime(0,0,0,0).TimeString()) {
		   AvailableTime+=120; // 120 min because for nigth shift time is shifted forward by 2 hours
		   Start = IncHour(Start,-2);
		 }

	   }
	   else
	   {
		 QueryString = QueryA1;
		 Query->SQL->Add(QueryString);
		 Query->Parameters->ParamByName("ShiftID")->Value = ShiftID;
		 //Query->Parameters->ParamByName("PrevID")->Value = PrevID;
		 AvailableTime = 480;
	   }



	   Query->Open();
	   Query->ParamCheck=true;

	   Schedule.Clear();
	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+1;
	   AnsiString TempAnsi;
	   std::map<int,int>::iterator pos;         //Setup Schedule variable
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  Schedule.Add(Query) ;
		  Query->Next();
	   }
	   Query->Close();
	   OEE_Struct.Uptime =  Schedule.GetNoMin(1);  // 1 - means Uptime
	   int StandByTime =  Schedule.GetNoMin(8);    // 8 - means StandBy
	   StandByTime += Schedule.GetNoMin(6);  // 6 - means Development time
	   AvailableTime -= StandByTime;
	   double UptimeHour = OEE_Struct.Uptime/60;
	   OEE_Struct.Start=Start;
	   OEE_Struct.Stop=Stop;

	   if(AvailableTime!=0)
	   {
				OEE_Struct.Uptime = OEE_Struct.Uptime / AvailableTime;
	   }
	   else
	   {
				OEE_Struct.Uptime = 0;
	   }
	   TShiftProd Production;
	   ProductionSummary(Production, ShiftID);
	   if (Production.In!=0) {
			 OEE_Struct.Yield = Production.Out/Production.In;
	   }
	   else
	   {
			OEE_Struct.Yield =0;
	   }
	   //OEE_Struct.Speed = 11; // theoretical maximum line speed for ClimaGuard;
	   //double MaxInputPerHour = 2084;
	   double TheoreticalCGInput = OEE_Struct.MaxInputPerHour*UptimeHour;//3.31*AvailableTime*Speed*(6/6.1);
	   if (TheoreticalCGInput!=0) {
		  OEE_Struct.Performance = Production.InCorrected/TheoreticalCGInput;
	   }
	   else
	   {
		   OEE_Struct.Performance = 0;
	   }

	   OEE_Struct.OEE = OEE_Struct.Yield*OEE_Struct.Uptime*OEE_Struct.Performance*100;
	   return OEE_Struct;
	}
	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}

}

TOEE_Struct OEE(TDateTime Begin,TDateTime End)
{
  SetDateShiftWise(Begin,End);
  return OEE();
}

void SunGuardsTime()
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	  // ClearSheet(Sheet);
//	   TDateTime Start, Stop;
	   AnsiString QueryString, QueryA1, QueryA2;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
//       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
//                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
//     Query->SQL->Add(QueryString);
//     Query->Parameters->ParamByName("DateR")->Value=DateR;
//     Query->Parameters->ParamByName("DateR1")->Value=DateR;
//     Query->Parameters->ParamByName("DateR2")->Value=DateR;
	   Query->Close();
		Query->SQL->Clear();

//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.

	   SetDateShiftWise();
// QueryString =
//          "     SELECT WORKSHIFTDAYS.DATE_OF_WORK, PROD_SUMMARY.TOTAL_IN, PROD_SUMMARY.TOTAL_OUT, PRODTYPE.FULLNAME, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME)) AS Czas, SHIFTGROUP.NAME "
//          "     , QueryLineStatusRemarks.Remarks  "
//          "     FROM ((((PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE) INNER JOIN LINE_STATUS ON WORKSHIFTDAYS.ID_WORKSHIFTDAY = LINE_STATUS.ID_WORKSHIFTDAY) INNER JOIN LINESTATUS_TYPE ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS) INNER JOIN SHIFTGROUP ON WORKSHIFTDAYS.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP "
//          "		, QueryLineStatusRemarks "
//
//          "		WHERE (((PRODTYPE.FULLNAME) Like \"*SunGuard*\") AND (([WORKSHIFTDAYS]![DATE_OF_WORK]) Between :StartDate And :StopDate) AND ((LINESTATUS_TYPE.LINESTATUS_NAME) Like \"*Production*\"))      	"
//          "     AND (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS) "
//          "    ORDER BY WORKSHIFTDAYS.DATE_OF_WORK;                                                                             																								"
//               ;

	   QueryA1 =" SELECT WORKSHIFTDAYS.ID_WorkShiftDay, WORKSHIFTDAYS.DATE_OF_WORK,  PRODTYPE.FULLNAME as Name, PRODTYPE.PRODTYPE_NAME as ShortName,LINESTATUS_TYPE.LINESTATUS_NAME, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME , (IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME))) AS Czas             "
			 "  , QueryLineStatusRemarks.Remarks                                                                                                                                                                                                                                                                                                   "
			 " , ShiftGroup.Name as Shift "
			 "  FROM  ShiftGroup INNER JOIN ( (((( PRODTYPE  INNER JOIN Prod_Summary ON ProdType.ID_ProdType=Prod_Summary.ID_Prod_Type) INNER JOIN Workshiftdays ON WorkShiftDays.ID_WorkshiftDay = Prod_Summary.ID_WorkshiftDay) Inner JOIN LINE_STATUS                                                                                                                      "
			 "             ON LINE_STATUS.ID_Workshiftday = Workshiftdays.ID_WorkShiftDay) INNER JOIN LineStatus_Type  ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS)    "
			 " LEFT JOIN  QueryLineStatusRemarks ON  "
			 "  (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS)                                                             "
			 "  )  ON    ShiftGroup.ID_ShiftGroup = WorkShiftDays.ID_ShiftGroup                                                                                                                                                                                                                                                                                                                              "
			 "	WHERE (WORKSHIFTDAYS.DATE_OF_WORK Between :StartDate and :StopDate) "
   //          " AND (LINESTATUS_TYPE.LINESTATUS_NAME Like \'%cja%\')   "
   //         " AND ShiftGroup.ID_ShiftGroup = WorkShiftDays.ID_ShiftGroup  "
			 " ORDER BY WORKSHIFTDAYS.DATE_OF_WORK,PRODTYPE.FULLNAME, LINE_STATUS.START_TIME, Workshiftdays.ID_WorkShiftDay;					 "
			  ;
	   QueryA2=	" Select  * from QueryGather "
				" WHERE (DATE_OF_WORK Between :StartDate and :StopDate) "
				;
			 QueryString = QueryA2;



	   Query->SQL->Add(QueryString);
   //     FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParseSQL(Query->SQL->Text,true)) ;

	 Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";


/*            Dzia³aj¹ce pod accessem

  SELECT WORKSHIFTDAYS.ID_WorkShiftDay, WORKSHIFTDAYS.DATE_OF_WORK,  PRODTYPE.FULLNAME as Name, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, (IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME))) AS Czas
			   , QueryLineStatusRemarks.Remarks
			   FROM ( ((( PRODTYPE  INNER JOIN Prod_Summary ON ProdType.ID_ProdType=Prod_Summary.ID_Prod_Type) INNER JOIN Workshiftdays ON WorkShiftDays.ID_WorkshiftDay = Prod_Summary.ID_WorkshiftDay) Inner JOIN LINE_STATUS
						  ON LINE_STATUS.ID_Workshiftday = Workshiftdays.ID_WorkShiftDay) INNER JOIN LineStatus_Type  ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS) LEFT JOIN  QueryLineStatusRemarks    ON     (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS)

		   WHERE (((PRODTYPE.FULLNAME) Like "*SunGuard*") AND (([WORKSHIFTDAYS]![DATE_OF_WORK]) Between #08/01/2011# and #09/09/2011#) AND ((LINESTATUS_TYPE.LINESTATUS_NAME) Like "*Production*"))

			  ORDER BY WORKSHIFTDAYS.DATE_OF_WORK;


*/





	  //	  Query->Parameters->ParamByName("ID_LineStatus")->Value = ID_LineStatus; //"\'*"+Type+"*\'";
//       ShowMessage(Query->Parameters->ParamByName("DateStart")->Value);
//       ShowMessage(Query->Parameters->ParamByName("DateStop")->Value);
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParseSQL(Query->SQL->GetText(), false));
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("StartDate")->Value);// = Start ;
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("StopDate")->Value);// =  Stop ;
//        FormMain->MemoGeneralRemarks->Lines->Add(Query->Parameters->ParamByName("LineStatus_Name")->Value);// = "\'%"+Type+"%\'";

	   Query->Open();
	   Query->ParamCheck=true;

	   Schedule.Clear();
	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+1;
	   Sheet->ColCount=8;
	   AnsiString TempAnsi;
	   int FieldWidth[8] ={0,0,0,0,0,0,0,0};
	   int Width;
	   std::map<int,int>::iterator pos;
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  Schedule.Add(Query) ;
		   Query->Next();
	   }
	   Query->Close();
	   Schedule.PrintAll(Sheet) ;
	}
	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}

}


void ShowDefects()
{
	try{
	   DataModule1->ADOConnectionGIP->Connected=true;
	   if(DataModule1->ADOConnectionGIP->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryDefectsSummary;//   ADOQueryGIP;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	 //  ClearSheet(Sheet);
	  TDateTime Start, Stop;
	  AnsiString QueryString;
	  unsigned short year;
	  unsigned short month;
	  unsigned short day;
	  unsigned short hour;
	  unsigned short min;
	  unsigned short sec;
	  unsigned short msec;
	  Start =  StartTime() ;
	  Stop =   StopTime();
	  double Sum=0, TempInt, TempArea;
	  std::map<UnicodeString, int> DefectCategoryName;
	  std::map<int,double> CategoryAreas;
	  std::map<UnicodeString,int> DefectNames;
	  std::map<int, int>	DefectToCategory;
	  std::map<int, double> DefectAreas;
	  UnicodeString DefType;
	  int DefID;
	  DataModule1->ADOQueryDefCategoryNames->Open();
	  while(!DataModule1->ADOQueryDefCategoryNames->Eof)
	  {
		   DefID= DataModule1->ADOQueryDefCategoryNames->FieldByName("DEF_CAT_ID")->AsInteger;
		   DefectCategoryName[DataModule1->ADOQueryDefCategoryNames->FieldByName("DEF_CAT_NAME")->AsString]= DefID;
		   CategoryAreas[DefID]=0;
		   DataModule1->ADOQueryDefCategoryNames->Next();
	  }
	  DataModule1->ADOQueryDefCategoryNames->Close();
	  DataModule1->ADOQueryDefectsParam->Open();
	  while(!DataModule1->ADOQueryDefectsParam->Eof)
	  {
		   DefID = DataModule1->ADOQueryDefectsParam->FieldByName("ID")->AsInteger;
		   DefectNames[DataModule1->ADOQueryDefectsParam->FieldByName("VALUE")->AsString]= DefID;
		   DefectAreas[DefID]=0;
		   DataModule1->ADOQueryDefectsParam->Next();
	  }
	  DataModule1->ADOQueryDefectsParam->Close();
	  DataModule1->ADOQueryDefToCategory->Open();
	  while(!DataModule1->ADOQueryDefToCategory->Eof)
	  {
		   DefectToCategory[DataModule1->ADOQueryDefToCategory->FieldByName("DEFECT_ID")->AsInteger]=DataModule1->ADOQueryDefToCategory->FieldByName("DEFCAT_ID")->AsInteger;
		   DataModule1->ADOQueryDefToCategory->Next();
	  }
	  DataModule1->ADOQueryDefToCategory->Close();






// QueryString =
////"         SELECT   DEFECTS.Value AS DefectType, Sum((Quantity*CP_Param.Val1*CP_Param.Val2)/1000000) AS Area "
////" FROM (CP_Defect INNER JOIN CP_Param ON CP_Defect.SizeID = CP_Param.ID) INNER JOIN [SELECT * FROM CP_Param WHERE TYPE = 'DEFECT']. AS DEFECTS ON CP_DEFECT.DefectID = DEFECTS.ID "
////"  "
////" WHERE DateTime>= :StartDate And DateTime<= :StopDate  "
////" GROUP BY  DEFECTS.Value     "
//
////" Select SubQuery.*, CP_Param.Value AS DefectType From (SELECT  DEFECTS.ID,  Sum((Quantity*CP_Param.Val1*CP_Param.Val2)/1000000) AS Area  "
//" Select PARAM_DEFECT_VIEW.Value AS DefectType, SubQuery.Area From (SELECT * FROM PARAM_DEFECT_VIEW) AS DEFECTY LEFT JOIN  (SELECT  PARAM_DEFECT_VIEW.ID,  Sum((Quantity*PARAM_SIZE_VIEW.Val1*PARAM_SIZE_VIEW.Val2)/1000000) AS Area "
////" FROM (CP_Defect INNER JOIN CP_Param ON CP_Defect.SizeID = CP_Param.ID) INNER JOIN [SELECT * FROM CP_Param WHERE TYPE = 'DEFECT']. AS DEFECTS ON CP_DEFECT.DefectID = DEFECTS.ID "
//" FROM (CP_Defect INNER JOIN PARAM_SIZE_VIEW ON CP_Defect.SizeID = PARAM_SIZE_VIEW.ID) INNER JOIN PARAM_DEFECT_VIEW  ON CP_DEFECT.DefectID = PARAM_DEFECT_VIEW.ID "
//"                                                                                                                                                                                 "
//" WHERE DateTime>= :StartDate And DateTime<= :StopDate                                                                                                                      "
//" GROUP BY  DEFECTS.ID                                                                                                                                                            "
//" ) As SubQuery ON SubQuery.ID = PARAM_DEFECT_VIEW.ID                                                                                                                                                        "
////" Where SubQuery.ID = CP_Param.ID                                                                                                                                                 "
//" Order by DefectY.ID                                                                                                                                                            "
//			   ;


//
//  " Select DEFECTY.Value AS DefectType, SubQuery.Area "
//  " FROM (SELECT * FROM PARAM_DEFECT_VIEW) AS DEFECTY LEFT JOIN                                                                                                         "
//  "	  (SELECT  PARAM_DEFECT_VIEW.ID,  Sum((Quantity*PARAM_SIZE_VIEW.Val1*PARAM_SIZE_VIEW.Val2)/1000000.0) AS Area                                                       "
//  "		FROM (CP_Defect INNER JOIN PARAM_SIZE_VIEW ON CP_Defect.SizeID = PARAM_SIZE_VIEW.ID) INNER JOIN PARAM_DEFECT_VIEW  ON CP_DEFECT.DefectID = PARAM_DEFECT_VIEW.ID "
//  "		WHERE DateTime>= :StartDate And DateTime<= :StopDate "
//  "		GROUP BY  PARAM_DEFECT_VIEW.ID          "
//  "	  ) As SubQuery ON SubQuery.ID = DEFECTY.ID "
//  " Order by DefectY.ID "
//	   ;
	  // Query->SQL->Clear();
	  // Query->SQL->Add(QueryString);
  //     FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
	   Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;
	   int NoRec = Query->RecordCount;

	   Sheet->ColCount=2;
	   AnsiString TempAnsi;
	   int FieldWidth[2] ={0,0};
	   int Width;
	   Sheet->Cells[0][0]="Defect ";
	   Sheet->Cells[1][0]="Area ";

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  DefType = Query->FieldByName("DefectType")->AsString;
		  TempArea = Query->FieldByName("Area")->AsFloat;

		  if (Options.GetRestrictedLiteArea()) {
			TempArea = Query->FieldByName("RoundedArea")->AsFloat;
		  }


		  DefID = Query->FieldByName("DefectID")->AsInteger;
		  DefectAreas[DefID] += TempArea;
		  CategoryAreas[ DefectToCategory[DefID] ] +=TempArea;
		  Sum+=TempArea;
		  Query->Next();
	   }
	   Query->Close();
	   std::map<UnicodeString,int>::iterator it;
	   int i=1;
	   for(it=DefectNames.begin();it!=DefectNames.end();it++,i++)
	   {

		  TempAnsi= it->first;  //Query->FieldByName("DefectType")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  TempInt = DefectAreas[it->second];//Query->FieldByName("Area")->AsFloat;
		  TempAnsi= AnsiString().FormatFloat("#.00", TempInt);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter

	   }
	   Sheet->Cells[0][i+2]="Total ";
	   Sheet->Cells[1][i+2]=AnsiString().FormatFloat("#.00",Sum);
	   i=i+5;
	   int DefectCatID;
	   for(it=DefectCategoryName.begin();it!=DefectCategoryName.end();it++,i++)
	   {
		  DefectCatID = it->second;
		  TempAnsi= it->first;  //Query->FieldByName("DefectType")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  TempInt = CategoryAreas[it->second];//Query->FieldByName("Area")->AsFloat;
		  TempAnsi= AnsiString().FormatFloat("#.00", TempInt);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter

		  switch (DefectCatID) {
				case 1: KPIReport.Reject.Development=CategoryAreas[DefectCatID];
						break;
				case 2: KPIReport.Reject.Equipment=CategoryAreas[DefectCatID];
						break;
				case 3: KPIReport.Reject.InputQuality =CategoryAreas[DefectCatID];
										break;
				case 4: KPIReport.Reject.ProcessQuality  =CategoryAreas[DefectCatID];
										break;
				case 5: KPIReport.Reject.QCBurnIns  =CategoryAreas[DefectCatID];
										break;
		  default:
		   ;
		  }
	   }

//	   Width =  Sheet->Canvas->TextWidth(Sheet->Cells[1][NoRec+2].c_str());
//       FieldWidth[1] = Width>FieldWidth[1]?Width:FieldWidth[1];

	   Sheet->Cells[0][i+4]="Report defect from "+(Start+1).DateString()+" till "+Stop.DateString();
       FormMain->StringGridReport->RowCount = i+5;
       for(int i=0;i<2;i++)
       {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }





    }
    __finally{
       DataModule1->ADOConnectionGIP->Connected=false;
    }

}



void ProductInfo()
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
       {
           ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	 //  ClearSheet(Sheet);
//	   TDateTime Start, Stop;
	   AnsiString QueryString;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
//       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
//                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
//     Query->SQL->Add(QueryString);
//     Query->Parameters->ParamByName("DateR")->Value=DateR;
//     Query->Parameters->ParamByName("DateR1")->Value=DateR;
//     Query->Parameters->ParamByName("DateR2")->Value=DateR;


//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.

	 SetDateShiftWise();
	QueryString =

			  " SELECT ProdType.ProdType_Name,ID_PROD_TYPE, SUM( PROD_SUMMARY.TOTAL_IN)as ProdIn, SUM(PROD_SUMMARY.TOTAL_OUT) as ProdOut           "
			  "	FROM (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE "
		 //     " WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) Between :StartDate And :StopDate "
			  "	WHERE WORKSHIFTDAYS.DATE_OF_WORK Between :StartDate And :StopDate   "
			  "	GROUP BY ProdType_Name,ID_PROD_TYPE                                               "
			   ;
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
  //     FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+3;
	   Sheet->ColCount=4;
	   AnsiString TempAnsi;
	   int FieldWidth[7] ={0,0,0,0,0,0,0};
	   int Width;
	   float ProdIn, ProdOut, Yield, SumIn=0, SumOut=0;
	   std::map<int,int>::iterator pos;
	   int i=1;
	   for(;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {


		  TempAnsi= Query->FieldByName("ProdType_Name")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  ProdIn =  Query->FieldByName("ProdIn")->AsFloat ;
		  SumIn += ProdIn;
		  TempAnsi= AnsiString().FormatFloat("0.##",ProdIn);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter

		  ProdOut  =  Query->FieldByName("ProdOut")->AsFloat;
		  SumOut+=ProdOut;
		  TempAnsi= AnsiString().FormatFloat("0.##",ProdOut) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  Sheet->Cells[2][i] = TempAnsi;
		  if (ProdIn==0) {
			Yield = 0;
		  }
		  else
		  {
			Yield = (ProdOut/ProdIn) * 100;
		  }
		  TempAnsi= AnsiString().FormatFloat("0.##",Yield);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  Sheet->Cells[3][i]= TempAnsi;   //TimeString();


		  Query->Next();
	   }
	   Query->Close();

	   for(int i=0;i<4;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }
	Sheet->Cells[0][0]= "Product";
	Sheet->Cells[1][0]= " In ";
	Sheet->Cells[2][0]= " Out ";
	Sheet->Cells[3][0]= " Yield ";


	Sheet->Cells[0][Sheet->RowCount-1]="Total ";
	Sheet->Cells[1][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",SumIn);
	Sheet->Cells[2][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",SumOut);
	Yield = SumIn!=0? (SumOut/SumIn)*100: 0 ;
	Sheet->Cells[3][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",Yield);

	i= Sheet->RowCount+2;

   QueryString =

   "	 Select  ID,Category, SUM( ProdIn)as ProdIn, SUM(ProdOut) as ProdOut                                                                                                                                                       "
   "                                                                                                                                                                                                                               "
   "	 FROM                                                                                                                                                                                                                      "
   "		 [dbo].[PartToProdTypeViewCategorized]  INNER JOIN                                                                                                                                                                     "
   "			   (SELECT ProdType.ProdType_Name,ID_PROD_TYPE, SUM( PROD_SUMMARY.TOTAL_IN)as ProdIn, SUM(PROD_SUMMARY.TOTAL_OUT) as ProdOut                                                                                       "
   "					FROM (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE                       "
   "                                                                                                                                                                                                                               "
   "					WHERE WORKSHIFTDAYS.DATE_OF_WORK Between :StartDate And :StopDate                                                                                                                                          "
   "					GROUP BY ProdType_Name,ID_PROD_TYPE)  Products ON     [ID_PRODTYPE] = Products.ID_PROD_TYPE                                                                                                                "
   "		GROUP BY ID, Category                                                                                                                                                                                                  "
   "		ORDER BY ID                                                                                                                                                                                                            ";


	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
  //     FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;

		NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=FormMain->StringGridReport->RowCount + NoRec+3;

	   int ProdCategoryID;
	   SumIn=0, SumOut=0;
	   double SummaryOutput = 0;
	   while(!Query->Eof)               //  , round(sum(Rozm),0) Rozmiar
	   {


		  ProdCategoryID = Query->FieldByName("ID")->AsInteger;







		  TempAnsi= Query->FieldByName("Category")->AsString;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

		  Sheet->Cells[0][i] = TempAnsi;

		  ProdIn =  Query->FieldByName("ProdIn")->AsFloat ;
		  //SumIn += ProdIn;
		  TempAnsi= AnsiString().FormatFloat("0.##",ProdIn);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

		  Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter

		  ProdOut  =  Query->FieldByName("ProdOut")->AsFloat;
		  //SumOut+=ProdOut;
		  TempAnsi= AnsiString().FormatFloat("0.##",ProdOut) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  Sheet->Cells[2][i] = TempAnsi;
		  if (ProdIn==0) {
			Yield = 0;
		  }
		  else
		  {
			Yield = (ProdOut/ProdIn) * 100;
		  }
		  TempAnsi= AnsiString().FormatFloat("0.##",Yield);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  Sheet->Cells[3][i]= TempAnsi;   //TimeString();



		  switch(ProdCategoryID)
		  {
		   case 6:
					KPIReport.Input.CGA = ProdIn;
					KPIReport.Output.CGA = ProdOut;
					SummaryOutput +=  ProdOut;
					break;
		   case 5:  KPIReport.Input.CGHT = ProdIn;
					KPIReport.Output.CGHT = ProdOut;
					SummaryOutput +=  ProdOut;
					break;
		   case 4:  KPIReport.Input.SGA = ProdIn;
					KPIReport.Output.SGA = ProdOut;
					SummaryOutput +=  ProdOut;
					break;

		   case 3:  KPIReport.Input.SGHT = ProdIn;
					KPIReport.Output.SGHT = ProdOut;
					SummaryOutput +=  ProdOut;
					break;
		   default:
					break;
		  }



		  i++;
		  Query->Next();

	   }
	   KPIReport.ProductionMix.Commercial = ( (KPIReport.Output.SGA + KPIReport.Output.SGHT)/SummaryOutput)*100 ;
	   //( (KPIReport.Output.SGA + KPIReport.Output.SGHT)/Output)*100;
	   KPIReport.ProductionMix.Residential = ( (KPIReport.Output.CGA + 	KPIReport.Output.CGHT)/SummaryOutput)*100;
	   Query->Close();
	   for(int i=0;i<4;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }
	}


	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}
}

void ProductionSummary(TShiftProd& Prod, int ShiftID=0)
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
//	   TDateTime Start, Stop;
	   UnicodeString QueryString, Query2String;
//	   unsigned short year;
//	   unsigned short month;
//	   unsigned short day;
//	   unsigned short hour;
//	   unsigned short min;
//	   unsigned short sec;
//	   unsigned short msec;
//       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
//                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
//     Query->SQL->Add(QueryString);
//     Query->Parameters->ParamByName("DateR")->Value=DateR;
//     Query->Parameters->ParamByName("DateR1")->Value=DateR;
//     Query->Parameters->ParamByName("DateR2")->Value=DateR;



//    QueryString =
//
//			  " SELECT ProdType.ProdType_Name, SUM( PROD_SUMMARY.TOTAL_IN)as ProdIn, SUM(PROD_SUMMARY.TOTAL_OUT) as ProdOut           "
//			  "	FROM (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE "
//		 //     " WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) Between :StartDate And :StopDate "
//			  "	WHERE WORKSHIFTDAYS.DATE_OF_WORK Between :StartDate And :StopDate   "
//			  "	GROUP BY ProdType_Name                                               "
//               ;
	   QueryString =
			  " SELECT SUM( PROD_SUMMARY.TOTAL_IN)as ProdIn, SUM(PROD_SUMMARY.TOTAL_OUT) as ProdOut           					"
			  "	FROM (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) 	"  ;
	   Query2String =
				   "  SELECT SUM(CAST(Width AS float)*CAST([Length] as float) *Quantity/1000000.0) AS Area, "
				   "         SUM(ROUND(CAST(Width AS float)*CAST([Length] as float) *Quantity/1000000.0,2)) AS RoundedArea "
				   "  FROM [276CoaterReports].[dbo].[CP_PARAM_DEFECT_EXPANDED_VIEW]                        "
				   "  WHERE DEFECTID= :DEF                                                   "   ;

	   Query->SQL->Clear();

	   int Def = 14; // Def - Defect ID

	   if(ShiftID==0)
	   {
//		   Start =  StartTime() ;
//		   Stop =   StopTime();
//		   Start.DecodeDate(&year, &month, &day);
//		   Start.DecodeTime(&hour, &min, &sec, &msec);
//		   if (hour >= 22) {
//			   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//		   }
//		   Stop.DecodeDate(&year, &month, &day);
//		   Stop.DecodeTime(&hour, &min, &sec, &msec);
//		   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.

		 //  SetDateShiftWise();
		   QueryString +=  " WHERE WORKSHIFTDAYS.DATE_OF_WORK Between :StartDate And :StopDate   ";
		   Query->SQL->Add(QueryString);
		   Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
		   Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   }
	   else
	   {
		   QueryString += " WHERE WORKSHIFTDAYS.ID_WORKSHIFTDAY = :ShiftID   " ;
		   Query->SQL->Add(QueryString);
		   Query->Parameters->ParamByName("ShiftID")->Value = ShiftID;
	   }

	   Query->Open();
	   Query->ParamCheck=true;

	   int NoRec = Query->RecordCount;
	   if (NoRec==0) { // No results
		 return;
	   }
	   Prod.In = Query->FieldByName("ProdIn")->AsFloat;
	   Prod.Out = Query->FieldByName("ProdOut")->AsFloat;
//*********************************************************************************************
//*** In order to calculate corrected input we have to add to the input the area used for development
//***********************************************************************************************
	   Query->SQL->Clear();
	   if(ShiftID==0)
	   {
		   Query2String +=   "  AND  DATA BETWEEN :START AND :STOP  ";
		   Query->SQL->Add(Query2String);
		   Query->Parameters->ParamByName("Start")->Value = Start;// "#08/01/2011 12:01:00 AM#";
		   Query->Parameters->ParamByName("Stop")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   }
	   else
	   {
		   Query2String += " AND ID_WORKSHIFTDAY = :ShiftID   " ;
		   Query->SQL->Add(Query2String);
		   Query->Parameters->ParamByName("ShiftID")->Value = ShiftID;
	   }
        Query->Parameters->ParamByName("DEF")->Value = Def;
	   Query->Open();
	   Query->ParamCheck=true;

		NoRec = Query->RecordCount;
	   if (NoRec==0) { // No results
		 return;
	   }
	   Prod.InCorrected = Prod.In + Query->FieldByName("Area")->AsFloat;



	}


	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	}
}

void LineStatusSummary()
{
	try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie udało się połączyć z bazą Creport, \nprawdopodobnie brak uprawnień");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	 //  ClearSheet(Sheet);
//	   TDateTime Start, Stop;
	   AnsiString QueryString;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
////       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
////                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
////     Query->SQL->Add(QueryString);
////     Query->Parameters->ParamByName("DateR")->Value=DateR;
////     Query->Parameters->ParamByName("DateR1")->Value=DateR;
////     Query->Parameters->ParamByName("DateR2")->Value=DateR;
//
//
//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.

	SetDateShiftWise();
	QueryString =

  "  SELECT WORKSHIFTDAYS.DATE_OF_WORK, LineStatus_Type.LINESTATUS_NAME, LineStatus_Type.ID_LINESTATUS, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME,"
  "         IIf(LINE_STATUS.START_TIME>LINE_STATUS.END_TIME,24*60-DateDiff(minute,LINE_STATUS.END_TIME,LINE_STATUS.START_TIME),DateDiff(minute,LINE_STATUS.START_TIME,LINE_STATUS.END_TIME)) AS Czas, "
  "         WORKSHIFTDAYS.ID_SHIFTGROUP, CategoriesLS.Main, CategoriesLS.Auxiliary "
  "  FROM SHIFTTIME INNER JOIN (((LineStatus_Type INNER JOIN LINE_STATUS ON LineStatus_Type.ID_LINESTATUS = LINE_STATUS.ID_LINESTATUS)  "
  "                 INNER JOIN CategoriesLS ON LineStatus_Type.CategoryID = CategoriesLS.CategoryID) "
  "                 INNER JOIN WORKSHIFTDAYS ON LINE_STATUS.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTTIME.ID_SHIFTTIME = WORKSHIFTDAYS.ID_SHIFTTIME "
  "  WHERE (((WORKSHIFTDAYS.DATE_OF_WORK) Between :StartDate And :StopDate)) "
  "  ORDER BY WORKSHIFTDAYS.DATE_OF_WORK "
			   ;
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
  //     FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
      Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
       Query->Open();
       Query->ParamCheck=true;

       int NoRec = Query->RecordCount;
       FormMain->StringGridReport->RowCount=NoRec+3;
       Sheet->ColCount=9;
       AnsiString TempAnsi;
       int FieldWidth[9] ={0,0,0,0,0,0,0,0,0};
       int Width, LineStatus;
       float ProdIn, ProdOut, Yield, SumIn=0, SumOut=0;
       char Shifts[4]={'A','B','C','D'}  ;
       std::map<int,int>::iterator pos;
       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
       {


		  TempAnsi= Query->FieldByName("DATE_OF_WORK")->AsString;//AsDateTime;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

          Sheet->Cells[0][i] = TempAnsi;

          TempAnsi =  Query->FieldByName("LINESTATUS_NAME")->AsString ;
         // SumIn += ProdIn;
         // TempAnsi= AnsiString().FormatFloat("0.##",ProdIn);
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

          Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter


          LineStatus  =  Query->FieldByName("ID_LINESTATUS")->AsInteger;
//          SumOut+=ProdOut;
//          TempAnsi= AnsiString().FormatFloat("0.##",ProdOut) ;
          Width =  Sheet->Canvas->TextWidth(TempAnsi=AnsiString(LineStatus));
          FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

          Sheet->Cells[2][i] = TempAnsi;

		  TempAnsi= Query->FieldByName("START_TIME")->AsString; //DateTime;
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

          Sheet->Cells[3][i]= TempAnsi;   //TimeString();


		  TempAnsi= Query->FieldByName("END_TIME")->AsString;//DateTime;
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];

          Sheet->Cells[4][i]= TempAnsi;   //TimeString();

          TempAnsi= Query->FieldByName("Czas")->AsInteger;
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

          Sheet->Cells[5][i]= TempAnsi;   //TimeString();


             //  					Main	Auxiliary



          TempAnsi= Shifts[Query->FieldByName("ID_SHIFTGROUP")->AsInteger - 1 ];
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];

          Sheet->Cells[6][i]= TempAnsi;   //TimeString();

          TempAnsi= Query->FieldByName("Main")->AsString;
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];

          Sheet->Cells[7][i]= TempAnsi;   //TimeString();

          TempAnsi= Query->FieldByName("Auxiliary")->AsString;
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[8]=Width>FieldWidth[8]?Width:FieldWidth[8];

          Sheet->Cells[8][i]= TempAnsi;   //TimeString();

          Query->Next();
       }
       Query->Close();

       for(int i=0;i<Sheet->ColCount;i++)
       {
           Sheet->ColWidths[i]=FieldWidth[i]+6;
       }
    Sheet->Cells[0][0]= "Data";
    Sheet->Cells[1][0]= " Line Status ";
    Sheet->Cells[2][0]= " ID ";
    Sheet->Cells[3][0]= " Start Time ";
    Sheet->Cells[4][0]= " Stop Time ";
    Sheet->Cells[5][0]= "  Time ";
    Sheet->Cells[6][0]= " Shift ";
    Sheet->Cells[7][0]= " Main Status ";
    Sheet->Cells[8][0]= " Auxiliary Status ";
  //  Sheet->Cells[9][0]= " Start Time ";


//    Sheet->Cells[0][Sheet->RowCount-1]="Total ";
//    Sheet->Cells[1][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",SumIn);
//    Sheet->Cells[2][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",SumOut);
//    Yield = SumIn!=0? (SumOut/SumIn)*100: 0 ;
//    Sheet->Cells[3][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",Yield);
    }


    __finally{
       DataModule1->ADOConnectionCreport->Connected=false;
    }

}

int MaxDaysMonth(int Month,int Year)
{
    switch (Month) {
		case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
            	return 31;
            	break;
        case 2: if(IsLeapYear(Year))
                	return 29;
            	else
                	return 28;
                break;
    default:
             return 30;
             break;
        ;
    }
}

void GenerateCOC_Time()
{
    try{
       DataModule1->ADOConnectionCreport->Connected=true;
       if(DataModule1->ADOConnectionCreport->Connected==false)
       {
           ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
           return;
       }
       TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
       TStringGrid* Sheet =  FormMain->StringGridReport;
     //  ClearSheet(Sheet);

	   AnsiString QueryString;
//	  TDateTime Start, Stop;
//	  unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
////       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
////                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
////     Query->SQL->Add(QueryString);
////     Query->Parameters->ParamByName("DateR")->Value=DateR;
////     Query->Parameters->ParamByName("DateR1")->Value=DateR;
////     Query->Parameters->ParamByName("DateR2")->Value=DateR;
//
//
//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); 	//reporting is shiftwise therfore we need to change
																						//start and stop date.
	   SetDateShiftWise();
       unsigned short year = YearOf(Start);
	   int MaxDays =  MonthDays[IsLeapYear(year)][2];
	   int StartingRow = 7;
	   int StartingCol = 15;
	   int CurrMonth = MonthOf(Start);
	   int CurrYear = year;

       std::set<int> Main;  // gathering all unique Main categories
     QueryString =
      " SELECT * "
      " FROM CategoriesLS ORDER BY CategoryID ASC ";

       Query->SQL->Clear();
       Query->SQL->Add(QueryString);
       Query->Open();
       int NoCategories = Query->RecordCount;
       AnsiString* CategoriesName = new AnsiString[NoCategories];

       for (int i = 0; i < NoCategories; i++) {
           CategoriesName[i] = Query->FieldByName("Auxiliary")->AsString;
          Main.insert(Query->FieldByName("MainID")->AsInteger); // gathering every new category.
           Query->Next();
       }
      // Query->Close();



    QueryString =
      " SELECT LineStatus_Type.*, CategoriesLS.Main, CategoriesLS.Auxiliary, CategoriesLS.MainID "
      " FROM LineStatus_Type INNER JOIN CategoriesLS ON LineStatus_Type.CategoryID = CategoriesLS.CategoryID "
      " ORDER BY  LineStatus_Type.CategoryID ASC "
      ;




//  "  SELECT WORKSHIFTDAYS.DATE_OF_WORK, LineStatus_Category.LINESTATUS_NAME, LineStatus_Category.ID_LINESTATUS, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME)) AS Czas, WORKSHIFTDAYS.ID_SHIFTGROUP, CategoriesLS.Main, CategoriesLS.Auxiliary "
//  "  FROM SHIFTTIME INNER JOIN (((LineStatus_Category INNER JOIN LINE_STATUS ON LineStatus_Category.ID_LINESTATUS = LINE_STATUS.ID_LINESTATUS) INNER JOIN CategoriesLS ON LineStatus_Category.CategoryID = CategoriesLS.CategoryID) INNER JOIN WORKSHIFTDAYS ON LINE_STATUS.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTTIME.ID_SHIFTTIME = WORKSHIFTDAYS.ID_SHIFTTIME "
//
//  "  WHERE DatePart('m',WorkshiftDays.Date_OF_WORK) = CurrMonth and DatePart('yyyy', WorkshiftDays.Date_OF_WORK) = CurrYear "
//  "  ORDER BY WORKSHIFTDAYS.DATE_OF_WORK "
//               ;
       Query->SQL->Clear();
       Query->SQL->Add(QueryString);
   //    FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
   //   Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
   //   Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
       Query->Open();
   //    Query->ParamCheck=true;

       int NoRec = Query->RecordCount;

    //   NoRec = NoRec+  NoCategories;// + StartingRow

	   int* TimeArray[31*4 + 4 + 1+ 4 + 1] ;
	   for (int i = 0; i < 31*4 + 4 +1+ 4 + 1; i++) {
           TimeArray[i]=new int[NoRec + NoCategories+1];
           for (int j = 0; j < NoRec + NoCategories +1; j++) {
               TimeArray[i][j] = 0;
		   }
       }

       int RowNo = NoRec + NoCategories + StartingRow + 2;

       FormMain->StringGridReport->RowCount=RowNo;

       struct LineStatus {
          int Id;
          AnsiString Name;
          int Category;
          AnsiString CategoryName;
          int MainID;
       };

       LineStatus* ArrayLS = new LineStatus[NoRec];

       for( int i = 0;i< NoRec;i++)
       {
         ArrayLS[i].Id =  Query->FieldByName("ID_LineStatus")->AsInteger;
         ArrayLS[i].Name = Query->FieldByName("LineStatus_Name")->AsString;
         ArrayLS[i].Category = Query->FieldByName("CategoryID")->AsInteger;
         ArrayLS[i].CategoryName = Query->FieldByName("Main")->AsString;
         ArrayLS[i].MainID = Query->FieldByName("MainID")->AsInteger;
         Query->Next();
       }


       int* IndexesLS=new int[NoRec+1];             // indexes of given Line Status
       int* IndexesCategory= new int[NoCategories+1]; // indexes of given categories

       bool* NewCategory  = new bool[NoCategories];
       for (int i=1; i < NoCategories+1; i++) {
           NewCategory[i]= true ;
       }

       IndexesLS[1] = 0 ;  // first row is for Uptime
       NewCategory[2]=false;
       Main.erase(2); // Erase Uptime from Main Categories
       IndexesCategory[2]=0;     // uptime row in the array
       IndexesCategory[1] = 1 ; //  Scheduled downtime row in the array
       NewCategory[1]=false;
       Main.erase(1); //Erase Scheduled downtime from main categories.

       int LastIndex= 1;
       int LastCategory = 2;
       int tempCat;
       AnsiString tempMain;
       int tempLS;

//       for (int i = 0; i < NoRec ; i++) {
//
//           tempMain = ArrayLS[i].CategoryMain;
//           if (NewCategory[tempCat]) {             // inexex for new categories
//               IndexesCategory[tempCat]= ++LastIndex;
//               NewCategory[tempCat]=false;
//       }
       for (int i = 0; i < NoRec ; i++) {
           tempCat = ArrayLS[i].Category;
           if (NewCategory[tempCat]) {             // indicies for new categories
               if(Main.count(ArrayLS[i].MainID)==1) // if it is main category it should be first
               {
                 Main.erase(ArrayLS[i].MainID);     // removing each not new aleready main category
                 IndexesCategory[ArrayLS[i].MainID]= ++LastIndex;
                 NewCategory[ArrayLS[i].MainID]=false;
               }
               IndexesCategory[tempCat]= ++LastIndex;
               NewCategory[tempCat]=false;
              // continue;
           }
           tempLS = ArrayLS[i].Id;
           if(tempLS == 1) continue; //skip production which is uptime.
           IndexesLS[tempLS] = ++LastIndex;
       }
//       for (int i=0; i < LastIndex; i++) {
//           Sheet->Cells[1][i]=IndexesLS[i];
//       }
//       return;
       FormMain->StringGridReport->RowCount=NoRec+NoCategories+3;
       Sheet->ColCount=31*4 + 4 +1+4 + 5;
       AnsiString TempAnsi;
       int FieldWidth[31*4 + 4 +1+4] ;
       for(int j=0;j<(31*4 +9);j++)

            FieldWidth[j]=0;
       int Width, LineStatus;
       int Offset = 7;

       for(int i=0;i<NoRec;i++)               //  Filling Up Name column of sheet - LineStatus
       {
          TempAnsi= ArrayLS[i].Name;
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

          Sheet->Cells[0][IndexesLS[ArrayLS[i].Id]+Offset] = TempAnsi;
       }

       for (int i = 0; i < NoCategories; i++) {
          TempAnsi= CategoriesName[i];
          Width =  Sheet->Canvas->TextWidth(TempAnsi);
          FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

          Sheet->Cells[0][IndexesCategory[i+1]+Offset] = TempAnsi;
       }

       for(int i=0;i<Sheet->ColCount;i++)
       {
           Sheet->ColWidths[i]=FieldWidth[i]+6;
       }

    //   std::list<LineStatus*> SubCategories;



    //   std::list<LineStatus*>* Categories = new std::list<LineStatus*>[NoCategories+1];











       Query->Close();





    QueryString =
      "  SELECT WORKSHIFTDAYS.DATE_OF_WORK, LineStatus_Type.LINESTATUS_NAME, LineStatus_Type.ID_LINESTATUS, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, CategoriesLS.CategoryID,MainID , IIf(LINE_STATUS.START_TIME>LINE_STATUS.END_TIME,24*60-DateDiff(minute,LINE_STATUS.END_TIME,LINE_STATUS.START_TIME),DateDiff(minute,LINE_STATUS.START_TIME,LINE_STATUS.END_TIME)) AS Czas, WORKSHIFTDAYS.ID_SHIFTGROUP, CategoriesLS.Main, CategoriesLS.Auxiliary "
	  "  FROM SHIFTTIME INNER JOIN (((LineStatus_Type INNER JOIN LINE_STATUS ON LineStatus_Type.ID_LINESTATUS = LINE_STATUS.ID_LINESTATUS) INNER JOIN CategoriesLS ON LineStatus_Type.CategoryID = CategoriesLS.CategoryID) INNER JOIN WORKSHIFTDAYS ON LINE_STATUS.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTTIME.ID_SHIFTTIME = WORKSHIFTDAYS.ID_SHIFTTIME "

	  "  WHERE DatePart(month,WorkshiftDays.Date_OF_WORK) = :CurrMonth and DatePart(year, WorkshiftDays.Date_OF_WORK) = :CurrYear "
      "  ORDER BY WORKSHIFTDAYS.DATE_OF_WORK "
                   ;
       Query->SQL->Clear();
       Query->SQL->Add(QueryString);
  //    FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
      Query->Parameters->ParamByName("CurrMonth")->Value = CurrMonth;// "#08/01/2011 12:01:00 AM#";
      Query->Parameters->ParamByName("CurrYear")->Value = CurrYear; // "#08/31/2011 9:59:59 PM#";
      Query->Open();
      Query->ParamCheck=true;

      int  NoRec2 = Query->RecordCount;


       float ProdIn, ProdOut, Yield, SumIn=0, SumOut=0;
       char Shifts[4]={'A','B','C','D'}  ;
       std::map<AnsiString,int> Cat;
       int DaysInMonth =  DaysInAMonth(CurrYear,CurrMonth);
       TDateTime DateOfWork;
       int IdLS;
       int Time;
       int Shift;
       int CategoryID;
       AnsiString MainName;
       AnsiString Auxiliary;
       int MainID;
       int DayNo;
       int ShiftTime[4*31];
       Sheet->RowCount = NoRec2+2;
       for(int i=0;i<4*31;i++) ShiftTime[i]=0;
	   unsigned short month, day;
	   for(int i=1;i<=NoRec2;i++)               //  , round(sum(Rozm),0) Rozmiar
       {
          DateOfWork = Query->FieldByName("Date_Of_Work")->AsDateTime;
          IdLS = Query->FieldByName("ID_LineStatus")->AsInteger;
          Time = Query->FieldByName("Czas")->AsInteger;
          Shift = Query->FieldByName("ID_ShiftGroup")->AsInteger;
          MainName = Query->FieldByName("Main")->AsString;
          Auxiliary = Query->FieldByName("Auxiliary")->AsString;
          MainID = Query->FieldByName("MainID")->AsInteger;
          CategoryID = Query->FieldByName("CategoryID")->AsInteger;
		  DateOfWork.DecodeDate(&year,&month,&day);
          if(MainID != CategoryID)
          {
			  TimeArray[9+ (day-1)*4+Shift][IndexesCategory[MainID]] +=Time;

              TimeArray[5+ Shift][IndexesCategory[MainID]] +=Time;
              TimeArray[3][IndexesCategory[MainID]]+=Time;
          }
          TimeArray[9+ (day-1)*4+Shift][IndexesLS[IdLS]]+=Time; // Time for given date and category
          TimeArray[5 + Shift] [IndexesLS[IdLS]]+=Time;         // Time for given Shift (sumarized) and category and Date
          if (MainName!="Uptime") {                              // Skip categorize for uptime
            TimeArray[9+ (day-1)*4+Shift][IndexesCategory[CategoryID]]+=Time; // Time for given category
          	TimeArray[5 + Shift] [IndexesCategory[CategoryID]]+=Time;  // Time for given Shift (sumarized) and category
            TimeArray[3][IndexesCategory[CategoryID]]+=Time;
          }

          ShiftTime[(day-1)*4+Shift-1] +=Time;                       // Summary for given date and shift
          TimeArray[3][IndexesLS[IdLS]]+=Time;                   //summary for given possition


//          Sheet->Cells[1][i] = DateOfWork  ;
//          Sheet->Cells[2][i] =IdLS;
//          Sheet->Cells[3][i] = MainID;
//          Sheet->Cells[4][i] = Auxiliary;
//          Sheet->Cells[5][i] = Main;
//          Sheet->Cells[6][i] = CategoryID;
//          Sheet->Cells[7][i] = Time;
//          Sheet->Cells[8][i] = Shift;
          Query->Next();
       }

//       Sheet->Cells[1][0] = "DateOfWork"  ;
//       Sheet->Cells[2][0] = "IdLS";
//       Sheet->Cells[3][0] = "MainID";
//       Sheet->Cells[4][0] = "Auxiliary";
//       Sheet->Cells[5][0] = "Main";
//       Sheet->Cells[6][0] = "CategoryID";
//       Sheet->Cells[7][0] = "Time";
//       Sheet->Cells[8][0] = "Shift";

       Query->Close();
//       return;

//       for(int i=0; i<(NoRec+NoCategories); i++)
//       {
//           TimeArray[2][i]= TimeArray[3][i]*100/DaysInMonth*24*60
//           TimeArray[4][i] = TimeArray[3][i]/60;
//       }

       Sheet->RowCount= NoRec+NoCategories+Offset+3;
       int Wyn;
       for(int i=0; i<(NoRec+NoCategories); i++)
       {
           for(int j = 6; j<(31*4 + 4 +1+4);j++)           // Sheet->ColCount=31*4 + 4 +1+4 + 5;
           {
              Wyn = TimeArray[j][i];
              if (Wyn!=0) {
                 Sheet->Cells[j][i+Offset]= Wyn;
              }
           }
           Sheet->Cells[2][i+Offset]= AnsiString().FormatFloat("0.00" , float(TimeArray[3][i])*100/(DaysInMonth*24*60))+"%";
           Sheet->Cells[3][i+Offset]= TimeArray[3][i];
           Sheet->Cells[4][i+Offset] = AnsiString().FormatFloat("0.00" ,float(TimeArray[3][i])/60);
       }

       for(int j = 9; j<(31*4 + 4 +1+4);j++)
       {
           Sheet->Cells[j+1][Offset-1] = ShiftTime[j-9];
           Sheet->Cells[j+1][Offset-2] = Shifts[(j-9)%4];
           if( ((j-9)%4) ==0 ) Sheet->Cells[j+1][Offset-3]= 1+int((j-9)/4);
       }

  /*
       for(int i=0; i<(NoRec+NoCategories); i++)
       {
           for(int j = 2; j<=(31*4 + 4 +1+4);j++)
           {
              Sheet->Cells[j][i+Offset]= TimeArray[j][i];
           }
       }
  */
       for(int i=2;i<Sheet->ColCount;i++)
       {
           Sheet->ColWidths[i]= 50;
       }


//          TempAnsi= Query->FieldByName("DATE_OF_WORK")->AsDateTime;
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
//
//          Sheet->Cells[0][i] = TempAnsi;
//
//          TempAnsi =  Query->FieldByName("LINESTATUS_NAME")->AsString ;
//         // SumIn += ProdIn;
//         // TempAnsi= AnsiString().FormatFloat("0.##",ProdIn);
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
//
//          Sheet->Cells[1][i] = TempAnsi ; //ShiftLetter
//
//
//          LineStatus  =  Query->FieldByName("ID_LINESTATUS")->AsInteger;
////          SumOut+=ProdOut;
////          TempAnsi= AnsiString().FormatFloat("0.##",ProdOut) ;
//          Width =  Sheet->Canvas->TextWidth(TempAnsi=AnsiString(LineStatus));
//          FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];
//
//          Sheet->Cells[2][i] = TempAnsi;
//
//          TempAnsi= Query->FieldByName("START_TIME")->AsDateTime;
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
//
//          Sheet->Cells[3][i]= TempAnsi;   //TimeString();
//
//
//          TempAnsi= Query->FieldByName("END_TIME")->AsDateTime;
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
//
//          Sheet->Cells[4][i]= TempAnsi;   //TimeString();
//
//          TempAnsi= Query->FieldByName("Czas")->AsInteger;
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];
//
//          Sheet->Cells[5][i]= TempAnsi;   //TimeString();
//
//
//             //  					Main	Auxiliary
//
//
//
//          TempAnsi= Shifts[Query->FieldByName("ID_SHIFTGROUP")->AsInteger - 1 ];
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];
//
//          Sheet->Cells[6][i]= TempAnsi;   //TimeString();
//
//          TempAnsi= Query->FieldByName("Main")->AsString;
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];
//
//          Sheet->Cells[7][i]= TempAnsi;   //TimeString();
//
//          TempAnsi= Query->FieldByName("Auxiliary")->AsString;
//          Width =  Sheet->Canvas->TextWidth(TempAnsi);
//          FieldWidth[8]=Width>FieldWidth[8]?Width:FieldWidth[8];
//
//          Sheet->Cells[8][i]= TempAnsi;   //TimeString();
//






//    Sheet->Cells[0][0]= "Data";
//    Sheet->Cells[1][0]= " Line Status ";
//    Sheet->Cells[2][0]= " ID ";
//    Sheet->Cells[3][0]= " Start Time ";
//    Sheet->Cells[4][0]= " Stop Time ";
//    Sheet->Cells[5][0]= "  Time ";
//    Sheet->Cells[6][0]= " Shift ";
//    Sheet->Cells[7][0]= " Main Status ";
//    Sheet->Cells[8][0]= " Auxiliary Status ";

      delete[] IndexesLS;
       delete[] IndexesCategory;
       delete[] NewCategory;
       for (int i = 0; i < 31*4 + 4 +1; i++) {
           delete[] TimeArray[i];
       }
       delete[] CategoriesName;

    }
    __finally{
       DataModule1->ADOConnectionCreport->Connected=false;
    }
}


struct TShiftDataStruct
{
   TDateTime Date;
   int ID;
   int Shift;
};

struct TGipShiftDataStruct
{
	TDateTime Date;
	UnicodeString ID;
	int Shift;
};



void DefectsDiffrences_Products()
{
	 try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   DataModule1->ADOConnectionGIP->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TADOQuery* QueryGIP = DataModule1->ADOQueryGIP;
       TStringGrid* Sheet =  FormMain->StringGridReport;
	 //  ClearSheet(Sheet);
	 AnsiString QueryString;

//	  TDateTime Start, Stop;
//	  unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//      unsigned short min;
//	  unsigned short sec;
//	  unsigned short msec;
////       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
////                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
////     Query->SQL->Add(QueryString);
////     Query->Parameters->ParamByName("DateR")->Value=DateR;
////     Query->Parameters->ParamByName("DateR1")->Value=DateR;
////     Query->Parameters->ParamByName("DateR2")->Value=DateR;
//	   QueryString = "SELECT CAST(DateFromParts(Year(Date_of_Work),Month(Date_of_Work),Day(Date_Of_Work)) AS DATETIME) AS ExactDate,ID_WORKSHIFTDAY,ID_SHIFTGROUP "
//					 " FROM Workshiftdays "
//					 " WHERE ((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate); ";
//
//
//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.           \\, Sum([Total_Out])*100/Sum([Total_In]) AS Yield "

		SetDateShiftWise();
	   //************** FIND VALID SHIFT ID  *****************
		QueryString = "SELECT CAST( DateFromParts(Year(Date_of_Work),Month(Date_of_Work),Day(Date_Of_Work)) AS DATETIME) AS ExactDate,ID_WORKSHIFTDAY,ID_SHIFTGROUP "
					 " FROM Workshiftdays "
					 " WHERE ((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate); ";

	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);

	   Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;

	   std::list<TShiftDataStruct> ShiftData;
	   TShiftDataStruct ShiftRec;
	   while(!Query->Eof)
	   {
		ShiftRec.Date = Query->FieldByName("ExactDate")->AsDateTime;
		ShiftRec.ID = Query->FieldByName("ID_WORKSHIFTDAY")->AsInteger;
		ShiftRec.Shift = Query->FieldByName("ID_SHIFTGROUP")->AsInteger;
		ShiftData.push_back(ShiftRec);
		Query->Next();
	   }
	   //************************************************************



	   //***************** Establish relationship with GIP_SHIFT_ID **********************************
	   QueryString = "SELECT CAST(DateFromParts(Year(Data),Month(Data),Day(Data)) AS DATETIME) AS ExactDate,DocNum,Shift "
					 " FROM CP_HEAD "
					 " WHERE ((Data)>:StartDate And (Data) <= :StopDate); ";

	   QueryGIP->SQL->Clear();
	   QueryGIP->SQL->Add(QueryString);

	   QueryGIP->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   QueryGIP->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   QueryGIP->Open();
	   QueryGIP->ParamCheck=true;

	   std::map<UnicodeString, int> GipToWorkshift;
	   std::list<TShiftDataStruct>::iterator itShift;
	   TGipShiftDataStruct GIP_ShiftRec;
	   UnicodeString TempCheck;
	   int IntCheck;
	   while(!QueryGIP->Eof)
	   {
		GIP_ShiftRec.Date = QueryGIP->FieldByName("ExactDate")->AsDateTime;
		GIP_ShiftRec.ID = QueryGIP->FieldByName("DocNum")->AsString;
		TempCheck = QueryGIP->FieldByName("Shift")->AsString;
		IntCheck =  TempCheck[1]-'A' + 1 ;
		GIP_ShiftRec.Shift = IntCheck;
		for(itShift = ShiftData.begin();itShift!=ShiftData.end();itShift++)
		{
			 if(itShift->Date == GIP_ShiftRec.Date && itShift->Shift == GIP_ShiftRec.Shift) // valid ID found;
			 {
			   GipToWorkshift[ GIP_ShiftRec.ID] = itShift->ID;
			   break;
			 }
		}
		QueryGIP->Next();
	   }

	   //***************** END of Establishing relationship with GIP_SHIFT_ID **********************************


	   QueryString = "SELECT QueryDefectSum.DefectType AS DefectName, QueryDefectSum.ID as DefectID, QueryDefectSum.Area AS SumOfArea, CP_HEAD.DOCNUM as ID "
					 "       , Sum((ROUND(CAST(Val1 as Int)*CAST(Val2 as Int)/1000000.0,2))*NumberOf) as RoundedArea  "
					 " FROM QueryDefectSum INNER JOIN CP_HEAD ON QueryDefectSum.DocNum = CP_HEAD.DOCNUM "
					// " GROUP BY CP_HEAD.DATA, CP_HEAD.DOCNUM "
					 " WHERE ((Data)>:StartDate And (Data) <= :StopDate); ";

	   QueryGIP->SQL->Clear();
	   QueryGIP->SQL->Add(QueryString);

	   QueryGIP->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   QueryGIP->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   QueryGIP->Open();
	   QueryGIP->ParamCheck=true;

	   std::map<int, double> WorkshiftToArea;
	   std::map<int, double> WorkshiftToDevelopmentArea;
	   UnicodeString GIP_ID;
	   double Area=0, TempArea;
	   double DevelopmentArea=0;
	   // std::list<TShiftDataStruct>::iterator itShift;
	   // TGipShiftDataStruct GIP_ShiftRec;
	   int DefectID; // For defelopment DefectID = 14

	   GIP_ID = QueryGIP->FieldByName("ID")->AsString;
	   UnicodeString Prev_GIP_ID = GIP_ID;
	   UnicodeString RightArea="SumOfArea";

	   if (Options.GetRestrictedLiteArea()) {
		  RightArea = "RoundedArea";
	   }

	   while(!QueryGIP->Eof)
	   {
		GIP_ID = QueryGIP->FieldByName("ID")->AsString;
		if (GIP_ID!=Prev_GIP_ID) {
		 WorkshiftToArea[GipToWorkshift[Prev_GIP_ID]] = Area;
		 WorkshiftToDevelopmentArea[GipToWorkshift[Prev_GIP_ID]]=DevelopmentArea;
		 Prev_GIP_ID = GIP_ID;
		 Area = 0;
		 DevelopmentArea = 0;
		}
		DefectID = QueryGIP->FieldByName("DefectID")->AsInteger;
		TempArea = QueryGIP->FieldByName(RightArea)->AsFloat;
		Area += TempArea;
		if (DefectID==14) {    // Development
		  DevelopmentArea+= QueryGIP->FieldByName(RightArea)->AsFloat;
		}
		//WorkshiftToArea[ GipToWorkshift[GIP_ID] ] = Area;
		QueryGIP->Next();
	   }
		// ************ Adding last values from previous loop *****************
		WorkshiftToArea[GipToWorkshift[GIP_ID]] = Area;
		WorkshiftToDevelopmentArea[GipToWorkshift[GIP_ID]]=DevelopmentArea;
		// ************ END of Adding last values from previous loop *****************


	   QueryString =
			  " SELECT Day([DATE_OF_WORK]) AS DayNo, SHIFTGROUP.NAME,WORKSHIFTDAYS.ID_WORKSHIFTDAY as ID, WORKSHIFTDAYS.ID_SHIFTTIME, Sum(PROD_SUMMARY.TOTAL_IN) AS Sqm_IN, Sum(PROD_SUMMARY.TOTAL_OUT) AS Sqm_OUT "
			  " FROM SHIFTGROUP INNER JOIN (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTGROUP.ID_SHIFTGROUP = WORKSHIFTDAYS.ID_SHIFTGROUP "
			  " WHERE (((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate)) "
			  " GROUP BY Day([DATE_OF_WORK]),  WORKSHIFTDAYS.ID_SHIFTTIME,WORKSHIFTDAYS.ID_WORKSHIFTDAY,SHIFTGROUP.NAME  ORDER BY Day([Date_Of_WORK]); "
					  ;
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
  //     FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=200;//NoRec+3;
	   Sheet->ColCount=10;
	   AnsiString TempAnsi,ShiftLetter, DiffString, DiffDevelString;
	   int FieldWidth[10] ={0};
	   int Width, Day, PrevDay=0,   ShiftType,ShiftID, NextRow=2;
	   float DayIn=0, DayOut=0, Yield, SumIn=0, SumOut=0, SqmIn, SqmOut, Difference, DifferenceDevelopment, Defects, Development;
	   UnicodeString ShiftKind[] = {"Dzień","Popołudnie","Nocka" };
	   std::map<int,int>::iterator pos;
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  ShiftType =  Query->FieldByName("ID_SHIFTTIME")->AsInteger ;
		  SqmIn =  Query->FieldByName("Sqm_In")->AsFloat ;
		  SumIn += SqmIn;
		  DayIn += SqmIn;



		  SqmOut =  Query->FieldByName("Sqm_Out")->AsFloat ;
		  SumOut += SqmOut;
		  DayOut += SqmOut;


		  ShiftLetter =  Query->FieldByName("NAME")->AsString ;



		  if (SqmIn==0) {
			Yield = 0;
		  }
		  else
		  {
			Yield = (SqmOut/SqmIn) * 100;
		  }


		  ShiftID = Query->FieldByName("ID")->AsInteger;
		  Defects = WorkshiftToArea[ShiftID];
		  Development = WorkshiftToDevelopmentArea[ShiftID];
		  Difference =  (SqmIn-SqmOut) - WorkshiftToArea[ShiftID] ;
		  DifferenceDevelopment = Defects - WorkshiftToDevelopmentArea[ShiftID];
		  DiffString ="";
		  DiffDevelString="";
		  Day= Query->FieldByName("DayNo")->AsInteger;

		  if (fabs(Difference)>0.5) {
			   DiffString =  AnsiString().FormatFloat("0.##",Difference);
			   if (fabs(DifferenceDevelopment)>0.5)
			   {
				 DiffDevelString = AnsiString().FormatFloat("0.##",DifferenceDevelopment);
			   }

			  TempAnsi =  AnsiString().FormatFloat("0.##",SqmIn);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
			  Sheet->Cells[3][NextRow] = TempAnsi;

			  TempAnsi =  AnsiString().FormatFloat("0.##",SqmOut);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
			  Sheet->Cells[4][NextRow] = TempAnsi;


			  TempAnsi =  AnsiString().FormatFloat("0.##",SqmIn-SqmOut);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];
			  Sheet->Cells[5][NextRow] = TempAnsi;

			  TempAnsi =  AnsiString().FormatFloat("0.##",Defects);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];
			  Sheet->Cells[6][NextRow] = TempAnsi;




			  TempAnsi =  ShiftLetter;   //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];
			  Sheet->Cells[2][NextRow] = TempAnsi;




			  TempAnsi =  DiffString;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];
			  Sheet->Cells[7][NextRow] = TempAnsi;


			  TempAnsi =  DiffDevelString;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[9]=Width>FieldWidth[9]?Width:FieldWidth[9];
			  Sheet->Cells[9][NextRow] = TempAnsi;


			  TempAnsi =  AnsiString().FormatFloat("0.##",WorkshiftToDevelopmentArea[ShiftID]);;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[8]=Width>FieldWidth[8]?Width:FieldWidth[8];
			  Sheet->Cells[8][NextRow] = TempAnsi;

			  TempAnsi =  ShiftKind[ShiftType-1];
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
			  Sheet->Cells[1][NextRow] = TempAnsi;





			  TempAnsi =  Day;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
			  Sheet->Cells[0][NextRow] = TempAnsi;

			  NextRow++;
		  }

		  Query->Next();
	   }
	   NextRow +=3;
	   Query->First();
	   SumIn = DayIn = SumOut = DayOut = 0;
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		  ShiftType =  Query->FieldByName("ID_SHIFTTIME")->AsInteger ;
		  SqmIn =  Query->FieldByName("Sqm_In")->AsFloat ;
		  SumIn += SqmIn;
		  DayIn += SqmIn;



		  SqmOut =  Query->FieldByName("Sqm_Out")->AsFloat ;
		  SumOut += SqmOut;
		  DayOut += SqmOut;


		  ShiftLetter =  Query->FieldByName("NAME")->AsString ;



		  if (SqmIn==0) {
			Yield = 0;
		  }
		  else
		  {
			Yield = (SqmOut/SqmIn) * 100;
		  }


		  ShiftID = Query->FieldByName("ID")->AsInteger;
		  Defects = WorkshiftToArea[ShiftID];
		  Development = WorkshiftToDevelopmentArea[ShiftID];
		  Difference =  (SqmIn-SqmOut) - WorkshiftToArea[ShiftID] ;
		  DifferenceDevelopment = Defects - WorkshiftToDevelopmentArea[ShiftID];
		  DiffString ="";
		  DiffDevelString="";
		  Day= Query->FieldByName("DayNo")->AsInteger;

		//  if (fabs(Difference)>0.5 || Development!=0) {
			   DiffString =  AnsiString().FormatFloat("0.##",Difference);
		 //	   if (fabs(DifferenceDevelopment)>0.5)
		 //	   {
				 DiffDevelString = AnsiString().FormatFloat("0.##",DifferenceDevelopment);
		 //	   }

			  TempAnsi =  AnsiString().FormatFloat("0.##",SqmIn);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
			  Sheet->Cells[3][NextRow] = TempAnsi;

			  TempAnsi =  AnsiString().FormatFloat("0.##",SqmOut);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
			  Sheet->Cells[4][NextRow] = TempAnsi;


			  TempAnsi =  AnsiString().FormatFloat("0.##",SqmIn-SqmOut);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];
			  Sheet->Cells[5][NextRow] = TempAnsi;

			  TempAnsi =  AnsiString().FormatFloat("0.##",Defects);    //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];
			  Sheet->Cells[6][NextRow] = TempAnsi;

			  TempAnsi =  ShiftLetter;   //ShiftType describes column of given shift
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];
			  Sheet->Cells[2][NextRow] = TempAnsi;

			  TempAnsi =  DiffString;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];
			  Sheet->Cells[7][NextRow] = TempAnsi;

			  TempAnsi =  DiffDevelString;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[9]=Width>FieldWidth[9]?Width:FieldWidth[9];
			  Sheet->Cells[9][NextRow] = TempAnsi;

			  TempAnsi =  AnsiString().FormatFloat("0.##",WorkshiftToDevelopmentArea[ShiftID]);;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[8]=Width>FieldWidth[8]?Width:FieldWidth[8];
			  Sheet->Cells[8][NextRow] = TempAnsi;

			  TempAnsi =  ShiftKind[ShiftType-1];
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
			  Sheet->Cells[1][NextRow] = TempAnsi;

			  TempAnsi =  Day;
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
			  Sheet->Cells[0][NextRow] = TempAnsi;

			  NextRow++;
		 // }

		  Query->Next();
	   }




	   Query->Close();




//    Sheet->Cells[0][0]= " R A P O R T    Z    P R O D U K C J I - CoC L I K E ";
//    Sheet->Cells[0][1]= "Day";
//    Sheet->Cells[1][1]= "In";
//    Sheet->Cells[2][1]= "Out";
//    Sheet->Cells[3][1]= "Yield";
//    Sheet->Cells[4][1]= "Day Shift";
//    Sheet->Cells[5][1]= "Day In";
//    Sheet->Cells[6][1]= "Day Out";
//    Sheet->Cells[7][1]= "Day Yield";
//    Sheet->Cells[8][1]= "Noon Shift";
//    Sheet->Cells[9][1]= "Noon In";
//    Sheet->Cells[10][1]= "Noon Out";
//    Sheet->Cells[11][1]= "Noon Yield";
//    Sheet->Cells[12][1]= "Night Shift";
//    Sheet->Cells[13][1]= "Night In";
//    Sheet->Cells[14][1]= "Night Out";
//    Sheet->Cells[15][1]= "Night Yield";
	   Sheet->Cells[0][0] = Start.DateString();
	   Sheet->Cells[0][2]="Day";
	   Sheet->Cells[1][2]= "Shift Kind";
	   Sheet->Cells[2][2]= "Shift";
	   Sheet->Cells[3][2]= "In";
	   Sheet->Cells[4][2]= "Out";
	   Sheet->Cells[5][2]= "In - Out";
	   Sheet->Cells[6][2]= "Defects";
	   Sheet->Cells[7][2]= "(In-Out) - Defects";
	   Sheet->Cells[8][2]= "Development";
	   Sheet->Cells[9][2]= "Defects-Development";


	for(int i=0;i<10;i++)
   {
		TempAnsi = Sheet->Cells[i][2];
		Width =  Sheet->Canvas->TextWidth(TempAnsi);
		FieldWidth[i]=Width>FieldWidth[i]?Width:FieldWidth[i];
		Sheet->ColWidths[i]=FieldWidth[i]+6;
   }
   FormMain->StringGridReport->RowCount=NextRow+3;

//	Sheet->Cells[0][PrevDay+3]="Total ";
//	Sheet->Cells[1][PrevDay+3]= AnsiString().FormatFloat("0.##",SumIn);
//	Sheet->Cells[2][PrevDay+3]= AnsiString().FormatFloat("0.##",SumOut);
//	Yield = SumIn!=0? (SumOut/SumIn)*100: 0 ;
//    Sheet->Cells[6][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",Yield);

    return;
//
//           QueryString = " Select *  from ProdType " ;
//           Query->SQL->Clear();
//           Query->SQL->Add(QueryString);
//           Query->Open();
//           NoRec = Query->RecordCount;
//           AnsiString* Products = new AnsiString[NoRec];
//           for (int i = 0; i < NoRec; i++) {
//
//                 Products[i]=Query->FieldByName("FULLNAME")->AsString;
//                 Query->Next();
//           }
//
//           QueryString =
//              " SELECT Day([DATE_OF_WORK]) AS DayNo, SHIFTGROUP.NAME, WORKSHIFTDAYS.ID_SHIFTTIME, Sum(PROD_SUMMARY.TOTAL_IN) AS Sqm_IN, Sum(PROD_SUMMARY.TOTAL_OUT) AS Sqm_OUT, Sum([Total_Out])*100/Sum([Total_In]) AS Yield "
//              " FROM "
//              " PRODTYPE  INNER JOIN (SHIFTGROUP INNER JOIN (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTGROUP.ID_SHIFTGROUP = WORKSHIFTDAYS.ID_SHIFTGROUP) ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE "
//              " WHERE (((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate)) "
//              " AND FULLNAME = :Product "
//              " GROUP BY Day([DATE_OF_WORK]),  WORKSHIFTDAYS.ID_SHIFTTIME,SHIFTGROUP.NAME  ORDER BY Day([Date_Of_WORK]); "
//                      ;







}


	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	   DataModule1->ADOConnectionGIP->Connected=false;
    }
}



void GenerateCOC_Products()
{    /* DONE : Code copied from another function, it is meant to be rebuild.
To get production information for each day and shift */
	 try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   DataModule1->ADOConnectionGIP->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
           ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TADOQuery* QueryGIP = DataModule1->ADOQueryGIP;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	 //  ClearSheet(Sheet);
//	   TDateTime Start, Stop;
	   AnsiString QueryString;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
	  unsigned short msec;
//       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
//                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
//     Query->SQL->Add(QueryString);
//     Query->Parameters->ParamByName("DateR")->Value=DateR;
//     Query->Parameters->ParamByName("DateR1")->Value=DateR;
//     Query->Parameters->ParamByName("DateR2")->Value=DateR;
	   QueryString = "SELECT CAST(DateFromParts(Year(Date_of_Work),Month(Date_of_Work),Day(Date_Of_Work)) AS DATETIME) AS ExactDate,ID_WORKSHIFTDAY,ID_SHIFTGROUP "
					 " FROM Workshiftdays "
					 " WHERE ((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate); ";


//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.           \\, Sum([Total_Out])*100/Sum([Total_In]) AS Yield "
	   SetDateShiftWise();
	   //************** FIND VALID SHIT ID  *****************
		QueryString = "SELECT CAST(DateFromParts(Year(Date_of_Work),Month(Date_of_Work),Day(Date_Of_Work)) AS DATETIME) AS ExactDate,ID_WORKSHIFTDAY,ID_SHIFTGROUP "
					 " FROM Workshiftdays "
					 " WHERE ((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate); ";

	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);

	   Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;

	   std::list<TShiftDataStruct> ShiftData;
	   TShiftDataStruct ShiftRec;
	   while(!Query->Eof)
	   {
		ShiftRec.Date = Query->FieldByName("ExactDate")->AsDateTime;
		ShiftRec.ID = Query->FieldByName("ID_WORKSHIFTDAY")->AsInteger;
		ShiftRec.Shift = Query->FieldByName("ID_SHIFTGROUP")->AsInteger;
		ShiftData.push_back(ShiftRec);
		Query->Next();
	   }
	   //************************************************************



	   //***************** Establish relationship with GIP_SHIFT_ID **********************************
	   QueryString = "SELECT CAST(DateFromParts(Year(Data),Month(Data),Day(Data)) AS DATETIME) AS ExactDate,DocNum,Shift "
					 " FROM CP_HEAD "
					 " WHERE ((Data)>:StartDate And (Data) <= :StopDate); ";

	   QueryGIP->SQL->Clear();
	   QueryGIP->SQL->Add(QueryString);

	   QueryGIP->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   QueryGIP->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   QueryGIP->Open();
	   QueryGIP->ParamCheck=true;

	   std::map<UnicodeString, int> GipToWorkshift;
	   std::list<TShiftDataStruct>::iterator itShift;
	   TGipShiftDataStruct GIP_ShiftRec;
	   UnicodeString TempCheck;
	   int IntCheck;
	   while(!QueryGIP->Eof)
	   {
		GIP_ShiftRec.Date = QueryGIP->FieldByName("ExactDate")->AsDateTime;
		GIP_ShiftRec.ID = QueryGIP->FieldByName("DocNum")->AsString;
		TempCheck = QueryGIP->FieldByName("Shift")->AsString;
		IntCheck =  TempCheck[1]-'A' + 1 ;
		GIP_ShiftRec.Shift = IntCheck;
		for(itShift = ShiftData.begin();itShift!=ShiftData.end();itShift++)
		{
			 if(itShift->Date == GIP_ShiftRec.Date && itShift->Shift == GIP_ShiftRec.Shift) // valid ID found;
			 {
			   GipToWorkshift[ GIP_ShiftRec.ID] = itShift->ID;
			   break;
			 }
		}
		QueryGIP->Next();
	   }

	   //***************** END of Establish relationship with GIP_SHIFT_ID **********************************


	   QueryString = "SELECT Sum(QueryDefectSum.Area) AS SumOfArea, CP_HEAD.DOCNUM as ID "
					 " , Sum((ROUND(CAST(Val1 as Int)*CAST(Val2 as Int)/1000000.0,2))*NumberOf) as RoundedArea "
					 " FROM QueryDefectSum INNER JOIN CP_HEAD ON QueryDefectSum.DocNum = CP_HEAD.DOCNUM "
					 " GROUP BY CP_HEAD.DATA, CP_HEAD.DOCNUM "
					 " HAVING ((Data)>:StartDate And (Data) <= :StopDate); ";

	   QueryGIP->SQL->Clear();
	   QueryGIP->SQL->Add(QueryString);

	   QueryGIP->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   QueryGIP->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   QueryGIP->Open();
	   QueryGIP->ParamCheck=true;

	   std::map<int, double> WorkshiftToArea;
	   UnicodeString GIP_ID;
	   double Area, RoundedArea;
	   // std::list<TShiftDataStruct>::iterator itShift;
	   // TGipShiftDataStruct GIP_ShiftRec;
	   while(!QueryGIP->Eof)
	   {
		GIP_ID = QueryGIP->FieldByName("ID")->AsString;
		Area = QueryGIP->FieldByName("SumOfArea")->AsFloat;
		RoundedArea = QueryGIP->FieldByName("RoundedArea")->AsFloat;
		WorkshiftToArea[ GipToWorkshift[GIP_ID] ] = Area;
		QueryGIP->Next();
	   }

	   if (Options.GetRestrictedLiteArea()) {
		  Area = RoundedArea;
	   }




	   QueryString =
			  " SELECT Day([DATE_OF_WORK]) AS DayNo, SHIFTGROUP.NAME,WORKSHIFTDAYS.ID_WORKSHIFTDAY as ID, WORKSHIFTDAYS.ID_SHIFTTIME, Sum(PROD_SUMMARY.TOTAL_IN) AS Sqm_IN, Sum(PROD_SUMMARY.TOTAL_OUT) AS Sqm_OUT "
			  " FROM SHIFTGROUP INNER JOIN (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTGROUP.ID_SHIFTGROUP = WORKSHIFTDAYS.ID_SHIFTGROUP "
			  " WHERE (((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate)) "
			  " GROUP BY Day([DATE_OF_WORK]),  WORKSHIFTDAYS.ID_SHIFTTIME,WORKSHIFTDAYS.ID_WORKSHIFTDAY,SHIFTGROUP.NAME  ORDER BY Day([Date_Of_WORK]); "
					  ;
	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);
  //     FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=35;//NoRec+3;
	   Sheet->ColCount=22;
	   AnsiString TempAnsi,ShiftLetter;
	   int FieldWidth[22] ={0};
	   int Width, Day, PrevDay=0,   ShiftType,ShiftID;
	   float DayIn=0, DayOut=0, Yield, SumIn=0, SumOut=0, SqmIn, SqmOut, Difference;

	   std::map<int,int>::iterator pos;
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {

		  Day= Query->FieldByName("DayNo")->AsInteger;

		  if(Day!=PrevDay && PrevDay!=0) // New day is considered
		  {


			TempAnsi =  AnsiString().FormatFloat("0.##",PrevDay);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
			Sheet->Cells[0][PrevDay+1] = PrevDay;


			TempAnsi =  AnsiString().FormatFloat("0.##",DayIn);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
			Sheet->Cells[1][PrevDay+1] = TempAnsi;



			TempAnsi =  AnsiString().FormatFloat("0.##",DayOut);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];
			Sheet->Cells[2][PrevDay+1] = TempAnsi;

			if (DayIn==0) {
				Yield = 0;
			  }
			  else
			  {
				Yield = (DayOut/DayIn) * 100;
			  }
			TempAnsi =  AnsiString().FormatFloat("0.##",Yield);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];
			Sheet->Cells[6][PrevDay+1] = TempAnsi;
			DayIn=0; DayOut=0;
		  }

		  ShiftType =  Query->FieldByName("ID_SHIFTTIME")->AsInteger ;


		  SqmIn =  Query->FieldByName("Sqm_In")->AsFloat ;
		  SumIn += SqmIn;
		  DayIn += SqmIn;
		  TempAnsi =  AnsiString().FormatFloat("0.##",SqmIn);    //ShiftType describes column of given shift
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[ShiftType*5+3]=Width>FieldWidth[ShiftType*5+3]?Width:FieldWidth[ShiftType*5+3];
		  Sheet->Cells[ShiftType*5+3][Day+1] = TempAnsi;


		  SqmOut =  Query->FieldByName("Sqm_Out")->AsFloat ;
		  SumOut += SqmOut;
		  DayOut += SqmOut;
		  TempAnsi =  AnsiString().FormatFloat("0.##",SqmOut);    //ShiftType describes column of given shift
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[ShiftType*5+4]=Width>FieldWidth[ShiftType*5+4]?Width:FieldWidth[ShiftType*5+4];
		  Sheet->Cells[ShiftType*5+4][Day+1] = TempAnsi;

		  ShiftLetter =  Query->FieldByName("NAME")->AsString ;
		  TempAnsi =  ShiftLetter;   //ShiftType describes column of given shift
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[ShiftType*5+2]=Width>FieldWidth[ShiftType*5+2]?Width:FieldWidth[ShiftType*5+2];
		  Sheet->Cells[ShiftType*5+2][Day+1] = TempAnsi;


		  if (SqmIn==0) {
			Yield = 0;
		  }
		  else
		  {
			Yield = (SqmOut/SqmIn) * 100;
		  }

		  TempAnsi =  AnsiString().FormatFloat("0.##",Yield);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[ShiftType*5+5]=Width>FieldWidth[ShiftType*5+5]?Width:FieldWidth[ShiftType*5+5];
		  Sheet->Cells[ShiftType*5+5][Day+1] = TempAnsi;

		  ShiftID = Query->FieldByName("ID")->AsInteger;

		  Difference =  (SqmIn-SqmOut) - WorkshiftToArea[ShiftID] ;
		  if (fabs(Difference)>0.5) {
			   TempAnsi =  AnsiString().FormatFloat("0.##",Difference);
		  }
		  else
		  {
			 TempAnsi =  "";
		  }

		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[ShiftType*5+6]=Width>FieldWidth[ShiftType*5+6]?Width:FieldWidth[ShiftType*5+6];
		  Sheet->Cells[ShiftType*5+6][Day+1] = TempAnsi;


		  PrevDay = Day;  //To check if it is next day;
		  Query->Next();
	   }
	   Query->Close();

			TempAnsi =  AnsiString().FormatFloat("0.##",PrevDay);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
			Sheet->Cells[0][PrevDay+1] = PrevDay;


			TempAnsi =  AnsiString().FormatFloat("0.##",DayIn);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
			Sheet->Cells[1][PrevDay+1] = TempAnsi;


			TempAnsi =  AnsiString().FormatFloat("0.##",DayOut);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];
			Sheet->Cells[2][PrevDay+1] = TempAnsi;

			if (DayIn==0) {
				Yield = 0;
			  }
			  else
			  {
				Yield = (DayOut/DayIn) * 100;
			  }
			TempAnsi =  AnsiString().FormatFloat("0.##",Yield);
			Width =  Sheet->Canvas->TextWidth(TempAnsi);
			FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];
			Sheet->Cells[6][PrevDay+1] = TempAnsi;
			DayIn=0; DayOut=0;


//    Sheet->Cells[0][0]= " R A P O R T    Z    P R O D U K C J I - CoC L I K E ";
//    Sheet->Cells[0][1]= "Day";
//    Sheet->Cells[1][1]= "In";
//    Sheet->Cells[2][1]= "Out";
//    Sheet->Cells[3][1]= "Yield";
//    Sheet->Cells[4][1]= "Day Shift";
//    Sheet->Cells[5][1]= "Day In";
//    Sheet->Cells[6][1]= "Day Out";
//    Sheet->Cells[7][1]= "Day Yield";
//    Sheet->Cells[8][1]= "Noon Shift";
//    Sheet->Cells[9][1]= "Noon In";
//    Sheet->Cells[10][1]= "Noon Out";
//    Sheet->Cells[11][1]= "Noon Yield";
//    Sheet->Cells[12][1]= "Night Shift";
//    Sheet->Cells[13][1]= "Night In";
//    Sheet->Cells[14][1]= "Night Out";
//    Sheet->Cells[15][1]= "Night Yield";

	   Sheet->Cells[0][1]="Day";
	   Sheet->Cells[1][1]= "Daily In";
	   Sheet->Cells[2][1]= " Daily Out";
	   Sheet->Cells[3][1]= "Sqm out in pr";
	   Sheet->Cells[4][1]= "Tlds ";
	   Sheet->Cells[5][1]= "HWieCo";
	   Sheet->Cells[6][1]= "Daily Yield";
	   Sheet->Cells[7][1]= "Morning";
	   Sheet->Cells[8][1]= "In";
	   Sheet->Cells[9][1]= "Out";
	   Sheet->Cells[10][1]= "Yield";
	   Sheet->Cells[11][1]= "Diff of Deff";
	   Sheet->Cells[12][1]= "Afternoon";
	   Sheet->Cells[13][1]= "In";
	   Sheet->Cells[14][1]= "Out";
	   Sheet->Cells[15][1]= "Yield";
	   Sheet->Cells[16][1]= "Diff of Deff";
	   Sheet->Cells[17][1]= "Night";
	   Sheet->Cells[18][1]= "In";
	   Sheet->Cells[19][1]= "Out";
	   Sheet->Cells[20][1]= "Yield";
	   Sheet->Cells[21][1]= "Diff of Deff";


	for(int i=0;i<22;i++)
   {
		TempAnsi = Sheet->Cells[i][1];
		Width =  Sheet->Canvas->TextWidth(TempAnsi);
		FieldWidth[i]=Width>FieldWidth[i]?Width:FieldWidth[i];
		Sheet->ColWidths[i]=FieldWidth[i]+6;
   }


	Sheet->Cells[0][PrevDay+3]="Total ";
	Sheet->Cells[1][PrevDay+3]= AnsiString().FormatFloat("0.##",SumIn);
	Sheet->Cells[2][PrevDay+3]= AnsiString().FormatFloat("0.##",SumOut);
	Yield = SumIn!=0? (SumOut/SumIn)*100: 0 ;
	Sheet->Cells[6][Sheet->RowCount-1]= AnsiString().FormatFloat("0.##",Yield);

	return;
//
//           QueryString = " Select *  from ProdType " ;
//           Query->SQL->Clear();
//           Query->SQL->Add(QueryString);
//           Query->Open();
//           NoRec = Query->RecordCount;
//           AnsiString* Products = new AnsiString[NoRec];
//           for (int i = 0; i < NoRec; i++) {
//
//                 Products[i]=Query->FieldByName("FULLNAME")->AsString;
//                 Query->Next();
//           }
//
//           QueryString =
//              " SELECT Day([DATE_OF_WORK]) AS DayNo, SHIFTGROUP.NAME, WORKSHIFTDAYS.ID_SHIFTTIME, Sum(PROD_SUMMARY.TOTAL_IN) AS Sqm_IN, Sum(PROD_SUMMARY.TOTAL_OUT) AS Sqm_OUT, Sum([Total_Out])*100/Sum([Total_In]) AS Yield "
//              " FROM "
//              " PRODTYPE  INNER JOIN (SHIFTGROUP INNER JOIN (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTGROUP.ID_SHIFTGROUP = WORKSHIFTDAYS.ID_SHIFTGROUP) ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE "
//              " WHERE (((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate)) "
//              " AND FULLNAME = :Product "
//              " GROUP BY Day([DATE_OF_WORK]),  WORKSHIFTDAYS.ID_SHIFTTIME,SHIFTGROUP.NAME  ORDER BY Day([Date_Of_WORK]); "
//                      ;







}


	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	   DataModule1->ADOConnectionGIP->Connected=false;
	}
}



void V2_In()
{
   try{

	   AnsiString QueryString;

	   DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryConsumedMorning;
	   TStringGrid* Sheet =  FormMain->StringGridReport;







   //    ClearSheet(Sheet);
	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();
//       ShowMessage(Query->Parameters->ParamByName("DateStart")->Value);
//       ShowMessage(Query->Parameters->ParamByName("DateStop")->Value);
	   Screen->Cursor=crSQLWait;
	   Query->Open();

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount= 33; //NoRec+1;
       Sheet->ColCount=21;
	   int ValRow ;
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
           ValRow = Query->FieldByName("Day")->AsInteger;
		  Sheet->Cells[0][ValRow+1]=ValRow;
		  Sheet->Cells[7][ValRow+1] = Query->FieldByName("Zmiana")->AsString;
		  Sheet->Cells[8][ValRow+1] = Query->FieldByName("Consumed Morning Shift")->AsFloat;
          Query->Next();
	   }

	   Query->Close();
       FormMain->RichEditReport->Lines->Add("Consumed Morning Shift");
	   DataModule1->ADOConnectionV2->Connected=false;
	   DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
       {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }




       Query = DataModule1->ADOQueryConsumedAfternoon;

	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();

	   Query->Open();
	   NoRec = Query->RecordCount;

       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		   ValRow = Query->FieldByName("Day")->AsInteger;
		  Sheet->Cells[0][ValRow+1]=ValRow;
		  Sheet->Cells[12][ValRow+1] = Query->FieldByName("Zmiana")->AsString;
		  Sheet->Cells[13][ValRow+1] = Query->FieldByName("Consumed Afternoon Shift")->AsFloat;
          Query->Next();
	   }
	   Query->Close();

       DataModule1->ADOConnectionV2->Close();//=false;
	   FormMain->RichEditReport->Lines->Add("Consumed Afternoon Shift");
       DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
           return;
	   }

	   Query = DataModule1->ADOQueryConsumedNight;

	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
       Query->Parameters->ParamByName("DateStop")->Value = StopTime();

       Query->Open();
	   NoRec = Query->RecordCount;

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		   ValRow = Query->FieldByName("Day")->AsInteger;
          Sheet->Cells[0][ValRow+1]=ValRow;
		  Sheet->Cells[17][ValRow+1] = Query->FieldByName("Shift")->AsString;
		  Sheet->Cells[18][ValRow+1] = Query->FieldByName("Consumed Night Shift")->AsFloat;
		  Query->Next();
       }


	   Query->Close();

	   DataModule1->ADOConnectionV2->Close();//=false;

	   // -- headers ----------------
	   Sheet->Cells[0][1]="Day";
	   Sheet->Cells[1][1]= "Daily In";
	   Sheet->Cells[2][1]= "Daily Out";
	   Sheet->Cells[3][1]= "Sqm out in pr";
	   Sheet->Cells[4][1]= "Tlds ";
	   Sheet->Cells[5][1]= "HWieCo";
	   Sheet->Cells[6][1]= "Daily Yield";
	   Sheet->Cells[7][1]= "Morning";
	   Sheet->Cells[8][1]= "In";
	   Sheet->Cells[9][1]= "Out";
	   Sheet->Cells[10][1]= "Yield";
	   Sheet->Cells[11][1]= "Tlds";
	   Sheet->Cells[12][1]= "Afternoon";
	   Sheet->Cells[13][1]= "In";
	   Sheet->Cells[14][1]= "Out";
	   Sheet->Cells[15][1]= "Yield";
	   Sheet->Cells[16][1]= "Tlds";
	   Sheet->Cells[17][1]= "Night";
	   Sheet->Cells[18][1]= "In";
	   Sheet->Cells[19][1]= "Out";
	   Sheet->Cells[20][1]= "Yield";
	   Sheet->Cells[21][1]= "Tlds";

	}
	__finally{
		Screen->Cursor=crDefault;
	   DataModule1->ADOConnectionV2->Connected=false;
	}
}



void DefectExtednded()
{
  try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   DataModule1->ADOConnectionGIP->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
           ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryCreport;     //ADOQueryLineStatusReport;
	   TADOQuery* QueryGIP = DataModule1->ADOQueryGIP;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	 //  ClearSheet(Sheet);
//	   TDateTime Start, Stop;
	   AnsiString QueryString;
//	   unsigned short year;
//	  unsigned short month;
//	  unsigned short day;
//	  unsigned short hour;
//	  unsigned short min;
//	  unsigned short sec;
	  unsigned short msec;
//       "WHERE DateSerial(DatePart('yyyy',WorkshiftDays.Date_OF_WORK),DatePart('m',WorkshiftDays.Date_OF_WORK),DatePart('d',WorkshiftDays.Date_OF_WORK)) = "
//                  "		 DateSerial(DatePart('yyyy',:DateR),DatePart('m',:DateR1),DatePart('d',:DateR2)) ";
//     Query->SQL->Add(QueryString);
//     Query->Parameters->ParamByName("DateR")->Value=DateR;
//     Query->Parameters->ParamByName("DateR1")->Value=DateR;
//     Query->Parameters->ParamByName("DateR2")->Value=DateR;
	   QueryString =
				  "	 DECLARE @START AS DATETIME                                                           "
				  "	 DECLARE @STOP  AS DATETIME                                                           "
				  "	 SET @START = :StartDate                                                              "
				  "	 SET @STOP = :StopDate                                                                "
				  "                                                                                       "
				  "	 SELECT [Defect]                                                             		  "
				  "		   ,[Width]                                                                       "
				  "		   ,[Length]                                                                      "
				  "		   ,[AddDefect] AS [Additional Defect]                                            "
				  "		   ,[EngAddDefect] AS [Eng. Description]                                          "
				  "		   ,[Thickness]                                                                   "
				  "		   ,[Substrate Origin]                                                            "
				  "		   ,[Substrate Type]                                                              "
				  "		   ,[QUANTITY]                                                                    "
				  "		   ,[COMMENT]                                                                     "
				  "		   ,[DATA] As SHIFT_DATE                                                          "
				  "		   ,CAST(Width AS float)*CAST([Length] as float) *Quantity/1000000.0 AS Area      "
				  "                                                                                       "
				  "	   FROM [276CoaterReports].[dbo].[CP_PARAM_DEFECT_EXPANDED_VIEW]                      "
				  "	   WHERE DATA BETWEEN @START AND @STOP                                                "
				  "	   ORDER BY [DATA]                                                                    "  ;

//					 "SELECT CAST(DateFromParts(Year(Date_of_Work),Month(Date_of_Work),Day(Date_Of_Work)) AS DATETIME) AS ExactDate,ID_WORKSHIFTDAY,ID_SHIFTGROUP "
//					 " FROM Workshiftdays "
//					 " WHERE ((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate); ";


//	   Start =  StartTime() ;
//	   Stop =   StopTime();
//	   Start.DecodeDate(&year, &month, &day);
//	   Start.DecodeTime(&hour, &min, &sec, &msec);
//	   if (hour >= 22) {
//		   Start = TDateTime(TDateTime(Start+1).DateString()+" "+TDateTime(0,1,0,0).TimeString());
//	   }
//	   Stop.DecodeDate(&year, &month, &day);
//	   Stop.DecodeTime(&hour, &min, &sec, &msec);
//	   Stop = TDateTime(TDateTime(Stop).DateString()+" "+TDateTime(23,59,59,0).TimeString()); //reporting is shiftwise therfore we need to change start and stop date.           \\, Sum([Total_Out])*100/Sum([Total_In]) AS Yield "

	   SetDateShiftWise();

//	   //************** FIND VALID SHIT ID  *****************
//		QueryString = "SELECT CAST(DateFromParts(Year(Date_of_Work),Month(Date_of_Work),Day(Date_Of_Work)) AS DATETIME) AS ExactDate,ID_WORKSHIFTDAY,ID_SHIFTGROUP "
//					 " FROM Workshiftdays "
//					 " WHERE ((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate); ";

	   Query->SQL->Clear();
	   Query->SQL->Add(QueryString);

	   Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
	   Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
	   Query->Open();
	   Query->ParamCheck=true;

//	   std::list<TShiftDataStruct> ShiftData;
//	   TShiftDataStruct ShiftRec;
//	   while(!Query->Eof)
//	   {
//		ShiftRec.Date = Query->FieldByName("ExactDate")->AsDateTime;
//		ShiftRec.ID = Query->FieldByName("ID_WORKSHIFTDAY")->AsInteger;
//		ShiftRec.Shift = Query->FieldByName("ID_SHIFTGROUP")->AsInteger;
//		ShiftData.push_back(ShiftRec);
//		Query->Next();
//	   }
//	   //************************************************************
//
//
//
//	   //***************** Establish relationship with GIP_SHIFT_ID **********************************
//	   QueryString = "SELECT CAST(DateFromParts(Year(Data),Month(Data),Day(Data)) AS DATETIME) AS ExactDate,DocNum,Shift "
//					 " FROM CP_HEAD "
//					 " WHERE ((Data)>:StartDate And (Data) <= :StopDate); ";
//
//	   QueryGIP->SQL->Clear();
//	   QueryGIP->SQL->Add(QueryString);
//
//	   QueryGIP->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
//	   QueryGIP->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
//	   QueryGIP->Open();
//	   QueryGIP->ParamCheck=true;
//
//	   std::map<UnicodeString, int> GipToWorkshift;
//	   std::list<TShiftDataStruct>::iterator itShift;
//	   TGipShiftDataStruct GIP_ShiftRec;
//	   UnicodeString TempCheck;
//	   int IntCheck;
//	   while(!QueryGIP->Eof)
//	   {
//		GIP_ShiftRec.Date = QueryGIP->FieldByName("ExactDate")->AsDateTime;
//		GIP_ShiftRec.ID = QueryGIP->FieldByName("DocNum")->AsString;
//		TempCheck = QueryGIP->FieldByName("Shift")->AsString;
//		IntCheck =  TempCheck[1]-'A' + 1 ;
//		GIP_ShiftRec.Shift = IntCheck;
//		for(itShift = ShiftData.begin();itShift!=ShiftData.end();itShift++)
//		{
//			 if(itShift->Date == GIP_ShiftRec.Date && itShift->Shift == GIP_ShiftRec.Shift) // valid ID found;
//			 {
//			   GipToWorkshift[ GIP_ShiftRec.ID] = itShift->ID;
//			   break;
//			 }
//		}
//		QueryGIP->Next();
//	   }
//
//	   //***************** END of Establish relationship with GIP_SHIFT_ID **********************************
//
//
//	   QueryString = "SELECT Sum(QueryDefectSum.Area) AS SumOfArea, CP_HEAD.DOCNUM as ID "
//					 " FROM QueryDefectSum INNER JOIN CP_HEAD ON QueryDefectSum.DocNum = CP_HEAD.DOCNUM "
//					 " GROUP BY CP_HEAD.DATA, CP_HEAD.DOCNUM "
//					 " HAVING ((Data)>:StartDate And (Data) <= :StopDate); ";
//
//	   QueryGIP->SQL->Clear();
//	   QueryGIP->SQL->Add(QueryString);
//
//	   QueryGIP->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
//	   QueryGIP->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
//	   QueryGIP->Open();
//	   QueryGIP->ParamCheck=true;
//
//	   std::map<int, double> WorkshiftToArea;
//	   UnicodeString GIP_ID;
//	   double Area;
//	   // std::list<TShiftDataStruct>::iterator itShift;
//	   // TGipShiftDataStruct GIP_ShiftRec;
//	   while(!QueryGIP->Eof)
//	   {
//		GIP_ID = QueryGIP->FieldByName("ID")->AsString;
//		Area = QueryGIP->FieldByName("SumOfArea")->AsFloat;
//		WorkshiftToArea[ GipToWorkshift[GIP_ID] ] = Area;
//		QueryGIP->Next();
//	   }
//
//
//
//
//
//	   QueryString =
//			  " SELECT Day([DATE_OF_WORK]) AS DayNo, SHIFTGROUP.NAME,WORKSHIFTDAYS.ID_WORKSHIFTDAY as ID, WORKSHIFTDAYS.ID_SHIFTTIME, Sum(PROD_SUMMARY.TOTAL_IN) AS Sqm_IN, Sum(PROD_SUMMARY.TOTAL_OUT) AS Sqm_OUT "
//			  " FROM SHIFTGROUP INNER JOIN (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTGROUP.ID_SHIFTGROUP = WORKSHIFTDAYS.ID_SHIFTGROUP "
//			  " WHERE (((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate)) "
//			  " GROUP BY Day([DATE_OF_WORK]),  WORKSHIFTDAYS.ID_SHIFTTIME,WORKSHIFTDAYS.ID_WORKSHIFTDAY,SHIFTGROUP.NAME  ORDER BY Day([Date_Of_WORK]); "
//					  ;
//	   Query->SQL->Clear();
//	   Query->SQL->Add(QueryString);
//  //     FormMain->MemoGeneralRemarks->Lines->Add(QueryString);
//	  Query->Parameters->ParamByName("StartDate")->Value = Start;// "#08/01/2011 12:01:00 AM#";
//	  Query->Parameters->ParamByName("StopDate")->Value = Stop; // "#08/31/2011 9:59:59 PM#";
//	   Query->Open();
//	   Query->ParamCheck=true;

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount=NoRec+3;
	   Sheet->ColCount=12;
	   AnsiString TempAnsi,ShiftLetter;
	   int FieldWidth[12] ={0};
//	   int Width, Day, PrevDay=0,   ShiftType,ShiftID;
//	   float DayIn=0, DayOut=0, Yield, SumIn=0, SumOut=0, SqmIn, SqmOut, Difference;
	   UnicodeString Defect, Additional, EngDescr, Thickness,Substrate,SubstrateType,Comment,ShiftDate;
	   int Width, LWidth, LLenght, Quantity;
	   float Area, SummaryArea=0;

	   std::map<int,int>::iterator pos;
	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {

		  Defect= Query->FieldByName("Defect")->AsString;
		  TempAnsi =  Defect;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][1+i] = TempAnsi;

		  LWidth =  Query->FieldByName("Width")->AsInteger ;
		  TempAnsi =  LWidth;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][1+i] = TempAnsi;

		  LLenght =  Query->FieldByName("Length")->AsInteger ;
		  TempAnsi =  LLenght;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];
		  Sheet->Cells[2][1+i] = TempAnsi;

		  Additional= Query->FieldByName("Additional Defect")->AsString;
		  TempAnsi =  Additional;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][1+i] = TempAnsi;

		  EngDescr= Query->FieldByName("Eng. Description")->AsString;
		  TempAnsi =  EngDescr;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][1+i] = TempAnsi;

		  Thickness = Query->FieldByName("Thickness")->AsString;
		  TempAnsi = Thickness;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];
		  Sheet->Cells[5][1+i] = TempAnsi;

		  Substrate = Query->FieldByName("Substrate Origin")->AsString;
		  TempAnsi = Substrate;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];
		  Sheet->Cells[6][1+i] = TempAnsi;

		  SubstrateType = Query->FieldByName("Substrate Type")->AsString;
		  TempAnsi = SubstrateType;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];
		  Sheet->Cells[7][1+i] = TempAnsi;

		  Quantity =  Query->FieldByName("Quantity")->AsInteger ;
		  TempAnsi =  Quantity;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[8]=Width>FieldWidth[8]?Width:FieldWidth[8];
		  Sheet->Cells[8][1+i] = TempAnsi;

		  Comment = Query->FieldByName("Comment")->AsString;
		  TempAnsi = Comment;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[9]=Width>FieldWidth[9]?Width:FieldWidth[9];
		  Sheet->Cells[9][1+i] = TempAnsi;

		  ShiftDate = Query->FieldByName("SHIFT_DATE")->AsString;
		  TempAnsi = ShiftDate;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[10]=Width>FieldWidth[10]?Width:FieldWidth[10];
		  Sheet->Cells[10][1+i] = TempAnsi;

		  Area =  Query->FieldByName("Area")->AsFloat ;
		  SummaryArea+=Area;
		  TempAnsi =  AnsiString().FormatFloat("0.##",Area);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[11]=Width>FieldWidth[11]?Width:FieldWidth[11];
		  Sheet->Cells[11][1+i] = TempAnsi;


		  Query->Next();
	   }
	   Query->Close();



	   Sheet->Cells[0][1]="Defect";
	   Sheet->Cells[1][1]= "Width";
	   Sheet->Cells[2][1]= "Lenght";
	   Sheet->Cells[3][1]= "Additional info";
	   Sheet->Cells[4][1]= "Additinal (eng)";
	   Sheet->Cells[5][1]= "Thickness/LamiCompostion";
	   Sheet->Cells[6][1]= "Substrate";
	   Sheet->Cells[7][1]= "Substrate Kind";
	   Sheet->Cells[8][1]= "Quantity";
	   Sheet->Cells[9][1]= "Comment";
	   Sheet->Cells[10][1]= "Shift Date";
	   Sheet->Cells[11][1]= "Area";



	for(int i=0;i<12;i++)
   {
		TempAnsi = Sheet->Cells[i][1];
		Width =  Sheet->Canvas->TextWidth(TempAnsi);
		FieldWidth[i]=Width>FieldWidth[i]?Width:FieldWidth[i];
		Sheet->ColWidths[i]=FieldWidth[i]+6;
   }


	Sheet->Cells[10][NoRec+2]="Total ";
	Sheet->Cells[11][NoRec+2]= AnsiString().FormatFloat("0.##",SummaryArea);


	return;



}


	__finally{
	   DataModule1->ADOConnectionCreport->Connected=false;
	   DataModule1->ADOConnectionGIP->Connected=false;
	}

}



void  V2_Production()
{
	try{

	   AnsiString QueryString;

	   DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query = DataModule1->ADOQueryConsumedMorning;
	   TStringGrid* Sheet =  FormMain->StringGridReport;







   //    ClearSheet(Sheet);
	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();
//       ShowMessage(Query->Parameters->ParamByName("DateStart")->Value);
//       ShowMessage(Query->Parameters->ParamByName("DateStop")->Value);
	   Screen->Cursor=crSQLWait;
	   Query->Open();

	   int NoRec = Query->RecordCount;
	   FormMain->StringGridReport->RowCount= 33; //NoRec+1;
       Sheet->ColCount=21;
	   int ValRow ;
       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
           ValRow = Query->FieldByName("Day")->AsInteger;
		  Sheet->Cells[0][ValRow+1]=ValRow;
          Sheet->Cells[7][ValRow+1] = Query->FieldByName("Zmiana")->AsString;
		  Sheet->Cells[8][ValRow+1] = Query->FieldByName("Consumed Morning Shift")->AsFloat;
          Query->Next();
	   }

	   Query->Close();
       FormMain->RichEditReport->Lines->Add("Consumed Morning Shift");
	   DataModule1->ADOConnectionV2->Connected=false;
       DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
       {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
           return;
	   }




       Query = DataModule1->ADOQueryConsumedAfternoon;

       Query->Parameters->ParamByName("DateStart")->Value = StartTime();
	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();

	   Query->Open();
       NoRec = Query->RecordCount;

       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
           ValRow = Query->FieldByName("Day")->AsInteger;
		  Sheet->Cells[0][ValRow+1]=ValRow;
		  Sheet->Cells[12][ValRow+1] = Query->FieldByName("Zmiana")->AsString;
		  Sheet->Cells[13][ValRow+1] = Query->FieldByName("Consumed Afternoon Shift")->AsFloat;
          Query->Next();
	   }
       Query->Close();

       DataModule1->ADOConnectionV2->Close();//=false;
	   FormMain->RichEditReport->Lines->Add("Consumed Afternoon Shift");
       DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
       {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
           return;
	   }

	   Query = DataModule1->ADOQueryConsumedNight;

	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
       Query->Parameters->ParamByName("DateStop")->Value = StopTime();

       Query->Open();
	   NoRec = Query->RecordCount;

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		   ValRow = Query->FieldByName("Day")->AsInteger;
          Sheet->Cells[0][ValRow+1]=ValRow;
		  Sheet->Cells[17][ValRow+1] = Query->FieldByName("Shift")->AsString;
          Sheet->Cells[18][ValRow+1] = Query->FieldByName("Consumed Night Shift")->AsFloat;
		  Query->Next();
       }


	   Query->Close();

	   DataModule1->ADOConnectionV2->Close();//=false;
//	   FormMain->RichEditReport->Lines->Add("Consumed Night Shift");
//	   DataModule1->ADOConnectionV2->Connected=true;
//	   if(DataModule1->ADOConnectionV2->Connected==false)
//	   {
//		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
//		   return;
//	   }
//
//	   Query = DataModule1->ADOQueryOutputMorning;
//
//	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
//	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();
//
//	   Query->Open();
//	   NoRec = Query->RecordCount;
//
//	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
//	   {
//		   ValRow = Query->FieldByName("Day")->AsInteger;
//		  Sheet->Cells[0][ValRow+1]=ValRow;
//	   //   Sheet->Cells[7][ValRow] = Query->FieldByName("Shift")->AsString;
//		  Sheet->Cells[9][ValRow+1] = Query->FieldByName("Output Day Shift")->AsFloat;
//		  Query->Next();
//	   }
//
//
//
//
//	   Query->Close();
//	   DataModule1->ADOConnectionV2->Close();//=false;
//	   FormMain->RichEditReport->Lines->Add("Output Day Shift");
//	   DataModule1->ADOConnectionV2->Connected=true;
//	   if(DataModule1->ADOConnectionV2->Connected==false)
//	   {
//		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
//		   return;
//	   }
//
//	   Query = DataModule1->ADOQueryOutputAfternoon;
//
//	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
//	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();
//
//	   Query->Open();
//	   NoRec = Query->RecordCount;
//
//	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
//	   {
//		   ValRow = Query->FieldByName("Day")->AsInteger;
//		  Sheet->Cells[0][ValRow+1]=ValRow;
//		 // Sheet->Cells[12][ValRow] = Query->FieldByName("Shift")->AsString;
//		  Sheet->Cells[14][ValRow+1] = Query->FieldByName("Output Afternoon Shift")->AsFloat;
//          Query->Next();
//	   }
//       Query->Close();
//	   DataModule1->ADOConnectionV2->Close();//=false;
//       FormMain->RichEditReport->Lines->Add("Output Afternoon Shift");
//	   DataModule1->ADOConnectionV2->Connected=true;
//       if(DataModule1->ADOConnectionV2->Connected==false)
//	   {
//           ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
//		   return;
//       }
//
//       Query = DataModule1->ADOQueryOutputNight;
//
//       Query->Parameters->ParamByName("DateStart")->Value = StartTime();
//	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();
//
//	   Query->Open();
//       NoRec = Query->RecordCount;
//
//       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
//	   {
//           ValRow = Query->FieldByName("Day")->AsInteger;
//		  Sheet->Cells[0][ValRow+1]=ValRow;
//         // Sheet->Cells[17][ValRow] = Query->FieldByName("Shift")->AsString;
//		  Sheet->Cells[19][ValRow+1] = Query->FieldByName("Output Night Shift")->AsFloat;
//          Query->Next();
//	   }
//
//		FormMain->RichEditReport->Lines->Add("Output Night Shift");



	   // -- headers ----------------
	   Sheet->Cells[0][1]="Day";
	   Sheet->Cells[1][1]= "Daily In";
	   Sheet->Cells[2][1]= "Daily Out";
	   Sheet->Cells[3][1]= "Sqm out in pr";
	   Sheet->Cells[4][1]= "Tlds ";
	   Sheet->Cells[5][1]= "HWieCo";
	   Sheet->Cells[6][1]= "Daily Yield";
	   Sheet->Cells[7][1]= "Morning";
	   Sheet->Cells[8][1]= "In";
	   Sheet->Cells[9][1]= "Out";
	   Sheet->Cells[10][1]= "Yield";
	   Sheet->Cells[11][1]= "Tlds";
	   Sheet->Cells[12][1]= "Afternoon";
	   Sheet->Cells[13][1]= "In";
	   Sheet->Cells[14][1]= "Out";
	   Sheet->Cells[15][1]= "Yield";
	   Sheet->Cells[16][1]= "Tlds";
	   Sheet->Cells[17][1]= "Night";
	   Sheet->Cells[18][1]= "In";
	   Sheet->Cells[19][1]= "Out";
	   Sheet->Cells[20][1]= "Yield";
	   Sheet->Cells[21][1]= "Tlds";

	}
	__finally{
		Screen->Cursor=crDefault;
	   DataModule1->ADOConnectionV2->Connected=false;
	}
}

void  V2_ProductionOut()
{
	try{

	   AnsiString QueryString;

	   DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
	   TADOQuery* Query; // = DataModule1->ADOQueryConsumedMorning;
	   TStringGrid* Sheet =  FormMain->StringGridReport;
	   int NoRec; // = Query->RecordCount;
	   int ValRow ;

	   Screen->Cursor=crSQLWait;
//
//
//
//
//
//
//
//   //    ClearSheet(Sheet);
//       Query->Parameters->ParamByName("DateStart")->Value = StartTime();
//       Query->Parameters->ParamByName("DateStop")->Value = StopTime();
////       ShowMessage(Query->Parameters->ParamByName("DateStart")->Value);
////       ShowMessage(Query->Parameters->ParamByName("DateStop")->Value);
//       Screen->Cursor=crSQLWait;
//       Query->Open();
//

//       FormMain->StringGridReport->RowCount= 33; //NoRec+1;
//       Sheet->ColCount=21;
//
//       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
//       {
//           ValRow = Query->FieldByName("Day")->AsInteger;
//          Sheet->Cells[0][ValRow+1]=ValRow;
//          Sheet->Cells[7][ValRow+1] = Query->FieldByName("Shift")->AsString;
//          Sheet->Cells[8][ValRow+1] = Query->FieldByName("Consumed Morning Shift")->AsFloat;
//          Query->Next();
//       }
//
//       Query->Close();
//       FormMain->RichEditReport->Lines->Add("Consumed Morning Shift");
//       DataModule1->ADOConnectionV2->Connected=false;
//       DataModule1->ADOConnectionV2->Connected=true;
//       if(DataModule1->ADOConnectionV2->Connected==false)
//       {
//           ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
//           return;
//       }
//
//
//
//
//       Query = DataModule1->ADOQueryConsumedAfternoon;
//
//       Query->Parameters->ParamByName("DateStart")->Value = StartTime();
//       Query->Parameters->ParamByName("DateStop")->Value = StopTime();
//
//       Query->Open();
//       NoRec = Query->RecordCount;
//
//       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
//       {
//           ValRow = Query->FieldByName("Day")->AsInteger;
//          Sheet->Cells[0][ValRow+1]=ValRow;
//          Sheet->Cells[12][ValRow+1] = Query->FieldByName("Shift")->AsString;
//          Sheet->Cells[13][ValRow+1] = Query->FieldByName("Consumed Afternoon Shift")->AsFloat;
//          Query->Next();
//       }
//       Query->Close();
//
//       DataModule1->ADOConnectionV2->Close();//=false;
//       FormMain->RichEditReport->Lines->Add("Consumed Afternoon Shift");
//       DataModule1->ADOConnectionV2->Connected=true;
//       if(DataModule1->ADOConnectionV2->Connected==false)
//       {
//           ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
//           return;
//       }
//
//       Query = DataModule1->ADOQueryConsumedNight;
//
//       Query->Parameters->ParamByName("DateStart")->Value = StartTime();
//       Query->Parameters->ParamByName("DateStop")->Value = StopTime();
//
//       Query->Open();
//       NoRec = Query->RecordCount;
//
//       for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
//       {
//           ValRow = Query->FieldByName("Day")->AsInteger;
//          Sheet->Cells[0][ValRow+1]=ValRow;
//          Sheet->Cells[17][ValRow+1] = Query->FieldByName("Shift")->AsString;
//          Sheet->Cells[18][ValRow+1] = Query->FieldByName("Consumed Night Shift")->AsFloat;
//          Query->Next();
//       }
//
//
//       Query->Close();
//
//       DataModule1->ADOConnectionV2->Close();//=false;
//       FormMain->RichEditReport->Lines->Add("Consumed Night Shift");
//       DataModule1->ADOConnectionV2->Connected=true;
//       if(DataModule1->ADOConnectionV2->Connected==false)
//       {
//           ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
//           return;
//       }

	   Query = DataModule1->ADOQueryOutputMorning;

	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();

	   Query->Open();
	   NoRec = Query->RecordCount;

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		   ValRow = Query->FieldByName("Day")->AsInteger;
		  Sheet->Cells[0][ValRow+1]=ValRow;
	   //   Sheet->Cells[7][ValRow] = Query->FieldByName("Shift")->AsString;
		  Sheet->Cells[9][ValRow+1] = Query->FieldByName("Output Day Shift")->AsFloat;
		  Query->Next();
	   }




	   Query->Close();
	   DataModule1->ADOConnectionV2->Close();//=false;
	   FormMain->RichEditReport->Lines->Add("Output Day Shift");
	   DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ V2, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }

	   Query = DataModule1->ADOQueryOutputAfternoon;

	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();

	   Query->Open();
	   NoRec = Query->RecordCount;

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		   ValRow = Query->FieldByName("Day")->AsInteger;
		  Sheet->Cells[0][ValRow+1]=ValRow;
		 // Sheet->Cells[12][ValRow] = Query->FieldByName("Shift")->AsString;
		  Sheet->Cells[14][ValRow+1] = Query->FieldByName("Output Afternoon Shift")->AsFloat;
		  Query->Next();
	   }
	   Query->Close();
	   DataModule1->ADOConnectionV2->Close();//=false;
	   FormMain->RichEditReport->Lines->Add("Output Afternoon Shift");
	   DataModule1->ADOConnectionV2->Connected=true;
	   if(DataModule1->ADOConnectionV2->Connected==false)
	   {
		   ShowMessage("Nie udało się połączyć z bazą V2, \nprawdopodobnie brak uprawnień");
		   return;
	   }

	   Query = DataModule1->ADOQueryOutputNight;

	   Query->Parameters->ParamByName("DateStart")->Value = StartTime();
	   Query->Parameters->ParamByName("DateStop")->Value = StopTime();

	   Query->Open();
	   NoRec = Query->RecordCount;

	   for(int i=1;i<=NoRec;i++)               //  , round(sum(Rozm),0) Rozmiar
	   {
		   ValRow = Query->FieldByName("Day")->AsInteger;
		  Sheet->Cells[0][ValRow+1]=ValRow;
		 // Sheet->Cells[17][ValRow] = Query->FieldByName("Shift")->AsString;
		  Sheet->Cells[19][ValRow+1] = Query->FieldByName("Output Night Shift")->AsFloat;
		  Query->Next();
	   }

		FormMain->RichEditReport->Lines->Add("Output Night Shift");



	   // -- headers ----------------
//	   Sheet->Cells[0][1]="Day";
//	   Sheet->Cells[1][1]= "Daily In";
//	   Sheet->Cells[2][1]= "Daily Out";
//	   Sheet->Cells[3][1]= "Sqm out in pr";
//	   Sheet->Cells[4][1]= "Tlds ";
//	   Sheet->Cells[5][1]= "HWieCo";
//	   Sheet->Cells[6][1]= "Daily Yield";
//	   Sheet->Cells[7][1]= "Morning";
//	   Sheet->Cells[8][1]= "In";
//	   Sheet->Cells[9][1]= "Out";
//	   Sheet->Cells[10][1]= "Yield";
//	   Sheet->Cells[11][1]= "Tlds";
//	   Sheet->Cells[12][1]= "Afternoon";
//	   Sheet->Cells[13][1]= "In";
//	   Sheet->Cells[14][1]= "Out";
//	   Sheet->Cells[15][1]= "Yield";
//	   Sheet->Cells[16][1]= "Tlds";
//	   Sheet->Cells[17][1]= "Night";
//	   Sheet->Cells[18][1]= "In";
//	   Sheet->Cells[19][1]= "Out";
//	   Sheet->Cells[20][1]= "Yield";
//       Sheet->Cells[21][1]= "Tlds";

	}
	__finally{
		Screen->Cursor=crDefault;
	   DataModule1->ADOConnectionV2->Connected=false;
	}
}

void SunGuardsTimeEvent::SetValues(TDateTime Date, char Shift, TDateTime StartT, TDateTime EndT,UnicodeString Full, UnicodeString Short,UnicodeString LineStat,UnicodeString Remark, int Prod,int LineStatID)
{
	DateOfWork = Date;
	ShiftC = Shift;
	StartTime = StartT;
	EndTime = EndT;
	ProductShort= Short;
	ProductFull = Full;
	LineStatus = LineStat;
	Remarks = Remark;
	ElapsedTime = MinuteDiff(StartT,EndT);
	ProdType = Prod;
    LineStatusID= LineStatID;
}

	void SunGuardsTimeEvent::SetAdditional(UnicodeString Add)
	{
	   AdditionalStr = Add;
	   double ID;
	   try{
		if (Add.IsEmpty() || Add.Pos("G")!=0 || Add.Pos("H")!=0 || Add.Pos("A")!=0 ) { // G, H, A are letters which usually makes names of products used in Additional
			AdditionalID=-1;
			return;
		}
		int i=0;

		ID=Add.ToDouble();
		AdditionalID = ID;
	   }
	   catch(EConvertError &E)
	   {
		  AdditionalID = -1;
	   }
	   catch (...)
	   {
		 ShowMessage("There were unexpected error in function SunGuardsTimeEvent::SetAdditional\nWystąpił błąd w/w funckcji");
	   }
	}
	void SunGuardsTimeEvent::SetWorkshiftID(int ShiftID)
	{
		WorkShiftID = ShiftID;
	}

// The hours from the night shift between 0:00 and 6:00 are
// greater than 22:00 - 0:00
bool SunGuardsTimeEvent::operator >(SunGuardsTimeEvent S)
{
	TDateTime time22(22,0,0,0);
	TDateTime time24(23,59,59,99);
	TDateTime temp, Date1, Date2;
	unsigned short dday,mmonth,yyear;
	DateOfWork.DecodeDate(&yyear,&mmonth,&dday);
	Date1=TDateTime(yyear,mmonth,dday);
	S.DateOfWork.DecodeDate(&yyear,&mmonth,&dday);
	Date2=TDateTime(yyear,mmonth,dday);
	if (Date1 > Date2 ) {
			 return true;
	}
	if (Date1< Date2) {
		return false;
	}
	bool result;
	// Date1 == Date2
	temp = S.GetStartTime();
	if((StartTime >= time22) && (StartTime <= time24))
	{
		if( (temp >= TDateTime(0,0,0,0) ) && (temp <= TDateTime(21,59,59,99)) )
		return false;
	}
	if ((temp >= time22) && (temp <= time24))
	{
		if( (StartTime >= TDateTime(0,0,0,0) ) && (StartTime <= TDateTime(21,59,59,99)) )
		return true;
	}

	result = StartTime > temp;
	return result;
}

//equal means starttimes are equal
bool SunGuardsTimeEvent::operator==(SunGuardsTimeEvent S)
{
	return (StartTime == S.GetStartTime()) && (DateOfWork==S.DateOfWork);
}

bool SunGuardsTimeEvent::operator <(SunGuardsTimeEvent S)
{
	return !(operator>( S) || operator==(S));
}

bool SunGuardsTimeEvent::operator <=(SunGuardsTimeEvent S)
{
	return operator<(S) || operator==(S);
}

bool SunGuardsTimeEvent::operator >=(SunGuardsTimeEvent S)
{
	return operator>( S) || operator==(S);
}

bool SunGuardsTimeEvent::operator !=(SunGuardsTimeEvent S)
{
	return !operator==(S);
}

void IsGreater()
{
	TDateTimePicker* Left = FormMain->DateTimePickerLeft;
	TDateTimePicker* Right = FormMain->DateTimePickerRight;
	SunGuardsTimeEvent LeftEvent;
	SunGuardsTimeEvent RightEvent;
	LeftEvent.SetValues(TDateTime().CurrentDate(),'D',Left->Time,Right->Time,L"Coœ",L"Coœ jeszcze",L"Awaria",L"Remark",1,0 );
	RightEvent.SetValues(TDateTime().CurrentDate(),'c',Right->Time,Right->Time,L"Coœ",L"Coœ jeszcze",L"Awaria",L"Remark",1,0 );
	FormMain->EditIsTimeGreater->Text = LeftEvent > RightEvent?"true":"false";
	FormMain->EditLess->Text =  LeftEvent < RightEvent?"true":"false";
	FormMain->EditEqual->Text =  LeftEvent == RightEvent?"true":"false";
	FormMain->EditEqualOrGreater->Text =  LeftEvent >= RightEvent?"true":"false";
	FormMain->EditEqualOrLess->Text =  LeftEvent <= RightEvent?"true":"false";
	FormMain->EditUnequal->Text =  LeftEvent != RightEvent?"true":"false";
}

void SunGuardsTimeClass::Add(TADOQuery* Q)
{

		  TDateTime Start, Stop, DateOfWork;
		  AnsiString TempAnsi, Product, Short, LineStatus, Shift, Remarks,Additional;
		  int ShiftID, LineStID, CatID;
		  Start = Q->FieldByName("Start_Time")->AsDateTime;
		  Stop  = Q->FieldByName("End_Time")->AsDateTime;
		  int minutes,ProdID;
		  DateOfWork= TDateTime(Q->FieldByName("Date_Of_Work")->AsDateTime);
		  Product= Q->FieldByName("Name")->AsString;
		  Short= Q->FieldByName("ShortName")->AsString;
		  LineStatus= Q->FieldByName("LINESTATUS_NAME")->AsString;
		  Shift= Q->FieldByName("Shift")->AsString;
		  minutes= MinuteDiff(Start,Stop);
		  Remarks = Q->FieldByName("Remarks")->AsString;
		  ProdID = Q->FieldByName("ID_PRODTYPE")->AsInteger;
		  ShiftID = Q->FieldByName("ID_WORKSHIFTDAY")->AsInteger;
		  Additional = Q->FieldByName("Additional")->AsString;
		  LineStID = Q->FieldByName("ID_LINESTATUS")->AsInteger;
		  CatID = Q->FieldByName("CategoryID")->AsInteger;
		  SunGuardsTimeEvent Event;
		  Event.SetValues(DateOfWork,Shift[1],Start,Stop,Product,Short,LineStatus,Remarks,ProdID,LineStID);
		  Event.SetWorkshiftID(ShiftID);
		  Event.SetAdditional(Additional);
		  Event.SetCategoryID(CatID);
		  if (DateOfWork != TempDate && TempDate!=TDateTime(0)) {  // initiate new temporary list


		  }
		  else
		  {

		  }
		  list<SunGuardsTimeEvent>::iterator it;
		//  for(it=Temp.begin();it!=Temp.end();it++)
		  it=Temp.begin();
		  while(it!=Temp.end()  )
		  {
			if (Event < *it) {
					break;
			}
			it++;
		  }
		  Temp.insert(it,Event);
}

	void SunGuardsTimeClass::ReportPrepare()
	{
        list<SunGuardsTimeEvent>::iterator it;
		 int Width,i, LineStatusID,ProductID,ElapsTime,WholeTime;
		  UnicodeString TempAnsi;
		  int FieldWidth[8] ={0,0,0,0,0,0,0,0};
		bool Service = false;
		it = Temp.begin();

        UnicodeString TempStr;
		try{
		   DataModule1->ADOConnectionCreport->Connected=true;
		   if(DataModule1->ADOConnectionCreport->Connected==false)
		   {
			   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
			   return;
		   }
		   TADOQuery* Query = DataModule1->ADOQueryTemp ;
		   UnicodeString QueryString = "Select * From PROD_BY_CATEGORY";
		   Query->Close();
		   Query->SQL->Clear();
		   Query->SQL->Add(QueryString);
		   Query->Open();
		   int ID;
		   while(!Query->Eof)
		   {
			  ID = Query->FieldByName("ID_Category")->Value;
			  switch(ID)
			  {
			   case 6:    //CG A
			   case 5:    //CG HT
			   case 4:    //SG A
			   case 3:    //SG HT
					   ProdCathegories[Query->FieldByName("ID_PRODTYPE")->Value]=ID;
					   break;
			   default:
					   break;
			  }

			 Query->Next();
		   }
		}
		__finally{
					DataModule1->ADOConnectionCreport->Connected=false;
		  }

		WholeTime = 0;
		for(i=1;it!=Temp.end() ;i++,it++)
		  {
			 ElapsTime = it->GetElapsedTime();
			 LineStatusID= it->GetLineStatusID();
			 ProductID = it->GetProductID();

			 switch(LineStatusID)
			 {
			   case 1:  Service=false;    // Uptime
			   switch( ProdCathegories[ProductID])
			   {
						 case 6:		        //ClimaGuardsA
							  KPIReport.Uptime.CGA += ElapsTime;

										break;
						 case  5:					//ClimaGuardsHT
							   KPIReport.Uptime.CGHT += ElapsTime;
										break;
						case 4:			//SunGuardsA
							   KPIReport.Uptime.SGA += ElapsTime;
										break;
						 case 3:			//SunGuardsHT
							   KPIReport.Uptime.SGHT += ElapsTime;
										break;
					   default:
						;
			   }

//					   switch (ProductID)
//					   {
//						 case 1:
//						 case 2:
//						 case 3:
//						 case 17:		        //ClimaGuardsA
//							  KPIReport.Uptime.CGA += ElapsTime;
//
//										break;
//						 case  14:
//						 case  18:					//ClimaGuardsHT
//							   KPIReport.Uptime.CGHT += ElapsTime;
//										break;
//						 case  4:
//						 case  6 :
//						 case  8 :
//						 case  10 :
//						 case  12  :
//						 case  16:			//SunGuardsA
//							   KPIReport.Uptime.SGA += ElapsTime;
//										break;
//						 case 5:
//						 case  7:
//						 case  9:
//						 case   11:
//						 case  13:
//						 case  15:			//SunGuardsHT
//							   KPIReport.Uptime.SGHT += ElapsTime;
//										break;
//					   default:
//						;
//					   }
					   break;
			   case 2:  Service = true;   //Service
						 KPIReport.ScheduledDowntime.Overhaul.Vent += ElapsTime;
						break;
			   case 6:
			   case	39:       		  //Development
						 KPIReport.ScheduledDowntime.Developement += ElapsTime;
							break;
			   case 7:					//Machine upgrade
						 KPIReport.ScheduledDowntime.MachineUpgrades += ElapsTime;
							break;
			   case 5:					//Pumping conditioning
						 if(Service)
						 {
							 KPIReport.ScheduledDowntime.Overhaul.Pumping += ElapsTime;
						 }
						 else
						 {
							 KPIReport.ScheduledDowntime.Setup += ElapsTime;
						 }
						  break;
			   case 38:
			   case       3:
			   case  32:
			   case  4 :		//Setup/Burn-in
						   KPIReport.ScheduledDowntime.Setup += ElapsTime;
						  break;
			   case 8:					//StandBy
						KPIReport.ScheduledDowntime.StandBy += ElapsTime;
						break;
			   case 12:
			   case  29:
			   case  11:
			   case  10:                 //ProcessIssues
						KPIReport.UnscheduledDowntime.ProcessIssues  += ElapsTime;
						break;
				case  34  : case  33 : case  35 : case  36 : case  37 : case  31 : case  24 : case  25 : case  26 : case  27 : case  13 : case  14 : case  28 : case  22: //AncillaryEquipment
						KPIReport.UnscheduledDowntime.AncillaryEquipment  += ElapsTime;
						break;
			   case 16: case 17: case 18: case 19: case 20: case 21: case 9:		//Coater
						KPIReport.UnscheduledDowntime.Coater  += ElapsTime;
						break;

			   case 30:							//Lack of glass, containers
						KPIReport.UnscheduledDowntime.ProductionOrganization  += ElapsTime;
						break;
			   case 15: case 23: 						//washer
						KPIReport.UnscheduledDowntime.WasherAndWatertreatment  += ElapsTime;
						break;
			 }

			 KPIReport.WholeTime += ElapsTime;




		   }
	}

	void SunGuardsTimeClass::PrintAll(TStringGrid* Sheet)
	{
		  list<SunGuardsTimeEvent>::iterator it;
		  int Width,i;
		  //TFont* Font;
		  UnicodeString TempAnsi;
		  int FieldWidth[8] ={0,0,0,0,0,0,0,0};

		  it = Temp.begin();
		  //Font =Sheet->Font;
		  Sheet->Font->Style = TFontStyles() << fsBold;
		  TempAnsi= "Data";
		  Sheet->Cells[0][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];


		  TempAnsi= "Product";
		  Sheet->Cells[1][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];


		  TempAnsi=   "Event";
		  Sheet->Cells[2][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

		  TempAnsi= "Shift";
		  Sheet->Cells[3][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

		  TempAnsi= "Start";
		  Sheet->Cells[4][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];


		  TempAnsi=  "Stop";
		  Sheet->Cells[5][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

		  TempAnsi= "Time";
		  Sheet->Cells[6][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];

		  TempAnsi=   "Comment";
		  Sheet->Cells[7][0]= TempAnsi;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];
		  Sheet->Font->Style = TFontStyles();  // Clear font styles



		  for(i=1;it!=Temp.end() ;i++,it++)
		  {
			  TempAnsi= it->GetDate().DateString();
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];

			  Sheet->Cells[0][i] = TempAnsi;

			  TempAnsi= it->GetProductFull();
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];

			  Sheet->Cells[1][i] = TempAnsi ; //Product


			  TempAnsi= it->GetLineStatus();
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[2]=Width>FieldWidth[2]?Width:FieldWidth[2];

			  Sheet->Cells[2][i] = TempAnsi ; //LineStatus
			  TempAnsi= it->GetShift();

			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];

			  Sheet->Cells[3][i]= TempAnsi;    //ShiftLetter

			  TempAnsi= it->GetStartTime().FormatString("hh:nn");
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];

			  Sheet->Cells[4][i]= TempAnsi;   //TimeString();

			  TempAnsi= it->GetEndTime().FormatString("hh:nn");
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[5]=Width>FieldWidth[5]?Width:FieldWidth[5];

			  Sheet->Cells[5][i]= TempAnsi;//TimeString();



			  TempAnsi= it->GetElapsedTime();// MinuteDiff(Start,Stop);
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[6]=Width>FieldWidth[6]?Width:FieldWidth[6];

			  Sheet->Cells[6][i]= TempAnsi;

			  TempAnsi= it->GetRemarks();
			  Width =  Sheet->Canvas->TextWidth(TempAnsi);
			  FieldWidth[7]=Width>FieldWidth[7]?Width:FieldWidth[7];

			  Sheet->Cells[7][i]= TempAnsi.Trim();
		   }
		   for(int i=0;i<8;i++)
		   {
			   Sheet->ColWidths[i]=FieldWidth[i]+6;
		   }

	}


	int SunGuardsTimeClass::GetNoMin(int LineStatusID)
	{
		  list<SunGuardsTimeEvent>::iterator it;
		  int ID,i,SumMin;
		  //TFont* Font;
		  UnicodeString TempAnsi;
		  int FieldWidth[8] ={0,0,0,0,0,0,0,0};

		  it = Temp.begin();
		  //Font =Sheet->Font;
		  SumMin=0;



		  for(i=1;it!=Temp.end() ;i++,it++)
		  {
			if(LineStatusID==it->GetLineStatusID())
			{
			  SumMin += it->GetElapsedTime();
            }
		  }
		  return SumMin;

	}


	void SunGuardsTimeClass::ExportToReportDB()
	{
        try{
	   DataModule1->ADOConnectionCreport->Connected=true;
	   if(DataModule1->ADOConnectionCreport->Connected==false)
	   {
		   ShowMessage("Nie uda³o siê po³aczyc z baz¹ Creport, \nprawdopodobnie brak uprawnieñ");
		   return;
	   }
		  UnicodeString TempStr;
		  TADOQuery* Query = DataModule1->ADOQueryTemp ;
		  UnicodeString QueryString = "Delete From EventReport";
		  Query->Close();
		  Query->SQL->Clear();
		  Query->SQL->Add(QueryString);
		  Query->ExecSQL();

		  QueryString = "INSERT INTO EventReport (EventDate, Product, Event, Shift, Start, Stop, Time, Comment,Additional) "
						" VALUES (:EventDate, :Product, :Event, :Shift, :Start, :Stop, :Time, :Comment,:Additional) ";

		  list<SunGuardsTimeEvent>::iterator it;
        //  Query->Close();
		  Query->SQL->Clear();
		 // Query->Parameters->Clear();
		  Query->SQL->Add(QueryString);
		  Query->Parameters->Refresh();
		  Query->ParamCheck = true;
		  for(it=Temp.begin();it!=Temp.end() ;it++)
		  {
              Query->SQL->Clear();
			  Query->SQL->Add(QueryString);
			  Query->Parameters->ParamByName("Product")->Value = it->GetProductShort();
			  Query->Parameters->ParamByName("EventDate")->Value  = it->GetDate().DateString();
			  Query->Parameters->ParamByName("Event")->Value = it->GetLineStatus();
			  Query->Parameters->ParamByName("Shift")->Value = it->GetShift();
			  Query->Parameters->ParamByName("Start")->Value = it->GetStartTime().FormatString("hh:nn");
			  Query->Parameters->ParamByName("Stop")->Value =  it->GetEndTime().FormatString("hh:nn");
			  Query->Parameters->ParamByName("Time")->Value =  it->GetElapsedTime();// MinuteDiff(Start,Stop);
			  Query->Parameters->ParamByName("Comment")->Value =  it->GetRemarks();
			  if(it->GetAddID()==0)
			  {
				  Query->Parameters->ParamByName("Additional")->Value = NULL;
			  }
			  else
			  {
			      Query->Parameters->ParamByName("Additional")->Value = it->GetAddStr();
			  }

			  Query->ExecSQL();
		   }
		   Query->Close();

		}
	  __finally{
				DataModule1->ADOConnectionCreport->Connected=false;
	  }

}

SunGuardsTimeClass::SunGuardsTimeClass()
{
	TempDate = TDateTime(0);   //Set initial date to defaoul 30.12.1899

}

void SunGuardsTimeClass::Clear()
{
	TempDate = TDateTime(0);
	Temp.clear();
}


void TMonthlyData::Put(int Day,int ShiftType,char Shift,double In, double Out)
{
   if(Out>In) throw std::runtime_error("TMonthlyData::Put : Out is greater than In");
   std::vector<TShiftProd> Daily(3);
   ShiftProd[Day][ShiftType].ShiftType=ShiftType;
   ShiftProd[Day][ShiftType].Shift = Shift;
   ShiftProd[Day][ShiftType].In= In;
   ShiftProd[Day][ShiftType].Out =  Out ;

}


void TMonthlyData::PutIn(int Day,int ShiftType,char Shift,double In)
{
   if(ShiftProd[Day][ShiftType].Out >In) throw std::runtime_error("TMonthlyData::PutIn : Out is greater than In");
   ShiftProd[Day][ShiftType].ShiftType=ShiftType;
   ShiftProd[Day][ShiftType].Shift = Shift;
   ShiftProd[Day][ShiftType].In= In;
}


void TMonthlyData::PutOut(int Day,int ShiftType,char Shift, double Out)
{
   if(Out >ShiftProd[Day][ShiftType].In) throw std::runtime_error("TMonthlyData::PutIn : Out is greater than In");
   ShiftProd[Day][ShiftType].ShiftType=ShiftType;
   ShiftProd[Day][ShiftType].Shift = Shift;
   ShiftProd[Day][ShiftType].Out= Out;
}

double TMonthlyData::GetYield(int Day,int ShiftType,char Shift)
{

}

double TMonthlyData::GetWholeYield()
{

}



 TKPIReport::TKPIReport()
 {
	BedUtil=0;
	WholeTime=0;

	Uptime.SGA=0;
	Uptime.SGHT=0;
	Uptime.CGA=0;
	Uptime.CGHT=0;

	ScheduledDowntime.Overhaul.Vent=0;
	ScheduledDowntime.Overhaul.Pumping=0;

	ScheduledDowntime.Setup=0;
	ScheduledDowntime.Developement=0;
	ScheduledDowntime.MachineUpgrades=0;
	ScheduledDowntime.StandBy=0;

	UnscheduledDowntime.ProcessIssues=0;
	UnscheduledDowntime.AncillaryEquipment=0;
	UnscheduledDowntime.ProductionOrganization=0;
	UnscheduledDowntime.WasherAndWatertreatment=0;
	UnscheduledDowntime.Coater=0;

	Input.SGA=0;
	Input.SGHT=0;
	Input.CGA=0;
	Input.CGHT=0;

	InputConverted.SGA=0;
	InputConverted.SGHT=0;
	InputConverted.CGA=0;
	InputConverted.CGHT=0;

	Output.SGA=0;
	Output.SGHT=0;
	Output.CGA=0;
	Output.CGHT=0;

	Reject.InputQuality=0;
	Reject.ProcessQuality=0;
	Reject.Equipment=0;
	Reject.QCBurnIns=0;
	Reject.Development=0;

	ProductionMix.Commercial=0;
	ProductionMix.Residential=0;

 }

 void TKPIReport::Clear()
 {
   	BedUtil=0;
	WholeTime=0;

	Uptime.SGA=0;
	Uptime.SGHT=0;
	Uptime.CGA=0;
	Uptime.CGHT=0;

	ScheduledDowntime.Overhaul.Vent=0;
	ScheduledDowntime.Overhaul.Pumping=0;

	ScheduledDowntime.Setup=0;
	ScheduledDowntime.Developement=0;
	ScheduledDowntime.MachineUpgrades=0;
	ScheduledDowntime.StandBy=0;

	UnscheduledDowntime.ProcessIssues=0;
	UnscheduledDowntime.AncillaryEquipment=0;
	UnscheduledDowntime.ProductionOrganization=0;
	UnscheduledDowntime.WasherAndWatertreatment=0;
	UnscheduledDowntime.Coater=0;

	Input.SGA=0;
	Input.SGHT=0;
	Input.CGA=0;
	Input.CGHT=0;

	InputConverted.SGA=0;
	InputConverted.SGHT=0;
	InputConverted.CGA=0;
	InputConverted.CGHT=0;

	Output.SGA=0;
	Output.SGHT=0;
	Output.CGA=0;
	Output.CGHT=0;

	Reject.InputQuality=0;
	Reject.ProcessQuality=0;
	Reject.Equipment=0;
	Reject.QCBurnIns=0;
	Reject.Development=0;

	ProductionMix.Commercial=0;
	ProductionMix.Residential=0;

 }

 void TKPIReport::Generate()
 {
   Clear();
   SunGuardsTime();
   Schedule.ReportPrepare();
   BedUtilization();
   ProductInfo();
   ShowDefects();
   double ProductionInput = KPIReport.Input.CGA + KPIReport.Input.CGHT +KPIReport.Input.SGA + KPIReport.Input.SGHT;
   double ProductionOutput = KPIReport.Output.CGA + KPIReport.Output.CGHT +KPIReport.Output.SGA + KPIReport.Output.SGHT;
   KPIReport.InputConverted.SGA =   (KPIReport.Input.SGA/ProductionInput)*KPIReport.Reject.Development + KPIReport.Input.SGA;
   KPIReport.InputConverted.SGHT =   (KPIReport.Input.SGHT/ProductionInput)*KPIReport.Reject.Development + KPIReport.Input.SGHT;
   KPIReport.InputConverted.CGA =   (KPIReport.Input.CGA/ProductionInput)*KPIReport.Reject.Development + KPIReport.Input.CGA;
   KPIReport.InputConverted.CGHT =   (KPIReport.Input.CGHT/ProductionInput)*KPIReport.Reject.Development + KPIReport.Input.CGHT;
   double ProductionInputConverted = KPIReport.InputConverted.CGA + KPIReport.InputConverted.CGHT +KPIReport.InputConverted.SGA + KPIReport.InputConverted.SGHT;

   TOEE_Struct ResultsOEE =   OEE();

   TStringGrid* Sheet =  FormMain->StringGridReport;
   Sheet->RowCount = 400;
   Sheet->ColCount = 5;
   for(int i=0;i<Sheet->ColCount;i++)
   { Sheet->Cols[i]->Clear(); }
   UnicodeString TempAnsi,ShiftLetter;
   int Width;
   int Row = 1;
   int FieldWidth[6] ={0};
   double TempDouble;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ResultsOEE.OEE);
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "OEE (%): ";
		  Sheet->Cells[0][Row ] = TempAnsi;
		   Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ResultsOEE.Uptime*100);
		  Sheet->Cells[1][Row ] = TempAnsi;
				  TempAnsi = "Uptime OEE (%): ";
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00",ResultsOEE.Yield*100);
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Yield OEE (%): ";
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ResultsOEE.Performance*100);
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Performance OEE (%): ";
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;


		  Row++;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00",ScheduledDowntime.StandBy  /60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "StandBy  (h) ";
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.StandBy  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "StandBy (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ScheduledDowntime.ScheduledDowntime() /60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Scheduled Downtime with StandBy (h) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.ScheduledDowntime()  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Scheduled Downtime with StandBy (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ScheduledDowntime.ScheduledOperationalDowntime() /60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Operational Scheduled Downtime (without StandBy)  (h) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.ScheduledOperationalDowntime()  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Operational Scheduled CoC-like Downtime \n(Scheduled + StandBy)/(Month hours - StandBy)  (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

//		  TempAnsi = TempAnsi.FormatFloat("0.00",ScheduledDowntime.ScheduledOperationalDowntime() /60.0 );
//		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
//		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
//		  Sheet->Cells[1][Row ] = TempAnsi;
//		  TempAnsi = "Operational Scheduled Downtime (without StandBy)  (h) ";
//		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
//		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
//		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.ScheduledOperationalDowntime()  /double(WholeTime - ScheduledDowntime.StandBy))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Operational Scheduled Downtime\n(Scheduled - StandBy)/(Month hours - StandBy)  (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;




		  Row++;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ScheduledDowntime.Overhaul.Vent  /60.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Service - Vent (h)  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.Overhaul.Vent/double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Service - Vent (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00", ScheduledDowntime.Overhaul.Pumping /60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Service - Pump Down (h)  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.Overhaul.Pumping /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Service - Pump Down (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ScheduledDowntime.Setup  /60.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Setup  (h)  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.Setup /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Setup (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00", ScheduledDowntime.Developement /60.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Development time  (h) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.Developement /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Development time (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ScheduledDowntime.MachineUpgrades  /60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Machine Upgrade (h)  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(ScheduledDowntime.MachineUpgrades /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Machine Upgrade (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;


		  Row++;
		  TempAnsi = TempAnsi.FormatFloat("0.00",UnscheduledDowntime.UnscheduledDowntime() /60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Unscheduled Downtime (h)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(UnscheduledDowntime.UnscheduledDowntime() /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Unscheduled Downtime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;
		  TempAnsi = TempAnsi.FormatFloat("0.00",(UnscheduledDowntime.UnscheduledDowntime() /double(WholeTime - ScheduledDowntime.StandBy))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Unscheduled Operational Downtime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;




		  Row++;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00", UnscheduledDowntime.ProcessIssues /60.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Process Quality Downtime (h)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(UnscheduledDowntime.ProcessIssues  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Process Quality Downtime  (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00", UnscheduledDowntime.AncillaryEquipment /60.0 )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Ancillary Equipment Downtime (h) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(UnscheduledDowntime.AncillaryEquipment  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Ancillary Equipment Downtime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",UnscheduledDowntime.Coater/60.0 )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Coater Equipment Downtime  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(UnscheduledDowntime.Coater  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Coater Equipment Downtime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",UnscheduledDowntime.ProductionOrganization /60.0 ) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Organization Downtime (h)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(UnscheduledDowntime.ProductionOrganization  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Organization Downtime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",UnscheduledDowntime.WasherAndWatertreatment /60.0)  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Washer Downtime (h)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(UnscheduledDowntime.WasherAndWatertreatment  /double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Washer Downtime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00", Reject.Reject()) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row] = TempAnsi;
		  TempAnsi = "Rejects Summary (m^2): ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;



		  Row++;
		  Row++;



		  TempAnsi = TempAnsi.FormatFloat("0.00", Reject.InputQuality) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row] = TempAnsi;
		  TempAnsi = "Input Quality Rejects (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(Reject.InputQuality  /double(Reject.Reject()))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Input Quality Rejects (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00", Reject.ProcessQuality)  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row] = TempAnsi;
		  TempAnsi = "Process Quality Rejects (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(Reject.ProcessQuality  /double(Reject.Reject()))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Process Quality Rejects (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

          Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Reject.Equipment)   ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Equipment Rejects (m^2) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(Reject.Equipment  /double(Reject.Reject()))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Process Quality Rejects (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Reject.QCBurnIns)   ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Start-up/Burn-In Rejects (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(Reject.QCBurnIns  /double(Reject.Reject()))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Start-up/Burn-In Rejects (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;


          Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Reject.Development)  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Development Rejects (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(Reject.Development  /double(Reject.Reject()))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Development Rejects (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

          Row++;
		  Row++;
		  Row++;



		  TempAnsi = TempAnsi.FormatFloat("0.00",BedUtil) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Bed Utilization (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00",ProductionMix.Commercial )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row] = TempAnsi;
		  TempAnsi = "ProductionMix Commercial (%)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",ProductionMix.Residential)  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row] = TempAnsi;
		  TempAnsi = "ProductionMix Residential (%)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row] = TempAnsi;

		  Row++;
		  Row++;
		  TempAnsi = "***";
		  for(int j=0;j<Sheet->ColCount;j++)
		  {
			   Sheet->Cells[j][Row] = TempAnsi;
		  }
		  Row++;
		  Row++;

		  TempAnsi = "11";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Operators Per Shift ClimaGuards Annealed:";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][ Row] = TempAnsi;
		  Row++;

		  TempAnsi = "12";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Operators Per Shift ClimaGuards HT:";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][ Row] = TempAnsi;
		  Row++;

		   TempAnsi = "11";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Operators Per Shift SG Annealed:";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][ Row] = TempAnsi;
		  Row++;

		  TempAnsi = "13";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Operators Per Shift SG HT:";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][ Row] = TempAnsi;
		  Row++;
		  Row++;

          TempAnsi = TempAnsi.FormatFloat("0.00",Uptime.Uptime()/60.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Uptime (h) ";
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  TempAnsi = TempAnsi.FormatFloat("0.00",(Uptime.Uptime()/double(WholeTime))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Uptime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;
		  TempAnsi = TempAnsi.FormatFloat("0.00",(Uptime.Uptime()/double(WholeTime-ScheduledDowntime.StandBy))*100.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Operational Uptime (%) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;
		  Row++;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00",Uptime.CGA/60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Uptime CG Annealed (h)  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][ Row] = TempAnsi;
		  Row++;



		  TempAnsi = TempAnsi.FormatFloat("0.00",Uptime.CGHT/60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Uptime ClimaGuardsHT (h)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][ Row] = TempAnsi;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00",Uptime.SGA  /60.0) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Uptime SunGuards Annealed (h)  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][ Row] = TempAnsi;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00",Uptime.SGHT  /60.0 );
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Uptime Sunguards HT  (h) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;
		  Row++;


           Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Output.Output() )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Production Output (m^2) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Output.CGA )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Output CG Annealed (m^2) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Output.CGHT )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Output CG HT (m^2) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Output.SGA )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Output SG Annealed (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Output.SGHT )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Output SunGuard HT (m^2) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  Row++;
		  TempAnsi = "\"Converted\" below denotes Input values summed with glass for development\n - it is assumed as proper by Analytics page"  ;
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;
		  Row++;


		  TempAnsi = TempAnsi.FormatFloat("0.00",InputConverted.Input())   ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Glass Input Converted (Included Development)  (m^2) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",InputConverted.CGA )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Input Converted CG Annealed (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",InputConverted.CGHT )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Input Converted  CG HT (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",InputConverted.SGA)   ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Input Converted  SG Annealed (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",InputConverted.SGHT )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Input Converted  SunGuard HT (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;
		  Row++;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Input.Input() )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Glass Input (m^2) ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Input.CGA  ) ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Input CG Annealed (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Input.CGHT )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Input CG HT (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Input.SGA )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Input SG Annealed (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",Input.SGHT )  ;
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][ Row] = TempAnsi;
		  TempAnsi = "Input SunGuard HT (m^2)";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;
		  Row++;






		  Row++;
		  Row++;

		  Row++;

		  TempAnsi = TempAnsi.FormatFloat("0.00",WholeTime/60.0);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[1]=Width>FieldWidth[1]?Width:FieldWidth[1];
		  Sheet->Cells[1][Row ] = TempAnsi;
		  TempAnsi = "Total Time  ";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[0]=Width>FieldWidth[0]?Width:FieldWidth[0];
		  Sheet->Cells[0][Row ] = TempAnsi;

		  TempAnsi = TempAnsi.FormatFloat("0.00",(WholeTime - ScheduledDowntime.StandBy)/60.0);
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[4]=Width>FieldWidth[4]?Width:FieldWidth[4];
		  Sheet->Cells[4][Row ] = TempAnsi;
		  TempAnsi = "Total Available Time (Total-Standby):";
		  Width =  Sheet->Canvas->TextWidth(TempAnsi);
		  FieldWidth[3]=Width>FieldWidth[3]?Width:FieldWidth[3];
		  Sheet->Cells[3][Row ] = TempAnsi;

		  Sheet->RowCount = Row+1;

	   for(int i=0;i<5;i++)
	   {
		   Sheet->ColWidths[i]=FieldWidth[i]+6;
	   }



 }
