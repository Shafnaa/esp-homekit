# Flashing And Monitoring

This procedure builds and flashes the verification firmware on a Waveshare
ESP32-S3-Zero-M, then opens the native USB serial monitor.

## Prerequisites

- PlatformIO Core (`pio`) installed and available on your `PATH`
- Waveshare ESP32-S3-Zero-M
- USB-C data cable (a charge-only cable will not work)

The board uses native USB CDC, so the serial port is normally available only
after the firmware starts running.

## Clone

Clone the repository and enter its directory:

```sh
git clone https://github.com/Shafnaa/esp-homekit.git
cd esp-homekit
```

## Build

From the repository root, run:

```sh
pio run -e esp32s3-zero-m
```

The build should finish successfully without a warning that the image is
`smaller than the size` of the detected flash. Do not replace the
`waveshare_esp32_s3_zero_m` board with a stock ESP32-S3 board.

## Enter Download Mode

Holding `BOOT` is required each time firmware is downloaded. Use either of
these vendor-supported sequences before uploading:

1. Disconnect the USB cable.
2. Hold the `BOOT` button.
3. Connect the USB cable while continuing to hold `BOOT`.
4. Release `BOOT` after the board is detected.

If the first sequence does not work:

1. Disconnect and reconnect the USB cable.
2. Hold `BOOT`.
3. Press and release `RESET` while holding `BOOT`.
4. Release `BOOT` after the board enters download mode.

## Upload

Run the standard PlatformIO upload target:

```sh
pio run -t upload -e esp32s3-zero-m
```

The command should complete without errors. If upload fails, re-enter
download mode and retry once before investigating the cable, port, or board.

## Monitor

After the firmware starts, open the native USB monitor:

```sh
pio device monitor -e esp32s3-zero-m -b 115200
```

Expected signs of life:

- The onboard WS2812 RGB LED blinks approximately once per second, alternating
  between on and off every 500 ms.
- The monitor displays `[esp-homekit] verification firmware booted`.
- The monitor displays a second line identifying the firmware build date and
  time.

The startup lines are printed once during boot. If the monitor is opened after
boot, those lines may already have been emitted; close and reopen the monitor
after resetting the board if the message is needed.

## Troubleshooting

### No USB port appears

- Confirm that the USB-C cable supports data.
- Disconnect and reconnect the board.
- Re-enter download mode using the `BOOT` sequence above for uploading.
- After uploading, wait for the firmware to start before opening the monitor;
  native USB CDC enumerates when the running firmware initializes it.

### Upload cannot enter download mode

- Hold `BOOT` before connecting USB, rather than pressing it after the cable
  is connected.
- Try the alternate `BOOT` then `RESET` sequence.
- Release `BOOT` only after the board is recognized by PlatformIO.

### Flash-size mismatch warning

Stop if the build or upload reports that the detected flash is smaller than
the image requirement. Verify that the active environment is
`esp32s3-zero-m` and that the board is `waveshare_esp32_s3_zero_m`; do not use
a stock board definition. This warning indicates a configuration problem,
not a normal upload condition.
