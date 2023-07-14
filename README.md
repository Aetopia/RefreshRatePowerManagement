# RefreshRatePowerManagement
Dynamically change the screen refresh rate based on system power status.

# Usage.
1. Download the latest release.
2. Run the program.
3. The refresh rate will dynamically change based on the AC adapter status for your primary monitor.
    |AC Adapter Status|Refresh Rate|
    |-|-|
    |Plugged In| Highest available refresh rate.|
    |Not Plugged In| Lowest available refresh rate.|

# Building
1. Install [`GCC`](https://winlibs.com/) & [`UPX`](https://upx.github.io/).
2. Run `build.bat`.