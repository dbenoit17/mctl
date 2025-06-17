# mctl: Minimal C Testing Library

## Usage

Tests are defined in `_MCTL_MODE_TEST` blocks as functions matching the signature:

```void Test\S+(TestingT *t)```

The script `generate_tests.sh *.c` will extract tests via ctags and output a test driver.
Build the driver with the `-D_MCTL_MODE_TEST`.

