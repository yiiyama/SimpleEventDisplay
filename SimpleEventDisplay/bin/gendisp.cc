#include "TFile.h"
#include "TSystem.h"

#include "DataFormats/FWLite/interface/Event.h"
#include "DataFormats/FWLite/interface/Handle.h"
#include "FWCore/FWLite/interface/AutoLibraryLoader.h"

#include "DataFormats/HepMCCandidate/interface/GenParticleFwd.h"

#include "../interface/SimpleEventDisplay.h"

#include <iostream>
#include <set>
#include <cstdlib>
#include <string>
#include <sstream>

int
main(int argc, char** argv)
{
  gSystem->Load("libFWCoreFWLite.so");
  AutoLibraryLoader::enable();

  TFile* source(TFile::Open(argv[1]));
  if(!source){
    std::cerr << "Usage: gendisp source.root event1 [event2 ..]" << std::endl;
    return 1;
  }

  std::set<unsigned> eventNumbers;
  for(int iArg(2); iArg != argc; ++iArg) eventNumbers.insert(std::atoi(argv[iArg]));

  SimpleEventDisplay display;  

  fwlite::Event event(source);
  fwlite::Handle<reco::GenParticleCollection> genParticles;

  for(event.toBegin(); !event.atEnd() && eventNumbers.size() != 0; ++event){
    std::set<unsigned>::iterator eItr(eventNumbers.find(event.id().event()));
    if(eItr == eventNumbers.end()) continue;
    eventNumbers.erase(eItr);

    genParticles.getByLabel(event, "genParticles");

    display.showEvent(event.id().event(), *genParticles.product());

    std::stringstream outputName;
    outputName << event.id().event() << ".pdf";

    display.print(outputName.str().c_str());
  }

  delete source;

  return 0;
}
