# TODO

## Week 1 — Core functionality

- [x] Decide project structure
- [x] Set up C17 build
- [x] Define command-line interface (basic)
- [x] Implement path handling (basic)
- [x] Implement directory traversal
- [x] Read file metadata
- [x] Calculate file sizes
- [x] Calculate directory totals
- [x] Add basic output

**Week 1 milestone:** `dirsize <path>` works reliably on normal directories.

## Week 2 — Reliability

- [x] Add human-readable sizes
- [x] Add depth limiting
- [ ] Add sorting by size
- [x] Decide hidden-file behavior
- [x] Add graceful permission/error handling
- [ ] Handle unusual filesystem entries
- [ ] Add unit tests
- [ ] Add integration tests
- [x] Run AddressSanitizer
- [x] Run UndefinedBehaviorSanitizer
- [ ] Test large directory trees

**Feature freeze:** No new major features after Week 2.

## Week 3 — Release

- [x] Clean up project structure
- [x] Review error handling
- [ ] Test on Linux
- [ ] Test on Windows
- [ ] Add CI
- [ ] Write build instructions
- [ ] Write usage documentation
- [ ] Add examples
- [ ] Add a changelog
- [ ] Prepare release binaries
- [ ] Tag v1.0.0
- [ ] Create GitHub release

## Optional — Only if v1.0 is already complete

- [ ] JSON output
- [ ] Show the largest N entries
- [ ] Additional output formatting

## Definition of Done

The project is considered finished when:

- [x] Core functionality works
- [x] Important error cases are handled
- [ ] Tests pass
- [ ] Sanitizers report no known issues
- [ ] Linux build works
- [ ] Windows build works
- [ ] CI passes
- [ ] README explains how to build and use it
- [ ] v1.0.0 is tagged
- [ ] A GitHub release is published
