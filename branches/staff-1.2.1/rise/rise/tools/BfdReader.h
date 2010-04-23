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

#ifndef _BFDREADER_H_
#define _BFDREADER_H_

#include <rise/os/osdllexport.h>
#include <rise/string/String.h>
#include <rise/tools/AddrInfo.h>

namespace rise
{
  namespace tools
  {
    //!  Binary File Descriptor reader
    class RISE_EXPORT CBfdReader
    {
    public:
      //!         constructor
      CBfdReader();

      //!         destructor
      ~CBfdReader();
      
      //!         open binary file for dump
      /*! \param  sFileName - file name
          \return true - file successfully opened
          */
      bool Open(const CString& sFileName);

      //!         close file
      void Close();
      
      //!         if file open
      /*! \return true - file is open
      */
      bool IsOpen();

      //!         get file name
      /*! \return file name
          */
      const rise::CString& GetFileName();

      //!         lookup address info by pointer
      /*! \param  pAddr - address pointer
          \param  rAddrInfo - address info
          \return true - info was successfully get
          */
      bool LookupAddrPtr( const void* pAddr, SAddrInfo& rAddrInfo );

      //!         lookup address info by pointer
      /*! \param  sAddrHex - hexdecimal address
          \param  rAddrInfo - address info
          \return true - info was successfully get
          */
      bool LookupAddr( const CString& sAddrHex, SAddrInfo& rAddrInfo );

    private:
      CBfdReader& operator=(const CBfdReader&);
      CBfdReader(const CBfdReader&);
      class CBfdReaderImpl;
      CBfdReaderImpl* m_pImpl;
    };
  }
}

#endif // _BFDREADER_H_