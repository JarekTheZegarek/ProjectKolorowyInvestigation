"Select   *     from   PROD_SUMMARY "
"WHERE                                "
"  ID_Workshiftday = :ID_Workshiftday  "
"AND                                    "
"  ID_ProdType = :ID_ProdType            "

INSERT INTO   "
              " SPEED, TOTAL_IN, TOTAL_OUT, YIELD,ID_Workshiftday, ID_Prod_Type  "
              "VALUES   :SPEED, :TOTAL_IN, :TOTAL_OUT,:YIELD, :ID_Workshiftday, :ID_ProdType "
              ;
       Query->SQL->Add(QueryString);
       Query->Parameters->ParamByName("ID_WorkshifdaDate")->Value=CurrentProduction->GetID_WorkshiftDay();
       Query->Parameters->ParamByName("ID_ProdType")->Value=CurrentProduction->GetID_Product_Type();
       Query->Parameters->ParamByName("SPEED")->Value=CurrentProduction->GetSpeed();
       Query->Parameters->ParamByName("TOTAL_IN")->Value=CurrentProduction->GetTotalIn();
       Query->Parameters->ParamByName("TOTAL_OUT")->Value=CurrentProduction->GetTotalOut();
       Query->Parameters->ParamByName("YIELD")->Value=CurrentProduction->GetYield();
       Query->Open();


	Select   *     from   PROD_SUMMARY
					WHERE
					  ID_Workshiftday = :ID_Workshiftday
					AND
					  ID_Prod_Type = :ID_ProdType
					;


SELECT  Prod_Summary.*, ProductionRemarks.Remarks
FROM   Prod_Summary LEFT JOIN ProductionRemarks
       ON Prod_Summary.ID_Workshiftday = ProductionRemarks.ID_Workshiftday
          AND
       Prod_Summary.ID_Prod_Type = ProductionRemarks.ID_Prod_Type
WHERE
        Prod_Summary.ID_Workshiftday = :ID_Workshiftday



DELETE * FROM Prod_Summary
WHERE
       ID_Workshiftday = :ID_Workshiftday
AND
       ID_Prod_Type = :ID_ProdType


DELETE * FROM ProductionRemarks
WHERE
       ID_Workshiftday = :ID_Workshiftday
AND
       ID_Prod_Type = :ID_ProdType




INSERT INTO  Line_Status
 (ID_Workshiftday, , End_Time, ID_LineStatus)
VALUES   (1502, "10:00", "13:00",1 )
                  ;

 DELETE * FROM     Line_Status
    WHERE ID_Workshiftday = 1502 AND Start_Time = #10:00#


SELECT DEFECTS.Value, CP_Param.Value AS Size,Sum(Quantity) AS NumberOf, Sum((Quantity*CP_Param.Val1*CP_Param.Val2)/1000000) AS Area
FROM (CP_Defect INNER JOIN CP_Param ON CP_Defect.SizeID = CP_Param.ID)
     INNER JOIN (SELECT * FROM CP_Param WHERE TYPE = 'DEFECT') AS DEFECTS
     ON CP_DEFECT.DefectID = DEFECTS.ID
WHERE DocNum = 'PC000259'
GROUP BY DEFECTS.Value, CP_Param.Value


SELECT DocNum FROM CP_HEAD WHERE Data BETWEEN #13/06/2011 10:00:00 PM# AND #14/06/2011 6:00:00 AM#


                      SELECT DEFECTS.Value as DefectType, CP_Param.Value AS Size,Sum(Quantity) AS NumberOf, Sum((Quantity*CP_Param.Val1*CP_Param.Val2)/1000000) AS Area
                      FROM (CP_Defect INNER JOIN CP_Param ON CP_Defect.SizeID = CP_Param.ID)
     				  INNER JOIN (SELECT * FROM CP_Param WHERE TYPE = 'DEFECT') AS DEFECTS
     				  ON CP_DEFECT.DefectID = DEFECTS.ID
					  WHERE DocNum = 'PC007807'
                      GROUP BY DEFECTS.Value, CP_Param.Value


