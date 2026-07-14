```sh
# https://opam.ocaml.org/doc/Install.html
bash -c "sh <(curl -fsSL https://opam.ocaml.org/install.sh)"
opam init
```

```sh
# https://ocaml.org/docs/install-a-specific-ocaml-compiler-version
opam switch create 4.14.0 ocaml-base-compiler.4.14.0
opam switch set 4.14.0
eval $(opam env)
```

```sh
# https://github.com/nlsandler/nqcc2/tree/extra-credit
cd nqcc2
opam install . --deps-only
dune build
```
