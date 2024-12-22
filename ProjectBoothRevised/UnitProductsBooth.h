//---------------------------------------------------------------------------

#ifndef UnitProductsBoothH
#define UnitProductsBoothH
#include <vcl.h>
#include <list>
#include <vector>
#include <map>
//---------------------------------------------------------------------------

class TCoating
{
	int ID_ProdType;
	AnsiString ProdType_Name;
	AnsiString FullName;
	TStringList* Remarks;
public:
	TCoating();
	~TCoating();
	void SetID(int ID);
	void SetShortName(AnsiString Name);
	void SetFullName(AnsiString Name);
	void SetRemarks(TStringList* Remarks);
	AnsiString GetFullName();
	AnsiString GetShortName();
	AnsiString GetCombinedName();
	TStringList* GetRemarks();
	//void operator=(TCoating& In);
	void operator=(TCoating In);

	bool operator==(const TCoating& Coat);
	int GetID();
	void Reset() { ID_ProdType=0; ProdType_Name = "Unknown"; FullName="Unknown"; if(Remarks!=0) Remarks->Clear(); }
};

class TSubstrate
{
	int ID;
	UnicodeString Name;  // Lami or Float if float then origin of the glass e.g. GIP, Ohroshaza
	int ClassID;
	UnicodeString Class; //e.g. for float "TRR", "Architecture" ...
	int ThicknessID;
	UnicodeString Thickness; //UnicodeString because it could be also Lami composition e.g. "3.3.1"
	UnicodeString Size;
	int SizeID;
	int Width;
	int Length;
public:
	TSubstrate(): ID(0), Name("Unknown"),ClassID(0),Class("Unknown"),ThicknessID(0), Thickness("Unknown"), SizeID(0), Size("Unknown")
	{}
	void Add(int SubstrateID, UnicodeString NameOfSubstrate, int SubstrateClassID, UnicodeString SubstrateClass,int SubstrateThicknessID, UnicodeString SubstrateThickness, UnicodeString SubstrateSizeName, int SubstrateSizeID, int SizeWidth, int SizeLength);
	bool operator==(const TSubstrate& Sub)
	{ return (ID==Sub.ID) && (ClassID == Sub.ClassID) && (ThicknessID == Sub.ThicknessID) && (SizeID == Sub.SizeID)  ; }
	int GetID() { return ID; }
	UnicodeString GetSubName() { return Name; }
	int GetThicknessID() { return ThicknessID; }
	UnicodeString GetThicknessName() { return Thickness; }
	int GetSubClassID() { return ClassID; }
	UnicodeString GetSubClassName() { return Class;}
	int GetSizeID() {return SizeID; }
	UnicodeString GetSizeName() { return Size; }
	int GetWidth() { return Width; }
	int GetLength() { return Length; }
	double GetAreaM2() { return (Width*Length)/1000000.0 ; }
	void Reset() {ID=0; Name="Unknown"; ClassID=0; Class="Unknown"; ThicknessID=0; Thickness="Unknown"; Width=Length=SizeID=0; Size="Unknown";}
};


class TDefectDetail
{
   UnicodeString KindName;
   UnicodeString AdditionalDescription;
   int Kind;
   int Number; //Number of Defects
   int AddDefID;
   int ID; // ID_DEFECT - ID of each registered defect issue (new field in CP_DEFECT relation).  - EntityID
//   double  Area;
//   double Width;
//   double Length;
public:
   TDefectDetail():KindName(""), Kind(0),AdditionalDescription(""),Number(0), AddDefID(0), ID(0)//,Area(0),Width(0),Length(0)
   {}

   TDefectDetail(UnicodeString KindNa,UnicodeString AddDescr,int KindNr, int NumberOfElements,int AddDefectID): KindName(KindNa),AdditionalDescription(AddDescr),Kind(KindNr),Number(NumberOfElements),AddDefID(AddDefectID)
   {}
   TDefectDetail(UnicodeString KindNa,UnicodeString AddDescr,int KindNr, int NumberOfElements,int AddDefectID,int DefectID): KindName(KindNa),AdditionalDescription(AddDescr),Kind(KindNr),Number(NumberOfElements),AddDefID(AddDefectID),ID(DefectID)  //ID_Defect is ID of next entity in CP_Defect relation
   {}
   void Add(UnicodeString KindNa,UnicodeString AddDescr,int KindNr, int NumberOfElements,int AddDefectID);
   void Add(UnicodeString KindNa,UnicodeString AddDescr,int KindNr, int NumberOfElements,int AddDefectID,int ID_Def);
   UnicodeString GetKindName();//{return KindName;}
   UnicodeString GetAddidtionalDescription();//{return AdditionalDescription;}
   bool operator==(TDefectDetail Def)
   {
	 return (Def.Kind==Kind) && (Def.AddDefID==AddDefID) ;
   }
   int GetNumberOfDefects() const {return Number; }
   int GetKindNumber() { return Kind; }
   int GetAddDefID() { return AddDefID; }
   TDefectDetail IncreaseNumber(int NoOfDef = 1) {if (NoOfDef>0) Number += NoOfDef; return *this; }
   void Reset() { Number =0; KindName=0; AdditionalDescription=0; Kind=0;AddDefID=0; }
   void AddAddDeffID(int AddID) { AddDefID = AddID; }
   void AddID(int ID_Def) {ID = ID_Def;}
   int GetEntityID() {return ID; }


};


