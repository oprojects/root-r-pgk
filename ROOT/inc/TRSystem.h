// @(#)root/r:$Id$
// Author: Omar Zapata   11/06/2014


/*************************************************************************
 * Copyright (C)  2014, Omar Andres Zapata Mesa                          *
 * All rights reserved.                                                  *
 *                                                                       *
 * For the licensing terms see $ROOTSYS/LICENSE.                         *
 * For the list of contributors see $ROOTSYS/README/CREDITS.             *
 *************************************************************************/
#ifndef ROOT_R_TRSYSTEM
#define ROOT_R_TRSYSTEM

#ifndef ROOT_R_RExports
#include<RExports.h>
#endif

#ifndef ROOT_TSystem
#include<TSystem.h>
#endif

#ifndef ROOT_TThread
#include<TThread.h>
#endif

#ifndef ROOT_TApplication
#include<TApplication.h>
#endif

#include<TROOT.h>
#include<TGlobal.h>
#include<RCppyy.h>

//________________________________________________________________________________________________________
/**
   This is a gSystem wrap for R


   @ingroup R
*/
namespace ROOT {
   namespace R {

      class TRSystem: public TObject {
      private:
         TThread *th;
      public:
         TRSystem();
         ~TRSystem() {
            if (th) delete th;
         }
         void ProcessEventsLoop();
         Int_t   Load(TString module);
         void AddIncludePath(TString path);
         ptrdiff_t GetObject(TString name);
      };
   }
}


ROOTR_EXPOSED_CLASS_INTERNAL(TRSystem)


//______________________________________________________________________________
ROOT::R::TRSystem::TRSystem(): TObject()
{
   th = nullptr;
}

//______________________________________________________________________________
void ROOT::R::TRSystem::ProcessEventsLoop()
{
   if (!gApplication) {
      Error("TRSystem", "Running ProcessEventsLoop without global object gApplication.");
      return;
   }
   th = new TThread([](void * args) {
      while (kTRUE) {
         gSystem->ProcessEvents();
         gSystem->Sleep(100);
      }
   }, (void *)this);
   th->Run();
}

//______________________________________________________________________________
Int_t ROOT::R::TRSystem::Load(TString module)
{
   return gSystem->Load(module.Data());
}

//______________________________________________________________________________
void ROOT::R::TRSystem::AddIncludePath(TString path)
{
    gSystem->AddIncludePath(path.Data());
}

//______________________________________________________________________________
 ptrdiff_t ROOT::R::TRSystem::GetObject(TString name)
{
    auto collection=gROOT->GetListOfGlobals();
    auto obj=(TGlobal*)collection->FindObject(name.Data());
    return ( ptrdiff_t)obj->GetAddress ();
}

ROOTR_MODULE(ROOTR_TRSystem)
{

   ROOT::R::class_<ROOT::R::TRSystem>("TRSystem", "TSystem class to manipulate ROOT's Process.")
   .constructor()
   .method("ProcessEventsLoop", &ROOT::R::TRSystem::ProcessEventsLoop)
   .method("Load", (Int_t(ROOT::R::TRSystem::*)(TString))&ROOT::R::TRSystem::Load)
   .method("AddIncludePath", (void(ROOT::R::TRSystem::*)(TString))&ROOT::R::TRSystem::AddIncludePath)
   .method("GetObject", ( ptrdiff_t(ROOT::R::TRSystem::*)(TString))&ROOT::R::TRSystem::GetObject)
   ;
}

#endif
