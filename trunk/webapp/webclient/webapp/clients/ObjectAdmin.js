// This file generated by staff_codegen
// DO NOT EDIT
namespace('staff.admin');

///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct serializators
function SerializeStruct_staff_admin_SPermission(tOperation, rstStruct, tNode)
{
  tOperation.AddParameter('bRead', rstStruct.bRead, tNode);
  tOperation.AddParameter('bWrite', rstStruct.bWrite, tNode);
  tOperation.AddParameter('bExecute', rstStruct.bExecute, tNode);
}

function SerializeStruct_staff_admin_SPermissions(tOperation, rstStruct, tNode)
{
  SerializeStruct_staff_admin_SPermission(tOperation, rstStruct.stUser, tOperation.AddParameter('stUser', '', tNode));
  SerializeStruct_staff_admin_SPermission(tOperation, rstStruct.stGroup, tOperation.AddParameter('stGroup', '', tNode));
  SerializeStruct_staff_admin_SPermission(tOperation, rstStruct.stOthers, tOperation.AddParameter('stOthers', '', tNode));
}

function SerializeStruct_staff_admin_SObject(tOperation, rstStruct, tNode)
{
  tOperation.AddParameter('nObjectId', rstStruct.nObjectId, tNode);
  tOperation.AddParameter('sObjectName', rstStruct.sObjectName, tNode);
  tOperation.AddParameter('nType', rstStruct.nType, tNode);
  tOperation.AddParameter('sDescription', rstStruct.sDescription, tNode);
  tOperation.AddParameter('nUserId', rstStruct.nUserId, tNode);
  tOperation.AddParameter('nGroupId', rstStruct.nGroupId, tNode);
  tOperation.AddParameter('nParentObjectId', rstStruct.nParentObjectId, tNode);
  SerializeStruct_staff_admin_SPermissions(tOperation, rstStruct.stPermissions, tOperation.AddParameter('stPermissions', '', tNode));
}

function SerializeStruct_staff_admin_SObjectType(tOperation, rstStruct, tNode)
{
  tOperation.AddParameter('nId', rstStruct.nId, tNode);
  tOperation.AddParameter('sName', rstStruct.sName, tNode);
  tOperation.AddParameter('sDescription', rstStruct.sDescription, tNode);
}


///////////////////////////////////////////////////////////////////////////////////////////////////////
// struct deserializators
function DeserializeStruct_staff_admin_SPermission(tOperation, tNode)
{
  var tResult = {};

  tResult.bRead = tOperation.SubNodeText("bRead", tNode);
  tResult.bWrite = tOperation.SubNodeText("bWrite", tNode);
  tResult.bExecute = tOperation.SubNodeText("bExecute", tNode);
  return tResult;
}

function DeserializeStruct_staff_admin_SPermissions(tOperation, tNode)
{
  var tResult = {};

  tResult.stUser = DeserializeStruct_staff_admin_SPermission(tOperation, tOperation.SubNode("stUser", tNode));
  tResult.stGroup = DeserializeStruct_staff_admin_SPermission(tOperation, tOperation.SubNode("stGroup", tNode));
  tResult.stOthers = DeserializeStruct_staff_admin_SPermission(tOperation, tOperation.SubNode("stOthers", tNode));
  return tResult;
}

function DeserializeStruct_staff_admin_SObject(tOperation, tNode)
{
  var tResult = {};

  tResult.nObjectId = tOperation.SubNodeText("nObjectId", tNode);
  tResult.sObjectName = tOperation.SubNodeText("sObjectName", tNode);
  tResult.nType = tOperation.SubNodeText("nType", tNode);
  tResult.sDescription = tOperation.SubNodeText("sDescription", tNode);
  tResult.nUserId = tOperation.SubNodeText("nUserId", tNode);
  tResult.nGroupId = tOperation.SubNodeText("nGroupId", tNode);
  tResult.nParentObjectId = tOperation.SubNodeText("nParentObjectId", tNode);
  tResult.stPermissions = DeserializeStruct_staff_admin_SPermissions(tOperation, tOperation.SubNode("stPermissions", tNode));
  return tResult;
}

function DeserializeStruct_staff_admin_SObjectType(tOperation, tNode)
{
  var tResult = {};

  tResult.nId = tOperation.SubNodeText("nId", tNode);
  tResult.sName = tOperation.SubNodeText("sName", tNode);
  tResult.sDescription = tOperation.SubNodeText("sDescription", tNode);
  return tResult;
}

//-----------------------------------------------------------------------------------------------------


///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef serializators

// ::staff::admin::TObjectIdList  Typedef.DataType.Type template std::vector
function SerializeTypedef_staff_admin_TObjectIdList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = generic
  tOperation.AddParameter('Item', rtType[i], tNode);
  }
  return tNode;
}

// ::staff::admin::TObjectList  Typedef.DataType.Type template std::list
function SerializeTypedef_staff_admin_TObjectList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = struct
    SerializeStruct_staff_admin_SObject(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode));
  }
  return tNode;
}

