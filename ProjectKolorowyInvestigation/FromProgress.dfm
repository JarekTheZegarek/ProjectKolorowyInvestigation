object FormProgress: TFormProgress
  Left = 0
  Top = 0
  BorderIcons = []
  Caption = 'Progress'
  ClientHeight = 89
  ClientWidth = 635
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  Position = poMainFormCenter
  Scaled = False
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 26
    Top = 16
    Width = 601
    Height = 26
    Alignment = taCenter
    AutoSize = False
    Caption = 'Prosz'#281' czeka'#263' ci'#261'gle pracuj'#281'!'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Arial Black'
    Font.Style = [fsBold, fsItalic]
    ParentFont = False
  end
  object ProgressBar: TProgressBar
    Left = 26
    Top = 48
    Width = 601
    Height = 17
    BarColor = clLime
    TabOrder = 0
  end
end
