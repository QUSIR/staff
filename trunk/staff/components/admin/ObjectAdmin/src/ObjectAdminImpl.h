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
// Service Implementation

#ifndef _ObjectAdminImpl_h_
#define _ObjectAdminImpl_h_

#include "ObjectAdmin.h"

namespace staff
{
  namespace admin
  {
    class CObjectAdminImpl: public CObjectAdmin
    {
    public:
      CObjectAdminImpl();
      virtual ~CObjectAdminImpl();

      virtual ::staff::admin::TObjectIdList GetObjectIdList();
      virtual ::staff::admin::TObjectIdList GetObjectIdListWithParent(int nParentId);
      virtual ::staff::admin::SObject GetObjectById(int nObjectId);
      virtual ::staff::admin::SObject GetObjectByNameAndType(const std::string& sObjectName, int nType);
      virtual ::staff::admin::TObjectList GetObjectList(const ::staff::admin::TObjectIdList& rlsObjectIdList);
      virtual int AddObject(const ::staff::admin::SObject& rstObject);
      virtual void RemoveObject(int nObjectId);
      virtual void ReplaceObject(const ::staff::admin::SObject& rstObject);

      virtual ::staff::admin::TObjectTypesList GetObjectTypeList();
      virtual int AddObjectType(const ::staff::admin::SObjectType& rstObjectType);
      virtual void ReplaceObjectType(const ::staff::admin::SObjectType& rstObjectType);
      virtual void RemoveObjectType(int nObjectTypeId);
    };
  }
}


#endif // _ObjectAdminImpl_h_
