{ pkgs ? import <nixpkgs> {} }:

pkgs.clangStdenv.mkDerivation {
  name = "my-env";
  nativeBuildInputs = with pkgs; [
    clang-tools
    gcc
    coreutils-prefixed
    re2
    gtest
    doxygen
    nasm
    cmake
  ];
}