class TDefect
{
   TSubstrate* Substrate;
   TCoating* Coating;
   TDefectDetail Defect;
public:
   TDefect(): Substrate(0), Coating(0)
   {}

};



//class TDefectsSummary
//{
//   std::list<TDefectDetail> DefectsList;
//   double SummaryArea;
//   std::list<TDefectDetail>::iterator it;
// public:
//   TDefectsSummary(): SummaryArea(0), it(0) {}
//   void Add(UnicodeString KindNa,UnicodeString AdditionalN,int KindNr, int NumberOfElements,int AddNr);
//   void Clear(){SummaryArea=0;DefectsList.clear();it=0;}
//
//   //Methode returns next defect from list, after reaching last defects keep returning last element
//   TDefectDetail GetNextDefect();
//
//
//   TDefectDetail GetCurrentDefect();
//
//
//   bool NextEfectExists();
//   bool StartIteration();
//};

struct TTempDefect
{
	double SumArea;
	double SumAreaDevelopment;
	TTempDefect(): SumArea(0), SumAreaDevelopment(0) {}
};





class TDefectExpanded
{
protected:
	TSubstrate Substrate;
	TCoating	 Coating;
	TDefectDetail Defect;
	UnicodeString Comment;
	TDateTime Date;
public:
	TDefectExpanded():Comment("")
	{}
	void Add(TSubstrate Sub, TCoating Coat)
	{
	  Substrate = Sub;
	  Coating = Coat;
	}
	void Add(TSubstrate Sub, TCoating Coat,TDefectDetail Def, UnicodeString Com, TDateTime D)
	{
	  Substrate = Sub;
	  Coating = Coat;
	  Defect = Def;
	  Comment=Com;
	  Date = D;
	}

	bool operator==(const TDefectExpanded& SubExp)
	{
	   bool DefectComparision = true;
	   if (Defect.GetKindNumber()!=0) { //if Defect exists;
			DefectComparision = (Defect == SubExp.GetDefect()) ;
	   }
	   return (Substrate == SubExp.Substrate) && (Coating == SubExp.Coating) && DefectComparision;
	}

	TDefectExpanded operator=(const TDefectExpanded& Sub)
	{
		Substrate = Sub.GetSubstrate();
		Coating = Sub.GetCoating();
		Defect = Sub.GetDefect();
		Date = Sub.GetDate();
		Comment = Sub.Comment;
//		if(Sub.Comment==0)
//		{
//		  Comment = 0;
//		}
//		else
//		{
//		 if(Comment!=0)
//			delete Comment;
//		 Comment = new TStringList(Sub.Comment);
//		}
		return *this;
	}

	TSubstrate GetSubstrate()const { return Substrate; }
	TCoating GetCoating()const { return Coating; }
	TDefectDetail GetDefect() const { return Defect; }
	UnicodeString GetComment() const { return Comment; }
	void AddSubstrate(TSubstrate Sub) {Substrate = Sub ; }
	void AddCoating(TCoating Coat) {Coating = Coat; }
	void AddDefect(TDefectDetail Def) {Defect = Def; }
	void AddComment(UnicodeString Str)
	{
		 Comment = Str;
	}

	TDefectExpanded IncreaseNumber(int NoOfDef=1)
	{
		Defect.IncreaseNumber(NoOfDef);
		return *this;
	}

	int GetNumberOfDefects() const { return Defect.GetNumberOfDefects(); }
	TDateTime GetDate() const { return Date; }
	void AddDate(TDateTime Dat) { Date = Dat; }
	void Reset() {
		 Comment="";
		 Date=0;
		 Defect.Reset();
		 Coating.Reset();
		 Substrate.Reset();
	}

	void AddAdditionalDefect(int AddDefID)
	{
	  Defect.AddAddDeffID(AddDefID);
    }
};



