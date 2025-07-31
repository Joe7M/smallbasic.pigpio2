.SECONDARY :
.PHONY : all clean

pandoc = /usr/bin/pandoc
mkdir = @mkdir -p $(dir $@)
out = _out
pandoc_options=--from markdown-tex_math_dollars --to html5 --syntax-definition=smallbasic_syntax.xml --standalone --css style.css --highlight-style  breezedark --template template.html
pages = $(patsubst pages/%.md, $(out)/pages/%.html, $(wildcard pages/*.md))

# rules

$(out)/pages/%.html: pages/%.md
	$(mkdir)
	$(pandoc) $(pandoc_options) -o $@ $<

all: $(pages) style.css
	@cp $(out)/pages/*.html ../
	@cp -r pages/images ../
	@cp -r pages/files ../
	@cp style.css ../

clean:
	rm -rf $(out)
