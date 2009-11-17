// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Service Implementation

#ifndef _ProfileAdminImpl_h_
#define _ProfileAdminImpl_h_

#include "ProfileAdmin.h"

namespace webapp
{
namespace admin
{

  class CProfileAdminImpl: public CProfileAdmin
  {
  public:
    CProfileAdminImpl();
    virtual ~CProfileAdminImpl();
    virtual ::webapp::admin::TStringList GetProfileList();
    virtual void RemoveProfile(const std::string& sName);
    virtual ::webapp::admin::SProfile GetProfile(const std::string& sName);
    virtual void SetProfile(const ::webapp::admin::SProfile& rProfile);

  private:
    static std::string m_sProfilesPath;
  };
}
}


#endif // _ProfileAdminImpl_h_