// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

#ifndef _$(Interface.Name)Proxy_h_
#define _$(Interface.Name)Proxy_h_

#include <staff/client/Axis2Client.h>
#include <staff/common/WsdlTypes.h>
#include "$(Interface.FileName)"

#foreach $(Interface.Classes)
$(Class.OpeningNs)
//! Proxy for component service $(Class.ServiceNsName)
class $(Class.Name)Proxy: public $(Class.Name)
{
public:
  $(Class.Name)Proxy();
  ~$(Class.Name)Proxy();
  void Init(const std::string& sServiceUri, const std::string& sSessionId, const std::string& sInstanceId);
  void Deinit();
#foreach $(Class.Members)
  $(Member.Return) $(Member.Name)($(Member.Params))$(Member.Const);
#end
private:
  mutable staff::CAxis2Client m_tClient;
};

$(Class.EndingNs)
#end
#endif