//---------------------------------------------------------------------------

#ifndef FromProgressH
#define FromProgressH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Vcl.Controls.hpp>
#include <Vcl.StdCtrls.hpp>
#include <Vcl.Forms.hpp>
#include <Vcl.ComCtrls.hpp>
//---------------------------------------------------------------------------
class TFormProgress : public TForm
{
__published:	// IDE-managed Components
	TProgressBar *ProgressBar;
	TLabel *Label1;
private:	// User declarations
public:		// User declarations
	__fastcall TFormProgress(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TFormProgress *FormProgress;
//---------------------------------------------------------------------------
#endif
