// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Client skeleton

#include <stdio.h>
#include <iostream>
#include <rise/threading/SharedPtr.h>
#include <rise/string/String.h>
#include <rise/common/Log.h>
#include <rise/common/ExceptionTemplate.h>
#include <rise/common/console.h>
#include <staff/common/Exception.h>
#include "ServiceFactory.h"
#include "Ticket.h"
#include "Checker.h"
#include "Issuer.h"

int main(int nArgs, const char* paszArgs[])
{
  try
  {
    {
      rise::CSharedPtr< ::samples::sharedtypes::Checker > pChecker = 
        ::staff::CServiceFactory::Inst().GetService< ::samples::sharedtypes::Checker >();

      RISE_ASSERTES(pChecker != NULL, rise::CLogicNoItemException, "Cannot get client for service samples.sharedtypes.Checker!");

      rise::CSharedPtr< ::samples::sharedtypes::Issuer > pIssuer = 
        ::staff::CServiceFactory::Inst().GetService< ::samples::sharedtypes::Issuer >();

      RISE_ASSERTES(pIssuer != NULL, rise::CLogicNoItemException, "Cannot get client for service samples.sharedtypes.Issuer!");


      ::samples::ticket::Ticket stTicket = pIssuer->Issue("me");
      pIssuer->Issue("Owner1");
      pIssuer->Issue("Owner2");
      std::cout << "issued new ticket with id: " << stTicket.nId << std::endl;

      bool tCheckResult = pChecker->Check(stTicket);
      std::cout << "check status: " << (tCheckResult ? "Not used" : "Used") << std::endl;

      std::cout << "Using ticket" << std::endl;
      pChecker->Use(stTicket);

      tCheckResult = pChecker->Check(stTicket);
      std::cout << "check status: " << (tCheckResult ? "Not used" : "Used") << std::endl;

      ::samples::sharedtypes::IssuedTicketList tGetAllTicketsResult = pChecker->GetAllTickets();
      for (::samples::sharedtypes::IssuedTicketList::const_iterator itTicket = tGetAllTicketsResult.begin();
          itTicket != tGetAllTicketsResult.end(); ++itTicket)
      {
        std::cout << "id: " << itTicket->nId << " owner: " << itTicket->sOwner << " used: " << itTicket->bUsed << std::endl;
      }
    }
  }
  catch(const staff::CRemoteException& rEx)
  {
    rise::LogError() << rEx.GetDescr();
  }
  RISE_CATCH_ALL

  return 0;
}

