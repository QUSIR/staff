/*
 *  Copyright 2009 Utkin Dmitry
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 */
 
/* 
 *  This file is part of the WSF Staff project.
 *  Please, visit http://code.google.com/p/staff for more information.
 */

// This file generated by staff_codegen
// Client skeleton

#include <stdio.h>
#include <time.h>
#include <iostream>
#include <rise/threading/SharedPtr.h>
#include <rise/string/String.h>
#include <rise/common/Log.h>
#include <rise/common/ExceptionTemplate.h>
#include <rise/common/console.h>
#include <staff/common/Exception.h>
#include "ServiceFactory.h"
#include "CalcService.h"

int main(int nArgs, const char* paszArgs[])
{
  try
  {
    {
      // use anonymous account
      rise::CSharedPtr< ::samples::calc::CCalcService > pCalcService = 
        staff::CServiceFactory::Inst().GetService< ::samples::calc::CCalcService >("", "", "mycalc");

      RISE_ASSERTES(pCalcService != NULL, rise::CLogicNoItemException, "Cannot get client for service calc.CalcService!");

      // Invoke Your service here:
      rise::LogInfo() << "1 + 2 = " << pCalcService->Add(1, 2);
      
//      rise::LogInfo() << "3 - 1 = " << pCalcService->Sub(3, 1);

      rise::LogInfo() << "Saved service mem: " << pCalcService->GetMem();

      srand(static_cast<unsigned int>(time(NULL)));
      int nNewMem = rand() % 100;

      rise::LogInfo() << "Setting mem: " << nNewMem;
      pCalcService->SetMem(nNewMem);
      rise::LogInfo() << pCalcService->GetMem();
      rise::LogInfo() << "Service mem: " << pCalcService->GetMem();
      
    }
  }
  catch(const staff::CRemoteException& rEx)
  {
    rise::LogError() << rEx.GetDescr();
  }
  RISE_CATCH_ALL

//  rise::LogNotice() << "\n\n[Press Any Key...]";
//  getchar();

  return 0;
}
