#!/usr/bin/bash
laptop=eDP-1
if grep -q closed /proc/acpi/button/lid/LID*/state; then
    swaymsg output $laptop disable
else
    swaymsg output $laptop enable
fi
