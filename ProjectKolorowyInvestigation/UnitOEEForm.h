//---------------------------------------------------------------------------

#ifndef UnitOEEFormH
#define UnitOEEFormH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormOEE : public TForm
{
__published:	// IDE-managed Components
	TButton *ButtonCloseOEE;
	TEdit *EditCG_LineSpeed;
	TLabel *LabelCurrentShift;
	TLabel *LabelCurrentShiftOEEResult;
	TLabel *LabelMonthlyOEEResult;
	TLabel *LabelMonthlyOEE;
	TLabel *Label1;
	TLabel *Label2;
	TLabel *LabelMaxInputPerHourResult;
	TLabel *LabelArbitraryPeriod;
	TLabel *LabelArbitraryPeriodResult;
	TLabel *Label5;
	TButton *ButtonCalculateOEEwithGivenSpeed;
	TLabel *LabelYieldShift;
	TLabel *LabelYieldResultShift;
	TLabel *LabellUptimeShift;
	TLabel *LabelUptimeResultShift;
	TLabel *LabelPerformance;
	TLabel *LabelPerformanceResultShift;
	TLabel *Label3;
	TLabel *LabelYieldResultMonthly;
	TLabel *Label6;
	TLabel *LabelUptimeResultMonthly;
	TLabel *Label8;
	TLabel *LabelPerformanceResultMonthly;
	TLabel *Label10;
	TLabel *LabelYieldResultArbitrary;
	TLabel *Label12;
	TLabel *LabelUptimeResultArbitrary;
	TLabel *Label14;
	TLabel *LabelPerformanceResultArbitrary;
	TLabel *LabelGoal;
	TLabel *LabelGoalResult;
	TLabel *Label4;
	TLabel *LabelMonthFrom;
	TLabel *Label9;
	TLabel *LabelMonthTo;
	TLabel *Label7;
	TLabel *LabelArbFrom;
	TLabel *Label13;
	TLabel *LabelArbTo;
	TLabel *Label11;
	TLabel *LabelOldShiftOEE;
	TLabel *Label16;
	TLabel *Label17;
	TLabel *LabelOldShiftPerformance;
	TLabel *Label15;
	TLabel *LabelOldMonthOEE;
	TLabel *Label19;
	TLabel *Label20;
	TLabel *LabelOldMonthPerformance;
	TLabel *LabelOldArbitraryPerformance;
	TLabel *Label23;
	TLabel *Label24;
	TLabel *LabelOldArbitraryOEE;
	TLabel *Label26;
	void __fastcall ButtonCloseOEEClick(TObject *Sender);
	void __fastcall FormCreate(TObject *Sender);
	void __fastcall ButtonCalculateOEEwithGivenSpeedClick(TObject *Sender);
	void __fastcall FormClose(TObject *Sender, TCloseAction &Action);
private:	// User declarations
	bool OpenedBeforePrinting;

public:		// User declarations
	__fastcall TFormOEE(TComponent* Owner);
	__fastcall TFormOEE(TComponent* Owner,bool BeforePrinting);
	void ShowOEE();
};
//---------------------------------------------------------------------------
extern PACKAGE TFormOEE *FormOEE;
//---------------------------------------------------------------------------
#endif
