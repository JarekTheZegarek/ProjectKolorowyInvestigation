//---------------------------------------------------------------------------


#pragma hdrstop

#include "UnitTryby.h"
#include "Global.h"


 /* TODO : Add to the UnitTryby (modes) the possibility
of mulitple modes simultanous selection */
//extern TForm3* Form3;
TTryb Mode;
//TTryb SearchTryb(LastID);
//TTryb DeviceMode;
//TTryb MeasurementsMode; //Trend or profile
//TTryb OpticType; // transmision, reflection
//TTryb TemperatureMode; // baked (tempered), annealed (relaxed)


void PopulateAllowedModes()
{
   for (int i=0; i < LastTryb-1; i++) {
	  Mode.AddAllowed(Tryby(i));
   }

//	Mode.AddAllowed(TestingMode);
//    Mode.AddAllowed(Editing);
//    Mode.AddAllowed(NonEditing);
//    Mode.AddAllowed(ModeLocal);
//    Mode.AddAllowed(ModeReport);
 //   Modes.AddAllowed();


}

void TTryb::AddAllowed(Tryby T)
{
    AllowedModes.push_back(T);
}

//std::list<int> cos;
//list<int> cos;
TTryb::TTryb(void): Tryb(Editing)
{
        Set(Editing);
}

bool TTryb::CheckIfAllowed(Tryby T)
{

   if(find(AllowedModes.begin(),AllowedModes.end(),T)!=AllowedModes.end())
   {
       return true;
   }
   else
   {
       return false;
   }
}

bool TTryb::Set(Tryby T)
	{   if(CheckIfAllowed(T))
    	{
    		Tryb = T;
            return true;
        }
        else
        {
           return false;
        }
    }

void TTryb::SetRadio(int Mode)
{
//   switch (Mode)
//   {
//       case 0: Set(Spectra);
//        	break;
//       case 1: Set(Astar);
//        	break;
//       case 2: Set(Bstar);
//        	break;
//       case 3: Set(Lstar);
//        	break;
//       case 4: Set(Ystar);
//        	break;
//       default: Set(NoSpectra);
//   }

}
//---------------------------------------------------------------------------

#pragma package(smart_init)
