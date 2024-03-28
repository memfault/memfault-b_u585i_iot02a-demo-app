# Zephyr Example Application

cmake_minimum_required(VERSION 3.20.0)

# The default board for this example is the b_u585i_iot02a, but can be
# overridden by passing --board=<board> to west or -DBOARD=<board> to cmake
set(BOARD b_u585i_iot02a)

find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})

target_sources(app PRIVATE src/main.c src/cdr.c)
zephyr_include_directories(config)
project(memfault-app LANGUAGES C VERSION 1.0.0)
