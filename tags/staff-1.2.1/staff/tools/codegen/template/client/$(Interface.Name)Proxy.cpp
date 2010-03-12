// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#include <staff/common/Operation.h>
#include <staff/common/Exception.h>
#include <staff/common/Value.h>
#include "$(Interface.Name)Proxy.h"

#cginclude <common/Serialization.cpp>

///////////////////////////////////////////////////////////////////////////////////////////////////////
// classes implementation

#foreach $(Interface.Classes)
$(Class.OpeningNs)
$(Class.Name)Proxy::$(Class.Name)Proxy()
{
}

void $(Class.Name)Proxy::Init( const std::string& sServiceUri, const std::string& sSessionId )
{
  m_tClient.Init(sServiceUri, sSessionId);
#ifneq($(Interface.TargetNamespace),)
  m_tClient.SetTargetNamespace("$(Interface.TargetNamespace)");
#else
\
#ifeqend
}
#foreach $(Class.Members)

$(Member.Return) $(Class.Name)Proxy::$(Member.Name)($(Member.Params))$(Member.Const)
{
  staff::COperation tOperation("$(Member.Name)", "$(Member.Return.ResponseName)", "$(Member.Return.NodeName)");
#ifneq($(Member.SoapAction),)

  tOperation.SetSoapAction("$(Member.SoapAction)");
#ifeqend
#foreach $(Member.Params)
  staff::CDataObject tdoParam$(Param.Name) = tOperation.Request().CreateChild("$(Param.Name)");
#ifeq($(Param.DataType.Type),generic)    // !!generic!!
  tdoParam$(Param.Name).SetValue($(Param.Name));
#else
#ifeq($(Param.DataType.Type),string)    // !!string!!
  tdoParam$(Param.Name).SetText($(Param.Name));
#else
#ifeq($(Param.DataType.Type),dataobject) // !!dataobject!! 
  tdoParam$(Param.Name).AppendChild($(Param.Name));
#else
#ifeq($(Param.DataType.Type),struct)     // !!struct!! 
  tdoParam$(Param.Name) << $(Param.Name);
#else
#ifeq($(Param.DataType.Type),typedef)    // !!typedef!!
  tdoParam$(Param.Name) << $(Param.Name);
#else
#ifeq($(Param.DataType.Type),template)    // !!template!!
  tdoParam$(Param.Name) << $(Param.Name);
#else
#cgerror "Param.DataType.Type = $(Param.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#end

  m_tClient.Invoke(tOperation);
  RISE_ASSERTES(!tOperation.IsFault(), staff::CRemoteException, tOperation.GetFaultString());

#ifeq($(Member.Return.Type),generic)    // !!generic!!
#ifneq($(Member.Return.Name),void)      // !!void!!
  return const_cast<const staff::COperation&>(tOperation).ResultValue();
#else
\
#ifeqend
#else
#ifeq($(Member.Return.Type),string)    // !!string!!
  return const_cast<const staff::COperation&>(tOperation).ResultValue();
#else
#ifeq($(Member.Return.Type),dataobject) // !!dataobject!! 
  return tOperation.Result();
#else
#ifeq($(Member.Return.Type),struct)     // !!struct!! 
  $(Member.Return.Name) stReturn;
  tOperation.Result() >> stReturn;
  return stReturn;
#else
#ifeq($(Member.Return.Type),typedef)    // !!typedef!!
  $(Member.Return.Name) tReturn;
  tOperation.Result() >> tReturn;
  return tReturn;
#else
#ifeq($(Member.Return.Type),template)    // !!template!!
  $(Member.Return) tReturn;
  tOperation.Result() >> tReturn;
  return tReturn;
#else
#cgerror "Member.Return.Type = $(Member.Return.Type);"
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
#ifeqend
}
#end
$(Class.EndingNs)
#end
