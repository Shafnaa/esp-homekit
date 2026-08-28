# AGENTS.md

## Project

HomeKit accessory firmware for the Waveshare ESP32-S3-Zero-M (ESP32-S3FH4R2: 4 MB flash,
2 MB QSPI PSRAM) built with PlatformIO (`espressif32` platform, Arduino framework).

The scaffold is in place: `platformio.ini` (env `esp32s3-zero-m`),
`boards/waveshare_esp32_s3_zero_m.json`, and a BLE verification firmware in `src/main.cpp`.
See open tasks in `.issues/open/` before adding features. Canonical build environment name
is `esp32s3-zero-m`.

## Build & Verify

- `pio run` — success means exit 0, `SUCCESS` in output, and zero matches for
  `smaller than the size` (any flash-size mismatch warning = wrong board config, treat as failure).
- Board is NOT natively supported by PlatformIO. Always select board ID
  `waveshare_esp32_s3_zero_m` — the custom JSON in `boards/`; ID is exactly the filename
  minus extension. Never substitute a stock ESP32-S3 board.
- Board JSON must keep `flash_size: 4MB` / `maximum_size: 4194304` pinned. Stock 8 MB
  defaults compile fine but fail to boot ("Detected size(4096k) smaller than ...").
- Serial is native USB CDC only (`ARDUINO_USB_CDC_ON_BOOT=1`, no USB-UART bridge):
  monitor at 115200 baud; the port appears only after firmware starts, and flashing may
  require holding the BOOT button (download mode).
- Validate board JSON changes: `python3 -m json.tool boards/waveshare_esp32_s3_zero_m.json`.
- Never commit `.pio/` build output.

## Environment & OpenCode

- Start OpenCode via `.opencode/run.sh`, not bare `opencode` — it sources `.env.local`
  (gitignored) which provides `GITHUB_PERSONAL_ACCESS_TOKEN` for the GitHub MCP server.
- After any GitHub issue change, run `gh-issue-sync pull` to refresh the local mirror.

## Issues Workflow

- GitHub Issues are the source of truth (resolve `owner/repo` from `git remote get-url origin`;
  currently `Shafnaa/esp-homekit`). NEVER hand-edit files under `.issues/` — they are
  `gh-issue-sync` mirrors.
- Strict Epic -> Story -> Task hierarchy via sub-issues; exactly one label per issue:
  `epic` | `story` | `task`. Follow `.opencode/skills/issue-management/SKILL.md` templates;
  create Tasks only after the user approves the parent Story (the `project-manager`
  agent owns issue creation).

## Git

Full workflow is in `docs/GIT_WORKFLOW.md` (tiered `main → epic → story → task` branches
mapped to the issue hierarchy, git-only commands). Key rules:

- Never push directly to `main`; open PRs that bubble up task → story → epic → main.
- Conventional commit messages (`feat(scope):`, `chore:`, `fix:`) with `Refers #<n>`.

## Further reading

- `docs/FLASHING.md` — build, flash, and native-USB monitor procedure.
- `docs/GIT_WORKFLOW.md` — branch hierarchy and git-only PR flow.
- `.opencode/skills/issue-management/SKILL.md` — Epic/Story/Task issue conventions.
