//---------------------------------------------------------------------------

#ifndef UnitProductsH
#define UnitProductsH
#include <vcl.h>
#include <set>

//---------------------------------------------------------------------------

typedef UnicodeString TProdKinds ;

class TProducts  //In the  meaning of coatings  - there is also name TCoating definied for this class
{
	int ID_ProdType;
	AnsiString ProdType_Name;
	AnsiString FullName;
	TStringList* Remarks;
	UnicodeString Part;
	std::set<TProdKinds> ProdKind;
public:
	TProducts();
	~TProducts();
	void SetID(int ID);
	void SetShortName(AnsiString Name);
	void SetFullName(AnsiString Name);
	void SetRemarks(TStringList* Remarks);
	void SetPart(UnicodeString P) {Part = P ; }
	void AddProdKind(TProdKinds K) {ProdKind.insert(K); }
	bool Is(TProdKinds K) {if(ProdKind.find(K)!=ProdKind.end())
								return true;
							  else
								return false;
							 }
	AnsiString GetFullName();
	AnsiString GetShortName();
	AnsiString GetCombinedName();
	TStringList* GetRemarks();
	void operator=(TProducts& In);
	int GetID();
	bool operator==(const TProducts& Coat)
	{
		return (ID_ProdType == Coat.ID_ProdType);
	}
	void Reset() { ID_ProdType=0; ProdType_Name = "Unknown"; FullName="Unknown";
				   if(Remarks!=0)
					 Remarks->Clear();
				   ProdKind.clear();
				 }
	UnicodeString GetPart() { return Part; }

};

typedef  TProducts TCoating;








#endif
