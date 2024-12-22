//---------------------------------------------------------------------------

#ifndef UnitTrybyH
#define UnitTrybyH
#include <list>
#include <algorithm>
using namespace std;
enum  Tryby{TestingMode, Editing, NonEditing, ModeLocal, ModeReport, ModePrevious, \
			ModeCurrent,															\
			LastTryb}; 					// LastTryb is not valid mode it means last of enumerators to iterate easily through them.
										// LastTryb should always be last item of Tryby
										// ModePrevious means using old style operators indication (only supervisor)
										// default ModeCurrent means using Supervisor, Process and Booth operators names


class TTryb            // Class setting given working modes
{

  Tryby Tryb;             //mode
  list<int> AllowedModes;
  bool CheckIfAllowed(Tryby T);
public:
  bool Set(Tryby T) ;
  Tryby Get() { return Tryb; }
  void Set(void);
  void AddAllowed(Tryby T);
  void SetRadio(int);  // Set(ItemIndex) from radio group
  TTryb(void);
  TTryb(Tryby T):Tryb(T){}


};


//---------------------------------------------------------------------------
#endif
