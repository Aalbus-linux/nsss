/* ISC license. */

#include <nsss/config.h>
#include <nsss/grp-switch.h>
#include <nsss/nsss-switch.h>
#include "nsss-switch-internal.h"

void nsss_switch_setgrent (void)
{
  if (!nsss_switch_start(&nsss_switch_here, NSSS_SWITCH_GRP, NSSS_NSSSD_PATH, 0, 0)) return ;
  nsss_switch_grp_rewind(&nsss_switch_here, 0, 0) ;
}
