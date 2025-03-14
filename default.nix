{ pkgs ? import <nixpkgs> {} }:

let pythonEnv = pkgs.python3.withPackages (ps: with ps; [
  matplotlib
]);
in 
pkgs.stdenv.mkDerivation {
  name = "my-env";
  nativeBuildInputs = with pkgs; [
    pythonEnv
    clang-tools
    gcc
    coreutils-prefixed
    re2
    gtest
    doxygen
    nasm
    cmake
    eigen
    texliveFull
    openssl
  ];
}