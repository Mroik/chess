AUTOMAKE_OPTIONS = foreign subdir-objects
EXTRA_DIST = assets
bin_PROGRAMS = chess
chess_SOURCES = draw.c game.c input.c utils.c main.c
dist_data_DATA = assets
generate-assets:
	@cp -r $(top_srcdir)/assets .
