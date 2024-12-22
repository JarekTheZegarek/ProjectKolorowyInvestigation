object FormCoCExport: TFormCoCExport
  Left = 460
  Top = 0
  Caption = 'FormCocExport'
  ClientHeight = 750
  ClientWidth = 1521
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  PixelsPerInch = 96
  TextHeight = 13
  object LabelOpisZmiany: TLabel
    Left = 688
    Top = 0
    Width = 67
    Height = 19
    Caption = 'ZMIANA'
    Color = clFuchsia
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clFuchsia
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold, fsItalic]
    ParentColor = False
    ParentFont = False
  end
  object Label1: TLabel
    Left = 663
    Top = 112
    Width = 132
    Height = 22
    Caption = 'Dane wej'#347'ciowe'
    Color = clFuchsia
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clFuchsia
    Font.Height = -19
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label2: TLabel
    Left = 672
    Top = 328
    Width = 123
    Height = 22
    Caption = 'Podsumowanie'
    Color = clFuchsia
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clNavy
    Font.Height = -19
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentColor = False
    ParentFont = False
  end
  object Label3: TLabel
    Left = 560
    Top = 536
    Width = 311
    Height = 22
    Caption = 'Dane przygotowane do zapisu do CoC'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object StringGridFrom: TStringGrid
    Left = 48
    Top = 356
    Width = 681
    Height = 174
    ColCount = 2
    FixedColor = clMoneyGreen
    FixedCols = 0
    RowCount = 10
    TabOrder = 0
    RowHeights = (
      24
      17
      24
      24
      24
      24
      24
      24
      24
      24)
  end
  object StringGridBeforeExport: TStringGrid
    Left = 48
    Top = 564
    Width = 681
    Height = 165
    ColCount = 4
    FixedColor = clMoneyGreen
    FixedCols = 0
    RowCount = 2
    TabOrder = 1
    RowHeights = (
      24
      17)
  end
  object Memo1: TMemo
    Left = 176
    Top = 25
    Width = 1322
    Height = 81
    Lines.Strings = (
      'Memo1')
    TabOrder = 2
  end
  object StringGrid2: TStringGrid
    Left = 48
    Top = 140
    Width = 1465
    Height = 186
    FixedColor = clMoneyGreen
    FixedCols = 0
    TabOrder = 3
  end
  object ButtonPrepareForExport: TButton
    Left = 48
    Top = 23
    Width = 122
    Height = 25
    Caption = 'Przygotuj dane'
    TabOrder = 4
    OnClick = ButtonPrepareForExportClick
  end
  object ButtonExportToCoC: TButton
    Left = 48
    Top = 52
    Width = 122
    Height = 25
    Caption = 'Zapisz do CoC'
    TabOrder = 5
    OnClick = ButtonExportToCoCClick
  end
  object ButtonExitCoC: TButton
    Left = 48
    Top = 81
    Width = 122
    Height = 25
    Caption = 'Powr'#243't'
    TabOrder = 6
    OnClick = ButtonExitCoCClick
  end
  object StringGridProductsForCoC: TStringGrid
    Left = 752
    Top = 356
    Width = 761
    Height = 174
    ColCount = 4
    FixedColor = clMoneyGreen
    FixedCols = 0
    RowCount = 10
    TabOrder = 7
  end
  object StringGridProductPrepared: TStringGrid
    Left = 752
    Top = 564
    Width = 761
    Height = 163
    ColCount = 4
    FixedColor = clMoneyGreen
    FixedCols = 0
    RowCount = 10
    TabOrder = 8
  end
  object SimpleObjectBroker1: TSimpleObjectBroker
    Left = 232
    Top = 832
  end
end
