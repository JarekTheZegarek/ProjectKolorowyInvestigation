//---------------------------------------------------------------------------

#ifndef DataBoothH
#define DataBoothH
//---------------------------------------------------------------------------
#include <System.Classes.hpp>
#include <Data.DB.hpp>
#include <Data.Win.ADODB.hpp>
//---------------------------------------------------------------------------
class TDataModule2 : public TDataModule
{
__published:	// IDE-managed Components
	TADOConnection *ADOConnectionCreport;
	TADOQuery *ADOQueryCreport;
	TADOQuery *ADOQueryGIP_Seek;
	TADOConnection *ADOConnection1;
	TADOQuery *ADOQueryTemp;
	TADOQuery *ADOQueryUpdateComment;
private:	// User declarations
public:		// User declarations
	__fastcall TDataModule2(TComponent* Owner);
};
//---------------------------------------------------------------------------
extern PACKAGE TDataModule2 *DataModule2;
//---------------------------------------------------------------------------
#endif
