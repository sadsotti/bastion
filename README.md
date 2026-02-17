# 🛡️ Bastion++

> **Secure Steganography System**

Robust and lightweight steganography tool written in pure C++. It allows users to hide encrypted text files inside `.BMP` images using **LSB (Least Significant Bit)** manipulation combined with **XOR Encryption**.

The tool features a custom style CLI interface with animations little animations, progress bars, and secure password protection.

---

## ✨ Features

| Feature | Description |
|---|---|
| 🔒 **XOR Encryption** | Data is scrambled with a custom password before embedding. Without the key, the hidden data is just random noise. |
| 🖼️ **LSB Steganography** | Modifies only the least significant bits of the pixels. The visual difference is imperceptible to the human eye. |
| 💻 **CLI Interface** | Custom Green-on-Black terminal aesthetic, loading bars, and ASCII art headers. |
| ⚡ **Zero Dependencies** | Written in standard C++ with Windows API. No external libraries required. |
| 🚀 **Portable** | Runs as a standalone `.exe` file. |

---

## 🛠️ How to Compile

This project is optimized for **Windows** (uses `<windows.h>` for UI features).

### Option 1: Microsoft Visual Studio (Recommended)

1. Create a new **Console App** project in C++.
2. Copy the source code from `bastion.cpp`.
3. Set the build configuration to **Release (x64)**.
4. **Critical Step:** Go to `Properties` → `C/C++` → `Code Generation` → set `Runtime Library` to **Multi-threaded (/MT)**.
5. Build the solution with `Ctrl + Shift + B`.

---

## 📖 How to Use

### 1. Setup

- Ensure your image (`.bmp`) and secret text (`.txt`) are in the same folder as the executable.
- ⚠️ **Do NOT use spaces in filenames.**

| ❌ Incorrect | ✅ Correct |
|---|---|
| `my photo.bmp` | `my_photo.bmp` |

### 2. Run the Program

Launch `bastion.exe`. You will be greeted by the main menu:

```
  Select operation:
  [1] Encrypt & Hide (Lock)
  [2] Decrypt & Open (Unlock)
  [0] Exit
```

### 3. Encrypt (Hide a Secret)

1. Select **Option 1**.
2. Enter the source image filename (e.g., `boat.bmp`).
3. Enter the secret text filename (e.g., `secret.txt`).
4. Enter the output filename (e.g., `secure_boat.bmp`).
5. Set a **Password** — do not forget it!

### 4. Decrypt (Reveal a Secret)

1. Select **Option 2**.
2. Enter the image containing the secret.
3. Enter the filename to save the revealed text.
4. Enter the **Password**.

---

## ⚠️ Requirements & Limitations

| Item | Detail |
|---|---|
| **Image Format** | Only supports 24-bit `.BMP` images |
| **Capacity** | The image must be large enough to hold the text (~8 pixels per character) |
| **Operating System** | Windows 10 / 11 |

---

## 📄 License

This project is released under the [MIT License](https://opensource.org/licenses/MIT).

---
*© 2026 Lorenzo Sottile. Made by Me with ♡.*
