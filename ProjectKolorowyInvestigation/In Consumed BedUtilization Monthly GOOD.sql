DECLARE @DateStart datetime
DECLARE @DateStop datetime
DECLARE @HourStart int
DECLARE @HourStop int
Set @DateStart = '2011-03-31 22:00:00'
Set @DateStop = '2011-04-30 21:59:59'
Set @HourStart = 14
Set @HourStop = 21
SELECT Dimensions, ROUND(Sum(Rozmiar),0)
FROM
	(SELECT Dimensions, round(sum(Rozm),0) Rozmiar
	 FROM
		(
		SELECT
			ConsumeTransactionView.Lites Rozm,
			Dimensions
		FROM
			(ConsumeTransactionView
				JOIN
			 ProductViewNEW
				ON
			 ProductViewNEW.ProductViewOidValue = ConsumeTransactionView.ProductOidValue)

		WHERE
			ConsumeTransactionView.ShiftDate BETWEEN @DateStart AND @DateStop
			and
				ConsumeTransactionView.WorkCenter = 'Coater'
		) AS Czesc
		GROUP By Dimensions


         UNION ALL

		SELECT
		Dimensions,
		ConsumeTransactionView.Lites Rozm
			
		FROM
			(
                         ConsumeTransactionView
				JOIN
			 ProductViewNEW
				ON
			 ProductViewNEW.ProductViewOidValue = ConsumeTransactionView.ProductOidValue
                         )
		WHERE 
			ConsumeTransactionView.ReversalShiftDate BETWEEN @DateStart AND @DateStop
			AND
                        ConsumeTransactionView.ReversalWorkCenter = 'Coater'
         )  AS Tabelka

		GROUP BY Dimensions 
		ORDER BY Dimensions Desc