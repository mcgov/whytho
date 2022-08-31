Info Gathering Tools

Some commands and scripts to run to get info about memory latency, storage, and CPU info.

usage:
`./RUN_ALL_TESTS.sh`

outputs logs into a zip file into the top level dir `hostname-timestamp`.zip

notes:
- some package install lines only support centos/rhel/oracle
- scripts were written to run on Azure and expect passwordless sudo is enabled in your shell session, might be annoying if you have to type in each time.
- no guarantee package names are correct, packages will be available, etc. You may need to add EPEL etc
- memnuma and memhammer rely on bash hacks to provide arguments. If they don't work and you get them to work, raise a PR!
- installed packages are not cleaned up after installation.
- If running suite in a virtualized environment, your node is the noisy neighbor.
- May leak sensitive information, scripts attempt to not collect keys etc but you may want to double check that no secrets are present before sharing data.
- Will attempt to convert between centos libnuma-devel to ubuntu libnuma-dev form of package names, so use centos version when declaring depedencies
