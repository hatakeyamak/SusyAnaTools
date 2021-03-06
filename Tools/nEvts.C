#include "samples.h"
#include "NTupleReader.h"

#include "TFile.h"
#include "TTree.h"
#include "TChain.h"

#include <iostream>
#include <sstream>
#include <getopt.h>

int main(int argc, char *argv[])
{
  int opt;
  int option_index = 0;
  static struct option long_options[] = {
      {"negw",     no_argument, 0, 'w'},
      {"skipData", no_argument, 0, 's'}
  };

  bool getNegWeights = false, skipData = false;
  while((opt=getopt_long(argc, argv, "ws", long_options, &option_index)) != -1)
  {
    switch(opt)
    {
      case 'w':
      getNegWeights = true;
      break;

      case 's':
      skipData = true;
      break;
    }
  }
  
  if(getNegWeights)
  std::cout << "Will compute negative weight fraction" << std::endl;
    
  AnaSamples::SampleSet        ss("sampleSets.txt");
  AnaSamples::SampleCollection sc("sampleCollections.txt", ss);

  std::string selKeyStr;
  if(argc >= optind+1)
  {
    selKeyStr = argv[optind];
    std::cout << "selKeyStr : " << selKeyStr << std::endl;
  }

  std::stringstream ssSelKey(selKeyStr);
  std::string buf;
  std::vector<std::string> keyStrVec;
    
  while(ssSelKey >> buf) keyStrVec.push_back(buf);

  for(auto& file : ss) 
  {
    if( !keyStrVec.empty() )
    {
      bool found = false;
      for(auto& str : keyStrVec ){ if( file.first.find(str) !=std::string::npos ) found = true; }
      if( !found ) continue;
    }
    if(skipData && file.first.find("Data") != std::string::npos)
    {
        std::cout << "Skipping " << file.first << std::endl;
        continue;
    }

    TChain *t = new TChain(file.second.treePath.c_str());
    file.second.addFilesToChain(t);
    
    int nEntries = t->GetEntries();
    if(nEntries != file.second.nEvts)
        std::cout << "DIFFERENT:\t";
    std::cout << "Processing file(s): " << file.second.tag << "\t" << file.second.filePath + file.second.fileName << "\t" << nEntries << "\twas: " << file.second.nEvts  << std::endl;

    if(getNegWeights)
    {
      NTupleReader tr = NTupleReader(t);
    
      int negw = 0;
      int posw = 0;
      while(tr.getNextEvent())
      {
        double stored_weight = 0;
        if( tr.checkBranch("stored_weight") )
        {
          stored_weight = tr.getVar<double>("stored_weight");
        }
        else if( tr.checkBranch("Weight") )
        {
          stored_weight = tr.getVar<double>("Weight");
        }
        else
        {
          std::cout<<"No weight information in NTupleReader object"<<std::endl;
          break;
        }
        if(stored_weight > 0) ++posw;
        else ++negw;
      }
      std::cout << "Processing file(s): " << file.second.tag << "\t" << file.second.filePath + file.second.fileName << "\t" << "Neg weigths = " << negw << ", Pos weights = " << posw << std::endl;
    }
  }   
}
