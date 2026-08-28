# Git Workflow

Agent reference for the git workflow on this repository. It is a strict 1:1 mirror of
the GitHub **Epic → Story → Task** issue hierarchy. GitHub Issues remain the source of
truth; this document only covers the git side.

> All commands in this document are **git-native**. Pull requests are opened through the
> GitHub web UI or the GitHub MCP server — never the `gh` CLI. Issue-hierarchy resolution
> relies on `gh-issue-sync pull` (documented in `AGENTS.md`); no `gh` commands are added here.

## Invariants

- GitHub Issues are the source of truth. Run `gh-issue-sync pull` after any issue change so
  `.issues/` reflects the current parent/child relationships. Never hand-edit `.issues/`.
- Every branch maps to exactly one issue. The branch's parent branch is the parent issue's
  branch (resolved from the sub-issue hierarchy), not `main` — except Epics, which branch
  from `main`.
- Never push directly to `main`. All changes reach `main` by bubbling up through PRs.
- Never commit `.pio/` build output (already gitignored; do not force-add it).
- Keep branch scope == issue scope. Do not fold unrelated work into a branch.

## Branch hierarchy ↔ issue tiers

| Issue label | Branch tier | Branched from |
| ----------- | ----------- | ------------- |
| `epic`      | `epic-<n>/<slug>`   | `main` |
| `story`     | `story-<n>/<slug>`  | its parent `epic-<n>/<slug>` |
| `task`      | `task-<n>/<slug>`   | its parent `story-<n>/<slug>` |

Merges bubble **up** the same chain, each via a PR:

```
main  ←  epic-<n>  ←  story-<n>  ←  task-<n>
```

## Branch naming

Format: `<epic|story|task>-<n>/<slug>`

- `<n>` — the GitHub **issue number** (not the in-title sequence number).
- `<slug>` — a kebab-case slug derived from the issue **title** (e.g. issue "Epic 3: Control
  LED over BLE" → `control-led-over-ble`). Keep it short and human-readable; drop prefixes
  like `Epic 3:` or `Task 1:`.

Examples (matching current open issues):

- `epic-27/control-led-over-ble`
- `story-29/toggle-led-from-iphone`
- `task-34/implement-command-characteristic`

## Creating a branch

1. Resolve the parent branch:
   - For an `epic`: parent is `main`.
   - For a `story`/`task`: read the parent from `.issues/` (the `Parent:` section of the
     mirrored issue body) after `gh-issue-sync pull`, then use that issue's branch name.
2. Create and track the branch off its parent:

   ```sh
   # task example; replace parent with the resolved story branch
   git switch -c task-34/implement-command-characteristic story-29/toggle-led-from-iphone
   git push -u origin task-34/implement-command-characteristic
   ```

3. If the parent branch does not exist locally yet, fetch it first:

   ```sh
   git fetch origin story-29/toggle-led-from-iphone
   git switch -c task-34/implement-command-characteristic origin/story-29/toggle-led-from-iphone
   git push -u origin task-34/implement-command-characteristic
   ```

## Commit conventions

Use [Conventional Commits](https://www.conventionalcommits.org/). Common types here:
`feat`, `fix`, `chore`, `docs`, `refactor`, `test`. Add a scope for the subsystem
(`ble`, `homekit`, `board`, `build`), and reference the issue number in the footer.

```sh
git commit -m "feat(ble): toggle LED from command characteristic

Implements the GATT command characteristic and wires it to the LED driver.

Refers #34"
```

- Keep commits focused on the single issue the branch belongs to.
- Before opening a PR, rebase onto the latest parent so history stays linear:

  ```sh
  git fetch origin story-29/toggle-led-from-iphone
  git rebase origin/story-29/toggle-led-from-iphone
  git push --force-with-lease
  ```

  Avoid merge commits on feature branches; prefer `rebase` + `--force-with-lease`.

## Opening the PR

Opening a PR is **not** a git operation. Push the branch (above), then open the PR through
the GitHub web UI or the GitHub MCP server, targeting the **parent branch** (task → story,
story → epic, epic → main).

To produce a paste-ready summary for the PR body, git can generate one:

```sh
git request-pull origin/story-29/toggle-led-from-iphone origin task-34/implement-command-characteristic
```

This prints a summary of the commits and the fetch line; paste it into the PR description.
It does not create the PR.

## After a PR is merged

The merge happens on GitHub. Integrate it locally with git, then delete the leaf branch:

```sh
# move to the parent branch and fast-forward it
git switch story-29/toggle-led-from-iphone
git pull --ff-only

# delete the now-merged leaf branch locally and remotely
git branch -d task-34/implement-command-characteristic
git push origin --delete task-34/implement-command-characteristic
```

Repeat the same sequence up the chain:

- After a story PR merges: `git switch epic-27/control-led-over-ble && git pull --ff-only`,
  delete the story branch.
- After an epic PR merges: `git switch main && git pull --ff-only`, delete the epic branch.

Always verify the merge target fast-forwards (`--ff-only`) — if it does not, the parent
received other work; rebase your branch and re-open the PR rather than forcing a merge.

## Keeping history clean

- Feature branches (story/task) should be linear: rebase onto the parent, never merge it.
- Reserve merge commits for the integration points on `main`/epic if the project chooses
  `--no-ff` there; otherwise keep everything fast-forward.
- `git push --force-with-lease` (never plain `--force`) after a rebase.

## Guardrails (do / don't)

- Do branch from the correct parent tier.
- Do name branches `<type>-<n>/<slug>` exactly.
- Do re-sync `.issues/` (`gh-issue-sync pull`) before resolving a parent branch.
- Don't push to `main` directly.
- Don't commit `.pio/`.
- Don't widen a branch's scope beyond its issue.
- Don't use `gh` commands in this workflow — PR creation is web UI / MCP only.
