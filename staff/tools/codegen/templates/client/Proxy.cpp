// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#ifneq($(Interface.Classes.$Count),0)
#include <memory>
#include <staff/utils/SharedPtr.h>
#ifeqend
#include <staff/utils/Log.h>
#include <staff/utils/fromstring.h>
#include <staff/utils/tostring.h>
#include <staff/utils/HexBinary.h>
#include <staff/utils/Base64Binary.h>
#include <staff/common/DataObject.h>
#include <staff/common/Attribute.h>
#include <staff/common/Exception.h>
#ifneq($(Interface.Classes.$Count),0)
#include <staff/common/Operation.h>
#include <staff/client/ServiceFactory.h>
#include <staff/client/IProxyAllocator.h>
#include <staff/client/ICallback.h>
#include <staff/client/Options.h>
#ifeqend // #ifneq($(Interface.Classes.$Count),0)
\
#foreach $(Interface.Structs) // headers for extern abstract types
#ifeq($(Struct.Extern),false)
#ifeq($(Struct.Options.*abstract),true||1)
#var sThisStructNsName $(Struct.NsName)
#var sThisInterfaceNsName $(Interface.NsName)
#var bThisInterfaceIncluded 0
#foreach $(Project.Interfaces)
#foreach $(Interface.Structs)
#ifeq($(Struct.ParentNsName)-$(Struct.Extern),$($sThisStructNsName)-false)
#ifneq($(Interface.NsName),$($sThisInterfaceNsName))
#ifeq($($bThisInterfaceIncluded),0)
#include "$(Interface.FilePath)$(Interface.Name)Proxy.h"
#var bThisInterfaceIncluded 1
#ifeqend
#ifeqend
#ifeqend
#end
#end
#ifeqend
#ifeqend
#end
\
#include "$(Interface.FilePath)$(Interface.Name)Proxy.h"

#cginclude <common/Serialization.cpp>

#ifeq($($c++11),true)
#var SmartPtr std::unique_ptr
#else
#var SmartPtr std::auto_ptr
#ifeqend
\
#ifneq($(Interface.Classes.$Count),0)
///////////////////////////////////////////////////////////////////////////////////////////////////////
// classes implementation

#foreach $(Interface.Classes)
$(Class.OpeningNs)

// proxy allocator
class $(Class.Name)ProxyAllocator: public staff::IProxyAllocator
{
public:
  $(Class.Name)ProxyAllocator()
  {
    try
    {
      staff::ServiceFactory::Inst().RegisterProxyAllocator(typeid($(Class.Name)).name(), *this);
    }
    STAFF_CATCH_ALL_DESCR("Failed to register proxy allocator $(Class.Name)");
  }

  virtual staff::IService* AllocateProxy(const std::string& sServiceUri,
                                         const std::string& sSessionId,
                                         const std::string& sInstanceId)
  {
    $($SmartPtr)<$(Class.Name)Proxy> tpProxy(new $(Class.Name)Proxy);
    tpProxy->Init(sServiceUri, sSessionId, sInstanceId);
    return tpProxy.release();
  }

  virtual staff::IService* AllocateProxy(const std::string& sBaseUri,
                                         const std::string& sServiceName,
                                         const std::string& sSessionId,
                                         const std::string& sInstanceId)
  {
    $($SmartPtr)<$(Class.Name)Proxy> tpProxy(new $(Class.Name)Proxy);
    tpProxy->Init(sBaseUri + (sServiceName.empty() ? "$(Class.ServiceNsName)" : sServiceName),
                  sSessionId, sInstanceId);
    return tpProxy.release();
  }
};

$(Class.Name)ProxyAllocator t$(Class.Name)ProxyAllocatorInitializer;

// asynch proxies

#foreach $(Class.Members)
#ifneq($(Member.Params.$Count),0)
#foreach $(Member.Params)
#ifeq($(Param.DataType.Name),ICallback)
// asynch proxy for $(Class.Name)::$(Member.Name)
class $(Class.Name)$(Member.Name)AsynchCallProxy: public staff::ICallback<const staff::DataObject&>
{
public:
  $(Class.Name)$(Member.Name)AsynchCallProxy($(Param.DataType) rCallback):
    m_rCallback(rCallback)
  {
  }

