# Contributing

Thank you for your interest in contributing to Powerblock! This document explains how to propose changes and report problems.

There is no mailing list or forum for this project — all discussion happens through GitHub issues and pull requests.

## Reporting bugs

Before opening a new issue, please check whether it has already been reported.

When filing a bug report, please include:

- Hardware revision and firmware build/version (or commit hash) you are using.
- Build configuration (Debug/Release, TMC/CDC mode).
- Steps to reproduce the issue.
- Expected behavior vs. what actually happened.
- Relevant logs, SCPI command traces, or TracerEMB output, if available.

## Suggesting enhancements

Feature requests and design proposals are also welcome as issues. Please describe the use case and, if relevant, how it fits with the existing SCPI command set and hardware constraints.

## Submitting changes

1. Fork the repository and create your branch from `main`.
2. Make sure submodules are initialized (`git submodule update --init --recursive`).
3. Make your changes, following the existing code style in the surrounding files.
4. If you changed peripheral configuration via CubeMX, run `make cube_after_gen` afterwards, as described in the [README](README.md#configuring).
5. Build the project and verify the affected build configurations work on real hardware where possible.
6. Commit your changes with a clear, descriptive commit message explaining *why* the change is needed.
7. Push to your fork and open a pull request against `main`.

In your pull request description, please include:

- What the change does and why it's needed.
- How it was tested (build configuration, hardware used, manual steps).
- Any related issues (e.g. `Fixes #123`).

## Code style

- Match the formatting and naming conventions already used in the file you're editing.
- Keep changes focused; avoid mixing unrelated refactors with functional changes.
- Do not commit generated build artifacts.

## License

By submitting a pull request, you agree that your contribution will be licensed under the project's [Apache License 2.0](LICENSE.md).

## Code of Conduct

This project follows the [Contributor Covenant Code of Conduct](CODE_OF_CONDUCT.MD). By participating, you are expected to uphold it.
