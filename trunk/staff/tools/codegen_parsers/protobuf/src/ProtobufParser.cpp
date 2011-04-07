/*
 *  Copyright 2011 Utkin Dmitry
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

#include <limits.h>
#include <errno.h>
#include <string.h>
#include <list>
#include <set>
#include <iostream>
#include <fstream>
#include <sstream>
#include <rise/common/Log.h>
#include <rise/common/exmacros.h>
#include <rise/common/ExceptionTemplate.h>
#include <rise/plugin/PluginExport.h>
#include <staff/codegen/tools.h>
#include "ProtobufParser.h"

RISE_DECLARE_PLUGIN(staff::codegen::CProtobufParser)

namespace staff
{
namespace codegen
{
  class CProtobufHeaderParser
  {
  public:
    CProtobufHeaderParser(const std::string& sRootNs):
      m_sRootNamespace(sRootNs), m_sCurrentNamespace(sRootNs), m_nLine(1), m_pProject(NULL)
    {
    }

    void SkipWs()
    {
      char chData = 0;
      while (m_tFile.good() && !m_tFile.eof())
      {
        chData = m_tFile.peek();
        if (chData == '\n')
        {
          ++m_nLine;
        }

        if(chData != ' ' && chData != '\n' && chData != '\r' && chData != '\t')
        {
          if (chData == '/') // comment
          {
            m_tFile.ignore();
            if (m_tFile.peek() == '/') // single line comment
            {
              m_tFile.ignore(INT_MAX, '\n');
              ++m_nLine;
              continue;
            }
            else
            if (m_tFile.peek() == '*') // multiline comment
            {
              m_tFile.ignore();
              while (m_tFile.good() && !m_tFile.eof())
              {
                m_tFile.get(chData);
                if (chData == '*')
                {
                  m_tFile.get(chData);
                  if (chData == '/')
                  {
                    break;
                  }
                }

                if (chData == '\n')
                {
                  ++m_nLine;
                }
              }
            }
            else
            {
              m_tFile.unget();
              break;
            }
          }
          else
          {
            break;
          }
        }
        m_tFile.ignore();
      }
    }

    void SkipWsOnly()
    {
      char chData = 0;
      while (m_tFile.good() && !m_tFile.eof())
      {
        chData = m_tFile.peek();
        if (chData == '\n')
        {
          ++m_nLine;
        }

        if(chData != ' ' && chData != '\n' && chData != '\r' && chData != '\t')
        {
          break;
        }
        m_tFile.ignore();
      }
    }

    void SkipWsInLine()
    {
      char chData = 0;
      while (m_tFile.good() && !m_tFile.eof())
      {
        chData = m_tFile.peek();

        if(chData != ' ' && chData != '\t')
        {
          break;
        }
        m_tFile.ignore();
      }
    }

    void SkipSingleLineComment()
    {
      char chData = '\0';
      while (m_tFile.good() && !m_tFile.eof())
      {
        chData = m_tFile.peek();
        if(chData != ' ' && chData != '\t')
        {
          break;
        }
        m_tFile.ignore();
      }

      if (m_tFile.good() && !m_tFile.eof())
      {
        if (m_tFile.peek() == '/') // comment
        {
          m_tFile.ignore();
          if (m_tFile.peek() == '/') // single line comment
          {
            m_tFile.ignore(INT_MAX, '\n');
            ++m_nLine;
          } else
          {
            m_tFile.unget();
          }
        }
      }
    }

    void ReadStr(std::string& sString, bool bSkipWS = true)
    {
      sString.erase();
      if (bSkipWS)
      {
        SkipWs();
      }
      m_tFile >> sString;
    }

    void ReadBefore(std::string& sOut, const std::string& sDelim = " \r\n\t,();[]{}<>\\/*-+!@#$%^&*=")
    {
      char chData = 0;

      sOut.erase();

      while (m_tFile.good() && !m_tFile.eof())
      {
        chData = m_tFile.peek();
        if (sDelim.find(chData) != std::string::npos)
        {
          break;
        }

        if (chData == '\n')
        {
          ++m_nLine;
        }

        sOut += chData;
        m_tFile.ignore();
      }
    }

    bool ReadComment(std::string& sComment)
    {
      if (m_tFile.peek() != '/')
      {
        return false;
      }

      sComment.erase();

      m_tFile.ignore();
      if (m_tFile.peek() == '/')
      {
        m_tFile.ignore();
        ReadBefore(sComment, "\n\r");
      }
      else
      if (m_tFile.peek() == '*')
      {
        char chData = '\0';
        std::string sTmp;

        m_tFile.ignore();
        while (m_tFile.good() && !m_tFile.eof())
        {
          ReadBefore(sTmp, "*");

          sComment += sTmp;
          m_tFile.get(chData);
          if (chData == '*')
          {
            if (m_tFile.peek() == '/')
            {
              m_tFile.ignore();
              break;
            }
            sComment += chData;
          }

          if (chData == '\n')
          {
            ++m_nLine;
          }
        }
      }
      else
      {
        m_tFile.unget();
        return false;
      }

      rise::StrTrim(sComment);

      return true;
    }
    
    void CheckAndFixName(std::string& sName, bool bIgnoreBool = false)
    {
      static std::set<std::string> setCppReservedWords;
      if (setCppReservedWords.empty())
      {
        const unsigned nCppReservedWordsCount = 78;
        const char* aszCppReservedWords[nCppReservedWordsCount] = 
        {
          "and", "and_eq", "asm", "auto", "bitand", "bitor", "bool", "_Bool", "break", "case", "catch",
          "char", "class", "compl", "_Complex", "const", "const_cast", "continue", "default", "delete",
          "do", "double", "dynamic_cast", "else", "enum", "explicit", "export", "extern", "false", "float",
          "for", "friend", "goto", "if", "_Imaginary", "inline", "int", "long", "mutable", "namespace",
          "new", "not", "not_eq", "operator", "or", "or_eq", "private", "protected", "public", "register",
          "reinterpret_cast", "restrict", "return", "short", "signed", "sizeof", "static", "static_cast",
          "struct", "switch", "template", "this", "throw", "true", "try", "typedef", "typeid", "typename",
          "union", "unsigned", "using", "virtual", "void", "volatile", "wchar_t", "while", "xor", "xor_eq"
        };
        for (unsigned nIndex = 0; nIndex < nCppReservedWordsCount; ++nIndex)
        {
          setCppReservedWords.insert(aszCppReservedWords[nIndex]);
        }
      }
      
      if (setCppReservedWords.count(sName))
      {
        if (!bIgnoreBool || (sName != "true" && sName != "false"))
        {
          sName += '_';
        }
      }
    }

    template<typename TStructType>
    bool ParseCompositeDataType(const std::list<TStructType>& rList, SDataType& rDataType)
    {
      for (typename std::list<TStructType>::const_iterator it = rList.begin(); it != rList.end(); ++it)
      {
        if (it->sName == rDataType.sName)
        {
          // namespace match
          if (it->sNamespace == rDataType.sNamespace)
          {
            return true;
          }

          std::string::size_type nPos = m_sCurrentNamespace.find_last_of("::");

          while (nPos != std::string::npos)
          {
            ++nPos;
            if((m_sCurrentNamespace.substr(0, nPos) + rDataType.sNamespace) == it->sNamespace)
            {
              if (it->sNamespace != rDataType.sNamespace) // correct namespace
              {
                rDataType.sNamespace = it->sNamespace;
              }

              return true;
            }

            if (nPos < 4)
            {
              break;
            }

            nPos = m_sCurrentNamespace.find_last_of("::", nPos - 3);
          }
        }
      }

      return false;
    }

    void GetDataType(const std::string& sDataTypeName, SDataType& rDataType)
    {
      rDataType.sUsedName = sDataTypeName;
      std::string::size_type nPos = sDataTypeName.find_last_of("::");
      if (nPos != std::string::npos)
      {
        ++nPos;
        rDataType.sName = sDataTypeName.substr(nPos);
        rDataType.sNamespace = sDataTypeName.substr(0, nPos);
      }
      else
      {
        rDataType.sName = sDataTypeName;
        rDataType.sNamespace = ""; //"::";
      }

      if (
          sDataTypeName == "double" ||
          sDataTypeName == "float" ||
          sDataTypeName == "bool"
          )
      {
        rDataType.eType = SDataType::EGeneric;
      }
      else
      if (
          sDataTypeName == "int32" ||
          sDataTypeName == "uint32" ||
          sDataTypeName == "sint32" ||
          sDataTypeName == "fixed32" ||
          sDataTypeName == "sfixed32"
          )
      {
        rDataType.eType = SDataType::EGeneric;
        rDataType.sName = "int";
        rDataType.sUsedName = rDataType.sName;
      }
      else
      if (
          sDataTypeName == "int64" ||
          sDataTypeName == "uint64" ||
          sDataTypeName == "sint64" ||
          sDataTypeName == "fixed64" ||
          sDataTypeName == "sfixed64"
          )
      {
        rDataType.eType = SDataType::EGeneric;
        rDataType.sName = "long";
        rDataType.sUsedName = rDataType.sName;
      }
      else
      if (
          sDataTypeName == "string" ||
          sDataTypeName == "bytes"
          )
      {
        rDataType.eType = SDataType::EString;
        rDataType.sName = "string";
        rDataType.sNamespace = "std::";
        rDataType.sUsedName = rDataType.sNamespace + rDataType.sName;
      }
      else
      if(ParseCompositeDataType(m_stInterface.lsStructs, rDataType))
      {
        rDataType.eType = SDataType::EStruct;
      }
      else
      {
        rDataType.eType = SDataType::EUnknown;
      }
    }

    void IgnoreFunction()
    {
      char chTmp = '\0';
      int nRecursion = 0;

      while (m_tFile.good() && !m_tFile.eof())
      {
        SkipWs();
        m_tFile >> chTmp;

        if (chTmp == ';' && nRecursion == 0)
        {
          SkipSingleLineComment();
          break;
        }
        else
        if (chTmp == '}')
        {
          --nRecursion;
          CSP_ASSERT(nRecursion >= 0, "mismatch {}", m_stInterface.sFileName, m_nLine);

          if (nRecursion == 0)
          {
            while (!m_tFile.eof() && m_tFile.peek() == ';')
            {
              m_tFile.ignore();
            }
            break;
          }
        }
        else
        if (chTmp == '{')
        {
          ++nRecursion;
        }
      }
    }

    // datatype
    void ParseDataType( const std::string& sDataType, SDataType& rDataType, const SStruct* pstParent = NULL )
    {
      std::string sTmp;

      bool bIsRef = false;

      rDataType.bIsConst = false;
      rDataType.bIsRef = false;
      rDataType.eType = SDataType::EUnknown;
      rDataType.sUsedName.erase();
      rDataType.lsParams.clear();
      rDataType.sNodeName.erase();
      rDataType.sNamespace.erase();

      std::string sTypeName;

      std::string::size_type nSize = sDataType.size();

      for (std::string::size_type nBegin = 0, nEnd = 0;
           nEnd != std::string::npos && nEnd < nSize; nBegin = nEnd)
      {
        nEnd = sDataType.find_first_of(" \n\r\t", nBegin);
        if (nEnd != std::string::npos)
        {
          sTmp = sDataType.substr(nBegin, nEnd  - nBegin);
          nEnd = sDataType.find_first_not_of(" \n\r\t", nEnd); // skip whitespaces
        }
        else
        {
          sTmp = sDataType.substr(nBegin);
        }

        if (!sTmp.empty())
        { // name of type
          CSP_ASSERT(!rDataType.bIsRef, "type after reference: [" + sDataType + "]",
                     m_stInterface.sFileName, m_nLine);
          if (!sTypeName.empty())
          {
            sTypeName += ' ';
          }

          sTypeName += sTmp;
        }

        rDataType.bIsRef = bIsRef;
      }

      if (rDataType.eType == SDataType::EUnknown)
      {
        GetDataType(sTypeName, rDataType);
      }

      if (rDataType.eType == SDataType::EUnknown)
      {
        const SBaseType* pstType = GetBaseType(sTypeName, m_stInterface, SBaseType::EAny, pstParent);
        if (pstType)
        {
          rDataType.eType = pstType->eType == SBaseType::EStruct ? SDataType::EStruct : SDataType::EEnum;
          rDataType.sNamespace = pstType->sNamespace;
          rDataType.sName = pstType->sName;
          if ((pstType->eType == SBaseType::EStruct || SBaseType::EEnum) &&
              !pstType->sOwnerName.empty())
          {
            rDataType.sName = pstType->sOwnerName + "::" + rDataType.sName;
          }

          rDataType.sUsedName = sTypeName;
        }
      }
    }

    // member
    void ParseMember( SMember& rMember )
    {
      std::string sTmp;

      rMember.bIsConst = false;
      rMember.bIsAsynch = false;

      SkipWs();

      // read return type and operation name
      std::string sOperation;
      ReadBefore(sOperation);
      CSP_ASSERT(!sOperation.empty(), "Can't get operation name", m_stInterface.sFileName, m_nLine);
      CheckAndFixName(sOperation);
      rMember.sName = sOperation;

      SkipWs();
      CSP_ASSERT(m_tFile.get() == '(', "Error after operation name: [" + sOperation + "]",
                 m_stInterface.sFileName, m_nLine);

      SkipWs();

      std::string sMessage;
      ReadBefore(sMessage);

      SkipWs();
      CSP_ASSERT(m_tFile.get() == ')', "Error after message name in operation: [" + sOperation + "]",
                 m_stInterface.sFileName, m_nLine);

      if (!sMessage.empty())
      {
        rise::StrReplace(sMessage, ".", "::", true);

        SParam stParam;
        ParseDataType(sMessage, stParam.stDataType);
        stParam.sName = "request";
        stParam.stDataType.bIsConst = true;
        stParam.stDataType.bIsRef = true;
        OptimizeCppNs(stParam.stDataType.sUsedName, m_sCurrentNamespace);
        rMember.lsParams.push_back(stParam);

//      expand requests
//        const SStruct* pMessage = GetStruct(sMessage, m_stInterface);
//        CSP_ASSERT(pMessage, "Can't find message declaration: [" + sMessage + "]",
//                   m_stInterface.sFileName, m_nLine);

//        // todo: remove const_cast
//        const_cast<SStruct*>(pMessage)->bExtern = true; // mark struct as extern to hide it from interface file

//        rMember.lsParamList = pMessage->lsMembers;

//        for (std::list<SParam>::iterator itParam = rMember.lsParamList.begin();
//          itParam != rMember.lsParamList.end(); ++itParam)
//        {
//          SDataType& rstDataType = itParam->stDataType;

//          OptimizeCppNs(rstDataType.sUsedName, m_sCurrentNamespace);
//          if (rstDataType.eType == SDataType::EString ||
//              rstDataType.eType == SDataType::ETemplate ||
//              rstDataType.eType == SDataType::EStruct)
//          {
//            rstDataType.bIsConst = true;
//            rstDataType.bIsRef = true;
//          }
//        }
      }

      SkipWs();
      ReadBefore(sTmp);

      CSP_ASSERT(sTmp == "returns", "Missing return message name", m_stInterface.sFileName, m_nLine);

      std::string sReturn;
      SkipWs();
      CSP_ASSERT(m_tFile.get() == '(', "Error after \"returns\" in operation: [" + sOperation + "]",
                 m_stInterface.sFileName, m_nLine);
      SkipWs();
      ReadBefore(sReturn);
      CSP_ASSERT(!sReturn.empty(), "Can't get result message name", m_stInterface.sFileName, m_nLine);
      SkipWs();
      CSP_ASSERT(m_tFile.get() == ')', "Error after message type in operation: [" + sOperation + "]",
                 m_stInterface.sFileName, m_nLine);

      rise::StrReplace(sReturn, ".", "::", true);

      ParseDataType(sReturn, rMember.stReturn.stDataType);
      if (rMember.stReturn.stDataType.eType == SDataType::EStruct)
      {
        OptimizeCppNs(rMember.stReturn.stDataType.sUsedName, m_sCurrentNamespace);
      }
      else
      {
        rise::LogWarning() << "Return type for " << sOperation << " is not struct";
      }

      SkipWs();

      char chTmp = m_tFile.peek();
      if (chTmp == '{')
      {
        IgnoreFunction();
        SkipWsOnly();
        while ((chTmp = m_tFile.peek()) == ';' && !m_tFile.eof())
        {
          m_tFile.ignore();
        }
      }
      else
      {
        CSP_ASSERT(chTmp == ';', "Missing ';' or '{' after operation declaration",
                 m_stInterface.sFileName, m_nLine);

        do
        {
          m_tFile.ignore();
        }
        while ((chTmp = m_tFile.peek()) == ';' && !m_tFile.eof());
      }


      SkipSingleLineComment();
    }

    // service
    void ParseClass( SClass& rClass )
    {
      char chTmp = '\0';
      std::string sTmp;

      CheckAndFixName(rClass.sName);
      SkipWs();
      rClass.sNamespace = m_sCurrentNamespace;

      ReadStr(sTmp);
      CSP_ASSERT(!m_tFile.eof(), "unexpected EOF(after classname and '{')", m_stInterface.sFileName, m_nLine);
      CSP_ASSERT(sTmp == "{", "'{' after classname expected ", m_stInterface.sFileName, m_nLine);

      // parsing members
      while (m_tFile.good() && !m_tFile.eof())
      {
        SMember stMember;
        SkipWsOnly();
        while (ReadComment(sTmp))
        {
          rise::StrTrim(sTmp);
          if (sTmp.size() != 0)
          {
            if (stMember.sDescr.size() != 0)
            {
              stMember.sDescr += ' ';
            }
            stMember.sDescr += sTmp;
          }
          SkipWsOnly();
        }

        chTmp = m_tFile.peek();
        if (chTmp == '}')
        {
          m_tFile.ignore();
          break;
        }

        ReadStr(sTmp);

        if (sTmp == "rpc")
        {
          SkipWs();
          ParseMember(stMember);
          rClass.lsMembers.push_back(stMember);
        }
        else
        if (sTmp == "option")
        { // skip options
          ReadBefore(sTmp, ";");
          rise::LogDebug() << "Ignoring option" << sTmp;
          m_tFile.ignore();
          continue;
        }
        else
        {
          CSP_THROW("parse error! before: (" + sTmp + ")", m_stInterface.sFileName, m_nLine);
        }
      }
    }

    void ParseEnum( SEnum& rEnum )
    {
      char chTmp = '\0';
      std::string sTmp;
      std::string sDescr;

      CheckAndFixName(rEnum.sName);

      CSP_ASSERT(!m_tFile.eof(), "unexpected EOF(after enum name): " + rEnum.sName,
                 m_stInterface.sFileName, m_nLine);

      ReadStr(sTmp);
      CSP_ASSERT(!m_tFile.eof(), "unexpected EOF(after enum name): " + rEnum.sName,
                 m_stInterface.sFileName, m_nLine);

      if (sTmp == ";")
      {
        m_tFile.unget();
        return;
      }

      rEnum.bForward = false;

      CSP_ASSERT(sTmp == "{", "'{' or ';' after enum name expected: " + rEnum.sName,
                 m_stInterface.sFileName, m_nLine);

      while (m_tFile.good() && !m_tFile.eof())
      {
        SEnum::SEnumMember stMember;

        SkipWsOnly();
        while (ReadComment(sTmp))
        {
          rise::StrTrim(sTmp);
          if (sTmp.size() != 0)
          {
            sDescr = sTmp.substr(1);
            rise::StrTrim(sDescr);
          }
          SkipWsOnly();
        }

        ReadBefore(stMember.sName, "=;}");
        rise::StrTrim(stMember.sName);
        CheckAndFixName(stMember.sName);

        chTmp = m_tFile.peek();
        if (chTmp == '=')
        { // read value
          m_tFile.ignore();
          ReadBefore(stMember.sValue, ";}");
          rise::StrTrim(stMember.sValue);
          CheckAndFixName(stMember.sValue);
          chTmp = m_tFile.peek();
        }

        if (!stMember.sName.empty())
        {
          rEnum.lsMembers.push_back(stMember);
        }

        m_tFile.ignore(); // '}' or ';'

        if (chTmp == '}')
        {
          break;
        }

        sDescr.erase();
      }

      SkipWsInLine();
      if (m_tFile.peek() == ';')
      {
        m_tFile.ignore();
      }

      SkipSingleLineComment();
    }

    void ParseStruct( SStruct& rStruct )
    {
      char chTmp = '\0';
      std::string sTmp;

      CheckAndFixName(rStruct.sName);

      const std::string& sOwnerName = (!rStruct.sOwnerName.empty() ?
                                        (rStruct.sOwnerName + "::") : "") + rStruct.sName;

      CSP_ASSERT(!m_tFile.eof(), "unexpected EOF(after message name): " + rStruct.sName,
                 m_stInterface.sFileName, m_nLine);
      SkipWs();
      chTmp = m_tFile.get();
      if (chTmp == ';')
      {
        return; // forward declaration
      }

      CSP_ASSERT(!m_tFile.eof(), "unexpected EOF(after message name): " + rStruct.sName,
                 m_stInterface.sFileName, m_nLine);

      rStruct.bForward = false;

      CSP_ASSERT(chTmp == '{', "'{' or ';' after structname expected: " + rStruct.sName,
                 m_stInterface.sFileName, m_nLine);

      while (m_tFile.good() && !m_tFile.eof())
      {
        SParam stParamTmp;

        SkipWsOnly();
        while (ReadComment(sTmp))
        {
          rise::StrTrim(sTmp);
          if (sTmp.size() != 0)
          {
            if (stParamTmp.sDescr.size() != 0)
            {
              stParamTmp.sDescr += ' ';
            }
            stParamTmp.sDescr += sTmp;
          }
          SkipWsOnly();
        }

        SkipWs();
        chTmp = m_tFile.peek();
        if (chTmp == '}')
        {
          m_tFile.ignore();
          break;
        }

        std::string sToken;
        ReadBefore(sToken);
        bool bRepeated = false;

        if (sToken == "required")
        {
        }
        else
        if (sToken == "optional")
        {
          stParamTmp.mOptions["optional"] = "true";
        }
        else
        if (sToken == "repeated")
        {
          bRepeated = true;
        }
        else
        if (sToken == "enum")
        {
          std::string sName;
          SkipWs();
          ReadBefore(sName, " \r\n\t;{}");
          CheckAndFixName(sName);

          SEnum& rstEnum = TypeInList(rStruct.lsEnums, sName, m_sCurrentNamespace, sOwnerName);

          ParseEnum(rstEnum);
          if (!rstEnum.bForward)
          {
            rstEnum.sDescr = stParamTmp.sDescr;
            rstEnum.mOptions = stParamTmp.mOptions;
          }

          continue;
        }
        else
        if (sToken == "message")
        {
          std::string sName;
          SkipWs();
          ReadBefore(sName, " \r\n\t;{}");
          CheckAndFixName(sName);

          SStruct& rstStruct = TypeInList(rStruct.lsStructs, sName, m_sCurrentNamespace, sOwnerName);

          ParseStruct(rstStruct);
          if (!rstStruct.bForward)
          {
            rstStruct.sDescr = stParamTmp.sDescr;
            rstStruct.mOptions = stParamTmp.mOptions;
          }

          continue;
        }
        else
        if (sToken == "option")
        { // skip options
          ReadBefore(sTmp, ";");
          rise::LogDebug() << "Ignoring option" << sTmp;
          m_tFile.ignore();
          continue;
        }
        else
        if (sToken == "extensions")
        { // skip options
          ReadBefore(sTmp, ";");
          rise::LogDebug() << "Ignoring extensions " << sTmp;
          m_tFile.ignore();
          continue;
        }
        else
        {
          CSP_THROW("missing field specificator. message: " + rStruct.sName, m_stInterface.sFileName, m_nLine);
        }

        SkipWs();

        // data type
        ReadBefore(sToken);
        rise::StrReplace(sToken, ".", "::", true);

        if (bRepeated)
        {
          stParamTmp.stDataType.eType = SDataType::ETemplate;
          stParamTmp.stDataType.sName = "list";
          stParamTmp.stDataType.sNamespace = "std::";
          stParamTmp.stDataType.sUsedName.erase();

          SDataType stDataType;
          ParseDataType(sToken, stDataType, &rStruct);
          stParamTmp.stDataType.lsParams.push_back(stDataType);
        }
        else
        {
          ParseDataType(sToken, stParamTmp.stDataType, &rStruct);
        }

        // member name
        SkipWs();
        ReadBefore(sToken);
        stParamTmp.sName = sToken;
        CheckAndFixName(stParamTmp.sName);

        SkipWs();
        chTmp = m_tFile.peek();
        CSP_ASSERT(chTmp == '=', "missing tag. message: " + rStruct.sName,
                   m_stInterface.sFileName, m_nLine);
        m_tFile.ignore();

        SkipWs();
        ReadBefore(sToken); // tag id
        rise::StrTrim(sToken);
        CSP_ASSERT(!sToken.empty(), "missing tag id. message: " + rStruct.sName,
                   m_stInterface.sFileName, m_nLine);

        stParamTmp.mOptions["protobufTag"] = sToken;

        SkipWs();

        // optional parameters block
        chTmp = m_tFile.peek();
        if (chTmp == '[')
        {
          m_tFile.ignore();

          while ((chTmp = m_tFile.peek()) != ']')
          {
            if (chTmp == ',')
            {
              m_tFile.ignore();
            }

            // option name
            SkipWs();
            ReadBefore(sToken, " \r\n\t=");
            rise::StrTrim(sToken);

            SkipWs();
            chTmp = m_tFile.peek();
            CSP_ASSERT(chTmp == '=', "missing '=' after option name. message: " + rStruct.sName,
                       m_stInterface.sFileName, m_nLine);
            m_tFile.ignore();

            SkipWs();
            ReadBefore(sTmp, ",]");
            rise::StrTrim(sTmp);

            if (sToken == "default")
            {
              if (stParamTmp.stDataType.eType == SDataType::EEnum)
              {
                // fix namespace
                CheckAndFixName(sTmp);
                sTmp = stParamTmp.stDataType.sNamespace + sTmp;
                OptimizeCppNs(sTmp, rStruct.sNamespace + sOwnerName);
              }
              else
              {
                CheckAndFixName(sTmp, true);
              }
              
              stParamTmp.mOptions["defaultValue"] = sTmp;
            }
          }
          m_tFile.ignore();
        }

        SkipWs();
        m_tFile >> chTmp;
        CSP_ASSERT(chTmp == ';', "missing end of message field. message: " + rStruct.sName,
                   m_stInterface.sFileName, m_nLine);

        SkipWsOnly();
        if (ReadComment(sTmp))
        {
          rise::StrTrim(sTmp);
          if (sTmp.size() != 0)
          {
            if (stParamTmp.sDescr.size() != 0)
            {
              stParamTmp.sDescr += ' ';
            }
            stParamTmp.sDescr += sTmp;
          }
        }

        rStruct.lsMembers.push_back(stParamTmp);
      }

      SkipWsInLine();
      if (m_tFile.peek() == ';')
      {
        m_tFile.ignore();
      }

      SkipSingleLineComment();
    }

    void ImportEnums(const std::list<SEnum>& rlsSrc, std::list<SEnum>& rlsDst)
    {
      for (std::list<SEnum>::const_iterator itEnum = rlsSrc.begin();
          itEnum != rlsSrc.end(); ++itEnum)
      {
        rlsDst.push_back(*itEnum);
        rlsDst.back().bExtern = true;
      }
    }

    void ImportStruct(const std::list<SStruct>& rlsSrc, std::list<SStruct>& rlsDst)
    {
      for (std::list<SStruct>::const_iterator itStruct = rlsSrc.begin();
          itStruct != rlsSrc.end(); ++itStruct)
      {
        rlsDst.push_back(SStruct());
        SStruct& rstStruct = rlsDst.back();
        rstStruct.sName = itStruct->sName;
        rstStruct.sNamespace = itStruct->sNamespace;
        rstStruct.sParentName = itStruct->sParentName;
        rstStruct.sDescr = itStruct->sDescr;
        rstStruct.sDetail = itStruct->sDetail;
        rstStruct.bExtern = true;
        rstStruct.sOwnerName = itStruct->sOwnerName;
        ImportEnums(itStruct->lsEnums, rstStruct.lsEnums);
        ImportStruct(itStruct->lsStructs, rstStruct.lsStructs);
      }
    }

    void Import(const std::string& sFileName)
    {
      CProtobufHeaderParser tProtobufHeaderParser(m_sRootNamespace);
      
      rise::LogDebug() << "importing file " << sFileName << " from " << m_stInterface.sFileName;
      const SInterface& rInterface = tProtobufHeaderParser.Parse(m_sInDir, sFileName, *m_pProject);

      // use extern enums
      ImportEnums(rInterface.lsEnums, m_stInterface.lsEnums);

      // use extern structs
      ImportStruct(rInterface.lsStructs, m_stInterface.lsStructs);

      SInclude stInclude;
      stInclude.sInterfaceName = rInterface.sName;
      stInclude.sNamespace = rInterface.sNamespace;
      stInclude.sFileName = rInterface.sFileName;
      stInclude.sFilePath = rInterface.sFilePath;
      stInclude.sTargetNs = rInterface.sTargetNs;
      m_stInterface.lsIncludes.push_back(stInclude);
    }

    void ParseHeaderBlock( SInterface& rInterface )
    {
      char chData = 0;
      std::string sDescr;
      std::string sTmp;
      TStringMap mOptions;
      TStringList lsModules;

      SkipWsOnly();
      while (ReadComment(sTmp))
      {
        rise::StrTrim(sTmp);
        if (sTmp.size() != 0)
        {
          if (sDescr.size() != 0)
          {
            sDescr += ' ';
          }
          sDescr += sTmp;
        }

        // don't get file header comment
        int nLine = m_nLine;
        SkipWsOnly();
        if (m_nLine > (nLine + 1))
        {
          sDescr.erase();
        }
      }

      chData = m_tFile.peek();
      if (chData == '}') // end of namespace
      {
        return;
      }

      ReadStr(sTmp);

      if (sTmp.size() == 0)
      {
        return; // eof
      }

      if (sTmp == "service")
      {
        SClass stClass;

        SkipWs();

        // checking for service class
        ReadBefore(stClass.sName, " \t\n\r:{;");

        SkipWs();
        char chTmp = m_tFile.peek();

        if (chTmp == ';')
        {
          m_tFile.ignore();
          return; // class forward
        }

        rise::LogDebug() << "Using [" << stClass.sName << "] as service class";

        ParseClass(stClass);
        stClass.sDescr = sDescr;
        stClass.lsModules = lsModules;
        stClass.mOptions = mOptions;
        rInterface.lsClasses.push_back(stClass);

        SkipWs();

        SkipSingleLineComment();
        lsModules.clear();
        mOptions.clear();
        sDescr.erase();
      }
      else
      if (sTmp == "enum")
      {
        std::string sName;
        SkipWs();
        ReadBefore(sName, " \r\n\t;{}");

        SEnum& rstEnum = TypeInList(rInterface.lsEnums, sName, m_sCurrentNamespace);

        ParseEnum(rstEnum);
        if (!rstEnum.bForward)
        {
          rstEnum.sDescr = sDescr;
          rstEnum.mOptions = mOptions;
        }

        sDescr.erase();
      }
      else
      if (sTmp == "message")
      {
        std::string sName;
        SkipWs();
        ReadBefore(sName, " \r\n\t;{}");

        SStruct& rstStruct = TypeInList(rInterface.lsStructs, sName, m_sCurrentNamespace);

        ParseStruct(rstStruct);
        if (!rstStruct.bForward)
        {
          rstStruct.sDescr = sDescr;
          rstStruct.mOptions = mOptions;
        }

        sDescr.erase();
      }
      else
      if (sTmp == "import")
      {
        std::string sFileName;
        SkipWs();
        char chTmp = '\0';
        m_tFile.get(chTmp);
        CSP_ASSERT(chTmp == '"', "import: filename is not found!", m_stInterface.sFileName, m_nLine);

        ReadBefore(sFileName, "\"");
        m_tFile.get(chTmp);
        CSP_ASSERT(chTmp == '"', "import: filename end is not found!", m_stInterface.sFileName, m_nLine);

        SkipWs();
        m_tFile.get(chTmp);
        CSP_ASSERT(chTmp == ';', "';' is not found!", m_stInterface.sFileName, m_nLine);

        Import(sFileName);

        SkipSingleLineComment();
      }
      else
      if (sTmp == "package")
      {
        std::string sNamespace;
        ReadBefore(sNamespace, ";");
        rise::StrTrim(sNamespace);
        rise::StrReplace(sNamespace, ".", "::", true);
        m_sCurrentNamespace = m_sRootNamespace + sNamespace + "::";

        SkipWs();
        char chTmp = '\0';
        m_tFile.get(chTmp);
        CSP_ASSERT(chTmp == ';', "';' is not found!", m_stInterface.sFileName, m_nLine);
      }
      else
      if (sTmp == "extend")   // extend -ignore
      {
        ReadBefore(sTmp, "{;");
        rise::LogDebug() << "Ignoring extend" << sTmp;
        IgnoreFunction();
        SkipSingleLineComment();
      }
      else
      if (sTmp == ";")
      {
        SkipSingleLineComment();
      }
      else
      if (sTmp == "option")
      { // skip options
        ReadBefore(sTmp, ";");
        rise::LogDebug() << "Ignoring option" << sTmp;
      }
      else
      {
        CSP_THROW(("Unknown lexeme: \"" + sTmp + "\""), m_stInterface.sFileName, m_nLine);
      }
    }

    void ParseBracketedBlock( SInterface& rInterface )
    {
      char chData = 0;

      SkipWs();
      m_tFile.get(chData);
      CSP_ASSERT(chData == '{', "ParseBracketedBlock: \"{\" is not found!", m_stInterface.sFileName, m_nLine);

      while (m_tFile.good() && !m_tFile.eof())
      {
        SkipWsOnly();
        chData = m_tFile.peek();

        if (chData == '}')
        {
          m_tFile.ignore();
          return;
        }
        else
        if (chData == '{')
        {
          ParseBracketedBlock(rInterface);
        }
        else
        {
          ParseHeaderBlock(rInterface);
        }
      }

      CSP_THROW("ParseBracketedBlock: EOF found!", m_stInterface.sFileName, m_nLine);
    }

    void ParseHeader( SInterface& rInterface )
    {
      char chData = 0;

      while (m_tFile.good() && !m_tFile.eof())
      {
        SkipWsOnly();
        chData = m_tFile.peek();

        if (chData == '{')
        {
          ParseBracketedBlock(rInterface);
        }
        else
        if (chData == '}')
        {
          CSP_THROW("unexpected '}' found", m_stInterface.sFileName, m_nLine);
        }
        else // text
        {
          ParseHeaderBlock(rInterface);
        }
      }
    }


    static bool IsStructUsesAnother(const SStruct& rstStruct, const SStruct& rstOtherStruct)
    {
      bool bResult = false;
      // check the same level
      for (std::list<SParam>::const_iterator itMember = rstStruct.lsMembers.begin();
          itMember != rstStruct.lsMembers.end(); ++itMember)
      {
        const SDataType& rstDataType = itMember->stDataType;
        if (rstDataType.eType == SDataType::EStruct &&
            rstDataType.sNamespace == rstOtherStruct.sNamespace &&
            rstDataType.sName == rstOtherStruct.sName)
        {
          bResult = true;
          break;
        }
      }

      if (!bResult) // check nested structs
      {
        for (std::list<SStruct>::const_iterator itStruct = rstStruct.lsStructs.begin();
            itStruct != rstStruct.lsStructs.end(); ++itStruct)
        {
          if (IsStructUsesAnother(*itStruct, rstOtherStruct))
          {
            bResult = true;
            break;
          }
        }
      }

      return bResult;
    }

    bool FixStructOrderByUsing(std::list<SStruct>& rlsStructs)
    {
      bool bWasChanged = false;
      for (std::list<SStruct>::size_type nRetry = rlsStructs.size(); nRetry; --nRetry)
      {
        bWasChanged = false;
        for (std::list<SStruct>::iterator itThisStruct = rlsStructs.begin();
            itThisStruct != rlsStructs.end();)
        {
          // check is
          bool bFound = false;
          std::list<SStruct>::iterator itOtherStruct = itThisStruct;
          ++itOtherStruct;
          for (; itOtherStruct != rlsStructs.end(); ++itOtherStruct)
          {
            if (IsStructUsesAnother(*itThisStruct, *itOtherStruct))
            {
              rise::LogDebug() << "Moving " << itOtherStruct->sName
                  << " to before " << itThisStruct->sName;
              rlsStructs.splice(itThisStruct++, rlsStructs, itOtherStruct);
              // now itThisStruct points to new pos of the itThisStruct
              bWasChanged = true;
              bFound = true;
              break;
            }
          }

          ++itThisStruct;
        }

        for (std::list<SStruct>::iterator itStruct = rlsStructs.begin();
            itStruct != rlsStructs.end(); ++itStruct)
        {
          bWasChanged |= FixStructOrderByUsing(itStruct->lsStructs);
        }

        if (!bWasChanged)
        {
          break;
        }
      }

      if (bWasChanged)
      {
        rise::LogWarning() << "Failed to reorder structures. Build may fail";
      }

      return bWasChanged;
    }


    void CheckAndFixUnknownDataType(SDataType& rstDataType, SStruct* pstStruct = NULL)
    {
      if (rstDataType.eType == SDataType::EUnknown)
      {
        rise::LogDebug() << "2nd pass: fixing datatype: " << rstDataType.sName;
        ParseDataType(rstDataType.sNamespace + rstDataType.sName, rstDataType, pstStruct);
      }
      
      if (rstDataType.eType == SDataType::ETemplate && !rstDataType.lsParams.empty())
      {
        SDataType& rstTemplParam = rstDataType.lsParams.front();
        if (rstTemplParam.eType == SDataType::EUnknown)
        {
          rise::LogDebug() << "2nd pass fixing datatype: " << rstTemplParam.sName;
          ParseDataType( rstTemplParam.sNamespace + rstTemplParam.sName, rstTemplParam, pstStruct);
        }
      }
    }

    void FixUnknownDataTypesInStructs(std::list<SStruct>& rlsStructs)
    {
      // search unknown data types in structures
      for (std::list<SStruct>::iterator itStruct = rlsStructs.begin();
          itStruct != rlsStructs.end(); ++itStruct)
      {
        for (std::list<SParam>::iterator itMember = itStruct->lsMembers.begin();
            itMember != itStruct->lsMembers.end(); ++itMember)
        {
          CheckAndFixUnknownDataType(itMember->stDataType, &*itStruct);
        }
        FixUnknownDataTypesInStructs(itStruct->lsStructs);
      }
    }

    void FixUnknownDataTypesInClasses(std::list<SClass>& rlsClasses)
    {
      // search unknown data types in services
      for (std::list<SClass>::iterator itClass = rlsClasses.begin();
          itClass != rlsClasses.end(); ++itClass)
      {
        for (std::list<SMember>::iterator itMember = itClass->lsMembers.begin();
            itMember != itClass->lsMembers.end(); ++itMember)
        {
          CheckAndFixUnknownDataType(itMember->stReturn.stDataType);

          for (std::list<SParam>::iterator itParam = itMember->lsParams.begin();
              itParam != itMember->lsParams.end(); ++itParam)
          {
            CheckAndFixUnknownDataType(itParam->stDataType);
          }

        }
      }
    }


    // Interface
    const SInterface& Parse( const std::string& sInDir, const std::string& sFileName, SProject& rProject )
    {
      m_pProject = &rProject;
      m_sInDir = sInDir;

      std::string::size_type nPos = sFileName.find_last_of("/\\");
      const std::string& sInterfaceFileName = (nPos != std::string::npos) ?
                                              sFileName.substr(nPos + 1) : sFileName;
      const std::string& sInterfaceFilePath = (nPos != std::string::npos) ?
                                              sFileName.substr(0, nPos + 1) : "";

      for (std::list<SInterface>::const_iterator itInterface = rProject.lsInterfaces.begin();
          itInterface != rProject.lsInterfaces.end(); ++itInterface)
      {
        if (itInterface->sFileName == sInterfaceFileName &&
            itInterface->sFilePath == sInterfaceFilePath)
        {
          return *itInterface; // already parsed
        }
      }

      rProject.lsInterfaces.push_back(m_stInterface);
      SInterface& rProjectThisInterface = rProject.lsInterfaces.back();

      m_stInterface.sFileName = sInterfaceFileName;
      m_stInterface.sFilePath = sInterfaceFilePath;

      const std::string& sFilePath = m_sInDir + sFileName;

      rise::LogDebug() << "processing file: " << sFilePath;
      m_tFile.open(sFilePath.c_str());
      CSP_ASSERT(m_tFile.good(), std::string("can't open file: ") + sFilePath + ": "
                 + std::string(strerror(errno)), m_stInterface.sFileName, m_nLine);
      try
      {
        m_stInterface.sName = m_stInterface.sFileName.substr(m_stInterface.sFileName.size() - 6, 6) == ".proto" ?
            m_stInterface.sFileName.substr(0, m_stInterface.sFileName.size() - 6) :
            m_stInterface.sFileName;

        ParseHeader(m_stInterface);
        CorrectStuctParentNs();

        // detect interface main namespace
        if (m_stInterface.sNamespace.empty())
        {
          for (std::list<SClass>::const_iterator itClass = m_stInterface.lsClasses.begin();
              itClass != m_stInterface.lsClasses.end(); ++itClass)
          {
            if (!itClass->sNamespace.empty())
            {
              m_stInterface.sNamespace = itClass->sNamespace;
              break;
            }
          }
        }

        if (m_stInterface.sNamespace.empty())
        {
          for (std::list<SStruct>::const_iterator itStruct = m_stInterface.lsStructs.begin();
              itStruct != m_stInterface.lsStructs.end(); ++itStruct)
          {
            if (!itStruct->sNamespace.empty())
            {
              m_stInterface.sNamespace = itStruct->sNamespace;
              break;
            }
          }
        }
        
        FixUnknownDataTypesInStructs(m_stInterface.lsStructs);
        FixUnknownDataTypesInClasses(m_stInterface.lsClasses);
        FixStructOrderByUsing(m_stInterface.lsStructs);

        rProjectThisInterface = m_stInterface;
        m_tFile.close();
      }
      catch (CParseException& rParseException)
      {
        std::stringbuf sbData;
        m_tFile.get(sbData, '\n');
        m_tFile.ignore();
        m_tFile.get(sbData, '\n');

        rParseException.Message() += ": before\n-----------------\n" + sbData.str() + "\n-----------------\n";

        throw rParseException;
      }

      return m_stInterface;
    }
    
    void CorrectStuctParentNs()
    {
      // correct structs parent namespaces
      for (std::list<SStruct>::iterator itStruct = m_stInterface.lsStructs.begin();
          itStruct != m_stInterface.lsStructs.end(); ++itStruct)
      {
        std::string& sNsParent = itStruct->sParentName;
        // skip structs with no parent and with namespace, declared from global scope
        if (sNsParent.empty() || sNsParent.substr(0, 2) == "::")
        {
          continue;
        }

        const SStruct* pstParent = GetStruct(sNsParent, m_stInterface);
        if (pstParent)
        {
          itStruct->sParentName = pstParent->sName;
          itStruct->sParentNamespace = pstParent->sNamespace;
        }
      }
    }

    std::string m_sInDir;
    std::string m_sRootNamespace;
    std::string m_sCurrentNamespace;
    std::ifstream m_tFile;
    int m_nLine;
    SInterface m_stInterface;
    SProject* m_pProject;
  };


  const std::string& CProtobufParser::GetId()
  {
    return m_sId;
  }

  void CProtobufParser::Process(const SParseSettings& rParseSettings, SProject& rProject)
  {
    unsigned uServicesCount = 0;

    std::string sRootNs = "::";
    TStringMap::const_iterator itRootNs = rParseSettings.mEnv.find("rootns");
    if (itRootNs != rParseSettings.mEnv.end() && !itRootNs->second.empty())
    {
      sRootNs = "::" + itRootNs->second + "::";
      rise::StrReplace(sRootNs, ".", "::", true);
    }

    for (TStringList::const_iterator itFile = rParseSettings.lsFiles.begin();
        itFile != rParseSettings.lsFiles.end(); ++itFile)
    {
      rise::LogDebug() << "---- " << *itFile << " -------------------------------------------------------";
      CProtobufHeaderParser tProtobufHeaderParser(sRootNs);
      const SInterface& rInterface = tProtobufHeaderParser.Parse(rParseSettings.sInDir, *itFile, rProject);
      uServicesCount += rInterface.lsClasses.size();
    }

    TStringMap::const_iterator itComponentNs = rParseSettings.mEnv.find("componentns");
    if (itComponentNs != rParseSettings.mEnv.end())
    {
      rProject.sNamespace = "::" + itComponentNs->second + "::";
      rise::StrReplace(rProject.sNamespace, ".", "::", true);
    }
    else
    { // autodetect: take first defined namespace
      for (std::list<SInterface>::const_iterator itInterface = rProject.lsInterfaces.begin();
        itInterface != rProject.lsInterfaces.end(); ++itInterface)
      {
        for (std::list<SClass>::const_iterator itClass = itInterface->lsClasses.begin();
            itClass != itInterface->lsClasses.end(); ++itClass)
        {
          if (!itClass->sNamespace.empty())
          {
            rProject.sNamespace = itClass->sNamespace;
            break;
          }
        }

        if (!rProject.sNamespace.empty())
        {
          break;
        }
      }
    }

    CSP_ASSERT(!(uServicesCount == 0 && rParseSettings.bNoServiceError),
               "No staff service interfaces found. Staff services must inherited from staff::IService.\n"
                "Example:\n----\n#include <staff/common/IService.h>\n\n  class Calc: public staff::IService\n"
                "  {\n  public:\n    virtual int Add(int nA, int nB) = 0;\n  };\n----\n\n",
                "", 0);
  }

  const std::string CProtobufParser::m_sId = "protobuf";
}
}
