# dirsize

A small, cross-platform disk usage analyzer written in C17.

`dirsize` recursively walks a directory tree, calculates the space used by files and directories, and presents the results in a readable form.

## Goals

The goal of this project is to build, test, document, and release a small useful systems program with a strict scope.

This is **not** intended to become a full replacement for tools such as `du`.

## Example

```text
$ dirsize ~/Downloads

18.7 GB  ~/Downloads
7.2 GB   ~/Downloads/Software
4.6 GB   ~/Downloads/Videos
3.8 GB   ~/Downloads/Documents
2.1 GB   ~/Downloads/Archives
1.0 GB   ~/Downloads/Other
```

## Building

Build instructions will be added once the project structure and build system are established.

## Development

See [docs/TODO.md](docs/TODO.md) for the development plan and current checklist.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