//PC007679 - different kind of deffects and sizes


 SELECT * FROM  Line_Status
                              WHERE  ID_Workshiftday = 1505
                              AND  Start_Time > #06:00#
                              ORDER BY Start_Time


 SELECT * FROM  Line_Status
                              WHERE  ID_Workshiftday = :ID_Workshiftday
                              AND  Start_Time < :Start_Time
                              ORDER BY Start_Time
                                        ;

SELECT LINE_STATUS.ID_WORKSHIFTDAY, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, LINE_STATUS.ID_LINESTATUS, LineStatusRemarks.Remarks
FROM LINE_STATUS LEFT JOIN LineStatusRemarks ON (LINE_STATUS.ID_LINESTATUS = LineStatusRemarks.ID_LINESTATUS) AND (LINE_STATUS.END_TIME = LineStatusRemarks.END_TIME) AND (LINE_STATUS.START_TIME = LineStatusRemarks.START_TIME) AND (LINE_STATUS.ID_WORKSHIFTDAY = LineStatusRemarks.ID_WORKSHIFTDAY);


SELECT *
FROM QueryLineStatusRemarks  INNER JOIN LineStatus_Type
        ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus

SELECT Workshiftdays.*, ShiftGroup.Name, QueryLineStatusRemarks.*, LineStatus_Type.LineStatus_Name
FROM ((QueryLineStatusRemarks  INNER JOIN LineStatus_Type
        ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus)
     INNER JOIN Workshiftdays ON Workshiftdays.ID_Workshiftday = QueryLineStatusRemarks.ID_Workshiftday)
     INNER JOIN SHIFTGROUP ON Workshiftdays.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP
WHERE Date_Of_Work Between #07/01/2011# and #07/31/2011#
AND  LineStatus_Name LIKE '%Developme%'

SELECT *
FROM QueryLineStatusRemarks  INNER JOIN LineStatus_Type
        ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus


SELECT Workshiftdays.*, ShiftGroup.Name, QueryLineStatusRemarks.*, LineStatus_Type.LineStatus_Name
FROM ((QueryLineStatusRemarks  INNER JOIN LineStatus_Type
        ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus)
     INNER JOIN Workshiftdays ON Workshiftdays.ID_Workshiftday = QueryLineStatusRemarks.ID_Workshiftday)
     INNER JOIN SHIFTGROUP ON Workshiftdays.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP
WHERE Date_Of_Work Between :StartDate and :StopDate
AND  LineStatus_Name LIKE '%:LineStatus_Name%'
ORDER BY Date_Of_Work ASC


  SELECT Workshiftdays.*, ShiftGroup.Name, QueryLineStatusRemarks.*, LineStatus_Type.LineStatus_Name
                FROM ((QueryLineStatusRemarks  INNER JOIN LineStatus_Type
                        ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus)
                     INNER JOIN Workshiftdays ON Workshiftdays.ID_Workshiftday = QueryLineStatusRemarks.ID_Workshiftday)
                     INNER JOIN SHIFTGROUP ON Workshiftdays.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP
                WHERE Date_Of_Work Between #08/01/2011 12:01:00 AM# and #08/31/2011 9:59:59 PM#
                AND  LineStatus_Name LIKE '*Develop*'
                ORDER BY Date_Of_Work ASC
    Query->Parameters->ParamByName("StartDate")->Value = "#08/01/2011 12:01:00 AM#";//Start ;
       Query->Parameters->ParamByName("StopDate")->Value = "#08/31/2011 9:59:59 PM#"; Stop ;
       Query->Parameters->ParamByName("LineStatus_Name")->Value = "\'*"+Type+"*\'"


   SELECT Workshiftdays.*, ShiftGroup.Name, QueryLineStatusRemarks.*, LineStatus_Type.LineStatus_Name
                FROM ((QueryLineStatusRemarks  INNER JOIN LineStatus_Type
                        ON QueryLineStatusRemarks.ID_Linestatus = LineStatus_Type.ID_Linestatus)
                     RIGHT JOIN Workshiftdays ON Workshiftdays.ID_Workshiftday = QueryLineStatusRemarks.ID_Workshiftday)
                     INNER JOIN SHIFTGROUP ON Workshiftdays.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP
                WHERE
                 Date_Of_Work Between #08/01/2011 12:01:00 AM# and #08/31/2011 9:59:59 PM#
                ORDER BY Date_Of_Work ASC


