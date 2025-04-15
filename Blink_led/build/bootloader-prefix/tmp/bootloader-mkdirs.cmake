# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "C:/Espressif/frameworks/esp-idf-v4.4.7/components/bootloader/subproject"
  "D:/RND/ESP32_ESP-IDF/Blink_led/build/bootloader"
  "D:/RND/ESP32_ESP-IDF/Blink_led/build/bootloader-prefix"
  "D:/RND/ESP32_ESP-IDF/Blink_led/build/bootloader-prefix/tmp"
  "D:/RND/ESP32_ESP-IDF/Blink_led/build/bootloader-prefix/src/bootloader-stamp"
  "D:/RND/ESP32_ESP-IDF/Blink_led/build/bootloader-prefix/src"
  "D:/RND/ESP32_ESP-IDF/Blink_led/build/bootloader-prefix/src/bootloader-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "D:/RND/ESP32_ESP-IDF/Blink_led/build/bootloader-prefix/src/bootloader-stamp/${subDir}")
endforeach()