// ::staff::admin::TObjectTypesList  Typedef.DataType.Type template std::list
function SerializeTypedef_staff_admin_TObjectTypesList(tOperation, rtType, tNode)
{
  for(var i = 0; i != rtType.length; ++i)
  {
// Typedef.DataType.TemplateParams.TemplateParam1.Type = struct
    SerializeStruct_staff_admin_SObjectType(tOperation, rtType[i], tOperation.AddParameter('Item', '', tNode));
  }
  return tNode;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// typedef deserializators
function DeserializeTypedef_staff_admin_TObjectIdList(tOperation, tNode)
{
// container :: std::vector< int >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = new Array();
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::vector<int>
    aResult[j++] = tResult.childNodes[i].firstChild != null ? tResult.childNodes[i].firstChild.nodeValue : ""; // *** generic int
    }
  }

  return aResult;
}

function DeserializeTypedef_staff_admin_TObjectList(tOperation, tNode)
{
// container :: std::list< ::staff::admin::SObject >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = new Array();
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::list<::staff::admin::SObject>
    aResult[j++] = DeserializeStruct_staff_admin_SObject(tOperation, tResult.childNodes[i]); // *** struct ::staff::admin::SObject
    }
  }

  return aResult;
}

function DeserializeTypedef_staff_admin_TObjectTypesList(tOperation, tNode)
{
// container :: std::list< ::staff::admin::SObjectType >
  var tItem = null;

  var tResult = tNode == null ? tOperation.ResultElement() : tNode;
  var aResult = new Array();
  var j = 0;

  for (var i = 0; i < tResult.childNodes.length; i++)
  {
    if( tResult.childNodes[i].nodeName == "Item")
    {
//template std::list<::staff::admin::SObjectType>
    aResult[j++] = DeserializeStruct_staff_admin_SObjectType(tOperation, tResult.childNodes[i]); // *** struct ::staff::admin::SObjectType
    }
  }

  return aResult;
}


//-----------------------------------------------------------------------------------------------------

///////////////////////////////////////////////////////////////////////////////////
// class: staff.admin.ObjectAdmin

staff.admin.ObjectAdmin = Class.create();
staff.admin.ObjectAdmin.prototype = 
{
  initialize: function(sServiceUri, sSessionId, sTargetNamespace)
  {
    if (!sServiceUri)
    {
      sServiceUri = webapp.Env.protocol + Session.sHost + (Session.sPort ? (':' + Session.sPort) : '') + '/axis2/services/staff.admin.ObjectAdmin';
    }
    
    if (!sTargetNamespace)
    {
      sTargetNamespace = sServiceUri;
    }
    
    this.sTargetNamespace = sTargetNamespace || sServiceUri;

    this.tClient = new staff.Client(sServiceUri, sSessionId || Session.sID || "");
  },
  
  SetID: function(sSessionId)
  {
    this.tClient.SetSessionId(sSessionId);
  },

  GetObjectIdList: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetObjectIdList', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_staff_admin_TObjectIdList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_staff_admin_TObjectIdList(tOperation);
    }
  },

  GetObjectIdListWithParent: function(nParentId, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetObjectIdListWithParent', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('nParentId', nParentId);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_staff_admin_TObjectIdList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_staff_admin_TObjectIdList(tOperation);
    }
  },

  GetObjectById: function(nObjectId, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetObjectById', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('nObjectId', nObjectId);
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeStruct_staff_admin_SObject(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeStruct_staff_admin_SObject(tOperation);
    }
  },

  GetObjectList: function(rlsObjectIdList, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetObjectList', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeTypedef_staff_admin_TObjectIdList(tOperation, rlsObjectIdList, tOperation.AddParameter('rlsObjectIdList'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_staff_admin_TObjectList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_staff_admin_TObjectList(tOperation);
    }
  },

  AddObject: function(rstObject, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AddObject', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_staff_admin_SObject(tOperation, rstObject, tOperation.AddParameter('rstObject'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return tOperation.ResultValue();
    }
  },

  RemoveObject: function(nObjectId, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('RemoveObject', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('nObjectId', nObjectId);
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

  ReplaceObject: function(rstObject, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('ReplaceObject', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_staff_admin_SObject(tOperation, rstObject, tOperation.AddParameter('rstObject'));
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

  GetObjectTypeList: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetObjectTypeList', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(DeserializeTypedef_staff_admin_TObjectTypesList(tOperation), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return DeserializeTypedef_staff_admin_TObjectTypesList(tOperation);
    }
  },

  AddObjectType: function(rstObjectType, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('AddObjectType', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_staff_admin_SObjectType(tOperation, rstObjectType, tOperation.AddParameter('rstObjectType'));
    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue(), tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return tOperation.ResultValue();
    }
  },

  ReplaceObjectType: function(rstObjectType, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('ReplaceObjectType', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    SerializeStruct_staff_admin_SObjectType(tOperation, rstObjectType, tOperation.AddParameter('rstObjectType'));
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

  RemoveObjectType: function(nObjectTypeId, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('RemoveObjectType', this.sTargetNamespace, '', '');
    tOperation.SetSoapAction("");
    
    tOperation.AddParameter('nObjectTypeId', nObjectTypeId);
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

