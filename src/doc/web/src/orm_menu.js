// You can find instructions for this file here:
// http://www.treeview.net

// Decide if the names are links or just the icons
USETEXTLINKS = 1  //replace 0 with 1 for hyperlinks

// Decide if the tree is to start all open or just showing the root folders
STARTALLOPEN = 0 //replace 0 with 1 to show the whole tree

ICONPATH = '../../' //change if the gif's folder is a subfolder, for example: 'images/'

HIGHLIGHT = 1
HIGHLIGHT_COLOR = '#FF9900'
HIGHLIGHT_BG = '#000066'


foldersTree = gFld("<b>Object Reference Manual</b>", "orm_intro.html")

  aux1 = insFld(foldersTree, gFld("<b>pwrs</b>", "pwrs_index.html"))
  pwrs(aux1)

  aux1 = insFld(foldersTree, gFld("<b>pwrb</b>", "pwrb_index.html"))
  pwrb(aux1)

  aux1 = insFld(foldersTree, gFld("<b>BaseComponent</b>", "basecomponent_index.html"))
  BaseComponent(aux1)

  aux1 = insFld(foldersTree, gFld("<b>NMps</b>", "nmps_index.html"))
  NMps(aux1)

  aux1 = insFld(foldersTree, gFld("<b>Remote</b>", "remote_index.html"))
  Remote(aux1)

  aux1 = insFld(foldersTree, gFld("<b>TLog</b>", "tlog_index.html"))
  TLog(aux1)

  aux1 = insFld(foldersTree, gFld("<b>OtherManufacturer</b>", "othermanufacturer_index.html"))
  OtherManufacturer(aux1)

  aux1 = insFld(foldersTree, gFld("<b>ABB</b>", "abb_index.html"))
  ABB(aux1)

  aux1 = insFld(foldersTree, gFld("<b>Siemens</b>", "siemens_index.html"))
  Siemens(aux1)

  aux1 = insFld(foldersTree, gFld("<b>Telemecanique</b>", "telemecanique_index.html"))
  Telemecanique(aux1)

  aux1 = insFld(foldersTree, gFld("<b>SsabOx</b>", "ssabox_index.html"))
  SsabOx(aux1)

  aux1 = insFld(foldersTree, gFld("<b>Inor</b>", "inor_index.html"))
  Inor(aux1)

  aux1 = insFld(foldersTree, gFld("<b>KlocknerMoeller</b>", "klocknermoeller_index.html"))
  KlocknerMoeller(aux1)








