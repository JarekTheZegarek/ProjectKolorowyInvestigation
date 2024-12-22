object FormAdditionalDefectDetails: TFormAdditionalDefectDetails
  Left = 0
  Top = 0
  Caption = 'Additional Defect Details'
  ClientHeight = 194
  ClientWidth = 731
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnCreate = FormCreate
  PixelsPerInch = 96
  TextHeight = 13
  object LabelDefectAdditional: TLabel
    Left = 74
    Top = 15
    Width = 271
    Height = 22
    Caption = 'Szczeg'#243#322'y defektu / Defect details'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -19
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object LabelComment: TLabel
    Left = 478
    Top = 15
    Width = 184
    Height = 22
    Alignment = taCenter
    Caption = 'Komentarz / Comment'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -19
    Font.Name = 'Times New Roman'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ComboBoxDefectDetails: TComboBox
    Left = 18
    Top = 40
    Width = 383
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'ComboBoxDefectDetails'
  end
  object MemoComment: TMemo
    Left = 418
    Top = 40
    Width = 305
    Height = 137
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
  end
  object ButtonAddDetails: TButton
    Left = 18
    Top = 88
    Width = 383
    Height = 89
    Caption = 'Dodaj / Add'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -21
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    OnClick = ButtonAddDetailsClick
  end
end
