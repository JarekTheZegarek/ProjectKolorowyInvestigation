//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitProducts.h"
#include <map>
#include "UnitMain.h"
#include "Global.h"
//---------------------------------------------------------------------------

#pragma package(smart_init)




 void TProducts::SetID(int ID)
 {
     ID_ProdType = ID;
 }

 int TProducts::GetID()
 {
     return ID_ProdType;
 }

void TProducts::SetShortName(AnsiString Name)
{
    ProdType_Name = Name;
}

void TProducts::SetFullName(AnsiString Name)
{
    FullName = Name;
}

TProducts::TProducts()
{
	Remarks = 0;
	ID_ProdType = 0;
	FullName="";
	Part="";
	ProdKind.clear();

}

void TProducts::SetRemarks(TStringList* R)
{
    if(Remarks!=0)
    	delete Remarks;
    Remarks = new TStringList();
	Remarks->Text = R->Text;
}

TProducts::~TProducts()
{
//   if(Remarks!=0)
//    delete Remarks;
}

AnsiString TProducts::GetFullName()
{
  return FullName;
}

AnsiString TProducts::GetShortName()
{
    return ProdType_Name;
}

AnsiString TProducts::GetCombinedName()
{
   AnsiString Tym;
   Tym = "("+ProdType_Name+") "+FullName;
   return Tym;
}

TStringList* TProducts::GetRemarks()
 {
    return Remarks;
 }

void TProducts::operator=(TProducts& Source)
{
	ID_ProdType = Source.ID_ProdType;
	ProdType_Name= Source.ProdType_Name;
	FullName = Source.FullName;
	if(Remarks != 0)
	{
		delete Remarks;
	}
	if (Source.Remarks==0) {
	   Remarks = 0;
	   return;
	}
	Remarks = new TStringList;
	Remarks->AddStrings(Source.Remarks);
}


//class   TProducts
//{
//    int ID_ProdType;
//    AnsiString ProdType_Name;
//    AnsiString FullName;
//    TStringList* Remarks;
//public:
//	;
//
//    ;
//
//    ;
//    ;
//    TStringList* ;
//    int GetID();
//};

