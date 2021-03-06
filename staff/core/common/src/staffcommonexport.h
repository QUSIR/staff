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

#ifndef _STAFFCOMMONEXPORT_H_
#define _STAFFCOMMONEXPORT_H_

#ifdef WIN32
  #ifdef _MSC_VER
    #pragma warning(disable: 4786 4251)
  #endif

  #ifdef STAFF_COMMON_DLL_EXPORTS
    #define STAFF_COMMON_EXPORT __declspec(dllexport)
  #else
    #define STAFF_COMMON_EXPORT __declspec(dllimport)
  #endif
#else
  #define STAFF_COMMON_EXPORT 
#endif

#ifndef STAFF_DEPRECATED
  #ifdef _MSC_VER
    #if _MSC_FULL_VER >= 140050320
      #define STAFF_DEPRECATED(Replacement) __declspec(deprecated("You are using deprecated API. Consider using " #Replacement " instead."))
    #else
      #define STAFF_DEPRECATED(Replacement) __declspec(deprecated)
    #endif
  #elif __GNUC__ >= 3
    #define STAFF_DEPRECATED(Replacement) __attribute__ ((deprecated))
  #else
    #define STAFF_DEPRECATED(Replacement)
  #endif
#endif

#endif // _STAFFCOMMONEXPORT_H_

/*! \namespace staff 
    \brief staff namespace
*/

/*! \mainpage Staff

Staff is Web Service Framework for C++/JavaScript/J2ME based on Apache Axis2/C.

Together with the support of all functions of Apache Axis2/C, WSF Staff provides additional features:
 - stateful web services and clients implementation in C++, client implementaion in JavaScript, J2ME;
 - service-component architecture implementation;
 - security support (acls. users, groups support, access to components, services, operations, widgets, etc.);
 - extensible code generator.

Code generator include following common templates:
  - web service interface generation by WSDL file. This interface is an abstract C++ class that can be used for creating web services and clients;
  - WSDL generation by web service interface;
  - BPEL support: codegen supports WSDL generation compatible with BPEL;
  - web service stub generation;
  - client skeleton generation;
  - internal files generation (Proxies, Wrappers, etc).

See also <a href="annotated.html">annotated class list</a>, implemented by library.
*/


