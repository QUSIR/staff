// This file generated by staff_codegen from wsdl
// For more information please visit: http://code.google.com/p/staff/

#ifndef _$(Interface.Name)_h_
#define _$(Interface.Name)_h_

#include <staff/common/WsdlTypes.h>

// targetNamespace: $(Interface.TargetNamespace)
#ifneq($(Interface.Structs.$Count),0)

#foreach $(Interface.Structs)
struct $(Struct.Name);
#end
#else
\
#ifeqend
#ifneq($(Interface.Typedefs.$Count),0)

#foreach $(Interface.Typedefs)
typedef $(Typedef.DataType) $(Typedef.Name);
#end
#else
\
#ifeqend

#foreach $(Interface.Structs)
struct $(Struct.Name)$(Struct.ParentDecl)
{
#foreach $(Struct.Members)
  $(Param.DataType) $(Param.Name);
#end
};

#end
#foreach $(Interface.Classes)
$(Class.OpeningNs)
//!  service $(Class.ServiceNsName)
#ifneq($(Class.Description),)
/*! $(Class.Description) */
#else
\
#ifeqend
// uri: $(Class.ServiceUri)
class $(Class.Name)
{
public:
  virtual ~$(Class.Name)() {}
#foreach $(Class.Members)

#ifneq($(Member.Description),)
  /*! $(Member.Description) */
#else
\
#ifeqend
#ifneq($(Member.SoapAction),)
  // soapAction: $(Member.SoapAction)
#else
\
#ifeqend
#ifneq($(Member.Return.ResponseName),)
  // responseElement: $(Member.Return.ResponseName)
#else
\
#ifeqend
#ifneq($(Member.Return.NodeName),)
  // resultElement: $(Member.Return.NodeName)
#else
\
#ifeqend
  virtual $(Member.Return) $(Member.Name)($(Member.Params))$(Member.Const) = 0;
#end
};

$(Class.EndingNs)
#end
#endif // _$(Interface.Name)_h_
