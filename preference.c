/* File: preference.c
   Time-stamp: <2013-01-23 03:43:41 gawen>

   Copyright (C) 2013 David Hauweele <david@hauweele.net>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program. If not, see <http://www.gnu.org/licenses/>. */

#include <stdlib.h>
#include <stdbool.h>
#include <assert.h>
#include <err.h>

#include <confuse.h>

#include "path.h"

cfg_t *mainprefs;

void init_mainprefs(void)
{
  const char *conf = configuration_file("crastino.conf");

  cfg_opt_t opts[] = {
    CFG_BOOL("show_main", true, CFGF_NONE),
    CFG_BOOL("log", true, CFGF_NONE),
    CFG_BOOL("confirm_delete", true, CFGF_NONE),
    CFG_BOOL("confirm_done", false, CFGF_NONE),
    CFG_BOOL("confirm_showup", false, CFGF_NONE),
    CFG_BOOL("confirm_modify", false, CFGF_NONE),
    CFG_BOOL("always_showup", false, CFGF_NONE),
    CFG_BOOL("systray", true, CFGF_NONE),
    CFG_BOOL("swap_systray", false, CFGF_NONE),
    CFG_BOOL("exhibit_dirty_category", true, CFGF_NONE),
    CFG_BOOL("save_button", true, CFGF_NONE),
    CFG_INT("autosave", 3600, CFGF_NONE),
    CFG_STR("default_color", "gray", CFGF_NONE),
    CFG_STR("task_format", "<b>%t</b>", CFGF_NONE),
    CFG_STR("recurrent_task_format", "<b>%t</b> <i>(%D)</i>", CFGF_NONE),
    CFG_STR("systray_task_format", "%t", CFGF_NONE),
    /* TODO: add key_* options. */
    CFG_END()
  };

  mainprefs = cfg_init(opts, CFGF_NONE);
  if(cfg_parse(mainprefs, conf) == CFG_PARSE_ERROR)
    errx(EXIT_FAILURE, "cannot parse configuration file");

}

void free_mainprefs(void)
{
  assert(mainprefs != NULL);

  cfg_free(mainprefs);
}
