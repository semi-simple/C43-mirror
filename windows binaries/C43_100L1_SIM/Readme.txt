24 Dec 2020 release pre-alpha REL100.

This is the pre-alpha version of the C43 modifications made on top of the WP43S prelim alpha emulator. Do not expect it to work. It probably does not.

Current releases, pre-programming:
Rel48 : 2020-12-08. Dead branch. Last stable release of the pre-programming model C43.
Rel48b: 2020-12-18, Small Gitlab code update of branch C43-29-Working only, no binaries released.
Rel49 : Upcoming final release as soon as the shift lock keys on L1 are settled.

Current release, programming model, based on WP43S in progress:
Rel100: 2020-12-24. 


The basic C43 write-up is on the first page of https://forum.swissmicros.com/viewtopic.php?f=2&t=2216. The source code is on https://gitlab.com/Jaymos/wp43c.

If you do test, please submit well structured comments to the above group. With well structured, I mean documented repeatable errant behaviour, preferably listed in a group of errors, and/or comments, all in one (or few) post(s), numbered for reference in follow up posts.

Questions welcomed on the above group.

Praise and compliments to Martin, Pauli and Walter for the work on the base project WP43S. The C43 work done thus far is way less than the effort made by these guys, and the C43 cannot exist without the base code of the WP43S.

---
Notes:

1. Retired the concept of indicating shifts with the dots on the f- and g- lines in favour of the underlining of the selected softkey.

2. Refactored the XEQM code due to previous code simply inefficient, and prevented the C43 code to fit in the DM42 memory space.

3. The setting for triple shift HOME (HOME.3) is enabled by default. Triple shift accesses HOME. Or [g][7] does. Triple shift or EXIT to restores to previous menu.

4. Triple shift selecting HOME can still be disabled using the HOME.3 switch in [MODE]. Triple shift is also influenced by the triple shift timing switch SH.3T. Shift time out is controlled by SH_4s. None of these settings are really end user settings, and are currently test settings.

5. HOME (triple shift) comprises a number of convenient menus and a number of keyboard mirroring menus, which have the purpose to make the C43 operable without a dedicated DM42 template. Arrow [Dn] to access the next screen in a circular fashion.

6. A lot of effort was put into the emulator keyboard control to make it a useful Windows Calc replacement. Welcome to test that. Clipboard control was added, via the usual CTRL-C, and also -H, -D, -A, -X. Keyboard macros were added to be able to type plain text and numbers interchangably without shifts. Test the PC KEYS on the simulator. This menu has various key layouts, including a WP43S layout if you have re-painted your DM42 keys that way.

####################################################################################
7. Be sure to delete the previous backup.bin file prior to using this version. Also, on the DM42 hardware, do a CLR RESET to re-initialize the calculator.
####################################################################################

8. To make a screen shot from the DM42 hardware screen, keep in shift f/g and press DISP. The screen shot will appear in the DM42 flash disk, under /SCREENS/

9. Try SHOW. Up and Dn. With and without menus visible. If there are multiple screens indicated by the ^v arrow on screen, arrows operate the menu. If no or one screen only, then the arrows control the SHOW command and shows all named registers.

10. Try XEQM on long press XEQ, then XEQM. These are softmenu buttons reflecting 18 user RPN program files on the flash disk under /PROGRAMS/. 

11. Try some notable differences form the 43S, i.e.

11a. BASE menu, 
11b. Alpha mode multi line text editor,
11c. Longpress EXIT clears input buffers
11d. Longpress backspace clear alpha buffer
11e. STAT / PLOT. DEMO data is availble. Scroll down using Dn.

---

Jaymos
Jaco Mostert


----------------------ORIGINAL WP43S README COPIED FROM WP43S GITLAB PROJECT---------------------------------------------
Windows 64 bit binaries


This binary is provided for your convenience.


You must install the latest GTK3+ libraries. 

If you do not already have them: download and install the latest release of the gtk3-runtime (not gtk2-runtime) for win64 here https://github.com/tschoonj/GTK-for-Windows-Runtime-Environment-Installer/releases
Install
or re-install the WP43S_StandardFont.ttf font. The WP43S_NumericFont.ttf font isn't needed.


Copy wp43s.exe in a new or pre-existing directory.

Copy dm42l.png, dm42lshort.png and wp43s_pre.css from the root of this gitlab project to the same directory
.
If there is a backup.bin file in your already existing directory: delete it
.

Run wp43s.exe and enjoy.
