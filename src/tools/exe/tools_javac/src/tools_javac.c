/* 
 * Proview   $Id: tools_javac.c,v 1.2 2005-09-01 14:58:00 claes Exp $
 * Copyright (C) 2005 SSAB Oxelösund AB.
 *
 * This program is free software; you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as 
 * published by the Free Software Foundation, either version 2 of 
 * the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License 
 * along with the program, if not, write to the Free Software 
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

char prefix[][10] = {"gdhr", "gdh", "cdhr", "cdh", "pwrb", "pwrt", "pwr", 
	"rt", "co", "jop", "ge", "qcomr", "qcom", "cli"};

int main( int argc, char *argv[])
{
  char filename[80];
  char destination[80];
  char cmd[200];
  char *s, *t;
  int i;
  int sts;

  if ( argc > 3 && strcmp( argv[1], "-d") == 0)
  {
    strcpy( destination, argv[2]);

    /* Cut last to segments in directory (package name) */
    if ((s = strrchr( destination, '/')))
    {
      *s = 0;
      if ((s = strrchr( destination, '/')))
      {
        *s = 0;
        if ((s = strrchr( destination, '/')))
          *s = 0;
      }
    }
    strcpy( filename, argv[3]);
  }
  else if ( argc > 1)
  {
    strcpy( filename, argv[1]);
    strcpy( destination, "");
  }
  else
  {
    printf("Usage:\n");
    printf("	-d	target directory\n");
    printf("	arg	filename\n");
    exit(0);
  }

  if (( s = strrchr( filename, '/')))
    s++;
  else
    s = filename;
  /* Special cases */
  if ( strcmp( s, "cdhrclassid.java") == 0)
    strcpy( s, "CdhrClassId.java");
  else if ( strcmp( s, "gdhrrefobjectinfo.java") == 0)
    strcpy( s, "GdhrRefObjectInfo.java");
  else if ( strcmp( s, "pwrtrefid.java") == 0)
    strcpy( s, "PwrtRefId.java");
  else if ( strcmp( s, "qcomrcreateq.java") == 0)
    strcpy( s, "QcomrCreateQ.java");
  else if ( strcmp( s, "gecformat.java") == 0)
    strcpy( s, "GeCFormat.java");
  else if ( strcmp( s, "jopspiderframe.java") == 0)
    strcpy( s, "JopSpiderFrame.java");
  else if ( strcmp( s, "gecoloreditor.java") == 0)
    strcpy( s, "GeColorEditor.java");
  else if ( strcmp( s, "gecolorbrightnesseditor.java") == 0)
    strcpy( s, "GeColorBrightnessEditor.java");
  else if ( strcmp( s, "gecolortoneeditor.java") == 0)
    strcpy( s, "GeColorToneEditor.java");
  else if ( strcmp( s, "gecolorintensityeditor.java") == 0)
    strcpy( s, "GeColorIntensityEditor.java");
  else if ( strcmp( s, "gecolorshifteditor.java") == 0)
    strcpy( s, "GeColorShiftEditor.java");
  else if ( strcmp( s, "clickactioneditor.java") == 0)
    strcpy( s, "ClickActionEditor.java");
  else if ( strcmp( s, "jopconfirmdialog.java") == 0)
    strcpy( s, "JopConfirmDialog.java");
  else if ( strcmp( s, "getextfield.java") == 0)
    strcpy( s, "GeTextField.java");
  else if ( strcmp( s, "joptextfield.java") == 0)
    strcpy( s, "JopTextField.java");
  else if ( strcmp( s, "joptextfieldbeaninfo.java") == 0)
    strcpy( s, "JopTextFieldBeanInfo.java");
  else if ( strcmp( s, "joploginapplet.java") == 0)
    strcpy( s, "JopLoginApplet.java");
  else if ( strcmp( s, "joploginframe.java") == 0)
    strcpy( s, "JopLoginFrame.java");
  else
  {
    for ( i = 0; i < sizeof(prefix)/sizeof(prefix[0]); i++)
    {
      if ( strncmp( s, prefix[i], strlen(prefix[i])) == 0)
      {
        *s = _toupper(*s);
        if ( strlen(s) > strlen(prefix[i]))
          *(s+strlen(prefix[i])) = _toupper(*(s+strlen(prefix[i])));
        break;
      }
    }
  }
  if (( s = strrchr( filename, '/')))
    s++;
  else
    s = filename;
  if ( (t = strstr( s, "beaninfo.java")))
    strcpy( t, "BeanInfo.java");


  if ( strcmp( destination, "") == 0)
    sprintf( cmd, "javac %s", filename);
  else
    sprintf( cmd, "javac -d %s %s", destination, filename);

  printf( "Cmd: %s\n", cmd);
  sts = system( cmd);
  exit(sts);
}

