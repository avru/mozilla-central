/*  */
/* ----- BEGIN LICENSE BLOCK -----
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public
 * License Version 1.1 (the "License"); you may not use this file
 * except in compliance with the License. You may obtain a copy of
 * the License at http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS
 * IS" basis, WITHOUT WARRANTY OF ANY KIND, either express or
 * implied. See the License for the specific language governing
 * rights and limitations under the License.
 *
 * The Original Code is .
 *
 * The Initial Developer of the Original Code is Netscape Communications Corporation.
 * Portions created by Netscape Communications Corporation are
 * Copyright (C) 1998-1999 Netscape Communications Corporation. All
 * Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either of the GNU General Public License Version 2 or later (the "GPL"),
 * or the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the LGPL or the GPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ----- END LICENSE BLOCK ----- */


//Cancel() is in EdDialogCommon.js
var insertNew = true;
var tagname = "TAG NAME"

// dialog initialization code
function Startup()
{
  if (!InitEditorShell())
    return;

  doSetOKCancel(onOK, onCancel);

  // Create dialog object to store controls for easy access
  dialog = new Object;
  // GET EACH CONTROL -- E.G.:
  //dialog.editBox = document.getElementById("editBox");

  initDialog();
  
  // SET FOCUS TO FIRST CONTROL
  //SetTextboxFocus(dialog.editBox);
  SetWindowLocation();
}

function InitDialog() {
  // Get a single selected element of the desired type
  element = editorShell.GetSelectedElement(tagName);

  if (element) {
    // We found an element and don't need to insert one
    insertNew = false;
    dump("Found existing image\n");
  } else {
    insertNew = true;
    // We don't have an element selected, 
    //  so create one with default attributes
    dump("Element not selected - calling createElementWithDefaults\n");
    element = editorShell.createElementWithDefaults(tagName);
  }

  if(!element)
  {
    dump("Failed to get selected element or create a new one!\n");
    window.close();
  }
}

function onOK()
{
// Set attribute example:
//  imageElement.setAttribute("src",dialog.srcInput.value);
  if (insertNew) {
    try {
      editorShell.InsertElementAtSelection(element, false);
    } catch (e) {
      dump("Exception occured in InsertElementAtSelection\n");
    }
  }
  SaveWindowLocation();
  return true; // do close the window
}
