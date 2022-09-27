# whytho: Info gathering tools for Linux on Azure

Some commands and scripts to run to get info about memory latency, storage, and CPU info.

**usage:**

Run info gathering only:
`./RUN_INFO_SCRIPTS.sh && ./ZIP_RESULTS.sh`

Run perf tests only:
`./RUN_PERF_BENCHMARKS.sh && ./ZIP_RESULTS.sh`

Run perf and info gathering:
`./RUN_ALL_TESTS.sh`


Outputs are logged and placed into a zip file into the top level dir: `$(hostname)-$(timestamp).zip`

notes:
- Some package dependency names may be different on your system (tested on ubuntu, opensuse and centos7.7)
- Scripts were written to run on Azure and expect passwordless sudo is enabled in your shell session, might be annoying if you have to type in each time.
- No guarantee packages will be available, etc. You may need to add EPEL etc.
- Memnuma and memhammer rely on bash hacks to provide arguments. If they don't work on your system and you get them to work, raise a PR :)
- Installed packages are not cleaned up after installation.
- If running suite in a virtualized environment, your node may be the noisy neighbor.
- May leak sensitive information, scripts attempt to not collect keys etc but you may want to double check that no secrets are present before sharing data.
- Will attempt to convert between centos "libnuma-devel" to ubuntu "libnuma-dev" form for package names.
