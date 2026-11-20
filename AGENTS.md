# AGENTS.md

Guidance for AI agents (and humans) working in this repository.

## Purpose

LoopHero-Snow is a mod for **Loop Hero** (a GameMaker: Studio game). It adds a cozy
snow effect to the battle and map screens, rendered with the game's built-in
particle system. The mod is a native C++ DLL plugin built against the game's
YYTK plugin SDK. The plugin ID is `sam-k0.loophero.snow`.

Snow intensity is configurable via `options.ini` in the game directory
(section `[Snow]`, key `Intensity`, default `8.0`). The plugin also exposes the
exported APIs `API_LHSnow_SetSnowIntensity` and `API_LHSnow_GetSnowIntensity`
for other plugins.

## Tooling

- **Language:** C++ (no explicit standard set in the project; the code is
  C++14-compatible).
- **Build system:** MSBuild via the Visual Studio solution `LHSnow.sln` /
  project `LHSnow.vcxproj`.
- **Local toolset:** `v145` (Visual Studio 2026 insiders), configured in the
  `.vcxproj`. The shipping target is **Release | x86 (Win32)**; the built
  `LHSnow.dll` is what gets installed into the game.
- **SDK:** The `SDK/` folder contains the YYTK SDK sources and headers
  (compiled into the plugin). It is tracked in git — do not delete or
  reformat it.
- **CI/CD:** GitHub Actions (`.github/workflows/build-and-release.yml`):
  - Every push to any branch: builds `Release x86` on `windows-latest` using
    the `v143` toolset (overridden in the workflow because `v145` is not
    installed on the runner), uploads `LHSnow.dll` as an artifact.
  - Pushes to `main`: additionally creates a GitHub release tagged
    `v.YYYY.MM.DD` (UTC date), appending `-1`, `-2`, ... when multiple
    releases exist for the same day. Release title is `Release YYYY.MM.DD`.
- **Default branch:** `main` (there is no `master`).

## Commit guidelines (Mandatory for agents)

- **NEVER commit or push on your own initiative.** An agent must not run
  `git commit`, `git push`, `git merge`, `git rebase`, or any other command
  that writes to the repository's history or remote, without an explicit,
  unambiguous instruction from the user for that specific action.
- Editing files in the working tree is fine; committing or pushing them is not.
- When the user asks for a change, make the change, show the result, and ask
  before committing or pushing.
- The same rule applies to destructive git operations (reset, rebase,
  force-push) and to creating GitHub releases, tags, PRs, or issues.
