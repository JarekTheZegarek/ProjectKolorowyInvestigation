object DataModule2: TDataModule2
  OldCreateOrder = False
  Height = 588
  Width = 893
  object ADOConnectionCreport: TADOConnection
    Connected = True
    ConnectionString = 
      'Provider=SQLNCLI.1;Password=276CoaterReports;Persist Security In' +
      'fo=True;User ID=276CoaterReports;Initial Catalog=276CoaterReport' +
      's;Data Source=276L030W7\JSDEVELOP'
    LoginPrompt = False
    Mode = cmShareDenyNone
    Provider = 'SQLNCLI.1'
    Left = 48
    Top = 8
  end
  object ADOQueryCreport: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    Left = 192
    Top = 8
  end
  object ADOQueryGIP_Seek: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    Left = 312
    Top = 16
  end
  object ADOConnection1: TADOConnection
    ConnectionString = 
      'Provider=SQLNCLI11.1;Integrated Security="";Persist Security Inf' +
      'o=False;User ID=276CoaterReports;Password=276CoaterReports;Initi' +
      'al Catalog=276CoaterReports;Data Source=276DB03.czestochowa.guar' +
      'dian.corp;Initial File Name="";Server SPN=""'
    LoginPrompt = False
    Provider = 'SQLNCLI11.1'
    Left = 616
    Top = 328
  end
  object ADOQueryTemp: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <>
    Left = 432
    Top = 17
  end
  object ADOQueryUpdateComment: TADOQuery
    Connection = ADOConnectionCreport
    Parameters = <
      item
        Name = 'EntityID'
        DataType = ftInteger
        Value = Null
      end
      item
        Name = 'Comment'
        DataType = ftString
        Value = Null
      end>
    SQL.Strings = (
      'DECLARE @ID AS INT'
      'SET @ID = :EntityID'
      'DECLARE @NoRows AS INT '
      ' '
      'SELECT @NoRows= COUNT(*)  FROM CP_DEFECTEXPANDED'
      'WHERE ID_DEFECT = @ID'
      ''
      'IF @NoRows = 0'
      '     INSERT INTO CP_DEFECTEXPANDED'
      '     (ID_DEFECT)'
      '    VALUES '
      '    ( @ID) '
      ''
      'UPDATE CP_DEFECTEXPANDED'
      'SET  COMMENT = :Comment '
      'WHERE ID_DEFECT = @ID ')
    Left = 40
    Top = 120
  end
end
