# Example Memfault Zephyr `b_u585i_iot02a` application

A minimal (non-networked) reference for Memfault integration.

## Usage

After setting up a zephyr development environment
(https://docs.zephyrproject.org/latest/getting_started/index.html), you can run
the following commands to test the application:

```shell
# set up a zephyr workspace for the project
❯ mkdir memfault-app-workspace
❯ cd memfault-app-workspace
# clone the project
❯ git clone https://github.com/memfault/memfault-b_u585i_iot02a-demo-app.git memfault-app
# initialize the workspace
❯ west init --local memfault-app
# shallow update
❯ west update --narrow -o=--depth=1

# build and run the target program
❯ west build memfault-app
❯ west flash

# then open a serial terminal to interact with the device

*** Booting Zephyr OS build 468eb56cf242 ***
[00:00:00.004,000] <inf> mflt: GNU Build ID: 6557afaa20ace20965d95945e675c0cd51028860
[00:00:00.012,000] <inf> main: 👋 Memfault Demo App! Board b_u585i_iot02a

[00:00:00.020,000] <inf> mflt: S/N: 20353339575250180030001c
[00:00:00.026,000] <inf> mflt: SW type: zephyr-app
[00:00:00.031,000] <inf> mflt: SW version: 0.0.1
[00:00:00.036,000] <inf> mflt: HW version: b_u585i_iot02a

uart:~$
```
