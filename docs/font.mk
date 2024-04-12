TEX_GYRE_SCHOLA += res/font/texgyreschola-bold.otf
TEX_GYRE_SCHOLA += res/font/texgyreschola-bolditalic.otf
TEX_GYRE_SCHOLA += res/font/texgyreschola-italic.otf
TEX_GYRE_SCHOLA += res/font/texgyreschola-regular.otf
$(TEX_GYRE_SCHOLA) &:
	mkdir -p res/font
	curl -sLo- 'https://www.gust.org.pl/projects/e-foundry/tex-gyre/schola/qcs2.005otf.zip' > texgyreschola.zip
	unzip -oq texgyreschola.zip -d res/font
	rm texgyreschola.zip

JETBRAINS_MONO += res/font/JetBrainsMono-Bold.ttf
JETBRAINS_MONO += res/font/JetBrainsMono-BoldItalic.ttf
JETBRAINS_MONO += res/font/JetBrainsMono-Italic.ttf
JETBRAINS_MONO += res/font/JetBrainsMono-Regular.ttf
$(JETBRAINS_MONO) &:
	mkdir -p res/font
	curl -sLo- 'https://download-cdn.jetbrains.com/fonts/JetBrainsMono-2.304.zip' > jetbrainsmono.zip
	unzip -oqj jetbrainsmono.zip -x 'fonts/webfonts/*' 'fonts/variable/*' '*/JetBrainsMonoNL-*' 'AUTHORS.txt' 'OFL.txt' -d res/font
	rm jetbrainsmono.zip

INTER += res/font/Inter-Bold.otf
INTER += res/font/Inter-BoldItalic.otf
INTER += res/font/Inter-Italic.otf
INTER += res/font/Inter-Regular.otf
$(INTER) &:
	mkdir -p res/font
	curl -sLo- 'https://github.com/rsms/inter/releases/download/v4.0/Inter-4.0.zip' > inter.zip
	unzip -oqj inter.zip -x 'web/*' 'extras/ttf/*' '*.ttc' '*Variable*.ttf' '*/InterDisplay-*' 'LICENSE.txt' 'help.txt' -d res/font
	rm inter.zip

SANS_SERIF_FONTS := $(INTER)
SERIF_FONTS := $(TEX_GYRE_SCHOLA)
MONOSPACE_FONTS := $(JETBRAINS_MONO)

ALL_FONTS := $(SANS_SERIF_FONTS) $(SERIF_FONTS) $(MONOSPACE_FONTS)

