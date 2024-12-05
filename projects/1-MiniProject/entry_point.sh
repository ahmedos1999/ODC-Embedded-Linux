#!/bin/bash

    # Check if xterm is installed
    if command -v xterm &>/dev/null; then
        # Launch xterm with blue background and grey foreground
        xterm -bg blue -fg grey -e "/home/ahmed/ODC-Embedded-Linux/projects/1-MiniProject/loop.sh"
    else
        echo "xterm not found. Please install it or use a compatible terminal."
        exit 1
    fi
