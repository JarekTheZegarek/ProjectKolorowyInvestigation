object FormCoCincompatibility: TFormCoCincompatibility
  Left = 0
  Top = 0
  Caption = 
    'Przypisz w'#322'a'#347'ciw'#261' kategori'#281' CoC do nierozpoznanego statusu linii' +
    ' w E-Logbook'#39'u'
  ClientHeight = 184
  ClientWidth = 801
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  OnActivate = FormActivate
  PixelsPerInch = 96
  TextHeight = 13
  object Label1: TLabel
    Left = 56
    Top = 75
    Width = 523
    Height = 69
    Caption = 
      'W E-Logbook'#39'u wybrano status linii, kt'#243'ry nie ma swojego odpowie' +
      'dnika w CoC. Prosz'#281' o wybranie kategorii CoC, kt'#243'ra najlepiej od' +
      'powiada statusowi linii wybranemu w E-Logbook'#39'u.'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clBlue
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    WordWrap = True
  end
  object EditIncompatiblileWithCoC: TEdit
    Left = 32
    Top = 40
    Width = 193
    Height = 21
    TabOrder = 0
  end
  object ComboBoxCoCCategories: TComboBox
    Left = 241
    Top = 40
    Width = 361
    Height = 21
    TabOrder = 1
  end
  object ButtonNewStatusCompatibileWithCoC: TButton
    Left = 608
    Top = 38
    Width = 75
    Height = 25
    Caption = 'Przypisz'
    TabOrder = 2
    OnClick = ButtonNewStatusCompatibileWithCoCClick
  end
end
