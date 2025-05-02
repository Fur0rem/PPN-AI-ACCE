# Fetch 24.05 channel
let 
  pkgs = import (fetchTarball("channel:nixpkgs-unstable")) {};
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
    linuxPackages_latest.perf
    vulkan-tools
    vulkan-loader
    vulkan-headers
    vulkan-tools-lunarg
    vulkan-validation-layers
    vulkan-extension-layer
    vulkan-memory-allocator
    vulkan-utility-libraries
    shaderc
  ];
  NIX_ENFORCE_NO_NATIVE=0;
}