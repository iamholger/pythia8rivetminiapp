#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC3.h"
#include "HepMC3/HepMC3.h"


using namespace std;
using namespace Pythia8;

void run(size_t nEvents=10000, int seed=1234, bool verbose=true)
{
  Pythia pythia;

  if (!verbose) pythia.readString("Print:quiet = on");
  pythia.readString("Main:numberOfEvents = " + std::to_string(nEvents));
  pythia.readString("Main:timesAllowErrors = 3");

  // Extract settings to be used in the main program.
  int    nEvent    = pythia.mode("Main:numberOfEvents");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");

  // Hard coded physics setup
  pythia.readString("Random:setSeed = on");
  pythia.readString("Random:seed = " + std::to_string(seed));
  pythia.readString("Beams:idA = 2212");
  pythia.readString("Beams:idB = 2212");
  pythia.readString("Beams:eCM = 100000");
  pythia.readString("Top:all = on");
  

  // Initialization.
  pythia.init();
  Event& event = pythia.event;
  HepMC3::Pythia8ToHepMC3 ToHepMC;

  
  // Begin event loop.
  int iAbort = 0;
  for (int iEvent = 0; iEvent < nEvent; ++iEvent)
  {

    // Generate event.
    if (!pythia.next()) 
    {

      // First few failures write off as "acceptable" errors, then quit.
      if (++iAbort < nAbort) continue;
      cout << " Event generation aborted prematurely, owing to error!\n";
      break;
    }

    HepMC3::GenEvent* hepmcevt = new HepMC3::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );


    delete hepmcevt;
    if (verbose && iEvent%1000 == 0) cerr << iEvent << "/" << nEvent << "\n";

  }
  if (verbose) pythia.stat();

}


int main(int argc, char* argv[]) {

    run();

    return 0;
}
