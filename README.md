# Bongo Cat for Omarchy

A keyboard-reactive Bongo Cat and rolling WPM meter for the Omarchy Quickshell bar.

![Built for Omarchy: Plugin](https://raw.githubusercontent.com/tcballard/omarchy-badges/refs/heads/main/badges/v1/omarchy-plugin.svg) ![Supported Omarchy versions: 4.0.0+](https://raw.githubusercontent.com/tcballard/omarchy-badges/refs/heads/main/badges/v1/compatibility/omarchy-4.0.0-plus.svg)

## Features

- Tracks physical keyboard press and release events.
- Mirrors left- and right-side keyboard input to Bongo Cat’s paws; both paws stay down while keys on both sides are held.
- Shows a rolling 10-second WPM estimate from printable key presses, using the standard five characters per word.
- Left-click the bar item to cycle **Bongo Cat + WPM**, **Bongo Cat**, and **WPM**.
- Uses white outline art on dark themes and black outline art on light themes automatically.
- Uses Omarchy’s font and spacing tokens so the widget follows display text scaling and theme changes.

## Requirements

- Omarchy Quattro with Quickshell.
- A C compiler and `make` (`base-devel` on Arch) to build the local keyboard listener.
- Permission to read keyboard event devices. Grant it once:

  ```bash
  sudo usermod -aG input "$USER"
  ```

  Bongo Cat starts its listener with the new `input` group immediately; signing out is not needed. If Bongo Cat is already enabled, reload its listener:

  ```bash
  omarchy restart shell
  ```

The listener reads keyboard events locally only. It does not log, persist, or transmit keystrokes.

## Install

Clone the plugin, build its local listener, then enable it:

```bash
omarchy plugin add https://github.com/spicybackend/omarchy-bongo-cat.git --yes
cd ~/.config/omarchy/plugins/spicybackend.bongo-cat
make
omarchy plugin enable spicybackend.bongo-cat
omarchy bar move spicybackend.bongo-cat --section right
```

If `make` is unavailable:

```bash
sudo pacman -S --needed base-devel
```

## Update

```bash
omarchy plugin update spicybackend.bongo-cat --yes
cd ~/.config/omarchy/plugins/spicybackend.bongo-cat
make
omarchy restart shell
```

## Remove

```bash
omarchy plugin remove spicybackend.bongo-cat --yes
```

## License and attribution

Plugin code is MIT-licensed. The cat frame derivation and upstream MIT license are documented in [`THIRD_PARTY_NOTICES`](THIRD_PARTY_NOTICES) and [`LICENSE.externalizable-bongo-cat`](LICENSE.externalizable-bongo-cat).
