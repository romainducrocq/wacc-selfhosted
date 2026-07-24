### NQCC2 build instructions for bootstrapping

1. Install opam, the OCaml package manager (see other options in the link).
```sh
# https://opam.ocaml.org/doc/Install.html
bash -c "sh <(curl -fsSL https://opam.ocaml.org/install.sh)"
opam init
```

2. Install the OCaml compiler version 4.14.0 required for batteries.
```sh
# https://ocaml.org/docs/install-a-specific-ocaml-compiler-version
opam switch create 4.14.0 ocaml-base-compiler.4.14.0
opam switch set 4.14.0
eval $(opam env)
```

3. Build the NQCC2 compiler on branch extra-credit in this repo's submodule.
```sh
# https://github.com/nlsandler/nqcc2/tree/extra-credit#building-the-compiler
cd nqcc2
opam install . --deps-only
dune build
```
