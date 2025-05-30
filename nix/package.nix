{
  lib,
  stdenv,
  withDevBinaries ? false,
}:
stdenv.mkDerivation {
  name = "scom";
  version = "dev";

  src = ./.;
  enableParallelBuilding = true;

  makeFlags = [ "PREFIX=${placeholder "out"}"]
    ++ (lib.optional withDevBinaries ["debug" "check"]);

  postInstall = lib.optional withDevBinaries ''
    install -Dm755 -t $out/bin debug
  '';

  meta = {
    description = "scom is a small, fast, and minimalistic command-line tool to open and interact with a serial port on Linux.";
    license = [ lib.licenses.gpl3 ];
    mainProgram = "scom";
    platforms = lib.platforms.unix;
    maintainers = with lib.maintainers; [ savalet ];
  };
}
