![cpu](/assets/cpu.png) **miosb**

hobby operating system version b.

## building

```sh
meson setup build --cross-file cross/clang-aarch64.ini
meson compile -C build
```

