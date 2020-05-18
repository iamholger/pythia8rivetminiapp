
#include "YODA/ReaderYODA.h"
#include "YODA/WriterYODA.h"
#include "YODA/AnalysisObject.h"

#include "Pythia8/Pythia.h"
#include "Pythia8Plugins/HepMC2.h"
#include "Rivet/AnalysisHandler.hh"

#include "HepMC/IO_GenEvent.h"


using namespace std;
using namespace Pythia8;
using namespace Eigen;

ArrayXXd run(ArrayXd const & params, size_t nEvents=10000, bool verbose=false, bool writeYODA=false)
{
  Pythia pythia;

  if (!verbose) pythia.readString("Print:quiet = on");
  pythia.readString("Main:numberOfEvents = " + std::to_string(nEvents));
  pythia.readString("Main:timesAllowErrors = 3");

  // Extract settings to be used in the main program.
  int    nEvent    = pythia.mode("Main:numberOfEvents");
  int    nAbort    = pythia.mode("Main:timesAllowErrors");

  // Hard coded physics setup
  pythia.readString("Beams:idA = 11");
  pythia.readString("Beams:idB = -11");
  pythia.readString("Beams:eCM = 91.2");
  pythia.readString("WeakSingleBoson:ffbar2gmZ = on");
  pythia.readString("23:onMode = off");
  pythia.readString("23:onIfAny = 1 2 3 4 5");
  pythia.readString("PDF:lepton = off");
  pythia.readString("SpaceShower:QEDshowerByL = off");
  pythia.readString("ParticleDecays:limitTau0 = On");
  pythia.readString("ParticleDecays:tau0Max = 100.0");
  
  pythia.readString("StringZ:aLund = "  + std::to_string(params[0]));
  pythia.readString("StringZ:bLund = "  + std::to_string(params[1]));
  pythia.readString("StringPT:sigma = " + std::to_string(params[2]));

  // Initialization.
  pythia.init();
  Event& event = pythia.event;
  HepMC::Pythia8ToHepMC ToHepMC;

  if (!verbose)  Rivet::Log::setLevel("Rivet", Rivet::Log::ERROR);
  Rivet::AnalysisHandler* ah = new Rivet::AnalysisHandler();
  ah->addAnalysis("ALEPH_1997_I427131");
  
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

    HepMC::GenEvent* hepmcevt = new HepMC::GenEvent();
    ToHepMC.fill_next_event( pythia, hepmcevt );

    try {ah->analyze( *hepmcevt ) ;} catch (const std::exception& e)
    {
      cerr << "Exception in Rivet::analyze: " << e.what() << "\n";
    }

    // Write the HepMC event to file. Done with it.
    delete hepmcevt;
    if (verbose && iEvent%1000 == 0) cerr << iEvent << "/" << nEvent << "\n";

  }
  if (verbose) pythia.stat();

  // Event loop is done, set xsection correctly and normalise histos
  ah->setCrossSection(pythia.info.sigmaGen() * 1.0E9, pythia.info.sigmaErr() * 1e9);
  ah->finalize();

  if (writeYODA) ah->writeData("output.yoda");

  vector<string>
      hlist = 
      {
          "/ALEPH_1997_I427131/d02-x01-y02",
          "/ALEPH_1997_I427131/d03-x01-y01",
          "/ALEPH_1997_I427131/d04-x01-y01"
      };
  vector<const YODA::Histo1D*> output;

  auto raos = ah->getRivetAOs();

  // Cast and make sure order is always the same
  for (auto hname : hlist)
  {
      for ( auto rao : raos )
      {
        rao.get()->setActiveFinalWeightIdx(0);
        if ( rao->path() == hname )
        {
            auto active=rao.get()->activeYODAPtr();
            output.push_back(dynamic_cast<const YODA::Histo1D*> (active.get()));
        }
      }
  }


  // Data wrangling

  size_t nbins(0), currbin(0);
  for (auto ao : output) nbins+= ao->numBins();

  ArrayXXd data(2,nbins);
  for (auto ao : output)
  {
      for (auto b : ao->bins()) 
      {
          data(0,currbin) = b.area();
          data(1,currbin) = b.areaErr();
          currbin++;
      }
  }


  return data;
}


int main(int argc, char* argv[]) {

    // Baseline: ./miniapp 0.68 0.98 0.335 10000 0 1
    ArrayXd params(3);
    params[0] = atof(argv[1]);
    params[1] = atof(argv[2]);
    params[2] = atof(argv[3]);
    ArrayXXd result = run(params, atoi(argv[4]), bool(atoi(argv[5])), bool(atoi(argv[6])));
    std::cerr << result.transpose() << "\n";
    return 0;
}