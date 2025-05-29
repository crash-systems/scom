{
  inputs.nixpkgs.url = "github:NixOS/nixpkgs/nixpkgs-unstable";

  outputs = {
    self,
    nixpkgs,
  }: let
    forAllSystems = function:
      nixpkgs.lib.genAttrs [
        "x86_64-linux"
        "aarch64-linux"
      ] (system: function nixpkgs.legacyPackages.${system});
  in {
    devShells = forAllSystems (pkgs: {
      default = pkgs.mkShell {
        hardeningDisable = ["fortify"];
        packages = with pkgs; [
          gcc
          valgrind
          compiledb
          gcovr
        ];
      };
    });

    formatter = forAllSystems (pkgs: pkgs.alejandra);

    packages = forAllSystems (pkgs: {
      default = self.packages.${pkgs.system}._42sh;
      _scom = pkgs.callPackage ./nix/package.nix { };
    });
  };
}
