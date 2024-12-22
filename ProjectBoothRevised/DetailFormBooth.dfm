object FormAdditionalDetails: TFormAdditionalDetails
  Left = 0
  Top = 0
  Width = 1144
  Height = 652
  AutoScroll = True
  Caption = 'Szczeg'#243#322'y defektu / Defect details'
  Color = clBtnFace
  Font.Charset = DEFAULT_CHARSET
  Font.Color = clWindowText
  Font.Height = -11
  Font.Name = 'Tahoma'
  Font.Style = []
  GlassFrame.Enabled = True
  OldCreateOrder = False
  DesignSize = (
    1128
    614)
  PixelsPerInch = 96
  TextHeight = 13
  object LabelThickness: TLabel
    Left = 682
    Top = 63
    Width = 124
    Height = 19
    Caption = 'Grubo'#347#263' / Thickness'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object Label4: TLabel
    Left = 560
    Top = 63
    Width = 87
    Height = 19
    Caption = 'Rozmiar / Size'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object LabelType: TLabel
    Left = 420
    Top = 63
    Width = 65
    Height = 19
    Caption = 'Typ / Type'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object LabelKind: TLabel
    Left = 219
    Top = 63
    Width = 154
    Height = 19
    Caption = 'Szk'#322'o bazowe / Substrate'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object LabelCoating: TLabel
    Left = 128
    Top = 63
    Width = 47
    Height = 19
    Caption = 'Coating'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object LabelDefectAdditional: TLabel
    Left = 963
    Top = 63
    Width = 113
    Height = 19
    Caption = 'Szczeg'#243#322'y / Details'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object LabelDefect: TLabel
    Left = 833
    Top = 63
    Width = 93
    Height = 19
    Caption = 'Defekt / Defect'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object Label1: TLabel
    Left = 20
    Top = 63
    Width = 70
    Height = 19
    Caption = 'Data / Date'
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clBlack
    Font.Height = -16
    Font.Name = 'Times New Roman'
    Font.Style = []
    ParentFont = False
  end
  object LabelComment: TLabel
    Left = 16
    Top = 125
    Width = 949
    Height = 18
    Alignment = taCenter
    AutoSize = False
    Caption = 'Komentarz / Comment'
    Font.Charset = ANSI_CHARSET
    Font.Color = clBlack
    Font.Height = -19
    Font.Name = 'Lucida Calligraphy'
    Font.Style = [fsBold]
    ParentFont = False
  end
  object ComboBoxThickness: TComboBox
    Left = 700
    Top = 88
    Width = 82
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 0
    Text = 'ComboBoxThickness'
  end
  object ComboBoxSize: TComboBox
    Left = 539
    Top = 88
    Width = 145
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 1
    Text = 'ComboBoxSize'
  end
  object ComboBoxSubstrateClass: TComboBox
    Left = 388
    Top = 88
    Width = 145
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 2
    Text = 'ComboBoxSubstrateClass'
  end
  object ComboBoxSubstrate: TComboBox
    Left = 205
    Top = 88
    Width = 177
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 3
    Text = 'ComboBoxSubstrate'
    OnChange = ComboBoxSubstrateChange
  end
  object ComboBoxProduct: TComboBox
    Left = 112
    Top = 88
    Width = 87
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 4
    Text = 'ComboBoxProduct'
  end
  object ButtonDelete: TButton
    Left = 16
    Top = 16
    Width = 155
    Height = 41
    Caption = 'Usu'#324' / Remove '
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 5
    OnClick = ButtonDeleteClick
  end
  object ComboBoxDefectDetails: TComboBox
    Left = 963
    Top = 88
    Width = 145
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 6
    Text = 'ComboBoxDefectDetails'
  end
  object ComboBoxDefect: TComboBox
    Left = 812
    Top = 88
    Width = 145
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 7
    Text = 'ComboBoxDefect'
  end
  object ComboBoxDate: TComboBox
    Left = 16
    Top = 88
    Width = 87
    Height = 31
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 8
    Text = 'ComboBoxDate'
  end
  object StringGridAdditional: TStringGrid
    Left = 16
    Top = 232
    Width = 1092
    Height = 369
    Anchors = [akLeft, akTop, akRight, akBottom]
    ColCount = 13
    DrawingStyle = gdsClassic
    FixedColor = clInfoBk
    FixedCols = 0
    RowCount = 20
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -13
    Font.Name = 'Times New Roman'
    Font.Style = []
    Options = [goFixedVertLine, goFixedHorzLine, goVertLine, goHorzLine, goRangeSelect, goDrawFocusSelected, goRowSelect]
    ParentFont = False
    TabOrder = 9
    OnSelectCell = StringGridAdditionalSelectCell
    ColWidths = (
      102
      98
      74
      113
      106
      90
      75
      64
      64
      78
      196
      64
      64)
  end
  object EditDate: TEdit
    Left = 16
    Top = 88
    Width = 91
    Height = 31
    Alignment = taCenter
    AutoSize = False
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 10
    Text = 'EditDate'
  end
  object ButtonAddComent: TButton
    Left = 951
    Top = 146
    Width = 157
    Height = 80
    Caption = 'Dodaj Komentarz Add Comment'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = [fsBold]
    ImageAlignment = iaCenter
    ParentFont = False
    TabOrder = 11
    WordWrap = True
    OnClick = ButtonAddComentClick
  end
  object RichEditComment: TRichEdit
    Left = 20
    Top = 149
    Width = 925
    Height = 77
    Font.Charset = EASTEUROPE_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 12
  end
  object EditDateDate: TEdit
    Left = 15
    Top = 118
    Width = 91
    Height = 31
    Alignment = taCenter
    AutoSize = False
    Enabled = False
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -16
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 13
    Text = 'EditDate'
  end
  object ButtonClose: TButton
    Left = 963
    Top = 8
    Width = 145
    Height = 49
    Caption = 'Zamknij / Close'
    TabOrder = 14
    OnClick = ButtonCloseClick
  end
  object ButtonUpdate: TButton
    Left = 192
    Top = 16
    Width = 155
    Height = 41
    Caption = 'Popraw / Update'
    Font.Charset = DEFAULT_CHARSET
    Font.Color = clWindowText
    Font.Height = -19
    Font.Name = 'Tahoma'
    Font.Style = []
    ParentFont = False
    TabOrder = 15
    OnClick = ButtonUpdateClick
  end
end
