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

#ifndef _OPERATION_H_
#define _OPERATION_H_

#include "staffcommonexport.h"
#include <string>
#include "DataObject.h"

namespace staff
{
  class CValue;

  //!  Service Operation
  class STAFF_COMMON_EXPORT COperation
  {
  public:
    //!        constructor
    COperation(const std::string& sName = "", const std::string& sResponseName = "", const std::string& sResultName = "");

    //!        destructor
    virtual ~COperation();

    //!         set operation name
    /*! \param  sName - operation name
        */
    void SetName(const std::string& sName);

    //!         get operation name
    /*! \return operation name
    */
    const std::string GetName() const;

    //!         set response node name
    /*! \param  sResponseName - response node name
        */
    void SetResponseName(const std::string& sResponseName);

    //!         get response node name
    /*! \return response node name
    */
    const std::string GetResponseName() const;

    //!         set result node name
    /*! \param  sResultName - result node name
        */
    void SetResultName(const std::string& sResultName);

    //!         get result node name
    /*! \return result node name
    */
    const std::string& GetResultName() const;

    //!         set SOAP action
    /*! \param  sSoapAction - SOAP action
        */
    void SetSoapAction(const std::string& sSoapAction);

    //!         get SOAP action
    /*! \return SOAP action
        */
    const std::string& GetSoapAction() const;

    //!         add operation parameter
    /*! \param  sName - parameter name
        \param  tValue - parameter value
        */
    void AddParameter(const std::string& sName, const CValue& tValue);
    
    //!         add operation parameter as DataObject
    /*! \param  rDataObject - DataObject
        */
    void AddParameter(CDataObject& rDataObject);

    //!         get request DataObject
    /*! \return request DataObject
    */
    const CDataObject& Request() const;

    //!         get request DataObject
    /*! \return request DataObject(mutable)
    */
    CDataObject& Request();

    //!         get result DataObject
    /*! \return result DataObject
    */
    const CDataObject& Result() const;

    //!         get result DataObject
    /*! \return result DataObject(mutable)
    */
    CDataObject& Result();

    //!         prepare Operation for sending result
    void PrepareResult();

    //!         get response DataObject
    /*! \return response DataObject
        */
    CDataObject& GetResponse();

    //!         get response DataObject
    /*! \return response DataObject
        */
    const CDataObject& GetResponse() const;

    //!         set response DataObject
    /*! \param  rdoResponse - response DataObject
        */
    void SetResponse(staff::CDataObject& rdoResponse);

    //!         get/set result value
    /*! \return value
    */
    CValue ResultValue();

    //!         get result value
    /*! \return value
    */
    const CValue ResultValue() const;

    //!         set result
    /*! \param  rDataObject - DataObject with results
        */
    void SetResult(CDataObject& rDataObject);

    //!         set result value
    /*! \param  rValue - result value
        */
    void SetResultValue(const CValue& rValue);

    //!         check an operation is fault or not
    /*! \return true - operation is fault
        */
    bool IsFault() const;

    //!         get fault description
    /*! \return fault description or empty string
        */
    std::string GetFaultString() const;

    //!         get fault reason
    /*! \return fault reason or empty string
    */
    std::string GetFaultReason() const;

    //!         get fault code
    /*! \return fault code or empty string
        */
    std::string GetFaultCode() const;

    //!         get fault detail
    /*! \return fault detail or empty string
    */
    std::string GetFaultDetail() const;

    //!         get fault DataObject
    /*! \return fault DataObject
        */
    const CDataObject GetFault() const;

    //!         set fault
    /*! \param  sReason - fault reason
        \param  sFaultDetail - fault detail
        \param  sFaultCode - fault code
        */
    void SetFault(const std::string& sReason, const std::string& sFaultDetail = "", const std::string& sFaultCode = "");

    //!         reset fault
    void ResetFault();

    //!         set fault as DataObject
    /*! \param  rDataObjectFault - DataObject with fault description
        */
    void SetUserFault(CDataObject& rDataObjectFault);

  private:
    CDataObject m_tdoRequest;        //!<  request DataObject
    CDataObject m_tdoResponse;       //!<  response DataObject
    mutable CDataObject m_tdoResult; //!<  result DataObject
    std::string m_sResultName;       //!<  result name
    std::string m_sSoapAction;       //!<  SOAP action
  };
}

#endif // _OPERATION_H_
