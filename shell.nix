with import <nixpkgs> {};
mkShell {
  nativeBuildInputs = [
    bashInteractive
    popt
    pkg-config
    meson
    ninja
    gnome2.glib
    gnome2.libIDL
    qt4
  ];
}
