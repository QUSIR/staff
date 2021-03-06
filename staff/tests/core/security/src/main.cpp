/*
 *  Copyright 2010 Utkin Dmitry
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

#include <iostream>
#include <list>
#include <staff/common/Exception.h>
#include <staff/utils/Log.h>
#include <staff/security/DbConn.h>
#include <staff/security/Sessions.h>
#include <staff/security/Users.h>
#include <staff/security/Groups.h>
#include <staff/security/UsersToGroups.h>
#include <staff/security/Objects.h>
#include <staff/security/Acl.h>

void Header(const std::string& sText)
{
  std::cout
    << "\n\n---------------------------------------------------------------\n"
    << sText
    << "\n---------------------------------------------------------------\n";
}

/*std::ostream& operator<<(std::ostream& rStream, const staff::security::SSession& rSession)
{
  return rStream
    << "\n        id: " << rSession.nId
    << "\n   extraid: " << rSession.nExtraId
    << "\nsession id: " << rSession.sSessionId
    << "\n   user id: " << rSession.nUserId
    << "\nlogin time: " << rSession.nTime
    << std::endl;
}*/

std::ostream& operator<<(std::ostream& rStream, const staff::security::User& rUser)
{
  return rStream
    << "\n         id: " << rUser.nId
    << "\n       name: " << rUser.sName
    << "\ndescription: " << rUser.sDescription
    << std::endl;
}

std::ostream& operator<<(std::ostream& rStream, const staff::security::Group& rGroup)
{
  return rStream
    << "\n         id: " << rGroup.nId
    << "\n       name: " << rGroup.sName
    << "\ndescription: " << rGroup.sDescription
    << std::endl;
}

std::ostream& operator<<(std::ostream& rStream, const staff::security::Object& rObject)
{
  return rStream
    << "\n         id: " << rObject.nId
    << "\n       name: " << rObject.sName
    << "\ndescription: " << rObject.sDescription
    << "\n   parentid: " << rObject.nParentId
    << std::endl;
}

template <typename TYPE>
std::ostream& operator<<(std::ostream& rStream, const std::list<TYPE>& rlsSessions)
{
  rStream << "--------------------------------\n";
  for (typename std::list<TYPE>::const_iterator itItem = rlsSessions.begin();
      itItem != rlsSessions.end(); ++itItem)
  {
    rStream << *itItem;
  }
  rStream << "--------------------------------\n";

  return rStream;
}

