object DataModule1: TDataModule1
  OldCreateOrder = False
  Height = 888
  Width = 1145
  object ADOConnectionCreport: TADOConnection
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'SQLOLEDB'
    Left = 48
    Top = 8
  end
  object ADOTableWorkshiftDays: TADOTable
    Connection = ADOConnectionExchange
    CursorType = ctStatic
    LockType = ltReadOnly
    MaxRecords = 4
    TableName = 'WORKSHIFTDAYS'
    Left = 160
    Top = 424
  end
  object ADOConnectionExchange: TADOConnection
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 32
    Top = 424
  end
  object DataSourceCreport: TDataSource
    DataSet = ADODataSetCreport
    Left = 352
    Top = 8
  end
  object ADODataSetCreport: TADODataSet
    Connection = ADOConnectionCreport
    CommandType = cmdTable
    Parameters = <>
    Left = 240
    Top = 8
  end
  object ADOTable2: TADOTable
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    TableName = 'CURRENT_WORKSHIFTDAYS_LIST Query'
    Left = 144
    Top = 8
  end
  object ADOQueryCreport: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    Left = 456
    Top = 8
  end
  object ADOQueryExchange: TADOQuery
    Connection = ADOConnectionExchange
    Parameters = <
      item
        Name = 'DateR'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateR'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateR'
        Size = -1
        Value = Null
      end
      item
        Name = 'ShiftLetterR'
        Size = -1
        Value = Null
      end
      item
        Name = 'ShiftR'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      
        'SELECT * FROM WorkshiftDays LEFT JOIN WorkshiftDaysRemarks ON Wo' +
        'rkshiftDays.ID_WORKSHIFTDAY = WorkshiftDaysRemarks.ID_WorkshiftD' +
        'ay'
      
        'WHERE DateSerial(DatePart('#39'yyyy'#39',WorkshiftDays.Date_OF_WORK),Dat' +
        'ePart('#39'm'#39',WorkshiftDays.Date_OF_WORK),DatePart('#39'd'#39',WorkshiftDays' +
        '.Date_OF_WORK)) ='
      
        ' DateSerial(DatePart('#39'yyyy'#39',:DateR),DatePart('#39'm'#39',:DateR),DatePar' +
        't('#39'd'#39',:DateR))'
      'AND   ID_SHIFTGROUP = :ShiftLetterR'
      'AND   ID_SHIFTTIME  = :ShiftR   ;')
    Left = 32
    Top = 480
  end
  object ADOTableShiftsGroup: TADOTable
    Connection = ADOConnectionExchange
    CursorType = ctStatic
    TableDirect = True
    TableName = 'SHIFTGROUP'
    Left = 160
    Top = 480
  end
  object ADODataSetTableWorkshiftDays: TADODataSet
    Connection = ADOConnectionExchange
    CursorType = ctStatic
    CommandText = 'WORKSHIFTDAYS'
    CommandType = cmdTable
    Parameters = <>
    Left = 472
    Top = 424
  end
  object DataSourceTableWorkshiftDays: TDataSource
    DataSet = ADODataSetTableWorkshiftDays
    Left = 312
    Top = 424
  end
  object ADODataSetTableShiftsGroup: TADODataSet
    Connection = ADOConnectionExchange
    CursorType = ctStatic
    CommandText = 'SHIFTGROUP'
    CommandType = cmdTable
    DataSource = DataSourceTableWorkshiftDays
    Parameters = <>
    Left = 472
    Top = 480
  end
  object DataSourceTableShiftsGroup: TDataSource
    DataSet = ADODataSetTableShiftsGroup
    Left = 312
    Top = 480
  end
  object ADOConnectionGIP: TADOConnection
    LoginPrompt = False
    Provider = 'SQLNCLI'
    Left = 24
    Top = 576
  end
  object ADOQueryGIP: TADOQuery
    Connection = ADOConnectionGIP
    Parameters = <>
    Left = 168
    Top = 576
  end
  object ADOQueryGIP_Seek: TADOQuery
    Connection = ADOConnectionGIP
    Parameters = <>
    Left = 280
    Top = 576
  end
  object ADOConnectionV2: TADOConnection
    CommandTimeout = 390
    ConnectionString = 
      'Provider=SQLOLEDB.1;Integrated Security=SSPI;Persist Security In' +
      'fo=False;Initial Catalog=V2;Data Source=276DB01;'
    ConnectionTimeout = 180
    LoginPrompt = False
    Provider = 'SQLOLEDB.1'
    Left = 24
    Top = 744
  end
  object ADOQueryBedUtilization: TADOQuery
    Connection = ADOConnectionV2
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop '
      'Set @HourStart = 14'
      'Set @HourStop = 21'
      
        'SELECT Dimensions, ROUND(Sum(Rozmiar),0) BedSum, WidthDim, Lengt' +
        'hDim'
      'FROM'
      
        #9'(SELECT Dimensions, round(sum(Rozm),0) Rozmiar, WidthDim, Lengt' +
        'hDim'
      #9' FROM'
      #9#9'('
      #9#9'SELECT'
      #9#9#9'Reporting.ConsumeTransactionView.Lites Rozm,'
      #9#9#9'Dimensions, WidthDim, LengthDim'
      ''
      #9#9'FROM'
      #9#9#9'(Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue = Reporting.Con' +
        'sumeTransactionView.ProductOidValue)'
      ''
      #9#9'WHERE'
      
        #9#9#9'Reporting.ConsumeTransactionView.ShiftDate BETWEEN @DateStart' +
        ' AND @DateStop'
      #9#9#9'and'
      #9#9#9#9'Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      #9#9') AS Czesc'
      #9#9'GROUP By Dimensions, WidthDim, LengthDim'
      ''
      ''
      ''
      '         UNION ALL'
      ''
      #9#9'SELECT'
      #9#9'Dimensions,'
      
        #9#9'Reporting.ConsumeTransactionView.Lites Rozm, WidthDim, LengthD' +
        'im'
      ''
      #9#9#9
      #9#9'FROM'
      #9#9#9'('
      '       Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue = ConsumeTransa' +
        'ctionView.ProductOidValue'
      '                         )'
      #9#9'WHERE '
      
        #9#9#9'Reporting.ConsumeTransactionView.ReversalShiftDate BETWEEN @D' +
        'ateStart AND @DateStop'
      #9#9#9'AND'
      
        '                        Reporting.ConsumeTransactionView.Reversa' +
        'lWorkCenter = '#39'Coater'#39
      '         )  AS Tabelka'
      ''
      #9#9'GROUP BY Dimensions,  WidthDim, LengthDim'
      ' '
      #9#9'ORDER BY Dimensions Desc')
    Left = 144
    Top = 744
  end
  object ADOQueryLineStatusReport: TADOQuery
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    ParamCheck = False
    Parameters = <>
    Left = 576
    Top = 8
  end
  object ADOQueryV2Production: TADOQuery
    Connection = ADOConnectionV2
    CommandTimeout = 330
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop '
      'Set @HourStart = 14'
      'Set @HourStop = 21'
      ''
      ''
      'SELECT Dimensions, ROUND(Sum(Rozmiar),0) BedSum'
      'FROM'
      #9'(SELECT Dimensions, round(sum(Rozm),0) Rozmiar'
      #9' FROM'
      #9#9'('
      #9#9'SELECT'
      #9#9#9'Reporting.ConsumeTransactionView.Lites Rozm,'
      #9#9#9'Dimensions'
      #9#9'FROM'
      #9#9#9'(Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue = ConsumeTransa' +
        'ctionView.ProductOidValue)'
      ''
      #9#9'WHERE'
      
        #9#9#9'Reporting.ConsumeTransactionView.ShiftDate BETWEEN @DateStart' +
        ' AND @DateStop'
      #9#9#9'and'
      #9#9#9#9'Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      #9#9') AS Czesc'
      #9#9'GROUP By Dimensions'
      ''
      ''
      'UNION ALL'
      ''
      #9#9'SELECT'
      #9#9'Dimensions,'
      #9#9'Reporting.ConsumeTransactionView.Lites Rozm'
      ''
      #9#9'FROM'
      #9#9#9'('
      '       Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue = ConsumeTransa' +
        'ctionView.ProductOidValue'
      '       )'
      #9#9'WHERE '
      
        #9#9#9'Reporting.ConsumeTransactionView.ReversalShiftDate BETWEEN @D' +
        'ateStart AND @DateStop'
      #9#9#9'AND'
      
        '      Reporting.ConsumeTransactionView.ReversalWorkCenter = '#39'Coa' +
        'ter'#39
      '    )  AS Tabelka'
      ''
      #9#9'GROUP BY Dimensions '
      #9#9'ORDER BY Dimensions Desc')
    Left = 32
    Top = 808
  end
  object ADOQueryConsumedMorning: TADOQuery
    Connection = ADOConnectionV2
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop'
      'Set @HourStart = 6'
      'Set @HourStop = 13'
      
        'SELECT Dzien as Day, Zmiana, ROUND(Sum(Rozmiar),2) "Consumed Mor' +
        'ning Shift"'
      'FROM'
      #9'(SELECT Dzien , Zmiana, round(sum(Rozm),0) Rozmiar'
      #9' FROM'
      #9#9'('
      #9#9'SELECT'
      
        #9#9#9'datepart(day, Reporting.ConsumeTransactionView.ShiftDate)  Dz' +
        'ien,'
      
        #9#9#9' Reporting.ConsumeTransactionView.ShiftDate,  Reporting.Consu' +
        'meTransactionView.Shift Zmiana,'
      
        #9#9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite AS Roz' +
        'm'
      #9#9'FROM'
      #9#9#9'( Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Co' +
        'nsumeTransactionView.ProductOidValue)'
      ''
      #9#9'WHERE'
      
        #9#9#9' Reporting.ConsumeTransactionView.ShiftDate BETWEEN @DateStar' +
        't AND @DateStop'
      #9#9#9'and'
      #9#9#9'('
      
        #9#9#9#9'Datepart(hour, Reporting.ConsumeTransactionView.ShiftDate)Be' +
        'tween @HourStart AND @HourStop'
      #9#9#9')'
      #9#9#9'and'
      #9#9#9#9' Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      #9#9') AS Czesc'
      #9#9'GROUP By Dzien, Zmiana'
      ''
      ''
      'UNION  ALL'
      ''
      #9#9'SELECT'
      
        #9#9#9'datepart(day, Reporting.ConsumeTransactionView.ReversalShiftD' +
        'ate)  Dzien,  Reporting.ConsumeTransactionView.ReversalShift Zmi' +
        'ana,'
      
        #9#9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite AS Roz' +
        'miar'
      #9#9'FROM'
      #9#9#9'( Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Co' +
        'nsumeTransactionView.ProductOidValue)'
      #9#9'WHERE'
      
        #9#9#9' Reporting.ConsumeTransactionView.ReversalShiftDate BETWEEN @' +
        'DateStart AND @DateStop'
      #9#9#9'and'
      #9#9#9'('
      
        #9#9#9#9'Datepart(hour, Reporting.ConsumeTransactionView.ReversalShif' +
        'tDate)Between @HourStart AND @HourStop'
      #9#9#9')'
      #9#9#9'and'
      
        #9#9#9#9' Reporting.ConsumeTransactionView.ReversalWorkCenter = '#39'Coat' +
        'er'#39
      #9#9')  AS Tabelka'
      ''
      #9#9'GROUP BY Dzien, Zmiana'
      #9#9'ORDER BY Dzien ASC')
    Left = 288
    Top = 744
  end
  object ADOQueryConsumedAfternoon: TADOQuery
    Connection = ADOConnectionV2
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop'
      'Set @HourStart = 14'
      'Set @HourStop = 21'
      ''
      ''
      
        'SELECT Dzien as Day, Zmiana, ROUND(Sum(Rozmiar),2) "Consumed Aft' +
        'ernoon Shift"'
      'FROM'
      #9'(SELECT Dzien,  Zmiana, round(sum(Rozm),0) Rozmiar'
      #9' FROM'
      #9#9'('
      #9#9'SELECT'
      
        #9#9#9'datepart(day, Reporting.ConsumeTransactionView.ShiftDate)  Dz' +
        'ien,'
      
        #9#9#9' Reporting.ConsumeTransactionView.ShiftDate ,  Reporting.Cons' +
        'umeTransactionView.Shift Zmiana,'
      
        #9#9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite AS Roz' +
        'm'
      #9#9'FROM'
      #9#9#9'( Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Co' +
        'nsumeTransactionView.ProductOidValue)'
      ''
      #9#9'WHERE'
      
        #9#9#9' Reporting.ConsumeTransactionView.ShiftDate BETWEEN @DateStar' +
        't AND @DateStop'
      #9#9#9'and'
      #9#9#9'('
      
        #9#9#9#9'Datepart(hour, Reporting.ConsumeTransactionView.ShiftDate)Be' +
        'tween @HourStart AND @HourStop'
      #9#9#9')'
      #9#9#9'and'
      #9#9#9#9' Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      ''
      ''
      ''
      'UNION  ALL'
      ''
      #9#9'SELECT '
      
        #9#9#9'datepart(day, Reporting.ConsumeTransactionView.ReversalShiftD' +
        'ate)  Dzien,'
      '      Reporting.ConsumeTransactionView.ShiftDate,'
      '      Reporting.ConsumeTransactionView.ReversalShift Zmiana,'
      
        #9#9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite AS Roz' +
        'miar'
      #9#9'FROM '
      #9#9#9'( Reporting.ConsumeTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Co' +
        'nsumeTransactionView.ProductOidValue)'
      #9#9'WHERE'
      
        #9#9#9' Reporting.ConsumeTransactionView.ReversalShiftDate BETWEEN @' +
        'DateStart AND @DateStop'
      #9#9#9'and'
      #9#9#9'('
      
        #9#9#9#9'Datepart(hour, Reporting.ConsumeTransactionView.ReversalShif' +
        'tDate)Between @HourStart AND @HourStop'
      #9#9#9')'
      #9#9#9'and '
      
        #9#9#9#9' Reporting.ConsumeTransactionView.ReversalWorkCenter = '#39'Coat' +
        'er'#39
      #9#9')  AS Tabelka'
      ''
      #9'GROUP BY Dzien, Zmiana'
      ' --ORDER BY Dzien ASC'
      ''
      ''
      '    ) AS Czesc'
      #9#9'GROUP By Dzien, Zmiana'
      '    ORDER BY Dzien ASC')
    Left = 472
    Top = 744
  end
  object ADOQueryConsumedNight: TADOQuery
    Connection = ADOConnectionV2
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop'
      ''
      ''
      ''
      ''
      'SELECT '
      #9'Czesc3.Dzien as Day, '
      #9'Czesc3.Zmiana as Shift, '
      '  ROUND(sum(Czesc3.Rozm),2) "Consumed Night Shift"'
      'FROM'
      '('
      #9
      #9#9'SELECT  '
      
        #9#9#9'datepart(day, Reporting.ConsumeTransactionView.ShiftDate)  Dz' +
        'ien,'
      #9#9#9' Reporting.ConsumeTransactionView.ShiftDate,'
      #9#9#9' Reporting.ConsumeTransactionView.Shift  Zmiana,'
      
        #9#9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite as Roz' +
        'm'
      #9#9'FROM '
      #9#9'('
      #9#9#9' Reporting.ConsumeTransactionView'
      #9#9#9'JOIN'
      #9#9#9'Reporting.ProductViewNEW'
      #9#9#9'ON'
      
        #9#9#9'Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Con' +
        'sumeTransactionView.ProductOidValue'
      #9#9')'
      #9#9'WHERE'
      
        #9#9' Reporting.ConsumeTransactionView.ShiftDate BETWEEN @DateStart' +
        ' AND @DateStop'
      #9#9'and'
      #9#9'('
      
        #9#9#9'Datepart(hour, Reporting.ConsumeTransactionView.ShiftDate)Bet' +
        'ween 0 AND 5'
      #9#9')'
      #9#9'and '
      #9#9' Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      'UNION ALL'
      #9'SELECT  '
      
        #9#9'datepart(day, Reporting.ConsumeTransactionView.ShiftDate)+ 1  ' +
        'Dzien,'
      #9#9' Reporting.ConsumeTransactionView.ShiftDate,'
      #9#9' Reporting.ConsumeTransactionView.Shift  Zmiana,'
      #9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite as Rozm'
      #9'FROM'
      #9'('
      #9#9' Reporting.ConsumeTransactionView'
      #9#9'JOIN'
      #9#9'Reporting.ProductViewNEW'
      #9#9'ON'
      
        #9#9'Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Cons' +
        'umeTransactionView.ProductOidValue'
      #9')'
      #9'WHERE'
      
        #9' Reporting.ConsumeTransactionView.ShiftDate BETWEEN DATEADD(day' +
        ',1,@DateStart) AND @DateStop --DATEADD(day,1,@DateStop)'
      #9'and'
      #9'('
      
        #9#9'Datepart(hour, Reporting.ConsumeTransactionView.ShiftDate)Betw' +
        'een 22 AND 23'
      #9')'
      #9'and '
      #9' Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      ''
      ''
      ''
      'UNION ALL'
      ''
      #9#9'SELECT  '
      
        #9#9#9'datepart(day, Reporting.ConsumeTransactionView.ReversalShiftD' +
        'ate)  Dzien,'
      #9#9#9' Reporting.ConsumeTransactionView.ReversalShiftDate,'
      #9#9#9' Reporting.ConsumeTransactionView.ReversalShift  Zmiana,'
      
        #9#9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite as Roz' +
        'm'
      #9#9'FROM '
      #9#9'('
      #9#9#9' Reporting.ConsumeTransactionView'
      #9#9#9'JOIN'
      #9#9#9'Reporting.ProductViewNEW'
      #9#9#9'ON'
      
        #9#9#9'Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Con' +
        'sumeTransactionView.ProductOidValue'
      #9#9')'
      #9#9'WHERE'
      
        #9#9' Reporting.ConsumeTransactionView.ReversalShiftDate BETWEEN DA' +
        'TEADD(day,1,@DateStart) AND @DateStop'
      #9#9'and'
      #9#9'('
      
        #9#9#9'Datepart(hour, Reporting.ConsumeTransactionView.ReversalShift' +
        'Date)Between 0 AND 5'
      #9#9')'
      #9#9'and '
      #9#9' Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      'UNION ALL'
      #9'SELECT  '
      
        #9#9'datepart(day, Reporting.ConsumeTransactionView.ReversalShiftDa' +
        'te)+ 1  Dzien,'
      #9#9' Reporting.ConsumeTransactionView.ReversalShiftDate,'
      #9#9' Reporting.ConsumeTransactionView.ReversalShift  Zmiana,'
      
        #9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite as Rozm' +
        ' -- Reporting.ConsumeTransactionView.*, ProductViewNEW.*'
      #9'FROM '
      #9'('#9
      #9#9' Reporting.ConsumeTransactionView'
      #9#9'JOIN'
      #9#9'Reporting.ProductViewNEW'
      #9#9'ON'
      
        #9#9'Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Cons' +
        'umeTransactionView.ProductOidValue'
      #9')'
      #9'WHERE'
      
        #9' Reporting.ConsumeTransactionView.ReversalShiftDate BETWEEN DAT' +
        'EADD(day,1,@DateStart) AND @DateStop'
      #9'and'
      #9'('
      
        #9#9'Datepart(hour, Reporting.ConsumeTransactionView.ReversalShiftD' +
        'ate)Between 22 AND 23'
      #9')'
      #9'and'
      #9' Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      ''
      ''
      'UNION ALL'
      #9'SELECT'
      #9#9'1  Dzien,'
      #9#9' Reporting.ConsumeTransactionView.ShiftDate,'
      #9#9' Reporting.ConsumeTransactionView.Shift  Zmiana,'
      #9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite as Rozm'
      #9'FROM'
      #9'('
      #9#9' Reporting.ConsumeTransactionView'
      #9#9'JOIN'
      #9#9'Reporting.ProductViewNEW'
      #9#9'ON'
      
        #9#9'Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Cons' +
        'umeTransactionView.ProductOidValue'
      #9')'
      #9'WHERE'
      
        #9' Reporting.ConsumeTransactionView.ShiftDate BETWEEN @DateStart ' +
        'AND DATEADD(second,59,DATEADD(minute,119,@DateStart))'
      #9#9'and'
      #9' Reporting.ConsumeTransactionView.WorkCenter = '#39'Coater'#39
      ''
      ''
      'UNION ALL'
      #9'SELECT'
      #9#9'1  Dzien,'
      #9#9' Reporting.ConsumeTransactionView.ReversalShiftDate,'
      #9#9' Reporting.ConsumeTransactionView.ReversalShift  Zmiana,'
      #9#9' Reporting.ConsumeTransactionView.Lites*AreaSqMPerLite as Rozm'
      #9'FROM'
      #9'('
      #9#9' Reporting.ConsumeTransactionView'
      #9#9'JOIN'
      #9#9'Reporting.ProductViewNEW'
      #9#9'ON'
      
        #9#9'Reporting.ProductViewNEW.ProductViewOidValue =  Reporting.Cons' +
        'umeTransactionView.ProductOidValue'
      #9')'
      #9'WHERE'
      
        #9' Reporting.ConsumeTransactionView.ReversalShiftDate BETWEEN @Da' +
        'teStart AND DATEADD(second,59,DATEADD(minute,119,@DateStart))'
      #9#9'and'
      #9' Reporting.ConsumeTransactionView.ReversalWorkCenter = '#39'Coater'#39
      ')as Czesc3'
      ''
      'Group By  Dzien'
      #9'        ,Zmiana'
      'Order By Dzien ASC')
    Left = 624
    Top = 744
  end
  object ADOQueryOutputMorning: TADOQuery
    Connection = ADOConnectionV2
    CommandTimeout = 330
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop'
      'Set @HourStart = 6'
      'Set @HourStop = 13'
      ''
      
        #9'SELECT Dzien as "Day", Shift, round(sum(Rozm),2) "Output Day Sh' +
        'ift"'
      #9' FROM'
      #9#9'('
      #9#9'SELECT'
      
        #9#9#9'datepart(day,Reporting.ProductionTransactionView.ShiftDate)  ' +
        'Dzien,'
      
        #9#9#9'Reporting.ProductionTransactionView.ShiftDate, Reporting.Prod' +
        'uctionTransactionView.Shift,'
      
        #9#9#9'Reporting.ProductionTransactionView.Lites*AreaSqMPerLite AS R' +
        'ozm'
      #9#9'FROM'
      #9#9#9'(Reporting.ProductionTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue = Reporting.Pro' +
        'ductionTransactionView.ProductOidValue)'
      ''
      #9#9'WHERE'
      
        #9#9#9'Reporting.ProductionTransactionView.ShiftDate BETWEEN @DateSt' +
        'art AND @DateStop'
      #9#9#9'and'
      #9#9#9'('
      
        #9#9#9#9'Datepart(hour, Reporting.ProductionTransactionView.ShiftDate' +
        ')Between @HourStart AND @HourStop'
      #9#9#9')'
      #9#9#9'and'
      #9#9#9#9'Reporting.ProductionTransactionView.WorkCenter = '#39'Coater'#39
      #9#9') AS Czesc'
      #9#9'GROUP By Dzien, Shift'
      #9#9'ORDER BY Dzien ASC')
    Left = 288
    Top = 816
  end
  object ADOQueryOutputAfternoon: TADOQuery
    Connection = ADOConnectionV2
    CommandTimeout = 330
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop'
      'Set @HourStart = 14'
      'Set @HourStop = 21'
      ''
      
        #9'SELECT Dzien as "Day", Shift,round(sum(Rozm),2) "Output Afterno' +
        'on Shift"'
      #9' FROM'
      #9#9'('
      #9#9'SELECT '
      
        #9#9#9'datepart(day,Reporting.ProductionTransactionView.ShiftDate)  ' +
        'Dzien,'
      
        #9#9#9'Reporting.ProductionTransactionView.ShiftDate, Reporting.Prod' +
        'uctionTransactionView.Shift,'
      
        #9#9#9'Reporting.ProductionTransactionView.Lites*AreaSqMPerLite AS R' +
        'ozm'
      #9#9'FROM '
      #9#9#9'(Reporting.ProductionTransactionView'
      #9#9#9#9'JOIN'
      #9#9#9' Reporting.ProductViewNEW'
      #9#9#9#9'ON'
      
        #9#9#9' Reporting.ProductViewNEW.ProductViewOidValue = Reporting.Pro' +
        'ductionTransactionView.ProductOidValue)'
      ''
      #9#9'WHERE '
      
        #9#9#9'Reporting.ProductionTransactionView.ShiftDate BETWEEN @DateSt' +
        'art AND @DateStop'
      #9#9#9'and'
      #9#9#9'('
      
        #9#9#9#9'Datepart(hour,Reporting.ProductionTransactionView.ShiftDate)' +
        'Between @HourStart AND @HourStop'
      #9#9#9')'
      #9#9#9'and '
      #9#9#9#9'Reporting.ProductionTransactionView.WorkCenter = '#39'Coater'#39
      #9#9') AS Czesc'
      #9#9'GROUP By Dzien, Shift '
      #9#9'ORDER BY Dzien ASC'
      '')
    Left = 472
    Top = 808
  end
  object ADOQueryOutputNight: TADOQuery
    Connection = ADOConnectionV2
    CommandTimeout = 300
    Parameters = <
      item
        Name = 'DateStart'
        Size = -1
        Value = Null
      end
      item
        Name = 'DateStop'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @DateStart datetime'
      'DECLARE @DateStop datetime'
      'DECLARE @HourStart int'
      'DECLARE @HourStop int'
      'Set @DateStart = :DateStart'
      'Set @DateStop = :DateStop'
      ''
      ''
      'SELECT '
      #9'Czesc3.Dzien as "Day", '
      #9'Czesc3.Zmiana as "Shift", '
      '  ROUND(sum(Czesc3.Rozm),2) "Output Night Shift"'
      'FROM'
      '('
      ''
      #9#9'SELECT'
      
        #9#9#9'datepart(day,Reporting.ProductionTransactionView.ShiftDate)  ' +
        'Dzien,'
      #9#9#9'Reporting.ProductionTransactionView.ShiftDate,'
      #9#9#9'Reporting.ProductionTransactionView.Shift  Zmiana,'
      
        #9#9#9'Reporting.ProductionTransactionView.Lites*AreaSqMPerLite as R' +
        'ozm --ProductionTransactionView.*, ProductViewNEW.*'
      #9#9'FROM'
      #9#9'('
      #9#9#9'Reporting.ProductionTransactionView'
      #9#9#9'JOIN'
      #9#9#9'Reporting.ProductViewNEW'
      #9#9#9'ON'
      
        #9#9#9'Reporting.ProductViewNEW.ProductViewOidValue = Reporting.Prod' +
        'uctionTransactionView.ProductOidValue'
      #9#9')'
      #9#9'WHERE'
      
        #9#9'Reporting.ProductionTransactionView.ShiftDate BETWEEN @DateSta' +
        'rt AND @DateStop'
      #9#9'and'
      #9#9'('
      
        #9#9#9'Datepart(hour,Reporting.ProductionTransactionView.ShiftDate)B' +
        'etween 0 AND 5'
      #9#9')'
      #9#9'and'
      #9#9'Reporting.ProductionTransactionView.WorkCenter = '#39'Coater'#39
      'UNION ALL'
      #9'SELECT'
      
        #9#9'datepart(day,Reporting.ProductionTransactionView.ShiftDate)+ 1' +
        '  Dzien,'
      #9#9'Reporting.ProductionTransactionView.ShiftDate,'
      #9#9'Reporting.ProductionTransactionView.Shift  Zmiana,'
      
        #9#9'Reporting.ProductionTransactionView.Lites*AreaSqMPerLite as Ro' +
        'zm'
      #9'FROM'
      #9'('
      #9#9'Reporting.ProductionTransactionView'
      #9#9'JOIN'
      #9#9'Reporting.ProductViewNEW'
      #9#9'ON'
      
        #9#9'Reporting.ProductViewNEW.ProductViewOidValue = ProductionTrans' +
        'actionView.ProductOidValue'
      #9')'
      #9'WHERE'
      
        #9'Reporting.ProductionTransactionView.ShiftDate BETWEEN DATEADD(d' +
        'ay,1,@DateStart) AND @DateStop --DATEADD(day,1,@DateStop)'
      #9'and'
      #9'('
      
        #9#9'Datepart(hour,Reporting.ProductionTransactionView.ShiftDate) B' +
        'etween 22 AND 23'
      #9')'
      #9'and'
      #9'Reporting.ProductionTransactionView.WorkCenter = '#39'Coater'#39
      'UNION ALL'
      #9'SELECT'
      #9#9'1  Dzien,'
      #9#9'Reporting.ProductionTransactionView.ShiftDate,'
      #9#9'Reporting.ProductionTransactionView.Shift  Zmiana,'
      
        #9#9'Reporting.ProductionTransactionView.Lites*AreaSqMPerLite as Ro' +
        'zm --ProductionTransactionView.*, ProductViewNEW.*'
      #9'FROM'
      #9'('
      #9#9'Reporting.ProductionTransactionView'
      #9#9'JOIN'
      #9#9'Reporting.ProductViewNEW'
      #9#9'ON'
      
        #9#9'Reporting.ProductViewNEW.ProductViewOidValue = Reporting.Produ' +
        'ctionTransactionView.ProductOidValue'
      #9')'
      #9'WHERE'
      
        #9'Reporting.ProductionTransactionView.ShiftDate BETWEEN @DateStar' +
        't AND DATEADD(second,59,DATEADD(minute,119,@DateStart))'
      #9#9'and'
      #9'Reporting.ProductionTransactionView.WorkCenter = '#39'Coater'#39
      ')as Czesc3'
      ''
      'Group By Dzien'
      '        ,Zmiana'
      'Order By Dzien ASC')
    Left = 624
    Top = 808
  end
  object ADOQueryPacksID: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    SQL.Strings = (
      'SELECT IDENT_CURRENT('#39'PACKED'#39') AS LastIdentity')
    Left = 32
    Top = 136
  end
  object ADOConnectionGIPAlternative: TADOConnection
    ConnectionString = 
      'Provider=Microsoft.Jet.OLEDB.4.0;Data Source=C:\Users\jsolecki\D' +
      'ocuments\NowyRaportCoatera\Local\gip.mdb;Persist Security Info=F' +
      'alse'
    LoginPrompt = False
    Provider = 'Microsoft.Jet.OLEDB.4.0'
    Left = 24
    Top = 632
  end
  object ADOQueryGIPAlternative: TADOQuery
    Connection = ADOConnectionGIPAlternative
    Parameters = <>
    Left = 168
    Top = 632
  end
  object ADOQueryDefectsSummary: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'StartDate'
        Size = -1
        Value = Null
      end
      item
        Name = 'StopDate'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @StartDate as DateTime, @StopDate as DateTime'
      'SET @StartDate = :StartDate'
      'SET @StopDate = :StopDate'
      ''
      
        'SELECT  DefectType,DefectID, Sum(SumOfArea) Area, Sum((ROUND(CAS' +
        'T(Val1 as Int)*CAST(Val2 as Int)/1000000.0,2))*NumberOf) as Roun' +
        'dedArea FROM'
      '('
      
        #9#9'SELECT QueryDefectSum.Val1, QueryDefectSum.Val2,QueryDefectSum' +
        '.NumberOf, QueryDefectSum.DefectType AS DefectType, QueryDefectS' +
        'um.ID as DefectID, QueryDefectSum.Area AS SumOfArea, CP_HEAD.DOC' +
        'NUM as ID'
      
        #9#9'FROM QueryDefectSum INNER JOIN CP_HEAD ON QueryDefectSum.DocNu' +
        'm = CP_HEAD.DOCNUM'
      #9#9'WHERE ((Data)>@StartDate And (Data) <= @StopDate)'
      ') AS DEFECTY'
      'GROUP BY DefectType, DefectID')
    Left = 408
    Top = 576
  end
  object ADOQueryDefectsParam: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    SQL.Strings = (
      'SELECT'
      '       [ID]'
      '      ,[VALUE]'
      '      ,[ACTIVE]'
      'FROM [CP_PARAM_DEFECT_VIEW]')
    Left = 400
    Top = 632
  end
  object ADOQueryDefCategoryNames: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    SQL.Strings = (
      'SELECT [DEF_CAT_ID]'
      '      ,[DEF_CAT_NAME]'
      'FROM [CP_DEFECT_CATEGORIES_PARAM]')
    Left = 552
    Top = 576
  end
  object ADOQueryDefToCategory: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    SQL.Strings = (
      'SELECT  '
      '      [DEFECT_ID]'
      '      ,[DEFCAT_ID]'
      'FROM [CP_DEFECT_CATEGORIES]')
    Left = 552
    Top = 632
  end
  object ADOQueryTemp: TADOQuery
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    ParamCheck = False
    Parameters = <>
    Prepared = True
    SQL.Strings = (
      'DECLARE @SHIFT AS char(2)'
      'DECLARE @SUPERVISOR AS VARCHAR(50)'
      'DECLARE @PROCES AS VARCHAR(50)'
      'DECLARE @BOOTH AS VARCHAR(50)'
      'DECLARE @DAT AS DATE'
      'DECLARE @KIND AS VARCHAR(50)'
      'DECLARE @SNo AS CHAR(9)'
      'DECLARE @ShiftKind As VarChar(20)'
      'Set @SNo =:DOCNUM'
      ''
      'SET @SHIFT = (SELECT [SHIFT] FROM CP_HEAD  WHERE DOCNUM = @SNo)'
      
        'SET @SUPERVISOR = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS' +
        '] INNER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HE' +
        'AD].USER_SV'
      '                  WHERE [dbo].[CP_HEAD].DOCNUM =@SNo)'
      
        'SET @PROCES = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS] IN' +
        'NER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HEAD].' +
        'USER_CO'
      '                  WHERE [dbo].[CP_HEAD].DOCNUM = @SNo)'
      
        'SET @BOOTH = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS] INN' +
        'ER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HEAD].U' +
        'SER_DO'
      '                  WHERE [dbo].[CP_HEAD].DOCNUM = @SNo)'
      'SELECT @DAT = [DATA], @SHIFT = [SHIFT], @ShiftKind ='
      #9#9#9#9'CASE'
      #9#9#9#9#9'WHEN DATEPART(hour,[DATA]) BETWEEN 6 and 13  THEN '#39'DAY'#39
      
        #9#9#9#9#9'WHEN DATEPART(hour,[Data]) BETWEEN 14 and 21  THEN '#39'AFTERNO' +
        'ON'#39
      #9#9#9#9#9'WHEN DATEPART(hour,[DATA]) BETWEEN 0 and 5  THEN '#39'NIGHT'#39
      #9#9#9#9'END'
      ''
      'FROM CP_HEAD WHERE [dbo].[CP_HEAD].DOCNUM = @SNo'
      ''
      ''
      
        'SELECT  @SHIFT as SHIFT, @SUPERVISOR as SuperVisor ,@PROCES as P' +
        'roces, @BOOTH  AS Booth ,@Dat as StartDate,@SHIFTKIND as Kind')
    Left = 752
    Top = 16
  end
  object ADOQueryInsertPack: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'ID_Workshiftday'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_PROD_TYPE'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_SUBSTRATE'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_SUBSTRATECLASS'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_THICKNESS'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_SIZE'
        Size = -1
        Value = Null
      end
      item
        Name = 'NO_PACKS'
        Size = -1
        Value = Null
      end
      item
        Name = 'NO_PANES'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'INSERT INTO '
      
        'PACKED (ID_Workshiftday, ID_PROD_TYPE,[ID_SUBSTRATE],[ID_SUBSTRA' +
        'TECLASS],[ID_THICKNESS], [ID_SIZE], [NO_PACKS],[NO_PANES])   '
      'VALUES'
      
        '(:ID_Workshiftday, :ID_PROD_TYPE,:ID_SUBSTRATE, :ID_SUBSTRATECLA' +
        'SS,:ID_THICKNESS, :ID_SIZE, :NO_PACKS, :NO_PANES)')
    Left = 432
    Top = 136
  end
  object ADOQueryPacksWorkProdID: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'ID_Prod_Type'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_Workshiftday'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'SELECT * '
      'FROM [dbo].[PACKED]'
      
        'WHERE [ID_PROD_TYPE] = :ID_Prod_Type AND [ID_Workshiftday] = :ID' +
        '_Workshiftday'
      'ORDER BY ID')
    Left = 160
    Top = 136
  end
  object ADOQueryPacksInputGlass: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'ID_Prod_Type'
        Size = -1
        Value = Null
      end
      item
        Name = 'STR_DOCNUM'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'SELECT * '
      'FROM CP_PARAM_DEFECT_EXPANDED_VIEW'
      'WHERE [ID_PRODTYPE] = :ID_Prod_Type AND [DOCNUM]= :STR_DOCNUM'
      ''
      'ORDER BY [DATETIME]')
    Left = 304
    Top = 136
  end
  object ADOQueryUpdatePacks: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'ID_SUBSTRATE'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_SUBSTRATECLASS'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_THICKNESS'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID_SIZE'
        Size = -1
        Value = Null
      end
      item
        Name = 'NO_PACKS'
        Size = -1
        Value = Null
      end
      item
        Name = 'NO_PANES'
        Size = -1
        Value = Null
      end
      item
        Name = 'ID'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'UPDATE   PACKED '
      'SET'
      ''
      ''
      '[ID_SUBSTRATE] = :ID_SUBSTRATE,'
      '[ID_SUBSTRATECLASS]=  :ID_SUBSTRATECLASS,'
      '[ID_THICKNESS] = :ID_THICKNESS, '
      '[ID_SIZE] = :ID_SIZE, '
      '[NO_PACKS] =  :NO_PACKS,'
      '[NO_PANES]= :NO_PANES   '
      ''
      'WHERE ID = :ID;')
    Left = 576
    Top = 136
  end
  object ADOQueryDeletePack: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'ID'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DELETE  FROM  PACKED '
      'WHERE ID = :ID;')
    Left = 688
    Top = 136
  end
  object ADOQuery1: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'StartDate'
        Size = -1
        Value = Null
      end
      item
        Name = 'StopDate'
        Size = -1
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @StartDate as DateTime, @StopDate as DateTime'
      'SET @StartDate = :StartDate  '
      'SET @StopDate = :StopDate'
      ''
      'SELECT  DefectType,DefectID, Sum(SumOfArea) Area  FROM'
      '('
      
        #9#9'SELECT QueryDefectSum.DefectType AS DefectType, QueryDefectSum' +
        '.ID as DefectID, QueryDefectSum.Area AS SumOfArea, CP_HEAD.DOCNU' +
        'M as ID '
      
        #9#9'FROM QueryDefectSum INNER JOIN CP_HEAD ON QueryDefectSum.DocNu' +
        'm = CP_HEAD.DOCNUM '
      #9#9'WHERE ((Data)>@StartDate And (Data) <= @StopDate)'
      ') AS DEFECTY'
      'GROUP BY DefectType, DefectID'
      '')
    Left = 280
    Top = 632
  end
  object ADOQueryHeader: TADOQuery
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    Parameters = <
      item
        Name = 'DOCN'
        DataType = ftWideString
        Size = 10
        Value = 'PC012077'
      end>
    Prepared = True
    SQL.Strings = (
      'DECLARE @SHIFT AS char(2)'
      'DECLARE @SUPERVISOR AS NVARCHAR(50)'
      'DECLARE @PROCES AS NVARCHAR(50)'
      'DECLARE @BOOTH AS NVARCHAR(50)'
      'DECLARE @DAT AS DATE'
      'DECLARE @KIND AS NVARCHAR(50)'
      'DECLARE @SNo AS CHAR(9)'
      'DECLARE @ShiftKind As NVarChar(20)'
      'DECLARE @NoOperators AS INTEGER'
      ' Set @SNo = :DOCN'
      ''
      '-- Set @SNo='#39'PC012074'#39
      ''
      'SET @SHIFT = (SELECT [SHIFT] FROM CP_HEAD  WHERE DOCNUM = @SNo'
      ')'
      
        'SET @SUPERVISOR = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS' +
        '] INNER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HE' +
        'AD].USER_SV'
      '                  WHERE [dbo].[CP_HEAD].DOCNUM =@SNo)'
      
        'SET @PROCES = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS] IN' +
        'NER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HEAD].' +
        'USER_CO'
      '                  WHERE [dbo].[CP_HEAD].DOCNUM = @SNo)'
      
        'SET @BOOTH = (SELECT [dbo].[USERS].[NAME] FROM [dbo].[USERS] INN' +
        'ER JOIN [dbo].[CP_HEAD] ON [dbo].[USERS].NUM = [dbo].[CP_HEAD].U' +
        'SER_DO'
      '                  WHERE [dbo].[CP_HEAD].DOCNUM = @SNo)'
      'SELECT   @DAT = [DATA], @SHIFT = [SHIFT], @ShiftKind ='
      #9#9#9#9'CASE'
      #9#9#9#9#9'WHEN DATEPART(hour,[DATA]) BETWEEN 6 and 13  THEN '#39'DAY'#39
      
        #9#9#9#9#9'WHEN DATEPART(hour,[Data]) BETWEEN 14 and 21  THEN '#39'AFTERNO' +
        'ON'#39
      #9#9#9#9#9'WHEN DATEPART(hour,[DATA]) BETWEEN 0 and 5  THEN '#39'NIGHT'#39
      #9#9#9#9'END'
      ''
      'FROM CP_HEAD WHERE [dbo].[CP_HEAD].DOCNUM = @SNo'
      ''
      ''
      
        'SELECT  @SHIFT as SHIFT, @SUPERVISOR as SuperVisor ,@PROCES as P' +
        'roces, @BOOTH  AS Booth ,@Dat as StartDate,@SHIFTKIND as Kind')
    Left = 56
    Top = 72
  end
  object ADOTableEventReport: TADOTable
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    TableName = 'EventReport'
    Left = 160
    Top = 72
  end
  object ADOQueryPacksView: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    Left = 280
    Top = 72
  end
  object ADODataSet1: TADODataSet
    Connection = ADOConnectionCreport
    Parameters = <>
    Left = 400
    Top = 72
  end
  object ADOTablePacksView: TADOTable
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    IndexFieldNames = 'ID_Workshiftday;ID_PROD_TYPE'
    MasterFields = 'ID_WORKSHIFTDAY;ID_PROD_TYPE'
    MasterSource = DataSourceProdPacks
    TableName = 'PACKS_VIEW'
    Left = 520
    Top = 72
  end
  object ADOTableProd: TADOTable
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    Filter = 'ID_WORKSHIFTDAY = 5650'
    Filtered = True
    TableName = 'PROD_SUMMARY Query'
    Left = 616
    Top = 72
  end
  object DataSourceProdPacks: TDataSource
    DataSet = ADOTableProd
    Left = 712
    Top = 72
  end
  object frxReport1: TfrxReport
    Version = '4.12.2'
    DotMatrixReport = False
    IniFile = '\Software\Fast Reports'
    PreviewOptions.Buttons = [pbPrint, pbLoad, pbSave, pbExport, pbZoom, pbFind, pbOutline, pbPageSetup, pbTools, pbEdit, pbNavigator, pbExportQuick]
    PreviewOptions.Zoom = 1.000000000000000000
    PrintOptions.Printer = 'Default'
    PrintOptions.PrintOnSheet = 0
    ReportOptions.CreateDate = 41348.630751655100000000
    ReportOptions.LastChange = 42365.940756643490000000
    ScriptLanguage = 'PascalScript'
    ScriptText.Strings = (
      'begin'
      ''
      'end.')
    OnGetValue = frxReport1GetValue
    Left = 24
    Top = 272
    Datasets = <
      item
        DataSet = frxUserDataSetCathodes
        DataSetName = 'Cathodes'
      end
      item
        DataSet = frxDBDatasetDefects
        DataSetName = 'Defects'
      end
      item
        DataSet = frxDBDataset1
        DataSetName = 'frxDBDataset1'
      end
      item
        DataSet = frxDBDataset2
        DataSetName = 'frxDBDataset2'
      end
      item
        DataSet = frxDBDataseWorkshiftRemarks
        DataSetName = 'GeneralRemarks'
      end
      item
        DataSet = frxUserDataSetMeasurements
        DataSetName = 'Measurements'
      end
      item
        DataSet = frxUserDataSetOEE
        DataSetName = 'OEE'
      end
      item
        DataSet = frxDBDatasetPacksView
        DataSetName = 'PacksView'
      end
      item
        DataSet = frxDBDatasetProduction
        DataSetName = 'Production'
      end
      item
        DataSet = frxDBDatasetWorkshiftdays
        DataSetName = 'Workshiftdays'
      end>
    Variables = <>
    Style = <>
    object Data: TfrxDataPage
      Height = 1000.000000000000000000
      Width = 1000.000000000000000000
    end
    object Page1: TfrxReportPage
      PaperWidth = 210.000000000000000000
      PaperHeight = 297.000000000000000000
      PaperSize = 9
      LeftMargin = 10.000000000000000000
      RightMargin = 10.000000000000000000
      TopMargin = 10.000000000000000000
      BottomMargin = 10.000000000000000000
      LargeDesignHeight = True
      object PageFooter1: TfrxPageFooter
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Height = 22.677180000000000000
        ParentFont = False
        Top = 1122.520410000000000000
        Width = 718.110700000000000000
        object SysMemo3: TfrxSysMemoView
          Left = 551.811380000000000000
          Width = 136.063080000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Strona [PAGE#] z [TOTALPAGES#]')
          ParentFont = False
        end
        object Memo2: TfrxMemoView
          Left = 22.677180000000000000
          Width = 94.488250000000000000
          Height = 26.456710000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Zmiana: [frxDBDataset1."SHIFT"]')
          ParentFont = False
        end
        object Memo4: TfrxMemoView
          Left = 132.283550000000000000
          Width = 234.330860000000000000
          Height = 26.456710000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Mistrz: [frxDBDataset1."SuperVisor"]')
          ParentFont = False
        end
        object Memo5: TfrxMemoView
          Left = 393.071120000000000000
          Width = 45.354360000000000000
          Height = 26.456710000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Data: ')
          ParentFont = False
        end
        object frxDBDataset1StartDate: TfrxMemoView
          Left = 453.543600000000000000
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxDBDataset1
          DataSetName = 'frxDBDataset1'
          DisplayFormat.FormatStr = 'dd.mm.yyyy'
          DisplayFormat.Kind = fkDateTime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."StartDate"]')
          ParentFont = False
        end
      end
      object ReportTitle1: TfrxReportTitle
        Height = 60.256410250000000000
        Top = 18.897650000000000000
        Width = 718.110700000000000000
        Child = frxReport1.Child1
        object Memo1: TfrxMemoView
          Left = 1.082179740000000000
          Top = 3.249336870000001000
          Width = 324.323192680000000000
          Height = 26.456710000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Shift Report / Raport Zmianowy')
          ParentFont = False
        end
        object SysMemo1: TfrxSysMemoView
          Left = 601.155109300000000000
          Width = 74.267780000000000000
          Height = 19.015770000000000000
          ShowHint = False
          DisplayFormat.FormatStr = 'dd.mm.yyyy'
          DisplayFormat.Kind = fkDateTime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[DATE]')
          ParentFont = False
        end
        object SysMemo2: TfrxSysMemoView
          Left = 675.478281380000000000
          Width = 69.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DisplayFormat.FormatStr = 'HH:mm'
          DisplayFormat.Kind = fkDateTime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[TIME]')
          ParentFont = False
        end
        object Memo117: TfrxMemoView
          Left = 188.461538460000000000
          Top = 31.102350000000000000
          Width = 136.154916660000000000
          Height = 24.025855130000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'OEE = [OEE]')
          ParentFont = False
        end
        object Memo118: TfrxMemoView
          Left = 377.564102570000000000
          Top = 0.974144870000000000
          Width = 57.949788470000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Uptime =')
          ParentFont = False
        end
        object Memo119: TfrxMemoView
          Left = 377.564102570000000000
          Top = 20.204914100000000000
          Width = 57.949788460000000000
          Height = 17.615598720000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Yield =')
          ParentFont = False
        end
        object Memo120: TfrxMemoView
          Left = 480.769230770000000000
          Top = 0.974144870000000000
          Width = 105.385685900000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Uptime]')
          ParentFont = False
        end
        object Memo121: TfrxMemoView
          Left = 480.769230770000000000
          Top = 20.204914100000000000
          Width = 105.385685900000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Yield]')
          ParentFont = False
        end
        object Memo122: TfrxMemoView
          Left = 377.564102570000000000
          Top = 38.153632050000000000
          Width = 91.924147440000000000
          Height = 17.615598720000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Performance =')
          ParentFont = False
        end
        object Memo123: TfrxMemoView
          Left = 480.769230770000000000
          Top = 38.153632050000000000
          Width = 105.385685900000000000
          Height = 17.615598720000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Performance]')
          ParentFont = False
        end
        object Line1: TfrxLineView
          Left = 4.487179480000000000
          Top = 58.025426920000000000
          Width = 710.256410260000000000
          ShowHint = False
          Diagonal = True
        end
        object Memo124: TfrxMemoView
          Left = 1.282051280000000000
          Top = 30.461324360000000000
          Width = 184.231839740000000000
          Height = 24.025855130000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -19
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Monthly OEE = [MonthOEE]')
          ParentFont = False
        end
      end
      object Child1: TfrxChild
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        Height = 60.999253790000000000
        ParentFont = False
        Top = 102.047310000000000000
        Width = 718.110700000000000000
        object Memo6: TfrxMemoView
          Left = 303.579969320000000000
          Width = 41.574830000000000000
          Height = 22.677165354330700000
          ShowHint = False
          StretchMode = smActualHeight
          AutoWidth = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Shift: ')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo7: TfrxMemoView
          Left = 349.596837590000000000
          Width = 34.143422410000000000
          Height = 22.677165350000000000
          ShowHint = False
          AutoWidth = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."SHIFT"]')
          ParentFont = False
        end
        object Memo8: TfrxMemoView
          Left = 387.256403100000000000
          Width = 43.494487590000000000
          Height = 22.677165354330700000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Date: ')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo9: TfrxMemoView
          Left = 431.413398960000000000
          Width = 121.212387930000000000
          Height = 22.677165350000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."StartDate"]')
          ParentFont = False
        end
        object Memo11: TfrxMemoView
          Left = 9.614452070000000000
          Top = 30.236220469999990000
          Width = 132.283550000000000000
          Height = 22.677180000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Process op.:')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo12: TfrxMemoView
          Left = 100.913385830000000000
          Top = 30.236220469999990000
          Width = 197.397628960000000000
          Height = 22.677180000000000000
          ShowHint = False
          StretchMode = smActualHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsItalic]
          Memo.UTF8W = (
            '[frxDBDataset1."Proces"]')
          ParentFont = False
          WordWrap = False
        end
        object Memo13: TfrxMemoView
          Left = 303.220427940000000000
          Top = 30.236220469999990000
          Width = 78.372326550000000000
          Height = 22.677180000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Booth op.:')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo14: TfrxMemoView
          Left = 386.896861730000000000
          Top = 30.236220469999990000
          Width = 167.620295520000000000
          Height = 22.677180000000000000
          ShowHint = False
          StretchMode = smActualHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsItalic]
          Memo.UTF8W = (
            '[frxDBDataset1."Booth"]')
          ParentFont = False
          WordWrap = False
        end
        object Memo3: TfrxMemoView
          Left = 577.358358960000000000
          Width = 45.749440340000000000
          Height = 22.677165350000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Kind:')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo16: TfrxMemoView
          Left = 623.570746900000000000
          Width = 90.177905170000000000
          Height = 22.677165350000000000
          ShowHint = False
          AutoWidth = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."Kind"]')
          ParentFont = False
        end
        object Memo15: TfrxMemoView
          Left = 100.985680340000000000
          Width = 198.590952070000000000
          Height = 22.677165350000000000
          ShowHint = False
          StretchMode = smActualHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."SuperVisor"]')
          ParentFont = False
          WordWrap = False
        end
        object Memo10: TfrxMemoView
          Left = 9.614452080000000000
          Width = 85.536306210000000000
          Height = 22.677165350000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Supervisor:')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo51: TfrxMemoView
          Left = 577.511811023622000000
          Top = 29.450120340000000000
          Width = 66.039974130000000000
          Height = 22.345925860000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'No. Op.:')
          ParentFont = False
          VAlign = vaBottom
        end
        object WorkshiftdaysOPERATORS: TfrxMemoView
          Left = 647.413793110000000000
          Top = 29.450120340000000000
          Width = 65.577026550000000000
          Height = 22.345925860000000000
          ShowHint = False
          DataField = 'OPERATORS'
          DataSet = frxDBDatasetWorkshiftdays
          DataSetName = 'Workshiftdays'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsItalic]
          Memo.UTF8W = (
            '[Workshiftdays."OPERATORS"]')
          ParentFont = False
          VAlign = vaCenter
        end
      end
      object MasterData1: TfrxMasterData
        Height = 19.397650000000000000
        Top = 294.803340000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDataset2
        DataSetName = 'frxDBDataset2'
        RowCount = 0
        Stretched = True
        object frxDBDataset2Product: TfrxMemoView
          Left = 277.544936200000000000
          Width = 65.246870690000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Product'
          DataSet = frxDBDataset2
          DataSetName = 'frxDBDataset2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Product"]')
          ParentFont = False
          WordWrap = False
        end
        object frxDBDataset2Start: TfrxMemoView
          Left = -0.230044490000000000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataSet = frxDBDataset2
          DataSetName = 'frxDBDataset2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Start"]')
          ParentFont = False
        end
        object frxDBDataset2Comment: TfrxMemoView
          Left = 419.854737980000000000
          Width = 317.395058380000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Comment'
          DataSet = frxDBDataset2
          DataSetName = 'frxDBDataset2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftRight, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Comment"]')
          ParentFont = False
          WordBreak = True
        end
        object frxDBDataset2Stop: TfrxMemoView
          Left = 41.303285360000000000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataSet = frxDBDataset2
          DataSetName = 'frxDBDataset2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Stop"]')
          ParentFont = False
        end
        object frxDBDataset2Time: TfrxMemoView
          Left = 81.744230240000000000
          Width = 39.913420000000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Time'
          DataSet = frxDBDataset2
          DataSetName = 'frxDBDataset2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Time"]')
          ParentFont = False
        end
        object Memo26: TfrxMemoView
          Left = 344.264186800000000000
          Top = 0.046433799999988420
          Width = 75.590551180000000000
          Height = 18.897637800000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Additional"]')
          ParentFont = False
        end
        object frxDBDataset2Event: TfrxMemoView
          Left = 122.456964420000000000
          Width = 154.098661030000000000
          Height = 18.633483980000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Event'
          DataSet = frxDBDataset2
          DataSetName = 'frxDBDataset2'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Event"]')
          ParentFont = False
        end
      end
      object Header1: TfrxHeader
        Height = 47.936525860000000000
        Top = 222.992270000000000000
        Width = 718.110700000000000000
        Stretched = True
        object Memo25: TfrxMemoView
          Left = 15.000000000000000000
          Top = 2.551019999999994000
          Width = 688.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haCenter
          Memo.UTF8W = (
            'Line events / wydarzenia na linii')
          ParentFont = False
        end
        object Memo19: TfrxMemoView
          Left = 123.319033390000000000
          Top = 29.038875859999990000
          Width = 154.098661034828000000
          Height = 18.633483975042600000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Event')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo20: TfrxMemoView
          Left = -1.092113450000000000
          Top = 29.038875859999990000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Start'
            '(h:m)')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo21: TfrxMemoView
          Left = 41.303285360000000000
          Top = 29.038875859999990000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Stop'
            '(h:m)')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo22: TfrxMemoView
          Left = 81.744230240000000000
          Top = 29.038875859999990000
          Width = 39.913420000000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Time'
            '(min)')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo23: TfrxMemoView
          Left = 277.544936200000000000
          Top = 29.038875859999990000
          Width = 65.246870680000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Product')
          ParentFont = False
          WordWrap = False
          VAlign = vaCenter
        end
        object Memo24: TfrxMemoView
          Left = 419.854737980000000000
          Top = 29.038875859999990000
          Width = 317.381317360000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftRight, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Comment')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo27: TfrxMemoView
          Left = 344.264186800000000000
          Top = 29.038875859999990000
          Width = 75.590551180000000000
          Height = 18.897637800000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Add.')
          ParentFont = False
          VAlign = vaCenter
        end
      end
      object MasterData2: TfrxMasterData
        Height = 26.947944820000000000
        Top = 408.189240000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDatasetProduction
        DataSetName = 'Production'
        KeepFooter = True
        KeepHeader = True
        PrintIfDetailEmpty = True
        RowCount = 0
        object ProductionTOTAL_IN: TfrxMemoView
          Left = 459.482758640000000000
          Top = 3.779527560000020000
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxDBDatasetProduction
          DataSetName = 'Production'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Production."TOTAL_IN"]')
          ParentFont = False
        end
        object ProductionTOTAL_OUT: TfrxMemoView
          Left = 550.000000010000000000
          Top = 3.779527560000020000
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'TOTAL_OUT'
          DataSet = frxDBDatasetProduction
          DataSetName = 'Production'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Production."TOTAL_OUT"]')
          ParentFont = False
        end
        object ProductionYIELD: TfrxMemoView
          Left = 649.137931040000000000
          Top = 3.779527560000020000
          Width = 63.852888620000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxDBDatasetProduction
          DataSetName = 'Production'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Production."YIELD"]%')
          ParentFont = False
        end
        object ProductionFULLNAME: TfrxMemoView
          Left = 4.310344830000000000
          Top = 3.779527560000020000
          Width = 359.250869650000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'FULLNAME'
          DataSet = frxDBDatasetProduction
          DataSetName = 'Production'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Production."FULLNAME"]')
          ParentFont = False
        end
      end
      object DetailData1: TfrxDetailData
        Height = 20.820726920000000000
        Top = 536.693260000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDatasetPacksView
        DataSetName = 'PacksView'
        RowCount = 0
        object PacksViewThickness: TfrxMemoView
          Left = 132.758620690000000000
          Top = 0.219731789999968900
          Width = 73.043973100000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Thickness'
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          Memo.UTF8W = (
            '[PacksView."Thickness"]')
        end
        object PacksViewNO_PACKS: TfrxMemoView
          Left = 406.984969050000000000
          Top = 0.219731789999968900
          Width = 55.232198970000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'NO_PACKS'
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          HAlign = haRight
          Memo.UTF8W = (
            '[PacksView."NO_PACKS"]')
          ParentFont = False
        end
        object PacksViewNO_PANES: TfrxMemoView
          Left = 479.398762160000000000
          Top = 0.219731789999968900
          Width = 76.783923110000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          Memo.UTF8W = (
            '[PacksView."NO_PANES"] lites')
        end
        object PacksViewArea: TfrxMemoView
          Left = 568.103448280000000000
          Top = 0.219731789999968900
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          DisplayFormat.FormatStr = '%2.2f'
          DisplayFormat.Kind = fkNumeric
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          HAlign = haRight
          Memo.UTF8W = (
            '[PacksView."Area"]')
          ParentFont = False
        end
        object Memo18: TfrxMemoView
          Left = 464.301503100000000000
          Width = 15.177905170000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          HAlign = haCenter
          Memo.UTF8W = (
            'x')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo114: TfrxMemoView
          Left = 316.025641030000000000
          Top = 0.219731789999968900
          Width = 89.312761780000000000
          Height = 18.553946580000000000
          ShowHint = False
          DataField = 'Type'
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          Memo.UTF8W = (
            '[PacksView."Type"]')
        end
        object PacksViewSize: TfrxMemoView
          Left = 215.716180380000000000
          Top = 0.219731789999968900
          Width = 95.723018190000000000
          Height = 17.912920940000000000
          ShowHint = False
          DataField = 'Size'
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          Memo.UTF8W = (
            '[PacksView."Size"]')
        end
        object Memo116: TfrxMemoView
          Left = 57.692307690000000000
          Top = 0.219731789999968900
          Width = 73.043973100000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          Memo.UTF8W = (
            '[PacksView."Substrate"]')
        end
      end
      object Header2: TfrxHeader
        Height = 50.223806900000000000
        Top = 336.378170000000000000
        Width = 718.110700000000000000
        object Memo28: TfrxMemoView
          Left = 6.034482750000000000
          Top = 30.464087930000010000
          Width = 151.384801720000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Product')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo29: TfrxMemoView
          Left = 458.620689650000000000
          Top = 30.464087930000010000
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Input')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo30: TfrxMemoView
          Left = 552.586206890000000000
          Top = 30.464087930000010000
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Output')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo31: TfrxMemoView
          Left = 650.862068960000000000
          Top = 30.464087930000010000
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Yield')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo32: TfrxMemoView
          Top = 4.602018970000017000
          Width = 719.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haCenter
          Memo.UTF8W = (
            'Production results:')
          ParentFont = False
          VAlign = vaCenter
        end
      end
      object Footer1: TfrxFooter
        Height = 19.600568620000000000
        Top = 457.323130000000000000
        Width = 718.110700000000000000
        object Memo35: TfrxMemoView
          Left = 654.310344820000000000
          Top = 0.702918619999991300
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DisplayFormat.FormatStr = '%2.2f'
          DisplayFormat.Kind = fkNumeric
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            
              '[(SUM(<Production."TOTAL_OUT">,MasterData2)/SUM(<Production."TOT' +
              'AL_IN">,MasterData2))*100]%')
          ParentFont = False
        end
        object Memo36: TfrxMemoView
          Left = 6.034482760000000000
          Top = 0.702918619999991300
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Total:')
          ParentFont = False
        end
        object Memo33: TfrxMemoView
          Left = 458.620689660000000000
          Top = 0.367623449999996400
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[SUM(<Production."TOTAL_IN">,MasterData2)]')
          ParentFont = False
        end
        object Memo34: TfrxMemoView
          Left = 552.586206890000000000
          Top = 0.367623449999996400
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[SUM(<Production."TOTAL_OUT">,MasterData2)]')
          ParentFont = False
        end
      end
      object ReportSummary1: TfrxReportSummary
        Height = 67.289871730000000000
        Top = 1031.811690000000000000
        Width = 718.110700000000000000
        object Memo37: TfrxMemoView
          Left = 225.000000000000000000
          Top = 48.392221730000080000
          Width = 239.735818520000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            
              'Supervisor'#39's signature (Podpis mistrza): _______________________' +
              '___________________________')
          ParentFont = False
        end
        object Memo55: TfrxMemoView
          Left = 466.025641030000000000
          Top = 48.392221730000080000
          Width = 248.334403850000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Style = fsDot
          Frame.Typ = [ftBottom]
          ParentFont = False
        end
      end
      object MasterDataDefects: TfrxMasterData
        Height = 22.677180000000000000
        Top = 676.535870000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDatasetDefects
        DataSetName = 'Defects'
        KeepFooter = True
        KeepHeader = True
        RowCount = 0
        object DefectsDefect: TfrxMemoView
          Left = 4.310344830000000000
          Top = 0.184226889999990800
          Width = 111.814972210000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Defect'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."Defect"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object DefectsSize: TfrxMemoView
          Left = 224.071618050000000000
          Top = 0.825252539999951200
          Width = 99.768111030000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Size'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."Size"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object DefectsThickness: TfrxMemoView
          Left = 326.679929260000000000
          Top = 0.184226899999998800
          Width = 43.733628270000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Thickness'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."Thickness"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object DefectsSubstrateOrigin: TfrxMemoView
          Left = 373.253757730000000000
          Top = 0.184226899999998800
          Width = 56.129339940000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Substrate Origin'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."Substrate Origin"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object DefectsNumber: TfrxMemoView
          Left = 491.114058360000000000
          Top = 0.184226889999990800
          Width = 31.094267930000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Number'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."Number"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object DefectsArea: TfrxMemoView
          Left = 540.053050400000000000
          Top = 0.184226889999990800
          Width = 51.584984110000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Area'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."Area"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo108: TfrxMemoView
          Left = 430.128205130000000000
          Top = 0.054087689999960280
          Width = 56.735293570000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataField = 'Substrate Type'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."Substrate Type"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo110: TfrxMemoView
          Left = 595.512820510000000000
          Top = 0.054087689999960280
          Width = 118.847224360000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[Defects."COMMENT"]')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo112: TfrxMemoView
          Left = 123.717948720000000000
          Top = 0.695113330000026500
          Width = 99.616455120000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = []
          HAlign = haRight
          Memo.UTF8W = (
            '[Defects."EngAddDefect"]')
          ParentFont = False
          WordWrap = False
          VAlign = vaBottom
        end
      end
      object HeaderDefects: TfrxHeader
        Height = 70.953042070000000000
        Top = 582.047620000000000000
        Width = 718.110700000000000000
        object Memo17: TfrxMemoView
          Left = 4.310344820000000000
          Top = 24.118556209999950000
          Width = 712.591698280000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haCenter
          Memo.UTF8W = (
            'Defects:')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo38: TfrxMemoView
          Left = 4.310344830000000000
          Top = 49.980625170000050000
          Width = 110.425473700000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Defect kind')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo39: TfrxMemoView
          Left = 225.795755970000000000
          Top = 50.621650810000010000
          Width = 98.798594830000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Size')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo40: TfrxMemoView
          Left = 326.679929260000000000
          Top = 50.842694140000050000
          Width = 41.902043100000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Thick.')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo41: TfrxMemoView
          Left = 370.446507510000000000
          Top = 52.124745419999960000
          Width = 54.766764600000000000
          Height = 17.615598710000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Origin')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo42: TfrxMemoView
          Left = 489.323607430000000000
          Top = 50.842694140000050000
          Width = 44.930336660000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haCenter
          Memo.UTF8W = (
            'Quantity')
          ParentFont = False
          WordWrap = False
          VAlign = vaBottom
        end
        object Memo43: TfrxMemoView
          Left = 538.129973480000000000
          Top = 50.621650810000010000
          Width = 48.798594830000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Area')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo107: TfrxMemoView
          Left = 427.564102570000000000
          Top = 50.842694140000050000
          Width = 59.872865390000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Type')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo109: TfrxMemoView
          Left = 590.384615380000000000
          Top = 50.311568459999990000
          Width = 122.693378210000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Comment')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo111: TfrxMemoView
          Left = 123.717948710000000000
          Top = 49.670542820000040000
          Width = 98.975429490000000000
          Height = 19.538675640000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Additional')
          ParentFont = False
          VAlign = vaBottom
        end
      end
      object FooterDetail: TfrxFooter
        Height = 22.677180000000000000
        Top = 721.890230000000000000
        Width = 718.110700000000000000
        object Memo44: TfrxMemoView
          Left = 6.896551720000000000
          Top = 0.655256549999990000
          Width = 287.591698270000000000
          Height = 18.897650000000000000
          ShowHint = False
          DisplayFormat.FormatStr = '%2.2f'
          DisplayFormat.Kind = fkNumeric
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            
              'Production (In - Out) = [SUM(<Production."TOTAL_IN">,MasterData2' +
              ')-SUM(<Production."TOTAL_OUT">,MasterData2)]')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo45: TfrxMemoView
          Left = 485.344827590000000000
          Top = 0.655256549999990000
          Width = 94.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[SUM(<Defects."Area">,MasterDataDefects)]')
          ParentFont = False
          VAlign = vaBottom
        end
        object Memo46: TfrxMemoView
          Left = 354.310344830000000000
          Top = 0.655256549999990000
          Width = 123.798594830000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haRight
          Memo.UTF8W = (
            'Defects total area:')
          ParentFont = False
          VAlign = vaBottom
        end
      end
      object Header3: TfrxHeader
        Height = 16.642697240000000000
        Top = 498.897960000000000000
        Width = 718.110700000000000000
        object Memo47: TfrxMemoView
          Left = 134.482758620000000000
          Top = 1.900282759999982000
          Width = 70.350318970000000000
          Height = 12.863167240000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Thickness')
          ParentFont = False
        end
        object Memo48: TfrxMemoView
          Left = 213.793103440000000000
          Top = 1.900282759999982000
          Width = 95.311640180000000000
          Height = 13.065547190000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Size')
          ParentFont = False
        end
        object Memo49: TfrxMemoView
          Left = 406.984969050000000000
          Top = 1.900282759999982000
          Width = 149.660663790000000000
          Height = 12.863167240000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haCenter
          Memo.UTF8W = (
            'Packaging')
          ParentFont = False
        end
        object Memo50: TfrxMemoView
          Left = 566.379310340000000000
          Top = 1.900282759999982000
          Width = 80.695146550000000000
          Height = 12.863167240000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haRight
          Memo.UTF8W = (
            'Area')
          ParentFont = False
        end
        object Memo113: TfrxMemoView
          Left = 316.025641030000000000
          Top = 2.117595900000026000
          Width = 90.183435050000000000
          Height = 13.065547190000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Type')
          ParentFont = False
        end
        object Memo115: TfrxMemoView
          Left = 57.692307690000000000
          Top = 2.117595900000026000
          Width = 70.350318970000000000
          Height = 12.863167240000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -11
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Substrate')
          ParentFont = False
        end
      end
      object MasterData3: TfrxMasterData
        Height = 18.897650000000000000
        Top = 820.158010000000000000
        Width = 718.110700000000000000
        Columns = 3
        ColumnWidth = 226.771653543307000000
        ColumnGap = 12.472440944881900000
        DataSet = frxUserDataSetCathodes
        DataSetName = 'Cathodes'
        KeepFooter = True
        KeepHeader = True
        RowCount = 0
        object CathodesCathNames: TfrxMemoView
          Left = 18.965517240000000000
          Width = 70.749440340000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxUserDataSetCathodes
          DataSetName = 'Cathodes'
          Memo.UTF8W = (
            '[SwitchedCath]')
        end
        object CathodesNoSwitchedCath: TfrxMemoView
          Left = 114.655172410000000000
          Width = 110.404612760000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxUserDataSetCathodes
          DataSetName = 'Cathodes'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftRight]
          Memo.UTF8W = (
            '[NoSwitchedCath]')
          ParentFont = False
        end
      end
      object HeaderCathodes: TfrxHeader
        Height = 28.711662760000000000
        Top = 767.244590000000000000
        Width = 718.110700000000000000
        object Memo52: TfrxMemoView
          Left = 18.965517240000000000
          Top = 6.773770689999992000
          Width = 685.867560340000000000
          Height = 21.483856900000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftTop]
          Memo.UTF8W = (
            'Switched off cathodes:')
          ParentFont = False
          VAlign = vaBottom
        end
      end
      object FooterCathodes: TfrxFooter
        Height = 22.292564610000000000
        Top = 861.732840000000000000
        Width = 718.110700000000000000
        object Memo53: TfrxMemoView
          Left = 26.724137930000000000
          Top = 0.547809310000047900
          Width = 676.384801720000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smActualHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftBottom]
          Memo.UTF8W = (
            '[CathComment]')
          ParentFont = False
        end
      end
      object MasterData4: TfrxMasterData
        Height = 65.357712050000000000
        Top = 907.087200000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDataseWorkshiftRemarks
        DataSetName = 'GeneralRemarks'
        PrintIfDetailEmpty = True
        RowCount = 1
        Stretched = True
        object Memo54: TfrxMemoView
          Left = 3.846153850000000000
          Top = 1.588267180000003000
          Width = 713.719019230000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Style = fsDouble
          Frame.Typ = [ftLeft, ftRight, ftTop]
          Frame.Width = 1.500000000000000000
          Memo.UTF8W = (
            'GENERAL REMARKS / UWAGI OG'#211'LNE')
          ParentFont = False
        end
        object Memo106: TfrxMemoView
          Left = 3.846153850000000000
          Top = 18.895959490000000000
          Width = 713.719019230000000000
          Height = 41.333547430000000000
          ShowHint = False
          StretchMode = smActualHeight
          DataField = 'Remarks'
          DataSet = frxDBDataseWorkshiftRemarks
          DataSetName = 'GeneralRemarks'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftRight, ftBottom]
          Memo.UTF8W = (
            '[GeneralRemarks."Remarks"]')
          ParentFont = False
        end
      end
    end
  end
  object frxUserDataSetCathodes: TfrxUserDataSet
    RangeEnd = reCount
    UserName = 'Cathodes'
    Left = 200
    Top = 328
  end
  object frxDBDataset1: TfrxDBDataset
    UserName = 'frxDBDataset1'
    CloseDataSource = False
    DataSet = ADOQueryHeader
    BCDToCurrency = False
    Left = 104
    Top = 272
  end
  object frxDBDataset2: TfrxDBDataset
    UserName = 'frxDBDataset2'
    CloseDataSource = False
    DataSet = ADOTableEventReport
    BCDToCurrency = False
    Left = 200
    Top = 272
  end
  object frxDBDatasetProduction: TfrxDBDataset
    UserName = 'Production'
    CloseDataSource = False
    DataSet = ADOTableProd
    BCDToCurrency = False
    Left = 448
    Top = 272
  end
  object frxReport2: TfrxReport
    Version = '4.12.2'
    DotMatrixReport = False
    IniFile = '\Software\Fast Reports'
    PreviewOptions.Buttons = [pbPrint, pbLoad, pbSave, pbExport, pbZoom, pbFind, pbOutline, pbPageSetup, pbTools, pbEdit, pbNavigator, pbExportQuick]
    PreviewOptions.Zoom = 1.000000000000000000
    PrintOptions.Printer = 'Default'
    PrintOptions.PrintOnSheet = 0
    ReportOptions.CreateDate = 41348.630751655100000000
    ReportOptions.LastChange = 42233.792958333310000000
    ScriptLanguage = 'PascalScript'
    ScriptText.Strings = (
      'begin'
      ''
      'end.')
    Left = 24
    Top = 328
    Datasets = <
      item
        DataSet = frxDBDatasetPacksView
        DataSetName = 'PacksView'
      end
      item
        DataSet = frxDBDatasetDefects
        DataSetName = 'Defects'
      end
      item
        DataSet = frxUserDataSetMeasurements
        DataSetName = 'Measurements'
      end>
    Variables = <>
    Style = <>
    object Data: TfrxDataPage
      Height = 1000.000000000000000000
      Width = 1000.000000000000000000
    end
    object Page1: TfrxReportPage
      PaperWidth = 210.000000000000000000
      PaperHeight = 297.000000000000000000
      PaperSize = 9
      LeftMargin = 10.000000000000000000
      RightMargin = 10.000000000000000000
      TopMargin = 10.000000000000000000
      BottomMargin = 10.000000000000000000
      object PageFooter1: TfrxPageFooter
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -13
        Font.Name = 'Arial'
        Font.Style = [fsBold]
        Height = 22.677180000000000000
        ParentFont = False
        Top = 404.409710000000000000
        Width = 718.110700000000000000
        object SysMemo3: TfrxSysMemoView
          Left = 551.811380000000000000
          Width = 136.063080000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Strona [PAGE#] z [TOTALPAGES#]')
          ParentFont = False
        end
        object Memo2: TfrxMemoView
          Left = 22.677180000000000000
          Width = 94.488250000000000000
          Height = 26.456710000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Zmiana: [frxDBDataset1."SHIFT"]')
          ParentFont = False
        end
        object Memo4: TfrxMemoView
          Left = 132.283550000000000000
          Width = 234.330860000000000000
          Height = 26.456710000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Mistrz: [frxDBDataset1."SuperVisor"]')
          ParentFont = False
        end
        object Memo5: TfrxMemoView
          Left = 393.071120000000000000
          Width = 45.354360000000000000
          Height = 26.456710000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Data: ')
          ParentFont = False
        end
        object frxDBDataset1StartDate: TfrxMemoView
          Left = 453.543600000000000000
          Width = 79.370130000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DataSet = frxDBDatasetPacksView
          DataSetName = 'PacksView'
          DisplayFormat.FormatStr = 'dd.mm.yyyy'
          DisplayFormat.Kind = fkDateTime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."StartDate"]')
          ParentFont = False
        end
      end
      object ReportTitle1: TfrxReportTitle
        Height = 45.885174830000000000
        Top = 18.897650000000000000
        Width = 718.110700000000000000
        Child = frxReport1.FooterCathodes
        object Memo1: TfrxMemoView
          Left = 11.338590000000000000
          Top = 5.172413790000000000
          Width = 183.540699310000000000
          Height = 26.456710000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -21
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            'Raport Zmianowy')
          ParentFont = False
        end
        object SysMemo1: TfrxSysMemoView
          Left = 601.155109300000000000
          Width = 74.267780000000000000
          Height = 19.015770000000000000
          ShowHint = False
          DisplayFormat.FormatStr = 'dd.mm.yyyy'
          DisplayFormat.Kind = fkDateTime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[DATE]')
          ParentFont = False
        end
        object SysMemo2: TfrxSysMemoView
          Left = 675.478281380000000000
          Width = 69.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          DisplayFormat.FormatStr = 'HH:mm'
          DisplayFormat.Kind = fkDateTime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            '[TIME]')
          ParentFont = False
        end
      end
      object Child1: TfrxChild
        Font.Charset = DEFAULT_CHARSET
        Font.Color = clBlack
        Font.Height = -16
        Font.Name = 'Arial'
        Font.Style = []
        Height = 60.999253790000000000
        ParentFont = False
        Top = 124.724490000000000000
        Width = 718.110700000000000000
        object Memo6: TfrxMemoView
          Left = 303.579969320000000000
          Width = 41.574830000000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smActualHeight
          AutoWidth = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Shift: ')
          ParentFont = False
        end
        object Memo7: TfrxMemoView
          Left = 349.596837590000000000
          Width = 34.143422410000000000
          Height = 18.897650000000000000
          ShowHint = False
          AutoWidth = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."SHIFT"]')
          ParentFont = False
        end
        object Memo8: TfrxMemoView
          Left = 387.256403100000000000
          Width = 43.494487590000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Date: ')
          ParentFont = False
        end
        object Memo9: TfrxMemoView
          Left = 431.413398960000000000
          Width = 121.212387930000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."StartDate"]')
          ParentFont = False
        end
        object Memo11: TfrxMemoView
          Left = 9.614452070000000000
          Top = 29.968812060000000000
          Width = 132.283550000000000000
          Height = 22.677180000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Process op.:')
          ParentFont = False
        end
        object Memo12: TfrxMemoView
          Left = 100.913385830000000000
          Top = 29.968812060000000000
          Width = 197.397628960000000000
          Height = 22.677180000000000000
          ShowHint = False
          StretchMode = smActualHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsItalic]
          Memo.UTF8W = (
            '[frxDBDataset1."Proces"]')
          ParentFont = False
          WordWrap = False
        end
        object Memo13: TfrxMemoView
          Left = 303.220427940000000000
          Top = 29.968812070000010000
          Width = 78.372326550000000000
          Height = 22.677180000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Booth op.:')
          ParentFont = False
        end
        object Memo14: TfrxMemoView
          Left = 386.896861730000000000
          Top = 29.968812060000000000
          Width = 276.240985170000000000
          Height = 22.677180000000000000
          ShowHint = False
          StretchMode = smActualHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = '@Arial Unicode MS'
          Font.Style = [fsItalic]
          Memo.UTF8W = (
            '[frxDBDataset1."Booth"]')
          ParentFont = False
          WordWrap = False
        end
        object Memo3: TfrxMemoView
          Left = 554.944565860000000000
          Width = 45.749440340000000000
          Height = 18.897650000000000000
          ShowHint = False
          Memo.UTF8W = (
            'Kind:')
        end
        object Memo16: TfrxMemoView
          Left = 609.777643450000000000
          Width = 103.971008620000000000
          Height = 18.897650000000000000
          ShowHint = False
          AutoWidth = True
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."Kind"]')
          ParentFont = False
        end
        object Memo15: TfrxMemoView
          Left = 100.985680340000000000
          Top = 0.063826219999995710
          Width = 198.590952070000000000
          Height = 22.677180000000000000
          ShowHint = False
          StretchMode = smActualHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Memo.UTF8W = (
            '[frxDBDataset1."SuperVisor"]')
          ParentFont = False
          WordWrap = False
        end
        object Memo10: TfrxMemoView
          Left = 9.614452080000000000
          Top = 0.063826210000001980
          Width = 85.536306210000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = []
          Memo.UTF8W = (
            'Supervisor:')
          ParentFont = False
        end
      end
      object MasterData1: TfrxMasterData
        Height = 19.397650000000000000
        Top = 279.685220000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDatasetDefects
        DataSetName = 'Defects'
        RowCount = 0
        Stretched = True
        object frxDBDataset2EventDate: TfrxMemoView
          Width = 75.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          DisplayFormat.FormatStr = 'dd.mm.yy'
          DisplayFormat.Kind = fkDateTime
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."EventDate"]')
          ParentFont = False
        end
        object frxDBDataset2Product: TfrxMemoView
          Left = 337.889763780000000000
          Width = 58.350318970000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Product'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Product"]')
          ParentFont = False
          WordWrap = False
        end
        object frxDBDataset2Start: TfrxMemoView
          Left = 75.632024480000000000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Start"]')
          ParentFont = False
        end
        object frxDBDataset2Comment: TfrxMemoView
          Left = 472.440944881890000000
          Width = 264.808851480000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Comment'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftRight, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Comment"]')
          ParentFont = False
          WordBreak = True
        end
        object frxDBDataset2Stop: TfrxMemoView
          Left = 117.165354330000000000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Stop"]')
          ParentFont = False
        end
        object frxDBDataset2Time: TfrxMemoView
          Left = 157.606299210000000000
          Width = 39.913420000000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Time'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Time"]')
          ParentFont = False
        end
        object Memo26: TfrxMemoView
          Left = 396.850393700000000000
          Top = 0.046433799999988420
          Width = 75.590551181102400000
          Height = 18.897637800000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Additional"]')
          ParentFont = False
        end
        object frxDBDataset2Event: TfrxMemoView
          Left = 199.181102360000000000
          Width = 137.719350690000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          DataField = 'Event'
          DataSet = frxDBDatasetDefects
          DataSetName = 'Defects'
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = []
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          GapX = 5.000000000000000000
          Memo.UTF8W = (
            '[frxDBDataset2."Event"]')
          ParentFont = False
        end
      end
      object Header1: TfrxHeader
        Height = 47.936525860000000000
        Top = 207.874150000000000000
        Width = 718.110700000000000000
        Stretched = True
        object Memo25: TfrxMemoView
          Left = 15.000000000000000000
          Top = 2.551019999999994000
          Width = 688.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -16
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          HAlign = haCenter
          Memo.UTF8W = (
            'Line events / wydarzenia na linii')
          ParentFont = False
        end
        object Memo17: TfrxMemoView
          Top = 29.038875859999990000
          Width = 74.488250000000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Date')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo19: TfrxMemoView
          Left = 199.181102360000000000
          Top = 29.038875859999990000
          Width = 137.719350690000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Event')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo20: TfrxMemoView
          Left = 74.769955520000000000
          Top = 29.038875859999990000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Start'
            '(h:m)')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo21: TfrxMemoView
          Left = 117.165354330000000000
          Top = 29.038875859999990000
          Width = 43.086614170000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Stop'
            '(h:m)')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo22: TfrxMemoView
          Left = 157.606299210000000000
          Top = 29.038875859999990000
          Width = 39.913420000000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Time'
            '(min)')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo23: TfrxMemoView
          Left = 337.889763780000000000
          Top = 29.038875859999990000
          Width = 58.350318960000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Product')
          ParentFont = False
          WordWrap = False
          VAlign = vaCenter
        end
        object Memo24: TfrxMemoView
          Left = 472.440944880000000000
          Top = 29.038875859999990000
          Width = 264.795110460000000000
          Height = 18.897650000000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftRight, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Comment')
          ParentFont = False
          VAlign = vaCenter
        end
        object Memo27: TfrxMemoView
          Left = 396.850393700000000000
          Top = 29.038875859999990000
          Width = 75.590551180000000000
          Height = 18.897637800000000000
          ShowHint = False
          StretchMode = smMaxHeight
          Font.Charset = DEFAULT_CHARSET
          Font.Color = clBlack
          Font.Height = -13
          Font.Name = 'Arial'
          Font.Style = [fsBold]
          Frame.Typ = [ftLeft, ftTop, ftBottom]
          HAlign = haCenter
          Memo.UTF8W = (
            'Add.')
          ParentFont = False
          VAlign = vaCenter
        end
      end
      object MasterData2: TfrxMasterData
        Height = 22.677180000000000000
        Top = 321.260050000000000000
        Width = 718.110700000000000000
        DataSet = frxDBDatasetDefects
        DataSetName = 'Defects'
        RowCount = 0
      end
    end
  end
  object frxUserDataSetMeasurements: TfrxUserDataSet
    RangeEnd = reCount
    RangeEndCount = 1
    UserName = 'Measurements'
    Fields.Strings = (
      'Header')
    Left = 104
    Top = 328
  end
  object frxDBDatasetPacksView: TfrxDBDataset
    UserName = 'PacksView'
    CloseDataSource = False
    DataSet = ADOTablePacksView
    BCDToCurrency = False
    Left = 328
    Top = 272
  end
  object frxDBDatasetDefects: TfrxDBDataset
    UserName = 'Defects'
    CloseDataSource = False
    DataSet = ADOQueryDefectsShiftReport
    BCDToCurrency = False
    Left = 568
    Top = 272
  end
  object frxDBDatasetWorkshiftdays: TfrxDBDataset
    UserName = 'Workshiftdays'
    CloseDataSource = False
    DataSet = ADOTableWorkshitfdays
    BCDToCurrency = False
    Left = 696
    Top = 272
  end
  object ADOQueryDefectsShiftReport: TADOQuery
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    Parameters = <
      item
        Name = 'SHIFTID'
        DataType = ftWideString
        Size = 4
        Value = '5657'
      end>
    SQL.Strings = (
      'DECLARE @SHIFTID AS INT'
      'SET @SHIFTID = :SHIFTID'
      ''
      
        'SELECT Defect, CONCAT( [Width],'#39' x '#39',[Length]) AS Size, Thicknes' +
        's, [Substrate Origin],SUM(Quantity) AS Number,SUM(CAST(Width AS ' +
        'FLOAT)*CAST([Length] AS FLOAT)*Quantity/1000000) AS Area'
      ',COMMENT, [Substrate Type], EngAddDefect'
      ''
      'FROM [dbo].[CP_PARAM_DEFECT_EXPANDED_VIEW]'
      ''
      'WHERE ID_WORKSHIFTDAY = @SHIFTID'
      
        'GROUP BY DEFECT,Width,[Length],Thickness, [Substrate Origin], [S' +
        'ubstrate Type] , COMMENT, EngAddDefect'
      ''
      '')
    Left = 808
    Top = 72
  end
  object ADOTableWorkshitfdays: TADOTable
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    Filter = 'ID_WORKSHIFTDAY = 5657'
    Filtered = True
    TableName = 'WORKSHIFTDAYS'
    Left = 800
    Top = 136
  end
  object ADOTableDefectsExpanded: TADOTable
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    Filter = 'Where ID_PRODTYPE = 16'
    TableName = 'CP_DEFECT_EXPANDED_VIEW'
    Left = 64
    Top = 192
  end
  object ADOTableWorkshiftRemarks: TADOTable
    Connection = ADOConnectionCreport
    CursorType = ctStatic
    Filter = 'ID_WorkshiftDay = 4585'
    Filtered = True
    TableName = 'WorkshiftDaysRemarks'
    Left = 216
    Top = 192
  end
  object frxDBDataseWorkshiftRemarks: TfrxDBDataset
    UserName = 'GeneralRemarks'
    CloseDataSource = False
    DataSet = ADOTableWorkshiftRemarks
    BCDToCurrency = False
    Left = 848
    Top = 272
  end
  object frxUserDataSetOEE: TfrxUserDataSet
    UserName = 'OEE'
    Left = 424
    Top = 336
  end
  object ADOQueryInitializeAdditionalToProdID: TADOQuery
    Connection = ADOConnectionReport2
    Parameters = <>
    Left = 704
    Top = 576
  end
  object ADOConnectionReport2: TADOConnection
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'SQLOLEDB'
    Left = 704
    Top = 632
  end
end
