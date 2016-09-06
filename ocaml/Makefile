OCB_FLAGS = -use-ocamlfind -I src -I lib
OCB       = ocamlbuild $(OCB_FLAGS)

all: debug

clean:
	$(OCB) -clean

byte: sanity
	$(OCB) main.byte

debug: sanity
	$(OCB) main.d.byte

native: sanity
	$(OCB) main.native

sanity:
	# check that packages can be found
	ocamlfind query batteries ounit

test: native
	./main.native

.PHONY: all byte clean debug native sanity test
