{ pkgs ? import <nixpkgs> { } }:

pkgs.mkShell {
  packages = with pkgs; [
    acme
    arduino-cli
    arduino-language-server
    cpplint
    treefmt
    astyle
    python311Packages.mdit-py-plugins
    python311Packages.mdformat
    python311Packages.mdformat-gfm
  ];
}
