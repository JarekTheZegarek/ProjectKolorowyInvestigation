object FormOEE: TFormOEE
  Left = 200
  Top = 0
  Caption = 'OEE results form'
  ClientHeight = 620
  ClientWidth = 1059
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poDesigned
  OnClose = FormClose
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object LabelCurrentShift: TLabel
    Left = 64
    Top = 129
    Width = 255
    Height = 40
    Caption = 'Current Shift OEE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelCurrentShiftOEEResult: TLabel
    Left = 448
    Top = 129
    Width = 89
    Height = 40
    Caption = 'Result'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelMonthlyOEEResult: TLabel
    Left = 448
    Top = 263
    Width = 89
    Height = 40
    Caption = 'Result'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelMonthlyOEE: TLabel
    Left = 64
    Top = 263
    Width = 279
    Height = 40
    Caption = 'Current Month OEE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 392
    Top = 129
    Width = 12
    Height = 40
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label2: TLabel
    Left = 392
    Top = 263
    Width = 12
    Height = 40
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelMaxInputPerHourResult: TLabel
    Left = 968
    Top = 143
    Width = 83
    Height = 23
    AutoSize = False
    Caption = 'MaxInputPerHour'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    Visible = False
  end
  object LabelArbitraryPeriod: TLabel
    Left = 64
    Top = 385
    Width = 298
    Height = 40
    Caption = 'Arbitrary period OEE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelArbitraryPeriodResult: TLabel
    Left = 448
    Top = 385
    Width = 89
    Height = 40
    Caption = 'Result'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label5: TLabel
    Left = 392
    Top = 385
    Width = 12
    Height = 40
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelYieldShift: TLabel
    Left = 64
    Top = 187
    Width = 35
    Height = 19
    Caption = 'Yield'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelYieldResultShift: TLabel
    Left = 105
    Top = 187
    Width = 35
    Height = 19
    Caption = 'Yield'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabellUptimeShift: TLabel
    Left = 190
    Top = 187
    Width = 51
    Height = 19
    Caption = 'Uptime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelUptimeResultShift: TLabel
    Left = 247
    Top = 187
    Width = 51
    Height = 19
    Caption = 'Uptime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelPerformance: TLabel
    Left = 353
    Top = 187
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelPerformanceResultShift: TLabel
    Left = 448
    Top = 187
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label3: TLabel
    Left = 64
    Top = 317
    Width = 35
    Height = 19
    Caption = 'Yield'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelYieldResultMonthly: TLabel
    Left = 105
    Top = 317
    Width = 35
    Height = 19
    Caption = 'Yield'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label6: TLabel
    Left = 190
    Top = 317
    Width = 51
    Height = 19
    Caption = 'Uptime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelUptimeResultMonthly: TLabel
    Left = 247
    Top = 317
    Width = 51
    Height = 19
    Caption = 'Uptime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label8: TLabel
    Left = 353
    Top = 317
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelPerformanceResultMonthly: TLabel
    Left = 448
    Top = 317
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label10: TLabel
    Left = 64
    Top = 447
    Width = 35
    Height = 19
    Caption = 'Yield'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelYieldResultArbitrary: TLabel
    Left = 105
    Top = 447
    Width = 35
    Height = 19
    Caption = 'Yield'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label12: TLabel
    Left = 190
    Top = 447
    Width = 51
    Height = 19
    Caption = 'Uptime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelUptimeResultArbitrary: TLabel
    Left = 247
    Top = 447
    Width = 51
    Height = 19
    Caption = 'Uptime'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label14: TLabel
    Left = 353
    Top = 447
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelPerformanceResultArbitrary: TLabel
    Left = 448
    Top = 447
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelGoal: TLabel
    Left = 65
    Top = 41
    Width = 187
    Height = 40
    Caption = 'Target OEE :'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelGoalResult: TLabel
    Left = 258
    Top = 41
    Width = 89
    Height = 40
    Caption = 'Result'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -33
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 65
    Top = 342
    Width = 37
    Height = 19
    Caption = 'From'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelMonthFrom: TLabel
    Left = 108
    Top = 342
    Width = 37
    Height = 19
    Caption = 'From'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label9: TLabel
    Left = 353
    Top = 342
    Width = 19
    Height = 19
    Caption = 'To'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelMonthTo: TLabel
    Left = 385
    Top = 342
    Width = 19
    Height = 19
    Caption = 'To'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label7: TLabel
    Left = 65
    Top = 478
    Width = 37
    Height = 19
    Caption = 'From'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelArbFrom: TLabel
    Left = 108
    Top = 478
    Width = 37
    Height = 19
    Caption = 'From'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label13: TLabel
    Left = 353
    Top = 472
    Width = 19
    Height = 19
    Caption = 'To'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelArbTo: TLabel
    Left = 385
    Top = 472
    Width = 19
    Height = 19
    Caption = 'To'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label11: TLabel
    Left = 631
    Top = 142
    Width = 152
    Height = 29
    Caption = 'Old Value OEE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelOldShiftOEE: TLabel
    Left = 865
    Top = 142
    Width = 65
    Height = 29
    Caption = 'Result'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label16: TLabel
    Left = 847
    Top = 142
    Width = 8
    Height = 29
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label17: TLabel
    Left = 631
    Top = 191
    Width = 125
    Height = 19
    Caption = 'Performance OLD'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelOldShiftPerformance: TLabel
    Left = 762
    Top = 191
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label15: TLabel
    Left = 635
    Top = 272
    Width = 152
    Height = 29
    Caption = 'Old Value OEE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelOldMonthOEE: TLabel
    Left = 865
    Top = 272
    Width = 65
    Height = 29
    Caption = 'Result'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label19: TLabel
    Left = 851
    Top = 272
    Width = 8
    Height = 29
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label20: TLabel
    Left = 635
    Top = 321
    Width = 125
    Height = 19
    Caption = 'Performance OLD'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelOldMonthPerformance: TLabel
    Left = 766
    Top = 321
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelOldArbitraryPerformance: TLabel
    Left = 766
    Top = 447
    Width = 89
    Height = 19
    Caption = 'Performance'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label23: TLabel
    Left = 635
    Top = 447
    Width = 125
    Height = 19
    Caption = 'Performance OLD'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label24: TLabel
    Left = 851
    Top = 398
    Width = 8
    Height = 29
    Caption = ':'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object LabelOldArbitraryOEE: TLabel
    Left = 869
    Top = 398
    Width = 65
    Height = 29
    Caption = 'Result'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object Label26: TLabel
    Left = 635
    Top = 398
    Width = 152
    Height = 29
    Caption = 'Old Value OEE'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -24
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
  end
  object ButtonCloseOEE: TButton
    Left = 912
    Top = 8
    Width = 139
    Height = 73
    Caption = 'Zamknij/Close'
    TabOrder = 0
    OnClick = ButtonCloseOEEClick
  end
  object EditCG_LineSpeed: TEdit
    Left = 968
    Top = 172
    Width = 83
    Height = 21
    TabOrder = 1
    Visible = False
  end
  object ButtonCalculateOEEwithGivenSpeed: TButton
    Left = 968
    Top = 221
    Width = 83
    Height = 25
    Hint = 'Przelicza OEE dla wprowadzonej maksymalnej pr'#281'dko'#347'ci'
    Caption = 'Licz / Calculate'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 2
    Visible = False
    OnClick = ButtonCalculateOEEwithGivenSpeedClick
  end
end