  virtual void OnComplete(const staff::DataObject&\
#ifneq($(Param.DataType.TemplateParams.TemplateParam1.Name),void)
 rdoResponse\
#ifeqend
)
  {
#context $(Param.DataType.TemplateParams.TemplateParam1) // callback param
#ifeq($(.Name),void)      // !!void!!
    m_rCallback.OnComplete();
#else
    $(.NsName) tReturn\
#ifeq($(.Type),generic)    // !!generic!!
 = 0\
#ifeqend
;
#indent +
#var sParam tReturn
#var sdoParam rdoResponse
#var sParamName $(Member.Options.*resultElement)
#cginclude <common/TypeDeserialization.cpp>
#indent -
    m_rCallback.OnComplete(tReturn);
#ifeqend // void
#contextend // callback
  }

  virtual void OnFault(const staff::DataObject& rFault)
  {
    m_rCallback.OnFault(rFault);
  }

  virtual void Set(axis2_callback_t* pCallback, axutil_env_t* pEnv)
  {
    m_rCallback.Set(pCallback, pEnv);
  }

private:
  $(Param.DataType) m_rCallback;
};

#ifeqend // icallback
#end // member.params
#ifeqend // member.params
#end  // class.members

// service proxy
$(Class.Name)Proxy::$(Class.Name)Proxy()
{
}

$(Class.Name)Proxy::~$(Class.Name)Proxy()
{
  try
  {
    Deinit();
  }
  STAFF_CATCH_ALL;
}

void $(Class.Name)Proxy::Init(const std::string& sServiceUri, const std::string& sSessionId, const std::string& sInstanceId)
{
  staff::IService::Init("$(Class.ServiceNsName)", sSessionId, sInstanceId);
  m_sServiceUri = !sServiceUri.empty() ? sServiceUri : \
#ifeq($(Class.Options.*serviceUri),)
"http://localhost:9090/axis2/services/$(Class.ServiceNsName)"\
#else
"$(Class.Options.*serviceUri)"\
#ifeqend
;
  m_tClient.Init(m_sServiceUri);
#foreach $(Class.Modules)
  m_tClient.EngageModule("$(Module)");
#end
  staff::Options& rOptions = m_tClient.GetOptions();
#ifneq($(Class.Options.*targetNamespace),)
  rOptions.SetDefaultNamespace("$(Class.Options.*targetNamespace)", "$(Class.Options.*targetNamespacePrefix)");
#ifeqend
#ifneq($(Class.Options.*soapVersion),)
  rOptions.SetSoapVersion(staff::Options::Soap$(Class.Options.*soapVersion.!replace/.//));
#ifeqend
#ifneq($(Class.Options.*soapVersionUri),)
  rOptions.SetSoapVersionUri("$(Interface.Options.*soapVersionUri)");
#ifeqend

  rOptions.SetSessionId(sSessionId);

  if (!sInstanceId.empty())
  {
    staff::Operation tOperation("CreateInstance");
    tOperation.Request().CreateChild("sInstanceId").SetText(sInstanceId);
    tOperation.SetResponse(m_tClient.Invoke(tOperation.Request()));
    if (m_tClient.GetLastResponseHasFault())
    {
      STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                             tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
      STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " +
                  tOperation.GetResponse().ToString()); // other fault
    }
    rOptions.SetInstanceId(sInstanceId);
  }
}

void $(Class.Name)Proxy::Deinit()
{
  if (!staff::IService::GetInstanceId().empty())
  {
    staff::Operation tOperation("FreeInstance");
    tOperation.Request().CreateChild("sInstanceId").SetText(staff::IService::GetInstanceId());
    tOperation.SetResponse(m_tClient.Invoke(tOperation.Request()));
    if (m_tClient.GetLastResponseHasFault())
    {
      STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                             tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
      STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " +
                  tOperation.GetResponse().ToString()); // other fault
    }
  }
}

staff::ServiceClient* $(Class.Name)Proxy::GetClient()
{
  return &m_tClient;
}

#foreach $(Class.Members)

