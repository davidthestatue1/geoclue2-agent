bin_PROGRAMS=geoclue-agent-service
bindir = $(libexecdir)

geoclue_agent_service_SOURCES=app.c app.h \
appwin.c appwin.h \
service.c service.h \
servicewin.c servicewin.h \
../lib/geoclue-agent-interface.c ../lib/geoclue-agent-interface.h

geoclue_agent_service_CPPFLAGS = @GTK_CFLAGS@ -g -O3
DEFS += -DLOCALEDIR=\"$(localedir)\"
geoclue_agent_service_LDADD = @GTK_LIBS@

