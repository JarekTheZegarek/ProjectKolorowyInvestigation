object FormMagnification: TFormMagnification
  Left = 0
  Top = 0
  Caption = 'Powi'#281'kszenie / Magnification'
  ClientHeight = 529
  ClientWidth = 929
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  OldCreateOrder = False
  DesignSize = (
    929
    529)
  PixelsPerInch = 96
  TextHeight = 13
  object ButtonExport: TButton
    Left = 24
    Top = 24
    Width = 129
    Height = 41
    Caption = 'Export To Excel'
    TabOrder = 0
    OnClick = ButtonExportClick
  end
  object ButtonClose: TButton
    Left = 816
    Top = 24
    Width = 97
    Height = 41
    Anchors = [akTop, akRight]
    Caption = 'Zamknij / Close'
    TabOrder = 1
    OnClick = ButtonCloseClick
  end
  object StringGrid1: TStringGrid
    Left = 24
    Top = 88
    Width = 889
    Height = 425
    Anchors = [akLeft, akTop, akRight]
    FixedCols = 0
    TabOrder = 2
  end
end
