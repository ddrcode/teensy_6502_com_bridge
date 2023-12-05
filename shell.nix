{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  packages = with pkgs; [
    arduino-cli
    arduino-language-server
    treefmt
    astyle
    python311Packages.mdit-py-plugins
    python311Packages.mdformat
    python311Packages.mdformat-gfm
  ];
}