$(Member.Return) $(Class.Name)Proxy::$(Member.Name)($(Member.Params))$(Member.Const)
{
#var bRestEnable 0
#var bGenerateBody 1
#var PutOptions 0
#foreach $(Member.Options) // check supported options to avoid cpp warning
#ifeq($($ThisElementName),soapAction||restEnable||restMethod||wsaAction||wsaTo||wsaFrom||wsaFaultTo||wsaUseSeparateListener||timeout)
#var PutOptions 1
#ifeqend
#end
#ifneq($($PutOptions),0)
  staff::Options& rOptions = m_tClient.GetOptions();
#ifeqend
#ifneq($(Member.Options.*soapAction),)
  rOptions.SetSoapAction("$(Member.Options.*soapAction)");
#ifeqend
#ifeq($(Member.Options.*restEnable),true||yes||1)
#var bRestEnable 1
  rOptions.EnableRest($(Member.Options.*restEnable));
#ifneq($(Member.Options.*restLocation),)
#var sRestLocation "/$(Member.Options.*restLocation)"
#foreach $(Member.Params)
#ifeq($($sRestLocation.!match/{$(Param.Name)}/),true) // replace param in url only if it exists
#ifeq($(Param.DataType.Type),string)
#var sRestLocation $($sRestLocation.!replace/{$(Param.Name)}/" + $(Param.Name) + "/)
#else
#ifeq($(Param.DataType.Type),generic)
#var sRestLocation $($sRestLocation.!replace/{$(Param.Name)}/" + staff::ToString($(Param.Name)) + "/)
#else
#ifeq($(Param.DataType.Type)|$(Param.DataType.Name),template|Optional)
#ifeq($(Param.DataType.TemplateParams.TemplateParam1.Type),string)
#var sRestLocation $($sRestLocation.!replace/$(Param.Name)={$(Param.Name)}/" + ($(Param.Name).IsNull() ? std::string() : "$(Param.Name)=" + *$(Param.Name)) + "/) \
#else
#ifeq($(Param.DataType.TemplateParams.TemplateParam1.Type),generic)
#var sRestLocation $($sRestLocation.!replace/$(Param.Name)={$(Param.Name)}/" + ($(Param.Name).IsNull() ? std::string() : "$(Param.Name)=" + staff::ToString(*$(Param.Name))) + "/) \
#else
#cgerror cannot generate REST parameter for [$(Param.Name)] type [Optional<$(Param.DataType.TemplateParams.TemplateParam1.Type)>]
#ifeqend
#ifeqend
#else
#cgerror cannot generate REST parameter for [$(Param.Name)] type [$(Param.DataType.Type)]
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#end
#var sRestLocation $($sRestLocation.!replace/ + ""//)
  rOptions.SetToAddress(m_sServiceUri + $($sRestLocation));
#else
  rOptions.SetToAddress(m_sServiceUri);
#ifeqend
#ifeqend
#ifneq($(Member.Options.*restMethod),)
#var sRestMethod $(Member.Options.*restMethod)
  rOptions.SetHttpMethod("$(Member.Options.*restMethod)");
#ifeq($($bRestEnable),1)
#ifeq($(Member.Options.*restMethod),GET)
#var bGenerateBody 0
#ifeqend // method=GET
#ifeqend // rest enable
#ifeqend
#ifneq($(Member.Options.*wsaAction),)
  rOptions.SetAction("$(Member.Options.*wsaAction)");
#ifeqend
#ifneq($(Member.Options.*wsaTo),)
  rOptions.SetToAddress("$(Member.Options.*wsaTo)");
#ifeqend
#ifneq($(Member.Options.*wsaReplyTo),)
  rOptions.SetReplyToAddress("$(Member.Options.*wsaReplyTo)");
#ifeqend
#ifneq($(Member.Options.*wsaFrom),)
  rOptions.SetFromAddress("$(Member.Options.*wsaFrom)");
#ifeqend
#ifneq($(Member.Options.*wsaFaultTo),)
  rOptions.SetFaultToAddress("$(Member.Options.*wsaFaultTo)");
#ifeqend
#ifneq($(Member.Options.*wsaUseSeparateListener),)
  rOptions.UseSeparateListener($(Member.Options.*wsaUseSeparateListener));
#ifeqend
#ifneq($(Member.Options.*timeout),)
  rOptions.SetTimeout($(Member.Options.*timeout));
#ifeqend
#ifeq($(Member.Options.*mep),||in-out)
#var SendMethod Invoke
#else
#ifeq($(Member.Options.*mep),robust out-only)
#var SendMethod SendRobust
#ifneq($(Member.Return.Name),void)
#cgerror Error: MEP is set to $(Member.Options.*mep) but return value is not void! In $(Class.Name)::$(Member.Name).
#ifeqend
#ifneq($(Member.IsAsynch),false)
#cgerror Error: MEP is set to $(Member.Options.*mep) but operation is non blocking! Non-blocking operations must be in-out. In $(Class.Name)::$(Member.Name).
#ifeqend
#else
#ifeq($(Member.Options.*mep),in-only)
#var SendMethod Send
#ifneq($(Member.Return.Name),void)
#cgerror Error: MEP is set to $(Member.Options.*mep) but return value is not void! In $(Class.Name)::$(Member.Name).
#ifeqend
#ifneq($(Member.IsAsynch),false)
#cgerror Error: MEP is set to $(Member.Options.*mep) but operation is non blocking! Non-blocking operations must be in-out. In $(Class.Name)::$(Member.Name).
#ifeqend
#else
#cgerror unsupported MEP: "$(Member.Options.*mep)". Supported MEP are: in-out, robust out-only, in-only
#ifeqend
#ifeqend
#ifeqend
\
#var tCallbackParamName
  staff::Operation tOperation("$(Member.Options.*requestElement||Member.Name)"\
#ifneq($(Member.Options.*responseElement),)
, "$(Member.Options.*responseElement)"\
#ifneq($(Member.Options.*resultElement),)
#var bWriteResp 1
#ifeq($(Member.Return.Type),template)
#ifneq($(Member.Return.Name),Optional||Nillable)
#var bWriteResp 0
#ifeqend
#ifeqend
#ifeq($($bWriteResp),1)
, "$(Member.Options.*resultElement)"\
#ifeqend
#ifeqend
#ifeqend
);
  staff::DataObject& rdoRequest = tOperation.Request();
  rdoRequest.SetNamespaceUriGenPrefix("$(Member.Options.*requestTargetNamespace||Interface.Options.*targetNamespace||"http://tempui.org/$(Class.ServiceNsName)")");
\
#ifeq($($bGenerateBody),1) // do not generate the body for REST GET method
#ifneq($(Member.Params.$Count),0)
#foreach $(Member.Params)
#ifneq($(Param.DataType.Name),ICallback)
#context $(Param.DataType)
#var sParam $(Param.Name)
#var sParamName $(Param.Name)
#ifeq($(Param.Options.*useParentElement||Member.Options.*inlineRequestElement),)
#var sdoParam rdoRequest.CreateChild("$(Param.Options.*elementName||Param.Name)")
#var sElementName
#else
#var sdoParam rdoRequest
#var sElementName $(Param.Options.*elementName||Param.Name)
#ifeqend
#cginclude <common/TypeSerialization.cpp>
#contextend
#else // ICallback
#ifneq($($tCallbackParamName),)
#cgerror Duplicate callback definition in "$(Class.Name)::$(Member.Name)"
#ifeqend
#var tCallbackParamName $(Param.Name)
#ifeqend
#end // member.params

#ifeqend // Member.Params.$Count != 0
#ifeqend // bGenerateBody
#ifeq($($tCallbackParamName),)
  // synchronous call
#ifeq($($SendMethod),Invoke)
  tOperation.SetResponse(m_tClient.$($SendMethod)(rdoRequest));
  if (m_tClient.GetLastResponseHasFault())
  {
    STAFF_ASSERT_SOAPFAULT(!tOperation.IsFault(), tOperation.GetFaultCode(),
                           tOperation.GetFaultString(), tOperation.GetFaultDetail()); // soap fault
    STAFF_THROW(::staff::RemoteException, "Failed to invoke service: " + tOperation.GetResponse().ToString()); // other fault
  }
#else
  m_tClient.$($SendMethod)(rdoRequest);
#ifeqend
\
#ifneq($(Member.Return.Name),void)      // !!void!!

  const staff::DataObject& rdoResult = tOperation.ResultOpt();
#ifneq($(Member.Return.Type),string||dataobject)
  $(Member.Return.NsName) tReturn\
#ifeq($(Member.Return.Type),generic)    // !!generic!!
 = 0\
#ifeqend
;
#ifeqend
\
#ifeq($(Member.Return.Type)-$(Member.Options.*useParentElement),dataobject-true||dataobject-1)
  return rdoResult;
#else
#ifeq($(Member.Return.Type),string||dataobject)
  return \
#ifeqend
\
#context $(Member.Return)
#var sParam tReturn
#var sParamName
#var sdoParam rdoResult
#var sElementName $(Member.Options.*resultElement)
#cginclude <common/TypeDeserialization.cpp>
#contextend
#ifeq($(Member.Return.Type),struct||typedef||template||generic||enum)
  return tReturn\
#else
#ifneq($(Member.Return.Type),string||dataobject)
#cgerror "Member.Return.Name = $(Member.Return.Name);"
#ifeqend
#ifeqend
#ifeqend
;
\
#ifeqend
#else // is asynch
  // asynchronous call
  staff::PICallback tCallback(new $(Class.Name)$(Member.Name)AsynchCallProxy($($tCallbackParamName)));
  m_tClient.Invoke(rdoRequest, tCallback);
#ifeqend
}
#end
$(Class.EndingNs)
#end
#ifeqend // #ifneq($(Interface.Classes.$Count),0)

