// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT
\
#ifneq($(Interface.Namespace.!dot),)

staff.namespace("$(Interface.Namespace.!dot)");
#ifeqend

(function(){
#cginclude "Serialization.js"
#foreach $(Interface.Classes)
// service $(Class.Name)
$(Class.ServiceNsName) = function(sAddress, sSessionId, sInstanceId)
{
  if (!sAddress)
  {
    sAddress = "$($serviceuri||Class.Options.*serviceUri||Class.ServiceNsName.!prepend/\/axis2\/services\//)"
  }

  var _this = this;
  _this._oClient = new staff.SoapClient(sAddress, sSessionId, sInstanceId);
  _this._sSessionId = sSessionId;
  _this._sInstanced = sInstanceId;

  _this._sTns = \
#ifneq($(Class.Options.*targetNamespace),)
"$(Class.Options.*targetNamespace)";
#else
"http://localhost/axis2/services/$(Class.ServiceNsName)";
#ifeqend
  _this._sTnsPrefix = \
#ifneq($(Class.Options.*targetNamespacePrefix),)
"$(Class.Options.*targetNamespacePrefix)";
#else
"ns1";
#ifeqend
#ifneq($(Class.Options.*soapVersion),)
  _this._oClient.setSoapVersion("$(Class.Options.*soapVersion)");
#ifeqend
#ifneq($(Class.Options.*soapVersionUri),)
  _this._oClient.setSoapVersionUri("$(Interface.Options.*soapVersionUri)");
#ifeqend

  if (sInstanceId)
  {
    var oRequestElem = new staff.Element("CreateInstance");
    oRequestElem.createChild("sInstanceId").setText(sInstanceId);
    _this._oClient.invoke(oRequestElem);
  }

  // used to destroy the instance
  _this.destroy = function()
  {
    if (_this._sInstanced)
    {
      var oRequestElem = new staff.Element("FreeInstance");
      oRequestElem.createChild("sInstanceId").setText(_this._sInstanced);
      _this._oClient.invoke(oRequestElem);
    }
  }

  _this.getClient = function()
  {
    return _this._oClient;
  }

#foreach $(Class.Members)
  _this.$(Member.Name) = function(\
#foreach $(Member.Params)
$(Param.Name), \
#end
fnCallback)
  {
    var oRequestElem = new staff.Element("$(Member.Options.*requestElement||Member.Name)", _this._sTnsPrefix, _this._sTns);

#foreach $(Member.Params)
#ifeq($(Param.DataType.Type),generic||string)
    oRequestElem.createChild("$(Param.Name)").setText($(Param.Name));
#else
#ifeq($(Param.DataType.Type),dataobject)
    oRequestElem.createChild("$(Param.Name)").appendChild($(Param.Name));
#else
#ifeq($(Param.DataType.Type),struct||typedef||template||enum)
    $(Param.DataType.NsName.!dot).serialize(oRequestElem.createChild("$(Param.Name)"), $(Param.Name));
#else
#cgerror "unsupported Param.DataType.Type = $(Param.DataType.Type);"
#ifeqend
#ifeqend
#ifeqend
#end
    function parseResponse(oData)
    {
#ifneq($(Member.Return.Name),void)
      var oResultElement = staff.getChildElement(oData, \
#ifeq($(Member.Options.*responseElement),)
"$(Member.Name)Result");
#else
"$(Member.Options.responseElement)");
#ifneq($(Member.Options.*resultElement),)
      oResultElement = staff.getChildElement(oResultElement, "$(Member.Options.resultElement)");
#ifeqend
#ifeqend
\
\
#ifeq($(Member.Return.Type),string) // -------------------------
      return staff.getElementValue(oResultElement);

#else
#ifeq($(Member.Return.Type),generic)
\
      return \
#ifeq($(Member.Return.Name),byte||int||long||short||unsigned)
parseInt(staff.getElementValue(oResultElement));
#else
#ifeq($(Member.Return.Name),float||double)
parseFloat(staff.getElementValue(oResultElement));
#else
#ifeq($(Member.Return.Name),bool)
staff.getElementValue(oResultElement).toLowerCase() == "true";
#else
#cgwarning unsupported generic: $(Param.DataType.Name), parsing as string
      // unsupported generic: $(Param.DataType.Name)
      return staff.getElementValue(oResultElement);
#ifeqend // bool
#ifeqend // float
#ifeqend // int
\
#else
#ifeq($(Member.Return.Type),dataobject) // -------------------------
      return oResultElement;
#else
#ifeq($(Member.Return.Type),struct||typedef||template||enum) // -------------------------
      return $(Member.Return.NsName.!dot).deserialize(oResultElement);
#else
#cgerror "Member.Return.Type = $(Member.Return.Type);"
#ifeqend // struct/typedef/enum
#ifeqend // dataobject
#ifeqend // generic
#ifeqend // string
#ifeqend // !void
    }

#ifneq($(Member.Options.*soapAction),)
    _this._oClient.setSoapAction("$(Member.Options.*soapAction)");
#ifeqend
#ifneq($(Member.Options.*restMethod),)
    _this._oClient.setHttpHeader("Method", "$(Member.Options.*restMethod)");
#ifeqend
#ifneq($(Member.Options.*timeout),)
    _this._oClient.setTimeout($(Member.Options.*timeout));
#ifeqend
    if (typeof(fnCallback) != "function")
    { // synchronous call
      return parseResponse(_this._oClient.invoke(oRequestElem));
    }

    // asynchronous call
    _this._oClient.invoke(oRequestElem,
      function(oData)
      {
        fnCallback(parseResponse(oData));
      });
  }

#end // foreach Class.Members
}

#end // foreach Interface.Classes
})();
