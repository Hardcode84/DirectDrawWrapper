
#ifndef DDRAW_EXPORT_H
#define DDRAW_EXPORT_H

#ifdef DDRAW_STATIC_DEFINE
#  define DDRAW_EXPORT
#  define DDRAW_NO_EXPORT
#else
#  ifndef DDRAW_EXPORT
#    ifdef ddraw_EXPORTS
        /* We are building this library */
#      define DDRAW_EXPORT __declspec(dllexport)
#    else
        /* We are using this library */
#      define DDRAW_EXPORT __declspec(dllimport)
#    endif
#  endif

#  ifndef DDRAW_NO_EXPORT
#    define DDRAW_NO_EXPORT 
#  endif
#endif

#ifndef DDRAW_DEPRECATED
#  define DDRAW_DEPRECATED __attribute__ ((__deprecated__))
#  define DDRAW_DEPRECATED_EXPORT DDRAW_EXPORT __attribute__ ((__deprecated__))
#  define DDRAW_DEPRECATED_NO_EXPORT DDRAW_NO_EXPORT __attribute__ ((__deprecated__))
#endif

#define DEFINE_NO_DEPRECATED 0
#if DEFINE_NO_DEPRECATED
# define DDRAW_NO_DEPRECATED
#endif

#endif
