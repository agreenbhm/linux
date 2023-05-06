Apt repo hosting daily builds of latest upstream Linux kernel supporting the Starfive VisionFive2.  Tested with Ubuntu 23.04.

To use: 

1. Install GPG key (https://raw.githubusercontent.com/agreenbhm/linux/apt/repo/agreenbhm-linux-repo-key.asc) to /etc/apt/trusted.gpg.d/
2. Create a new repo file: /etc/apt/sources.list.d/custom_kernel.list
3. Add the following to custom_kernel.list.  Ensure you include the trailing " /":
   * deb https://agreenbhm.github.io/linux/repo/riscv64 /
4. Run “apt update” to update the local repo cache.
5. To install the kernel, run “apt install linux-image-6.3.0+ linux-headers-6.3.0+”.
6. In the future, you will be able to just do “apt update && apt upgrade” to install a new daily build.

Note: if you are on the v1.2a variant of the board make sure you edit “/etc/flash-kernel/machine” to show the correct board version before updating. Otherwise you will get the wrong DTB installed and you will lose network connectivity. This is true of any kernel update on Ubuntu for this board at the moment. 
