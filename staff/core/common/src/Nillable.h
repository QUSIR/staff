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


#ifndef _STAFF_NILLABLE_H_
#define _STAFF_NILLABLE_H_

#include <rise/common/MutablePtr.h>

namespace staff
{

  template <typename Type>
  class Nillable
  {
  public:
    Nillable()
    {
    }

    Nillable(const Type& rData):
      m_tpData(new Type(rData))
    {
    }

    Nillable(const Nillable& rOpt):
      m_tpData(const_cast< Nillable<Type>& >(rOpt).m_tpData)
    {
    }

    inline Nillable<Type>& operator=(const Type& rData)
    {
      Get() = rData;
      return *this;
    }

    inline Nillable<Type>& operator=(const Nillable<Type>& rOpt)
    {
      m_tpData = const_cast< Nillable<Type>& >(rOpt).m_tpData;
      return *this;
    }

    inline bool operator==(const Type& rOpt) const
    {
      if (!m_tpData.Get())
      {
        return false;
      }

      return *m_tpData.Get() == rOpt;
    }

    inline operator Type&()
    {
      return Get();
    }

    inline operator const Type&() const
    {
      return Get();
    }

    inline Type* operator->()
    {
      return &Get();
    }

    inline const Type* operator->() const
    {
      return &(const_cast< Nillable<Type>* >(this)->Get());
    }

    inline Type& operator*()
    {
      return Get();
    }

    inline const Type& operator*() const
    {
      return const_cast< Nillable<Type>* >(this)->Get();
    }

    inline void Reset()
    {
      m_tpData.Set(NULL);
    }

    inline bool IsNull() const
    {
      return !m_tpData.Get();
    }

    inline bool IsSet() const
    {
      return !!m_tpData.Get();
    }

    inline Type& Get()
    {
      Type* pData = m_tpData.Get();
      if (!pData)
      {
        pData = new Type;
        m_tpData.Set(pData);
      }

      return *pData;
    }

  private:
    rise::CMutablePtr<Type> m_tpData;
  };

} // namespace staff

#endif // _STAFF_NILLABLE_H_
