# scom

`scom` is a small, fast, and minimalistic command-line tool to open and interact with a serial port on Linux. It allows easy bidirectional communication with serial devices like `/dev/ttyUSB0`, `/dev/ttyS1`, or `/dev/ttyACM0` — without any bloated interface.

## 🚀 Features

- Simple serial port connection
- Customizable baudrate
- Full terminal-style input/output
- Raw mode (no line buffering or processing)
- Great for testing embedded boards, modems, or serial interfaces

## 📦 Installation

### From source

```bash
git clone https://github.com/crash-systems/scom.git
cd scom
make
make install
scom
```

### With Nix

scom is now available on the nixpkgs public repository, to use it from nixpkgs run:
```bash
nix run nixpkgs#scom
```
or via nix-shell:
```bash
nix-shell -p scom
```

## 🛠️ Usage

```bash
scom -s /dev/ttyUSB0 -b 9600
```

## 📄 License

GNU GPLv3 License – see the LICENSE file.

## 🤝 Contributing

Contributions are welcome! Feel free to open issues or submit pull requests.

## ✨ Author

[@savalet](https://github.com/savalet) for [@crash-systems](https://github.com/crash-systems)