SELECT WORKSHIFTDAYS.DATE_OF_WORK, PROD_SUMMARY.TOTAL_IN, PROD_SUMMARY.TOTAL_OUT, PRODTYPE.FULLNAME, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME)) AS Czas, SHIFTGROUP.NAME
FROM ((((PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE) INNER JOIN LINE_STATUS ON WORKSHIFTDAYS.ID_WORKSHIFTDAY = LINE_STATUS.ID_WORKSHIFTDAY) INNER JOIN LINESTATUS_TYPE ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS) INNER JOIN SHIFTGROUP ON WORKSHIFTDAYS.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP
WHERE (((PRODTYPE.FULLNAME) Like "* SunGuard*") AND (([WORKSHIFTDAYS]![DATE_OF_WORK]) Between #6/30/2011 22:0:0# And #7/31/2011 22:0:0#) AND ((LINESTATUS_TYPE.LINESTATUS_NAME) Like "*Production*"))
ORDER BY WORKSHIFTDAYS.DATE_OF_WORK;


     SELECT WORKSHIFTDAYS.DATE_OF_WORK, PROD_SUMMARY.TOTAL_IN, PROD_SUMMARY.TOTAL_OUT, PRODTYPE.FULLNAME, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME)) AS Czas, SHIFTGROUP.NAME
               , QueryLineStatusRemarks.Remarks
               FROM ((((PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE) INNER JOIN LINE_STATUS ON WORKSHIFTDAYS.ID_WORKSHIFTDAY = LINE_STATUS.ID_WORKSHIFTDAY) INNER JOIN LINESTATUS_TYPE ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS) INNER JOIN SHIFTGROUP ON WORKSHIFTDAYS.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP
          		, QueryLineStatusRemarks

          		WHERE (((PRODTYPE.FULLNAME) Like \"* SunGuard*\") AND (([WORKSHIFTDAYS]![DATE_OF_WORK]) Between :StartDate And :StopDate) AND ((LINESTATUS_TYPE.LINESTATUS_NAME) Like \"*Production*\"))
               AND (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS)
              ORDER BY WORKSHIFTDAYS.DATE_OF_WORK;


// Poni¿ej dla DataBase Explorera

SELECT WORKSHIFTDAYS.DATE_OF_WORK, PROD_SUMMARY.TOTAL_IN, PROD_SUMMARY.TOTAL_OUT, PRODTYPE.FULLNAME, LINE_STATUS.START_TIME,
LINE_STATUS.END_TIME,

 QueryLineStatusRemarks.Remarks

FROM ((((PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON
        PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE) INNER JOIN LINE_STATUS ON
                                                          WORKSHIFTDAYS.ID_WORKSHIFTDAY = LINE_STATUS.ID_WORKSHIFTDAY)
        INNER JOIN LINESTATUS_TYPE ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS) INNER JOIN SHIFTGROUP ON
                                                                               WORKSHIFTDAYS.ID_SHIFTGROUP = SHIFTGROUP.ID_SHIFTGROUP
           , QueryLineStatusRemarks

          WHERE ( (Date_Of_Work Between #08/01/2011 12:01:00 AM# and #08/31/2011 9:59:59 PM#)  AND  (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS) )
              AND Fullname like '%Sun%'
              ORDER BY WORKSHIFTDAYS.DATE_OF_WORK;


SELECT SUM( PROD_SUMMARY.TOTAL_IN), SUM(PROD_SUMMARY.TOTAL_OUT), PRODTYPE.FULLNAME


FROM (((PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) INNER JOIN PRODTYPE ON
        PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE) )
        INNER JOIN LINESTATUS_TYPE ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS

          WHERE ( (Date_Of_Work Between #08/01/2011 12:01:00 AM# and #08/31/2011 9:59:59 PM#)  AND  (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS) )

GROUP BY FULLNAME




SELECT WORKSHIFTDAYS.ID_WorkShiftDay, WORKSHIFTDAYS.DATE_OF_WORK,  PRODTYPE.FULLNAME, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, SUM(IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME))) AS Czas
               , QueryLineStatusRemarks.Remarks
               FROM (( PRODTYPE  INNER JOIN LINE_STATUS ON WORKSHIFTDAYS.ID_WORKSHIFTDAY = LINE_STATUS.ID_WORKSHIFTDAY) INNER JOIN LINESTATUS_TYPE ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS)
          		, QueryLineStatusRemarks

          		WHERE (((PRODTYPE.FULLNAME) Like \"*SunGuard*\") AND (([WORKSHIFTDAYS]![DATE_OF_WORK]) Between :StartDate And :StopDate) AND ((LINESTATUS_TYPE.LINESTATUS_NAME) Like \"*Production*\"))
               AND (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS)
              ORDER BY WORKSHIFTDAYS.DATE_OF_WORK;


SELECT WORKSHIFTDAYS.ID_WorkShiftDay, WORKSHIFTDAYS.DATE_OF_WORK,  PRODTYPE.FULLNAME, LINE_STATUS.START_TIME, LINE_STATUS.END_TIME, (IIf(LINE_STATUS!START_TIME>LINE_STATUS!END_TIME,24*60-DateDiff('n',LINE_STATUS!END_TIME,LINE_STATUS!START_TIME),DateDiff('n',LINE_STATUS!START_TIME,LINE_STATUS!END_TIME))) AS Czas
               , QueryLineStatusRemarks.Remarks
               FROM (( PRODTYPE  INNER JOIN LINE_STATUS ON WORKSHIFTDAYS.ID_WORKSHIFTDAY = LINE_STATUS.ID_WORKSHIFTDAY) INNER JOIN LINESTATUS_TYPE ON LINE_STATUS.ID_LINESTATUS = LINESTATUS_TYPE.ID_LINESTATUS)
          		, QueryLineStatusRemarks

          		WHERE (((PRODTYPE.FULLNAME) Like '%SunGuard%') AND (([WORKSHIFTDAYS]![DATE_OF_WORK]) #08/01/2011 12:01:00 AM# and #08/31/2011 9:59:59 PM#) AND ((LINESTATUS_TYPE.LINESTATUS_NAME) Like '%Production%'))
               AND (LINE_STATUS.ID_WORKSHIFTDAY=QueryLineStatusRemarks.ID_WORKSHIFTDAY) AND (LINE_STATUS.START_TIME=QueryLineStatusRemarks.START_TIME) AND (LINE_STATUS.END_TIME=QueryLineStatusRemarks.END_TIME) AND (LINE_STATUS.ID_LINESTATUS=QueryLineStatusRemarks.ID_LINESTATUS)
              ORDER BY WORKSHIFTDAYS.DATE_OF_WORK;




 SELECT Day([DATE_OF_WORK]) AS DayNo, SHIFTGROUP.NAME, WORKSHIFTDAYS.ID_SHIFTTIME, Sum(PROD_SUMMARY.TOTAL_IN) AS Sqm_IN, Sum(PROD_SUMMARY.TOTAL_OUT) AS Sqm_OUT, Sum([Total_Out])*100/Sum([Total_In]) AS Yield
               FROM
               PRODTYPE  INNER JOIN (SHIFTGROUP INNER JOIN (PROD_SUMMARY INNER JOIN WORKSHIFTDAYS ON PROD_SUMMARY.ID_WORKSHIFTDAY = WORKSHIFTDAYS.ID_WORKSHIFTDAY) ON SHIFTGROUP.ID_SHIFTGROUP = WORKSHIFTDAYS.ID_SHIFTGROUP) ON PROD_SUMMARY.ID_PROD_TYPE = PRODTYPE.ID_PRODTYPE
               WHERE (((WORKSHIFTDAYS.DATE_OF_WORK)>:StartDate And (WORKSHIFTDAYS.DATE_OF_WORK) <= :StopDate))
               GROUP BY Day([DATE_OF_WORK]),  WORKSHIFTDAYS.ID_SHIFTTIME,SHIFTGROUP.NAME  ORDER BY Day([Date_Of_WORK]);
                      ;





