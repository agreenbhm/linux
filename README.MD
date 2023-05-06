Apt repo hosting daily builds of latest upstream Linux kernel supporting the Starfive VisionFive2.  Tested with Ubuntu 23.04.

To use: 

1. Install GPG key (https://raw.githubusercontent.com/agreenbhm/linux/apt/repo/agreenbhm-linux-repo-key.asc) to /etc/apt/trusted.gpg.d/
2. Create a new repo file: /etc/apt/sources/list.d/custom_kernel.list
3. Add the following to custom_kernel.list.  Ensure you include the trailing "/":
   * deb https://agreenbhm.github.io/linux/repo/riscv64 /

