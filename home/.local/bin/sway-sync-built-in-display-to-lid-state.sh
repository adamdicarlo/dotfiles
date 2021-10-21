#!/usr/bin/bash
laptop=eDP-1
if grep -q open /proc/acpi/button/lid/LID/state; then
    swaymsg output $laptop enable
else
    swaymsg output $laptop disable
fi
