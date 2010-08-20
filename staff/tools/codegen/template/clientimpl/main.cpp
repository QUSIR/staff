// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Client skeleton

#var HasAsynchOps
#foreach $(Project.Interfaces)
#foreach $(Interface.Classes)
#foreach $(Class.Members)
#foreach $(Member.Params)
#ifeq($(Param.DataType.Name),ICallback)
#var HasAsynchOps true
#ifeqend
#end
#end
#end
#end
\
#include <memory>
#include <rise/common/Log.h>
#ifeq($($HasAsynchOps),true)
#include <rise/threading/Thread.h>
#include <staff/common/DataObject.h>
#include <staff/client/ICallback.h>
#ifeqend
#include <staff/common/Exception.h>
#include <staff/client/ServiceFactory.h>
#foreach $(Project.Interfaces)
#include "$(Interface.Name).h"
#end

#foreach $(Project.Interfaces)
#ifneq($(Interface.Classes.$Count),0)
// callbacks
#foreach $(Interface.Classes)
#foreach $(Class.Members)
#ifneq($(Member.Params.$Count),0)
#foreach $(Member.Params)
#ifeq($(Param.DataType.Name),ICallback)
#var HasAsynchOps true
// callback for $(Class.Name)::$(Member.Name)
class $(Class.Name)$(Member.Name)Callback: public staff::ICallback< $(Param.DataType.TemplateParams.TemplateParam1) >
{
public:
  virtual void OnComplete($(Param.DataType.TemplateParams.TemplateParam1) tResult)
  {
    // process result here
    // rise::LogInfo() << "$(Member.Name)(asynch) result: " << tResult;
  }

  void OnFault(const staff::CDataObject& rFault)
  {
    // process error here
    rise::LogError() << rFault.ToString();
  }
};

#ifeqend // icallback
#end // member.params
#ifeqend // member.params
#end  // class.members
#end  // interface.classes
#ifeqend // ifneq((Interface.Classes.Count),0)
#end // Project.Interfaces


int main(int nArgs, const char* paszArgs[])
{
  try
  {
#foreach $(Project.Interfaces)
#ifneq($(Interface.Classes.$Count),0)
#foreach $(Interface.Classes)
    std::auto_ptr< $(Class.NsName) > p$(Class.ServiceName)(::staff::CServiceFactory::Inst().GetService< $(Class.NsName) >());

    RISE_ASSERTS(p$(Class.ServiceName).get(), "Cannot get client for service $(Class.ServiceNsName)!");

    // Invoke Your service here:
#foreach $(Class.Members)

#ifeq($(Member.IsAsynch),0) // blocking call
#ifneq($(Member.Return.NodeName),)
#var VarName $(Member.Return.NodeName)
#else
#var VarName t$(Member.Name)Result
#ifeqend
    // \
#ifneq($(Member.Return.Name),void)
$(Member.Return.NsName) $($VarName) = \
#ifeqend
p$(Class.ServiceName)->$(Member.Name)(\
#foreach $(Member.Params)
#ifneq($(Param.$Num),0)
, \
#ifeqend
$(Param.Name)\
#end // Member.Params
);
#ifneq($(Member.Return.Name),void)
    // rise::LogInfo() << "$(Member.Name) result: " << $($VarName);
#else
    // rise::LogInfo() << "$(Member.Name) called";
#ifeqend
#else // non blocking call
    // $(Class.Name)$(Member.Name)Callback t$(Class.Name)$(Member.Name)Callback;
    // p$(Class.ServiceName)->$(Member.Name)(\
#foreach $(Member.Params)
#ifneq($(Param.$Num),0)
, \
#ifeqend
#ifeq($(Param.DataType.Name),ICallback)
t$(Class.Name)$(Member.Name)Callback\
#else
$(Param.Name)\
#ifeqend // ICallback
#end // Member.Params
);

    // // Wait for asynch call is completed
    // // Please note, that you cannot call any operation of this client
    // // until this asynchronous request is completed.
    // // To call operation while request is processing, please use another copy of client.
    // while (!t$(Class.Name)$(Member.Name)Callback.IsCompleted())
    // {
    //   rise::threading::CThread::Sleep(1000);
    // }

#ifeqend // blocking call
#end // class.members
#end // Interface.Classes
#ifeqend // (Interface.Classes.Count),0
#end // Project.Interfaces
  }
  RISE_CATCH_ALL

  return 0;
}
