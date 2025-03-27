## Changelog 


### New
- Added back FOV Support
- Added console title randomization
- Added `Performance Mode` which disables non UI rendering, meaning visuals like ESP won't be rendered. Greatly helps with decreasing CPU usage

### Fixes
- Fixed compilation errors caused by late `windows.h` include
- Fixed ESP not being cleared when not holding down the aim key
- Fixed vsync not working
- Fixed crash when no models are in the `models` directory