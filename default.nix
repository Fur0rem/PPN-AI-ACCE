# Fetch 24.05 channel
let 
  pkgs = import (fetchTarball "channel:nixos-24.05") {};
  pythonEnv = pkgs.python3.withPackages (ps: with ps; [
    matplotlib
    pandas
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