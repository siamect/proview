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

  aux1 = insFld(foldersTree, gFld("<b>nmps</b>", "nmps_index.html"))
  NMps(aux1)

  aux1 = insFld(foldersTree, gFld("<b>ssab</b>", "ssab_index.html"))
  SSAB(aux1)

  aux1 = insFld(foldersTree, gFld("<b>tlog</b>", "tlog_index.html"))
  TLog(aux1)








