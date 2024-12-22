//---------------------------------------------------------------------------

#ifndef UnitDefectsH
#define UnitDefectsH
#include <vcl.h>
#include <list>
#include <map>

class TDefectDetail
{
   UnicodeString KindName;
   UnicodeString SizeName;
   int Kind;
   int	Number; //Quantity of defects
   double  Area;
   double Width;
   double Length;
   int AdditionalID;
   UnicodeString AdditionalInfo;
   UnicodeString AdditionalInfoEng;
   int ProdID;
   UnicodeString ProdName;
   UnicodeString FullProdName;
   UnicodeString Comment;
   int SubstrateID;
   UnicodeString SubstrateName;
   int SubstrateTypeID;
   UnicodeString SubstrateType;
   int ThicknessID;
   UnicodeString ThicknessName;
   TDateTime ShiftDate;
   char Shift;
public:
   TDefectDetail():KindName(""), Kind(0),SizeName(""),Number(0),Area(0),Width(0),Length(0)
   {}

   TDefectDetail(UnicodeString KindNa,UnicodeString SizeN,int KindNr, int NumberOfElements,double DArea): KindName(KindNa),SizeName(SizeN),Kind(KindNr),Number(NumberOfElements),Area(DArea)
   {}
   void Add(UnicodeString KindNa,UnicodeString SizeN,int KindNr, int NumberOfElements,double DArea,
			   double Width,   double Length,   int AdditionalID,   UnicodeString AdditionalInfo,
				UnicodeString AdditionalInfoEng,   int ProdID,    UnicodeString ProdName,   UnicodeString FullProdName,
				UnicodeString Comment,    int SubstrateID,   UnicodeString SubstrateName,   int SubstrateTypeID,
				UnicodeString SubstrateType,   int ThicknessID,   UnicodeString ThicknessName,
				TDateTime ShDate, char Sh);

   UnicodeString GetKindName(){return KindName;}
   UnicodeString GetSizeName(){return SizeName;}
   bool operator==(const TDefectDetail& D)
   {
	 return Kind==D.Kind && Width==D.Width && Length == D.Length && AdditionalID == D.AdditionalID && ProdID == D.ProdID && SubstrateID == D.SubstrateID && SubstrateTypeID == D.SubstrateTypeID && ThicknessID == D.ThicknessID;
   }

   bool operator!=(const TDefectDetail& D)
   {
		return !operator==(D);
   }

   bool operator<(const TDefectDetail& D);
   bool operator<=(const TDefectDetail& D)
   {
	  return *this < D || *this == D ;
   }
   bool operator>(const TDefectDetail& D)
   {
		return !(*this <= D) ;
   }
   bool operator>=(const TDefectDetail& D)
   {
		return !(*this < D)   ;
   }
   double GetArea() {return Area; }
   int GetNumber() {return Number; }
   int GetKindNumber() { return Kind; }
   void IncreaseQuantity(int Quant)
   {
		Number+=Quant;
   }
   void AddComment(UnicodeString Remark)
   {
	 if (Comment.IsEmpty()) {
		 Comment=Remark;
	 }
	 if (!Remark.IsEmpty()) {
		 Comment+="\n"+Remark;
	 }
   }
   UnicodeString GetComment()
   {
     return Comment;
   }
   UnicodeString GetAdditionalInfo() { return AdditionalInfo; }
   UnicodeString GetAdditionalInfoEng() { return AdditionalInfoEng; }
   UnicodeString GetProdName() { return ProdName; }
   UnicodeString GetFullProdName() {return FullProdName; }
   UnicodeString GetSubstrateName() {return SubstrateName; }
   UnicodeString GetSubstrateType() {return SubstrateType; }
   UnicodeString GetThicknessName() { return ThicknessName; }
   TDateTime GetDate() { return ShiftDate; }
   char GetShiftLetter() { return Shift; }
};



class TDefects
{
   std::list<TDefectDetail> DefectsList;
   double SummaryArea;
   std::list<TDefectDetail>::iterator it;
   std::map<int,std::list<TDefectDetail> > Lists;
   std::list<int> ShiftsIDListsDateOrdered;
 public:
   TDefects(): SummaryArea(0), it(0) {}
   void Add(UnicodeString KindNa,UnicodeString SizeN,int KindNr, int NumberOfElements,double DArea);
   void Add(TDefectDetail Defect);
   void Add(TDefectDetail Defect, int ShiftID);
   void Clear(){SummaryArea=0;DefectsList.clear();it=0;Lists.clear();ShiftsIDListsDateOrdered.clear();}

   //Methode returns next defect from list, after reaching last defects keep returning last element
   TDefectDetail GetNextDefect();


   TDefectDetail GetCurrentDefect();


   bool NextEfectExists();
   bool StartIteration();
};

struct TTempDefect
{
	double SumArea;
	double SumAreaDevelopment;
	TTempDefect(): SumArea(0), SumAreaDevelopment(0) {}
};
//---------------------------------------------------------------------------
#endif
