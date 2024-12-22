//---------------------------------------------------------------------------


#pragma hdrstop

#include "DataBooth.h"
#include "vcl.h"
#include "UnitProductsBooth.h"
//---------------------------------------------------------------------------
#pragma package(smart_init)
#pragma classgroup "System.Classes.TPersistent"
#pragma resource "*.dfm"
TDataModule2 *DataModule2;
//---------------------------------------------------------------------------
__fastcall TDataModule2::TDataModule2(TComponent* Owner)
	: TDataModule(Owner)
{
}

void DeleteEntity(int EntityID)
{
	try {
	  try {
		if(!DataModule2->ADOConnectionCreport->Connected)
		{
		   DataModule2->ADOConnectionCreport->Connected=true;
		}
		TADOQuery* Query = DataModule2->ADOQueryCreport;
		UnicodeString QueryString;

		QueryString = "DELETE FROM CP_DEFECT WHERE [ID_DEFECT] = :EntityID";
		Query->SQL->Clear();
		Query->Parameters->Clear();
		Query->SQL->Add(QueryString);
		Query->Parameters->ParamByName("EntityID")->Value=EntityID;
		int NoOfRowAffected = Query->ExecSQL();
		if (NoOfRowAffected==0) {
		   throw int(0);
		}

	  } catch (...) {
		  ShowMessage("B³¹d w funkcji DeleteEntity(); Sorki \n\nError in function DeleteEntity()");
		  throw;
	  }
	}
	__finally {
		//DataModule2->ADOConnectionCreport->Connected=false;
	}
}

void UpdateComment(int EntityID, UnicodeString Comment)
{
  try {
	  try {
		if(!DataModule2->ADOConnectionCreport->Connected)
		{
		   DataModule2->ADOConnectionCreport->Connected=true;
		}
		TADOQuery* Query = DataModule2->ADOQueryUpdateComment;
		UnicodeString QueryString;

//		QueryString = 	" UPDATE CP_DEFECTEXPANDED "
//						" SET  COMMENT = :Comment "
//						" WHERE ID_DEFECT = :EntityID ";
//		Query->SQL->Clear();
//		Query->Parameters->Clear();
//		Query->SQL->Add(QueryString);
		Query->Parameters->ParamByName("EntityID")->Value=EntityID;
		Query->Parameters->ParamByName("Comment")->Value= Comment;
		int NoOfRowAffected = Query->ExecSQL();
		if (NoOfRowAffected==0) {
		   throw int(0);;
		}

	  } catch (...) {
		  ShowMessage("B³¹d w funkcji UpdateComment(); Sorki \n\nError in function UpdateComment()");
		  throw;
	  }
	}
	__finally {
		//DataModule2->ADOConnectionCreport->Connected=false;
	}

}


void UpdateEntity(TDefectExpanded Def)
{
  try {
	  try {
		if(!DataModule2->ADOConnectionCreport->Connected)
		{
		   DataModule2->ADOConnectionCreport->Connected=true;
		}
		TADOQuery* Query = DataModule2->ADOQueryCreport;
		int EntityID =  Def.GetDefect().GetEntityID();
		UnicodeString QueryString;
		QueryString = 	" UPDATE CP_DEFECT "
						" SET "
						" SIZEID = :SizeID, "
						" DEFECTID = :DefectID "
//						" , QUANTITY = :Quantity "
						" WHERE ID_DEFECT = :EntityID ";
		Query->SQL->Clear();
		Query->Parameters->Clear();
		Query->SQL->Add(QueryString);
		Query->Parameters->ParamByName("EntityID")->Value= EntityID;
		Query->Parameters->ParamByName("SizeID")->Value  = Def.GetSubstrate().GetSizeID();
		Query->Parameters->ParamByName("DefectID")->Value= Def.GetDefect().GetKindNumber();
//		Query->Parameters->ParamByName("Quantity")->Value= Def.GetDefect().GetNumberOfDefects();

		int NoOfRowAffected = Query->ExecSQL();
		if (NoOfRowAffected!=1) {
		   throw int(0);;
		}

		QueryString = 	" Select ID_DEFECT From CP_DEFECTEXPANDED "     //If there is no data in CP_DEFECTEXPANDED - Older data
						"  WHERE ID_DEFECT = :EntityID ";
		Query->SQL->Clear();
		Query->Parameters->Clear();
		Query->SQL->Add(QueryString);
		Query->Parameters->ParamByName("EntityID")->Value=Def.GetDefect().GetEntityID();

		Query->Open(); //ExecSQL();
		NoOfRowAffected  = Query->RecordCount;
		if (NoOfRowAffected==0) {   //No ID found
		   QueryString = 	" INSERT INTO CP_DEFECTEXPANDED  "
							" (ID_DEFECT) VALUES (:EntityID) ";
		   Query->SQL->Clear();
		   Query->Parameters->Clear();
		   Query->SQL->Add(QueryString);
		   Query->Parameters->ParamByName("EntityID")->Value=Def.GetDefect().GetEntityID();

		   NoOfRowAffected  = Query->ExecSQL();
		   if (NoOfRowAffected!=1) {
		   		throw int(0);;
			}

		}


		QueryString = 	" UPDATE CP_DEFECTEXPANDED "
						" SET  "
						"  ID_PRODTYPE = :ProdTypeID, "
						"  ID_SUBSTRATE = :SubstrateID, "
						"  ID_SUBSTRATECLASS = :SubClassID, "
						"  ID_THICKNESS = :ThicknessID, "
						"  ID_ADDITIONALDEFECT = :AdditionalDefectID "
						"  WHERE ID_DEFECT = :EntityID ";
		Query->SQL->Clear();
		Query->Parameters->Clear();
		Query->SQL->Add(QueryString);
		Query->Parameters->ParamByName("EntityID")->Value=Def.GetDefect().GetEntityID();
		Query->Parameters->ParamByName("ProdTypeID")->Value= Def.GetCoating().GetID();
		Query->Parameters->ParamByName("SubstrateID")->Value=Def.GetSubstrate().GetID();
		Query->Parameters->ParamByName("SubClassID")->Value= Def.GetSubstrate().GetSubClassID();
		Query->Parameters->ParamByName("ThicknessID")->Value=Def.GetSubstrate().GetThicknessID();
		Query->Parameters->ParamByName("AdditionalDefectID")->Value= Def.GetDefect().GetAddDefID();
		NoOfRowAffected = Query->ExecSQL();
		if (NoOfRowAffected==0) {
		   throw int(0);
		}

	  } catch (...) {
		  ShowMessage("B³¹d w funkcji UpdateEntity();\nSorki :-(\n\nError in function UpdateComment()");
		  throw;
	  }
	}
	__finally {
		//DataModule2->ADOConnectionCreport->Connected=false;
	}




}


//---------------------------------------------------------------------------
