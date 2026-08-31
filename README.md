# Bongo Cat for Omarchy

A keyboard-reactive Bongo Cat and rolling WPM meter for the Omarchy Quickshell bar.

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
- Permission to read keyboard event devices. Add your user to the `input` group, then sign out and back in:

  ```bash
  sudo usermod -aG input "$USER"
  ```

The listener reads keyboard events locally only. It does not log, persist, or transmit keystrokes.

## Install

Clone the plugin, build its local listener, then enable it:

```bash
omarchy plugin add https://github.com/spicybackend/omarchy-bongo-cat-shell-bar.git --yes
cd ~/.config/omarchy/plugins/io.github.spicybackend.bongo-cat
make
omarchy plugin enable io.github.spicybackend.bongo-cat
omarchy bar move io.github.spicybackend.bongo-cat --section right
```

If `make` is unavailable:

```bash
sudo pacman -S --needed base-devel
```

## Controls

| Action | Result |
| --- | --- |
| Left-click | Cycle Bongo Cat + WPM, Bongo Cat, and WPM |
| Hover | Show listener state, current WPM, and display mode |

## Update

```bash
omarchy plugin update io.github.spicybackend.bongo-cat --yes
cd ~/.config/omarchy/plugins/io.github.spicybackend.bongo-cat
make
omarchy restart shell
```

## Remove

```bash
omarchy plugin remove io.github.spicybackend.bongo-cat --yes
```

## License and attribution

Plugin code is MIT-licensed. The cat frame derivation and upstream MIT license are documented in [`THIRD_PARTY_NOTICES`](THIRD_PARTY_NOTICES) and [`LICENSE.externalizable-bongo-cat`](LICENSE.externalizable-bongo-cat).
