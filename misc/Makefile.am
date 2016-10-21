SUFFIXES = .desktop.in.in .desktop.in .desktop

gsettings_SCHEMAS = io.github.koko-ng.geoclue-agent.gschema.xml

desktopdir = /etc/xdg/autostart
desktop_DATA = io.github.koko-ng.geoclue-agent.desktop

.desktop.in.in.desktop.in:
	@sed -e "s|@libexecdir[@]|${libexecdir}|" \
	     -e "s|@VERSION[@]|${VERSION}|" $< > $@

.desktop.in.desktop:
	$(AM_V_GEN) $(MSGFMT) --desktop --template $< -d $(top_srcdir)/po -o $@

@GSETTINGS_RULES@

CLEANFILES = $(desktop_DATA) $(desktop_in_files)

dist_noinst_DATA = \
	$(srcdir)/io.github.koko-ng.geoclue-agent.desktop.in.in \
	$(gsettings_SCHEMAS)



