This is a very early version of the WP43S program for DM42. Use it at your own risk.  

From the version of March 19th 2020 21:30 it is also necessary to install the new version of the keymap.bin file.
This changes introduce a new feature: pressing [g] not releasing it and pressing [DISP] releasing [DISP] then releasing [g] creates a screen capture on the USB flash disk in the /SCREENS directory.

Installation instructions:  
Copy the **keymap.bin** file to the DM42 flash disk over USB. This is meant to relocate keys to match the WP43S layout.  
Copy the **WP43S.pgm** file to the DM42 flash disk over USB.  
Flash the **WP43S.pgm** file just like SDKdemo.pgm (https://github.com/swissmicros/SDKdemo)  
From free42: select [shift][SETUP][5][2][4][3] WP43S.pgm [ENTER][ENTER] wait [EXIT][EXIT]  
  
The keyboard layout is also that of WP43S when navigating in the system part of the DM42 (thanks to the keymap.bin file)
  
The Key_stickers.xcf and WP43S_overlay.xcf are GIMP image files to make your life easier!
  
To leave the WP43S program: in the menu [g][MODE][up] select [g][SYSTEM] ([g][F1]) to return to the DMCP system.
  
To retrieve the original DM42 keyboard layout: copy the file **original_DM42_keymap.bin** to the DM42 flash disk, rename it **keymap.bin** and reset the DM42. More informations here: http://www.swissmicros.com/dm42/devel/dmcp_devel_manual/  
  
![Alt text](https://gitlab.com/Over_score/wp43s/uploads/a5baf438f0ee3ba50b72e555bc16f7d5/image.png "DM42")
