/* ws_diag_control.h
 * Turn compiler diagnostic messages on and off.
 *
 * From FreeRADIUS build.h.
 *
 * @copyright 2013 The FreeRADIUS server project
 *
 * That project is covered by the GPLv2, so:
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 */
#ifndef __WS_DIAG_CONTROL_H__
#define __WS_DIAG_CONTROL_H__

#ifdef __cplusplus
extern "C" {
#endif

#define XSTRINGIFY(x) #x

/*
 *	Macros for controlling warnings in GCC >= 4.2 and clang >= 2.8
 */
#define DIAG_JOINSTR(x,y) XSTRINGIFY(x ## y)
#define DIAG_DO_PRAGMA(x) _Pragma (#x)

/* check the gcc or clang version

   pragma GCC diagnostic error/warning/ignored -Wxxx was introduced
   in gcc 4.2.0
   pragma GCC diagnostic push/pop was introduced in gcc 4.6.0

   pragma clang diagnostic error/warning/ignored -Wxxx and
   pragma clang diagnostic push/pop were introduced in clang 2.8 */

#if defined(__GNUC__) && ((__GNUC__ * 100) + __GNUC_MINOR__) >= 402
   /* gcc version is >= 4.2.0; we can use "GCC diagnostic ignored/warning
      -Wxxx" */
#  define DIAG_PRAGMA(x) DIAG_DO_PRAGMA(GCC diagnostic x)
#  if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
     /* gcc version is >= 4.6.0: we can use "GCC diagnostic push/pop" */
#    define DIAG_OFF(x) DIAG_PRAGMA(push) DIAG_PRAGMA(ignored DIAG_JOINSTR(-W,x))
#    define DIAG_ON(x) DIAG_PRAGMA(pop)
#  else
     /* gcc version is between 4.2.0 and 4.6.0:
        we can use "GCC diagnostic ignored/warning -Wxxx", but not
        "GCC diagnostic push/pop" */
#    define DIAG_OFF(x) DIAG_PRAGMA(ignored DIAG_JOINSTR(-W,x))
#    define DIAG_ON(x)  DIAG_PRAGMA(warning DIAG_JOINSTR(-W,x))
#  endif
#elif defined(__clang__) && ((__clang_major__ * 100) + __clang_minor__ >= 208)
   /* clang version is >= 2.8: we can use "clang diagnostic ignored -Wxxx"
      and "clang diagnostic push/pop" */
#  define DIAG_PRAGMA(x) DIAG_DO_PRAGMA(clang diagnostic x)
#  define DIAG_OFF(x) DIAG_PRAGMA(push) DIAG_PRAGMA(ignored DIAG_JOINSTR(-W,x))
#  define DIAG_ON(x) DIAG_PRAGMA(pop)
#else
   /* no gcc or clang, or gcc version < 4.2.0, or clang version < 2.8:
      we can't do anything */
#  define DIAG_OFF(x)
#  define DIAG_ON(x)
#endif

/*
 *	For dealing with APIs which are only deprecated in OS X (like the
 *	OpenSSL and MIT/Heimdal Kerberos APIs).
 *
 *	Dear Apple: this is a cross-platform program, and we're not
 *	going to use your Shiny New Frameworks on OS X unless there's
 *	a sufficiently clear benefit to make it worth our while to have
 *	both OS X and non-OS X versions of the code.
 */
#ifdef __APPLE__
#  define USES_APPLE_DEPRECATED_API DIAG_OFF(deprecated-declarations)
#  define USES_APPLE_RST DIAG_ON(deprecated-declarations)
#else
#  define USES_APPLE_DEPRECATED_API
#  define USES_APPLE_RST
#endif

#ifdef __cplusplus
}
#endif
#endif /* __WS_DIAG_CONTROL_H__ */
