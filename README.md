# dirsize

A small, cross-platform disk usage analyzer written in C17.

`dirsize` recursively walks a directory tree, calculates the space used by files and directories, and presents the results in a readable form.

## Goals

The goal of this project is to build, test, document, and release a small useful systems program with a strict scope.

This is **not** intended to become a full replacement for tools such as `du`.

## Planned features

- [ ] Analyze a directory recursively
- [ ] Calculate file and directory sizes
- [ ] Human-readable size formatting
- [ ] Limit traversal depth
- [ ] Sort results by size
- [ ] Handle inaccessible files and directories gracefully
- [ ] Optional inclusion of hidden files
- [ ] Clear exit codes
- [ ] Automated tests
- [ ] Sanitizer testing
- [ ] CI
- [ ] Linux and Windows support
- [ ] Documentation and usage examples
- [ ] v1.0.0 release

## Example

```text
$ dirsize ~/Projects

12.4 GB  ~/Projects
4.8 GB   ~/Projects/my-lang
3.1 GB   ~/Projects/kilo
2.2 GB   ~/Projects/clipboard-manager
1.1 GB   ~/Projects/other
```

## Building

Build instructions will be added once the project structure and build system are established.

## Development

See [docs/TODO.md](docs/TODO.md) for the development plan and current checklist.

## License

This project is licensed under the MIT License. See [LICENSE](LICENSE).
