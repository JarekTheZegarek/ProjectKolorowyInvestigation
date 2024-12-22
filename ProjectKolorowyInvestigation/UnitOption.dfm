object FormOption: TFormOption
  Left = 0
  Top = 0
  Caption = 'Opcje / Option'
  ClientHeight = 300
  ClientWidth = 635
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  PixelsPerInch = 96
  TextHeight = 13
  object CheckBoxRestrictedArea: TCheckBox
    Left = 56
    Top = 48
    Width = 97
    Height = 17
    Hint = 
      'If checked Area of single glass pane is restricted to 2 signific' +
      'ant digits (V2 Like).'
    Caption = 'Rounded Area'
    ParentShowHint = False
    ShowHint = True
    TabOrder = 0
    OnClick = CheckBoxRestrictedAreaClick
  end
end
