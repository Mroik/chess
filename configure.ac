AC_INIT([chess],[0.1],[mroik@delayed.space])
AC_CONFIG_SRCDIR(main.c)

dnl Check for SDL
SDL_VERSION=2.26.2
AM_PATH_SDL2($SDL_VERSION,
          :,
          AC_MSG_ERROR([*** SDL version $SDL_VERSION not found!])
)
CFLAGS="$CFLAGS $SDL_CFLAGS -lSDL2_image"
LIBS="$LIBS $SDL_LIBS"

AM_INIT_AUTOMAKE
AC_PROG_CC
AC_CONFIG_FILES([Makefile])
AC_OUTPUT
