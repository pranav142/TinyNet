#!/bin/bash

# Name tap0 is hardcoded
# Should expect to see 42 or 64 bytes on tap0 interface

ping -I tap0 8.8.8.8