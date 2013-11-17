// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// Client skeleton

#include <memory>
#include <staff/utils/Log.h>
#include <staff/common/logoperators.h>
#include <staff/common/Exception.h>
#include <staff/common/DataObject.h>
#include "floatPrecProxy.h"

#define Num 1.12345678901234567890123456789L

int main(int /*nArgs*/, const char* /*paszArgs*/[])
{
  try
  {
    staff::DataObject doTest1("test1");

    ::TestPrec test;
    test.longDoubleValueNoPrec = 0;
    test.doubleValueNoPrec = 0;
    test.floatValueNoPrec = 0;
    test.longDoubleValue = 0;
    test.doubleValue = 0;
    test.floatValue = 0;
    test.attrFloatValueNoPrec = 0;
    test.attrFloatValue = 0;

    doTest1 << test;

    staff::LogDebug() << doTest1.ToString();

    STAFF_ASSERT(doTest1.GetChildTextByLocalName("longDoubleValueNoPrec") == "0", "longDoubleValueNoPrec 1");
    STAFF_ASSERT(doTest1.GetChildTextByLocalName("doubleValueNoPrec") == "0", "doubleValueNoPrec 1");
    STAFF_ASSERT(doTest1.GetChildTextByLocalName("floatValueNoPrec") == "0", "floatValueNoPrec 1");
    STAFF_ASSERT(doTest1.GetChildTextByLocalName("longDoubleValue") == "0.0000000000000000", "longDoubleValue 1");
    STAFF_ASSERT(doTest1.GetChildTextByLocalName("doubleValue") == "0.000000000000", "doubleValue 1");
    STAFF_ASSERT(doTest1.GetChildTextByLocalName("floatValue") == "0.0000", "floatValue 1");
    STAFF_ASSERT(doTest1.GetAttributeTextByName("attrFloatValueNoPrec") == "0", "attrFloatValueNoPrec 1");
    STAFF_ASSERT(doTest1.GetAttributeTextByName("attrFloatValue") == "0.0000", "attrFloatValue 1");



    staff::DataObject doTest2("test2");

    test.longDoubleValueNoPrec = Num;
    test.doubleValueNoPrec = Num;
    test.floatValueNoPrec = Num;
    test.longDoubleValue = Num;
    test.doubleValue = Num;
    test.floatValue = Num;
    test.attrFloatValueNoPrec = Num;
    test.attrFloatValue = Num;

    doTest2 << test;

    staff::LogDebug() << doTest2.ToString();

    STAFF_ASSERT(doTest2.GetChildTextByLocalName("longDoubleValueNoPrec") == "1", "longDoubleValueNoPrec 2");
    STAFF_ASSERT(doTest2.GetChildTextByLocalName("doubleValueNoPrec") == "1", "doubleValueNoPrec 2");
    STAFF_ASSERT(doTest2.GetChildTextByLocalName("floatValueNoPrec") == "1", "floatValueNoPrec 2");
    STAFF_ASSERT(doTest2.GetChildTextByLocalName("longDoubleValue") == "1.1234567890123457", "longDoubleValue 2");
    STAFF_ASSERT(doTest2.GetChildTextByLocalName("doubleValue") == "1.123456789012", "doubleValue 2");
    STAFF_ASSERT(doTest2.GetChildTextByLocalName("floatValue") == "1.1235", "floatValue 2");
    STAFF_ASSERT(doTest2.GetAttributeTextByName("attrFloatValueNoPrec") == "1", "attrFloatValueNoPrec 2");
    STAFF_ASSERT(doTest2.GetAttributeTextByName("attrFloatValue") == "1.1235", "attrFloatValue 2");

    staff::LogInfo() << "tests are ok";

  }
  STAFF_CATCH_ALL

  return 0;
}

