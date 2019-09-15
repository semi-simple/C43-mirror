WP43C
15 Sept 2019 release pre-alpha03



This is the pre-alpha version of the WP43S modifications made on top of the WP43S prelim/alpha/early emulator. Do not expect it to work. It does not.

The basic WP43C write-up is on the first page of https://forum.swissmicros.com/viewtopic.php?f=2&t=2216. The source code is on https://gitlab.com/Jaymos/wp43c.

If you do test, please submit well structured comments to the above group. With well structured, I mean documented repeatable errant behaviour, preferably listed in a group of errors, and/or comments, all in one (or few) post(s), numbered for reference in follow up posts.

Questions welcomed on the above group.

Praise and compliments to Martin, Paul and Walter for the work on the base project WP43S.
The WP43C work done thus far is way less than the effort made by these guys, and the WP43C cannot exist without the base code.

---
Notes:

1. Note the dots above the shift key. One tap is for yellow shifted labels. Two taps for blue. Three taps clears the shift and pops up the HOME menu.

2. Triple shift mode (HOME.3) can be cleared in [MODE], [Dn].

3. The setting for triple shift HOME is enabled by default. Triple shift accesses HOME. Or [g][7] does.
   
   Triple shift or EXIT to restores to previous menu.

4. HOME comprises 5 screens at the moment, for testing purposes. Arrow [Dn] to access the next screen in a circular fashion.

5. The latest CSS and PNG file are supplied. These are different from the main proect WP43S.

6. A lot of effort was put into the emulator keyboard control to make it a useful Windows Calc replacement. Welcome to test that.


---

Jaymos
Jaco Mostert
15 Sept 2019


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
