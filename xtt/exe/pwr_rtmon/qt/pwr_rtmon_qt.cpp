/*
 * Proview   Open Source Process Control.
 * Copyright (C) 2005-2017 SSAB EMEA AB.
 *
 * This file is part of Proview.
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
 * along with Proview. If not, see <http://www.gnu.org/licenses/>
 *
 * Linking Proview statically or dynamically with other modules is
 * making a combined work based on Proview. Thus, the terms and
 * conditions of the GNU General Public License cover the whole
 * combination.
 *
 * In addition, as a special exception, the copyright holders of
 * Proview give you permission to, from the build function in the
 * Proview Configurator, combine Proview with modules generated by the
 * Proview PLC Editor to a PLC program, regardless of the license
 * terms of these modules. You may copy and distribute the resulting
 * combined work under the terms of your choice, provided that every
 * copy of the combined work is accompanied by a complete copy of
 * the source code of Proview (the version used to produce the
 * combined work), being distributed under the terms of the GNU
 * General Public License plus this exception.
 */

/* pwr_rtmonitor_qt.cpp -- Runtime Monitor */

#include <stdlib.h>
#include <locale.h>

#include "cow_qt_helpers.h"
#include "cow_rtmon_qt.h"
#include "cow_xhelp_qt.h"

#include <QApplication>

static void usage()
{
  printf("\nUsage: pwr_rtmonitor\n");
}

static void rtmon_help_cb(void* ctx, const char* topic)
{
  CoXHelp::dhelp(
      "index", "", navh_eHelpFile_Other, "$pwr_lang/rtmon_help.dat", 0);
}

static void rtmon_close_cb(void* ctx)
{
  debug_print("Shutting down...\n"); exit(0);
}

int main(int argc, char* argv[])
{
  int sts;
  char display[80] = "";

  debug_print("pwr_rtmon_qt: started with arguments:\n");

  if (argc > 1) {
    for (int i = 1; i < argc; i++) {
      debug_print("arg%d: %s\n", i, argv[i]);
      if (strcmp(argv[i], "-h") == 0) {
        usage();
        debug_print("Shutting down...\n"); exit(0);
      } else if (strcmp(argv[i], "--display") == 0 && i + 1 < argc) {
        strncpy(display, argv[i + 1], sizeof(display));
      }
    }
  }

  QApplication app(argc, argv);

  setlocale(LC_NUMERIC, "POSIX");
  setlocale(LC_TIME, "en_US");

  // Create help window
  CoXHelp* xhelp = new CoXHelpQt(0, 0, xhelp_eUtility_Xtt, &sts);
  CoXHelp::set_default(xhelp);

  RtMon* rtmon = new RtMonQt(NULL, NULL, "Runtime Monitor", display, &sts);
  rtmon->help_cb = rtmon_help_cb;
  rtmon->close_cb = rtmon_close_cb;

  return app.exec();
}