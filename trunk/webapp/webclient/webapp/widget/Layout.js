namespace('webapp.widget');

///////////////////////////////////////////////////////////////
// class Layout

webapp.widget.Layout = Class.create();
webapp.widget.Layout.prototype.extend(webapp.widget.Widget.prototype).extend
({
  Create: function(tParent, tOpts)
  {
    IncludeCss("webapp/assets/widgets/Layout/Layout");

    this.tLayoutUnits = 
      this.tProperties.sLayout ? this.DeserializeProps(this.tProperties.sLayout) : this._GetDefaultLayoutUnits();
        
    this.CreateControls(tParent, this.tLayoutUnits);
    
    var tParentDiv = new webapp.ui.Div(tParent);
    this.tParentElem = tParentDiv.Element();
    
    this.tLayout = new YAHOO.widget.Layout
    ( 
      this.tParentElem,
      {
        height: 300,
        width: 600,
        units: this.tLayoutUnits
      }
    );
    
    this.tLayout.on('beforeResize', this._OnBeforeResize.bind(this));
    this.tLayout.render();
    
    this.AddWidgetMenu(_("Configure layout"), this.Configure.bind(this));
    
    this._tParent = tParent;
    
    return this.tLayout;
  },

  Destroy: function()
  {
    if (this.tDiv != null)
    {
      for (var nIndex in this.tDiv)
      {
        if(this.tDiv[nIndex].destroy != null)
        {
          this.tDiv[nIndex].destroy();
        }
      }
    }
    
    var tLayoutElem = this.tLayout.get('element');
    tLayoutElem.parentNode.removeChild(tLayoutElem);
  },
  
  CreateControls: function(tParent, tLayoutUnits)
  {
    this.tDiv = {};
    for(var nArrIndex = 0; nArrIndex < tLayoutUnits.length; ++nArrIndex)
    {
      var tProps = tLayoutUnits[nArrIndex];
      this.tDiv[tProps.body] = new webapp.ui.Div(tParent, { sId: tProps.body });
    }
  },
  
  _Refresh: function()
  {
    //  adding/changing units
    for(var nArrIndex = 0; nArrIndex < this.tLayoutUnits.length; ++nArrIndex)
    {
      var tProps = this.tLayoutUnits[nArrIndex];
      if (!this.tDiv[tProps.body])
      {
        //  adding new unit
        this.tDiv[tProps.body] = new webapp.ui.Div(this._tParent, { sId: tProps.body });
        this.tLayout.addUnit(tProps);
      }
      else
      {
        // change
        var tUnit = this.tLayout.getUnitByPosition(tProps.position);
        if (tUnit)
        {
          for(var tIndex in tProps)
          {
            if (typeof tProps[tIndex] != 'function')
            {
              if (tUnit.get(tIndex) != tProps[tIndex])
              {
                tUnit.set(tIndex, tProps[tIndex] || false);
              }
            }
          }
        }
      }
    }
    
    // deleting old
    for(var tIndex in this.tDiv)
    {
      var tDiv = this.tDiv[tIndex];
      if (tDiv.Element)
      {
        var bFound = false;
        for(var nArrIndex = 0; nArrIndex < this.tLayoutUnits.length; ++nArrIndex)
        {
          var tProps = this.tLayoutUnits[nArrIndex];
          if (tIndex == tProps.body)
          {
            bFound = true;
            break;
          }
        }

        if (!bFound)
        {
          var tUnit = this.tLayout.getUnitById(tIndex);
          this.tLayout.removeUnit(tUnit);
          tUnit.destroy();

          this.tDiv[tIndex].destroy();
          delete this.tDiv[tIndex];
        }
      }
    }
    
    this.tLayout.refresh();
  },
  
  _GetDefaultLayoutUnits: function()
  {
    var tDefaultLayoutUnits = 
    [
      { body: 'divright' + this.sName, position: 'right', width: 515, resize: true, gutter: '5px', scroll: true },
      { body: 'divcenter' + this.sName, position: 'center', gutter: '5px 0px 5px 5px' }
    ];
    return tDefaultLayoutUnits;
  },
  
  DeserializeData: function() // записать динамические свойства в контрол
  {
    if (this.tProperties.sLayout)
    {
      this.tLayoutUnits = this.DeserializeProps(this.tProperties.sLayout);
    }
  },
  
  SerializeData: function() // получить динамические свойства из контрола
  {
    if(this.tLayoutUnits)
    {
      this.tProperties.sLayout = this.SerializeProps(this.tLayoutUnits);
    }
  },
  
  DeserializeProps: function(sProps)
  {
    var aProps = [];
    var aPropsArr = sProps.split('|');
    
    for(var nArrIndex = 0; nArrIndex < aPropsArr.length - 1; ++nArrIndex)
    {
      var tProps = {};
      var aPropsKeys = aPropsArr[nArrIndex].split(';');
      for(var nIndex = 0; nIndex < aPropsKeys.length - 1; ++nIndex)
      {
        var aPair = aPropsKeys[nIndex].split(':');
        if (aPair[1].substr(0, 1) === "'")
        {
          tProps[aPair[0]] = aPair[1].substr(1, aPair[1].length - 2);
        }
        else
        {
          var tValue = aPair[1];
          if (tValue === 'true')
          {
            tProps[aPair[0]] = true;
          }
          else
          if (tValue === 'false')
          {
            tProps[aPair[0]] = false;
          }
          else
          {
            var nValue = parseInt(tValue)
            if (!isNaN(nValue))
            {
              tProps[aPair[0]] = nValue;
            }
            else
            {
              tProps[aPair[0]] = tValue;
            }
          }
        }
      }
      aProps[aProps.length] = tProps;
    }
    
    return aProps;
  },
  
  SerializeProps: function(aProps)
  {
    var sProps = "";
    
    for(var nArrIndex = 0; nArrIndex < aProps.length; ++nArrIndex)
    {
      var tProps = aProps[nArrIndex];
      for(nIndex in tProps)
      {
        var sType = typeof tProps[nIndex];
        if (sType == "string")
        {
          sProps += nIndex + ':\'' + tProps[nIndex] + '\';';
        }
        else
        if (sType == "number" || sType == "boolean")
        {
          sProps += nIndex + ':' + tProps[nIndex] + ';';
        }
      }
      sProps += "|";
    }
    
    return sProps;
  },
  
  GetFrameOptions: function()
  {
    var tOpts = 
    {
      bNoFrame: true
    };

    return tOpts;
  },
  
  GetUnits: function()
  {
    var tUnits = {};
    for(var tUnit in this.tLayoutUnits)
    {
      var sUnitId = this.tLayoutUnits[tUnit].body;
      if (sUnitId != null)
      {
        tUnits[sUnitId] = 
        {
          sName: _(this.tLayoutUnits[tUnit].position), 
          sId: sUnitId, 
          tBody: this.tDiv[sUnitId]
        };
      }
    }
    return tUnits;
  },
  
  GetLayout: function()
  {
    return this.tLayout;
  },
  
  _OnBeforeResize: function()
  {
    if (this.tParentElem.parentNode.parentNode === document.body)
    {
      this.tLayout.set('width', YAHOO.util.Dom.getClientWidth());
      this.tLayout.set('height', YAHOO.util.Dom.getClientHeight() - 26);
    }
  },
  
  Configure: function()
  {
    this.tConfigureDlg = new YAHOO.widget.SimpleDialog
    (
      'LayoutConfigureDlg',
      {
        fixedcenter: true,
        visible: false,
        modal: true,
        width: '500px',
        constraintoviewport: true,
        buttons:
        [
          { text: _('OK'), handler: this._OnConfirmConfigure.bind(this), isDefault: true },
          { text: _('Cancel'), handler: this._OnCancelConfigure.bind(this) }
        ]
      }
    );
    this.tConfigureDlg.setHeader(_('Configure layout'));
    this.tConfigureDlg.render(document.body);
    
    //  init dlg
    var tBody = this.tConfigureDlg.body;
    
    // main markup table
    var tTableUnitProps = new webapp.ui.Table(tBody);
    var tRowBody = tTableUnitProps.AddRow();
    var tCellUnits = tTableUnitProps.AddCell(tRowBody);
    var tCellProps = tTableUnitProps.AddCell(tRowBody);

    // units markup
    var tDivUnits = new webapp.ui.Div(tCellUnits);
    var tLabelUnit = new webapp.ui.Label(tDivUnits, { sCaption: _('Click unit to edit')});
    var tTableUnits = new webapp.ui.Table(tDivUnits);
    // add units
    // top
    var tRowUnitTop = tTableUnits.AddRow();
    var tCellUnitTop = tTableUnits.AddCell(tRowUnitTop, { sClass: 'cellUnit', tContent: _('top') });
    tCellUnitTop.colSpan = 3;
    // left, center, right
    var tRowUnitsMid = tTableUnits.AddRow();
    var tCellUnitLeft = tTableUnits.AddCell(tRowUnitsMid, { sClass: 'cellUnit', tContent: _('left') });
    var tCellUnitCenter = tTableUnits.AddCell(tRowUnitsMid, { sClass: 'cellUnit', tContent: _('center') });
    var tCellUnitRight = tTableUnits.AddCell(tRowUnitsMid, { sClass: 'cellUnit', tContent: _('right') });
    // bottom
    var tRowUnitBottom = tTableUnits.AddRow();
    var tCellUnitBottom = tTableUnits.AddCell(tRowUnitBottom, { sClass: 'cellUnit', tContent: _('bottom') });
    tCellUnitBottom.colSpan = 3;
    
    tTableUnits.On('click', this._OnSelectUnit.bindAsEventListener(this));
    var tNoticeLabel = new webapp.ui.Label(tDivUnits, 
      { sCaption: _('After changing layout it\'s recommended to save widgets and reload page'), sClass: 'noticeLabel'});

    // properties markup
    var tDivProps = new webapp.ui.Div(tCellProps);
    var tLabelProps = new webapp.ui.Label(tDivProps, { sCaption: _('Properties')});
    var tTableProps = new webapp.ui.Table(tDivProps);
    
    this.tCells = 
    {
      top: tCellUnitTop,
      left: tCellUnitLeft,
      center: tCellUnitCenter,
      right: tCellUnitRight,
      bottom: tCellUnitBottom
    };

    this.tCtrl = {};
    this.tCtrl.enabled = new webapp.ui.LabeledCheckbox(tTableProps.AddRowCell(), { sCaption: _('Enabled'), sCheckId: 'chkLayoutCfg_enabled' });
    this.tCtrl.header = new webapp.ui.LabeledCheckbox(tTableProps.AddRowCell(), { sCaption: _('Header'), sCheckId: 'chkLayoutCfg_header', sClass: 'checkIdent' });
    this.tCtrl.valheader = new webapp.ui.Edit(tTableProps.AddRowCell(), { sClass: 'editLayoutPropLong' });
    this.tCtrl.gutter = new webapp.ui.LabeledCheckbox(tTableProps.AddRowCell(), { sCaption: _('Gutter'), sCheckId: 'chkLayoutCfg_gutter', sClass: 'checkIdent' });
    this.tCtrl.valgutter = new webapp.ui.Edit(tTableProps.AddRowCell(), { sClass: 'editLayoutProp' });
    this.tCtrl.width = new webapp.ui.LabeledCheckbox(tTableProps.AddRowCell(), { sCaption: _('Width'), sCheckId: 'chkLayoutCfg_width', sClass: 'checkIdent' });
    this.tCtrl.valwidth = new webapp.ui.Edit(tTableProps.AddRowCell(), { sClass: 'editLayoutProp' });
    this.tCtrl.height = new webapp.ui.LabeledCheckbox(tTableProps.AddRowCell(), { sCaption: _('Height'), sCheckId: 'chkLayoutCfg_height', sClass: 'checkIdent' });
    this.tCtrl.valheight = new webapp.ui.Edit(tTableProps.AddRowCell(), { sClass: 'editLayoutProp' });
    this.tCtrl.resize = new webapp.ui.LabeledCheckbox(tTableProps.AddRowCell(), { sCaption: _('Resize'), sCheckId: 'chkLayoutCfg_resize', sClass: 'checkIdent' });
    this.tCtrl.scroll = new webapp.ui.LabeledCheckbox(tTableProps.AddRowCell(), { sCaption: _('Scroll'), sCheckId: 'chkLayoutCfg_scroll', sClass: 'checkIdent' });

    this.tCtrl.enabled.On('click', this._OnConfigEnabled, this);
    this.tCtrl.header.On('click', this._OnConfigCheck, this);
    this.tCtrl.gutter.On('click', this._OnConfigCheck, this);
    this.tCtrl.width.On('click', this._OnConfigCheck, this);
    this.tCtrl.height.On('click', this._OnConfigCheck, this);
    
    this._tLayoutUnitsCfg = this.tLayoutUnits.clone();

    this.tConfigureDlg.show();
    this._OnSelectUnit({target: tCellUnitTop});
    tCellUnitTop.focus();

    addHandler(tBody, 'keydown', this._OnKeyDown.bindAsEventListener(this));
  },
  
  _OnCancelConfigure: function()
  {
    this.tConfigureDlg.hide();
    this.tConfigureDlg.destroy();
    delete this.tConfigureDlg;
    delete this.sUnit;
    delete this.tUnit;
    delete this.tCtrl;
    delete this._tLayoutUnitsCfg;
  },
  
  _OnConfirmConfigure: function()
  {
    this._SaveUnit(this.sUnit);
    this.tLayoutUnits = this._tLayoutUnitsCfg;
    this._Refresh();
    this._OnCancelConfigure();
    this.SetModify();
  },
  
  _OnSelectUnit: function(tEvent)
  {
    var tTarget = tEvent.target || tEvent.srcElement;
    if(tTarget && tTarget.tagName == "TD") // checking for td
    {
      // unselect old unit
      if(this.tUnit)
      {
        this.tUnit.className = 'cellUnit';
        this._SaveUnit(this.sUnit);
      }
      
      // save and select new unit
      this.tUnit = tTarget;
      this.tUnit.className = 'cellUnitSel';
      
      this.sUnit = '';

      // searching unit name by element
      for(var tIndex in this.tCells)
      {
        if(this.tCells[tIndex] == this.tUnit)
        {
          this.sUnit = tIndex;
          break;
        }
      }
      
      this._LoadUnit(this.sUnit);
    }
  },
  
  _OnConfigEnabled: function(tEvent)
  {
    var bEnable = this.tCtrl.enabled.GetChecked();
    for(var tIndex in this.tCtrl)
    {
      var tCtrl = this.tCtrl[tIndex];
      if(tCtrl.Element && tCtrl != this.tCtrl.enabled)
      {
        if(tIndex.substr(0, 3) == 'val')
        {
          tCtrl.Enable(bEnable && this.tCtrl[tIndex.substr(3)].GetChecked());
        }
        else
        {
          tCtrl.Enable(bEnable);
        }
      }
    }
  },
  
  _OnConfigCheck: function(tEvent)
  {
    var tTarget = tEvent.target || tEvent.srcElement;
    
    var nPos = tTarget.id.indexOf('_');
    if (nPos > 0)
    {
      var sCtrl = tTarget.id.substr(nPos + 1);
      var tCtrl = this.tCtrl[sCtrl];
      var tCtrlVal = this.tCtrl['val' + sCtrl];
      if (tCtrl && tCtrlVal)
      {
        tCtrlVal.Enable(tCtrl.GetChecked());
        if (tCtrlVal.Enabled())
        {
          tCtrlVal.Focus();
        }
      }
    }
  },
  
  _SaveUnit: function(sUnit)
  {
    var tUnitIndex = this._GetUnitIndexByName(sUnit);
    
    var bEnable = this.tCtrl.enabled.GetChecked();
    if (bEnable)
    {
      if (tUnitIndex == null)
      {
        tUnitIndex = this._tLayoutUnitsCfg.length;
      }

      if(!this._tLayoutUnitsCfg[tUnitIndex])
      {
        this._tLayoutUnitsCfg[tUnitIndex] = { body: 'div' + this.sUnit + this.sName, position: this.sUnit };
      }
      
      var tUnitProps = this._tLayoutUnitsCfg[tUnitIndex];
      
      for(var tIndex in this.tCtrl)
      {
        var tCtrl = this.tCtrl[tIndex];
        if(tCtrl.Element && tCtrl != this.tCtrl.enabled)
        {
          if(tIndex.substr(0, 3) != 'val')
          {
            var tCtrlVal = this.tCtrl['val' + tIndex];
            if (tCtrlVal)
            {
              if (tCtrlVal.Enabled())
              {
                tUnitProps[tIndex] = tCtrlVal.GetValue();
              }
              else
              {
                tUnitProps[tIndex] = false;
              }
            }
            else
            {
              tUnitProps[tIndex] = tCtrl.GetValue();
            }
          }
        }
      }
    }
    else
    {
      if(tUnitIndex && this._tLayoutUnitsCfg[tUnitIndex])
      {
        this._tLayoutUnitsCfg.splice(tUnitIndex, 1);
      }
    }
  },
  
  _LoadUnit: function(sUnit)
  {
    var tUnitIndex = this._GetUnitIndexByName(sUnit);
    var tUnit = tUnitIndex ? this._tLayoutUnitsCfg[tUnitIndex] : null;

    // resetting all properties controls
    for(var tIndex in this.tCtrl)
    {
      var tCtrl = this.tCtrl[tIndex];
      if(tCtrl && tCtrl.Element)
      {
        if(tCtrl != this.tCtrl.enabled)
        {
          if(tIndex.substr(0, 3) == 'val')
          {
            tCtrl.SetValue('');
            tCtrl.Disable();
          }
          else
          {
            tCtrl.SetValue(false);
            tCtrl.Enable(tUnit != null);
          }
        }
      }
    }

    this.tCtrl.enabled.SetValue(tUnit != null);

    // if unit already exists
    if(tUnit)
    {
      // reading unit's properties
      for(var tIndex in tUnit)
      {
        var tProp = tUnit[tIndex];
        if(typeof tProp != 'function')
        {
          var tCtrl = this.tCtrl[tIndex];
          if(tCtrl) // skipping body, position, etc..
          {
            var tCtrlVal = this.tCtrl['val' + tIndex];
            if(tCtrlVal && tProp !== false)
            {
              tCtrlVal.SetValue(tProp);
              tCtrl.SetValue(true);
              tCtrlVal.Enable(true);
            }
            else
            {
              tCtrl.SetValue(tProp);
            }
          }
        }
      }
    }
  },
  
  _GetUnitIndexByName: function(sUnit)
  {
    for(var tIndex in this._tLayoutUnitsCfg)
    {
      if(this._tLayoutUnitsCfg[tIndex].position == sUnit)
      {
        return tIndex;
      }
    }
  },

  _OnKeyDown: function(tEvent)
  {
    switch (tEvent.keyCode)
    {
      case 13: // Enter
      {
        this._OnConfirmConfigure();
        break;
      }
      
      case 27: // Escape
      {
        this._OnCancelConfigure();
        break;
      }
    }
  }
  
});
