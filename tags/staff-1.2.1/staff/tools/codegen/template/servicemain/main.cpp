// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Stub

#include <rise/common/ExceptionTemplate.h>
#include <rise/common/Log.h>
#include <rise/string/String.h>
#include <staff/service/ServiceDispatcherClient.h>
#foreach $(Project.Interfaces)
#include "$(Interface.Name)Wrapper.h"
#end

int main(int nArgs, const char* paszArgs[])
{
#foreach $(Project.Interfaces)
  try
  {
    rise::CString sSessionId;
    rise::CString sTmp;
    for(int i = 1; i < nArgs; ++i)
    {
      if(paszArgs[i] != NULL)
      {
        sTmp = paszArgs[i];
        if(sTmp.substr(0, 2) == _T("-s"))
          sSessionId = sTmp.substr(2);
      }
    }
#foreach $(Interface.Classes)
    {
      $(Class.NsName)Wrapper t$(Class.ServiceName)Wrapper;

      t$(Class.ServiceName)Wrapper.SetSessionId(sSessionId);
      staff::CServiceDispatcherClient::ServiceDispatcherClient().SetWrapper(t$(Class.ServiceName)Wrapper);
      staff::CServiceDispatcherClient::ServiceDispatcherClient().Start();
    }
#end
  }
  RISE_CATCH_ALL
#end

  return 0;
}