int main()
{
  int nSuccessed = 0;
  int nFailed = 0;

  #define Test(TEXT, EXPR) \
    if (EXPR)\
    {\
       staff::LogInfo() << TEXT": " << #EXPR << staff::LogResultSuccess;\
      ++nSuccessed;\
    }\
    else\
    {\
      staff::LogInfo() << TEXT": " << #EXPR << staff::LogResultFailed;\
      ++nFailed;\
    }

  try
  {
    using namespace staff::security;

    Session stSession;

    DbConn::Open();

    {
      SessionsList lsSessions;

      Header("Sessions");
      Sessions& rSessions = Sessions::Inst();

/*      rSessions.GetById(0, stSession);
      Test("GetById", stSession.sSessionId == Sessions::sNobodySessionId);*/

//      staff::LogDebug3() << stSession;

//      Test("Validate nobody", rSessions.Validate(stSession.sSessionId));
      Test("Validate nobody", rSessions.Validate(Sessions::sNobodySessionId));

/*      unsigned unSessionsCount = -1;
      rSessions.GetList(lsSessions);
      unSessionsCount = lsSessions.size();
      Test("Get list 1", unSessionsCount != 0);

      staff::LogDebug3() << lsSessions;*/

      std::string sSessionId;
      rSessions.Open("user", "user", true, sSessionId);
      Test("Open session", sSessionId.size() != 0);

      staff::LogDebug3() << "opened sessid: " << sSessionId;

/*      rSessions.GetList(lsSessions);
      Test("Get list 2", lsSessions.size() == (unSessionsCount + 1));

      staff::LogDebug3() << lsSessions;

      rSessions.GetBySessionId(sSessionId, stSession);
      Test("GetBySessionId", sSessionId == stSession.sSessionId);*/

      Test("Validate user", rSessions.Validate(sSessionId));

      std::string sUserName;
      rSessions.GetUserName(sSessionId, sUserName);
      Test("GetUserName", sUserName == "user");

      int nUserId = -1;
      rSessions.GetUserId(sSessionId, nUserId);
      Test("GetUserId", nUserId > 0);


      rSessions.Close(sSessionId);
      Test("CloseSession&Validate", !rSessions.Validate(sSessionId));

/*      rSessions.GetList(lsSessions);
      Test("Get list 3", lsSessions.size() == unSessionsCount);
      staff::LogDebug3() << lsSessions;*/
    }

    ////////////////////////////////////////////////////////////////
    // users & groups


    {
      Header("Users and groups");

      ////////////////////////////////////////////////////////////////
      // users

      Users& rUsers = Users::Inst();

      User stUser;
      int nUserId = -1;
      UsersList lsUsers;

      rUsers.Add("test", "test", "test user", nUserId);
      Test("Add user", nUserId > 0);

      staff::LogDebug3() << "created user id: " << nUserId;

      rUsers.GetById(nUserId, stUser);
      Test("GetById", (nUserId == stUser.nId) && (stUser.sName == "test") && (stUser.sDescription == "test user"));

      staff::LogDebug3() << "User: " << stUser;

      rUsers.SetDescription(nUserId, "descr");

      rUsers.GetById(nUserId, stUser);
      Test("SetDescription&GetById", (nUserId == stUser.nId) && (stUser.sName == "test") && (stUser.sDescription == "descr"));

      staff::LogDebug3() << "User: " << stUser;

      rUsers.GetList(lsUsers);
      Test("GetList", lsUsers.size() > 0);

      staff::LogDebug3() << "Users:\n\n" << lsUsers;

      ////////////////////////////////////////////////////////////////
      // groups

      Groups& rGroups = Groups::Inst();

      Group stGroup;
      int nGroupId = -1;
      GroupsList lsGroups;

      rGroups.Add("test", "test group", nGroupId);
      Test("Add group", nGroupId > 0);

      rGroups.GetById(nGroupId, stGroup);
      Test("GetById", (nGroupId == stGroup.nId) && (stGroup.sName == "test") && (stGroup.sDescription == "test group"));

      staff::LogDebug3() << "Group: " << stGroup;

      rGroups.SetDescription(nGroupId, "descr");

      rGroups.GetById(nGroupId, stGroup);
      Test("SetDescription&GetById", (nGroupId == stGroup.nId) && (stGroup.sName == "test") && (stGroup.sDescription == "descr"));

      staff::LogDebug3() << "Group: " << stGroup;

      rGroups.GetList(lsGroups);
      Test("GetList", lsGroups.size() > 0);


      ////////////////////////////////////////////////////////////////
      // users to groups

      IntList lsTmp;

      UsersToGroups& rUsersToGroups = UsersToGroups::Inst();

      rUsersToGroups.GetUserGroups(nUserId, lsTmp);
      Test("GetUserGroups", lsTmp.size() == 0);

      rUsersToGroups.AddUserToGroup(nUserId, nGroupId);

      Test("AddUserToGroup&IsMemberOfGroup", rUsersToGroups.IsUserMemberOfGroup(nUserId, nGroupId));

      rUsersToGroups.GetUserGroups(nUserId, lsTmp);
      Test("GetUserGroups", lsTmp.size() == 1);

      rUsersToGroups.GetGroupUsers(nGroupId, lsTmp);
      Test("GetGroupUsers1", lsTmp.size() == 1);

      rUsersToGroups.GetUserGroups(nUserId, lsTmp);
      Test("GetUserGroups1", lsTmp.size() == 1);

      rUsersToGroups.RemoveUserFromGroup(nUserId, nGroupId);
      Test("RemoveUserFromGroup&IsMemberOfGroup", !rUsersToGroups.IsUserMemberOfGroup(nUserId, nGroupId));

      rUsersToGroups.GetGroupUsers(nGroupId, lsTmp);
      Test("GetGroupUsers2", lsTmp.size() == 0);

      rUsersToGroups.GetUserGroups(nUserId, lsTmp);
      Test("GetUserGroups2", lsTmp.size() == 0);

      rUsers.Remove(nUserId);

      rGroups.Remove(nGroupId);
    }


    int nSamplesId = -1;
    Objects& rObjects = Objects::Inst();

    {
      Header("Objects");

      ////////////////////////////////////////////////////////////////
      // objects

      Object stObject;
      ObjectsList lsObjects;

      // create test tree
      {
        int nComponentsId = -1;
        int nCalcId = -1;
        int nCalculatorId = -1;
        int nSubId = -1;

        rObjects.GetIdByPathName("component", nComponentsId);
        rObjects.Add("samples", "", nComponentsId, nSamplesId);
        rObjects.Add("calc", "", nSamplesId, nCalcId);
        rObjects.Add("Calculator", "", nCalcId, nCalculatorId);
        rObjects.Add("Sub", "", nCalcId, nSubId);
      }


      rObjects.GetByPathName("component.samples.calc.Calculator", stObject);
      Test("GetByPathName", stObject.nId > 0 && stObject.nParentId > 0 && stObject.sName == "Calculator");

      staff::LogDebug3() << "object: " << stObject;

      int nObjId = -1;

      rObjects.GetParentId(stObject.nId, nSamplesId);
      Test("GetParentId", nSamplesId == stObject.nParentId);

      rObjects.GetParentId(stObject.nParentId, nSamplesId);

      rObjects.Add("Test", "test svc", nSamplesId, nObjId);

      rObjects.GetByPathName("component.samples.Test", stObject);
      Test("Add&GetByPathName", (stObject.nId == nObjId) && (stObject.nParentId == nSamplesId) && (stObject.sName == "Test"));

      staff::LogDebug3() << "Added object: " << stObject;

      rObjects.GetChilds(0, lsObjects);
      Test("GetChilds", lsObjects.size() > 0);

      staff::LogDebug3() << "Objects: " << lsObjects;

      rObjects.Remove(nObjId);
    }

    {
      Header("ACL");

      ////////////////////////////////////////////////////////////////
      // acl

      Acl& rAcl = Acl::Inst();

      Access eAccess = AccessInherited;
      bool bAccess = false;
      int nUserId = 2; // 'user'
      int nGroupId = 2; // 'user'
      int nObjectId = -1;
      int nParentObjectId = -1;

      rObjects.GetIdByPathName("component.samples.calc.Calculator", nObjectId);
      rObjects.GetParentId(nObjectId, nParentObjectId);


      // users acl
      rAcl.GetUserAccess(0, 0, eAccess);
      Test("Acl user 'nobody' to root object(multiple users)", eAccess == AccessInherited);
      rAcl.GetUserAccess(0, nUserId, eAccess);
      Test("Acl user 'user' to root object(multiple users)", eAccess == AccessInherited);

      rAcl.GetUserAccess(1, 0, eAccess);
      Test("Acl user 'nobody' to component object", eAccess == AccessGranted);
      rAcl.GetUserAccess(1, nUserId, eAccess);
      Test("Acl user 'user' to component object", eAccess == AccessGranted);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc user 'user' to inherited object", bAccess);


      rAcl.SetUserAccess(nObjectId, nUserId, AccessDenied);
      rAcl.GetUserAccess(nObjectId, 0, eAccess);
      Test("Acl user 'nobody' to inherited object(rule do not touch other users)", eAccess == AccessInherited);
      rAcl.GetUserAccess(nObjectId, nUserId, eAccess);
      Test("Acl user 'user' to denied object", eAccess == AccessDenied);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc user 'user' to inherited object", !bAccess);

      rAcl.SetUserAccess(nObjectId, nUserId, AccessGranted);
      rAcl.GetUserAccess(nObjectId, nUserId, eAccess);
      Test("Acl user 'user' to granted object", eAccess == AccessGranted);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc user 'user' to inherited object", bAccess);


      rAcl.SetUserAccess(nObjectId, nUserId, AccessInherited);
      rAcl.GetUserAccess(nObjectId, nUserId, eAccess);
      Test("Acl user 'user' to inherited object", eAccess == AccessInherited);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc user 'user' to inherited object", bAccess);


      // groups acl
      rAcl.GetGroupAccess(0, nGroupId, eAccess);
      Test("Acl group 'user' to root object(multiple groups)", eAccess == AccessInherited);

      rAcl.GetGroupAccess(1, nGroupId, eAccess);
      Test("Acl group 'user' to inherited object", eAccess == AccessInherited);


      rAcl.GetAnyGroupAccess(4, eAccess);
      Test("Acl user 'nobody' without group to admin object", eAccess == AccessDenied);
      rAcl.GetGroupAccess(4, nGroupId, eAccess);
      Test("Acl group 'user' to admin object", eAccess == AccessDenied);
      rAcl.GetGroupAccess(4, 1, eAccess);
      Test("Acl user 'admin' to admin object", eAccess == AccessGranted);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc group 'user' to inherited object", bAccess);

      rAcl.SetGroupAccess(nObjectId, nGroupId, AccessDenied);
      rAcl.GetGroupAccess(nObjectId, nGroupId, eAccess);
      Test("Acl group 'user' to denied object", eAccess == AccessDenied);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc group 'user' to inherited object", !bAccess);


      rAcl.SetGroupAccess(nObjectId, nGroupId, AccessGranted);
      rAcl.GetGroupAccess(nObjectId, nGroupId, eAccess);
      Test("Acl group 'user' to granted object", eAccess == AccessGranted);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc group 'user' to inherited object", bAccess);


      rAcl.SetGroupAccess(nObjectId, nGroupId, AccessInherited);
      rAcl.GetGroupAccess(nObjectId, nGroupId, eAccess);
      Test("Acl group 'user' to inherited object", eAccess == AccessInherited);

      bAccess = rAcl.CalculateUserAccess(nObjectId, nUserId);
      Test("Calc group 'user' to inherited object", bAccess);
    }

    rObjects.Remove(nSamplesId);

    Header("Results");
    staff::LogInfo() << "\n\n\033[1mTests successed: " << nSuccessed << "\nTests failed: " << nFailed << "\033[0m";

    DbConn::Close();
  }
  STAFF_CATCH_ALL;

  return 0;
}
