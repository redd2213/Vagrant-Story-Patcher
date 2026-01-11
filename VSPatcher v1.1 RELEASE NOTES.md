# VSPatcher v1.1 Release Notes

Following the successful launch of v1.0, this update focuses on visual polish, code cleanup, and Quality of Life improvements based on early user feedback. Thank you for using the Patcher in its early versions.

## Visuals & UI

- **Brighter Logo**: Fixed the ASCII logo looking too dim or grey. It is now bold and solid white.

- **Resolution Fix**: Fixed a layout bug where long status text would mess up the window and create unnecessary scrollbars.

- **Window Title**: Updated the app window to correctly show "v1.1".

## Fixes & Cleanup

- **No More Clutter**: Disabled imgui.ini creation. The tool won't leave config files in your game or patch folders anymore.

- **Code Cleanup**: Moved the massive ASCII logo into its own header file to make the source code cleaner and easier to read.