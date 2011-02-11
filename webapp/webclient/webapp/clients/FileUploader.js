// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

namespace('webapp.admin');

(function(){

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef serializators

// ::webapp::admin::TStringList  Typedef.DataType.Type template std::list
function SerializeTypedef_webapp_admin_TStringList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = string
    tOperation.AddParameter('Item', rtType[i], tNode);
  }
  return tNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef deserializators
function DeserializeTypedef_webapp_admin_TStringList(tOperation, tNode)
{
// container :: std::list< std::string >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = [];
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if (tResult.childNodes[i].nodeName == 'Item')
    {
//template std::list<std::string>
      aResult[j++] = tResult.childNodes[i].firstChild != null ? tResult.childNodes[i].firstChild.nodeValue : '';
    }
  }

  return aResult;
}


//-----------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////
// class: webapp.admin.FileUploader

webapp.admin.FileUploader = Class.create();
webapp.admin.FileUploader.prototype =
{
  initialize: function(sServiceUri, sSessionId, sInstanceId, sTargetNamespace)
  {
    if (!sServiceUri)
    {
      sServiceUri = webapp.Env.protocol + webapp.Session.sHost + (webapp.Session.sPort ? (':' + webapp.Session.sPort) : '') + '/axis2/services/webapp.admin.FileUploader';
    }

    if (!sTargetNamespace)
    {
      sTargetNamespace = sServiceUri;
    }

    this.sTargetNamespace = sTargetNamespace || sServiceUri;

    this.tClient = new staff.Client(sServiceUri, sSessionId || webapp.Session.sId || '', sInstanceId);
  },

  destroy: function()
  {
    this.tClient && this.tClient.destroy();
  },

  SetDataObjectAsXml: function(bDataObjectAsXml)
  {
    this.bDataObjectAsXml = bDataObjectAsXml;
  },

  SetID: function(sSessionId)
  {
    this.tClient.SetSessionId(sSessionId);
  },

  Move: function(sFileName, sPathTo, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Move', this.sTargetNamespace, '', '');

    tOperation.AddParameter('sFileName', sFileName);
    tOperation.AddParameter('sPathTo', sPathTo);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  Unpack: function(sFileName, sPathTo, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Unpack', this.sTargetNamespace, '', '');

    tOperation.AddParameter('sFileName', sFileName);
    tOperation.AddParameter('sPathTo', sPathTo);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  },

  GetUnpackingStatus: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetUnpackingStatus', this.sTargetNamespace, '', '');

    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(parseInt(tOperation.ResultValue()), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return parseInt(tOperation.ResultValue());
    }
  },

  GetUnpackedFiles: function(sMask, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetUnpackedFiles', this.sTargetNamespace, '', '');

    tOperation.AddParameter('sMask', sMask);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_webapp_admin_TStringList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_webapp_admin_TStringList(tOperation);
    }
  },

  Delete: function(sFileName, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Delete', this.sTargetNamespace, '', '');

    tOperation.AddParameter('sFileName', sFileName);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);
    }
  }
}


})();