class TSummaryDefectExpanded
{
  std::vector<TDefectExpanded> DefectExpanded;
  double SumArea;     //it is only auxiliary variable - it do not contain current summary arrea - this quantity is each time calculated by menthod
  std::vector<TDateTime> Dates;
  std::vector<UnicodeString> Comments;
  //std::map<TDateTime, UnicodeString> CommentsMap;
public:
  TSummaryDefectExpanded():SumArea(0), Dates(0), Comments(0)
  {
		//DefectExpanded.Reset();
		DefectExpanded.resize(0);

  }
  bool operator==(const TDefectExpanded& Def)
  {
	if (DefectExpanded.size()==0) {
		  return false;
	}
	return DefectExpanded[0] == Def;
  }
  TDefectExpanded operator=(const TDefectExpanded& Sub)
					{
					  //int No = Sub.GetNumberOfDefects();
					  //SumArea += No * Sub.GetSubstrate().GetAreaM2();
					  int Size = DefectExpanded.size();
					  DefectExpanded.resize(Size+1);
					  DefectExpanded[Size]=  Sub;
					  //DefectExpanded[0].IncreaseNumber(No);
					  return DefectExpanded[Size];

					 // Dates.push_back(Sub.GetDate());
					 // Comments.push_back(Sub.GetComment());
//					  if(DefectExpanded.GetNumberOfDefects() ==0 ) //First assignment
//					  {
//							return DefectExpanded = Sub;
//					  }
//					  else
//					  {
//							DefectExpanded.IncreaseNumber(No);
//							return DefectExpanded;
//					  }

					}
  void push_back(const TDefectExpanded& Def)
  {
	*this = Def;
  }
  //TDefectExpanded& GetDefExpanded() { return DefectExpanded; }
  TDefectExpanded& GetDefExpanded() { return DefectExpanded[0]; }
  TDefectExpanded& GetDefExpanded(int i) { return DefectExpanded[i]; }
  double GetSummaryArea();// { return SumArea; }
  int GetNoOfDefects();
  void Reset() { DefectExpanded.resize(0); SumArea=0;} //Dates.clear(); //Comments.clear(); }
  void Delete(int i) {DefectExpanded.erase(DefectExpanded.begin()+i);}
  int NumberOfElements() { return DefectExpanded.size(); }
  UnicodeString SummarizeString();
  //const std::vector<TDateTime> ReadDateVector() const { return Dates; }
  //const std::vector<UnicodeString> ReadCommentsVector() const {return Comments; }
};


class TSummarizedEntitiesVector: public std::vector<TSummaryDefectExpanded>
{
   unsigned int MaxRow;
public:
	  TSummarizedEntitiesVector():MaxRow(0)
	  {      }
	  TSummarizedEntitiesVector(int StartUp):MaxRow(0)
	  { //std::vector<TSummaryDefectExpanded>::std::vector<TSummaryDefectExpanded>(StartUp);
		TSummaryDefectExpanded empty;
		for(int i=0;i<StartUp;i++)
		{
		   std::vector<TSummaryDefectExpanded>::push_back(empty);
		}
	   //	assign(StartUp,empty);
	  }

	  reference operator[](unsigned int index)
	  {
		unsigned int Size = this->size();
		if(Size<(index+1))
		{
		  (*this).resize(Size+(index-Size+1));
		}

		if (MaxRow<index && index >=0 ) {
		  //if(std::vector<TSummaryDefectExpanded>::operator[](index-1).GetDefExpanded().GetDefect().GetKindNumber()!=0 ) //do not change MaxRow when initializing;
			MaxRow = index;
		}
		return std::vector<TSummaryDefectExpanded>::operator[](index);
	  }

	  iterator erase(iterator where)
	  {
		MaxRow--;
		return std::vector<TSummaryDefectExpanded>::erase(where);
	  }
	  iterator erase(iterator From, iterator To)
	  {
		MaxRow =  To-From;
		return std::vector<TSummaryDefectExpanded>::erase(From,To);
	  }
	  void clear()
	  {
		MaxRow=0;
		std::vector<TSummaryDefectExpanded>::clear();
		this->resize(0);
	  }

	  void push_back(const TDefectExpanded& Def)
	  {
		int Size = (*this).size();
		for (int i = 0; i < Size; i++)
		{
		   if ((*this)[i]== Def) {
			  (*this)[i]=Def;
			  return;
		   }
		}
		//std::vector<TSummaryDefectExpanded>::push_back(Def);
	   (*this)[Size]= Def;  // No the same DefectsExpanded were found
      }


	  unsigned int GetMaxRow() { return MaxRow; }
};

extern TDefectExpanded CurrentSubCoat;
extern std::vector<TDefectExpanded> ChoosenSubCoats;
extern std::vector<TDefectExpanded> Entities;
//extern std::vector<TSummaryDefectExpanded> SummarizedEntities;  //     TSummarizedEntitiesVector
extern TSummarizedEntitiesVector SummarizedEntities;



#endif
