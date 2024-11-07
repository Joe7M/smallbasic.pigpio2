.SECONDARY :
.PHONY : all clean

pandoc = /usr/bin/pandoc
mkdir = @mkdir -p $(dir $@)
out = _out
pandoc_options=--from markdown-tex_math_dollars --to html5 --syntax-definition=smallbasic_syntax.xml --standalone --css style.css --highlight-style breezedark
pages = $(patsubst pages/%.md, $(out)/pages/%.html, $(wildcard pages/*.md))

# rules

$(out)/pages/%.html: pages/%.md
	$(mkdir)
	$(pandoc) $(pandoc_options) -o $@ $<

all: $(pages) style.css
	@mkdir -p ../pages
	@cp $(out)/pages/*.html ../pages
	@cp style.css ../pages

clean:
	rm -rf $(out)
