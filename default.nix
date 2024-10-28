let
	# Pinned nixpkgs, deterministic. Last updated: 2/12/21.
	# pkgs = import (fetchTarball("https://github.com/NixOS/nixpkgs/archive/a58a0b5098f0c2a389ee70eb69422a052982d990.tar.gz")) {};

	# Rolling updates, not deterministic.
	pkgs = import (fetchTarball("channel:nixpkgs-unstable")) {};
	cur_dir = builtins.toString ./.;

in pkgs.mkShell {
	buildInputs = with pkgs; [ gcc re2 coreutils-prefixed gtest doxygen ];
	NIX_CFLAGS_COMPILE="-I${cur_dir}/nanobench/src/include";
}
