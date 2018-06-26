/* ISC license. */

#include <errno.h>
#include <skalibs/stralloc.h>
#include <skalibs/genalloc.h>
#include <nsss/config.h>
#include <nsss/grp-unix.h>
#include <nsss/grp-all.h>
#include <nsss/nsss-switch.h>
#include "nsss-internal.h"

int nsss_all_getgrnam_r (char const *name, struct group *gr, char *buf, size_t buflen, struct group **grp)
{
  struct group gr2 ;
  stralloc sa = STRALLOC_ZERO ;
  genalloc ga = GENALLOC_ZERO ;
  nsss_switch_t a = NSSS_SWITCH_ZERO ;
  int e = errno ;
  if (!nsss_switch_start(&a, NSSS_SWITCH_GRP, NSSS_NSSSD_PATH, 0, 0)) goto fallback ;
  errno = 0 ;
  if (!nsss_switch_grp_getbyname(&a, &gr2, &sa, &ga, name, 0, 0))
  {
    nsss_switch_end(&a, NSSS_SWITCH_GRP) ;
    *grp = 0 ;
    return errno ? errno : (errno = e, 0) ;
  }
  nsss_switch_end(&a, NSSS_SWITCH_GRP) ;
  if (!nsss_grp_copy(gr, buf, buflen, &gr2, sa.s, sa.len, genalloc_s(char *, &ga), genalloc_len(char *, &ga)))
  {
    stralloc_free(&sa) ;
    *grp = 0 ;
    return errno ;
  }
  stralloc_free(&sa) ;
  *grp = gr ;
  return (errno = e, 0) ;

 fallback:
  errno = e ;
  return nsss_unix_getgrnam_r(name, gr, buf, buflen, grp) ;
}
