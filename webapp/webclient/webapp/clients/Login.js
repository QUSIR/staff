// This file generated by staff_codegen
// For more information please visit: http://code.google.com/p/staff/
// DO NOT EDIT

namespace('staff');

(function(){
///////////////////////////////////////////////////////////////////////////////////
// class: staff.Login

staff.Login = Class.create();
staff.Login.prototype =
{
  initialize: function(sServiceUri, sSessionId, sInstanceId, sTargetNamespace)
  {
    if (!sServiceUri)
    {
      sServiceUri = webapp.Env.protocol + webapp.Session.sHost + (webapp.Session.sPort ? (':' + webapp.Session.sPort) : '') + '/axis2/services/staff.Login';
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

  Login: function(sUserName, sPassword, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Login', this.sTargetNamespace, '', '');

    tOperation.AddParameter('sUserName', sUserName);
    tOperation.AddParameter('sPassword', sPassword);
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

  OpenSession: function(sUserName, sPassword, bCloseExisting, pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('OpenSession', this.sTargetNamespace, '', '');

    tOperation.AddParameter('sUserName', sUserName);
    tOperation.AddParameter('sPassword', sPassword);
    tOperation.AddParameter('bCloseExisting', bCloseExisting);
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

  Logout: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('Logout', this.sTargetNamespace, '', '');

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

  KeepAliveSession: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('KeepAliveSession', this.sTargetNamespace, '', '');

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

  ValidateSession: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('ValidateSession', this.sTargetNamespace, '', '');

    if(typeof pOnComplete == 'function')
    { // make async call
      this.tClient.InvokeOperation(tOperation,
        function(tOperation)
        {
          pOnComplete(tOperation.ResultValue().toLowerCase() in {"1":1, "true":1}, tOperation);
        },
        pOnError
      );
    }
    else
    {
      this.tClient.InvokeOperation(tOperation);

      return tOperation.ResultValue().toLowerCase() in {"1":1, "true":1};
    }
  },

  GetUserName: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetUserName', this.sTargetNamespace, '', '');

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

  GetUserId: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetUserId', this.sTargetNamespace, '', '');

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

  GetSessionExpiration: function(pOnComplete, pOnError)
  {
    var tOperation = new staff.Operation('GetSessionExpiration', this.sTargetNamespace, '', '');

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
  }
}


})();