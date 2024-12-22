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
